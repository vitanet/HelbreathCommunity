#if !defined(AFX_ACCOUNT_H__A9554BE2_A96B_11D2_B143_00001C7030A7__INCLUDED_)
#define AFX_ACCOUNT_H__A9554BE2_A96B_11D2_B143_00001C7030A7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "Account.h"
#include "NetMessages.h"

class CAccount  
{
public:
		
	CAccount(char * pData, DWORD dwTime);
	virtual ~CAccount();

	char cAccountType;			// 4h
	char cAccountName[11];		// 5h
	char cPassword[11];			// 10h
	char cCharacterName[11];	// 18h
	char m_cOnGameServer[11];	// 1Bh
	int  m_iLevel;				// 28h
	DWORD dwLogInTime;			// 2Ch
	int Timeout;

};

#endif
