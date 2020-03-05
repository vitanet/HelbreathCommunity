//////////////////////////////////////////////////////////////////////
//
// Game.cpp: implementation of the Game class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGame::CGame(int iClientH, char *cGameServerName, char *cGameServerIP, int iGameServerPort, char *cGameServerLanAddress)
{
	m_cGameServerMode = 0;
	m_iTrackSock = iClientH;
	ZeroMemory(m_cGameServerName, sizeof(m_cGameServerName));
	ZeroMemory(m_cGameServerAddress, sizeof(m_cGameServerAddress));
	ZeroMemory(m_cGameServerLanAddress, sizeof(m_cGameServerLanAddress));
	memcpy(m_cGameServerName, cGameServerName, 10);
	memcpy(m_cGameServerAddress, cGameServerIP, 15);
	memcpy(m_cGameServerLanAddress, cGameServerLanAddress, 15);
	m_iGameServerPort = iGameServerPort;
}

CGame::~CGame()
{

}