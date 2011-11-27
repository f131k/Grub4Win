// Grub4WinDlg.h : header file
//

#if !defined(AFX_GRUB4WINDLG_H__DD2C35CB_D148_41B6_B925_6B4D62CEE307__INCLUDED_)
#define AFX_GRUB4WINDLG_H__DD2C35CB_D148_41B6_B925_6B4D62CEE307__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "diskinfo.h"
#include "ChildProcess.h"

/////////////////////////////////////////////////////////////////////////////
// CGrub4WinDlg dialog

#define BACKUP_DIR _T("RUNSISI.{0FC3E696-387C-4a04-8CDF-EF501BAE29AD}")
#define INSTALLED_FLAG_FILE_NAME _T("boot.{0FC3E696-387C-4a04-8CDF-EF501BAE29AD}")
#define EXEC_BAT_NAME _T("exec.bat")
#define LDR _T("grldr")
#define LDR_MBR _T("grldr.mbr")
#define MENU_LST _T("menu.lst")
#define RC_TYPE _T("BINARY")

#define VMLINUZ _T("vmlinuz")
#define INITRD_LZ _T("initrd.lz")

class CGrub4WinDlg : public CDialog
{
// Construction
public:
	CGrub4WinDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGrub4WinDlg)
	enum { IDD = IDD_GRUB4WIN_DIALOG };
	CComboBox	m_isoListCombo;
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
	BOOL m_bInstalled;				// Indicate if we have already installed or not
	CString m_strBatName;			// The bat file name we want to execute
	drv_list_t m_drvList;			// A list of fixed drive in current computer
	CString m_strInstalledRoot;		// The drive where we have installed
	int m_iCurrComboIndex;
	enum {WAIT_TIME = 200};
	HANDLE m_hThread;				// Monitor thread ID
	ThreadParam_t m_threadParam;	// Parameters passed to monitor thread
	CStringList m_fileNameList;		// File that need to be extracted from resource
	CList<DWORD, DWORD> m_rcIdList;	// File's resource ID
	CStringList m_isoList;			// Record the searched ISO file
	/* Record the last installation backup directory, in case of installation 
	 * failure to delete the backup directory */
	CString m_strBackupDir;

	void DetermineIfInstalled();						// Determine if we have already installed or not
	BOOL SearchFileUnderAllRoot(LPCTSTR lpFileName);	// Return true on found
	void UpdateCtrlStatus();							// Enable or disable controls
	void ProcessExitCode(RUNSISI_HUST::ExitCode_t code);// Set info according child process exit code
	BOOL CreateBackupDir(CString strPath);
	BOOL ExtractBatFile(CString strPath);	
	BOOL ExtractGrubFile(CString strPath);
	BOOL DelBatFile(CString strPath);
	BOOL DelBackupDir(CString strPath);
	void InvalidateCtrl(DWORD dwCtrlId);				// Force the control to be repainted
	/* ISO file search and modify menu.lst related member functions */
	void SearchIsoFileUnderAllRoot();					// The result will be set in m_isoList
	void UpdateIsoListCombo();							// Update the m_isoListCombo content
	void SetWidthIsoListCombo();						// Set the minimum width of m_isoListCombo control
	BOOL ValidateAllFilesAreReady();					// vmlinuz, initrd.lz and xxx.iso are ready
	void AppendEntryToMenuLst();						// Append an entry to menu.lst file

	// Generated message map functions
	//{{AFX_MSG(CGrub4WinDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonExec();
	afx_msg void OnDestroy();
	afx_msg void OnButtonRefresh();
	//}}AFX_MSG
	afx_msg LRESULT OnProcessing(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProcessFinished(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRUB4WINDLG_H__DD2C35CB_D148_41B6_B925_6B4D62CEE307__INCLUDED_)
