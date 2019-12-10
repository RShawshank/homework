#include <WinSock2.h>
#include <string>
#include "Webserver.h"
#include <iostream>
using namespace std;
#define CSOCKET_VERSION  2

//接收和发送
Socket::Socket(SOCKET s)
{
	sock_ = s;
}
Socket::~Socket()
{
	close();
}
void Socket::close()
{
	closesocket(sock_);
}
/*
//接收数据,
//recv():在已建立连接的套接字上接受数据
std::string Socket::rxData()
{
	char buffer[1024];
	int retval=0;//接受的比特数
	while (true)
	{
		retval = recv(sock_, buffer, sizeof(buffer) - 1, 0);
		if (retval==0)
		{
			break;
		}
		else if (retval==SOCKET_ERROR)
		{
			throw"socket error while receiving";
		}
		else
		{
			buffer[retval] = 0;//结尾为0
			return buffer;
		}
	}
	return "";
}
*/
std::string Socket::rxLine()
{
	string result;
	char r;
	int rStatus;
	while (true)
	{
		rStatus = recv(sock_, &r, 1, 0);
		if (rStatus == 0)
		{
			return result;
		}
		else if (rStatus == SOCKET_ERROR)
			return "";
		else
		{
			result += r;
		}
		if (r=='\n')
		{
			return result;
		}
	}
	return result;
}
//send():在已建立连接的套接字上发送数据，回应数据
void Socket::txData(char* data, int size)
{
	send(sock_, data, size, 0);
	std:cout<<data<<std::endl;
}
void Socket::txLine(std::string line)
{
	line += '\n';
	send(sock_, line.c_str(), line.length(), 0);
	std::cout << line << std::endl;

}






