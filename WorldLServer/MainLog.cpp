// MainLog.cpp: implementation of the Party class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMainLog::CMainLog(HWND hWnd)
{

	m_pXSock = NULL;
	m_pXSock = new class XSocket(hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
	m_pXSock->bInitBufferSize(DEF_MSGBUFFERSIZE);

	//ZeroMemory(m_cGameServerName, sizeof(m_cGameServerName));
	//ZeroMemory(m_cIPaddress, sizeof(m_cIPaddress));
	//m_bIsProcessingAvailable = FALSE;
	//m_cRegisterationStatus = 0;
	//m_cClientServerAddr = 0;
	//m_wClientListMember = 0;
	//bIsWSRegistered = FALSE;	//No its not registered
	m_bIsUsable = FALSE;			//Can this socket be used?
}

CMainLog::~CMainLog()
{
	if (m_pXSock != NULL) delete m_pXSock;
}
