#include <winsock2.h>
#include <iostream.h>

void main(void)
{
	SOCKET sock;
	
	//1.����SOCKET�⣬�汾Ϊ2.0
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;	
	wVersionRequested = MAKEWORD( 2, 0 );	
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) 
	{
		cout<<"Socket2.0��ʼ��ʧ�ܣ�Exit!";
		return;
	}	
	if ( LOBYTE( wsaData.wVersion ) != 2 ||	HIBYTE( wsaData.wVersion ) != 0 ) 
	{
		WSACleanup();
		return; 
	}

	//2.�����׽���
	sock = socket(AF_INET,SOCK_DGRAM,0);
	if (sock == INVALID_SOCKET )
	{
		cout<<"Socket ����ʧ�ܣ�Exit!";
		return;
	}

	//3.��
	sockaddr_in myaddr; //sockaddr_in�൱��sockaddr�ṹ
	memset(&myaddr,0,sizeof(myaddr));
	myaddr.sin_family=AF_INET;
	//����󶨵�ַ���Ǳ�����ַ����ADDR_ANY����recvfrom����������ȷ���գ��������̷���
 	myaddr.sin_addr.s_addr = inet_addr("192.168.0.7");
	//�˿ںű���Ϳͻ������Ķ˿ں�һ��
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
