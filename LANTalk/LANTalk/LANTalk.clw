; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CLANTalkDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "LANTalk.h"

ClassCount=3
Class1=CLANTalkApp
Class2=CLANTalkDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_LANTALK_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Resource4=IDR_TRAYICON

[CLS:CLANTalkApp]
Type=0
HeaderFile=LANTalk.h
ImplementationFile=LANTalk.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CLANTalkApp

[CLS:CLANTalkDlg]
Type=0
HeaderFile=LANTalkDlg.h
ImplementationFile=LANTalkDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CLANTalkDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=LANTalkDlg.h
ImplementationFile=LANTalkDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_LANTALK_DIALOG]
Type=1
Class=CLANTalkDlg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_IPADDRESS,SysIPAddress32,1342242816
Control5=IDC_BTN_IP,button,1342242816
Control6=IDC_LIST_LOG,listbox,1352728833
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_INPUT,edit,1350631552

[MNU:IDR_TRAYICON]
Type=1
Class=CLANTalkDlg
Command1=ID_APP_OPEN
Command2=ID_APP_ABOUT
Command3=ID_APP_SUSPEND
CommandCount=3

