#ifndef WIN32_LEAN_AND_MEAN//精简头文件
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef WEBSERVER_H
#define WEBSERVER_H
#pragma comment(lib, "Ws2_32.lib")
#include <string>
#include <map>
#include <WinSock2.h>
typedef std::map<std::string, std::string> string_map;
class Socket;
class Handler;
class WebServer
{
private:
	static string_map ContentTypes;     //网页类型
	static string_map StatusCodes;      //网页状态码
	static string_map StatusPages;     //HTML code
	SOCKET Sock_;
	static int closeAll;//关闭服务器
	friend class Socket;              
	friend class Handler;

	static unsigned   __stdcall   Request(void* ptrSock); //创建一个请求线程
	Socket* Accept();           //接受一个新的socket          
	string_map                  readMap(std::string f); 
	static void                 startWSA();             
	static void                 stopWSA();             
public:
	WebServer(char* ip,short port);
	~WebServer();
	void setcloseAll()
	{
		closeAll = 0;
	}
};
//套接字类
class Socket
{
private:
	SOCKET sock_;
public:
	Socket(SOCKET s);
	~Socket();

	void close();  //关闭连接
	std::string  rxData(); //接收数据
	std::string  rxLine();//接收字符串
	void txData(char* data, int size);
	void txLine(std::string line);
};
class Handler
{
private:
	struct Page
	{
		Socket* sock_;
		std::string path;  //request的path
		std::string status;//OK||NO found
		std::string contentTpye;
		struct Data
		{
			int size;
			char* content;
		}data;
	}page;
	std::string parsePath(std::string l);
	std::string parseContentType(std::string p);
	void constData(std::string s);
	void readData(std::string f);
	void createPage(std::string l);
	void parseData(std::string d);
	void sendPage();
	void setPage404(Page::Data& p);
public:
	Handler(Socket* s);
	~Handler();
	
};
#endif