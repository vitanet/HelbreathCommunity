#if !defined(AFX_MAIN_H__0089D9E3_74E6_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_MAIN_H__0089D9E3_74E6_11D2_A8E6_00001C7030A6__INCLUDED_

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "XSocket.h"
#include "NetMessages.h"
#include "Msg.h"
#include "Client.h"
#include "StrTok.h"
#include "Game.h"
#include "Map.h"
#include "Account.h"
#include "GlobalDef.h"

//#define DEF_MAXMAINLOGSOCK			500 //sockets used by Wlservers
#define DEF_MAXCLIENTSOCK			2000
#define DEF_MSGQUEUESIZE		    100000
#define DEF_MAXGAME					50 //total game servers
#define DEF_MAXMAPS					1000
#define DEF_MAXACCOUNTS				1000
#define DEF_MAXCHARACTER			4000

extern void UpdateScreen();

class CMainLog
{
public:
	void MsgProcess();
	BOOL bGetMsgQuene(char * pFrom, char * pData, DWORD * pMsgSize, int * pIndex, char * pKey);
	BOOL bPutMsgQuene(char cFrom, char * pData, DWORD dwMsgSize, int iIndex, char cKey);
	void OnTimer();
	BOOL bAccept(class XSocket * pXSock, bool bWorld);
	bool bInit();
	void OnClientSubLogSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void SendEventToWLS(DWORD dwMsgID, WORD wMsgType, char * pData, DWORD dwMsgSize, int iWorldH);
	void DeleteAccount(int iClientH, char cAccountName[11] = NULL);
	BOOL SaveAccountInfo(int iAccount, char cAccountName[11] = NULL, char cTemp[11] = NULL, char cCharName[11] = NULL, char cMode = NULL, int iClientH = NULL);
	void SaveInfo(char cFileName[255], char *pData,  DWORD dwStartSize);
	
	//functions used for messages recieved
	void RegisterSocketWL(int iClientH, char *pData, bool bRegister);
	void RegisterGameServer(int iClientH, char *pData, bool bRegister);
	BOOL bClientRegisterMaps(int iClientH, char *pData);
	void RequestLogin(int iClientH, char *pData);
	void TotalChar(int iClientH, char *pData);
	void ResponseCharacter(int iClientH, char *pData, char cMode = NULL);
	void DeleteCharacter(int iClientH, char *pData, char cMode = NULL);
	void ChangePassword(int iClientH, char *pData);
	void RequestEnterGame(int iClientH, char *pData, char cMode);

	//geting or checking account info
	int GetAccountInfo(int iClientH, char cAccountName[11], char cAccountPass[11], char cWorldName[30] = NULL, int * iAccount = NULL, char cMode = NULL);

	void PutPacketLogData(DWORD dwMsgID, char *cData, DWORD dwMsgSize);
	void CleanupLogFiles();
	BOOL bReadServerConfigFile(char *cFn);

	CMainLog(HWND hWnd);
	~CMainLog(void);
	
	class CMsg		* m_pMsgQueue[DEF_MSGQUEUESIZE];			// 10DCCh
	class CClient	* m_pClientList[DEF_MAXCLIENTSOCK];
	char  * m_pMsgBuffer[30000];
	int   m_iQueueHead, m_iQueueTail;	
	char m_cMainServerAddress[20];
	char m_cBackupDrive;
	int m_iMainServerInternalPort, m_iMainServerPort;
	int m_iValidAccounts, m_iTotalWorld, m_iTotalGame;
	HWND  m_hWnd;
//	class XSocket 	* m_pMainLogSock[DEF_MAXMAINLOGSOCK];		// 29478h
	int m_iTotalMainLogSock;
	BOOL bMainActivation;
	class CGame	* m_pGameList[DEF_MAXGAME];
	class CMap	* m_pMapList[DEF_MAXMAPS];

	class CAccount	* m_pAccountList[DEF_MAXACCOUNTS];
	class CCharacter  * m_pCharList[DEF_MAXCHARACTER];

	void CreateAccount(int iClientH, char* pData);
	void CreateNewAccountFile(char* pData, char* cFn, char* cAccountName, char* cAccountPassword, char* m_cEmailAddr, char* m_cAccountQuiz, char* m_cAccountAnswer);
};

#endif