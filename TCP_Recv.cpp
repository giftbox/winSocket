#include <winsock2.h>
#include <iostream.h>
#include <stdlib.h>

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
	sock = socket(AF_INET,SOCK_STREAM,0);
	if (sock == INVALID_SOCKET ) {
		cout<<"Socket 创建失败，Exit!";
		return;
	}

	//3.定义地址
	sockaddr_in myaddr; 
	memset(&myaddr,0,sizeof(myaddr));
	myaddr.sin_family=AF_INET;
	
	myaddr.sin_addr.s_addr = inet_addr("192.168.0.9");  
	//端口号必须和客户发往的端口号一致
	myaddr.sin_port=htons(7861);

	//4.connect
	if (connect(sock,(sockaddr*)&myaddr,sizeof(myaddr)) == SOCKET_ERROR)
	{
		cout<<"connect failed, exit!"<<endl;
		closesocket(sock);
		WSACleanup();
		exit(1);
	}
	else
	{
		cout<<"connect succeed!"<<endl;
	}
	
	char buf[256]="";	
	long number=0;
	int iResult = 0;
	while(true)
	{
		number++;
		iResult = recv(sock,buf,sizeof(buf),0);
		if (iResult < 0)
		{
			cout<<"服务方Socket关闭，退出！"<<endl;
			break;
		}
		Sleep(1000);
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
