#include "Crusade.h"

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

Crusade::Crusade()
{
}

Crusade::~Crusade()
{
}

void CGame::NpcBehavior_Stop(int iNpcH)
{
	char  cTargetType;
	short sTarget = NULL;
	BOOL  bFlag;

	if (m_pNpcList[iNpcH] == NULL) return;

	m_pNpcList[iNpcH]->m_sBehaviorTurnCount++;

	switch (m_pNpcList[iNpcH]->m_cActionLimit) {
	case 5:
		switch (m_pNpcList[iNpcH]->m_sType) {
		case 38:
			if (m_pNpcList[iNpcH]->m_sBehaviorTurnCount >= 3) {
				m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
				bFlag = _bNpcBehavior_ManaCollector(iNpcH);
				if (bFlag == TRUE) {
					SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTATTACK, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, 1);
				}
			}
			break;

		case 39: // Detector
		case 89: // AGC (Ancient Grand Cannon)	will turn as detectors	
			if (m_pNpcList[iNpcH]->m_sBehaviorTurnCount >= 3) {
				m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
				bFlag = _bNpcBehavior_Detector(iNpcH);

				if (bFlag == TRUE) {
					// ���� �߰��ߴ�. ���� �������� �˷��� �Ѵ�.	
					SendEventToNearClient_TypeA(iNpcH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTATTACK, m_pNpcList[iNpcH]->m_sX, m_pNpcList[iNpcH]->m_sY, 1);
				}
			}
			break;

		case 40: // Energy Shield Generator
			break;

		case 41: // Grand Magic Generator
			if (m_pNpcList[iNpcH]->m_sBehaviorTurnCount >= 3) {
				m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
				_NpcBehavior_GrandMagicGenerator(iNpcH);
			}
			break;

		case 42: // ManaStone: v2.05 ���������� ���������� �������� 5�� �����Ѵ�.
			m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
			m_pNpcList[iNpcH]->m_iV1 += 5;
			if (m_pNpcList[iNpcH]->m_iV1 >= 5) m_pNpcList[iNpcH]->m_iV1 = 5;
			break;

		default:
			TargetSearch(iNpcH, &sTarget, &cTargetType);
			break;
		}
		break;
	}

	if ((sTarget != NULL)) {

		// ���ݸ�ǥ �߰�. 
		m_pNpcList[iNpcH]->m_cBehavior = DEF_BEHAVIOR_ATTACK;
		m_pNpcList[iNpcH]->m_sBehaviorTurnCount = 0;
		m_pNpcList[iNpcH]->m_iTargetIndex = sTarget;
		m_pNpcList[iNpcH]->m_cTargetType = cTargetType;
		// ���⼭ ǥȿ ���۰������� ���� �޽��� �߼�. 

	}
}

void CGame::LocalStartCrusadeMode(DWORD dwCrusadeGUID)
{
	int i, z, tX, tY, iNamingValue, c, m;
	char cName[6], cNpcName[21], cNpcWayPoint[11];

	if (m_bIsCrusadeMode == TRUE) return;
	if (m_bIsHeldenianMode == TRUE) return;
	if (m_bIsApocalypseMode == TRUE) return;
	m_bIsCrusadeMode = TRUE;
	m_iCrusadeWinnerSide = 0;
	if (dwCrusadeGUID != NULL) {
		_CreateCrusadeGUID(dwCrusadeGUID, NULL);
		m_dwCrusadeGUID = dwCrusadeGUID;
	}

	for (c = 1; c < DEF_MAXCLIENTS; c++)
		if ((m_pClientList[c] != NULL) && (m_pClientList[c]->m_bIsInitComplete == TRUE)) {
			m_pClientList[c]->m_iCrusadeDuty = 0;
			m_pClientList[c]->m_iConstructionPoint = 0;
			m_pClientList[c]->m_dwCrusadeGUID = m_dwCrusadeGUID;
			SendNotifyMsg(NULL, c, DEF_NOTIFY_CRUSADE, (DWORD)m_bIsCrusadeMode, m_pClientList[c]->m_iCrusadeDuty, NULL, NULL);
		}

	for (m = 0; m < DEF_MAXMAPS; m++)
		if (m_pMapList[m] != NULL) m_pMapList[m]->RestoreStrikePoints();

	ZeroMemory(cName, sizeof(cName));
	ZeroMemory(cNpcName, sizeof(cNpcName));
	ZeroMemory(cNpcWayPoint, sizeof(cNpcWayPoint));

	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++)
		if (m_stCrusadeStructures[i].cType != NULL) {
			for (z = 0; z < DEF_MAXMAPS; z++)
				if ((m_pMapList[z] != NULL) && (strcmp(m_pMapList[z]->m_cName, m_stCrusadeStructures[i].cMapName) == 0)) {
					// À§Ä¡°¡ ÀÏÄ¡ÇÏ´Â ¸Ê¿¡ °ÇÃà¹°À» À§Ä¡½ÃÅ²´Ù.
					iNamingValue = m_pMapList[z]->iGetEmptyNamingValue();
					if (iNamingValue != -1) {
						// ´õÀÌ»ó ÀÌ ¸Ê¿¡ NPC¸¦ ¸¸µé¼ö ¾ø´Ù. ÀÌ¸§À» ÇÒ´çÇÒ ¼ö ¾ø±â ¶§¹®.
						// ÀÌ·± ÀÏÀÌ?

						// NPC¸¦ »ý¼ºÇÑ´Ù.
						wsprintf(cName, "XX%d", iNamingValue);
						cName[0] = '_';
						cName[1] = z + 65;

						switch (m_stCrusadeStructures[i].cType) {
						case 36:
							if (strcmp(m_pMapList[z]->m_cName, "aresden") == 0)
								strcpy(cNpcName, "AGT-Aresden");
							else if (strcmp(m_pMapList[z]->m_cName, "elvine") == 0)
								strcpy(cNpcName, "AGT-Elvine");
							break;

						case 37:
							if (strcmp(m_pMapList[z]->m_cName, "aresden") == 0)
								strcpy(cNpcName, "CGT-Aresden");
							else if (strcmp(m_pMapList[z]->m_cName, "elvine") == 0)
								strcpy(cNpcName, "CGT-Elvine");
							break;

						case 40:
							if (strcmp(m_pMapList[z]->m_cName, "aresden") == 0)
								strcpy(cNpcName, "ESG-Aresden");
							else if (strcmp(m_pMapList[z]->m_cName, "elvine") == 0)
								strcpy(cNpcName, "ESG-Elvine");
							break;

						case 41:
							if (strcmp(m_pMapList[z]->m_cName, "aresden") == 0)
								strcpy(cNpcName, "GMG-Aresden");
							else if (strcmp(m_pMapList[z]->m_cName, "elvine") == 0)
								strcpy(cNpcName, "GMG-Elvine");
							break;

						case 42:
							strcpy(cNpcName, "ManaStone");
							break;

						default:
							strcpy(cNpcName, m_pNpcConfigList[m_stCrusadeStructures[i].cType]->m_cNpcName);
							break;
						}

						tX = (int)m_stCrusadeStructures[i].dX;
						tY = (int)m_stCrusadeStructures[i].dY;
						if (bCreateNewNpc(cNpcName, cName, m_pMapList[z]->m_cName, 0, 0, DEF_MOVETYPE_RANDOM,
							&tX, &tY, cNpcWayPoint, NULL, NULL, -1, FALSE) == FALSE) {
							// ½ÇÆÐÇßÀ¸¹Ç·Î ¿¹¾àµÈ NameValue¸¦ ÇØÁ¦½ÃÅ²´Ù.
							m_pMapList[z]->SetNamingValueEmpty(iNamingValue);
						}
						else {
							wsprintf(G_cTxt, "(!) Creating Crusade Structure(%s) at %s(%d, %d)", cNpcName, m_stCrusadeStructures[i].cMapName, tX, tY);
							PutLogList(G_cTxt);
						}
					}
				}
		}

	PutLogList("(!)Crusade Mode ON.");
	_bCrusadeLog(DEF_CRUSADELOG_STARTCRUSADE, NULL, NULL, NULL);
}

void CGame::_CreateCrusadeGUID(DWORD dwCrusadeGUID, int iWinnerSide)
{
	char* cp, cTxt[256], cTemp[1024];
	FILE* pFile;

	

	pFile = fopen("..\\GameConfigs\\CrusadeGUID.Txt", "wt");
	if (pFile == NULL) {
		// ÆÄÀÏÀ» ¸¸µé ¼ö ¾ø°Å³ª »çÀÌÁî°¡ Áö³ªÄ¡°Ô ÀÛÀº °æ¿ì´Â . 
		wsprintf(cTxt, "(!) Cannot create CrusadeGUID(%d) file", dwCrusadeGUID);
		PutLogList(cTxt);
	}
	else {
		ZeroMemory(cTemp, sizeof(cTemp));

		ZeroMemory(cTxt, sizeof(cTxt));
		wsprintf(cTxt, "CrusadeGUID = %d\n", dwCrusadeGUID);
		strcat(cTemp, cTxt);

		ZeroMemory(cTxt, sizeof(cTxt));
		wsprintf(cTxt, "winner-side = %d\n", iWinnerSide);
		strcat(cTemp, cTxt);

		cp = (char*)cTemp;
		fwrite(cp, strlen(cp), 1, pFile);

		wsprintf(cTxt, "(!) CrusadeGUID(%d) file created", dwCrusadeGUID);
		PutLogList(cTxt);

		fclose(pFile);
	}
}

void CGame::bReadCrusadeGUIDFile(char* cFn)
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
	if (pFile == NULL) {
		// °ÔÀÓ¼­¹öÀÇ ÃÊ±âÈ­ ÆÄÀÏÀ» ÀÐÀ» ¼ö ¾ø´Ù.
		PutLogList("(!) Cannot open CrusadeGUID file.");
		return;
	}
	else {
		PutLogList("(!) Reading CrusadeGUID file...");
		cp = new char[dwFileSize + 2];
		ZeroMemory(cp, dwFileSize + 2);
		fread(cp, dwFileSize, 1, pFile);

		pStrTok = new class CStrTok(cp, seps);
		token = pStrTok->pGet();

		while (token != NULL) {

			if (cReadMode != 0) {
				switch (cReadMode) {
				case 1:
					m_dwCrusadeGUID = atoi(token);
					wsprintf(G_cTxt, "(!) Read CrusadeGUID (%d) file.", m_dwCrusadeGUID);
					PutLogList(G_cTxt);
					cReadMode = 0;
					break;

				case 2:
					// New 13/05/2004 Changed
					m_iLastCrusadeWinner = atoi(token);
					wsprintf(G_cTxt, "(!) Crusade Winner Side: (%d)", m_iLastCrusadeWinner);
					PutLogList(G_cTxt);
					cReadMode = 0;
					break;
				}
			}
			else {
				if (memcmp(token, "CrusadeGUID", 11) == 0) cReadMode = 1;
				if (memcmp(token, "winner-side", 11) == 0) cReadMode = 2;
			}

			token = pStrTok->pGet();
		}

		delete pStrTok;
		delete[] cp;

		fclose(pFile);
	}
}

/*********************************************************************************************************************
**  void CGame::GameProcess()																						**
**  DESCRIPTION			:: reads Crusade.cfg file																	**
**  LAST_UPDATED		:: March 17, 2005; 12:30 PM; Hypnotoad														**
**	RETURN_VALUE		:: void																						**
**  NOTES				::	n/a																						**
**	MODIFICATION		::	n/a																						**
**********************************************************************************************************************/
void CGame::bReadCrusadeStructureConfigFile(char* cFn)
{
	FILE* pFile;
	HANDLE hFile;
	DWORD  dwFileSize;
	char* cp, * token, cReadModeA, cReadModeB;
	char seps[] = "= \t\n";
	int   iIndex;
	class CStrTok* pStrTok;

	cReadModeA = 0;
	cReadModeB = 0;

	hFile = CreateFile(cFn, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	dwFileSize = GetFileSize(hFile, NULL);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

	pFile = fopen(cFn, "rt");
	if (pFile != NULL) {

		PutLogList("(!) Reading Crusade configuration file...");
		cp = new char[dwFileSize + 2];
		ZeroMemory(cp, dwFileSize + 2);
		fread(cp, dwFileSize, 1, pFile);

		iIndex = 0;
		pStrTok = new class CStrTok(cp, seps);
		token = pStrTok->pGet();
		while (token != NULL) {
			if (cReadModeA != 0) {
				switch (cReadModeA) {
				case 1:
					switch (cReadModeB) {
					case 1:
						if (_bGetIsStringIsNumber(token) == FALSE) {
							PutLogList("(!!!) CRITICAL ERROR!  Crusade configuration file error - Wrong Data format(1).");
							delete[] cp;
							delete pStrTok;
							return;
						}
						iIndex = atoi(token);

						if (m_stCrusadeStructures[iIndex].cType != NULL) {
							PutLogList("(!!!) CRITICAL ERROR!  Crusade configuration file error - Duplicate crusade number."); // changed from "duplicate potion number"
							delete[] cp;
							delete pStrTok;
							return;
						}
						cReadModeB = 2;
						break;

					case 2:
						ZeroMemory(m_stCrusadeStructures[iIndex].cMapName, sizeof(m_stCrusadeStructures[iIndex].cMapName));
						memcpy(m_stCrusadeStructures[iIndex].cMapName, token, strlen(token));
						cReadModeB = 3;
						break;

					case 3:
						if (_bGetIsStringIsNumber(token) == FALSE) {
							PutLogList("(!!!) CRITICAL ERROR!  Crusade configuration file error - Wrong Data format.");
							delete[] cp;
							delete pStrTok;
							return;
						}
						m_stCrusadeStructures[iIndex].cType = atoi(token);
						cReadModeB = 4;
						break;

					case 4:
						if (_bGetIsStringIsNumber(token) == FALSE) {
							PutLogList("(!!!) CRITICAL ERROR!  Crusade configuration file error - Wrong Data format.");
							delete[] cp;
							delete pStrTok;
							return;
						}
						m_stCrusadeStructures[iIndex].dX = atoi(token);
						cReadModeB = 5;
						break;

					case 5:
						if (_bGetIsStringIsNumber(token) == FALSE) {
							PutLogList("(!!!) CRITICAL ERROR!  Crusade configuration file error - Wrong Data format.");
							delete[] cp;
							delete pStrTok;
							return;
						}
						m_stCrusadeStructures[iIndex].dY = atoi(token);
						cReadModeA = 0;
						cReadModeB = 0;
						break;
					}
					break;

				default:
					break;
				}
			}
			else {
				if (memcmp(token, "crusade-structure", 17) == 0) {
					cReadModeA = 1;
					cReadModeB = 1;
				}
			}
			token = pStrTok->pGet();
		}
		delete pStrTok;
		delete[] cp;
		if ((cReadModeA != 0) || (cReadModeB != 0)) {
			PutLogList("(!!!) CRITICAL ERROR! Crusade Structure configuration file contents error!");
			return;
		}
		fclose(pFile);
	}
}

void CGame::CheckCrusadeResultCalculation(int iClientH)
{
	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_cWarType != 1) return;

	if ((m_bIsCrusadeMode == FALSE) && (m_pClientList[iClientH]->m_dwCrusadeGUID != 0)) {
		if (m_pClientList[iClientH]->m_dwCrusadeGUID == m_dwCrusadeGUID) {
			if (m_iCrusadeWinnerSide == 0) {
				GetExp(iClientH, (m_pClientList[iClientH]->m_iWarContribution / 6));
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CRUSADE, (DWORD)m_bIsCrusadeMode, NULL, m_pClientList[iClientH]->m_iWarContribution, NULL);
				_bCrusadeLog(DEF_CRUSADELOG_GETEXP, iClientH, (m_pClientList[iClientH]->m_iWarContribution / 6), NULL);
			}
			else {
				if (m_iCrusadeWinnerSide == m_pClientList[iClientH]->m_cSide) {
					if (m_pClientList[iClientH]->m_iLevel <= 80) {
						m_pClientList[iClientH]->m_iWarContribution += (m_pClientList[iClientH]->m_iLevel + m_pClientList[iClientH]->m_iMajesticLevel) * 100;
					}
					else if (m_pClientList[iClientH]->m_iLevel > 80 && m_pClientList[iClientH]->m_iLevel <= 100) {
						m_pClientList[iClientH]->m_iWarContribution += (m_pClientList[iClientH]->m_iLevel + m_pClientList[iClientH]->m_iMajesticLevel) * 40;
					}
					else if (m_pClientList[iClientH]->m_iLevel > 100)
					{
						m_pClientList[iClientH]->m_iWarContribution += m_pClientList[iClientH]->m_iLevel + m_pClientList[iClientH]->m_iMajesticLevel;
					}
					GetExp(iClientH, m_pClientList[iClientH]->m_iWarContribution);
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CRUSADE, (DWORD)m_bIsCrusadeMode, NULL, m_pClientList[iClientH]->m_iWarContribution, NULL);
					_bCrusadeLog(DEF_CRUSADELOG_GETEXP, iClientH, m_pClientList[iClientH]->m_iWarContribution, NULL);
				}
				else if (m_iCrusadeWinnerSide != m_pClientList[iClientH]->m_cSide) {
					m_pClientList[iClientH]->m_iWarContribution += m_pClientList[iClientH]->m_iLevel + m_pClientList[iClientH]->m_iMajesticLevel;
					GetExp(iClientH, (m_pClientList[iClientH]->m_iWarContribution / 10));
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CRUSADE, (DWORD)m_bIsCrusadeMode, NULL, -1 * m_pClientList[iClientH]->m_iWarContribution, NULL);
					_bCrusadeLog(DEF_CRUSADELOG_GETEXP, iClientH, (m_pClientList[iClientH]->m_iWarContribution / 10), NULL);
				}
			}
		}
		else {
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CRUSADE, (DWORD)m_bIsCrusadeMode, NULL, 0, NULL, -1);
		}
		m_pClientList[iClientH]->m_iCrusadeDuty = 0;
		m_pClientList[iClientH]->m_iWarContribution = 0;
		m_pClientList[iClientH]->m_dwCrusadeGUID = 0;
		m_pClientList[iClientH]->m_dwSpeedHackCheckTime = timeGetTime();
		m_pClientList[iClientH]->m_iSpeedHackCheckExp = m_pClientList[iClientH]->m_iExp;
	}
}

// New 11/05/2004 Changed
void CGame::DoMeteorStrikeDamageHandler(int iMapIndex)
{
	int i, iDamage;

	// ÇØ´ç ¸ÊÀÇ ÇÃ·¹ÀÌ¾îµéÀÇ HP¸¦ ÀÏ°ý »è°¨
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != NULL) && (m_pClientList[i]->m_cSide != 0) && (m_pClientList[i]->m_cMapIndex == iMapIndex)) {

			if (m_pClientList[i]->m_iLevel < 80)
				iDamage = m_pClientList[i]->m_iLevel + iDice(1, 100);
			else iDamage = (m_pClientList[i]->m_iLevel + m_pClientList[i]->m_iMajesticLevel) * 2 + iDice(1, 100);

			if (m_pClientList[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 2) { //magic cut in half
				iDamage = (iDamage / 2) - 2;
			}

			if (m_pClientList[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_PROTECT] == 5) {
				iDamage = 0;
			}

			if (m_pClientList[i]->m_iAdminUserLevel > 0) { // Admins get no damage
				iDamage = 0;
			}

			m_pClientList[i]->m_iHP -= iDamage;
			if (m_pClientList[i]->m_iHP <= 0) {
				// ÇÃ·¹ÀÌ¾î°¡ »ç¸ÁÇß´Ù.
				ClientKilledHandler(i, NULL, NULL, iDamage);
				m_stMeteorStrikeResult.iCasualties++;
			}
			else {
				if (iDamage > 0) {
					// ¹ÞÀº ´ë¹ÌÁö¸¦ Åëº¸ÇÑ´Ù. <- HP¸¦ ±×´ë·Î ¾Ë¸°´Ù.
					SendNotifyMsg(NULL, i, DEF_NOTIFY_HP, NULL, NULL, NULL, NULL);
					// Ãæ°ÝÀ» ¹Þ¾Ò´Ù¸é Ãæ°Ýµ¿ÀÛ Àü¼Û 
					SendEventToNearClient_TypeA(i, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, NULL, NULL);

					if (m_pClientList[i]->m_bSkillUsingStatus[19] != TRUE) {
						m_pMapList[m_pClientList[i]->m_cMapIndex]->ClearOwner(0, i, DEF_OWNERTYPE_PLAYER, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
						m_pMapList[m_pClientList[i]->m_cMapIndex]->SetOwner(i, DEF_OWNERTYPE_PLAYER, m_pClientList[i]->m_sX, m_pClientList[i]->m_sY);
					}

					if (m_pClientList[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] != 0) {
						SendNotifyMsg(NULL, i, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_HOLDOBJECT, m_pClientList[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT], NULL, NULL);
						m_pClientList[i]->m_cMagicEffectStatus[DEF_MAGICTYPE_HOLDOBJECT] = NULL;
						bRemoveFromDelayEventList(i, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_HOLDOBJECT);
					}
				}
			}
		}
}

void CGame::SyncMiddlelandMapInfo()
{
	int i;
	char* cp;
	short* sp;

	// �̵鷣���� ������ ��� �������� ��ũ��Ų��. �̵鷣�带 ����� ������� �ٷ� ����ü�� ������Ʈ �ϰ� ������ �����鿡�� ���� ����.
	if (m_iMiddlelandMapIndex != -1) {
		// ����ü Ŭ����
		for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++) {
			m_stMiddleCrusadeStructureInfo[i].cType = NULL;
			m_stMiddleCrusadeStructureInfo[i].cSide = NULL;
			m_stMiddleCrusadeStructureInfo[i].sX = NULL;
			m_stMiddleCrusadeStructureInfo[i].sY = NULL;
		}
		// ����ü ���� �� ���� �޽��� �ۼ� 
		m_iTotalMiddleCrusadeStructures = m_pMapList[m_iMiddlelandMapIndex]->m_iTotalCrusadeStructures;
		ZeroMemory(G_cData50000, sizeof(G_cData50000));
		cp = (char*)G_cData50000;
		*cp = GSM_MIDDLEMAPSTATUS;
		cp++;

		sp = (short*)cp;
		*sp = (short)m_iTotalMiddleCrusadeStructures;
		cp += 2;

		for (i = 0; i < m_iTotalMiddleCrusadeStructures; i++) {
			m_stMiddleCrusadeStructureInfo[i].cType = m_pMapList[m_iMiddlelandMapIndex]->m_stCrusadeStructureInfo[i].cType;
			m_stMiddleCrusadeStructureInfo[i].cSide = m_pMapList[m_iMiddlelandMapIndex]->m_stCrusadeStructureInfo[i].cSide;
			m_stMiddleCrusadeStructureInfo[i].sX = m_pMapList[m_iMiddlelandMapIndex]->m_stCrusadeStructureInfo[i].sX;
			m_stMiddleCrusadeStructureInfo[i].sY = m_pMapList[m_iMiddlelandMapIndex]->m_stCrusadeStructureInfo[i].sY;

			*cp = m_stMiddleCrusadeStructureInfo[i].cType;
			cp++;
			*cp = m_stMiddleCrusadeStructureInfo[i].cSide;
			cp++;
			sp = (short*)cp;
			*sp = (short)m_stMiddleCrusadeStructureInfo[i].sX;
			cp += 2;
			sp = (short*)cp;
			*sp = (short)m_stMiddleCrusadeStructureInfo[i].sY;
			cp += 2;
		}

	}
}

void CGame::GSM_SetGuildConstructLoc(int iGuildGUID, int dX, int dY, char* pMapName)
{
	int i, iIndex;
	DWORD dwTemp, dwTime;

	// Å¬¶óÀÌ¾ðÆ®·ÎºÎÅÍÀÇ ¿äÃ»ÀÌ ¾Æ´Ï¶ó ´Ù¸¥ ¼­¹ö·ÎºÎÅÍÀÇ ÅÚ·¹Æ÷Æ® ÁÂÇ¥ ¼³Á¤ ¿äÃ»ÀÓ. ÀÀ´äÇÒ ÇÊ¿ä ¾øÀ½.

	dwTime = timeGetTime();

	// ¸ÕÀú °°Àº GUID¸¦ °¡Áø ±æµå ÅÚ·¹Æ÷Æ® ÁÂÇ¥°¡ ¼³Á¤µÇ¾î ÀÖ´ÂÁö Ã£´Â´Ù.
	for (i = 0; i < DEF_MAXGUILDS; i++)
		if (m_pGuildTeleportLoc[i].m_iV1 == iGuildGUID) {
			// ÀÌ¹Ì ±æµå°¡ ÀÖ´Ù.
			if ((m_pGuildTeleportLoc[i].m_sDestX2 == dX) && (m_pGuildTeleportLoc[i].m_sDestY2 == dY) && (strcmp(m_pGuildTeleportLoc[i].m_cDestMapName2, pMapName) == 0)) {
				// ³»¿ëÀÌ ÀÏÄ¡ÇÑ´Ù. Å¸ÀÓ ÀÔ·ÂÈÄ ½ºÅµ.
				m_pGuildTeleportLoc[i].m_dwTime2 = dwTime;
				return;
			}
			else {
				// ³»¿ë °»½Å
				m_pGuildTeleportLoc[i].m_sDestX2 = dX;
				m_pGuildTeleportLoc[i].m_sDestY2 = dY;
				ZeroMemory(m_pGuildTeleportLoc[i].m_cDestMapName2, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName2));
				strcpy(m_pGuildTeleportLoc[i].m_cDestMapName2, pMapName);
				m_pGuildTeleportLoc[i].m_dwTime2 = dwTime;
				return;
			}
		}

	// ¼³Á¤µÈ ³»¿ëÀÌ ¾ø´Ù. »õ·Î ¼³Á¤ÇÑ´Ù.
	dwTemp = 0;
	iIndex = -1;
	for (i = 0; i < DEF_MAXGUILDS; i++) {
		if (m_pGuildTeleportLoc[i].m_iV1 == NULL) {

			m_pGuildTeleportLoc[i].m_iV1 = iGuildGUID;
			m_pGuildTeleportLoc[i].m_sDestX2 = dX;
			m_pGuildTeleportLoc[i].m_sDestY2 = dY;
			ZeroMemory(m_pGuildTeleportLoc[i].m_cDestMapName2, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName));
			strcpy(m_pGuildTeleportLoc[i].m_cDestMapName2, pMapName);
			m_pGuildTeleportLoc[i].m_dwTime2 = dwTime;
			return;
		}
		else {
			// °¡Àå ¿À·§µ¿¾È ÂüÁ¶µÇÁö ¾ÊÀº ¸®½ºÆ®ÀÇ ÀÎµ¦½º¸¦ Ã£´Â´Ù.
			if (dwTemp < (dwTime - m_pGuildTeleportLoc[i].m_dwTime2)) {
				dwTemp = (dwTime - m_pGuildTeleportLoc[i].m_dwTime2);
				iIndex = i;
			}
		}
	}

	m_pGuildTeleportLoc[i].m_iV1 = iGuildGUID;
	m_pGuildTeleportLoc[i].m_sDestX2 = dX;
	m_pGuildTeleportLoc[i].m_sDestY2 = dY;
	ZeroMemory(m_pGuildTeleportLoc[i].m_cDestMapName2, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName2));
	strcpy(m_pGuildTeleportLoc[i].m_cDestMapName2, pMapName);
	m_pGuildTeleportLoc[i].m_dwTime2 = dwTime;
}

void CGame::CheckCommanderConstructionPoint(int iClientH)
{
	char* cp, cData[120];
	int* ip, i;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_bIsCrusadeMode == FALSE) return;
	if (m_pClientList[iClientH]->m_iConstructionPoint <= 0) return;

	switch (m_pClientList[iClientH]->m_iCrusadeDuty) {
	case 1: // ÆÄÀÌÅÍ
	case 2: // °Ç¼³ÀÚ: ¸ð¾Æ ³õÀº Æ÷ÀÎÆ®¸¦ ±æµå¸¶½ºÅÍ ÁöÈÖ°ü¿¡°Ô Àü´ÞÇÑ´Ù.
		for (i = 0; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != NULL) && (m_pClientList[i]->m_iCrusadeDuty == 3) &&
				(m_pClientList[i]->m_iGuildGUID == m_pClientList[iClientH]->m_iGuildGUID)) {
				// ÇöÀç ¼­¹ö ³»¿¡ ±æµå¸¶½ºÅÍ ÁöÈÖ°üÀÌ ÀÖ´Ù. ÁöÈÖ°üÀÇ Æ÷ÀÎÆ®¸¦ ³ôÀÎ ÈÄ 
				m_pClientList[i]->m_iConstructionPoint += m_pClientList[iClientH]->m_iConstructionPoint;
				m_pClientList[i]->m_iWarContribution += (m_pClientList[iClientH]->m_iConstructionPoint / 10);



				SendNotifyMsg(NULL, i, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[i]->m_iConstructionPoint, m_pClientList[i]->m_iWarContribution, NULL, NULL);
				m_pClientList[iClientH]->m_iConstructionPoint = 0; // °ª ÃÊ±âÈ­ 
				return;
			}

		// ´Ù¸¥ ¼­¹öÀÇ ÁöÈÖ°ü¿¡°Ô ¾Ë·Á¾ß ÇÑ´Ù.
		ZeroMemory(cData, sizeof(cData));
		cp = (char*)cData;
		*cp = GSM_CONSTRUCTIONPOINT;
		cp++;
		ip = (int*)cp;
		*ip = m_pClientList[iClientH]->m_iGuildGUID;
		cp += 4;
		ip = (int*)cp;
		*ip = m_pClientList[iClientH]->m_iConstructionPoint;
		cp += 4;
		bStockMsgToGateServer(cData, 9);

		m_pClientList[iClientH]->m_iConstructionPoint = 0; // °ª ÃÊ±âÈ­ 
		break;

	case 3: // ÁöÈÖ°ü: º¸³¾ ÇÊ¿ä ¾øÀ½ 

		break;
	}
}

void CGame::GSM_ConstructionPoint(int iGuildGUID, int iPoint)
{
	int i;

	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != NULL) && (m_pClientList[i]->m_iCrusadeDuty == 3) &&
			(m_pClientList[i]->m_iGuildGUID == iGuildGUID)) {
			// ÇØ´ç ±æµå¸¶½ºÅÍ ÁöÈÖ°üÀ» Ã£¾Ò´Ù.
			m_pClientList[i]->m_iConstructionPoint += iPoint;
			m_pClientList[i]->m_iWarContribution += iPoint / 10;

			SendNotifyMsg(NULL, i, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[i]->m_iConstructionPoint, m_pClientList[i]->m_iWarContribution, NULL, NULL);

			break;
		}
}

void CGame::ManualEndCrusadeMode(int iWinnerSide)
{
	char* cp, cData[256];
	WORD* wp;

	// ¸Å´º¾ó·Î Å©·ç¼¼ÀÌµå ¸ðµå¸¦ Á¾·á½ÃÅ²´Ù. ºñ±ä »óÅÂ·Î Á¾·á½ÃÅ²´Ù.
	if (m_bIsCrusadeMode == FALSE) return;

	LocalEndCrusadeMode(iWinnerSide);

	// ´Ù¸¥ ¼­¹ö¿¡ Å©·ç¼¼ÀÌµå Á¾·á¸¦ ¾Ë¸².
	ZeroMemory(cData, sizeof(cData));
	cp = (char*)(cData);
	*cp = GSM_ENDCRUSADE;
	cp++;

	*cp = (char)iWinnerSide;
	cp++;

	wp = (WORD*)cp;
	*wp = NULL;
	cp += 2;

	wp = (WORD*)cp;
	*wp = NULL;
	cp += 2;

	wp = (WORD*)cp;
	*wp = NULL;
	cp += 2;

	cp += 10;

	bStockMsgToGateServer(cData, 18);
}

// New 13/05/2004
BOOL CGame::_bCrusadeLog(int iAction, int iClientH, int iData, char* cName)
{
	char  cTxt[200];

	//  ·Î±× ³²±ä´Ù. 
	ZeroMemory(cTxt, sizeof(cTxt));

	switch (iAction) {

	case DEF_CRUSADELOG_ENDCRUSADE:
		if (cName == NULL) return FALSE;
		wsprintf(cTxt, "\tEnd Crusade\t%s", cName);
		break;

	case DEF_CRUSADELOG_SELECTDUTY:
		if (cName == NULL) return FALSE;
		if (m_pClientList[iClientH] == NULL) return FALSE;
		wsprintf(cTxt, "PC(%s)\tSelect Duty\t(%s)\t \t(%s)", m_pClientList[iClientH]->m_cCharName, cName, m_pClientList[iClientH]->m_cGuildName);
		break;

	case DEF_CRUSADELOG_GETEXP:
		if (m_pClientList[iClientH] == NULL) return FALSE;
		wsprintf(cTxt, "PC(%s)\tGet Exp\t(%d)\t \t(%s)", m_pClientList[iClientH]->m_cCharName, iData, m_pClientList[iClientH]->m_cGuildName);
		break;

	case DEF_CRUSADELOG_STARTCRUSADE:
		wsprintf(cTxt, "\tStart Crusade");
		break;

	default:
		return FALSE;
	}

	bSendMsgToLS(MSGID_GAMECRUSADELOG, iClientH, NULL, cTxt);
	return TRUE;
}

void CGame::CrusadeWarStarter()
{
	SYSTEMTIME SysTime;
	int i;

	if (m_bIsCrusadeMode == TRUE) return;
	if (m_bIsApocalypseMode == TRUE) return;
	if (m_bIsHeldenianMode) return;
	if (m_bIsCrusadeWarStarter == FALSE) return;

	// ���� ���� 1�������� 
	GetLocalTime(&SysTime);

	for (i = 0; i < DEF_MAXSCHEDULE; i++)
		if ((m_stCrusadeWarSchedule[i].iDay == SysTime.wDayOfWeek) &&
			(m_stCrusadeWarSchedule[i].iHour == SysTime.wHour) &&
			(m_stCrusadeWarSchedule[i].iMinute == SysTime.wMinute)) {
			PutLogList("(!) Automated Crusade is being initiated!");
			GlobalStartCrusadeMode();
			break;
		}
}

void CGame::GlobalStartCrusadeMode()
{
	char* cp, cData[120];
	DWORD* dwp, dwCrusadeGUID;
	SYSTEMTIME SysTime;

	// ������ ���� �Ϸ翡 �ι� �������� �߻��ϴ� ���� ���� �ڵ� 
	GetLocalTime(&SysTime);
	if (m_iLatestCrusadeDayOfWeek != -1) {
		if (m_iLatestCrusadeDayOfWeek == SysTime.wDayOfWeek) return;
	}
	else m_iLatestCrusadeDayOfWeek = SysTime.wDayOfWeek;

	dwCrusadeGUID = timeGetTime();

	ZeroMemory(cData, sizeof(cData));
	cp = (char*)cData;
	*cp = GSM_BEGINCRUSADE;
	cp++;
	dwp = (DWORD*)cp;
	*dwp = dwCrusadeGUID;
	cp += 4;

	bStockMsgToGateServer(cData, 5);

	LocalStartCrusadeMode(dwCrusadeGUID);
}

void CGame::RequestGuildTeleportHandler(int iClientH)
{
	int i;
	char cMapName[11];

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_iLockedMapTime != NULL) {
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_LOCKEDMAP, m_pClientList[iClientH]->m_iLockedMapTime, NULL, NULL, m_pClientList[iClientH]->m_cLockedMapName);
		return;
	}

	// if a guild teleport is set when its not a crusade, log the hacker
	if (!m_bIsCrusadeMode) {
		wsprintf(G_cTxt, "Accessing crusade teleport: (%s) Player: (%s) - setting teleport location when crusade is disabled.",
			m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
		PutHackLogFileList(G_cTxt);
		DeleteClient(iClientH, TRUE, TRUE);
		return;
	}

	// if a player is using guild teleport and he is not in a guild, log the hacker
	if (m_pClientList[iClientH]->m_iCrusadeDuty == 0) {
		wsprintf(G_cTxt, "Accessing crusade teleport: (%s) Player: (%s) - teleporting when not in a guild",
			m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
		PutHackLogFileList(G_cTxt);
		DeleteClient(iClientH, TRUE, TRUE);
		return;
	}

	if ((m_pClientList[iClientH]->m_cMapIndex == m_iMiddlelandMapIndex) &&
		m_iMiddlelandMapIndex != -1)
		return;

	for (i = 0; i < DEF_MAXGUILDS; i++)
		if (m_pGuildTeleportLoc[i].m_iV1 == m_pClientList[iClientH]->m_iGuildGUID) {
			// ��带 ã�Ҵ�.
			ZeroMemory(cMapName, sizeof(cMapName));
			strcpy(cMapName, m_pGuildTeleportLoc[i].m_cDestMapName);



			// !!! RequestTeleportHandler������ m_cMapName�� ���� ������ �״�� �Ķ���ͷ� �Ѱ��ָ� ������
			RequestTeleportHandler(iClientH, "2   ", cMapName, m_pGuildTeleportLoc[i].m_sDestX, m_pGuildTeleportLoc[i].m_sDestY);
			break;
		}

}


void CGame::GSM_SetGuildTeleportLoc(int iGuildGUID, int dX, int dY, char* pMapName)
{
	int i, iIndex;
	DWORD dwTemp, dwTime;

	// Ŭ���̾�Ʈ�κ����� ��û�� �ƴ϶� �ٸ� �����κ����� �ڷ���Ʈ ��ǥ ���� ��û��. ������ �ʿ� ����.

	dwTime = timeGetTime();

	// ���� ���� GUID�� ���� ��� �ڷ���Ʈ ��ǥ�� �����Ǿ� �ִ��� ã�´�.
	for (i = 0; i < DEF_MAXGUILDS; i++)
		if (m_pGuildTeleportLoc[i].m_iV1 == iGuildGUID) {
			// �̹� ��尡 �ִ�.
			if ((m_pGuildTeleportLoc[i].m_sDestX == dX) && (m_pGuildTeleportLoc[i].m_sDestY == dY) && (strcmp(m_pGuildTeleportLoc[i].m_cDestMapName, pMapName) == 0)) {
				// ������ ��ġ�Ѵ�. Ÿ�� �Է��� ��ŵ.
				m_pGuildTeleportLoc[i].m_dwTime = dwTime;
				return;
			}
			else {
				// ���� ����
				m_pGuildTeleportLoc[i].m_sDestX = dX;
				m_pGuildTeleportLoc[i].m_sDestY = dY;
				ZeroMemory(m_pGuildTeleportLoc[i].m_cDestMapName, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName));
				strcpy(m_pGuildTeleportLoc[i].m_cDestMapName, pMapName);
				m_pGuildTeleportLoc[i].m_dwTime = dwTime;
				return;
			}
		}

	// ������ ������ ����. ���� �����Ѵ�.
	dwTemp = 0;
	iIndex = -1;
	for (i = 0; i < DEF_MAXGUILDS; i++) {
		if (m_pGuildTeleportLoc[i].m_iV1 == NULL) {

			m_pGuildTeleportLoc[i].m_iV1 = iGuildGUID;
			m_pGuildTeleportLoc[i].m_sDestX = dX;
			m_pGuildTeleportLoc[i].m_sDestY = dY;
			ZeroMemory(m_pGuildTeleportLoc[i].m_cDestMapName, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName));
			strcpy(m_pGuildTeleportLoc[i].m_cDestMapName, pMapName);
			m_pGuildTeleportLoc[i].m_dwTime = dwTime;
			return;
		}
		else {
			// ���� �������� �������� ���� ����Ʈ�� �ε����� ã�´�.
			if (dwTemp < (dwTime - m_pGuildTeleportLoc[i].m_dwTime)) {
				dwTemp = (dwTime - m_pGuildTeleportLoc[i].m_dwTime);
				iIndex = i;
			}
		}
	}

	m_pGuildTeleportLoc[i].m_iV1 = iGuildGUID;
	m_pGuildTeleportLoc[i].m_sDestX = dX;
	m_pGuildTeleportLoc[i].m_sDestY = dY;
	ZeroMemory(m_pGuildTeleportLoc[i].m_cDestMapName, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName));
	strcpy(m_pGuildTeleportLoc[i].m_cDestMapName, pMapName);
	m_pGuildTeleportLoc[i].m_dwTime = dwTime;
}


void CGame::RequestSetGuildTeleportLocHandler(int iClientH, int dX, int dY, int iGuildGUID, char* pMapName)
{
	char* cp, cData[120];
	int i;
	int* ip, iIndex;
	DWORD dwTemp, dwTime;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsOnServerChange == TRUE) return;

	// if a player is teleporting and its not a crusade, log the hacker
	if (!m_bIsCrusadeMode) {
		wsprintf(G_cTxt, "Accessing Crusade Set Teleport:(%s) Player: (%s) - setting point when not a crusade.",
			m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
		PutHackLogFileList(G_cTxt);
		DeleteClient(iClientH, TRUE, TRUE);
		return;
	}

	// if a player is teleporting and its not a crusade, log the hacker
	if (m_pClientList[iClientH]->m_iCrusadeDuty != 3) {
		wsprintf(G_cTxt, "Accessing Crusade Set Teleport: (%s) Player: (%s) - setting point when not a guildmaster.",
			m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
		PutHackLogFileList(G_cTxt);
		DeleteClient(iClientH, TRUE, TRUE);
		return;
	}

	// v2.04 ��ǥ ���� 
	if (dY < 100) dY = 100;
	if (dY > 600) dY = 600;

	// ����Ʈ ���� �޽��� �ۼ� 
	ZeroMemory(cData, sizeof(cData));
	cp = (char*)cData;
	*cp = GSM_SETGUILDTELEPORTLOC;
	cp++;

	ip = (int*)cp;
	*ip = iGuildGUID;
	cp += 4;

	ip = (int*)cp;
	*ip = dX;
	cp += 4;

	ip = (int*)cp;
	*ip = dY;
	cp += 4;

	memcpy(cp, pMapName, 10);
	cp += 10;
	//

	dwTime = timeGetTime();

	// ���� ���� GUID�� ���� ��� �ڷ���Ʈ ��ǥ�� �����Ǿ� �ִ��� ã�´�.
	for (i = 0; i < DEF_MAXGUILDS; i++)
		if (m_pGuildTeleportLoc[i].m_iV1 == iGuildGUID) {
			// �̹� ��尡 �ִ�.
			if ((m_pGuildTeleportLoc[i].m_sDestX == dX) && (m_pGuildTeleportLoc[i].m_sDestY == dY) && (strcmp(m_pGuildTeleportLoc[i].m_cDestMapName, pMapName) == 0)) {
				// ������ ��ġ�Ѵ�. Ÿ�� �Է��� ��ŵ.
				m_pGuildTeleportLoc[i].m_dwTime = dwTime;
				return;
			}
			else {
				// ���� ������ 
				m_pGuildTeleportLoc[i].m_sDestX = dX;
				m_pGuildTeleportLoc[i].m_sDestY = dY;
				ZeroMemory(m_pGuildTeleportLoc[i].m_cDestMapName, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName));
				strcpy(m_pGuildTeleportLoc[i].m_cDestMapName, pMapName);
				m_pGuildTeleportLoc[i].m_dwTime = dwTime;

				//����Ʈ ������ ���� �ٸ� ������ ���� ����
				bStockMsgToGateServer(cData, 23);
				return;
			}
		}

	// ������ ������ ����. ���� �����Ѵ�.
	dwTemp = 0;
	iIndex = -1;
	for (i = 0; i < DEF_MAXGUILDS; i++) {
		if (m_pGuildTeleportLoc[i].m_iV1 == NULL) {

			m_pGuildTeleportLoc[i].m_iV1 = iGuildGUID;
			m_pGuildTeleportLoc[i].m_sDestX = dX;
			m_pGuildTeleportLoc[i].m_sDestY = dY;
			ZeroMemory(m_pGuildTeleportLoc[i].m_cDestMapName, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName));
			strcpy(m_pGuildTeleportLoc[i].m_cDestMapName, pMapName);
			m_pGuildTeleportLoc[i].m_dwTime = dwTime;

			//����Ʈ ������ ���� �ٸ� ������ ���� ����  
			bStockMsgToGateServer(cData, 23);
			return;
		}
		else {
			// ���� �������� �������� ���� ����Ʈ�� �ε����� ã�´�.
			if (dwTemp < (dwTime - m_pGuildTeleportLoc[i].m_dwTime)) {
				dwTemp = (dwTime - m_pGuildTeleportLoc[i].m_dwTime);
				iIndex = i;
			}
		}
	}

	m_pGuildTeleportLoc[i].m_iV1 = iGuildGUID;
	m_pGuildTeleportLoc[i].m_sDestX = dX;
	m_pGuildTeleportLoc[i].m_sDestY = dY;
	ZeroMemory(m_pGuildTeleportLoc[i].m_cDestMapName, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName));
	strcpy(m_pGuildTeleportLoc[i].m_cDestMapName, pMapName);
	m_pGuildTeleportLoc[i].m_dwTime = dwTime;

	//����Ʈ ������ ���� �ٸ� ������ ���� ����
	bStockMsgToGateServer(cData, 23);
}

// New 12/05/2004 Changed
void CGame::MeteorStrikeHandler(int iMapIndex)
{
	int i, ix, iy, dX, dY, iIndex, iTargetIndex, iTotalESG, iEffect;
	int iTargetArray[DEF_MAXSTRIKEPOINTS];
	short sOwnerH;
	char  cOwnerType;
	DWORD dwTime = timeGetTime();

	PutLogList("(!) Beginning Meteor Strike Procedure...");

	if (iMapIndex == -1) {
		PutLogList("(X) MeteorStrikeHandler Error! MapIndex -1!");
		return;
	}

	if (m_pMapList[iMapIndex] == NULL) {
		PutLogList("(X) MeteorStrikeHandler Error! NULL Map!");
		return;
	}

	if (m_pMapList[iMapIndex]->m_iTotalStrikePoints == 0) {
		PutLogList("(X) MeteorStrikeHandler Error! No Strike Points!");
		return;
	}

	for (i = 0; i < DEF_MAXSTRIKEPOINTS; i++) iTargetArray[i] = -1;

	iIndex = 0;
	for (i = 1; i <= m_pMapList[iMapIndex]->m_iTotalStrikePoints; i++) {
		if (m_pMapList[iMapIndex]->m_stStrikePoint[i].iHP > 0) {
			iTargetArray[iIndex] = i; // ½ºÆ®¶óÀÌÅ© Æ÷ÀÎÆ®ÀÇ ÀÎµ¦½º¸¦ ³Ö´Â´Ù.
			iIndex++;
		}
	}

	//testcode
	wsprintf(G_cTxt, "(!) Map(%s) has %d available strike points", m_pMapList[iMapIndex]->m_cName, iIndex);
	PutLogList(G_cTxt);

	// °á°ú º¸°í¿ë ½ºÆ®·°Ãç Å¬¸®¾î
	m_stMeteorStrikeResult.iCasualties = 0;
	m_stMeteorStrikeResult.iCrashedStructureNum = 0;
	m_stMeteorStrikeResult.iStructureDamageAmount = 0;

	if (iIndex == 0) {
		// Æø°ÝÇÒ °Ç¹°ÀÌ ¾ø´Ù. ¸ðµÎ ÆÄ±«µÇ¾ú´Ù.
		PutLogList("(!) No strike points!");
		bRegisterDelayEvent(DEF_DELAYEVENTTYPE_CALCMETEORSTRIKEEFFECT, NULL, dwTime + 6000, NULL, NULL, iMapIndex, NULL, NULL, NULL, NULL, NULL);
	}
	else {

		// ÇØ´ç ¸ÊÀÇ Å¬¶óÀÌ¾ðÆ®µé¿¡°Ô ¸ÞÅ×¿À °ø°Ý È¿°ú¸¦ º¸¿©ÁÖ¶ó´Â ¸Þ½ÃÁö Àü¼Û
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != NULL) && (m_pClientList[i]->m_bIsInitComplete == TRUE) && (m_pClientList[i]->m_cMapIndex == iMapIndex)) {
				SendNotifyMsg(NULL, i, DEF_NOTIFY_METEORSTRIKEHIT, NULL, NULL, NULL, NULL);
			}

		for (i = 0; i < iIndex; i++) {
			// ¸ðµç Æø°Ý ¸ñÇ¥¿¡ ´ëÇØ µ¿ÀÏÇÑ °ø°ÝÀ» °¡ÇÑ´Ù.
			iTargetIndex = iTargetArray[i];

			if (iTargetIndex == -1) {
				PutLogList("(X) Strike Point MapIndex: -1!");
				goto MSH_SKIP_STRIKE;
			}

			dX = m_pMapList[iMapIndex]->m_stStrikePoint[iTargetIndex].dX;
			dY = m_pMapList[iMapIndex]->m_stStrikePoint[iTargetIndex].dY;

			// dX, dY À§Ä¡ ÁÖÀ§¿¡ ÃÖ¼Ò 2°³ ÀÌ»óÀÇ Energy Shield Generator°¡ Á¸ÀçÇÏ¸é ½ºÆ®¶óÀÌÅ© Æ÷ÀÎÆ®´Â ¹«»çÇÏ´Ù. ±×·¯³ª 1°³ ÀÌÇÏÀÌ¸é °ø°Ý¹Þ¾ÒÀ»¶§ HP°¡ ÁÙ¾îµç´Ù.
			// NPC ÁÖº¯¿¡ ÀÖ´Â Àû Ä³¸¯ÅÍ¸¦ Å½ÁöÇØ °æº¸¸¦ ¿ï¸°´Ù.
			iTotalESG = 0;
			for (ix = dX - 10; ix <= dX + 10; ix++)
				for (iy = dY - 10; iy <= dY + 10; iy++) {
					m_pMapList[iMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
					if ((cOwnerType == DEF_OWNERTYPE_NPC) && (m_pNpcList[sOwnerH] != NULL) && (m_pNpcList[sOwnerH]->m_sType == 40)) {
						iTotalESG++;
					}
				}

			// testcode
			wsprintf(G_cTxt, "(!) Meteor Strike Target(%d, %d) ESG(%d)", dX, dY, iTotalESG);
			PutLogList(G_cTxt);

			if (iTotalESG < 2) {

				m_pMapList[iMapIndex]->m_stStrikePoint[iTargetIndex].iHP -= (2 - iTotalESG);
				if (m_pMapList[iMapIndex]->m_stStrikePoint[iTargetIndex].iHP <= 0) {
					// °Ç¹°ÀÌ ÆÄ±«µÇ¾ú´Ù.
					m_pMapList[iMapIndex]->m_stStrikePoint[iTargetIndex].iHP = 0;
					// °Ç¹° ±â´É »ó½Ç
					m_pMapList[m_pMapList[iMapIndex]->m_stStrikePoint[iTargetIndex].iMapIndex]->m_bIsDisabled = TRUE;
					m_stMeteorStrikeResult.iCrashedStructureNum++;
				}
				else {
					// °Ç¹°ÀÌ ´ë¹ÌÁö¸¦ ¾ò¾ú´Ù.
					m_stMeteorStrikeResult.iStructureDamageAmount += (2 - iTotalESG);
					// °Ç¹° ´ë¹ÌÁö ÀÔÀº ÈÄ ºÒÅ¸´Â ÀÌ¹ÌÁö »ðÀÔ 
					iEffect = iDice(1, 5) - 1;
					iAddDynamicObjectList(NULL, DEF_OWNERTYPE_PLAYER_INDIRECT, DEF_DYNAMICOBJECT_FIRE2, iMapIndex,
						m_pMapList[iMapIndex]->m_stStrikePoint[iTargetIndex].iEffectX[iEffect] + (iDice(1, 3) - 2),
						m_pMapList[iMapIndex]->m_stStrikePoint[iTargetIndex].iEffectY[iEffect] + (iDice(1, 3) - 2), 60 * 1000 * 50);
				}
			}
		MSH_SKIP_STRIKE:;
		}

		// 1ÃÊ ÈÄ ¸ÞÅ×¿À ½ºÆ®¶óÀÌÅ© °ø°Ý ´ë¹ÌÁö È¿°ú
		bRegisterDelayEvent(DEF_DELAYEVENTTYPE_DOMETEORSTRIKEDAMAGE, NULL, dwTime + 1000, NULL, NULL, iMapIndex, NULL, NULL, NULL, NULL, NULL);
		// 4ÃÊ ÈÄ ¸ÞÅ×¿À ½ºÆ®¶óÀÌÅ© °ø°Ý ´ë¹ÌÁö È¿°ú
		bRegisterDelayEvent(DEF_DELAYEVENTTYPE_DOMETEORSTRIKEDAMAGE, NULL, dwTime + 4000, NULL, NULL, iMapIndex, NULL, NULL, NULL, NULL, NULL);
		// 6ÃÊ ÈÄ °ø°Ý °á°ú ÆÇ´Ü.
		bRegisterDelayEvent(DEF_DELAYEVENTTYPE_CALCMETEORSTRIKEEFFECT, NULL, dwTime + 6000, NULL, NULL, iMapIndex, NULL, NULL, NULL, NULL, NULL);
	}
}

void CGame::CalcMeteorStrikeEffectHandler(int iMapIndex)
{
	int i, iActiveStructure, iStructureHP[DEF_MAXSTRIKEPOINTS];
	char* cp, * cp2, cData[120], cWinnerSide, cTempData[120];
	WORD* wp;

	if (m_bIsCrusadeMode == FALSE) return;
	for (i = 0; i < DEF_MAXSTRIKEPOINTS; i++) iStructureHP[i] = 0;
	iActiveStructure = 0;
	for (i = 1; i <= m_pMapList[iMapIndex]->m_iTotalStrikePoints; i++) {
		if (m_pMapList[iMapIndex]->m_stStrikePoint[i].iHP > 0) {
			iActiveStructure++;
			iStructureHP[i] = m_pMapList[iMapIndex]->m_stStrikePoint[i].iHP;
		}
	}
	wsprintf(G_cTxt, "ActiveStructure:%d  MapIndex:%d AresdenMap:%d ElvineMap:%d", iActiveStructure, iMapIndex, m_iAresdenMapIndex, m_iElvineMapIndex);
	PutLogList(G_cTxt);
	if (iActiveStructure == 0) {
		if (iMapIndex == m_iAresdenMapIndex) {
			cWinnerSide = 2;
			LocalEndCrusadeMode(2);
		}
		else if (iMapIndex == m_iElvineMapIndex) {
			cWinnerSide = 1;
			LocalEndCrusadeMode(1);
		}
		else {
			cWinnerSide = 0;
			LocalEndCrusadeMode(0);
		}

		ZeroMemory(cData, sizeof(cData));
		cp = (char*)(cData);
		*cp = GSM_ENDCRUSADE;
		cp++;

		*cp = cWinnerSide;
		cp++;

		wp = (WORD*)cp;
		*wp = m_stMeteorStrikeResult.iCrashedStructureNum;
		cp += 2;

		wp = (WORD*)cp;
		*wp = m_stMeteorStrikeResult.iStructureDamageAmount;
		cp += 2;

		wp = (WORD*)cp;
		*wp = m_stMeteorStrikeResult.iCasualties;
		cp += 2;

		memcpy(cp, m_pMapList[iMapIndex]->m_cName, 10);
		cp += 10;

		bStockMsgToGateServer(cData, 18);
	}
	else {
		ZeroMemory(cData, sizeof(cData));
		cp = (char*)(cData);
		*cp = GSM_GRANDMAGICRESULT;
		cp++;

		memcpy(cp, m_pMapList[iMapIndex]->m_cName, 10);
		cp += 10;

		wp = (WORD*)cp;
		*wp = m_stMeteorStrikeResult.iCrashedStructureNum;
		cp += 2;

		wp = (WORD*)cp;
		*wp = m_stMeteorStrikeResult.iStructureDamageAmount;
		cp += 2;

		wp = (WORD*)cp;
		*wp = m_stMeteorStrikeResult.iCasualties;
		cp += 2;

		wp = (WORD*)cp;
		*wp = (WORD)iActiveStructure;
		cp += 2;

		ZeroMemory(cTempData, sizeof(cTempData));
		cp2 = (char*)(cTempData);

		wp = (WORD*)cp2;
		*wp = (WORD)m_pMapList[iMapIndex]->m_iTotalStrikePoints;
		cp2 += 2;

		for (i = 1; i <= m_pMapList[iMapIndex]->m_iTotalStrikePoints;i++) {
			wp = (WORD*)cp2;
			*wp = (WORD)iStructureHP[i];
			cp2 += 2;
		}
		memcpy(cp, cTempData, (m_pMapList[iMapIndex]->m_iTotalStrikePoints * 2) + 2);
		bStockMsgToGateServer(cData, (m_pMapList[iMapIndex]->m_iTotalStrikePoints * 2) + 20);
		GrandMagicResultHandler(m_pMapList[iMapIndex]->m_cName, m_stMeteorStrikeResult.iCrashedStructureNum, m_stMeteorStrikeResult.iStructureDamageAmount, m_stMeteorStrikeResult.iCasualties, iActiveStructure, m_pMapList[iMapIndex]->m_iTotalStrikePoints, cTempData);
	}
	m_stMeteorStrikeResult.iCasualties = 0;
	m_stMeteorStrikeResult.iCrashedStructureNum = 0;
	m_stMeteorStrikeResult.iStructureDamageAmount = 0;
}

void CGame::LocalEndCrusadeMode(int iWinnerSide)
{
	int i, n;

	if (m_bIsCrusadeMode == FALSE) return;

	m_bIsCrusadeMode = FALSE;
	PutLogList("(!)Crusade Mode OFF.");

	for (n = 0; n < DEF_MAXNPCS; n++)
		if (m_pNpcList[n] != NULL) {
			switch (m_pNpcList[n]->m_sType) {
			case 36:
			case 37:
			case 38:
			case 39:
			case 40:
			case 41:
			case 42:
			case 43:
			case 44:
			case 45:
			case 46:
			case 47:
			case 51:
				RemoveEventNpc(n);
				break;
			}
		}

	_CreateCrusadeGUID(m_dwCrusadeGUID, iWinnerSide);
	m_iCrusadeWinnerSide = iWinnerSide;
	m_iLastCrusadeWinner = iWinnerSide;

	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != NULL) && (m_pClientList[i]->m_bIsInitComplete == TRUE)) {
			m_pClientList[i]->m_iCrusadeDuty = 0;
			m_pClientList[i]->m_iConstructionPoint = 0;
			m_pClientList[i]->m_iTimeLeft_ForceRecall = 0;
			SendNotifyMsg(NULL, i, DEF_NOTIFY_CRUSADE, (DWORD)m_bIsCrusadeMode, NULL, NULL, NULL, m_iCrusadeWinnerSide);
		}

	if (iWinnerSide == 2) {
		_bCrusadeLog(DEF_CRUSADELOG_ENDCRUSADE, FALSE, FALSE, "Elvine Win!");
	}
	else if (iWinnerSide == 1) {
		_bCrusadeLog(DEF_CRUSADELOG_ENDCRUSADE, FALSE, FALSE, "Aresden Win!");
	}
	else {
		_bCrusadeLog(DEF_CRUSADELOG_ENDCRUSADE, FALSE, FALSE, "Draw!");
	}
}

void CGame::SendCollectedMana()
{
	char* cp, cData[120];
	WORD* wp;

	// ���� ������ ���ٸ� �޽��� �������� �ʴ´�. ���� ������ ���� �ʿ��� �޽����� �����ϸ� �ȵȴ�.
	if ((m_iCollectedMana[1] == 0) && (m_iCollectedMana[2] == 0)) return;

	ZeroMemory(cData, sizeof(cData));
	cp = (char*)(cData);
	*cp = GSM_COLLECTEDMANA;
	cp++;

	wp = (WORD*)cp;
	*wp = (WORD)m_iCollectedMana[1];
	cp += 2;

	wp = (WORD*)cp;
	*wp = (WORD)m_iCollectedMana[2];
	cp += 2;

	CollectedManaHandler(m_iCollectedMana[1], m_iCollectedMana[2]);

	bStockMsgToGateServer(cData, 5);

	// �׵��� ���� ������ ������ Ŭ����.
	m_iCollectedMana[0] = 0;
	m_iCollectedMana[1] = 0;
	m_iCollectedMana[2] = 0;
}

void CGame::CollectedManaHandler(WORD wAresdenMana, WORD wElvineMana)
{
	if (m_iAresdenMapIndex != -1) {
		m_iAresdenMana += wAresdenMana;

	}

	if (m_iElvineMapIndex != -1) {
		m_iElvineMana += wElvineMana;

	}
}

void CGame::_LinkStrikePointMapIndex()
{
	int i, z, x;

	// ������ �ǹ� ���θ� �ٸ� ������ ���� ��ũ�� �Ұ����ϹǷ� �ٿ�ȴ�!
	for (i = 0; i < DEF_MAXMAPS; i++)
		if ((m_pMapList[i] != NULL) && (m_pMapList[i]->m_iTotalStrikePoints != 0)) {
			// ��Ʈ����ũ ����Ʈ�� ���ǵ� ���̴�. ������ ��Ʈ����ũ ����Ʈ�� ����� ���� �ε����� ���� �����Ѵ�.
			for (z = 0; z < DEF_MAXSTRIKEPOINTS; z++)
				if (strlen(m_pMapList[i]->m_stStrikePoint[z].cRelatedMapName) != 0) {
					for (x = 0; x < DEF_MAXMAPS; x++)
						if ((m_pMapList[x] != NULL) && (strcmp(m_pMapList[x]->m_cName, m_pMapList[i]->m_stStrikePoint[z].cRelatedMapName) == 0)) {
							// �ε����� ���ߴ�. �����Ѵ�.
							m_pMapList[i]->m_stStrikePoint[z].iMapIndex = x;
							//testcode
							wsprintf(G_cTxt, "(!) Map(%s) Strike Point(%d) Related Map(%s) Index(%d)", m_pMapList[i]->m_cName, z, m_pMapList[i]->m_stStrikePoint[z].cRelatedMapName, x);
							PutLogList(G_cTxt);

							break;
						}
				}
		}
}

//New Changed 11/05/2004
void CGame::GrandMagicResultHandler(char* cMapName, int iCrashedStructureNum, int iStructureDamageAmount, int iCasualities, int iActiveStructure, int iTotalStrikePoints, char* cData)
{
	int i;

	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if (m_pClientList[i] != NULL) {
			SendNotifyMsg(NULL, i, DEF_NOTIFY_GRANDMAGICRESULT, iCrashedStructureNum, iStructureDamageAmount, iCasualities, cMapName, iActiveStructure, NULL, NULL, NULL, NULL, iTotalStrikePoints, cData);
		}
}

// October 19, 2004 - 3.51 translated
void CGame::_GrandMagicLaunchMsgSend(int iType, char cAttackerSide)
{
	char* cp, cBuff[120];
	WORD* wp;

	ZeroMemory(cBuff, sizeof(cBuff));
	cp = (char*)cBuff;
	*cp = GSM_GRANDMAGICLAUNCH;
	cp++;

	wp = (WORD*)cp;
	*wp = (WORD)iType;
	cp += 2;

	wp = (WORD*)cp;
	*wp = (WORD)cAttackerSide;
	cp += 2;

	bStockMsgToGateServer(cBuff, 5);
}

// October 19, 2004 - 3.51 translated
BOOL CGame::_bNpcBehavior_ManaCollector(int iNpcH)
{
	int dX, dY, iMaxMP, iTotal;
	short sOwnerH;
	char  cOwnerType;
	double dV1, dV2, dV3;
	BOOL bRet;

	if (m_pNpcList[iNpcH] == NULL) return FALSE;
	if (m_pNpcList[iNpcH]->m_sAppr2 != 0) return FALSE;

	bRet = FALSE;
	for (dX = m_pNpcList[iNpcH]->m_sX - 5; dX <= m_pNpcList[iNpcH]->m_sX + 5; dX++)
		for (dY = m_pNpcList[iNpcH]->m_sY - 5; dY <= m_pNpcList[iNpcH]->m_sY + 5; dY++) {
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
			if (sOwnerH != NULL) {
				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pNpcList[iNpcH]->m_cSide == m_pClientList[sOwnerH]->m_cSide) {
						iMaxMP = (2 * (m_pClientList[sOwnerH]->m_iMag + m_pClientList[sOwnerH]->m_iAngelicMag)) + (2 * (m_pClientList[sOwnerH]->m_iLevel + m_pClientList[sOwnerH]->m_iMajesticLevel)) + ((m_pClientList[sOwnerH]->m_iInt + m_pClientList[sOwnerH]->m_iAngelicInt) / 2);
						if (m_pClientList[sOwnerH]->m_iMP < iMaxMP) {
							iTotal = iDice(1, (m_pClientList[sOwnerH]->m_iMag + m_pClientList[sOwnerH]->m_iAngelicMag));
							if (m_pClientList[sOwnerH]->m_iAddMP != 0) {
								dV2 = (double)iTotal;
								dV3 = (double)m_pClientList[sOwnerH]->m_iAddMP;
								dV1 = (dV3 / 100.0f) * dV2;
								iTotal += (int)dV1;
							}

							m_pClientList[sOwnerH]->m_iMP += iTotal;

							if (m_pClientList[sOwnerH]->m_iMP > iMaxMP)
								m_pClientList[sOwnerH]->m_iMP = iMaxMP;

							SendNotifyMsg(NULL, sOwnerH, DEF_NOTIFY_MP, NULL, NULL, NULL, NULL);
						}
					}
					break;

				case DEF_OWNERTYPE_NPC:
					if ((m_pNpcList[sOwnerH]->m_sType == 42) && (m_pNpcList[sOwnerH]->m_iV1 > 0)) {
						if (m_pNpcList[sOwnerH]->m_iV1 >= 3) {
							m_iCollectedMana[m_pNpcList[iNpcH]->m_cSide] += 3;
							m_pNpcList[sOwnerH]->m_iV1 -= 3;
							bRet = TRUE;
						}
						else {
							m_iCollectedMana[m_pNpcList[iNpcH]->m_cSide] += m_pNpcList[sOwnerH]->m_iV1;
							m_pNpcList[sOwnerH]->m_iV1 = 0;
							bRet = TRUE;
						}
					}
					break;
				}
			}
		}
	return bRet;
}

// October 19, 2004 - 3.51 translated
void CGame::MeteorStrikeMsgHandler(char cAttackerSide)
{
	int i;
	DWORD dwTime = timeGetTime();

	switch (cAttackerSide) {
	case 1:
		if (m_iElvineMapIndex != -1) {
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if ((m_pClientList[i] != NULL) && (m_pClientList[i]->m_bIsInitComplete == TRUE)) {
					if (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cLocationName, "elvine") == 0) {
						SendNotifyMsg(NULL, i, DEF_NOTIFY_METEORSTRIKECOMING, 1, NULL, NULL, NULL);
					}
					else {
						SendNotifyMsg(NULL, i, DEF_NOTIFY_METEORSTRIKECOMING, 2, NULL, NULL, NULL);
					}
				}
			bRegisterDelayEvent(DEF_DELAYEVENTTYPE_METEORSTRIKE, NULL, dwTime + 5000, NULL, NULL, m_iElvineMapIndex, NULL, NULL, NULL, NULL, NULL);
		}
		else {
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if (m_pClientList[i] != NULL) {
					SendNotifyMsg(NULL, i, DEF_NOTIFY_METEORSTRIKECOMING, 2, NULL, NULL, NULL);
				}
		}
		break;

	case 2:
		if (m_iAresdenMapIndex != -1) {
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if ((m_pClientList[i] != NULL) && (m_pClientList[i]->m_bIsInitComplete == TRUE)) {
					if (strcmp(m_pMapList[m_pClientList[i]->m_cMapIndex]->m_cLocationName, "aresden") == 0) {
						SendNotifyMsg(NULL, i, DEF_NOTIFY_METEORSTRIKECOMING, 3, NULL, NULL, NULL);
					}
					else {
						SendNotifyMsg(NULL, i, DEF_NOTIFY_METEORSTRIKECOMING, 4, NULL, NULL, NULL);
					}
				}
			bRegisterDelayEvent(DEF_DELAYEVENTTYPE_METEORSTRIKE, NULL, dwTime + 1000 * 5, NULL, NULL, m_iAresdenMapIndex, NULL, NULL, NULL, NULL, NULL);
		}
		else {
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if (m_pClientList[i] != NULL) {
					SendNotifyMsg(NULL, i, DEF_NOTIFY_METEORSTRIKECOMING, 4, NULL, NULL, NULL);
				}
		}
		break;
	}
}

// October 19, 2004 - 3.51 translated
void CGame::_NpcBehavior_GrandMagicGenerator(int iNpcH)
{
	switch (m_pNpcList[iNpcH]->m_cSide) {
	case 1:
		if (m_iAresdenMana > DEF_GMGMANACONSUMEUNIT) {
			m_iAresdenMana = 0;
			m_pNpcList[iNpcH]->m_iManaStock++;
			if (m_pNpcList[iNpcH]->m_iManaStock > m_pNpcList[iNpcH]->m_iMaxMana) {
				_GrandMagicLaunchMsgSend(1, 1);
				MeteorStrikeMsgHandler(1);
				m_pNpcList[iNpcH]->m_iManaStock = 0;
				m_iAresdenMana = 0;
			}
		}
		break;

	case 2:
		if (m_iElvineMana > DEF_GMGMANACONSUMEUNIT) {
			m_iElvineMana = 0;
			m_pNpcList[iNpcH]->m_iManaStock++;
			if (m_pNpcList[iNpcH]->m_iManaStock > m_pNpcList[iNpcH]->m_iMaxMana) {
				_GrandMagicLaunchMsgSend(1, 2);
				MeteorStrikeMsgHandler(2);
				m_pNpcList[iNpcH]->m_iManaStock = 0;
				m_iElvineMana = 0;
			}
		}
		break;
	}
}

// October 19, 2004 - 3.51 translated
BOOL CGame::_bNpcBehavior_Detector(int iNpcH)
{
	int dX, dY;
	short sOwnerH;
	char  cOwnerType, cSide;
	BOOL  bFlag = FALSE;

	if (m_pNpcList[iNpcH] == NULL) return FALSE;
	if (m_pNpcList[iNpcH]->m_sAppr2 != 0) return FALSE;

	for (dX = m_pNpcList[iNpcH]->m_sX - 10; dX <= m_pNpcList[iNpcH]->m_sX + 10; dX++)
		for (dY = m_pNpcList[iNpcH]->m_sY - 10; dY <= m_pNpcList[iNpcH]->m_sY + 10; dY++) {
			m_pMapList[m_pNpcList[iNpcH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

			cSide = 0;
			if (sOwnerH != NULL) {
				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					cSide = m_pClientList[sOwnerH]->m_cSide;
					break;

				case DEF_OWNERTYPE_NPC:
					cSide = m_pNpcList[sOwnerH]->m_cSide;
					break;
				}
			}

			if ((cSide != 0) && (cSide != m_pNpcList[iNpcH]->m_cSide)) {
				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					if (m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != NULL) {
						m_pClientList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = NULL;
						SetInvisibilityFlag(sOwnerH, cOwnerType, FALSE);
					}
					break;

				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] != NULL) {
						m_pNpcList[sOwnerH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = NULL;
						SetInvisibilityFlag(sOwnerH, cOwnerType, FALSE);
					}
					break;
				}

				bFlag = TRUE;
			}
		}

	return bFlag;
}

BOOL CGame::__bSetConstructionKit(int iMapIndex, int dX, int dY, int iType, int iTimeCost, int iClientH)
{
	int iNamingValue, ix, iy, tX, tY;
	char cNpcName[21], cName[21], cNpcWaypoint[11], cOwnerType;
	short sOwnerH;

	// Å©·ç¼¼ÀÌµå ¸ðµå°¡ ¾Æ´Ï°Å³ª °Ç¼³ÇÑ »ç¶÷ÀÌ °øº´ÀÌ ¾Æ´Ï¸é ¹«½Ã.
	if ((m_bIsCrusadeMode == FALSE) || (m_pClientList[iClientH]->m_iCrusadeDuty != 2)) return FALSE;
	if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_iTotalCrusadeStructures >= DEF_MAXCRUSADESTRUCTURES) {
		// ÇØ´ç ¸Ê¿¡ Å©·ç¼¼ÀÌµå °Ç¹° °³¼ö Á¦ÇÑ¿¡ °É¸®Áö ¾Ê´Â´Ù¸é
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_NOMORECRUSADESTRUCTURE, NULL, NULL, NULL, NULL);
		return FALSE;
	}

	// ÇØ´ç À§Ä¡¿¡ °ÇÃà¹° NPC¸¦ »ý¼º.
	iNamingValue = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->iGetEmptyNamingValue();
	if (iNamingValue != -1) {
		// ´õÀÌ»ó ÀÌ ¸Ê¿¡ NPC¸¦ ¸¸µé¼ö ¾ø´Ù. ÀÌ¸§À» ÇÒ´çÇÒ ¼ö ¾ø±â ¶§¹®.


		// ¸ÕÀú ¼³Ä¡ÇÏ°íÀÚ ÇÏ´Â ±ÙÃ³¿¡ ±¸Á¶¹°ÀÌ ¾ø³ª È®ÀÎÇÑ´Ù.
		for (ix = dX - 3; ix <= dX + 5; ix++)
			for (iy = dY - 3; iy <= dX + 5; iy++) {
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
				if ((sOwnerH != NULL) && (cOwnerType == DEF_OWNERTYPE_NPC) && (m_pNpcList[sOwnerH]->m_cActionLimit == 5)) return FALSE;
			}

		// NPC¸¦ »ý¼ºÇÑ´Ù.
		ZeroMemory(cNpcName, sizeof(cNpcName));
		if (m_pClientList[iClientH]->m_cSide == 1) {
			switch (iType) {
			case 1: strcpy(cNpcName, "AGT-Aresden"); break;
			case 2: strcpy(cNpcName, "CGT-Aresen"); break;
			case 3: strcpy(cNpcName, "MS-Aresden"); break;
			case 4: strcpy(cNpcName, "DT-Aresden"); break;
			}
		}
		else if (m_pClientList[iClientH]->m_cSide == 2) {
			switch (iType) {
			case 1: strcpy(cNpcName, "AGT-Elvine"); break;
			case 2: strcpy(cNpcName, "CGT-Elvine"); break;
			case 3: strcpy(cNpcName, "MS-Elvine"); break;
			case 4: strcpy(cNpcName, "DT-Elvine"); break;
			}
		}


		ZeroMemory(cName, sizeof(cName));
		wsprintf(cName, "XX%d", iNamingValue);
		cName[0] = '_';
		cName[1] = m_pClientList[iClientH]->m_cMapIndex + 65;

		ZeroMemory(cNpcWaypoint, sizeof(cNpcWaypoint));

		tX = (int)dX;
		tY = (int)dY;
		if (bCreateNewNpc(cNpcName, cName, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, 0, (rand() % 9),
			DEF_MOVETYPE_RANDOM, &tX, &tY, cNpcWaypoint, NULL, NULL, -1, FALSE, FALSE) == FALSE) {
			// ½ÇÆÐÇßÀ¸¹Ç·Î ¿¹¾àµÈ NameValue¸¦ ÇØÁ¦½ÃÅ²´Ù.
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
		}
		else {
			// ¼º°ø
			wsprintf(G_cTxt, "Structure(%s) construction begin(%d,%d)!", cNpcName, tX, tY);
			PutLogList(G_cTxt);
			return TRUE;
		}
	}

	return FALSE;
}

void CGame::RequestSetGuildConstructLocHandler(int iClientH, int dX, int dY, int iGuildGUID, char* pMapName)
{
	char* cp, cData[120];
	int i;
	int* ip, iIndex;
	DWORD dwTemp, dwTime;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsOnServerChange == TRUE) return;

	// °ÔÀÌÆ® ¼­¹ö ¸Þ½ÃÁö ÀÛ¼º 
	ZeroMemory(cData, sizeof(cData));
	cp = (char*)cData;
	*cp = GSM_SETGUILDCONSTRUCTLOC;
	cp++;

	ip = (int*)cp;
	*ip = iGuildGUID;
	cp += 4;

	ip = (int*)cp;
	*ip = dX;
	cp += 4;

	ip = (int*)cp;
	*ip = dY;
	cp += 4;

	memcpy(cp, pMapName, 10);
	cp += 10;
	//

	dwTime = timeGetTime();



	// ¸ÕÀú °°Àº GUID¸¦ °¡Áø ±æµå ÄÁ½ºÆ®·°Æ® ÁÂÇ¥°¡ ¼³Á¤µÇ¾î ÀÖ´ÂÁö Ã£´Â´Ù. 
	// ÅÚ·¹Æ÷Æ® À§Ä¡ÀÇ 2¹ø º¯¼öµéÀ» »ç¿ëÇÑ´Ù.
	for (i = 0; i < DEF_MAXGUILDS; i++)
		if (m_pGuildTeleportLoc[i].m_iV1 == iGuildGUID) {
			// ÀÌ¹Ì ±æµå°¡ ÀÖ´Ù.
			if ((m_pGuildTeleportLoc[i].m_sDestX2 == dX) && (m_pGuildTeleportLoc[i].m_sDestY2 == dY) && (strcmp(m_pGuildTeleportLoc[i].m_cDestMapName2, pMapName) == 0)) {
				// ³»¿ëÀÌ ÀÏÄ¡ÇÑ´Ù. Å¸ÀÓ ÀÔ·ÂÈÄ ½ºÅµ.
				m_pGuildTeleportLoc[i].m_dwTime2 = dwTime;
				return;
			}
			else {
				// ³»¿ë °»½ÅÈÄ 
				m_pGuildTeleportLoc[i].m_sDestX2 = dX;
				m_pGuildTeleportLoc[i].m_sDestY2 = dY;
				ZeroMemory(m_pGuildTeleportLoc[i].m_cDestMapName2, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName2));
				strcpy(m_pGuildTeleportLoc[i].m_cDestMapName2, pMapName);
				m_pGuildTeleportLoc[i].m_dwTime2 = dwTime;

				//°ÔÀÌÆ® ¼­¹ö¸¦ ÅëÇØ ´Ù¸¥ ¼­¹ö·Î Á¤º¸ Àü¼Û
				bStockMsgToGateServer(cData, 23);
				return;
			}
		}

	// ¼³Á¤µÈ ³»¿ëÀÌ ¾ø´Ù. »õ·Î ¼³Á¤ÇÑ´Ù.
	dwTemp = 0;
	iIndex = -1;
	for (i = 0; i < DEF_MAXGUILDS; i++) {
		if (m_pGuildTeleportLoc[i].m_iV1 == NULL) {

			m_pGuildTeleportLoc[i].m_iV1 = iGuildGUID;
			m_pGuildTeleportLoc[i].m_sDestX2 = dX;
			m_pGuildTeleportLoc[i].m_sDestY2 = dY;
			ZeroMemory(m_pGuildTeleportLoc[i].m_cDestMapName2, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName2));
			strcpy(m_pGuildTeleportLoc[i].m_cDestMapName2, pMapName);
			m_pGuildTeleportLoc[i].m_dwTime2 = dwTime;

			//°ÔÀÌÆ® ¼­¹ö¸¦ ÅëÇØ ´Ù¸¥ ¼­¹ö·Î Á¤º¸ Àü¼Û  
			bStockMsgToGateServer(cData, 23);
			return;
		}
		else {
			// °¡Àå ¿À·§µ¿¾È ÂüÁ¶µÇÁö ¾ÊÀº ¸®½ºÆ®ÀÇ ÀÎµ¦½º¸¦ Ã£´Â´Ù.
			if (dwTemp < (dwTime - m_pGuildTeleportLoc[i].m_dwTime2)) {
				dwTemp = (dwTime - m_pGuildTeleportLoc[i].m_dwTime2);
				iIndex = i;
			}
		}
	}

	m_pGuildTeleportLoc[i].m_iV1 = iGuildGUID;
	m_pGuildTeleportLoc[i].m_sDestX2 = dX;
	m_pGuildTeleportLoc[i].m_sDestY2 = dY;
	ZeroMemory(m_pGuildTeleportLoc[i].m_cDestMapName2, sizeof(m_pGuildTeleportLoc[i].m_cDestMapName2));
	strcpy(m_pGuildTeleportLoc[i].m_cDestMapName, pMapName);
	m_pGuildTeleportLoc[i].m_dwTime2 = dwTime;

	//°ÔÀÌÆ® ¼­¹ö¸¦ ÅëÇØ ´Ù¸¥ ¼­¹ö·Î Á¤º¸ Àü¼Û
	bStockMsgToGateServer(cData, 23);
}

// New 14/05/2004 Changed
void CGame::RequestSummonWarUnitHandler(int iClientH, int dX, int dY, char cType, char cNum, char cMode)
{
	char cName[6], cNpcName[21], cMapName[11], cNpcWayPoint[11], cOwnerType;
	int i, x;
	int iNamingValue, tX, tY, ix, iy;
	BOOL bRet;
	short sOwnerH;
	DWORD dwTime = timeGetTime();

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;

	ZeroMemory(cNpcWayPoint, sizeof(cNpcWayPoint));
	ZeroMemory(cNpcName, sizeof(cNpcName));
	ZeroMemory(cMapName, sizeof(cMapName));

	// ¿¡·¯ Ã¼Å©¿ë 
	if (cType < 0) return;
	if (cType >= DEF_MAXNPCTYPES) return;
	if (cNum > 10) return;

	// °Ç¼³ÇÏ±â¿¡ Æ÷ÀÎÆ®°¡ ¸ðÀÚ¶õ´Ù.
	if (m_pClientList[iClientH]->m_iConstructionPoint < m_iNpcConstructionPoint[cType]) return;
	// °Ç¹° ³»ºÎ¸é ÀüÀï À¯´ÏÆ® ¼ÒÈ¯ ºÒ°¡.
	if ((m_pMapList[m_pClientList[iClientH]->m_cMapIndex] != NULL) && (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFixedDayMode == TRUE)) return;

	// ÀÏ´Ü ¿¡·¯¹æÁö¿ë
	cNum = 1;

	// ConstructionPoint Á¦ÇÑÀ» °è»êÇÏ°í À¯´ÏÆ® °³¼ö¸¸Å­ ÁÙÀÎ´Ù.
	for (x = 1; x <= cNum; x++) {
		// °³¼ö¸¸Å­  ÀüÀï À¯´ÏÆ® »ý¼º 
		iNamingValue = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->iGetEmptyNamingValue();
		if (iNamingValue != -1) {
			// ´õÀÌ»ó ÀÌ ¸Ê¿¡ NPC¸¦ ¸¸µé¼ö ¾ø´Ù. ÀÌ¸§À» ÇÒ´çÇÒ ¼ö ¾ø±â ¶§¹®.
			// ÀÌ·± ÀÏÀÌ?

			// NPC¸¦ »ý¼ºÇÑ´Ù.
			ZeroMemory(cName, sizeof(cName));
			wsprintf(cName, "XX%d", iNamingValue);
			cName[0] = '_';
			cName[1] = m_pClientList[iClientH]->m_cMapIndex + 65;

			// µ¿ÀÏÇÑ Å¸ÀÔÀ» °®Áö¸¸ ´Ù¸¥ »çÀÌµå¸¦ °®´Â °æ¿ì 

			switch (cType) {
			case 43: // Light War Beetle
				switch (m_pClientList[iClientH]->m_cSide) {
				case 1: strcpy(cNpcName, "LWB-Aresden"); break;
				case 2: strcpy(cNpcName, "LWB-Elvine"); break;
				}
				break;

			case 36: // Arrow Guard Tower
				switch (m_pClientList[iClientH]->m_cSide) {
				case 1: strcpy(cNpcName, "AGT-Aresden"); break;
				case 2: strcpy(cNpcName, "AGT-Elvine"); break;
				}
				break;

			case 37: // Cannon Guard Tower
				switch (m_pClientList[iClientH]->m_cSide) {
				case 1: strcpy(cNpcName, "CGT-Aresden"); break;
				case 2: strcpy(cNpcName, "CGT-Elvine"); break;
				}
				break;

			case 38: // Mana Collector
				switch (m_pClientList[iClientH]->m_cSide) {
				case 1: strcpy(cNpcName, "MS-Aresden"); break;
				case 2: strcpy(cNpcName, "MS-Elvine"); break;
				}
				break;

			case 39: // Detector
				switch (m_pClientList[iClientH]->m_cSide) {
				case 1: strcpy(cNpcName, "DT-Aresden"); break;
				case 2: strcpy(cNpcName, "DT-Elvine"); break;
				}
				break;

			case 51: // Catapult
				switch (m_pClientList[iClientH]->m_cSide) {
				case 1: strcpy(cNpcName, "CP-Aresden"); break;
				case 2: strcpy(cNpcName, "CP-Elvine"); break;
				}
				break;

			case 44:
				strcpy(cNpcName, "GHK");
				break;

			case 45:
				strcpy(cNpcName, "GHKABS");
				break;

			case 46:
				strcpy(cNpcName, "TK");
				break;

			case 47:
				strcpy(cNpcName, "BG");
				break;

			case 82:
				switch (m_pClientList[iClientH]->m_cSide) {
				case 1: strcpy(cNpcName, "Sor-Aresden"); break;
				case 2: strcpy(cNpcName, "Sor-Elvine"); break;
				}
				break;

			case 83:
				switch (m_pClientList[iClientH]->m_cSide) {
				case 1: strcpy(cNpcName, "ATK-Aresden"); break;
				case 2: strcpy(cNpcName, "ATK-Elvine"); break;
				}
				break;

			case 84:
				switch (m_pClientList[iClientH]->m_cSide) {
				case 1: strcpy(cNpcName, "Elf-Aresden"); break;
				case 2: strcpy(cNpcName, "Elf-Elvine"); break;
				}
				break;

			case 85:
				switch (m_pClientList[iClientH]->m_cSide) {
				case 1: strcpy(cNpcName, "DSK-Aresden"); break;
				case 2: strcpy(cNpcName, "DSK-Elvine"); break;
				}
				break;

			case 86:
				switch (m_pClientList[iClientH]->m_cSide) {
				case 1: strcpy(cNpcName, "HBT-Aresden"); break;
				case 2: strcpy(cNpcName, "HBT-Elvine"); break;
				}
				break;

			case 87:
				switch (m_pClientList[iClientH]->m_cSide) {
				case 1: strcpy(cNpcName, "CT-Aresden"); break;
				case 2: strcpy(cNpcName, "CT-Elvine"); break;
				}
				break;

			case 88:
				switch (m_pClientList[iClientH]->m_cSide) {
				case 1: strcpy(cNpcName, "Bar-Aresden"); break;
				case 2: strcpy(cNpcName, "Bar-Elvine"); break;
				}
				break;

			case 89:
				switch (m_pClientList[iClientH]->m_cSide) {
				case 1: strcpy(cNpcName, "AGC-Aresden"); break;
				case 2: strcpy(cNpcName, "AGC-Elvine"); break;
				}
				break;
			}

			//testcode
			wsprintf(G_cTxt, "(!) Request Summon War Unit (%d) (%s)", cType, cNpcName);
			PutLogList(G_cTxt);

			tX = (int)dX;
			tY = (int)dY;

			// ¸¸¾à ÀüÀï °ÇÃà¹°ÀÌ¶ó¸é °ÇÃà À§Ä¡·ÎºÎÅÍ ÀÎÁ¢ÇØ¾ß ÇÑ´Ù.
			bRet = FALSE;
			switch (cType) {
			case 36:
			case 37:
			case 38:
			case 39:
				// °Ç¼³ À§Ä¡¸¦ ºñ±³
				if (strcmp(m_pClientList[iClientH]->m_cConstructMapName, m_pClientList[iClientH]->m_cMapName) != 0) bRet = TRUE;
				if (abs(m_pClientList[iClientH]->m_sX - m_pClientList[iClientH]->m_iConstructLocX) > 10) bRet = TRUE;
				if (abs(m_pClientList[iClientH]->m_sY - m_pClientList[iClientH]->m_iConstructLocY) > 10) bRet = TRUE;

				if (bRet == TRUE) {
					// °Ç¼³ À§Ä¡¿Í ³Ê¹« ¸Ö¸® ¹þ¾î³ª ÀÖÀ¸¸é °ÇÃà ºÒ°¡.
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CANNOTCONSTRUCT, 2, NULL, NULL, NULL);
					return;
				}

				// ±æµå¿¡¼­ °Ç¼³ÇÑ °ÇÃà¹°ÀÇ °¹¼ö¸¦ Ä«¿îÆ®ÇÑ´Ù. ÀÏÁ¤ °¹¼ö ÀÌ»óÀº ÁöÀ» ¼ö ¾ø´Ù.
				/////
				for (i = 0; i < DEF_MAXGUILDS; i++)
					if (m_pGuildTeleportLoc[i].m_iV1 == m_pClientList[iClientH]->m_iGuildGUID) {
						m_pGuildTeleportLoc[i].m_dwTime = dwTime;
						if (m_pGuildTeleportLoc[i].m_iV2 >= DEF_MAXCONSTRUCTNUM) {
							// ±æµå °Ç¼³ °³¼ö ÃÊ°ú. ÁöÀ»¼ö ¾ø´Ù.
							m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
							SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CANNOTCONSTRUCT, 3, NULL, NULL, NULL);
							return;
						}
						else {
							// °Ç¼³µÈ °Ç¹° °¹¼ö Áõ°¡.
							m_pGuildTeleportLoc[i].m_iV2++;
							goto RSWU_LOOPBREAK;
						}
					}

				// ±æµå ¼³Á¤µÇ¾î ÀÖÁö ¾ÊÀ½. ÁöÀ»¼ö ¾ø´Ù.
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CANNOTCONSTRUCT, 3, NULL, NULL, NULL);
				break;


			case 43:
			case 44:
			case 45:
			case 46:
			case 47:
			case 51:
			case 40:
			case 41:
			case 42:
			case 48:
			case 49:
			case 50:
				break;
			}

		RSWU_LOOPBREAK:;

			// ¸¸¾à ÀüÀï °ÇÃà¹°Áß °¡µåÅ¸¿ö°°ÀÌ °ø°ÝÇüÀÌ¶ó¸é ³Ê¹« ÀÎÁ¢ÇØ¼­ °Ç¼³ÇÒ ¼ö ¾ø´Ù.
			bRet = FALSE;
			switch (cType) {
			case 36:
			case 37:
				for (ix = tX - 2; ix <= tX + 2; ix++)
					for (iy = tY - 2; iy <= tY + 2; iy++) {
						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, ix, iy);
						if ((sOwnerH != NULL) && (cOwnerType == DEF_OWNERTYPE_NPC)) {
							switch (m_pNpcList[sOwnerH]->m_sType) {
							case 36:
							case 37:
								bRet = TRUE;
								break;
							}
						}
					}

				// ÁöÀ» ¼ö ¾ø´Â »óÀ§ÁÂÇ¥
				if ((dY <= 32) || (dY >= 783)) bRet = TRUE;
				break;
			}

			if (bRet == TRUE) {
				// ±ÙÃ³¿¡ °¡µåÅ¸¿öµéÀÌ ÀÖ¾î¼­ °ÇÃàºÒ°¡.
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CANNOTCONSTRUCT, 1, NULL, NULL, NULL);
				return;
			}

			// °¡µå ¸ðµåÀÎ°¡ ÃßÁ¾ ¸ðµåÀÎ°¡ ¼±ÅÃ 
			if (cMode == NULL) {
				bRet = bCreateNewNpc(cNpcName, cName, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, 0, 0, DEF_MOVETYPE_FOLLOW, &tX, &tY, cNpcWayPoint, NULL, NULL, -1, FALSE, FALSE, FALSE, FALSE, m_pClientList[iClientH]->m_iGuildGUID);
				bSetNpcFollowMode(cName, m_pClientList[iClientH]->m_cCharName, DEF_OWNERTYPE_PLAYER);
			}
			else bRet = bCreateNewNpc(cNpcName, cName, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, 0, 0, DEF_MOVETYPE_GUARD, &tX, &tY, cNpcWayPoint, NULL, NULL, -1, FALSE, FALSE, FALSE, FALSE, m_pClientList[iClientH]->m_iGuildGUID);

			if (bRet == FALSE) {
				// ½ÇÆÐÇßÀ¸¹Ç·Î ¿¹¾àµÈ NameValue¸¦ ÇØÁ¦½ÃÅ²´Ù.
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetNamingValueEmpty(iNamingValue);
			}
			else {
				// °Ç¼³¿¡ ¼º°øÇßÀ¸¹Ç·Î °Ç¼³ Æ÷ÀÎÆ® °¨¼Ò ½ÃÅ²´Ù.
				m_pClientList[iClientH]->m_iConstructionPoint -= m_iNpcConstructionPoint[cType];
				if (m_pClientList[iClientH]->m_iConstructionPoint < 0) m_pClientList[iClientH]->m_iConstructionPoint = 0;
				// ÁöÈÖ°ü¿¡°Ô ¹Ù·Î Åëº¸.
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CONSTRUCTIONPOINT, m_pClientList[iClientH]->m_iConstructionPoint, m_pClientList[iClientH]->m_iWarContribution, NULL, NULL);
			}
		}
	}
}

void CGame::SelectCrusadeDutyHandler(int iClientH, int iDuty)
{

	if (m_pClientList[iClientH] == NULL) return;
	if ((m_pClientList[iClientH]->m_iGuildRank != 0) && (iDuty == 3)) return;

	if (m_iLastCrusadeWinner == m_pClientList[iClientH]->m_cSide &&
		m_pClientList[iClientH]->m_dwCrusadeGUID == 0 && iDuty == 3) {
		m_pClientList[iClientH]->m_iConstructionPoint = 3000;
	}
	m_pClientList[iClientH]->m_iCrusadeDuty = iDuty;

	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CRUSADE, (DWORD)m_bIsCrusadeMode, m_pClientList[iClientH]->m_iCrusadeDuty, NULL, NULL);
	if (iDuty == 1)
		_bCrusadeLog(DEF_CRUSADELOG_SELECTDUTY, iClientH, NULL, "Fighter");
	else if (iDuty == 2)
		_bCrusadeLog(DEF_CRUSADELOG_SELECTDUTY, iClientH, NULL, "Constructor");
	else
		_bCrusadeLog(DEF_CRUSADELOG_SELECTDUTY, iClientH, NULL, "Commander");
}

void CGame::MapStatusHandler(int iClientH, int iMode, char* pMapName)
{
	int i;

	if (m_pClientList[iClientH] == NULL) return;

	switch (iMode) {
	case 1:
		if (m_pClientList[iClientH]->m_iCrusadeDuty == NULL) return;

		for (i = 0; i < DEF_MAXGUILDS; i++)
			if ((m_pGuildTeleportLoc[i].m_iV1 != NULL) && (m_pGuildTeleportLoc[i].m_iV1 == m_pClientList[iClientH]->m_iGuildGUID)) {
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_TCLOC, m_pGuildTeleportLoc[i].m_sDestX, m_pGuildTeleportLoc[i].m_sDestY,
					NULL, m_pGuildTeleportLoc[i].m_cDestMapName, m_pGuildTeleportLoc[i].m_sDestX2, m_pGuildTeleportLoc[i].m_sDestY2,
					NULL, NULL, NULL, NULL, m_pGuildTeleportLoc[i].m_cDestMapName2);
				// �Ǽ� ��ġ�� ������ ���´�.
				ZeroMemory(m_pClientList[iClientH]->m_cConstructMapName, sizeof(m_pClientList[iClientH]->m_cConstructMapName));
				memcpy(m_pClientList[iClientH]->m_cConstructMapName, m_pGuildTeleportLoc[i].m_cDestMapName2, 10);
				m_pClientList[iClientH]->m_iConstructLocX = m_pGuildTeleportLoc[i].m_sDestX2;
				m_pClientList[iClientH]->m_iConstructLocY = m_pGuildTeleportLoc[i].m_sDestY2;
				break;
			}

		// ��� �Ҽ��� �ƴ� ������ ��û�̴�.
		break;

	case 3:
		// ������ ĳ�� ����ü �ʱ�ȭ 
		for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++) {
			m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cType = NULL;
			m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cSide = NULL;
			m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sX = NULL;
			m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sY = NULL;
		}
		m_pClientList[iClientH]->m_iCSIsendPoint = NULL;
		ZeroMemory(m_pClientList[iClientH]->m_cSendingMapName, sizeof(m_pClientList[iClientH]->m_cSendingMapName));

		if (strcmp(pMapName, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName) == 0) {
			// ���� ����ڰ� ��ġ�� ���� ������ �䱸�ߴ�.
			// �� �����͸� ��û�� ����� ������ �״�� ī���Ѵ�.
			for (i = 0; i < m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_iTotalCrusadeStructures; i++) {
				// v2.15 ����ڰ� Commander �� �ƴϾ �ǹ��� ��ġ�� �����ش�.
				if (m_pClientList[iClientH]->m_iCrusadeDuty == 3)
				{
					m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cType = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stCrusadeStructureInfo[i].cType;
					m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cSide = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stCrusadeStructureInfo[i].cSide;
					m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sX = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stCrusadeStructureInfo[i].sX;
					m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sY = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stCrusadeStructureInfo[i].sY;
				}
				else if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stCrusadeStructureInfo[i].cType == 42)
				{
					m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cType = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stCrusadeStructureInfo[i].cType;
					m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cSide = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stCrusadeStructureInfo[i].cSide;
					m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sX = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stCrusadeStructureInfo[i].sX;
					m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sY = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stCrusadeStructureInfo[i].sY;
				}
			}
			memcpy(m_pClientList[iClientH]->m_cSendingMapName, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, 10);
		}
		else {
			// �ٸ� ���� ��Ȳ�� ��û�ߴ�. �̵鷣���� ���� ���� 
			if (strcmp(pMapName, "middleland") == 0) {
				for (i = 0; i < m_iTotalMiddleCrusadeStructures; i++) {
					// v2.15 ����ڰ� Commander �� �ƴϾ �ǹ��� ��ġ�� �����ش�.
					if (m_pClientList[iClientH]->m_iCrusadeDuty == 3)
					{
						m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cType = m_stMiddleCrusadeStructureInfo[i].cType;
						m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cSide = m_stMiddleCrusadeStructureInfo[i].cSide;
						m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sX = m_stMiddleCrusadeStructureInfo[i].sX;
						m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sY = m_stMiddleCrusadeStructureInfo[i].sY;
					}
					else if (m_stMiddleCrusadeStructureInfo[i].cType == 42)
					{
						m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cType = m_stMiddleCrusadeStructureInfo[i].cType;
						m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].cSide = m_stMiddleCrusadeStructureInfo[i].cSide;
						m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sX = m_stMiddleCrusadeStructureInfo[i].sX;
						m_pClientList[iClientH]->m_stCrusadeStructureInfo[i].sY = m_stMiddleCrusadeStructureInfo[i].sY;
					}
				}
				strcpy(m_pClientList[iClientH]->m_cSendingMapName, "middleland");
			}
		}

		_SendMapStatus(iClientH);
		break;
	}
}

void CGame::_SendMapStatus(int iClientH)
{
	int i, iDataSize;
	char* cp, cData[DEF_MAXCRUSADESTRUCTURES * 6];
	short* sp;

	ZeroMemory(cData, sizeof(cData));
	cp = (char*)(cData);

	memcpy(cp, m_pClientList[iClientH]->m_cSendingMapName, 10);
	cp += 10;

	sp = (short*)cp;
	*sp = (short)m_pClientList[iClientH]->m_iCSIsendPoint;
	cp += 2;

	// ÃÑ ¸î°³ÀÇ µ¥ÀÌÅÍ°¡ ÀÖ´ÂÁö ¾Ë¸®´Â °÷ ¶ç¿ò.
	cp++;

	if (m_pClientList[iClientH]->m_iCSIsendPoint == NULL)
		m_pClientList[iClientH]->m_bIsSendingMapStatus = TRUE;

	// 100°³¾¿ Á¤º¸¸¦ º¸³½´Ù.
	iDataSize = 0;
	for (i = 0; i < 100; i++) {
		if (m_pClientList[iClientH]->m_iCSIsendPoint >= DEF_MAXCRUSADESTRUCTURES) goto SMS_ENDOFDATA;
		if (m_pClientList[iClientH]->m_stCrusadeStructureInfo[m_pClientList[iClientH]->m_iCSIsendPoint].cType == NULL) goto SMS_ENDOFDATA;

		*cp = m_pClientList[iClientH]->m_stCrusadeStructureInfo[m_pClientList[iClientH]->m_iCSIsendPoint].cType;
		cp++;
		sp = (short*)cp;
		*sp = m_pClientList[iClientH]->m_stCrusadeStructureInfo[m_pClientList[iClientH]->m_iCSIsendPoint].sX;
		cp += 2;
		sp = (short*)cp;
		*sp = m_pClientList[iClientH]->m_stCrusadeStructureInfo[m_pClientList[iClientH]->m_iCSIsendPoint].sY;
		cp += 2;
		*cp = m_pClientList[iClientH]->m_stCrusadeStructureInfo[m_pClientList[iClientH]->m_iCSIsendPoint].cSide;
		cp++;

		iDataSize += 6;
		m_pClientList[iClientH]->m_iCSIsendPoint++;
	}

	// ¿©±â±îÁö ¿Ô´Ù¸é ´õ º¸³¾ ¸Þ½ÃÁö°¡ ÀÖ´Ù´Â ÀÇ¹Ì.
	cp = (char*)(cData + 12);
	*cp = (iDataSize / 6);
	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_MAPSTATUSNEXT, iDataSize + 13, NULL, NULL, cData);


SMS_ENDOFDATA:;

	// µ¥ÀÌÅÍ°¡ ´Ù Àü¼Û µÇ¾úÀ½À» ÀÇ¹Ì.
	cp = (char*)(cData + 12);
	*cp = (iDataSize / 6);
	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_MAPSTATUSLAST, iDataSize + 13, NULL, NULL, cData);
	m_pClientList[iClientH]->m_bIsSendingMapStatus = FALSE;
}

void CGame::RequestHelpHandler(int iClientH)
{
	// ÇïÇÁ ¿äÃ»ÀÌ´Ù. ±æµå ¸¶½ºÅÍ¿¡°Ô ÁÂÇ¥¸¦ Àü´ÞÇØ ÁØ´Ù.
	int i;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_iGuildRank == -1) return;
	if (m_pClientList[iClientH]->m_iCrusadeDuty != 1) return;

	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != NULL) && (m_pClientList[i]->m_iGuildRank == 0) &&
			(m_pClientList[i]->m_iCrusadeDuty == 3) && (m_pClientList[i]->m_iGuildGUID == m_pClientList[iClientH]->m_iGuildGUID)) {
			// °°Àº ±æµå ¸¶½ºÅÍÀÌ°í ÁöÈÖ°ü ¿ªÈ°À» ¸ÃÀº »óÅÂÀÌ´Ù.
			SendNotifyMsg(NULL, i, DEF_NOTIFY_HELP, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, m_pClientList[iClientH]->m_iHP, m_pClientList[iClientH]->m_cCharName);
			return;
		}

	// ÇöÀç ¸Ê¿¡ ÁöÈÖ°üÀÌ ¾øÀ½À» Åëº¸ÇØ ÁØ´Ù.
	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_HELPFAILED, NULL, NULL, NULL, NULL);
}