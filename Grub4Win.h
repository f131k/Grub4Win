// Grub4Win.h : main header file for the GRUB4WIN application
//

#if !defined(AFX_GRUB4WIN_H__B15893B2_EBA4_4CAC_A27E_A9A039CBDE7A__INCLUDED_)
#define AFX_GRUB4WIN_H__B15893B2_EBA4_4CAC_A27E_A9A039CBDE7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGrub4WinApp:
// See Grub4Win.cpp for the implementation of this class
//

class CGrub4WinApp : public CWinApp
{
public:
	CGrub4WinApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrub4WinApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGrub4WinApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRUB4WIN_H__B15893B2_EBA4_4CAC_A27E_A9A039CBDE7A__INCLUDED_)
