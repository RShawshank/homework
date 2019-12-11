#include"RdtReceiver.h"
#include<deque>
using namespace std;
struct receiverPKT
{
	Message message;
	bool acpt;
};
class SRRdtReceiver :public RdtReceiver
{
private:
	int windowLen;
	int baseSeq;
	Packet ACKPKT;
	deque<receiverPKT>* window;
public:
	void receive(Packet&);//接收报文，将被NetworkService调用	
public:
	SRRdtReceiver();
	~SRRdtReceiver();
};
