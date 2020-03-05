#if !defined(AFX_CLIENT_H__A9554BE2_A96B_11D2_B143_00001C7030A7__INCLUDED_)
#define AFX_CLIENT_H__A9554BE2_A96B_11D2_B143_00001C7030A7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "XSocket.h"
#include "NetMessages.h"


class CClient  
{
public:
		
	CClient(HWND hWnd);
	virtual ~CClient();

	char  m_cMode;					// 4h
	char  m_cGameServerName[11];	// 5h
	char  m_cIPaddress[20];			// 10h
	class XSocket * m_pXSock;		// 24h
};

#endif
