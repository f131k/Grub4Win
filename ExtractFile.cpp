/*!
* \file ExtractFile.cpp
* \author runsisi@163.com
* \date 2011/11/26
*/

#include "StdAfx.h"
#include "ExtractFile.h"

LONG ExtractFileFromResource(LPCTSTR lpDstFullPathName, DWORD dwRcId, LPCTSTR lpRcType)
{
	/* TODO: Add error handler */
	LONG lRet = 0L;
	FILE* pFile = 0;
	HRSRC hRc = FindResource(0, MAKEINTRESOURCE(dwRcId), lpRcType);
	DWORD dwRcSize = SizeofResource(0, hRc);
	HGLOBAL hRcData = LoadResource(0, hRc);
	LPVOID pRcData = LockResource(hRcData);

	/* Write resource to file */
	DeleteFile(lpDstFullPathName);
	pFile = _tfopen(lpDstFullPathName, _T("wb"));
	size_t uiWritten = fwrite(pRcData, 1, dwRcSize, pFile);
	
	/* Close file */
	fclose(pFile);

	return ERROR_SUCCESS;
}
