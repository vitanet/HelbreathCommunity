#include "Heldenian.h"

extern void PutLogList(char* cMsg);
extern char G_cTxt[512];
extern char	G_cData50000[50000];
extern void PutLogFileList(char* cStr);
extern void PutAdminLogFileList(char* cStr);
extern void PutItemLogFileList(char* cStr);
extern void PutLogEventFileList(char* cStr);
extern void PutHackLogFileList(char* cStr);
extern void PutPvPLogFileList(char* cStr);
extern FILE* pLogFile;
extern HWND	G_hWnd;

#pragma warning (disable : 4996 6011 6001 4244 4018 6385 6386 26451 6054 4267 6053 6031)

Heldenian::Heldenian()
{
}

Heldenian::~Heldenian()
{
}

void CGame::_CheckStrategicPointOccupyStatus(char cMapIndex)
{
	class CTile* pTile;
	int i, iX, iY, iSide, iValue;

	m_iStrategicStatus = 0;

	for (i = 0; i < DEF_MAXSTRATEGICPOINTS; i++)
		if (m_pMapList[cMapIndex]->m_pStrategicPointList[i] != NULL) {

			iSide = m_pMapList[cMapIndex]->m_pStrategicPointList[i]->m_iSide;
			iValue = m_pMapList[cMapIndex]->m_pStrategicPointList[i]->m_iValue;
			iX = m_pMapList[cMapIndex]->m_pStrategicPointList[i]->m_iX;
			iY = m_pMapList[cMapIndex]->m_pStrategicPointList[i]->m_iY;

			pTile = (class CTile*)(m_pMapList[cMapIndex]->m_pTile + iX + iY * m_pMapList[cMapIndex]->m_sSizeY);

			// Ã€Ã¼Â·Â«Ã€Ã» ÃÃ¶ÃÂ¡Ã€Ã‡ Â°Â¡ÃÃŸÃ„Â¡Â¸Â¦ Â°Ã¶Ã‡Ã˜Â¼Â­ Â´ÃµÃ‡Ã‘Â´Ã™. 
			m_iStrategicStatus += pTile->m_iOccupyStatus * iValue;
		}
}

/*********************************************************************************************************************
**  BOOL CGame::__bSetOccupyFlag(char cMapIndex,int dX,int dY,int iSide,int iEKNum,int iClientH,BOOL bAutoFlag)		**
**  description		:: A player has used a Flag during Heldenian event...        									**
**	Called when player sets a flag, or at Heldenian Type 2 beginning												**
*********************************************************************************************************************/
BOOL CGame::__bSetOccupyFlag(char cMapIndex, int dX, int dY, int iSide, int iEKNum, int iClientH, BOOL bAutoFlag)
{
	int ix, iy;
	int   iDynamicObjectIndex, iIndex;
	class CTile* pTile;
	DWORD dwTime = timeGetTime();
	if (m_pMapList[cMapIndex] == NULL)									return FALSE;
	if ((m_bHeldenianWarInitiated == FALSE) && (bAutoFlag == FALSE))		return FALSE; // War must be started is player flag
	if (m_cHeldenianWinner != -1)										return FALSE; // War must not be finished
	if ((m_cHeldenianType == 1) && (m_iBTFieldMapIndex != cMapIndex))	return FALSE; // Type 1: only on BattleField
	if ((m_cHeldenianType == 2) && (m_iRampartMapIndex != cMapIndex)				  // Type 2: 
		&& (m_iGodHMapIndex != cMapIndex))                             return FALSE; // allowed only in HRampart / GodH
	if ((m_cHeldenianType == 2) 										   			  // Type 2: 
		&& (m_iGodHMapIndex == cMapIndex)											  // on GodH, allowed only
		&& (bAutoFlag == FALSE)														  // as autoflag
		&& (m_pClientList[iClientH] != NULL))
	{
		if (m_pClientList[iClientH]->m_cSide == m_sLastHeldenianWinner) return FALSE; // for attackers only
		if (m_pClientList[iClientH]->m_iGuildRank != 0)					return FALSE; // for a gm only
		if ((abs(m_pMapList[cMapIndex]->m_sHeldenianWinningZoneX - dX) > 2)
			|| (abs(m_pMapList[cMapIndex]->m_sHeldenianWinningZoneY - dY) > 2)) return FALSE; // and in wining zone only
	}
	if ((dX < 25) || (dX >= m_pMapList[cMapIndex]->m_sSizeX - 25)
		|| (dY < 25) || (dY >= m_pMapList[cMapIndex]->m_sSizeY - 25))	return FALSE;

	pTile = (class CTile*)(m_pMapList[cMapIndex]->m_pTile + dX + dY * m_pMapList[cMapIndex]->m_sSizeY);
	if (pTile->m_iAttribute != NULL)									return FALSE;
	if (pTile->m_iOccupyFlagIndex != NULL)								return FALSE;
	if (pTile->m_bIsMoveAllowed == FALSE)								return FALSE;
	if (m_pMapList[cMapIndex]->m_iTotalOccupyFlags >= DEF_MAXOCCUPYFLAG)
	{
		return FALSE;
	}
	if (bAutoFlag == FALSE) // Flag set by a character
	{
		if (m_pClientList[iClientH] == NULL) return FALSE;
		switch (m_pClientList[iClientH]->m_cSide) {
		case 1:	// Aresden
			iDynamicObjectIndex = iAddDynamicObjectList(NULL, NULL, DEF_DYNAMICOBJECT_ARESDENFLAG1, cMapIndex, dX, dY, NULL, NULL);
			break;
		case 2: // Elvine	
			iDynamicObjectIndex = iAddDynamicObjectList(NULL, NULL, DEF_DYNAMICOBJECT_ELVINEFLAG1, cMapIndex, dX, dY, NULL, NULL);
			break;
		default: // Others
			break;
		}
		if (iDynamicObjectIndex == NULL) return FALSE;
		iIndex = m_pMapList[cMapIndex]->iRegisterOccupyFlag(dX, dY, iSide, iEKNum, iDynamicObjectIndex);
		if (iIndex == -1) PutLogList("Error registering OccupyFlag.");
		for (ix = dX - iEKNum; ix <= dX + iEKNum; ix++)
			for (iy = dY - iEKNum; iy <= dY + iEKNum; iy++)
			{
				if ((ix != dX) && (iy != dY))
				{
					pTile = (class CTile*)(m_pMapList[cMapIndex]->m_pTile + ix + iy * m_pMapList[cMapIndex]->m_sSizeY);
					if (pTile->m_iOccupyFlagIndex == NULL)
					{
						switch (m_pClientList[iClientH]->m_cSide) {
						case 1:	// Aresden 
							pTile->m_iOccupyStatus -= iEKNum;
							break;
						case 2: // Elvine	
							pTile->m_iOccupyStatus += iEKNum;
							break;
						default:
							pTile->m_iOccupyStatus = 0;
							break;
						}
					}
				}
			}
		switch (m_pClientList[iClientH]->m_cSide) {
		case 1:	// Aresden 
			m_iHeldenianAresdenFlags++;
			break;
		case 2: // Elvine	
			m_iHeldenianElvineFlags++;
			break;
		}
		wsprintf(G_cTxt, "PC(%s) \tused Flag...\t%s(%d %d)"
			, m_pClientList[iClientH]->m_cCharName
			, m_pClientList[iClientH]->m_cMapName
			, dX
			, dY);
		PutItemLogFileList(G_cTxt);
		PutLogEventFileList(G_cTxt);
		PutLogList(G_cTxt);
		// Heldenian type 2 is won !
		if ((m_pClientList[iClientH]->m_iGuildRank == 0)					// only by GuildMaster
			&& (m_pClientList[iClientH]->m_cSide != m_sLastHeldenianWinner) // only by attacker side
			&& (m_iGodHMapIndex == cMapIndex))								// only on GodHMap
		{
			m_pClientList[iClientH]->m_iWarContribution += (1000 * iEKNum); // double bonus for gm putting the flag...
			HeldenianVictoryNow(m_pClientList[iClientH]->m_cSide);
		}
		// give some Summon points in case the character was killed, he will be able to catch more scrolls..
		m_pClientList[iClientH]->m_iConstructionPoint += (1000 * iEKNum);
		m_pClientList[iClientH]->m_iWarContribution += (100 * iEKNum);
		UpdateHeldenianStatus(-1);
	}
	else // bAutoFlag Flag set by server at Heldenian type 2 beginning
	{
		if (m_cHeldenianType != 2) return FALSE;
		switch (m_sLastHeldenianWinner) {
		case 1:	// Aresden:
			iDynamicObjectIndex = iAddDynamicObjectList(NULL, NULL, DEF_DYNAMICOBJECT_ARESDENFLAG1, cMapIndex, dX, dY, NULL, NULL);
			break;
		case 2: // Elvine	
			iDynamicObjectIndex = iAddDynamicObjectList(NULL, NULL, DEF_DYNAMICOBJECT_ELVINEFLAG1, cMapIndex, dX, dY, NULL, NULL);
			break;
		default: // Others
			break;
		}
		if (iDynamicObjectIndex == NULL) return FALSE;
		iIndex = m_pMapList[cMapIndex]->iRegisterOccupyFlag(dX, dY, iSide, 615, iDynamicObjectIndex);
		if (iIndex == -1) PutLogList("Error registering MasterOccupyFlag.");
		for (ix = dX - 20; ix <= dX + 35; ix++)
			for (iy = dY - 20; iy <= dY + 25; iy++)
			{
				if ((ix != dX) && (iy != dY))
				{
					pTile = (class CTile*)(m_pMapList[cMapIndex]->m_pTile + ix + iy * m_pMapList[cMapIndex]->m_sSizeY);
					if (pTile->m_iOccupyFlagIndex == NULL)
					{
						switch (m_sLastHeldenianWinner) {
						case 1:	// Aresden 
							pTile->m_iOccupyStatus -= 15;
							break;
						case 2: // Elvine	
							pTile->m_iOccupyStatus += 15;
							break;
						default:
							pTile->m_iOccupyStatus = 0;
							break;
						}
					}
				}
			}
		wsprintf(G_cTxt, "Server set Master Flag\t %s(%d %d)"
			, m_pMapList[cMapIndex]->m_cName
			, dX
			, dY);
		PutItemLogFileList(G_cTxt);
		PutLogEventFileList(G_cTxt);
		PutLogList(G_cTxt);
	}
	return TRUE;
}

/*********************************************************************************************************************
**  BOOL CGame::GetOccupyFlagHandler(int iClientH)																	**
**  description		:: Buy the occupy Flag from commander										       				**
*********************************************************************************************************************/
void CGame::GetOccupyFlagHandler(int iClientH)
{
	int   i, iNum, iRet, iEraseReq, iEKNum;
	char* cp, cData[256], cItemName[21];
	class CItem* pItem;
	DWORD* dwp;
	short* sp;
	WORD* wp;
	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_iEnemyKillCount < 10) return;
	if (m_pClientList[iClientH]->m_cSide < 0) return;
	if (m_pClientList[iClientH]->m_cSide > 2) return;

	ZeroMemory(cItemName, sizeof(cItemName));
	switch (m_pClientList[iClientH]->m_cSide) {
	case 1: strcpy(cItemName, "AresdenFlag"); break;
	case 2: strcpy(cItemName, "ElvineFlag");  break;
	}
	// ReqPurchaseItemHandler
	iNum = 1;
	for (i = 1; i <= iNum; i++)
	{
		pItem = new class CItem;
		if (_bInitItemAttr(pItem, cItemName) == FALSE)
		{
			delete pItem;
		}
		else
		{
			if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == TRUE)
			{
				if (m_pClientList[iClientH]->m_iCurWeightLoad < 0) m_pClientList[iClientH]->m_iCurWeightLoad = 0;
				if (m_pClientList[iClientH]->m_iEnemyKillCount > 10)
				{
					iEKNum = 10;
					m_pClientList[iClientH]->m_iEnemyKillCount -= 10;
				}
				else
				{
					iEKNum = m_pClientList[iClientH]->m_iEnemyKillCount;
					m_pClientList[iClientH]->m_iEnemyKillCount = 0;
				}
				// EKNum
				pItem->m_sItemSpecEffectValue2 = iEKNum;
				pItem->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
				pItem->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
				pItem->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
				pItem->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;
				//testcode 
				wsprintf(G_cTxt, "(*) Get Flag : Char(%s) Flag-EK(%d) Player-EK(%d)"
					, m_pClientList[iClientH]->m_cCharName
					, iEKNum
					, m_pClientList[iClientH]->m_iEnemyKillCount);
				PutLogList(G_cTxt);
				PutItemLogFileList(G_cTxt);
				PutLogEventFileList(G_cTxt);
				dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_ITEMOBTAINED;
				cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);
				*cp = 1;
				cp++;
				memcpy(cp, pItem->m_cName, 20);
				cp += 20;
				dwp = (DWORD*)cp;
				*dwp = pItem->m_dwCount;
				cp += 4;
				*cp = pItem->m_cItemType;
				cp++;
				*cp = pItem->m_cEquipPos;
				cp++;
				*cp = (char)0;
				cp++;
				sp = (short*)cp;
				*sp = pItem->m_sLevelLimit;
				cp += 2;
				*cp = pItem->m_cGenderLimit;
				cp++;
				wp = (WORD*)cp;
				*wp = pItem->m_wCurLifeSpan;
				cp += 2;
				wp = (WORD*)cp;
				*wp = pItem->m_wWeight;
				cp += 2;
				sp = (short*)cp;
				*sp = pItem->m_sSprite;
				cp += 2;
				sp = (short*)cp;
				*sp = pItem->m_sSpriteFrame;
				cp += 2;
				*cp = pItem->m_cItemColor;
				cp++;
				*cp = (char)pItem->m_sItemSpecEffectValue2;
				cp++;
				dwp = (DWORD*)cp;
				*dwp = pItem->m_dwAttribute;
				cp += 4;
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ENEMYKILLS, m_pClientList[iClientH]->m_iEnemyKillCount, NULL, NULL, NULL);
				if (iEraseReq == 1) delete pItem;
				iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);
				iCalcTotalWeight(iClientH);
				switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					DeleteClient(iClientH, TRUE, TRUE);
					break;
				}
			}
			else
			{
				delete pItem;
				iCalcTotalWeight(iClientH);
				dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
				iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
				switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					DeleteClient(iClientH, TRUE, TRUE);
					break;
				}
			}
		}
	}
}

int CGame::_iComposeFlagStatusContents(char* pData)
{
	SYSTEMTIME SysTime;
	char cTxt[120];
	int i;

	if (m_iMiddlelandMapIndex < 0) return 0;

	GetLocalTime(&SysTime);
	strcat(pData, "[FILE-DATE]\n\n");

	wsprintf(cTxt, "file-saved-date: %d %d %d %d %d\n", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(pData, cTxt);
	strcat(pData, "\n\n");

	// 
	for (i = 1; i < DEF_MAXOCCUPYFLAG; i++)
		if (m_pMapList[m_iMiddlelandMapIndex]->m_pOccupyFlag[i] != NULL) {

			wsprintf(cTxt, "flag = %d %d %d %d", m_pMapList[m_iMiddlelandMapIndex]->m_pOccupyFlag[i]->m_cSide,
				m_pMapList[m_iMiddlelandMapIndex]->m_pOccupyFlag[i]->m_sX,
				m_pMapList[m_iMiddlelandMapIndex]->m_pOccupyFlag[i]->m_sY,
				m_pMapList[m_iMiddlelandMapIndex]->m_pOccupyFlag[i]->m_iEKCount);
			strcat(pData, cTxt);
			strcat(pData, "\n");
		}

	strcat(pData, "\n\n");

	return strlen(pData);
}

BOOL CGame::_bDecodeOccupyFlagSaveFileContents(char* pData, DWORD dwMsgSize)
{
	char* pContents, * token;
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int  iFlagIndex = 0;
	int  iSide;
	class CStrTok* pStrTok;
	int dX, dY, iEKNum, iTotalFlags;

	if (m_iMiddlelandMapIndex < 0) {
		PutLogList("(!) This game server does not managing Middleland Map. OcuupyFlag data ignored.");
		return FALSE;
	}

	iTotalFlags = 0;

	pContents = new char[dwMsgSize + 10];
	ZeroMemory(pContents, dwMsgSize + 10);
	memcpy(pContents, pData, dwMsgSize - 9);

	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();

	while (token != NULL) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
			case 1:
				switch (cReadModeB) {
				case 1:
					// Side
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! OccupyFlag save file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					iSide = atoi(token);
					cReadModeB = 2;
					break;

				case 2:
					// X ÁÂÇ¥ 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! OccupyFlag save file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					dX = atoi(token);
					cReadModeB = 3;
					break;

				case 3:
					// Y ÁÂÇ¥  
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! OccupyFlag save file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					dY = atoi(token);
					cReadModeB = 4;
					break;

				case 4:
					// EKNum
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! OccupyFlag save file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					iEKNum = atoi(token);

					// µ¥ÀÌÅÍ¸¦ ¸ðµÎ ¸¸µé¾úÀ¸¹Ç·Î ±ê¹ßÀ» µî·ÏÇÑ´Ù. (!!! Master Flag·Î Ã³¸®ÇØ¾ß¸¸ ¼³Ä¡ÇÒ ¼ö ÀÖ´Ù)
					if (__bSetOccupyFlag(m_iMiddlelandMapIndex, dX, dY, iSide, iEKNum, -1, TRUE) == TRUE)
						iTotalFlags++;

					cReadModeA = 0;
					cReadModeB = 0;
					break;
				}

			default:
				break;
			}
		}
		else {
			if (memcmp(token, "flag", 4) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
			}
		}
		token = pStrTok->pGet();
	}

	delete pStrTok;
	delete[] pContents;

	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) CRITICAL ERROR! OccupyFlag save file contents error!");
		return FALSE;
	}

	wsprintf(G_cTxt, "(!) OccupyFlag save file decoding success! Total(%d)", iTotalFlags);
	PutLogList(G_cTxt);

	return TRUE;
}

void CGame::_DeleteRandomOccupyFlag(int iMapIndex)
{
	int i, iCount, iTotalFlags, iTargetFlag, iDynamicObjectIndex;
	int tx, ty, fx, fy, iLocalSide, iLocalEKNum, iPrevStatus;
	class CTile* pTile;
	DWORD dwTime;

	if (m_pMapList[iMapIndex] == NULL) return;

	dwTime = timeGetTime();

	// ÃÑ ±ê¹ß °¹¼ö¸¦ ±¸ÇÑ´Ù.
	iTotalFlags = 0;
	for (i = 1; i < DEF_MAXOCCUPYFLAG; i++)
		if (m_pMapList[iMapIndex]->m_pOccupyFlag[i] != NULL) {
			iTotalFlags++;
		}

	// ·£´ýÇÏ°Ô ÇÏ³ª Á¤ÇÑ´Ù.
	iTargetFlag = iDice(1, iTotalFlags);

	iCount = 0;
	for (i = 1; i < DEF_MAXOCCUPYFLAG; i++)
		if (m_pMapList[iMapIndex]->m_pOccupyFlag[i] != NULL) {
			iCount++;
			if ((iCount == iTotalFlags) && (m_pMapList[iMapIndex]->m_pOccupyFlag[i] != NULL)) {
				// ÀÌ ±ê¹ßÀ» ¾ø¾Ø´Ù.	

				//testcode
				wsprintf(G_cTxt, "(*)Delete OccupyFlag: Side(%d) XY(%d, %d)", m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_cSide, m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_sX, m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_sY);
				PutLogList(G_cTxt);

				fx = m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_sX;
				fy = m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_sY;
				iLocalSide = m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_cSide;
				iLocalEKNum = m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_iEKCount;

				pTile = (class CTile*)(m_pMapList[iMapIndex]->m_pTile + m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_sX +
					m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_sY * m_pMapList[iMapIndex]->m_sSizeY);
				// ±ê¹ß °´Ã¼ °¨¼Ò 
				m_pMapList[iMapIndex]->m_iTotalOccupyFlags--;

				iDynamicObjectIndex = m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_iDynamicObjectIndex;
				// Å¬¶óÀÌ¾ðÆ®¿¡°Ô ±ê¹ßÀÌ »ç¶óÁüÀ» ¾Ë¸®°í 
				SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_REJECT, m_pDynamicObjectList[iDynamicObjectIndex]->m_cMapIndex,
					m_pDynamicObjectList[iDynamicObjectIndex]->m_sX, m_pDynamicObjectList[iDynamicObjectIndex]->m_sY,
					m_pDynamicObjectList[iDynamicObjectIndex]->m_sType, iDynamicObjectIndex, NULL);
				// ¸Ê¿¡¼­ ±ê¹ß µ¿Àû °´Ã¼¸¦ »èÁ¦ÇÑ´Ù.
				m_pMapList[m_pDynamicObjectList[iDynamicObjectIndex]->m_cMapIndex]->SetDynamicObject(NULL, NULL, m_pDynamicObjectList[iDynamicObjectIndex]->m_sX, m_pDynamicObjectList[iDynamicObjectIndex]->m_sY, dwTime);

				// ±ê¹ß °´Ã¼¸¦ »èÁ¦ 
				delete m_pMapList[iMapIndex]->m_pOccupyFlag[i];
				m_pMapList[iMapIndex]->m_pOccupyFlag[i] = NULL;

				// Å¸ÀÏ »óÀÇ ±ê¹ß ÀÎµ¦½º¸¦ »èÁ¦
				pTile->m_iOccupyFlagIndex = NULL;

				// µ¿Àû °´Ã¼ »èÁ¦ 
				delete m_pDynamicObjectList[iDynamicObjectIndex];
				m_pDynamicObjectList[iDynamicObjectIndex] = NULL;

				// ÀÌ ±ê¹ßÀ» »èÁ¦ÇÏ¹Ç·Î ¿µÇâ±Ç ³»ÀÇ °ªÀ» º¯°æ½ÃÄÑ¾ß ÇÑ´Ù. *** ¸Ç ¸¶Áö¸·¿¡ Ã³¸®ÇØ¾ß ÇÑ´Ù.
				for (tx = fx - 12; tx <= fx + 12; tx++) // 10
					for (ty = fy - 9; ty <= fy + 9; ty++) {	 // 8
						if ((tx < 0) || (tx >= m_pMapList[iMapIndex]->m_sSizeX) ||
							(ty < 0) || (ty >= m_pMapList[iMapIndex]->m_sSizeY)) {
							// ¹«½ÃÇØ¾ß ÇÏ´Â ÁÂÇ¥ 
						}
						else {
							pTile = (class CTile*)(m_pMapList[iMapIndex]->m_pTile + tx + ty * m_pMapList[iMapIndex]->m_sSizeY);
							iPrevStatus = pTile->m_iOccupyStatus;
							// Side: ¾Æ·¹½ºµ§(1)  ¿¤¹ÙÀÎ(2)
							switch (iLocalSide) {
							case 1:
								pTile->m_iOccupyStatus += iLocalEKNum;
								if (pTile->m_iOccupyStatus > 0) pTile->m_iOccupyStatus = 0;
								break;
							case 2:
								pTile->m_iOccupyStatus -= iLocalEKNum;
								if (pTile->m_iOccupyStatus < 0) pTile->m_iOccupyStatus = 0;
								break;
							}
						}
					}
				//
				break;
			}
		}
}

/*********************************************************************************************************************
**  void CGame::GlobalEndHeldenianMode()																			**
**  description		:: Called by admin command, or winning condition			                        			**
**             		:: Type 1: Last Tower destroyed																	**
**					:: Type 2: ???																					**
*********************************************************************************************************************/
void CGame::GlobalEndHeldenianMode()
{
	char* cp, cData[32];
	DWORD* dwp;
	if (m_cHeldenianWinner == -1) return;
	if (m_bIsHeldenianMode == FALSE) return;
	ZeroMemory(cData, sizeof(cData));
	cp = (char*)cData;
	*cp = GSM_ENDHELDENIAN;
	cp++;
	dwp = (DWORD*)cp;
	*dwp = m_dwHeldenianGUID;
	cp += 4;
	dwp = (DWORD*)cp;
	*dwp = (int)m_cHeldenianWinner;
	cp += 4;
	bStockMsgToGateServer(cData, 9);
	LocalEndHeldenianMode(m_dwHeldenianGUID, (int)m_cHeldenianWinner);
}

/*********************************************************************************************************************
**  void CGame::LocalEndHeldenianMode(DWORD dwHeldenianGUID, int iWinner)											**
**  description		:: Called by global end or Gate if global end commes from another gserver             			**
**             		:: & writes the GUID file																		**
**					::																								**
*********************************************************************************************************************/
void CGame::LocalEndHeldenianMode(DWORD dwHeldenianGUID, int iWinner)
{
	if (m_bIsHeldenianMode == FALSE) return;

	m_bIsHeldenianMode = FALSE;
	m_bHeldenianWarInitiated = FALSE;
	m_cHeldenianWinner = iWinner;
	m_sLastHeldenianWinner = iWinner;
	m_dwHeldenianFinishTime = 0;
	m_dwHeldenianWarStartTime = 0;
	_CreateHeldenianGUID(m_dwHeldenianGUID, m_cHeldenianWinner);

	int m;
	int j, n;
	for (j = 0; j < DEF_MAXCLIENTS; j++)
	{
		if (m_pClientList[j] == NULL)					 continue;
		if (m_pClientList[j]->m_bIsInitComplete != TRUE) continue;
		SendNotifyMsg(NULL, j, DEF_NOTIFY_HELDENIANEND, NULL, NULL, NULL, NULL);
		// DEF_NOTIFY_HELDENIANVICTORY used at XP time
		m_pClientList[j]->m_cWarType = 0; // Client has been informed, he'll get XP
	}
	for (m = 0; m < DEF_MAXMAPS; m++)
	{
		if (m_pMapList[m] == NULL) break;
		if (m_pMapList[m]->m_bIsHeldenianMap == TRUE)
		{
			for (j = 0; j < DEF_MAXCLIENTS; j++)
			{
				if (m_pClientList[j] == NULL)					 continue;
				if (m_pClientList[j]->m_bIsInitComplete != TRUE) continue;
				if (m_pClientList[j]->m_cMapIndex != m)			 continue;
				SendNotifyMsg(NULL, j, DEF_NOTIFY_0BE8, NULL, NULL, NULL, NULL);
				RequestTeleportHandler(j, "1   ", NULL, -1, -1);
			}
			for (n = 0; n < DEF_MAXNPCS; n++)
			{
				if ((m_pNpcList[n] != NULL) && (m_pMapList[m_pNpcList[n]->m_cMapIndex] != NULL)
					&& (m_pNpcList[n]->m_cMapIndex == m))
				{
					if (m_pNpcList[n]->m_bIsSummoned == TRUE)
					{
						RemoveEventNpc(n);
					}
					else
					{
						m_pNpcList[n]->m_cSide = m_pNpcList[n]->m_cOriginalSide; // All mobs become agressive again
					}
				}
			}
			RemoveOccupyFlags(m);
		}
	}
	PutLogList("(!) Heldenian Mode OFF.");
	PutLogList(" ");
}

/*********************************************************************************************************************
**  BOOL CGame::RemoveOccupyFlags(int iMapIndex)																	**
**  description		:: Removes all occupy flags from given map										       			**
**             		:: 																								**
**	Called at Heldenian begin and Heldenian end...																	**
*********************************************************************************************************************/
void CGame::RemoveOccupyFlags(int iMapIndex)
{
	DWORD dwTime = timeGetTime();
	int i;
	short dX, dY;
	int iDynamicObjectIndex;
	class CTile* pTile;
	int iy, ix;
	if (m_pMapList[iMapIndex] == NULL) return;
	for (i = 1; i < DEF_MAXOCCUPYFLAG; i++)
	{
		if (m_pMapList[iMapIndex]->m_pOccupyFlag[i] == NULL)
		{
			continue;
		}
		else // remove this OccupyFlag
		{
			dX = m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_sX;
			dY = m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_sY;
			iDynamicObjectIndex = m_pMapList[iMapIndex]->m_pOccupyFlag[i]->m_iDynamicObjectIndex;
			m_pMapList[iMapIndex]->m_pOccupyFlag[i] = NULL;
			m_pMapList[iMapIndex]->m_iTotalOccupyFlags--;

			pTile = (class CTile*)(m_pMapList[iMapIndex]->m_pTile + dX + dY * m_pMapList[iMapIndex]->m_sSizeY);
			if (m_pDynamicObjectList[iDynamicObjectIndex] == NULL) continue;
			SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_REJECT
				, iMapIndex, dX, dY, m_pDynamicObjectList[iDynamicObjectIndex]->m_sType
				, iDynamicObjectIndex, NULL);
			m_pMapList[iMapIndex]->SetDynamicObject(NULL, NULL, dX, dY, dwTime);

			if (m_pDynamicObjectList[iDynamicObjectIndex] == NULL)
			{
				for (ix = dX - 2; ix <= dX + 2; ix++)
					for (iy = dY - 2; iy <= dY + 2; iy++)
					{
						pTile = (class CTile*)(m_pMapList[iMapIndex]->m_pTile + ix + iy * m_pMapList[iMapIndex]->m_sSizeY);
						pTile->m_sOwner = NULL;
					}
			}
		}
	}
	int j;
	for (i = 0; i < m_pMapList[iMapIndex]->m_sSizeX; i++) // Then remove all m_iOccupyStatus
		for (j = 0; j < m_pMapList[iMapIndex]->m_sSizeY; j++)
		{
			pTile = (class CTile*)(m_pMapList[iMapIndex]->m_pTile + i + j * m_pMapList[iMapIndex]->m_sSizeY);
			pTile->m_iOccupyStatus = 0;
		}
}

/*********************************************************************************************************************
**  void CGame::_CreateHeldenianGUID(DWORD dwHeldenianGUID, int iWinnerSide)										**
**  description		:: Create GUID file at event's strating, & finishing for each server	              			**
**             		:: Used when server starts																		**
**																													**
*********************************************************************************************************************/
void CGame::_CreateHeldenianGUID(DWORD dwHeldenianGUID, int iWinnerSide)
{
	char* cp, cTxt[256], cTemp[1024];
	FILE* pFile;
	pFile = fopen("..\\GameConfigs\\HeldenianGUID.Txt", "wt");
	if (pFile == NULL)
	{
		wsprintf(G_cTxt, "(!) Cannot create HeldenianGUID (%d) file.", dwHeldenianGUID);
		PutLogList(G_cTxt);
	}
	else
	{
		ZeroMemory(cTemp, sizeof(cTemp));
		ZeroMemory(cTxt, sizeof(cTxt));
		wsprintf(cTxt, "HeldenianGUID = %d\n", dwHeldenianGUID);
		strcat(cTemp, cTxt);
		ZeroMemory(cTxt, sizeof(cTxt));
		wsprintf(cTxt, "winner-side = %d\n", iWinnerSide);
		strcat(cTemp, cTxt);
		cp = (char*)cTemp;
		fwrite(cp, strlen(cp), 1, pFile);
		wsprintf(G_cTxt, "(!) HeldenianGUID (%d) file created. (Winner %d)", dwHeldenianGUID, iWinnerSide);
		PutLogList(G_cTxt);
		fclose(pFile);
	}
}

/*********************************************************************************************************************
**  BOOL CGame::bCheckHeldenianMap(int sAttackerH, int iMapIndex, char cType)										**
**  description			:: checks if the player is in the specified map												**
**  last updated		:: November 22, 2004; 6:19 PM; Hypnotoad													**
**	return value		:: BOOL																						**
**  commentary			::	-	translated from scratch using IDA Pro												**
**						::	-	changed pTile->m_cOwner to m_iOccupyStatus											**
**						::	-	added check to prevent access violation if pTile == NULL							**
**						::	-	removed 4 return(s) after "iRet = 1;" and placed at end								**
*********************************************************************************************************************/
BOOL CGame::bCheckHeldenianMap(int sAttackerH, char cType)
{
	if (m_bHeldenianWarInitiated == FALSE) return FALSE;
	short tX, tY;
	BOOL iRet;
	int  iMapIndex;
	class CTile* pTile;
	iRet = FALSE;
	if (cType == DEF_OWNERTYPE_PLAYER)
	{
		if (m_pClientList[sAttackerH] == NULL)					return FALSE;
		if (m_pClientList[sAttackerH]->m_cSide < 1)				return FALSE;
		if (m_pClientList[sAttackerH]->m_cSide > 2)				return FALSE;
		iMapIndex = m_pClientList[sAttackerH]->m_cMapIndex;
		if (m_pMapList[iMapIndex]->m_bIsHeldenianMap != TRUE)	return FALSE;
		if (m_iGodHMapIndex == iMapIndex)						return FALSE;
		tX = m_pClientList[sAttackerH]->m_sX;
		tY = m_pClientList[sAttackerH]->m_sY;
		if ((tX <= 0) || (tX >= m_pMapList[iMapIndex]->m_sSizeX)
			|| (tY <= 0) || (tY >= m_pMapList[iMapIndex]->m_sSizeY)) return FALSE;
		pTile = (class CTile*)(m_pMapList[iMapIndex]->m_pTile + tX + tY * m_pMapList[iMapIndex]->m_sSizeY);
		if (pTile == NULL) return FALSE;
		if (pTile->m_iOccupyStatus != NULL)
		{
			if (pTile->m_iOccupyStatus < 0)
			{
				if (m_pClientList[sAttackerH]->m_cSide == 1)
				{
					iRet = TRUE;
				}
			}
			else if (pTile->m_iOccupyStatus > 0)
			{
				if (m_pClientList[sAttackerH]->m_cSide == 2)
				{
					iRet = TRUE;
				}
			}
		}
	}
	else if (cType == DEF_OWNERTYPE_NPC)
	{
		if (m_pNpcList[sAttackerH] == NULL)						return FALSE;
		if (m_pNpcList[sAttackerH]->m_cSide < 1)				return FALSE;
		if (m_pNpcList[sAttackerH]->m_cSide > 2)				return FALSE;
		iMapIndex = m_pNpcList[sAttackerH]->m_cMapIndex;
		if (m_pMapList[iMapIndex]->m_bIsHeldenianMap != TRUE)	return FALSE;
		if (m_iGodHMapIndex == iMapIndex)						return FALSE;
		tX = m_pNpcList[sAttackerH]->m_sX;
		tY = m_pNpcList[sAttackerH]->m_sY;
		if ((tX <= 0) || (tX >= m_pMapList[iMapIndex]->m_sSizeX)
			|| (tY <= 0) || (tY >= m_pMapList[iMapIndex]->m_sSizeY)) return FALSE;
		pTile = (class CTile*)(m_pMapList[iMapIndex]->m_pTile + tX + tY * m_pMapList[iMapIndex]->m_sSizeY);
		if (pTile == NULL) return FALSE;
		if (pTile->m_iOccupyStatus != NULL)
		{
			if (pTile->m_iOccupyStatus < 0)
			{
				if (m_pNpcList[sAttackerH]->m_cSide == 1)
				{
					iRet = TRUE;
				}
			}
			else if (pTile->m_iOccupyStatus > 0)
			{
				if (m_pNpcList[sAttackerH]->m_cSide == 2)
				{
					iRet = TRUE;
				}
			}
		}
	}
	return iRet;
}

/*********************************************************************************************************************
**  BOOL CGame::RequestHeldenianTeleportList(int iClientH, char * pData, DWORD dwMsgSize)							**
**  description		:: A player requested to go Heldenian maps...										 			**
*********************************************************************************************************************/
void CGame::RequestHeldenianTeleportList(int iClientH, char* pData, DWORD dwMsgSize)
{
	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;
	if (m_pClientList[iClientH]->m_bIsKilled == TRUE) return;
	if (m_pClientList[iClientH]->m_bIsOnWaitingProcess == TRUE) return;
	if (m_pClientList[iClientH]->m_iLockedMapTime != NULL)
	{
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_LOCKEDMAP, m_pClientList[iClientH]->m_iLockedMapTime, NULL, NULL, m_pClientList[iClientH]->m_cLockedMapName);
		return;
	}
	// Prevent hack use in target maps
	if ((m_iBTFieldMapIndex != -1) && (m_pClientList[iClientH]->m_cMapIndex == m_iBTFieldMapIndex)) return;
	if ((m_iGodHMapIndex != -1) && (m_pClientList[iClientH]->m_cMapIndex == m_iGodHMapIndex)) return;
	char* cp, cData[512];
	int		iRet;
	DWORD* dwp;
	WORD* wp;
	int* listCount;
	char	cNpcName[21];
	int* ip;
	int		index = 0;
	int		m_iDestinationX, m_iDestinationY, m_iCost;
	char	m_cTargetMap[11];
	cp = (char*)(pData + DEF_INDEX2_MSGTYPE + 2);
	ZeroMemory(cNpcName, sizeof(cNpcName));
	strncpy(cNpcName, cp, 20);
	cp += 20;
	ZeroMemory(cData, sizeof(cData));
	dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_HELDENIAN_TP_LIST;
	wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
	*wp = DEF_MSGTYPE_CONFIRM;
	cp = cData + 6;
	listCount = (int*)cp;
	*listCount = 0;
	cp += 4; // sizeof(int)
	// Message must be sent by Gail...
	if (strcmp(cNpcName, "Gail") != 0) return;
	if ((m_bIsHeldenianMode == TRUE)
		&& (m_pClientList[iClientH]->m_bIsPlayerCivil != TRUE)
		&& (m_cHeldenianWinner == -1) // Not possible to TP if already won
		&& ((m_pClientList[iClientH]->m_cSide == 2) || (m_pClientList[iClientH]->m_cSide == 1)))
	{
		if (m_cHeldenianType == 2)
		{
			ZeroMemory(m_cTargetMap, sizeof(m_cTargetMap));
			memcpy(m_cTargetMap, "HRampart", 8);
			if (m_pClientList[iClientH]->m_cSide == m_sLastHeldenianWinner)
			{
				m_iDestinationX = 81;
				m_iDestinationY = 42;
				m_iCost = 0;
				index = 3;
			}
			else
			{
				m_iDestinationX = 156;
				m_iDestinationY = 153;
				m_iCost = 0;
				index = 4;
			}
		}
		else // Players can go fight Heldenian
		{
			ZeroMemory(m_cTargetMap, sizeof(m_cTargetMap));
			memcpy(m_cTargetMap, "BtField", 7);
			if (m_pClientList[iClientH]->m_cSide == 1)
			{
				m_iDestinationX = 68;
				m_iDestinationY = 225;
				m_iCost = 0;
				index = 1;
			}
			else
			{
				m_iDestinationX = 202;
				m_iDestinationY = 70;
				m_iCost = 0;
				index = 2;
			}
		}
		ip = (int*)cp;
		*ip = index;
		cp += 4;
		memcpy(cp, m_cTargetMap, 10);
		cp += 10;
		ip = (int*)cp;
		*ip = m_iDestinationX;
		cp += 4;
		ip = (int*)cp;
		*ip = m_iDestinationY;
		cp += 4;
		ip = (int*)cp;
		*ip = m_iCost;
		cp += 4;
		(*listCount)++;
	}
	if ((m_bIsHeldenianMode == FALSE)
		&& (m_pClientList[iClientH]->m_bIsPlayerCivil != TRUE)
		&& (m_sLastHeldenianWinner == m_pClientList[iClientH]->m_cSide))
		// Winners of previous Heldenian can go to BattleField all the time
	{
		ZeroMemory(m_cTargetMap, sizeof(m_cTargetMap));
		memcpy(m_cTargetMap, "BtField", 7);
		if (m_pClientList[iClientH]->m_cSide == 1)
		{
			m_iDestinationX = -1;
			m_iDestinationY = -1;
			m_iCost = 4000;
			index = 11;
		}
		else
		{
			m_iDestinationX = -1;
			m_iDestinationY = -1;
			m_iCost = 4000;
			index = 12;
		}
		ip = (int*)cp;
		*ip = index;
		cp += 4;
		memcpy(cp, m_cTargetMap, 10);
		cp += 10;
		ip = (int*)cp;
		*ip = m_iDestinationX;
		cp += 4;
		ip = (int*)cp;
		*ip = m_iDestinationY;
		cp += 4;
		ip = (int*)cp;
		*ip = m_iCost;
		cp += 4;
		(*listCount)++;
		ZeroMemory(m_cTargetMap, sizeof(m_cTargetMap));
		memcpy(m_cTargetMap, "middleland", 10);
		m_iDestinationX = 388 + iDice(1, 10);
		m_iDestinationY = 262 + iDice(1, 10);
		m_iCost = 1000;
		index = 10;
		ip = (int*)cp;
		*ip = index;
		cp += 4;
		memcpy(cp, m_cTargetMap, 10);
		cp += 10;
		ip = (int*)cp;
		*ip = m_iDestinationX;
		cp += 4;
		ip = (int*)cp;
		*ip = m_iDestinationY;
		cp += 4;
		ip = (int*)cp;
		*ip = m_iCost;
		cp += 4;
		(*listCount)++;
	}
	if ((*listCount) == 0)*wp = DEF_MSGTYPE_REJECT;
	iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 10 + (*listCount) * 26);
	switch (iRet) {
	case DEF_XSOCKEVENT_QUENEFULL:
	case DEF_XSOCKEVENT_SOCKETERROR:
	case DEF_XSOCKEVENT_CRITICALERROR:
	case DEF_XSOCKEVENT_SOCKETCLOSED:
		DeleteClient(iClientH, TRUE, TRUE);
		break;
	}
}
/*********************************************************************************************************************
**  BOOL CGame::RequestHeldenianTeleportNow(int iClientH, char * pData, DWORD dwMsgSize)							**
**  description		:: Suposed to be free TP to ML for heldenian winners								 			**
**  TP is available from Gail, here, but winner TP is also available at CH for higher price							**
*********************************************************************************************************************/
void CGame::RequestHeldenianTeleportNow(int iClientH, char* pData, DWORD dwMsgSize)
{
	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;
	if (m_pClientList[iClientH]->m_bIsKilled == TRUE) return;
	if (m_pClientList[iClientH]->m_bIsOnWaitingProcess == TRUE) return;
	int		m_iDestinationX, m_iDestinationY, m_iCost;
	char	m_cTargetMap[11];
	ZeroMemory(m_cTargetMap, sizeof(m_cTargetMap));
	char* cp, cData[64];
	int		iRet;
	DWORD* dwp;
	WORD* wp;
	int		index;
	WORD	wConfirm = DEF_MSGTYPE_CONFIRM;
	short	sError = 0;
	// CHARGED_TELEPORT index
	cp = (char*)(pData + DEF_INDEX2_MSGTYPE + 2);
	index = (int)(*cp);
	cp += 4;
	switch (index) {
	default: // Other Heldenian TP  (hack?)
		wConfirm = DEF_MSGTYPE_REJECT;
		sError = 7;
		break;
	case 1: // Ares TP to BtField, Heldenian time	
		if ((m_bIsHeldenianMode == FALSE) || (m_cHeldenianType != 1))
		{
			wConfirm = DEF_MSGTYPE_REJECT;
			sError = 7;
		}
		else
		{
			memcpy(m_cTargetMap, "BtField", 7);
			m_iDestinationX = 68;
			m_iDestinationY = 225;
			m_iCost = 0;
		}
		break;
	case 2: // Elvi TP to BtField, Heldenian time	
		if ((m_bIsHeldenianMode == FALSE) || (m_cHeldenianType != 1))
		{
			wConfirm = DEF_MSGTYPE_REJECT;
			sError = 7;
		}
		else
		{
			memcpy(m_cTargetMap, "BtField", 7);
			m_iDestinationX = 202;
			m_iDestinationY = 70;
			m_iCost = 0;
		}
		break;
	case 3: // Winners TP to Rampart, Heldenian time
	case 4: // Loosers TP to Rampart, Heldenian time	
		if ((m_bIsHeldenianMode == FALSE) || (m_cHeldenianType != 2))
		{
			wConfirm = DEF_MSGTYPE_REJECT;
			sError = 7;
		}
		else
		{
			memcpy(m_cTargetMap, "HRampart", 8);
			if (m_pClientList[iClientH]->m_cSide == m_sLastHeldenianWinner)
			{
				m_iDestinationX = 81;
				m_iDestinationY = 42;
				m_iCost = 0;
			}
			else
			{
				m_iDestinationX = 156;
				m_iDestinationY = 153;
				m_iCost = 0;
			}
		}
		break;
	case 10: // Winner TP to ML (near TOH)		
		if ((m_bIsHeldenianMode == TRUE)
			|| (m_pClientList[iClientH]->m_bIsPlayerCivil == TRUE)
			|| (m_sLastHeldenianWinner != m_pClientList[iClientH]->m_cSide))
		{
			wConfirm = DEF_MSGTYPE_REJECT;
			sError = 7;
		}
		else
		{
			memcpy(m_cTargetMap, "middleland", 10);
			m_iDestinationX = 388 + iDice(1, 10);
			m_iDestinationY = 262 + iDice(1, 10);
			m_iCost = 1000;
		}
		break;
	case 11: // Winner Ares TP to BtField	
	case 12: // Winner Elvi TP to BtField
		if ((m_bIsHeldenianMode == TRUE)
			|| (m_pClientList[iClientH]->m_bIsPlayerCivil == TRUE)
			|| (m_sLastHeldenianWinner != m_pClientList[iClientH]->m_cSide))
		{
			wConfirm = DEF_MSGTYPE_REJECT;
			sError = 7;
		}
		else
		{
			memcpy(m_cTargetMap, "BtField", 7);
			if (m_pClientList[iClientH]->m_cSide == 1)
			{
				m_iDestinationX = -1;
				m_iDestinationY = -1;
				m_iCost = 4000;
			}
			else if (m_pClientList[iClientH]->m_cSide == 2)
			{
				m_iDestinationX = -1;
				m_iDestinationY = -1;
				m_iCost = 4000;
			}
			else
			{
				wConfirm = DEF_MSGTYPE_REJECT;
				sError = 7;
			}
		}
		break;
	}
	if (wConfirm == DEF_MSGTYPE_CONFIRM)
	{
		DWORD dwGoldCount = dwGetItemCount(iClientH, "Gold");
		if (dwGoldCount >= m_iCost)
		{
			int iGoldWeight = SetItemCount(iClientH, "Gold", dwGoldCount - m_iCost);
			iCalcTotalWeight(iClientH);
		}
		else
		{
			wConfirm = DEF_MSGTYPE_REJECT;
			sError = 6;
		}
	}
	// Invalid Charged-teleport
	if (wConfirm == DEF_MSGTYPE_REJECT)
	{
		ZeroMemory(cData, sizeof(cData));
		dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_RESPONSE_CHARGED_TELEPORT;
		wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
		*wp = wConfirm;
		cp = cData + DEF_INDEX2_MSGTYPE + 2;
		short* sp;
		sp = (short*)cp;
		*sp = sError;
		cp += 2;
		// Teleport List
		iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6 + 2);
		switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			DeleteClient(iClientH, TRUE, TRUE);
			break;
		}
		return;
	}
	// teleport
	RequestTeleportHandler(iClientH, "2   ", m_cTargetMap, m_iDestinationX, m_iDestinationY);
}

/*********************************************************************************************************************
**  void CGame::GlobalStartHeldenianMode()																			**
**  description		:: Tell Gate server to ask all gservers to start Heldenian                           			**
**             		:: and starts Heldenian localy																	**
**	Called by ManualStartHeldenianMode																				**
*********************************************************************************************************************/
void CGame::GlobalStartHeldenianMode()
{
	char cData[120], * cp;
	DWORD dwNewHeldenianGUID, * dwp;
	WORD* wp;
	dwNewHeldenianGUID = timeGetTime();
	ZeroMemory(cData, sizeof(cData));
	cp = (char*)cData;
	*cp = GSM_STARTHELDENIAN; // 21
	cp++;
	wp = (WORD*)cp;
	*wp = m_cHeldenianType;
	cp += 2;
	wp = (WORD*)cp;
	*wp = m_sLastHeldenianWinner;
	cp += 2;
	dwp = (DWORD*)cp;
	*dwp = dwNewHeldenianGUID;
	cp += 4;
	bStockMsgToGateServer(cData, 9);
	LocalStartHeldenianMode(m_cHeldenianType, m_sLastHeldenianWinner, dwNewHeldenianGUID);
}

/*********************************************************************************************************************
**  void CGame::LocalStartHeldenianMode(short sV1, short sV2, DWORD dwHeldenianGUID)								**
**  description		:: localy performs all required tasks upon recieving heldenian initiate message					**
**					::																								**
**	Called locally if Heldenian started from this serger, or called by gate message									**
*********************************************************************************************************************/
void CGame::LocalStartHeldenianMode(short sV1, short sV2, DWORD dwHeldenianGUID)
{
	int i, x, iNamingValue;
	char cName[11], cTmp[21], cNpcWaypointIndex[10], cSide, cOwnerType;
	short sOwnerH;
	BOOL bRet;
	int dX, dY;
	if (m_bIsHeldenianMode == TRUE) return;
	m_cHeldenianType = sV1;
	m_sLastHeldenianWinner = sV2;
	m_cHeldenianWinner = -1;
	m_dwHeldenianGUID = dwHeldenianGUID;
	m_bHeldenianWarInitiated = FALSE;
	_CreateHeldenianGUID(m_dwHeldenianGUID, m_sLastHeldenianWinner);
	m_iHeldenianAresdenLeftTower = 0;
	m_iHeldenianElvineLeftTower = 0;
	m_iHeldenianAresdenDead = 0;
	m_iHeldenianElvineDead = 0;

	// Tell that you can TP to battlefield, and about construction points
	for (i = 0; i < DEF_MAXCLIENTS; i++)
	{
		if (m_pClientList[i] != NULL)
		{
			if (m_pClientList[i]->m_bIsInitComplete != TRUE) break;
			m_pClientList[i]->m_cWarType = 2;
			SendNotifyMsg(NULL, i, DEF_NOTIFY_HELDENIANTELEPORT, NULL, NULL, NULL, NULL); // You can now, go to the battle field
			m_pClientList[i]->m_dwHeldenianGUID = m_dwHeldenianGUID;
			m_pClientList[i]->m_iWarContribution = 0;
			m_pClientList[i]->m_iConstructionPoint = 10000 + (m_pClientList[i]->m_iCharisma * 100);
			m_pClientList[i]->m_cWarType = 2;// character have been informed of heldenian starting...
			SendNotifyMsg(NULL, i, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[i]->m_iConstructionPoint, m_pClientList[i]->m_iWarContribution, 0, NULL); //0: Tell player of acquired points
			UpdateHeldenianStatus(i);
		}
	}
	// Recall clients already on battlefield; remove any Heldenian mob, and create current war towers/gates
	for (x = 0; x < DEF_MAXMAPS; x++)
	{
		if (m_pMapList[x] == NULL) break;
		if (m_pMapList[x]->m_bIsHeldenianMap == TRUE)
		{
			RemoveOccupyFlags(x);
			for (i = 0; i < DEF_MAXCLIENTS; i++) // Tell everybody on Heldenian map and recall them
			{
				if ((m_pClientList[i] != NULL) && (m_pClientList[i]->m_bIsInitComplete == TRUE)
					&& (m_pClientList[i]->m_cMapIndex == x))
				{
					SendNotifyMsg(NULL, i, DEF_NOTIFY_0BE8, NULL, NULL, NULL, NULL);
					RequestTeleportHandler(i, "1   ", NULL, -1, -1);
				}
			}
			for (i = 0; i < DEF_MAXNPCS; i++) // Remove summons from last Heldenian
			{
				if ((m_pNpcList[i] != NULL) && (m_pNpcList[i]->m_bIsKilled == FALSE)
					&& (m_pNpcList[i]->m_cMapIndex == x))
				{
					if (m_pNpcList[i]->m_bIsSummoned == TRUE)
					{
						RemoveEventNpc(i);
					}
					else
					{	// Remove most mobs, and other Mobs become neutral
						// All mobs become neutral as players don't have spells or attacks...
						if (iDice(1, 10) != 5)
						{
							m_pNpcList[i]->m_bIsUnsummoned = TRUE;
							RemoveEventNpc(i);
						}
						else m_pNpcList[i]->m_cSide = 0;
					}
				}
			}
			if (m_cHeldenianType == 1)
			{
				if (strcmp(m_pMapList[x]->m_cName, "BtField") == 0)
				{
					for (i = 0; i < MAX_HELDENIANTOWER; i++)
					{
						iNamingValue = m_pMapList[x]->iGetEmptyNamingValue();
						if (m_pMapList[x]->m_stHeldenianTower[i].sTypeID < 1)  break;
						if (m_pMapList[x]->m_stHeldenianTower[i].sTypeID > DEF_MAXNPCTYPES) break;
						if (iNamingValue != -1)
						{
							dX = m_pMapList[x]->m_stHeldenianTower[i].dX;
							dY = m_pMapList[x]->m_stHeldenianTower[i].dY;
							cSide = m_pMapList[x]->m_stHeldenianTower[i].cSide;
							ZeroMemory(cTmp, sizeof(cTmp));
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID == 87) && (cSide == 1)) strcpy(cTmp, "CT-Aresden");
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID == 87) && (cSide == 2)) strcpy(cTmp, "CT-Elvine");
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID == 89) && (cSide == 1)) strcpy(cTmp, "AGC-Aresden");
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID == 89) && (cSide == 2)) strcpy(cTmp, "AGC-Elvine");
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID == 36) && (cSide == 1)) strcpy(cTmp, "AGT-Aresden");
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID == 36) && (cSide == 2)) strcpy(cTmp, "AGT-Elvine");
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID == 37) && (cSide == 1)) strcpy(cTmp, "CGT-Aresden");
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID == 37) && (cSide == 2)) strcpy(cTmp, "CGT-Elvine");
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID == 39) && (cSide == 1)) strcpy(cTmp, "DT-Aresden");
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID == 39) && (cSide == 2)) strcpy(cTmp, "DT-Elvine");
							ZeroMemory(cName, sizeof(cName));
							wsprintf(cName, "XX%d", iNamingValue);
							cName[0] = 95;
							cName[1] = i + 65;
							bRet = bCreateNewNpc(cTmp, cName, m_pMapList[x]->m_cName, (rand() % 3), 0, DEF_MOVETYPE_RANDOM, &dX, &dY, cNpcWaypointIndex, NULL, NULL, cSide, FALSE, TRUE, FALSE, TRUE, NULL);
							if (bRet == FALSE)
							{
								m_pMapList[x]->SetNamingValueEmpty(iNamingValue);
							}
							else
							{
								m_pMapList[x]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
								if ((m_pNpcList[sOwnerH] != NULL) && (sOwnerH > 0) && (sOwnerH < DEF_MAXNPCS))
								{
									m_pNpcList[sOwnerH]->m_iBuildCount = 0;
									m_pNpcList[sOwnerH]->m_sAppr2 = 0;
									m_pNpcList[sOwnerH]->m_cSide = cSide;
								}
								if (cSide == 1)	m_iHeldenianAresdenLeftTower++;
								if (cSide == 2) m_iHeldenianElvineLeftTower++;
							}
						}
					}
					wsprintf(G_cTxt, "HeldenianAresdenLeftTower : %d , HeldenianElvineLeftTower : %d", m_iHeldenianAresdenLeftTower, m_iHeldenianElvineLeftTower);
					PutLogList(G_cTxt);
					PutLogEventFileList(G_cTxt);
					UpdateHeldenianStatus(-1);
				}
			}
			else if (m_cHeldenianType == 2)
			{	// Set master flag
				if (strcmp(m_pMapList[x]->m_cName, "GodH") == 0)
				{
					__bSetOccupyFlag(x, m_pMapList[x]->m_sHeldenianWinningZoneX, m_pMapList[x]->m_sHeldenianWinningZoneY
						, m_sLastHeldenianWinner, 616, -1, TRUE);
				}
				if (strcmp(m_pMapList[x]->m_cName, "HRampart") == 0)
				{	// Possibilty to set towers, detectors etc....
					for (i = 0; i < MAX_HELDENIANTOWER; i++)
					{
						if (m_pMapList[x]->m_stHeldenianTower[i].sTypeID < 1)  break;
						if (m_pMapList[x]->m_stHeldenianTower[i].sTypeID > DEF_MAXNPCTYPES) break;
						iNamingValue = m_pMapList[x]->iGetEmptyNamingValue();
						if (iNamingValue != -1)
						{
							dX = m_pMapList[x]->m_stHeldenianTower[i].dX;
							dY = m_pMapList[x]->m_stHeldenianTower[i].dY;
							cSide = m_sLastHeldenianWinner;
							ZeroMemory(cTmp, sizeof(cTmp));
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID == 87) && (cSide == 1)) strcpy(cTmp, "CT-Aresden");
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID == 87) && (cSide == 2)) strcpy(cTmp, "CT-Elvine");
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID == 89) && (cSide == 1)) strcpy(cTmp, "AGC-Aresden");
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID == 89) && (cSide == 2)) strcpy(cTmp, "AGC-Elvine");
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID == 36) && (cSide == 1)) strcpy(cTmp, "AGT-Aresden");
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID == 36) && (cSide == 2)) strcpy(cTmp, "AGT-Elvine");
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID == 37) && (cSide == 1)) strcpy(cTmp, "CGT-Aresden");
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID == 37) && (cSide == 2)) strcpy(cTmp, "CGT-Elvine");
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID == 39) && (cSide == 1)) strcpy(cTmp, "DT-Aresden");
							if ((m_pMapList[x]->m_stHeldenianTower[i].sTypeID == 39) && (cSide == 2)) strcpy(cTmp, "DT-Elvine");
							ZeroMemory(cName, sizeof(cName));
							wsprintf(cName, "XX%d", iNamingValue);
							cName[0] = 95;
							cName[1] = i + 65;
							bRet = bCreateNewNpc(cTmp, cName, m_pMapList[x]->m_cName, (rand() % 3), 0, DEF_MOVETYPE_RANDOM, &dX, &dY, cNpcWaypointIndex, NULL, NULL, cSide, FALSE, TRUE, FALSE, TRUE, NULL);
							if (bRet == FALSE)
							{
								m_pMapList[x]->SetNamingValueEmpty(iNamingValue);
							}
							else
							{
								m_pMapList[x]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
								if ((m_pNpcList[sOwnerH] != NULL) && (sOwnerH > 0) && (sOwnerH < DEF_MAXNPCS))
								{
									m_pNpcList[sOwnerH]->m_iBuildCount = 0;
									m_pNpcList[sOwnerH]->m_sAppr2 = 0;
									m_pNpcList[sOwnerH]->m_cSide = cSide;
								}
							}
						}
					}
					// Set Gates
					for (i = 0; i < DEF_MAXHELDENIANDOOR; i++)
					{
						if (m_pMapList[x]->m_stHeldenianGateDoor[i].dX < 1)  break;
						iNamingValue = m_pMapList[x]->iGetEmptyNamingValue();
						if (iNamingValue != -1)
						{
							dX = m_pMapList[x]->m_stHeldenianGateDoor[i].dX;
							dY = m_pMapList[x]->m_stHeldenianGateDoor[i].dY;
							ZeroMemory(cTmp, sizeof(cTmp));
							if (m_sLastHeldenianWinner == 1) strcpy(cTmp, "gate-a");
							else							 strcpy(cTmp, "gate-e");
							cSide = m_sLastHeldenianWinner;
							ZeroMemory(cName, sizeof(cName));
							wsprintf(cName, "XX%d", iNamingValue);
							cName[0] = 95;
							cName[1] = i + 65;
							bRet = bCreateNewNpc(cTmp, cName, m_pMapList[x]->m_cName, (rand() % 3), 0, DEF_MOVETYPE_RANDOM, &dX, &dY, cNpcWaypointIndex, NULL, NULL, m_sLastHeldenianWinner, FALSE, TRUE, FALSE, TRUE, NULL);
							if (bRet == FALSE)
							{
								m_pMapList[x]->SetNamingValueEmpty(iNamingValue);
							}
							else
							{
								m_pMapList[x]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
								if ((m_pNpcList[sOwnerH] != NULL) && (sOwnerH > 0) && (sOwnerH < DEF_MAXNPCS))
								{
									m_pNpcList[sOwnerH]->m_iBuildCount = 0;
									m_pNpcList[sOwnerH]->m_cSide = cSide;
									m_pNpcList[sOwnerH]->m_cDir = m_pMapList[x]->m_stHeldenianGateDoor[i].cDir;
									wsprintf(G_cTxt, "(!) Gate installed at %d,%d", dX, dY);
									PutLogList(G_cTxt);
								}
							}
						}
					}
				}
			}
		}
	}
	m_bIsHeldenianMode = TRUE;
	m_dwHeldenianFinishTime = timeGetTime() + m_dwHeldenianEndTime;  // war will end   in 1h45 
	m_dwHeldenianWarStartTime = timeGetTime() + m_dwHeldenianWarTime;  // war will start in 0h15
	wsprintf(G_cTxt, "(!) Heldenian mode ON. (type %d, last winner:%d)", m_cHeldenianType, m_sLastHeldenianWinner);
	PutLogList(G_cTxt);
	PutLogList(" ");
	PutLogEventFileList(G_cTxt);
	PutLogEventFileList(" ");
}

/*********************************************************************************************************************
**  void CGame::ManualStartHeldenianMode(int iClientH, char *pData, DWORD dwMsgSize)								**
**  description		:: Admin order to star Heldenian, starts heldenian			                        			**
**             		:: "/beginheldenian 1" or "/beginheldenian 2"													**
**	Called by Amin order																							**
*********************************************************************************************************************/
void CGame::ManualStartHeldenianMode(int iClientH, char* pData, DWORD dwMsgSize)
{
	char cBuff[256], * token, seps[] = "= \t\n";
	class CStrTok* pStrTok;
	if (m_bIsHeldenianMode == TRUE) return;
	if (m_bIsApocalypseMode == TRUE) return;
	if (m_bIsCrusadeMode == TRUE) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < 3)
	{
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ADMINUSERLEVELLOW, NULL, NULL, NULL, NULL);
		return;
	}
	if (m_pClientList[iClientH]->m_bIsAdminCommandEnabled == FALSE) return;

	ZeroMemory(cBuff, sizeof(cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	m_cHeldenianType = 1;
	if (token != NULL)
	{
		switch (atoi(token)) {
		case 2:
			m_cHeldenianType = 2;
			break;
		default:
			m_cHeldenianType = 1;
			break;
		}
	}
	if (m_sLastHeldenianWinner == 0) m_cHeldenianType = 1;
	delete pStrTok;
	wsprintf(G_cTxt, "Admin Order(%s): begin Heldenian", m_pClientList[iClientH]->m_cCharName);
	PutLogList(G_cTxt);
	GlobalStartHeldenianMode();
}

/*********************************************************************************************************************
**  void CGame::ManualEndHeldenianMode(int iClientH, char *pData, DWORD dwMsgSize)									**
**  description		:: Called by Amin order, stops heldenian					                        			**
**             		:: "/endheldenian 0" or"/endheldenian 1" or "/endheldenian 2"									**
**																													**
*********************************************************************************************************************/
void CGame::ManualEndHeldenianMode(int iClientH, char* pData, DWORD dwMsgSize)
{
	char	seps[] = "= \t\n";
	char* token, cBuff[256];
	class	CStrTok* pStrTok;
	if ((dwMsgSize) <= 0) return;
	if (iClientH == NULL) return;
	if (m_bIsHeldenianMode == FALSE) return;
	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel < 3)
	{
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ADMINUSERLEVELLOW, NULL, NULL, NULL, NULL);
		return;
	}
	if (m_pClientList[iClientH]->m_bIsAdminCommandEnabled == FALSE) return;

	ZeroMemory(cBuff, sizeof(cBuff));
	memcpy(cBuff, pData, dwMsgSize);
	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();
	token = pStrTok->pGet();
	if (token == NULL) // draw
	{
		m_cHeldenianWinner = 0;
		wsprintf(G_cTxt, "GM Order(%s): Heldenian terminated, Draw", m_pClientList[iClientH]->m_cCharName);
	}
	else if (token != NULL)
	{
		if (_bGetIsStringIsNumber(token) == TRUE)
		{
			switch (atoi(token)) {
			case 1:
				m_cHeldenianWinner = 1;
				wsprintf(G_cTxt, "GM Order(%s): Heldenian terminated, Aresden victory", m_pClientList[iClientH]->m_cCharName);
				break;
			case 2:
				m_cHeldenianWinner = 2;
				wsprintf(G_cTxt, "GM Order(%s): Heldenian terminated, Elvine victory", m_pClientList[iClientH]->m_cCharName);
				break;
			default:
				m_cHeldenianWinner = 0;
				wsprintf(G_cTxt, "GM Order(%s): Heldenian terminated, Draw", m_pClientList[iClientH]->m_cCharName);
				break;
			}
		}
		else
		{
			m_cHeldenianWinner = 0;
			wsprintf(G_cTxt, "GM Order(%s): Heldenian terminated, Draw", m_pClientList[iClientH]->m_cCharName);
		}
	}
	PutLogList(G_cTxt);
	GlobalEndHeldenianMode();
	delete pStrTok;
}

void CGame::CheckHeldenianResultCalculation(int iClientH)
{
	double dV1, dV2, dV3;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_cWarType != 2) return;
	if ((m_cHeldenianType == 0) || (m_pClientList[iClientH]->m_dwHeldenianGUID == 0)) return;
	if (m_pClientList[iClientH]->m_dwHeldenianGUID == m_dwHeldenianGUID) {
		if (m_pClientList[iClientH]->m_cSide == m_sLastHeldenianWinner) {
			if (m_pClientList[iClientH]->m_iLevel <= 80) {
				m_pClientList[iClientH]->m_iWarContribution += (m_pClientList[iClientH]->m_iLevel + m_pClientList[iClientH]->m_iMajesticLevel) * 200;
			}
			else if (m_pClientList[iClientH]->m_iLevel > 80 && m_pClientList[iClientH]->m_iLevel <= 100) {
				m_pClientList[iClientH]->m_iWarContribution += (m_pClientList[iClientH]->m_iLevel + m_pClientList[iClientH]->m_iMajesticLevel) * 100;
			}
			else if (m_pClientList[iClientH]->m_iLevel > 100) {
				m_pClientList[iClientH]->m_iWarContribution += (m_pClientList[iClientH]->m_iLevel + m_pClientList[iClientH]->m_iMajesticLevel) * 30;
			}
			dV2 = (double)m_pClientList[iClientH]->m_iExp;
			dV3 = (double)m_pClientList[iClientH]->m_iWarContribution * 1.2f;
			dV1 = dV2 + dV3;
			GetExp(iClientH, dV1);
		}
		else {
			GetExp(iClientH, (m_pClientList[iClientH]->m_iWarContribution / 5));
		}
		m_pClientList[iClientH]->m_iWarContribution = 0;
		m_pClientList[iClientH]->m_dwHeldenianGUID = 0;
		m_pClientList[iClientH]->m_dwSpeedHackCheckTime = timeGetTime();
		m_pClientList[iClientH]->m_iSpeedHackCheckExp = m_pClientList[iClientH]->m_iExp;
	}
}

/*********************************************************************************************************************
**  BOOL CGame::UpdateHeldenianStatus(int iClientH)																	**
**  description		:: Informs every client of the progression of the battle (Type 1 Heldenian)	           			**
**             		:: give -1 as parameter to inform everybody on BtField.											**
**					:: give iClientH to inform only one client														**
*********************************************************************************************************************/
BOOL CGame::UpdateHeldenianStatus(int iClientH)
{
	if (m_bIsHeldenianMode == FALSE)  return FALSE;
	if (m_cHeldenianType != 1)		  return FALSE;
	if (iClientH != -1)
	{
		if ((m_pClientList[iClientH] != NULL)
			&& (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsHeldenianMap == TRUE)
			&& (m_pClientList[iClientH]->m_cMapIndex == m_iBTFieldMapIndex))
		{
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_HELDENIANCOUNT, m_iHeldenianAresdenLeftTower
				, m_iHeldenianElvineLeftTower, m_iHeldenianAresdenFlags, NULL, m_iHeldenianElvineFlags
				, m_iHeldenianAresdenDead, m_iHeldenianElvineDead);
			return TRUE;
		}
	}
	else // Update for everybody
	{
		int iShortCutIndex = 0;
		int i = m_iClientShortCut[iShortCutIndex];
		while (i != 0)
		{
			iShortCutIndex++;
			if ((m_pClientList[i] != NULL)
				&& (m_pClientList[i]->m_bIsInitComplete == TRUE)
				&& (m_pMapList[m_pClientList[i]->m_cMapIndex]->m_bIsHeldenianMap == TRUE)
				&& (m_pClientList[i]->m_cMapIndex == m_iBTFieldMapIndex))
			{
				SendNotifyMsg(NULL, i, DEF_NOTIFY_HELDENIANCOUNT, m_iHeldenianAresdenLeftTower
					, m_iHeldenianElvineLeftTower, m_iHeldenianAresdenFlags, NULL, m_iHeldenianElvineFlags
					, m_iHeldenianAresdenDead, m_iHeldenianElvineDead);
			}
			i = m_iClientShortCut[iShortCutIndex];
		}
	}
	return TRUE;
}

/*********************************************************************************************************************
**  BOOL CGame::RequestHeldenianScroll(int iClientH, char * pData, DWORD dwMsgSize)									**
**  description		:: A player requested some scroll										 						**
**  Scrolls are only available for current battle, and usable only by the player that brought it					**
**																													**
*********************************************************************************************************************/
void CGame::RequestHeldenianScroll(int iClientH, char* pData, DWORD dwMsgSize)
{
	char* cp, cData[256], cTmpName[21];
	int   iItemNbe;
	class CItem* pItem;
	int   iRet, iEraseReq, iNeededPts;
	short* sp;
	WORD* wp;
	DWORD* dwp;
	if (m_pClientList[iClientH] == NULL)					 return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;
	if (m_bIsHeldenianMode == FALSE)						 return;
	if (_iGetItemSpaceLeft(iClientH) == 0)
	{
		SendItemNotifyMsg(iClientH, DEF_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}
	cp = (char*)(pData + DEF_INDEX2_MSGTYPE + 2);
	ZeroMemory(cTmpName, sizeof(cTmpName));
	strcpy(cTmpName, cp);
	cp += 20;
	wp = (WORD*)cp;
	iItemNbe = (int)*wp; // 0x00 l a i
	cp += 2;
	wsprintf(G_cTxt, "PC(%s) obtained a summon scroll (%d).   %s(%d %d)"
		, m_pClientList[iClientH]->m_cCharName
		, iItemNbe
		, m_pClientList[iClientH]->m_cMapName
		, m_pClientList[iClientH]->m_sX
		, m_pClientList[iClientH]->m_sY);
	PutLogList(G_cTxt);
	switch (iItemNbe) {
	case 875: // Sorceress
		iNeededPts = 2000;
		break;
	case 876: // Ancient Temple Knight
		iNeededPts = 3000;
		break;
	case 877: // Elf Master
		iNeededPts = 1500;
		break;
	case 878: // DarkShadow Knight
		iNeededPts = 3000;
		break;
	case 879: // Heavy Battle Tank
		iNeededPts = 4000;
		break;
	case 880: // Barbarian
		iNeededPts = 3000;
		break;
	default:
		iNeededPts = 999999;
		PutLogList("Gail asked to create a wrong item!");
		break;
	}
	if (m_pClientList[iClientH]->m_iConstructionPoint < iNeededPts)
	{
		return; // Unable to get item...
	}
	else
	{	// Get the scroll
		m_pClientList[iClientH]->m_iConstructionPoint -= iNeededPts;
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[iClientH]->m_iConstructionPoint, m_pClientList[iClientH]->m_iWarContribution, 0, NULL); // 0:tell client
	}
	// Them create the summonScroll
	pItem = NULL;
	pItem = new class CItem;
	if (pItem == NULL) return;
	if ((_bInitItemAttr(pItem, iItemNbe) == TRUE))
	{
		pItem->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
		pItem->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
		pItem->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
		pItem->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;
		memcpy(&pItem->m_sItemSpecEffectValue1, &m_dwHeldenianGUID, 4);

		if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == TRUE)
		{
			ZeroMemory(cData, sizeof(cData));
			dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_ITEMOBTAINED;
			cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);
			*cp = 1;
			cp++;
			memcpy(cp, pItem->m_cName, 20);
			cp += 20;
			dwp = (DWORD*)cp;
			*dwp = pItem->m_dwCount;
			cp += 4;
			*cp = pItem->m_cItemType;
			cp++;
			*cp = pItem->m_cEquipPos;
			cp++;
			*cp = (char)0;
			cp++;
			sp = (short*)cp;
			*sp = pItem->m_sLevelLimit;
			cp += 2;
			*cp = pItem->m_cGenderLimit;
			cp++;
			wp = (WORD*)cp;
			*wp = pItem->m_wCurLifeSpan;
			cp += 2;
			wp = (WORD*)cp;
			*wp = pItem->m_wWeight;
			cp += 2;
			sp = (short*)cp;
			*sp = pItem->m_sSprite;
			cp += 2;
			sp = (short*)cp;
			*sp = pItem->m_sSpriteFrame;
			cp += 2;
			*cp = pItem->m_cItemColor;
			cp++;
			*cp = (char)pItem->m_sItemSpecEffectValue2; // v1.41 
			cp++;
			dwp = (DWORD*)cp;
			*dwp = pItem->m_dwAttribute;
			cp += 4;
			if (iEraseReq == 1) delete pItem;
			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);
			switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				DeleteClient(iClientH, TRUE, TRUE);
				break;
			}
		}
		else
		{
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
				m_pClientList[iClientH]->m_sY, pItem);
			SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
				m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
				pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4			
			dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;
			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
			switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				DeleteClient(iClientH, TRUE, TRUE);
				break;
			}
		}
	}
	else
	{
		delete pItem;
		pItem = NULL;
	}
}

/*********************************************************************************************************************
**  void CGame::HeldenianStartWarNow()																				**
**  description		:: War will start 15 min after event beginning for players to prepare themselves       			**
**             		:: This function will start war																	**
**	Called on timer, by OnTimer																												**
*********************************************************************************************************************/
void CGame::HeldenianStartWarNow()
{
	if (m_bHeldenianWarInitiated == TRUE)	return; // Already started
	if (m_cHeldenianWinner != -1)			return; // Battle is already won
	DWORD dwTime = timeGetTime();
	if (dwTime < m_dwHeldenianWarStartTime) return;	// Not yet time to start battle
	m_bHeldenianWarInitiated = TRUE;
	PutLogList("(!) HELDENIAN Start: Fight starting now...");
	int i;
	// Tell clients that war has begun
	for (i = 0; i < DEF_MAXCLIENTS; i++)
	{
		if ((m_pClientList[i] != NULL) && (m_pClientList[i]->m_bIsInitComplete == TRUE))
		{
			m_pClientList[i]->m_cWarType = 2;
			SendNotifyMsg(NULL, i, DEF_NOTIFY_HELDENIANSTART, NULL, NULL, NULL, NULL); // You can now, fight on the battle field
			UpdateHeldenianStatus(i);
		}
	}
	for (i = 0; i < DEF_MAXNPCS; i++) // Remove summons from last Heldenian
	{
		if ((m_pNpcList[i] != NULL) && (m_pNpcList[i]->m_bIsKilled == FALSE)
			&& (m_pMapList[m_pNpcList[i]->m_cMapIndex]->m_bIsHeldenianMap == TRUE))
		{
			if (m_pNpcList[i]->m_bIsSummoned == FALSE)
			{
				m_pNpcList[i]->m_cSide = m_pNpcList[i]->m_cOriginalSide; // All mobs become aggressive again
			}
		}
	}
}
/*********************************************************************************************************************
**  void CGame::HeldenianVictoryNow(int iSide) 																		**
**  description		:: Called by meating a victory condition before maximum time					      			**
**             		:: Called when all tower destroyed (type 1) or when flag set (type 2)							**
**					:: Will recall all defeated characters, others can stay until normal end time					**
*********************************************************************************************************************/
void CGame::HeldenianVictoryNow(int iSide)
{
	if (m_bHeldenianWarInitiated == FALSE) return;
	DWORD dwTime = timeGetTime();
	m_bHeldenianWarInitiated = FALSE;
	m_cHeldenianWinner = iSide;
	switch (m_cHeldenianWinner) {
	case 1:
		PutLogList("Heldenian victory before allowed time, Aresden's victory.");
		break;
	case 2:
		PutLogList("Heldenian victory before allowed time, Elvine's victory.");
		break;
	default:
		PutLogList("Heldenian victory before allowed time, Draw.");
		break;
	}

	m_dwHeldenianFinishTime = timeGetTime() + 5 * 60 * 1000;  // war will end in 5 minutes
	int m;
	int j, n;
	for (m = 0; m < DEF_MAXMAPS; m++)
	{
		if (m_pMapList[m] == NULL) break;
		if (m_pMapList[m]->m_bIsHeldenianMap == TRUE)
		{
			for (j = 0; j < DEF_MAXCLIENTS; j++)
			{
				if ((m_pClientList[j] != NULL)
					&& (m_pClientList[j]->m_bIsInitComplete == TRUE)
					&& (m_pClientList[j]->m_cMapIndex == m))
				{
					UpdateHeldenianStatus(j);
					SendNotifyMsg(NULL, j, DEF_NOTIFY_HELDENIANVICTORY, m_cHeldenianWinner, NULL, NULL, NULL); // Victory defeat screen
					if ((m_pClientList[j]->m_cSide != iSide)// recall loosers only
						&& (m_pClientList[j]->m_iAdminUserLevel == 0))
					{
						SendNotifyMsg(NULL, j, DEF_NOTIFY_0BE8, NULL, NULL, NULL, NULL);
						m_pClientList[j]->m_bIsWarLocation = TRUE;
						m_pClientList[j]->m_iTimeLeft_ForceRecall = 3;
					}
				}
			}
			for (n = 0; n < DEF_MAXNPCS; n++)
			{
				if ((m_pNpcList[n] != NULL)
					&& (m_pNpcList[n]->m_cMapIndex == m)
					&& (m_pNpcList[n]->m_cSide != iSide))
				{
					if (m_pNpcList[n]->m_bIsSummoned == TRUE)
					{
						NpcKilledHandler(NULL, NULL, n, NULL);
					}
					else
					{
						m_pNpcList[n]->m_cSide = 0; // All mobs become neutral as players no more have spells or attacks...
					}
				}
			}
		}
	}
}
/*********************************************************************************************************************
**  void CGame::HeldenianEndWarNow()																				**
**  description		:: Checks if Heldenian should be finished, and victory conditions                      			**
**             		:: used only on Heldenian server																**
**																													**
*********************************************************************************************************************/
void CGame::HeldenianEndWarNow()
{
	if (m_bIsHeldenianMode == FALSE) return;
	DWORD dwTime = timeGetTime();
	if (dwTime < m_dwHeldenianFinishTime) return;
	if (m_cHeldenianType == 1)
	{
		if (m_iBTFieldMapIndex == -1) return;
		// Type 1: BtField, victory depends on Tower destroyed...
		if (m_cHeldenianWinner == -1)
		{
			if (m_iHeldenianAresdenLeftTower > m_iHeldenianElvineLeftTower)
			{
				m_cHeldenianWinner = 1;
			}
			else if (m_iHeldenianAresdenLeftTower < m_iHeldenianElvineLeftTower)
			{
				m_cHeldenianWinner = 2;
			}
			else if (m_iHeldenianAresdenFlags > m_iHeldenianElvineFlags)
			{
				m_cHeldenianWinner = 1;
			}
			else if (m_iHeldenianAresdenFlags < m_iHeldenianElvineFlags)
			{
				m_cHeldenianWinner = 2;
			}
			else // 1st destroyed tower wins...
			{
				m_cHeldenianWinner = m_cHeldenianFirstDestroyedTowerWinner;
			}
		}
	}
	else if (m_cHeldenianType == 2)
	{
		if (m_iGodHMapIndex == -1) return;
		// Type 2: Victory for defenders if no victory before
		if (m_cHeldenianWinner == -1)
		{
			m_cHeldenianWinner = m_sLastHeldenianWinner;
		}
	}
	switch (m_cHeldenianWinner) {
	case 1:
		PutLogList("Heldenian Terminated, Aresden's victory.");
		break;
	case 2:
		PutLogList("Heldenian Terminated, Elvine's victory.");
		break;
	default:
		PutLogList("Heldenian Terminated, Draw.");
		break;
	}

	GlobalEndHeldenianMode();
}

void CGame::HeldenianWarStarter()
{
	SYSTEMTIME SysTime;
	int i;

	if (m_bIsCrusadeMode == TRUE) return;
	if (m_bIsApocalypseMode == TRUE) return;
	if (m_bIsHeldenianMode) return;
	if (m_bIsHeldenianReady == FALSE) return;

	GetLocalTime(&SysTime);
	for (i = 0; i < DEF_MAXSCHEDULE; i++) {
		if ((m_bIsHeldenianScheduleLoaded == TRUE) &&
			(m_stHeldenianSchedule[i].iDay == SysTime.wDayOfWeek) &&
			(m_stHeldenianSchedule[i].StartiHour == SysTime.wHour) &&
			(m_stHeldenianSchedule[i].StartiMinute == SysTime.wMinute) &&
			(m_bIsApocalypseMode != TRUE) &&
			(m_bIsCrusadeMode != TRUE)) {
			wsprintf(G_cTxt, "(!) Heldenian Start : time(%d %d:%d), index(%d)", m_stHeldenianSchedule[i].iDay, m_stHeldenianSchedule[i].StartiHour, m_stHeldenianSchedule[i].StartiMinute, i);
			PutLogFileList(G_cTxt);
			GlobalStartHeldenianMode();
			break;
		}
	}
}

/*********************************************************************************************************************
**  BOOL CGame::bReadHeldenianGUIDFile(char * cFn)																	**
**  description		:: Read thee GUID file for each server						                        			**
**             		:: Used when server starts																		**
**																													**
*********************************************************************************************************************/
void CGame::bReadHeldenianGUIDFile(char* cFn)
{
	FILE* pFile;
	HANDLE hFile;
	DWORD  dwFileSize;
	char* cp, * token, cReadMode;
	char seps[] = "= \t\n";
	class CStrTok* pStrTok;
	cReadMode = 0;
	hFile = CreateFile(cFn, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	dwFileSize = GetFileSize(hFile, NULL);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	pFile = fopen(cFn, "rt");
	if (pFile == NULL)
	{
		PutLogList("(!) Cannot open HeldenianGUID file.");
		return;
	}
	else
	{
		PutLogList("(!) Reading HeldenianGUID file...");
		cp = new char[dwFileSize + 2];
		ZeroMemory(cp, dwFileSize + 2);
		fread(cp, dwFileSize, 1, pFile);
		pStrTok = new class CStrTok(cp, seps);
		token = pStrTok->pGet();
		while (token != NULL)
		{
			if (cReadMode != 0)
			{
				switch (cReadMode) {
				case 1:
					m_dwHeldenianGUID = _atoi64(token);
					cReadMode = 0;
					break;
				case 2:
					m_sLastHeldenianWinner = atoi(token);
					cReadMode = 0;
					break;
				}
			}
			else
			{
				if (memcmp(token, "HeldenianGUID", 13) == 0) cReadMode = 1;
				if (memcmp(token, "winner-side", 11) == 0) cReadMode = 2;
			}
			token = pStrTok->pGet();
		}
		delete pStrTok;
		delete[] cp;
		fclose(pFile);
	}
	switch (m_sLastHeldenianWinner) {
	case 1:
		wsprintf(G_cTxt, "(!) Read HeldenianGUID (%d) file: Aresden owns BtField (%d).", m_dwHeldenianGUID, m_sLastHeldenianWinner);
		break;
	case 2:
		wsprintf(G_cTxt, "(!) Read HeldenianGUID (%d) file: Elvine owns BtField (%d).", m_dwHeldenianGUID, m_sLastHeldenianWinner);
		break;
	default:
		wsprintf(G_cTxt, "(!) Read HeldenianGUID (%d) file: Nobody owns BtField (%d).", m_dwHeldenianGUID, m_sLastHeldenianWinner);
		break;
	}
	PutLogList(G_cTxt);
}