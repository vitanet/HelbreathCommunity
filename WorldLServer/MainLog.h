#if !defined(AFX_CLIENT_H__A9554BE2_A96B_11D2_B143_00001C7030A6__INCLUDED_)
#define AFX_CLIENT_H__A9554BE2_A96B_11D2_B143_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "XSocket.h"
#include "NetMessages.h"


class CMainLog  
{
public:
		
	CMainLog(HWND hWnd);
	virtual ~CMainLog();

	class XSocket * m_pXSock;
	//BOOL bIsWSRegistered;	//Checks if we have registered to mainlog
	//char  m_cGameServerName[11];
	//char  m_cIPaddress[16];
	//BOOL  m_bIsProcessingAvailable;
	//char  m_cRegisterationStatus;
	//char  m_cClientServerAddr;
	//WORD  m_wClientListMember;
	//DWORD m_dwAcceptTime;
	//DWORD m_dwClientHd;
	BOOL m_bIsUsable;
};

#endif // !defined(AFX_STRTOK_H__A9554BE2_A96B_11D2_B143_00001C7030A6__INCLUDED_)
