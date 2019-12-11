#include "stdafx.h"
#include "Global.h"
#include "SRRdtReceiver.h"
#include<fstream>
ofstream Receiverlog("SRReceiverlog.txt");
SRRdtReceiver::SRRdtReceiver() : windowLen(4), baseSeq(0)
{
	ACKPKT.acknum = 0;
	ACKPKT.checksum = pUtils->calculateCheckSum(ACKPKT);
	ACKPKT.seqnum = -1;
	//初始化窗口,并始终保持窗口中有4个位置
	window = new deque<receiverPKT>;
	for (int i = 0; i < windowLen; i++)
	{
		receiverPKT blankPKT;
		blankPKT.acpt = false;
		window->push_back(blankPKT);
	}
}
SRRdtReceiver::~SRRdtReceiver()
{
	delete window;
}
void SRRdtReceiver::receive(Packet& packet)
{
	//检查校验和
	int checksum = pUtils->calculateCheckSum(packet);
	int offset = (packet.seqnum-baseSeq+8)%8;
	//检验和没问题
	if (checksum == packet.checksum)
	{
		Receiverlog<<"接受方接收到的报文的序号为："<<packet.seqnum<<endl;
		if (offset < windowLen)
		{
			window->at(offset).acpt = true;//收到序号并且正确
		   //窗口缓存
			memcpy(window->at(offset).message.data, packet.payload, sizeof(packet.payload));
		}
		//发送ACK给发送方
		ACKPKT.acknum = packet.seqnum;
		ACKPKT.checksum = pUtils->calculateCheckSum(ACKPKT);
		Receiverlog << "接收方发送的ACK序号为：" << ACKPKT.acknum << endl;
		pns->sendToNetworkLayer(SENDER, ACKPKT);

		//当窗口的最左边的报文已经确认收到了，移动窗口并且提交报文
		while (window->begin()->acpt == true)
		{
			Message message;
			memcpy(message.data, window->begin()->message.data, sizeof(window->begin()->message.data));
			pns->delivertoAppLayer(RECEIVER, message);
			//Receiverlog << message.data <<endl;
			window->pop_front();
			receiverPKT blankPKT;
			blankPKT.acpt = false;
			window->push_back(blankPKT);//补充窗口
			baseSeq++;
			baseSeq %= 8;
		}
	}
	else
			Receiverlog << "接收方收到的报文的检验和错误，其序号为：" << packet.seqnum << endl;	
}