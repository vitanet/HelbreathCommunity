#include "Game.h"

CGame::CGame(int iClientH, char *cGameServerName, char *cGameServerIP, int iGameServerPort, char *cWorldName, int iTotalMaps)
{
	ZeroMemory(m_cGameName, sizeof(m_cGameName));
	ZeroMemory(m_cGameServerAddress, sizeof(m_cGameServerAddress));
	ZeroMemory(m_cWorldName, sizeof(m_cWorldName));
	m_iGamePort = iGameServerPort;
	m_iTotalMaps = iTotalMaps;
	m_iTrackSock = iClientH; //WLserver ID
	memcpy(m_cGameName, cGameServerName, 10);
	memcpy(m_cGameServerAddress, cGameServerIP, 15);
	memcpy(m_cWorldName, cWorldName, 30);
}
CGame::~CGame()
{

}