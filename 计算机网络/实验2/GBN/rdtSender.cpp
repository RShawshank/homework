#include "stdafx.h"
#include "Global.h"
#include "GBNRdtSender.h"
#include <fstream>
ofstream Senderlog("senderlog.txt");
GBNRdtSender::GBNRdtSender() :seqRange(8), windowLen(4), baseSeq(0), NextSeqNum(0)
{
	//初始化窗口
	window = new deque<Packet>;
}
GBNRdtSender::~GBNRdtSender()
{
	delete window;
}
//返回RdtSender是否处于等待状态，如果发送方正等待确认或者发送窗口已满，返回true
bool GBNRdtSender::getWaitingState()
{
	return(( (window->size()) == windowLen)? true : false);
}
//发送应用层下来的Message，由NetworkService调用,如果发送方成功地将Message发送到网络层，返回true;如果因为发送方处于等待确认状态或发送窗口已满而拒绝发送Message，则返回false
bool GBNRdtSender::send(Message& message) 
{
	if (this->getWaitingState())//发送方正等待确认或者发送窗口已满
		return false;
	Packet NewPacket;
	NewPacket.acknum = -1;//确认号，不管
	NewPacket.seqnum = this->NextSeqNum;//序号
	memcpy(NewPacket.payload, message.data,sizeof(message.data));
	NewPacket.checksum = pUtils->calculateCheckSum(NewPacket);//校验和
	window->push_back(NewPacket);//将新的包加入窗口
	Senderlog<<"\n"<<"发送方发送数据报文，序号为："<< NewPacket.seqnum<<endl;
	if (this->baseSeq== this->NextSeqNum)//首次发
	{
		pns->startTimer(SENDER, Configuration::TIME_OUT, NewPacket.seqnum);//发送方启动定时器
	}
	pns->sendToNetworkLayer(RECEIVER, NewPacket);
	(this->NextSeqNum)++;
	this->NextSeqNum %= this->seqRange;
	return true;
}
//接受确认Ack，将被NetworkService调用	
void GBNRdtSender::receive(Packet &ACKPKT)
{
	int checksum = pUtils->calculateCheckSum(ACKPKT);//计算校验和
	//检验和相等并且序号不是之前理应收到的ACK序号包
	if (checksum == ACKPKT.checksum && ACKPKT.acknum != (this->baseSeq + 7) % 8)
	{
		pns->stopTimer(SENDER, baseSeq);//发送方停止计时器
		Senderlog << "发送方收到ACK为：" << ACKPKT.acknum << endl;
		Senderlog << "窗口的首序号为：" << baseSeq;
		Senderlog << "      窗口的下一个待发送包的序号为：" << NextSeqNum;
		Senderlog << "      窗口的大小为：" << window->size() << endl;
		while (baseSeq != (ACKPKT.acknum + 1) % seqRange)
		{
			window->pop_front();
			baseSeq++;
			baseSeq %= seqRange;
		}
		Senderlog << "发送方收到ACK为：" << ACKPKT.acknum << endl;
		Senderlog << "窗口后的首序号为：" << baseSeq ;
		Senderlog << "       窗口的下一个待发送包的序号为：" << NextSeqNum;
		Senderlog << "       窗口的大小为：" << window->size() << endl;
		//当包没有发送完时
		if (window->size())
		{//重新以第一个包开始计时
			pns->startTimer(SENDER, Configuration::TIME_OUT, window->front().seqnum);
		}
	}
}
//Timeout handler，将被NetworkService调用
void GBNRdtSender::timeoutHandler(int seqnum)
{
	Senderlog << "超时!现在重新发送窗口内的数据报文，";
	Senderlog << "此时的窗口的首序号为：" << baseSeq << ",下一个待发送包的序号为：" << NextSeqNum << ",窗口大小：" << window->size() << endl;
	pns->stopTimer(SENDER, seqnum);
	for (deque<Packet>::iterator iterator = window->begin(); iterator != window->end(); iterator++)
		pns->sendToNetworkLayer(RECEIVER, *iterator);//重新发送数据包
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqnum);
}
