#include "RdtSender.h"
#include<deque>
using namespace std;
struct sendPKT
{
	Packet packet;
	bool acpt;//标识是否收到，用于缓存
};
class SRRdtSender:public RdtSender
{
private:
	int seqRange;//序号范围
	int windowLen;//窗口长度
	int baseSeq;//首序号
	int NextSeqNum;//下一个等待发送包的序号
	deque<sendPKT>* window;
public:
	bool getWaitingState();
	bool send(Message&);
	void receive(Packet&);
	void timeoutHandler(int);
public:
	SRRdtSender();
	~SRRdtSender();
};