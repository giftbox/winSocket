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
	if (0 != err) 
	{
		cout<<"Socket2.0��ʼ��ʧ�ܣ�Exit!";
		return;
	}	
	if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 0 ) 
	{
		WSACleanup();
		return; 
	}
	
	//2.�����׽���
	sock = socket(AF_INET,SOCK_DGRAM,0);
	if (INVALID_SOCKET == sock) 
	{
		cout<<"Socket ����ʧ�ܣ�Exit!";
		return;
	}

	//3.���ø��׽���Ϊ�㲥���ͣ�
	bool opt=true;
	setsockopt(sock,SOL_SOCKET,	SO_BROADCAST,(char FAR *)&opt,sizeof(opt));
	
	//4.������ַ
	sockaddr_in from;
	memset(&from,0,sizeof(from));
	from.sin_family=AF_INET;
	//����󶨵�ַ���Ǳ�����ַ����ADDR_ANY����recvfrom����������ȷ���գ��������̷���
	// from.sin_addr.s_addr = ADDR_ANY;  
	from.sin_addr.s_addr = inet_addr("192.168.0.7");
	//�˿ںű���Ϳͻ������Ķ˿ں�һ��
	from.sin_port=htons(7861);
	
	//5.�󶨽��յ�ַ
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
