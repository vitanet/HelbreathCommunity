#if !defined(AFX_GAME_H__A9554BE2_A96B_11D2_B143_00001C7030A7__INCLUDED_)
#define AFX_GAME_H__A9554BE2_A96B_11D2_B143_00001C7030A7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "NetMessages.h"


class CGame  
{
public:
		
	CGame(int iClientH, char *cGameServerName, char *cGameServerIP, int iGameServerPort, char *cGameServerLanAddress);
	virtual ~CGame();

	char m_cGameServerMode;				// 4h
	int  m_iTrackSock;					// 8h
	char m_cGameServerName[11];			// 0Ch
	char m_cGameServerAddress[16];		// 17h
	int  m_iGameServerPort;				// 28h
	char m_cGameServerLanAddress[16];

};

#endif
