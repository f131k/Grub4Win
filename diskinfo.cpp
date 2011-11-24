/*!
* \file diskinfo.cpp
* \author runsisi@163.com
* \date 2011/11/23
*/

#include "stdafx.h"
#include <list>
#include <string>
#include "diskinfo.h"

LONG GetDriveList(drv_list_t& drvList)
{
	DWORD dwLen = GetLogicalDriveStrings(0, 0);
	TCHAR* tszDriveList = new TCHAR[dwLen + 1];
    dwLen = GetLogicalDriveStrings(dwLen, tszDriveList);

    LONG lRet = ParseDriveString(drvList, tszDriveList);
	delete[] tszDriveList;

	return lRet;
}

LONG ParseDriveString(drv_list_t& l, LPTSTR lpBuffer)
{
	if (!lpBuffer)
	{
		return ERROR_INVALID_PARAMETER;
	}
	TCHAR* curr = lpBuffer;
	int i = 0;
	TCHAR tszDrv[4] = {0};
	tstring tstrDrv;
	while (*curr)
	{
		while (*curr)
		{
			tszDrv[i++] = *curr++;
		}
		curr++;
		tszDrv[i] = 0;
		i = 0;
		tstrDrv = tszDrv;
		l.push_back(tstrDrv);
	}
	
	return ERROR_SUCCESS;
}
