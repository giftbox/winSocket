#include <winsock2.h>
#include <iostream.h>

void main(void)
{
	SOCKET sock;
	
	//1.启动SOCKET库，版本为2.0
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;	
	wVersionRequested = MAKEWORD( 2, 0 );	
	err = WSAStartup( wVersionRequested, &wsaData );
	if (0 != err) 
	{
		cout<<"Socket2.0初始化失败，Exit!";
		return;
	}	
	if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 0 ) 
	{
		WSACleanup();
		return; 
	}
	
	//2.创建套接字
	sock = socket(AF_INET,SOCK_DGRAM,0);
	if (INVALID_SOCKET == sock) 
	{
		cout<<"Socket 创建失败，Exit!";
		return;
	}

	//3.设置该套接字为广播类型，
	bool opt=true;
	setsockopt(sock,SOL_SOCKET,	SO_BROADCAST,(char FAR *)&opt,sizeof(opt));
	
	//4.创建地址
	sockaddr_in from;
	memset(&from,0,sizeof(from));
	from.sin_family=AF_INET;
	//如果绑定地址不是本机地址或者ADDR_ANY，则recvfrom函数不会正确接收，而是立刻返回
	// from.sin_addr.s_addr = ADDR_ANY;  
	from.sin_addr.s_addr = inet_addr("192.168.0.7");
	//端口号必须和客户发往的端口号一致
	from.sin_port=htons(7861);
	
	//5.绑定接收地址
 	bind(sock,(sockaddr*)&from,sizeof(from));
	
	memset(&from,0,sizeof(from));
	int fromlength = sizeof(SOCKADDR);
	char buf[256];	
	memset(buf,0,sizeof(buf));
	long number = 0;
	while(true){
		number++;
	// 	recvfrom(sock,buf,256,0,(struct sockaddr FAR *)&from,(int FAR *)&fromlength);
 		recv(sock,buf,256,0);
	 	Sleep(1000);
		cout<<number<<":"<<buf<<endl;
		memset(buf,0,sizeof(buf));
	}
	
	if (!closesocket(sock)) {
		WSAGetLastError();
		return;
	}
	if (!WSACleanup()) {
		WSAGetLastError();
		return;
	}
}
