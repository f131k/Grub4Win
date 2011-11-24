// Grub4WinDlg.cpp : implementation file
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
// CGrub4WinDlg dialog

CGrub4WinDlg::CGrub4WinDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGrub4WinDlg::IDD, pParent), 
	m_bInstall(TRUE), 
	m_strBatName(EXEC_BAT_NAME), 
	m_strInstalledRoot(_T("")), 
	m_iCurrComboIndex(0)
{
	//{{AFX_DATA_INIT(CGrub4WinDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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

	// Already installed, disable the combo box
	if (!m_bInstall)
	{
		m_drvListCombo.EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_DRV)->SetWindowText(_T("已安装在："));
		GetDlgItem(IDC_BUTTON_EXEC)->SetWindowText(_T("卸载"));
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
	m_bInstall = !(SearchFileUnderAllRoot(INSTALLED_FLAG_FILE_NAME));
}

void CGrub4WinDlg::OnButtonExec() 
{
	// TODO: Add your control notification handler code here
	m_redir.m_pWnd = (CEdit*) GetDlgItem(IDC_EDIT_OUTPUT);
	m_redir.m_pWnd->SetWindowText(_T(""));
	m_redir.Close();
	
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

#define STR_INSTALL _T("install")
#define STR_UNINSTALL _T("uninstall")
	// Make execution sentence
	CString strExec(m_strBatName);
	strExec += _T(" ");
	strExec += strDrv.Left(1);
	strExec += _T(" ");
	strExec += m_bInstall ? STR_INSTALL : STR_UNINSTALL;

	if (m_bInstall)
	{
		GetDlgItem(IDC_COMBO_DRV)->EnableWindow(FALSE);
	}

	m_redir.Open(strExec);
}
