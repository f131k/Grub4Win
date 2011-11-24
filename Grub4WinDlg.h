// Grub4WinDlg.h : header file
//

#if !defined(AFX_GRUB4WINDLG_H__DD2C35CB_D148_41B6_B925_6B4D62CEE307__INCLUDED_)
#define AFX_GRUB4WINDLG_H__DD2C35CB_D148_41B6_B925_6B4D62CEE307__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Redirect/DemoRedir.h"
#include "diskinfo.h"

/////////////////////////////////////////////////////////////////////////////
// CGrub4WinDlg dialog

#define EXEC_BAT_NAME _T("exec.bat")
#define INSTALLED_FLAG_FILE_NAME _T("{0FC3E696-387C-4a04-8CDF-EF501BAE29AD}.boot.runsisi")

class CGrub4WinDlg : public CDialog
{
// Construction
public:
	CGrub4WinDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGrub4WinDlg)
	enum { IDD = IDD_GRUB4WIN_DIALOG };
	CComboBox	m_drvListCombo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrub4WinDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CDemoRedirector m_redir;	// Console redirector
	BOOL m_bInstall;			// If we will install or uninstall
	CString m_strBatName;		// The bat file name we want to execute
	drv_list_t m_drvList;		// A list of fixed drive in current computer
	CString m_strInstalledRoot;	// The drive where we have installed
	int m_iCurrComboIndex;

	void DetermineIfInstalled();
	BOOL SearchFileUnderAllRoot(LPCTSTR lpFileName);

	// Generated message map functions
	//{{AFX_MSG(CGrub4WinDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonExec();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRUB4WINDLG_H__DD2C35CB_D148_41B6_B925_6B4D62CEE307__INCLUDED_)
