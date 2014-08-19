// LANTalkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LANTalk.h"
#include "LANTalkDlg.h"
#include "Mmsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_MY_TRAY_NOTIFICATION WM_USER+0

bool g_bRecv = true;

void RecvProcess(LPVOID pParam)
{
	CLANTalkDlg* pDlg = (CLANTalkDlg*)pParam;
	char buf[1024] = "";
	int fromlength = sizeof(SOCKADDR);

	while(g_bRecv)   //退出时g_bRecv被设置为false，因此子线程也会退出。
	{
		recv(pDlg->m_recvSock,buf,sizeof(buf),0);

		if (!g_bRecv)
		{
			break;
		}
	//保存聊天记录
		CString str = "Recv-";
		str +=	pDlg->m_szAddr;
		str += ": " ;
		str += buf;
		pDlg->m_listLog.AddString(str);
		pDlg->m_listLog.SetCurSel(pDlg->m_listLog.GetCount() - 1);

		//判断当前窗口是否是本窗口
		if(pDlg->IfForeWnd()) 
		{
			pDlg->SetDynamicIcon(false);
		}
		else
		{
			pDlg->SetDynamicIcon(true);
		}
		//播放声音
		sndPlaySound("recv.wav",SND_ASYNC);

		memset(buf,0,sizeof(buf));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLANTalkDlg dialog

CLANTalkDlg::CLANTalkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLANTalkDlg::IDD, pParent),m_trayIcon(IDR_TRAYICON),m_bShutdown(false)
	,m_iIcon(0),m_bDynamicIcon(false)
{
	//{{AFX_DATA_INIT(CLANTalkDlg)
	m_szInputMsg = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MAIN);
	m_hIcon1 = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_hIcon2 = AfxGetApp()->LoadIcon(IDI_ICON2);
	m_hIcon3 = AfxGetApp()->LoadIcon(IDI_ICON3);
	m_hIcon4 = AfxGetApp()->LoadIcon(IDI_ICON4);
	memset(m_szAddr,0,sizeof(m_szAddr));
}

CLANTalkDlg::~CLANTalkDlg()
{
	closesocket(m_sendSock);
	closesocket(m_recvSock);	
}

void CLANTalkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLANTalkDlg)
	DDX_Control(pDX, IDC_IPADDRESS, m_ipAddress);
	DDX_Control(pDX, IDC_LIST_LOG, m_listLog);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_szInputMsg);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLANTalkDlg, CDialog)
	//{{AFX_MSG_MAP(CLANTalkDlg)
	ON_MESSAGE(WM_MY_TRAY_NOTIFICATION, OnTrayNotification)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_COMMAND(ID_APP_SUSPEND, OnAppSuspend)
	ON_COMMAND(ID_APP_OPEN, OnAppOpen)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BTN_IP, OnBtnIp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLANTalkDlg message handlers

BOOL CLANTalkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// 托盘图标初始化
	m_trayIcon.SetNotificationWnd(this, WM_MY_TRAY_NOTIFICATION);
	m_trayIcon.SetIcon(IDI_MAIN);

	//设置Timer
	SetTimer(1,300,NULL);

	//创建socket
	m_sendSock = socket(AF_INET, SOCK_DGRAM,0);	
	if (INVALID_SOCKET == m_sendSock )
	{
		MessageBox("Socket 创建失败","错误",MB_OK);
	}

	m_recvSock = socket(AF_INET, SOCK_DGRAM,0);	
	if (INVALID_SOCKET == m_recvSock )
	{
		MessageBox("Socket 创建失败","错误",MB_OK);
	}

	//得到本机ip
	in_addr localaddr = getLocalhostAddress();

	m_ipAddress.SetAddress(localaddr.S_un.S_un_b.s_b1,
		localaddr.S_un.S_un_b.s_b2,
		localaddr.S_un.S_un_b.s_b3,
		localaddr.S_un.S_un_b.s_b4);
	//设定内部地址值
	OnBtnIp();   

	sockaddr_in m_localAddr ;
	m_localAddr.sin_family = AF_INET;
	m_localAddr.sin_port = htons(3456);
	m_localAddr.sin_addr = localaddr;  

	//绑定接收套接字
	if (SOCKET_ERROR == bind(m_recvSock,(sockaddr*)&m_localAddr,sizeof(m_localAddr)))
	{
		int nError = WSAGetLastError();
		MessageBox("绑定地址错误","Error");
	}

	 //创建接收报文子线程
	 m_pRecvThread = AfxBeginThread((AFX_THREADPROC)RecvProcess,(LPVOID)this);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLANTalkDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLANTalkDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLANTalkDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

LRESULT CLANTalkDlg::OnTrayNotification(WPARAM uID, LPARAM lEvent)
{
	return m_trayIcon.OnTrayNotification(uID, lEvent);
}


void CLANTalkDlg::OnClose() 
{
	if (m_bShutdown)
	{
		g_bRecv = false;
		//发送一个消息给自己      结束子线程中recv的阻塞
		m_szInputMsg = "聊天结束";
		sendto(m_sendSock, m_szInputMsg.GetBuffer(m_szInputMsg.GetLength()),
			m_szInputMsg.GetLength(), 0, reinterpret_cast<sockaddr*>(&m_localAddr),
			sizeof(m_localAddr));

		CDialog::OnClose();
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
}


void CLANTalkDlg::OnAppSuspend() 
{
	m_bShutdown = true;		// really exit
	SendMessage(WM_CLOSE);	
}


void CLANTalkDlg::OnAppOpen() 
{
	ShowWindow(SW_NORMAL);	
	SetForegroundWindow();	
}

void CLANTalkDlg::OnAppAbout() 
{	
	CAboutDlg dlg;
	dlg.DoModal();
}

void CLANTalkDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDynamicIcon)
	{
		switch(m_iIcon) {
		case 1:
			SetIcon(m_hIcon1, TRUE);			// Set big icon
			SetIcon(m_hIcon1, FALSE);		// Set small icon
			m_trayIcon.SetIcon(IDI_ICON1);
			break;
		case 2:
			SetIcon(m_hIcon2, TRUE);			// Set big icon
			SetIcon(m_hIcon2, FALSE);		// Set small icon
			m_trayIcon.SetIcon(IDI_ICON2);
			break;
		case 3:
			SetIcon(m_hIcon3, TRUE);			// Set big icon
			SetIcon(m_hIcon3, FALSE);		// Set small icon
			m_trayIcon.SetIcon(IDI_ICON3);
			break;
		case 4:
			SetIcon(m_hIcon4, TRUE);			// Set big icon
			SetIcon(m_hIcon4, FALSE);		// Set small icon
			m_trayIcon.SetIcon(IDI_ICON4);
			break;
		default:
			SetIcon(m_hIcon1, TRUE);			// Set big icon
			SetIcon(m_hIcon1, FALSE);		// Set small icon
			m_trayIcon.SetIcon(IDI_MAIN);
			break;	
		}
		m_iIcon = (m_iIcon < 4)? (m_iIcon+1):1;
	}
	else
	{
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon
		m_trayIcon.SetIcon(IDI_MAIN);		
	}
	CDialog::OnTimer(nIDEvent);
}

bool CLANTalkDlg::SetDynamicIcon(bool bDynamic)
{
	m_bDynamicIcon = bDynamic;
	return m_bDynamicIcon;
}

void CLANTalkDlg::OnActivate(UINT nState, CWnd *pWndOther, BOOL bMinimized)
{
	if ((nState == WA_ACTIVE ) || (nState == WA_CLICKACTIVE))
	{
		SetDynamicIcon(false);
	}
	else if (nState == WA_INACTIVE)
	{
		//SetDynamicIcon(true);
	}
	CWnd::OnActivate(nState, pWndOther, bMinimized);
}

void CLANTalkDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if (m_szInputMsg.GetLength() == 0)
	{
		MessageBox("不能发送空消息","Error");
	}

	if (m_szInputMsg.GetLength() > 0)  //如果输入的消息不为空，发送
	{
		if(sendto(m_sendSock, m_szInputMsg.GetBuffer(m_szInputMsg.GetLength()),
			m_szInputMsg.GetLength(), 0, reinterpret_cast<sockaddr*>(&m_addrto),
			sizeof(m_addrto)) == SOCKET_ERROR)
			MessageBox("发送报文失败","Error");
	}
	
//保存聊天记录
	CString str = "Send-";
	str +=	m_szAddr;
	str += ": " + m_szInputMsg;
	m_listLog.AddString(str);
	m_listLog.SetCurSel(m_listLog.GetCount() - 1);

	m_szInputMsg="";
	UpdateData(FALSE);
}

bool CLANTalkDlg::IfForeWnd()
{
	HWND hWnd = GetSafeHwnd();
	HWND hTopWnd = ::GetForegroundWindow();
	if (hWnd == hTopWnd)
	{
		return true;
	}
	else
		return false;

}

void CLANTalkDlg::OnCancel() 
{
	if (m_bShutdown)
		CDialog::OnCancel();
	else
	{
		ShowWindow(SW_HIDE);
	}
}

void CLANTalkDlg::OnBtnIp() 
{
	UpdateData(FALSE);
	//从ip地址控件得到地址
	BYTE a0,a1,a2,a3;
	m_ipAddress.GetAddress(a0,a1,a2,a3);

	sprintf(m_szAddr,"%d.%d.%d.%d",a0,a1,a2,a3);

	memset(&m_addrto,0,sizeof(m_addrto));
	m_addrto.sin_family = AF_INET;               
    m_addrto.sin_addr.s_addr = inet_addr(m_szAddr);
	m_addrto.sin_port = htons(3456);

	CString str;
	str.Format("你目前的信息发送对象为：%s",m_szAddr);
	
	MessageBox(str,"注意");
}

 in_addr CLANTalkDlg::getLocalhostAddress()
{
  //////////////////
   // 获得主机名.
   //
   char hostname[256];
   int res = gethostname(hostname, sizeof(hostname));
   if (res != 0) {
      printf("Error: %u\n", WSAGetLastError());
   }
   printf("hostname=%s\n", hostname);
   ////////////////
   // 根据主机名获取主机信息. 
   //
	PHOSTENT hostinfo;
	hostinfo = gethostbyname(hostname);
	in_addr addr = *(struct in_addr *)*hostinfo->h_addr_list;
//	CString ip;
//	ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);

   return addr;
}
