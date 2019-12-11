#include"SRRdtSender.h"
#include"Global.h"
#include"stdafx.h"
#include<fstream>
ofstream Senderlog("srsenderlog.txt");
SRRdtSender::SRRdtSender() :seqRange(8), windowLen(4), NextSeqNum(0), baseSeq(0)
{
	window = new deque<sendPKT>;
}
SRRdtSender::~SRRdtSender()
{
	delete window;
}
bool SRRdtSender::getWaitingState()
{
	return(((window->size()) == windowLen) ? true : false);
}
bool SRRdtSender::send(Message& message)
{
	if (this->getWaitingState())//发送方正等待确认或者发送窗口已满
		return false;
	sendPKT NewPacket;
	NewPacket.acpt = false;
	NewPacket.packet.acknum = -1;//确认号，不管
	NewPacket.packet.seqnum = this->NextSeqNum;//序号
	memcpy(NewPacket.packet.payload, message.data, sizeof(message.data));
	//Senderlog << message.data << endl;
	NewPacket.packet.checksum = pUtils->calculateCheckSum(NewPacket.packet);//校验和
	window->push_back(NewPacket);//将新的包加入窗口
	Senderlog << "\n" << "发送方发送数据报文，序号为：" << NewPacket.packet.seqnum << endl;
		pns->startTimer(SENDER, Configuration::TIME_OUT, NewPacket.packet.seqnum);//发送方启动定时器
	pns->sendToNetworkLayer(RECEIVER, NewPacket.packet);
	(this->NextSeqNum)++;
	this->NextSeqNum %= this->seqRange;
	return true;
}
void SRRdtSender::receive(Packet& ACKPKT)
{
	int checksum = pUtils->calculateCheckSum(ACKPKT);//计算校验和
	int offset = (ACKPKT.acknum - baseSeq+seqRange) % seqRange;//此时ack序号与首序号的差值
	//检验和相等并且只收在窗口内的ACK
	if (checksum == ACKPKT.checksum && offset < window->size() && window->at(offset).acpt==false )
	{
		pns->stopTimer(SENDER, ACKPKT.acknum);//发送方停止计时器
		window->at(offset).acpt = true;//标识该序号已经被收到
		Senderlog << "发送方收到ACK为：" << ACKPKT.acknum << endl;
		Senderlog << "窗口的首序号为：" << baseSeq;
		Senderlog << "窗口的下一个待发送包的序号为：" << NextSeqNum;
		Senderlog << "窗口的大小为：" << window->size() << endl;
		//当收到的包为窗口最左边的包时，窗口移动
		while (window->size()&&window->begin()->acpt==true)
		{
			baseSeq++;
			baseSeq %= seqRange;
			window->pop_front();
		}
		Senderlog << "窗口的首序号为：" << baseSeq;
		Senderlog << "窗口的下一个待发送包的序号为：" << NextSeqNum;
		Senderlog << "窗口的大小为：" << window->size() << endl;
	}
}
void SRRdtSender::timeoutHandler(int seqnum)
{
	//Senderlog << seqnum << endl;
	pns->stopTimer(SENDER, seqnum);
	Senderlog << "超时!现在重新发送窗口内的数据报文，";
	Senderlog << "此时的窗口的首序号为：" << baseSeq << ",下一个待发送包的序号为：" << NextSeqNum << ",窗口大小：" << window->size() << endl;
	int offset = (seqnum - baseSeq+seqRange) % seqRange;
	//当超时的包在窗口内，就重新发包
	if (offset< window->size())
	{
		pns->sendToNetworkLayer(RECEIVER, window->at(offset).packet);
		pns->startTimer(SENDER, Configuration::TIME_OUT, seqnum);
	}
}