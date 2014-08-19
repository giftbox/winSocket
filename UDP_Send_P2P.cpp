#include <winsock2.h>
#include <iostream.h>

void main()
{
	SOCKET sock;                   //socket
	char szMsg[] = "this is a UDP test package";//�����͵��ֶ�
	
	//1.����SOCKET�⣬�汾Ϊ2.0
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;	
	wVersionRequested = MAKEWORD( 2, 0 );	
	err = WSAStartup(wVersionRequested, &wsaData );
	if ( err != 0 ) 
	{
		cout<<"Socket2.0��ʼ��ʧ�ܣ�Exit!";
		return;
	}	
	if ( LOBYTE( wsaData.wVersion ) != 2 ||	HIBYTE( wsaData.wVersion ) != 0 ) 
	{
		WSACleanup( );
		return; 
	}
	
	//2.����socket��
	sock = socket(
		AF_INET,           //internetwork: UDP, TCP, etc
		SOCK_DGRAM,        //SOCK_DGRAM˵����UDP����
		0                  //protocol
		);
	
	if (sock == INVALID_SOCKET ) {
		cout<<"Socket ����ʧ�ܣ�Exit!";
		return;
	}
	
	//3.���÷����ĵ�ַ
	sockaddr_in addrto;            //�����ĵ�ַ	
	memset(&addrto,0,sizeof(addrto));
	addrto.sin_family=AF_INET;
	//��127��ͷ��ip�����ҿͻ��ͷ�������ͬһ�������������񷽶����Խ���(�൱�ڹ㲥)��
	//ָ������ip�Ŀ��Խ���
    addrto.sin_addr.s_addr=inet_addr("192.168.0.7");
	//�˿ںű���ͷ������󶨵Ķ˿ں�һ��
	addrto.sin_port=htons(7861);
	
	int nlen=sizeof(addrto);
	unsigned int uIndex = 1;
	while(true)
	{
		Sleep(1000);
		//�ӹ㲥��ַ������Ϣ
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