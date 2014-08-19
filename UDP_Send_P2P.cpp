#include <winsock2.h>
#include <iostream.h>

void main()
{
	SOCKET sock;                   //socket
	char szMsg[] = "this is a UDP test package";//被发送的字段
	
	//1.启动SOCKET库，版本为2.0
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;	
	wVersionRequested = MAKEWORD( 2, 0 );	
	err = WSAStartup(wVersionRequested, &wsaData );
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
		SOCK_DGRAM,        //SOCK_DGRAM说明是UDP类型
		0                  //protocol
		);
	
	if (sock == INVALID_SOCKET ) {
		cout<<"Socket 创建失败，Exit!";
		return;
	}
	
	//3.设置发往的地址
	sockaddr_in addrto;            //发往的地址	
	memset(&addrto,0,sizeof(addrto));
	addrto.sin_family=AF_INET;
	//以127开头的ip，并且客户和服务器在同一个局域网，服务方都可以接收(相当于广播)；
	//指定服务方ip的可以接收
    addrto.sin_addr.s_addr=inet_addr("192.168.0.7");
	//端口号必须和服务器绑定的端口号一致
	addrto.sin_port=htons(7861);
	
	int nlen=sizeof(addrto);
	unsigned int uIndex = 1;
	while(true)
	{
		Sleep(1000);
		//从广播地址发送消息
		if( sendto(sock,szMsg,strlen(szMsg),0,(sockaddr*)&addrto,nlen)
			== SOCKET_ERROR )
			cout<<WSAGetLastError()<<endl;
		else
			cout<<uIndex++<<":an UDP package is sended."<<endl;
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