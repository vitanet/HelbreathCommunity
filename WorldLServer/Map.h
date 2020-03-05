#if !defined(AFX_MAP_H__A9554BE2_A96B_11D2_B143_00001C7030A7__INCLUDED_)
#define AFX_MAP_H__A9554BE2_A96B_11D2_B143_00001C7030A7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "NetMessages.h"


class CMap  
{
public:
		
	CMap(int iClientH, char *pData);
	virtual ~CMap();

	char  m_cName[11];			// 4h
	char  m_cLocationName[11];
	int	  iIndex;				// 10h
};

#endif // !defined(AFX_MAP_H__A9554BE2_A96B_11D2_B143_00001C7030A7__INCLUDED_)
