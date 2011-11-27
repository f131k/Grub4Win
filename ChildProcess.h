/*!
* \file ChildProcess.h
* \author runsisi@163.com
* \date 2011/11/24
*/

#include "stdafx.h"

#define UWM_PROCESSING (WM_APP+44)
#define UWM_PROCESS_FINISHED (UWM_PROCESSING+1)

typedef enum tagExitCode
{
	Success = 0, FileNotFound = 1, NotSupportedSystem = 2, InvalidParameter = 3, Unknown,
} ExitCode_t;

typedef struct tagThreadParam
{
	HANDLE hProcess;
	CWnd* pWnd;
	DWORD dwWaitTime;
} ThreadParam_t;

LONG LaunchBatProcess(HANDLE& hChildProcess, LPCTSTR lpCmdLine);
unsigned __stdcall StatusThread(void* pThreadParam);
