#include"RdtSender.h"
#include<deque>
using namespace std;
class GBNRdtSender :public RdtSender
{
private:
	int seqRange;//序号范围
	int windowLen;//窗口长度
	int baseSeq;//首序号
	int NextSeqNum;//下个等待发送包的序号
	int DuplicateACK;//冗余的ack，实现快速重传
	deque<Packet>* window;//队列实现GBN中的窗口
public:
	bool getWaitingState();
	bool send(Message&);
	void receive(Packet&);
	void timeoutHandler(int);
public:
	GBNRdtSender();
	virtual ~GBNRdtSender();
};