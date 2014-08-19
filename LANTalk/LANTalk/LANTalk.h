// LANTalk.h : main header file for the LANTALK application
//

#if !defined(AFX_LANTALK_H__F5C69245_CBA0_4FC3_B4F8_BBC1264F16F6__INCLUDED_)
#define AFX_LANTALK_H__F5C69245_CBA0_4FC3_B4F8_BBC1264F16F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLANTalkApp:
// See LANTalk.cpp for the implementation of this class
//

class CLANTalkApp : public CWinApp
{
public:
	CLANTalkApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLANTalkApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLANTalkApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LANTALK_H__F5C69245_CBA0_4FC3_B4F8_BBC1264F16F6__INCLUDED_)
