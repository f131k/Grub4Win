// Grub4Win.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Grub4Win.h"
#include "Grub4WinDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrub4WinApp

BEGIN_MESSAGE_MAP(CGrub4WinApp, CWinApp)
	//{{AFX_MSG_MAP(CGrub4WinApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrub4WinApp construction

CGrub4WinApp::CGrub4WinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGrub4WinApp object

CGrub4WinApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGrub4WinApp initialization

BOOL CGrub4WinApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	const TCHAR* tszEvent = TEXT("{0FC3E696-387C-4a04-8CDF-EF501BAE29AD}");
	//const TCHAR* tszWindowClassName = "YiGe_RUNSISI_HUST";
	const TCHAR* tszWindowTitleName = "Grub4Win";
	HANDLE hEvent = CreateEvent(0, 0, 0, tszEvent);
	DWORD dwErr = GetLastError();
	if (dwErr == ERROR_ALREADY_EXISTS)
	{
		HWND hWindow = ::FindWindow(0, tszWindowTitleName);
		if (hWindow)
		{
			::FlashWindow(hWindow, TRUE);
			//::ShowWindow(hWindow, SW_SHOWNORMAL);
			//BOOL bOk = ::SetWindowPos(hWindow, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			return TRUE;
		}
	}

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CGrub4WinDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
