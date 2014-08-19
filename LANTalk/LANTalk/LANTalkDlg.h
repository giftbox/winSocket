// LANTalkDlg.h : header file
//

#if !defined(AFX_LANTALKDLG_H__C3E90F31_C21F_4B28_A222_BF560454697F__INCLUDED_)
#define AFX_LANTALKDLG_H__C3E90F31_C21F_4B28_A222_BF560454697F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "trayicon.h"

void RecvProcess(LPVOID pParam);
/////////////////////////////////////////////////////////////////////////////
// CLANTalkDlg dialog

class CLANTalkDlg : public CDialog
{
	friend void RecvProcess(LPVOID);
// Construction
public:
	 in_addr getLocalhostAddress();
	bool IfForeWnd();
	CLANTalkDlg(CWnd* pParent = NULL);	// standard constructor
	~CLANTalkDlg();

// Dialog Data
	//{{AFX_DATA(CLANTalkDlg)
	enum { IDD = IDD_LANTALK_DIALOG };
	CIPAddressCtrl	m_ipAddress;
	CListBox	m_listLog;
	CString	m_szInputMsg;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLANTalkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon,m_hIcon1,m_hIcon2,m_hIcon3,m_hIcon4;
	CTrayIcon	m_trayIcon;		// ����ͼ��
	bool m_bShutdown;           //ָʾ�Ƿ�����˳�
    UINT m_iIcon;               //ָʾ�ǵڼ���ͼ��
	bool m_bDynamicIcon;             //ָʾ�Ƿ���Ҫ��̬ͼ��
	SOCKET m_sendSock;			//���ͱ��ĵ�socket
	SOCKET m_recvSock;          //���ձ��ĵ�socket
	char m_szAddr[33];          //�������ĵ�ַ
	sockaddr_in m_addrto;         //���͵�ַ��socket��ʽ�ĵ�ַ
	sockaddr_in m_localAddr;     //������ַ
	CWinThread* m_pRecvThread;  //���Ľ������߳�

	// Generated message map functions
	//{{AFX_MSG(CLANTalkDlg)
	afx_msg LRESULT OnTrayNotification(WPARAM wp, LPARAM lp);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnAppSuspend();
	afx_msg void OnAppOpen();
	afx_msg void OnAppAbout();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized );
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBtnIp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool SetDynamicIcon(bool);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LANTALKDLG_H__C3E90F31_C21F_4B28_A222_BF560454697F__INCLUDED_)
