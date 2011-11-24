/*!
* \file diskinfo.h
* \author runsisi@163.com
* \date 2011/11/13
*/

#ifndef DISKINFO_RUNSISI_HUST_H_
#define DISKINFO_RUNSISI_HUST_H_

#include "stdafx.h"
#include <list>
#include <string>

// Type definition
typedef std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> > tstring;
typedef std::list<tstring> drv_list_t;

LONG GetDriveList(drv_list_t& drvList);
LONG ParseDriveString(drv_list_t& l, LPTSTR lpBuffer);

#endif