// Client.cpp: implementation of the Party class.
//
//////////////////////////////////////////////////////////////////////
#include "Client.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClient::CClient(HWND hWnd)
{

	m_pXSock = NULL;
	m_pXSock = new class XSocket(hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
	m_pXSock->bInitBufferSize(DEF_MSGBUFFERSIZE);
	
	m_cMode = 0;
	ZeroMemory(m_cGameServerName, sizeof(m_cGameServerName));
	ZeroMemory(m_cIPaddress, sizeof(m_cIPaddress));
}

CClient::~CClient()
{
	if (m_pXSock != NULL) delete m_pXSock;
}
