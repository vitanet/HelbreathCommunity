#pragma once

#include <windows.h>
#include <stdio.h>

#include "StrTok.h"
#include "Msg.h"
#include "Client.h"
#include "XSocket.h"
#include "Account.h"
#include "Item.h"
#include "Game.h"
#include "Map.h"

#define DEF_MAXPLAYERACCOUNTS		5000
#define DEF_MAXITEMS				2000
#define DEF_MAXHGSERVERLIST			10
#define DEF_MAXCLIENTSOCK			10000
#define DEF_MAXGAMESERVERSMAPS		100
#define DEF_MAXGAMESERVERS			100
#define DEF_MAXMAINLOGSOCK			10
#define DEF_MAXSUBLOGSOCK			500

extern void UpdateScreen();
#define DEF_MSGQUEUESIZE		100000

class CWorldLog
{
public:
	
	// Hypnotoad
	int iGetItemNumber(char *cItemName);
	void UpdateNoticement(WPARAM wParam);
	void EnterGameConfirm(int iClientH, char *pData);
	void ShutdownGameServer(int iClientH);
	void RequestSaveOccupyFlag(int iClientH, char *pData, char cType);
	int RequestSetAccountWaitStatus(char *pData);
	BOOL bReadItemConfigFile(char *cFn);
	void PutGMLogData(char * pData, DWORD dwMsgSize, BOOL bIsSave);
	void PutItemLogData(char * pData, DWORD dwMsgSize, BOOL bIsSave);
	void PutCrusadeLogData(char * pData, DWORD dwMsgSize, BOOL bIsSave);
	BOOL CWorldLog::bAccept(class XSocket * pXSock, BOOL bBool);
	int OnPlayerAccountMessage(DWORD dwMsgID, char * pData, char * pData2, int iLevel, char * pData3);
	bool bReadServerConfigFile(char *cFn);
	void EnterGame(int iClientH, char * pData);
	int iGetMapInformation(char *cMapName, char * cAddress, int * iPort, int iClientH);
	int iGetCharacterInformation(char * cAccountName, char * Password, int * iLevel);
	void SetAccountStatusInit(int iClientH, char *pData);
	void ForceDisconnectAccount(int iClientH, char *pData);
	void SendServerShutdownMessage();
	void CreateNewCharacterFile(char *pData, char *cFn, char * cNewCharName, char * cAccountName, char * cPassword, char cNewGender, char cNewSkin, int iTempAppr1, int iTempAppr2, int iTempAppr3, char cNewStr, char cNewVit, char cNewDex, char cNewInt, char cNewMag, char cNewChr, short sAppr1, short sAppr2, short sAppr3, short sAppr4);
	void RequestNoSaveLogout(int iClientH, char *pData);
	void VerifyCharacterIntegrity(char *cCharacterName, char *cAccountName, int *iLevel, char *cGuildName, char *cGuildRank, DWORD *dwGuildGUID);
	void VerifyGuildIntegrity(char *cGuildName, DWORD *dwGuildGUID);
	BOOL bAccept(class XSocket * pXSock, bool bCheck);
	bool bClientRegisterMaps(int iClientH, char *pData);
	void DeleteClient(int iClientH, short Mod);
	// complete ascii but uncoded sql:
	void RequestPlayerData(int iClientH, char *pData);
	int iGetCharacterData(char * cCharName, char * cMapName, short * sAppr1, short * sAppr2, short * sAppr3, short * sAppr4, int * iApprColor, char * cSex, char * cSkin, int * iLevel, int * iExp, int * iStr, int * iVit, int * iDex, int * iInt, int * iMag, int * iCharisma, int * iSaveYear, int * iSaveMonth, int * iSaveDay, int * iSaveHour, int * iSaveMinute);
	void RequestSavePlayerData(int iClientH, char *pData, DWORD dwMsgSize, BOOL bVar1, BOOL bVar2);
	void RequestCreateNewCharacter(int iClientH, char *pData);
	void RequestDeleteCharacter(int iClientH, char *pData);
	void RequestCreateNewGuild(int iClientH, char *pData);
	void RequestDisbandGuild(int iClientH, char *pData);
	// incomplete or obsolete:
	void WriteOccupyFlagFile(char *cFn);
	void UpdateGuildInfoNewGuildsman(int iClientH, char *pData);
	void UpdateGuildInfoDeleteGuildman(int iClientH, char *pData);
	// uncoded sql functions:
	int iCreateNewCharacterSQL(char * cCharacterName, char * cAccountName, char *pData, bool bGuildCheck);
	bool bInitSQLServer(void);
	bool bVerifyCharacterGuildSQL(char *pData, DWORD dwGuildGUID);
	void UpdateGuildsmanSQL(char *cMemberName, char *cGuildName, bool bUpdateType);
	int UpdateGuildFile_NewGuildsMan(char * cFile, char * pData, char * cp);
	int UpdateGuildFile_DeleteGuildsMan(char * cFile, char * pData, char * cp);
	int iSaveCharacterSQL(DWORD dwCharID, char *pData);

	// KLKS
	void CharInfoList(int iClientH, char *pData);
	void CleanupLogFiles();
	bool bSendClientConfig(int iClientH, char *cFile);
	void ResponseRegisterGameServer(int iClientH, BOOL bSuccesfull);
	void ClientRegisterGameserver(int iClientH, char * pData);
	void ClientMSLConfirmed(int iClientH);
	void SendEventToMLS(DWORD dwMsgID, WORD wMsgType, char * pData, DWORD dwMsgSize, int iMainH);
	void CheckClientTimeout();
	void MsgProcess();
	void OnTimer();
	BOOL bPutMsgQuene(char cFrom, char * pData, DWORD dwMsgSize, int iIndex, char cKey);
	BOOL bGetMsgQuene(char * pFrom, char * pData, DWORD * pMsgSize, int * pIndex, char * pKey);
	void OnClientRead(int iClientH);
	void RegisterWorldServerSocket(int iMainH);
	void RegisterWorldGameServer();
	void RegisterWorldServer(int iMainH);
	void OnMainSubLogSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void OnClientSubLogSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	CWorldLog(HWND hWnd);
	~CWorldLog(void);
	bool bInit();

	// customization:
	void PutPacketLogData(DWORD dwMsgID, char *cData, DWORD dwMsgSize); // KLKS

	// Variables
	HWND  m_hWnd;												// 4h
	char  m_cWorldLogName[30];									// 8h
	char  m_cWorldServerAddress[21];							// 27h
	class CItem		* m_pItemList[DEF_MAXITEMS];				// 3Ch
	int   m_iWorldServerPort;									// 1F7Ch
	int   m_iWorldServerInternalPort;							// 1F80h
	int   m_iTotalPlayers;										// 1F84h
	int   m_iBlockCount;										// 1F88h
	char  m_cBackupDrive;										// 1F8Ch
	int   m_iMainLogPort;										// 1FA4h
	char  m_sGameServerList[DEF_MAXHGSERVERLIST][16];			// 1FA8h
	char  m_cMainLogAddress[21];								// 1F8Dh
	class CClient	* m_pClientList[DEF_MAXCLIENTSOCK];			// 204Ch
	class CMap		* m_cMapList[DEF_MAXGAMESERVERSMAPS];		// 0BE1Ch
	class CGame		* m_pGameList[DEF_MAXGAMESERVERS];			// 0BC8Ch
	class CAccount	* m_pAccountList[DEF_MAXPLAYERACCOUNTS];	// 0BFACh
	class CMsg		* m_pMsgQueue[DEF_MSGQUEUESIZE];			// 10DCCh
	int   m_iQueueHead;											// 1AA0Ch
	int   m_iQueueTail;											// 1AA10h
	char  * m_pMsgBuffer[30000];								// 1AA14h
	class XSocket 	* m_pMainLogSock[DEF_MAXMAINLOGSOCK];		// 29478h
	BOOL  m_bisMainRegistered[DEF_MAXMAINLOGSOCK];				// 294A0h
	int   m_iActiveMainLogSock;									// 294C8h
	int   m_iCurMainLogSockIndex;								// 294CCh
	int   m_iTotalMainLogSock;									// 294D0h
	char  m_cGMLogBuffer[DEF_MSGBUFFERSIZE];					// 294D4h
	char  m_cGMLogCount;										// 30A04h
	DWORD m_dwGMLogTime;										// 30A08h
	char  m_cItemLogBuffer[DEF_MSGBUFFERSIZE];					// 30A0Ch
	char  m_cItemLogCount;										// 37F3Ch
	DWORD m_dwItemLogTime;										// 37F40h
	char  m_cCrusadeLogBuffer[DEF_MSGBUFFERSIZE];				// 37F44h
	char  m_cCrusadeLogCount;									// 3F474h
	DWORD m_dwCrusadeLogTime;									// 3F478h
	BOOL  m_bIsWorldRegistered;									// 3F47Ch	

	// customized
	BOOL  m_bGameServerList;
	void ServerList(bool Client);
	void ParseCommand(bool dlb, char* pMsg);
	char cGameSecurity[11];
	DWORD m_dwGameTime1;
};
