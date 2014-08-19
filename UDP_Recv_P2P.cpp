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
	if ( err != 0 ) 
	{
		cout<<"Socket2.0初始化失败，Exit!";
		return;
	}	
	if ( LOBYTE( wsaData.wVersion ) != 2 ||	HIBYTE( wsaData.wVersion ) != 0 ) 
	{
		WSACleanup();
		return; 
	}

	//2.创建套接字
	sock = socket(AF_INET,SOCK_DGRAM,0);
	if (sock == INVALID_SOCKET )
	{
		cout<<"Socket 创建失败，Exit!";
		return;
	}

	//3.绑定
	sockaddr_in myaddr; //sockaddr_in相当于sockaddr结构
	memset(&myaddr,0,sizeof(myaddr));
	myaddr.sin_family=AF_INET;
	//如果绑定地址不是本机地址或者ADDR_ANY，则recvfrom函数不会正确接收，而是立刻返回
 	myaddr.sin_addr.s_addr = inet_addr("192.168.0.7");
	//端口号必须和客户发往的端口号一致
	myaddr.sin_port=htons(7861);
 	bind(sock,(sockaddr*)&myaddr,sizeof(myaddr));
	
	int fromlength = sizeof(SOCKADDR);
	char buf[256]="";	
	long number=0;
	while(1){
		number++;
		recv(sock,buf,sizeof(buf),0);
		cout<<number<<":"<<buf<<endl;
		memset(buf,0,sizeof(buf));
	}

	if (!closesocket(sock)) 
	{
		WSAGetLastError();
		return;
	}
	if (!WSACleanup())
	{
		WSAGetLastError();
		return;
	}
}
