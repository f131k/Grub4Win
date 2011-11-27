// Grub4WinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Grub4Win.h"
#include "Grub4WinDlg.h"
#include <PROCESS.H>
#include "ExtractFile.h"
#include <aerosubc.h>
#include <aaeroint.h>
#include <aeroglss.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
	virtual BOOL OnInitDialog();
	void ApplyAeroGlass(CDC *pDC) const;

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnPaint();
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

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Aero effect subclass
	if(!AeroAutoSubclass(m_hWnd, ASC_NO_FRAME_EXTENSION, 0L))
    {
        TRACE(_T("Failed to autosubclass with %lu\n"), GetLastError());
    }
    else
    {
        /// 
        /// loop over the controls outside the placeholder control
        /// and let autoaero subclass them:
        /// 
		static const UINT dwAeroCtrlIds[] = 
		{
			IDOK,
			IDC_STATIC_ABOUT1,
			IDC_STATIC_ABOUT2,
		};

        size_t stIdx = 0;
#ifndef dimof
#define dimof(a) (sizeof(a) / sizeof(a[0]))
#endif
        for (; stIdx < dimof(dwAeroCtrlIds); stIdx++)
        {
            HWND hCtrl = ::GetDlgItem(m_hWnd, dwAeroCtrlIds[stIdx]);
            ASSERT(hCtrl);
            VERIFY(AeroSubClassCtrl(hCtrl));    
        }
    }
	return TRUE;
}

void CAboutDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if(!IsIconic())
        ApplyAeroGlass(&dc);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CAboutDlg::ApplyAeroGlass(CDC *pDC) const
{
    RECT rcPlaceholder;
    ASSERT(pDC);
    RECT rcClient;
    GetClientRect(&rcClient);
	
    MARGINS marGlassInset = {-1, -1, -1, -1}; 
    GetDlgItem(IDC_STATIC_PLACE_HOLDER)->GetWindowRect(&rcPlaceholder);
    ::ScreenToClient(m_hWnd, &rcPlaceholder);
    marGlassInset.cxLeftWidth = rcPlaceholder.left;
    marGlassInset.cxRightWidth = rcClient.right - rcPlaceholder.right;
    marGlassInset.cyBottomHeight = rcClient.bottom - rcPlaceholder.bottom;
    marGlassInset.cyTopHeight = rcPlaceholder.top;
	
	
	
    CDwmApiImpl dwmApi;
    if (dwmApi.Initialize() && dwmApi.IsDwmCompositionEnabled() && 
        SUCCEEDED(dwmApi.DwmExtendFrameIntoClientArea(m_hWnd, &marGlassInset)))
    {
        RECT rcScratch = rcClient;
        rcScratch.right = marGlassInset.cxLeftWidth;
        pDC->PatBlt(rcScratch.left, rcScratch.top, RECTWIDTH(rcScratch), RECTHEIGHT(rcScratch), BLACKNESS);
        rcScratch = rcClient;
        rcScratch.bottom = marGlassInset.cyTopHeight;
        pDC->PatBlt(rcScratch.left, rcScratch.top, RECTWIDTH(rcScratch), RECTHEIGHT(rcScratch), BLACKNESS);
        
        rcScratch = rcClient;
        rcScratch.left = rcScratch.right - marGlassInset.cxRightWidth;
        pDC->PatBlt(rcScratch.left, rcScratch.top, RECTWIDTH(rcScratch), RECTHEIGHT(rcScratch), BLACKNESS);
        
        rcScratch = rcClient;
        rcScratch.top = rcScratch.bottom - marGlassInset.cyBottomHeight;
        pDC->PatBlt(rcScratch.left, rcScratch.top, RECTWIDTH(rcScratch), RECTHEIGHT(rcScratch), BLACKNESS);
    }
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrub4WinDlg dialog

CGrub4WinDlg::CGrub4WinDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGrub4WinDlg::IDD, pParent), 
	m_bInstalled(FALSE), 
	m_strBatName(EXEC_BAT_NAME), 
	m_strInstalledRoot(_T("")), 
	m_iCurrComboIndex(0), 
	m_hThread(INVALID_HANDLE_VALUE)
{
	//{{AFX_DATA_INIT(CGrub4WinDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	/* Setup file name list */
	m_fileNameList.AddTail(LDR);
	m_fileNameList.AddTail(LDR_MBR);
	m_fileNameList.AddTail(MENU_LST);
	/* Setup file resource ID list */
	m_rcIdList.AddTail(IDR_LDR);
	m_rcIdList.AddTail(IDR_LDR_MBR);
	m_rcIdList.AddTail(IDR_MENU_LST);
}

void CGrub4WinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGrub4WinDlg)
	DDX_Control(pDX, IDC_COMBO_DRV, m_drvListCombo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGrub4WinDlg, CDialog)
	//{{AFX_MSG_MAP(CGrub4WinDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_EXEC, OnButtonExec)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UWM_PROCESSING, OnProcessing)
	ON_MESSAGE(UWM_PROCESS_FINISHED, OnProcessFinished)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrub4WinDlg message handlers

BOOL CGrub4WinDlg::OnInitDialog()
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
	
	// TODO: Add extra initialization here
	CString strText;
	strText.LoadString(IDS_EXIT);
	GetDlgItem(IDOK)->SetWindowText(strText);

	/* Set result static control text to null */
	GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T(""));

	if (GetDriveList(m_drvList) == ERROR_SUCCESS)
	{
		// Add to combo box
		for (drv_list_t::const_iterator iter = m_drvList.begin(); iter != m_drvList.end(); iter++)
        {
			UINT uiDrvType = GetDriveType(iter->c_str());
			if (uiDrvType == DRIVE_FIXED)
			{
				m_drvListCombo.AddString(iter->c_str());
			}
        }
	}

	// Determine if we already installed
	DetermineIfInstalled();

	// Set default
	m_drvListCombo.SetCurSel(m_iCurrComboIndex);

	// Update control status
	UpdateCtrlStatus();

	// Aero effect subclass
	if(!AeroAutoSubclass(m_hWnd, ASC_SUBCLASS_ALL_CONTROLS, 0L))
    {
        TRACE(_T("Failed to autosubclass with %lu.\n"), GetLastError());
    }
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGrub4WinDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGrub4WinDlg::OnPaint() 
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
HCURSOR CGrub4WinDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//////////////////////////////////////////////////////////////////////////
/* User defined functions*/
BOOL SearchFileUnderCurrentRoot(LPCTSTR lpRoot, LPCTSTR lpFileName)
{
	WIN32_FIND_DATA findData = {0};
	// Format search file name string
	TCHAR tszFile[MAX_PATH] = {0};
	_tcsncpy(tszFile, lpRoot, MAX_PATH);
	tszFile[MAX_PATH - 1] = 0;
	_tcsncat(tszFile, lpFileName, MAX_PATH);
	tszFile[MAX_PATH - 1] = 0;

	HANDLE hFindHandle = FindFirstFile(tszFile, &findData);
	if (hFindHandle == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	// Close find handle
	FindClose(hFindHandle);
	return TRUE;
}

BOOL CGrub4WinDlg::SearchFileUnderAllRoot(LPCTSTR lpFileName)
{
	int i = 0;
	for (drv_list_t::const_iterator iter = m_drvList.begin(); 
		iter != m_drvList.end(); iter++, i++)
	{
		if (SearchFileUnderCurrentRoot(iter->c_str(), lpFileName))
		{
			m_strInstalledRoot = iter->c_str();
			m_iCurrComboIndex = i;
			return TRUE;
		}
	}
	return FALSE;
}

void CGrub4WinDlg::DetermineIfInstalled()
{
	// Search for flag file(INSTALLED_FLAG_FILE_NAME) to determine if already installed
	m_bInstalled = SearchFileUnderAllRoot(BACKUP_DIR _T("\\") INSTALLED_FLAG_FILE_NAME);
}

void CGrub4WinDlg::UpdateCtrlStatus()
{
	/* Window text that displayed on controls */
	CString strText;

	if (!m_bInstalled)	// Have not installed
	{
		/* Set control window text */
		strText.LoadString(IDS_CHOOSE_DRV);
		GetDlgItem(IDC_STATIC_DRV)->SetWindowText(strText);
		InvalidateCtrl(IDC_STATIC_DRV);

		strText.LoadString(IDS_READY);
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strText);
		InvalidateCtrl(IDC_STATIC_STATUS);

		strText.LoadString(IDS_EXEC_INSTALL);
		GetDlgItem(IDC_BUTTON_EXEC)->SetWindowText(strText);
		//InvalidateCtrl(IDC_BUTTON_EXEC);

		/* Enable or disable control */
		m_drvListCombo.EnableWindow(TRUE);
		InvalidateCtrl(IDC_COMBO_DRV);
		GetDlgItem(IDC_BUTTON_EXEC)->EnableWindow(TRUE);
		InvalidateCtrl(IDC_BUTTON_EXEC);
	}
	else	// Installed
	{
		/* Set control window text */
		strText.LoadString(IDS_INSTALLED_DRV);
		GetDlgItem(IDC_STATIC_DRV)->SetWindowText(strText);
		InvalidateCtrl(IDC_STATIC_DRV);

		strText.LoadString(IDS_READY);
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strText);
		InvalidateCtrl(IDC_STATIC_STATUS);

		strText.LoadString(IDS_EXEC_UNINSTALL);
		GetDlgItem(IDC_BUTTON_EXEC)->SetWindowText(strText);
		//InvalidateCtrl(IDC_BUTTON_EXEC);
		
		/* Enable or disable control */
		m_drvListCombo.EnableWindow(FALSE);
		InvalidateCtrl(IDC_COMBO_DRV);
		GetDlgItem(IDC_BUTTON_EXEC)->EnableWindow(TRUE);
		InvalidateCtrl(IDC_BUTTON_EXEC);
	}	
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	InvalidateCtrl(IDOK);
}

void CGrub4WinDlg::InvalidateCtrl(DWORD dwCtrlId)
{
	CWnd* pCtrl = GetDlgItem(dwCtrlId);
	CRect rc;
	pCtrl->GetWindowRect(rc);
	ScreenToClient(rc);
	InvalidateRect(rc);
}

void CGrub4WinDlg::ProcessExitCode(RUNSISI_HUST::ExitCode_t code)
{
	CString strText;
	if (code == RUNSISI_HUST::Success)
	{
		strText.LoadString(IDS_SUCCESS);
	}
	else
	{	
		strText.LoadString(IDS_FAILURE);
		CString strTmp;
		if (code == RUNSISI_HUST::FileNotFound)
		{
			strTmp.LoadString(IDS_FILENOTFIND);
			strText += _T(": ");
			strText += strTmp;
		}
		else if (code == RUNSISI_HUST::NotSupportedSystem)
		{
			strTmp.LoadString(IDS_NOT_SUPPORTED_SYSTEM);
			strText += _T(": ");
			strText += strTmp;
		}
		else if (code == RUNSISI_HUST::InvalidParameter)
		{
			strTmp.LoadString(IDS_INVALID_PARAMETER);
			strText += _T(": ");
			strText += strTmp;
		}
	}
	GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(strText);
	InvalidateCtrl(IDC_STATIC_RESULT);
}

void CGrub4WinDlg::OnButtonExec() 
{
	// TODO: Add your control notification handler code here
	
	// Get current selected index
	int count = m_drvListCombo.GetCount();
	if (count == CB_ERR)
	{
		return;
	}
	int curr = m_drvListCombo.GetCurSel();
	// Get selected drive
	CString strDrv;
	m_drvListCombo.GetLBText(curr, strDrv);

	CString strBackupPath(strDrv);
	strBackupPath += BACKUP_DIR;

#define STR_INSTALL _T("install")
#define STR_UNINSTALL _T("uninstall")
	// Make execution sentence
	CString strExec(strBackupPath + _T("\\") + m_strBatName);
	strExec += _T(" ");
	strExec += strDrv.Left(1);
	strExec += _T(" ");
	strExec += m_bInstalled ? STR_UNINSTALL : STR_INSTALL;

	/* Create backup directory and related files */
	BOOL bOk = TRUE;
	if (!m_bInstalled)
	{	
		/* Install */
		bOk = CreateBackupDir(strBackupPath) && 
			ExtractBatFile(strBackupPath) && ExtractGrubFile(strBackupPath);
	}
	else
	{
		/* Uninstall */
		bOk = ExtractBatFile(strBackupPath);
	}

	if (bOk)
	{
		bOk = FALSE;
		// We are processing, update control status temporally
		GetDlgItem(IDC_COMBO_DRV)->EnableWindow(FALSE);
		InvalidateCtrl(IDC_COMBO_DRV);
		GetDlgItem(IDC_BUTTON_EXEC)->EnableWindow(FALSE);
		InvalidateCtrl(IDC_BUTTON_EXEC);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		InvalidateCtrl(IDOK);
		GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("                   "));
		InvalidateCtrl(IDC_STATIC_RESULT);


		/* Launch child process */
		HANDLE hChildProcess = INVALID_HANDLE_VALUE;
		LONG lRet = LaunchBatProcess(hChildProcess, strExec);
		if (lRet == ERROR_SUCCESS)
		{
			bOk = TRUE;
			/* Construct thread parameter */
			// Intervals to check if child process already terminated	
			m_threadParam.hProcess = hChildProcess;
			m_threadParam.pWnd = this;
			m_threadParam.dwWaitTime = WAIT_TIME;

			/* Create monitor thread */
			m_hThread = (HANDLE)_beginthreadex(0, 0, StatusThread, &m_threadParam, 0, 0);
			/* TODO: Error handler must be added here */
			ASSERT(m_hThread != 0);
		}
	}
	if (!bOk)
	{
		/* Update result static text */
		CString strText;
		strText.LoadString(IDS_LAUNCH_FAILURE);
		GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(strText);
		InvalidateCtrl(IDC_STATIC_RESULT);

		/* Create file failed or launch child process failed, update control status */
		UpdateCtrlStatus();
		DelBackupDir(strBackupPath);
	}
}


//////////////////////////////////////////////////////////////////////////
// User defined message handlers

LRESULT CGrub4WinDlg::OnProcessing(WPARAM wParam, LPARAM lParam)
{
	CStatic* status = (CStatic*)GetDlgItem(IDC_STATIC_STATUS);
	ASSERT(status != 0);

	static int iDot = 0;
	iDot++;
	if (iDot == 1)
	{
		status->SetWindowText(_T("."));
	}
	else if (iDot == 2)
	{
		status->SetWindowText(_T(".."));
	}
	else if (iDot == 3)
	{
		status->SetWindowText(_T("..."));
		iDot = 0;
	}
	InvalidateCtrl(IDC_STATIC_STATUS);

	return 0;
}

LRESULT CGrub4WinDlg::OnProcessFinished(WPARAM wParam, LPARAM lParam)
{
	/* Get child process exit code */
	DWORD dwExitCode = (DWORD)wParam;
	
	/* Process exit code */
	ProcessExitCode((RUNSISI_HUST::ExitCode_t)dwExitCode);

	/* Close monitor thread handle */
	CloseHandle(m_hThread);
	m_hThread = INVALID_HANDLE_VALUE;

	/* Update control status */
	DetermineIfInstalled();
	UpdateCtrlStatus();

	int curr = m_drvListCombo.GetCurSel();
	CString strDrv;
	m_drvListCombo.GetLBText(curr, strDrv);
	
	CString strBackupPath(strDrv);
	strBackupPath += BACKUP_DIR;

	if (m_bInstalled)
	{
		DelBatFile(strBackupPath);
	}
	else
	{
		DelBackupDir(strBackupPath);
	}

	return 0;
}

BOOL CGrub4WinDlg::CreateBackupDir(CString strPath)
{
	/* Create backup directory */
	BOOL bOk = CreateDirectory(strPath, 0);
	if (!bOk && GetLastError() != ERROR_ALREADY_EXISTS)
	{
		return bOk;
	}

	/* Set directory attribute */
	SetFileAttributes(strPath, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);

	return TRUE;
}


BOOL CGrub4WinDlg::ExtractBatFile(CString strPath)
{
	LONG lRet = 0L;
	CString strBatFullPathName(strPath);
	strBatFullPathName += _T("\\");
	strBatFullPathName += m_strBatName;
	lRet = ExtractFileFromResource(strBatFullPathName, IDR_EXEC_BAT, RC_TYPE);
	return (lRet == ERROR_SUCCESS);
}

BOOL CGrub4WinDlg::ExtractGrubFile(CString strPath)
{
	/* Extract grub file from app resource */
	LONG lRet = 0L;
	POSITION posName = m_fileNameList.GetHeadPosition();
	CString strFileName;
	
	POSITION posId = m_rcIdList.GetHeadPosition();
	DWORD dwId = 0;
	
	BOOL bOk = TRUE;
	while (posName)
	{
		strFileName = m_fileNameList.GetNext(posName);
		dwId = m_rcIdList.GetNext(posId);
		lRet = ExtractFileFromResource(strPath + _T("\\") + strFileName, dwId, RC_TYPE);
		if (lRet != ERROR_SUCCESS)
		{
			bOk = FALSE;
			break;
		}
	}
	
	return bOk;
}

BOOL CGrub4WinDlg::DelBackupDir(CString strPath)
{
	WIN32_FIND_DATA findData = {0};
	// Format search file name string
	CString strPattern(strPath + _T("\\*"));
	
	HANDLE hFindHandle = FindFirstFile(strPattern, &findData);
	if (hFindHandle == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	do
	{
		/* Delete found file */
		if (_tcscmp(_T("."), findData.cFileName) && _tcscmp(_T(".."), findData.cFileName))
		{
			DeleteFile(strPath + _T("\\") + findData.cFileName);
		}
	} while (FindNextFile(hFindHandle, &findData));

	// Close find handle
	FindClose(hFindHandle);

	RemoveDirectory(strPath);
	
	return TRUE;
}

BOOL CGrub4WinDlg::DelBatFile(CString strPath)
{
	CString strBatFullPathName(strPath);
	strBatFullPathName += _T("\\");
	strBatFullPathName += m_strBatName;
	DeleteFile(strBatFullPathName);
	return TRUE;
}
