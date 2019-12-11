#include "stdafx.h"
#include "Global.h"
#include "GBNRdtReceiver.h"
#include<fstream>
ofstream Receiverlog("Receiverlog.txt");
GBNRdtReceiver::GBNRdtReceiver() :WantSeqNum(0)
{
	lastACKPKT.acknum = (WantSeqNum + 7) % 8;//前一个序号表示收到的ACK,8是窗口大小
	lastACKPKT.checksum = pUtils->calculateCheckSum(lastACKPKT);
	lastACKPKT.seqnum = -1;
}
GBNRdtReceiver::~GBNRdtReceiver()
{
}
void GBNRdtReceiver::receive(Packet& packet)
{
	//检查校验和
	int checksum = pUtils->calculateCheckSum(packet);
	//检验和没问题同时接收方希望收到的序号是该序号
	if (checksum == packet.checksum && this->WantSeqNum == packet.seqnum)
	{
		Message message;
		memcpy(message.data, packet.payload, sizeof(packet.payload));
		pns->delivertoAppLayer(RECEIVER, message);//传递给应用层
		lastACKPKT.acknum = packet.seqnum;//将确认序号设置为数据包序号
		lastACKPKT.checksum = pUtils->calculateCheckSum(lastACKPKT);
		Receiverlog << "接收方发送的ACK序号为：" << lastACKPKT.acknum << endl;
		pns->sendToNetworkLayer(SENDER, lastACKPKT);
		this->WantSeqNum++;
		this->WantSeqNum %= 8;
	}
	else
	{
		//检验和不对
		if (checksum != packet.checksum)
		{
			Receiverlog << "接收方收到的报文的检验和错误" << endl;
		}
		//报文失序
		else
		{
			Receiverlog << "接收方收到的报文是失序报文，其序号为：" << packet.seqnum << endl;
		}
		//发送最近收到的ACK给发送方
		Receiverlog << "接收方发送最近收到的ACK给发送方，ACK为：" << lastACKPKT.acknum << endl;
		pns->sendToNetworkLayer(SENDER, lastACKPKT);
	}
}