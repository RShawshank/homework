#include "RdtReceiver.h"
class GBNRdtReceiver:public RdtReceiver
{
private:
	int WantSeqNum;//期待的序号
	Packet lastACKPKT;//最近收到的ACK	
public:
	void receive(Packet&);//接收报文，将被NetworkService调用	
public:
	GBNRdtReceiver();
	virtual ~GBNRdtReceiver();
};
