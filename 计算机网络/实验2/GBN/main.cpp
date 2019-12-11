#include"Global.h"
#include"GBNRdtReceiver.h"
#include"GBNRdtSender.h"
#include"stdafx.h"
#include "RdtReceiver.h"
#include "RdtSender.h"
int main(int argc, char** argv[])
{
	RdtSender *sender = new GBNRdtSender();
	RdtReceiver *receiver = new GBNRdtReceiver();
	pns->init();
	pns->setRtdSender(sender);
	pns->setRtdReceiver(receiver);
	pns->setInputFile("input.txt");
	pns->setOutputFile("output.txt");
	pns->start();
	delete sender;
	delete receiver;
	delete pUtils;
	delete pns;
	return 0;
}