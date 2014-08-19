#include <stdlib.h>
#include <winsock2.h>
#include <iostream.h>

void main()
{
	SOCKET sock, clientSocket;                   //socket
	char szMsg[] = "this is a UDP test package";//被发送的字段
	
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
		WSACleanup( );
		return; 
	}
	
	//2.创建socket，
	sock = socket(
		AF_INET,           //internetwork: UDP, TCP, etc
		SOCK_STREAM,        //SOCK_STREAM说明是TCP socket
		0                  //protocol
		);
	
	if (sock == INVALID_SOCKET ) 
	{
		cout<<"Socket 创建失败，Exit!";
		return;
	}
	
	//3.bind套接字，相当于给本地套接字赋值
	sockaddr_in myaddr; 
	memset(&myaddr,0,sizeof(myaddr));
	myaddr.sin_family=AF_INET;

	myaddr.sin_addr.s_addr = inet_addr("192.168.0.7");
	myaddr.sin_port=htons(7861);
	bind(sock,(sockaddr*)&myaddr,sizeof(myaddr));

	//4.设置发往的地址
	sockaddr_in addrto;            //发往的地址	
	memset(&addrto,0,sizeof(addrto));
	addrto.sin_family=AF_INET;
    addrto.sin_addr.s_addr=inet_addr("192.168.0.9");
	//端口号必须和服务器绑定的端口号一致
	addrto.sin_port=htons(7861);
	
	//5.listen 另一端的socket
	if (listen(sock,5) == SOCKET_ERROR )
	{
		closesocket(sock);
		WSACleanup();
		abort();
	}
	else
	{
		cout<<"listen succeed!"<<endl;
	}
	
	//6. accept 对方连接
	int nlen = sizeof(addrto);
	clientSocket = accept(sock,(sockaddr*)&addrto,&nlen);
	if (clientSocket < 0)
	{
		cout<<"client socket error, exit!"<<endl;
		abort();
	}
	else
	{
		cout<<"accepted client socket!"<<endl;
	}
	
	unsigned int uIndex = 1;
	while(true)
	{
		Sleep(1000);

		if( send(clientSocket, szMsg, strlen(szMsg), 0)	== SOCKET_ERROR )
			cout<<"error, error id = "<<WSAGetLastError()<<endl;
		else
			cout<<uIndex++<<":an UDP package is sended."<<endl;
	}
	
	if (!closesocket(sock)) 
	{
		WSAGetLastError();
		return;
	}
	if (!closesocket(clientSocket)) 
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