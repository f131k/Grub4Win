/*!
* \file ChildProcess.cpp
* \author runsisi@163.com
* \date 2011/11/24
*/

#include "stdafx.h"
#include "ChildProcess.h"

LONG LaunchBatProcess(HANDLE& hChildProcess, LPCTSTR lpCmdLine)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	
	ASSERT(::AfxIsValidString(lpCmdLine));
	ASSERT(hChildProcess == INVALID_HANDLE_VALUE);
	
	// Set up the start up info struct.
	::ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW;
	
	// Note that dwFlags must include STARTF_USESHOWWINDOW if we
	// use the wShowWindow flags. This also assumes that the
	// CreateProcess() call will use CREATE_NEW_CONSOLE.
	
	// Duplicate the command line string
	TCHAR* tszCmdLine = _tcsdup(lpCmdLine);
	TCHAR* tszCurDir = _tcsdup(lpCmdLine);
	TCHAR* p = tszCurDir;
	while (*p++)
		;
	while (*(--p) != '\\')
		*p = 0;
	// Launch the child process.
	if (!::CreateProcess(NULL, tszCmdLine, 0, 0, 0, 
		CREATE_NEW_CONSOLE,	0, 
		tszCurDir, 
		&si, &pi))
	{
		free(tszCmdLine);
		return ::GetLastError();
	}
	free(tszCmdLine);
	free(tszCurDir);
	hChildProcess = pi.hProcess;
	// Close any non-useful handles
	::CloseHandle(pi.hThread);
	
	return ERROR_SUCCESS;
}

unsigned __stdcall StatusThread(void* pThreadParam)
{
	ThreadParam_t* param = (ThreadParam_t*)pThreadParam;
	ASSERT(param != 0);

	HANDLE hProcess = param->hProcess;
	CWnd* pWnd = param->pWnd;
	DWORD dwWaitTime = param->dwWaitTime;

	ASSERT(hProcess != INVALID_HANDLE_VALUE);
	ASSERT(pWnd != 0);
	ASSERT(dwWaitTime > 0);

	// Wait for child process to terminate
	while (TRUE)
	{
		DWORD dwRet = ::WaitForSingleObject(hProcess, dwWaitTime);
		if (dwRet == WAIT_OBJECT_0)
		{
			break;
		}
		// Post message to indicate status
		pWnd->PostMessage(UWM_PROCESSING);
	}

	/* Get child process exit code */
	DWORD dwExitCode = 0;
	BOOL bOk = GetExitCodeProcess(hProcess, &dwExitCode);

	/* Child process terminated */
	pWnd->PostMessage(UWM_PROCESS_FINISHED, (WPARAM)dwExitCode);

	return 0;
}