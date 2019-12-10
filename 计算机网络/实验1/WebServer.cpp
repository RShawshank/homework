#include <process.h>
#include <WinSock2.h>
#include <fstream>
#include <sstream>
#include "Webserver.h"
#include <iostream>
#include<WS2tcpip.h>
int WebServer::closeAll = 0;
WebServer::WebServer(char* ip,short port)
{
	try
	{
		StatusCodes = readMap("config/StatusCodes.txt");
		ContentTypes = readMap("config/ContentTypes.txt");
		StatusPages = readMap("config/StatusHtml.txt");
		if (ContentTypes.empty()) throw "Missing contentypes";
		else if (StatusCodes.empty()) throw "Missing StatusCodes";
		else if (StatusPages.empty()) throw "Missing StatusPages";
		//加载协议栈
		WebServer::startWSA();
		
		//创建服务器地址：ip+端口号
		sockaddr_in addr;
		addr.sin_family = AF_INET;//地址族,IPV4
		addr.sin_port = htons(port);//端口号
		inet_pton(AF_INET, ip,(void*)&addr.sin_addr);//IP地址
		memset(addr.sin_zero, 0, 8);//填充sin_zero为0
		//创建一个监听套接字，用于监听用户请求
		Sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (Sock_ == INVALID_SOCKET) throw "INVALID SOCKET";

		//绑定监听套接字和服务器地址
		if (bind(Sock_, (LPSOCKADDR)&addr, sizeof(addr)) != 0)
		{
			closesocket(Sock_);
			WSAGetLastError();
			throw"INVALID SOCKET";
		}
		//监听客户端
		listen(Sock_, SOMAXCONN);
		//接受和处理新的连接
		unsigned ret=0;
		int count = 0;
		while(true)
		{
			Socket* s = this->Accept();
			//开启线程函数为Request，参数为*S的线程，ret为线程Id
		HANDLE handle = (HANDLE)_beginthreadex(0, 0,(unsigned int(__stdcall*)(void*))Request, (void*)s, 0, &ret);
		std::cout << "线程ID："<<ret<<std::endl;
		count++;
			if(count>10)
			{
				break;
			}
			
			if (handle==NULL)
			{
				std::cout << "创建线程失败" << std::endl;
			}
		}
	}
	catch (char* e)
	{
		std::cout << e << "\n";
		system("pause");
	}
}
WebServer::~WebServer()
{
	WebServer::stopWSA();
}
void WebServer::stopWSA()
{
	WSACleanup();//解除与Socket库的绑定并释放Socket库所占用的资源
}

unsigned WebServer::Request(void* ptrSock)
{
	Handler request_handler((reinterpret_cast<Socket*>(ptrSock)));
	return 0;
}
Socket* WebServer::Accept()
{
	struct sockaddr_in ConnAddrs;
	//不进行初始化的话，得到的客户端IP地址是204.204.204.204
	ConnAddrs.sin_port = htons(80);
	ConnAddrs.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	ConnAddrs.sin_family = AF_INET;
	SOCKET newSocket = accept(Sock_, (struct sockaddr*)&ConnAddrs, 0);
	if (newSocket == INVALID_SOCKET) throw"INVALID_SOCKET";
	int port = ntohs(ConnAddrs.sin_port);
	char sendBuf[20] = { '\0' };
	inet_ntop(AF_INET, &(ConnAddrs.sin_port), sendBuf, 16);
	std::cout<<"客户端的地址为：" <<sendBuf <<"  端口号为："<<port<<std::endl;
	Socket* s = new Socket(newSocket);
	return s;
}
void WebServer::startWSA()
{
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2,0),&wsadata)==0)
	{
		if(!(LOBYTE(wsadata.wVersion)>=2))
		{
			throw"VERSION NOT SUPPORTED";
		}
	}
	else
	{
		throw"Startup failed";
	}
}

string_map WebServer::readMap(std::string f)
{
	string_map vars;
	std::string tempKey, tempItem;
	std::ifstream file(f.c_str());
	if (file)
	{
		file >> tempKey;  //读取key值
		getline(file, tempItem, '\n');//读取value
		tempItem = tempItem.substr(1, tempItem.length() - 1);
		while (!file.eof())
		{
			vars.insert(std::pair<std::string, std::string>(tempKey, tempItem));
			file >> tempKey;
			getline(file, tempItem, '\n');
			tempItem = tempItem.substr(1, tempItem.length() - 1);
		}
	}
	return vars;
}



