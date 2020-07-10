; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CLwp544View
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "lwp544.h"
LastPage=0

ClassCount=6
Class1=CLwp544App
Class2=CLwp544Doc
Class3=CLwp544View
Class4=CMainFrame

ResourceCount=3
Resource1=IDR_MAINFRAME
Class5=CAboutDlg
Resource2=IDD_ABOUTBOX
Class6=CHistogramDlg
Resource3=IDD_DIALOG1

[CLS:CLwp544App]
Type=0
HeaderFile=lwp544.h
ImplementationFile=lwp544.cpp
Filter=N

[CLS:CLwp544Doc]
Type=0
HeaderFile=lwp544Doc.h
ImplementationFile=lwp544Doc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CLwp544Doc

[CLS:CLwp544View]
Type=0
HeaderFile=lwp544View.h
ImplementationFile=lwp544View.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=CLwp544View


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_HL




[CLS:CAboutDlg]
Type=0
HeaderFile=lwp544.cpp
ImplementationFile=lwp544.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_APP_ABOUT
CommandCount=16

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_OPEN
Command2=ID_APP_ABOUT
Command3=ID_GRAY
Command4=ID_HESTOGRAM
Command5=ID_EQUALIZE
Command6=ID_LINETRANS
Command7=ID_FOURIER
Command8=ID_IFOURIER
Command9=ID_FFOURIER
Command10=ID_IFFOURIER
Command11=ID_AVE
Command12=ID_MID
Command13=ID_STAIR
Command14=ID_RAPLAS
Command15=ID_GOSS
Command16=ID_HL
CommandCount=16

[DLG:IDD_DIALOG1]
Type=1
Class=CHistogramDlg
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[CLS:CHistogramDlg]
Type=0
HeaderFile=HistogramDlg.h
ImplementationFile=HistogramDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CHistogramDlg
VirtualFilter=dWC

