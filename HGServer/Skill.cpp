// Skill.cpp: implementation of the CSkill class.
//
//////////////////////////////////////////////////////////////////////

#include "Skill.h"

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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkill::CSkill()
{
	ZeroMemory(m_cName, sizeof(m_cName));
}

CSkill::~CSkill()
{ 

}

int CGame::_iGetWeaponSkillType(int iClientH)
{
	WORD wWeaponType;

	if (m_pClientList[iClientH] == NULL) return 0;

	wWeaponType = ((m_pClientList[iClientH]->m_sAppr2 & 0x0FF0) >> 4);

	if (wWeaponType == 0) {
		return 5;
	}
	else if ((wWeaponType >= 1) && (wWeaponType <= 2)) {
		return 7;
	}
	else if ((wWeaponType > 2) && (wWeaponType < 20)) {
		if (wWeaponType == 7)
			return 9;
		else return 8;
	}
	else if ((wWeaponType >= 20) && (wWeaponType < 30)) {
		return 10;
	}
	else if ((wWeaponType >= 30) && (wWeaponType < 35)) {
		return 14;
	}
	else if ((wWeaponType >= 35) && (wWeaponType < 40)) {
		return 21;
	}
	else if (wWeaponType >= 40) {
		return 6;
	}

	return 1;
}

void CGame::PoisonEffect(int iClientH, int iV1)
{
	int iPoisonLevel, iDamage, iPrevHP, iProb;

	// Áßµ¶À¸·Î Á×Áö´Â ¾Ê´Â´Ù. ´Ù¸¸ Ã¼·ÂÀÌ °è¼Ó ±ïÀÌ°í ÃÖ¼Ò 1¸¸ ³²´Â´Ù. 
	if (m_pClientList[iClientH] == NULL)     return;
	if (m_pClientList[iClientH]->m_bIsKilled == TRUE) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;

	iPoisonLevel = m_pClientList[iClientH]->m_iPoisonLevel;

	iDamage = iDice(1, iPoisonLevel);

	iPrevHP = m_pClientList[iClientH]->m_iHP;
	m_pClientList[iClientH]->m_iHP -= iDamage;
	if (m_pClientList[iClientH]->m_iHP <= 0) m_pClientList[iClientH]->m_iHP = 1;

	if (iPrevHP != m_pClientList[iClientH]->m_iHP)
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_HP, NULL, NULL, NULL, NULL);


	// µ¶¼º ÀúÇ× È®·ü·Î Áßµ¶ÀÌ Ç®¸± ¼ö ÀÖ´Ù.
	iProb = m_pClientList[iClientH]->m_cSkillMastery[23] - 10 + m_pClientList[iClientH]->m_iAddPR;
	if (iProb <= 10) iProb = 10;
	if (iDice(1, 100) <= iProb) {
		m_pClientList[iClientH]->m_bIsPoisoned = FALSE;
		// Áßµ¶ÀÌ Ç®·ÈÀ½À» ¾Ë¸°´Ù. 
		SetPoisonFlag(iClientH, DEF_OWNERTYPE_PLAYER, FALSE); // remove poison aura after effect complete
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_POISON, NULL, NULL, NULL);
	}
}
BOOL CGame::bCheckResistingPoisonSuccess(short sOwnerH, char cOwnerType)
{
	int iResist, iResult;

	// µ¶¼º ÀúÇ×ÀÌ ¼º°øÇß´ÂÁö¸¦ °è»êÇÑ´Ù. 
	switch (cOwnerType) {
	case DEF_OWNERTYPE_PLAYER:
		if (m_pClientList[sOwnerH] == NULL) return FALSE;
		iResist = m_pClientList[sOwnerH]->m_cSkillMastery[23] + m_pClientList[sOwnerH]->m_iAddPR;
		break;

	case DEF_OWNERTYPE_NPC:
		if (m_pNpcList[sOwnerH] == NULL) return FALSE;
		iResist = 0;
		break;
	}

	iResult = iDice(1, 100);
	if (iResult >= iResist) // µ¶¼º ÀúÇ× ½ÇÆÐ. Áßµ¶µÈ´Ù.
		return FALSE;

	// µ¶¼º ÀúÇ× ¼º°ø. ÇÃ·¹ÀÌ¾î¶ó¸é ½ºÅ³À» ¿Ã¸°´Ù. 
	if (cOwnerType == DEF_OWNERTYPE_PLAYER)
		CalculateSSN_SkillIndex(sOwnerH, 23, 1);

	return TRUE;
}

BOOL CGame::_bDecodeSkillConfigFileContents(char* pData, DWORD dwMsgSize)
{
	char* pContents, * token, cTxt[120];
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int  iSkillConfigListIndex = 0;
	class CStrTok* pStrTok;

	pContents = new char[dwMsgSize + 1];
	ZeroMemory(pContents, dwMsgSize + 1);
	memcpy(pContents, pData, dwMsgSize);

	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while (token != NULL) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
			case 1:
				switch (cReadModeB) {
				case 1:
					// ½ºÅ³ ¹øÈ£ 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					if (m_pSkillConfigList[atoi(token)] != NULL) {
						// ÀÌ¹Ì ÇÒ´çµÈ ¹øÈ£°¡ ÀÖ´Ù. ¿¡·¯ÀÌ´Ù.
						PutLogList("(!!!) CRITICAL ERROR! SKILL configuration file error - Duplicate magic number.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pSkillConfigList[atoi(token)] = new class CSkill;
					iSkillConfigListIndex = atoi(token);

					cReadModeB = 2;
					break;

				case 2:
					// ½ºÅ³ ÀÌ¸§ 
					ZeroMemory(m_pSkillConfigList[iSkillConfigListIndex]->m_cName, sizeof(m_pSkillConfigList[iSkillConfigListIndex]->m_cName));
					memcpy(m_pSkillConfigList[iSkillConfigListIndex]->m_cName, token, strlen(token));
					cReadModeB = 3;
					break;

				case 3:
					// ½ºÅ³ Á¾·ù m_sType
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pSkillConfigList[iSkillConfigListIndex]->m_sType = atoi(token);
					cReadModeB = 4;
					break;

				case 4:
					// m_sValue1
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pSkillConfigList[iSkillConfigListIndex]->m_sValue1 = atoi(token);
					cReadModeB = 5;
					break;

				case 5:
					// m_sValue2
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pSkillConfigList[iSkillConfigListIndex]->m_sValue2 = atoi(token);
					cReadModeB = 6;
					break;

				case 6:
					// m_sValue3
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pSkillConfigList[iSkillConfigListIndex]->m_sValue3 = atoi(token);
					cReadModeB = 7;
					break;

				case 7:
					// m_sValue4
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pSkillConfigList[iSkillConfigListIndex]->m_sValue4 = atoi(token);
					cReadModeB = 8;
					break;

				case 8:
					// m_sValue5
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pSkillConfigList[iSkillConfigListIndex]->m_sValue5 = atoi(token);
					cReadModeB = 9;
					break;

				case 9:
					// m_sValue6
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! SKILL configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pSkillConfigList[iSkillConfigListIndex]->m_sValue6 = atoi(token);
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
			if (memcmp(token, "skill", 5) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
			}

		}
		token = pStrTok->pGet();

	}

	delete pStrTok;
	delete[] pContents;

	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) CRITICAL ERROR! SKILL configuration file contents error!");
		return FALSE;
	}

	wsprintf(cTxt, "(!) SKILL(Total:%d) configuration - success!", iSkillConfigListIndex);
	PutLogList(cTxt);

	return TRUE;
}

void CGame::TrainSkillResponse(BOOL bSuccess, int iClientH, int iSkillNum, int iSkillLevel)
{
	char* cp, cData[100];
	DWORD* dwp;
	WORD* wp;
	int   iRet;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;
	if ((iSkillNum < 0) || (iSkillNum > 100)) return;
	if ((iSkillLevel < 0) || (iSkillLevel > 100)) return;

	if (bSuccess == TRUE) 
	{
		if (m_pClientList[iClientH]->m_cSkillMastery[iSkillNum] != 0) return;

		m_pClientList[iClientH]->m_cSkillMastery[iSkillNum] = iSkillLevel;
		bCheckTotalSkillMasteryPoints(iClientH, iSkillNum);

		dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_SKILLTRAINSUCCESS;

		cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);

		*cp = iSkillNum;
		cp++;

		*cp = iSkillLevel;
		cp++;

		if (m_pSkillConfigList[iSkillNum]->m_cName != NULL)
			_bItemLog(DEF_ITEMLOG_SKILLLEARN, iClientH, m_pSkillConfigList[iSkillNum]->m_cName, NULL);

		iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 8);

		switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
		case DEF_XSOCKEVENT_SOCKETCLOSED:
			// �޽����� ������ ������ �߻��ߴٸ� �����Ѵ�.
			DeleteClient(iClientH, TRUE, TRUE);
			break;
		}
	}
}


int CGame::_iGetSkillNumber(char* pSkillName)
{
	int i;
	char cTmpName[21];

	ZeroMemory(cTmpName, sizeof(cTmpName));
	strcpy(cTmpName, pSkillName);

	for (i = 1; i < DEF_MAXSKILLTYPE; i++)
		if (m_pSkillConfigList[i] != NULL) 
		{
			if (memcmp(cTmpName, m_pSkillConfigList[i]->m_cName, 20) == 0) 
			{
				return i;
			}
		}

	return 0;
}

int CGame::_iCalcSkillSSNpoint(int iLevel)
{
	int iRet;

	if (iLevel < 1) return 1;

	if (iLevel <= 50)
		iRet = iLevel;
	else if (iLevel > 50) {
		iRet = (iLevel * iLevel) / 10;
	}

	return iRet;
}

void CGame::CalculateSSN_ItemIndex(int iClientH, short sWeaponIndex, int iValue)
{
	short sSkillIndex;
	int   iOldSSN, iSSNpoint, iWeaponIndex;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;
	if (m_pClientList[iClientH]->m_pItemList[sWeaponIndex] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsKilled == TRUE) return;

	sSkillIndex = m_pClientList[iClientH]->m_pItemList[sWeaponIndex]->m_sRelatedSkill;
	if ((sSkillIndex < 0) || (sSkillIndex >= DEF_MAXSKILLTYPE)) return;
	if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] == 0) return;

	iOldSSN = m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex];
	m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] += iValue;

	iSSNpoint = m_iSkillSSNpoint[m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] + 1];

	if ((m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] < 100) &&
		(m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] > iSSNpoint)) {

		m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]++;

		switch (sSkillIndex) {

		case 22: // Taming
			if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iCharisma * 2)) {
				m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case 0:  // Mining
		case 5:  // Hand-Attack
		case 13: // Manufacturing
			if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > ((m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iAngelicStr) * 2)) {
				m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case 3: // Magic-Resistance
			if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > ((m_pClientList[iClientH]->m_iLevel) * 2)) {
				m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case 4:  // Magic
		case 18: // Crafting
		case 21: // Staff-Attack
			if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > ((m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iAngelicMag) * 2)) {
				m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case 1:  // Fishing
		case 6:  // Archery
		case 7:  // Short-Sword
		case 8:  // Long-Sword
		case 9:  // Fencing 
		case 10: // Axe-Attack
		case 11: // Shield        	
		case 14: // Hammer 
		case 20: // Dual-Wielding
			if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > ((m_pClientList[iClientH]->m_iDex + m_pClientList[iClientH]->m_iAngelicDex) * 2)) {
				m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case 2:	 // Farming
		case 12: // Alchemy
		case 15: // ����óġ
		case 19: // Pretend-Corpse
			if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > ((m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iAngelicInt) * 2)) {
				m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case 23: // Poison-Resistance
			if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iVit * 2)) {
				m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;

		default:
			m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;
		}

		if (m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] == 0) 
		{
			if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1) 
			{
				iWeaponIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
				if (m_pClientList[iClientH]->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sSkillIndex) 
				{
					m_pClientList[iClientH]->m_iHitRatio++;
				}
			}

			if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1) 
			{
				iWeaponIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
				if (m_pClientList[iClientH]->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sSkillIndex) 
				{
					m_pClientList[iClientH]->m_iHitRatio++;
				}
			}
		}

		if (m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] == 0) 
		{
			bCheckTotalSkillMasteryPoints(iClientH, sSkillIndex);
			//SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SKILL, sSkillIndex, m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex], NULL, NULL);
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SKILL, sSkillIndex, m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex], m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex], NULL);
		}
	}
}



void CGame::CalculateSSN_SkillIndex(int iClientH, short sSkillIndex, int iValue)
{
	int   iOldSSN, iSSNpoint, iWeaponIndex;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;
	if ((sSkillIndex < 0) || (sSkillIndex >= DEF_MAXSKILLTYPE)) return;
	if (m_pClientList[iClientH]->m_bIsKilled == TRUE) return;

	if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] == 0) return;
	
	iOldSSN = m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex];
	m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] += iValue;

	iSSNpoint = m_iSkillSSNpoint[m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] + 1];

	if ((m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] < 100) &&
		(m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] > iSSNpoint)) {

		// ½ºÅ³ÀÌ ¿Ã¶ú´Ù.
		m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]++;
		// Æ¯¼ºÄ¡ Á¦ÇÑÀÌ ÀÖ´Â SkillÀ» Ã³¸®ÇÑ´Ù.
		switch (sSkillIndex) {
		case 0:
		case 5:
		case 13:
			if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > ((m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iAngelicStr) * 2)) {
				// Á¦ÇÑÄ¡º¸´Ù Ä¿Á³´Ù. ¹«È¿ÀÌ¹Ç·Î ÀÌÀü»óÅÂ·Î µÇµ¹¸°´Ù.
				m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case 3:
			// ¸¶¹ý ÀúÇ×Àº ÃÖ´ë Level*2¸¸Å­ ¿À¸¥´Ù.
			if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > ((m_pClientList[iClientH]->m_iLevel) * 2)) {
				// Á¦ÇÑÄ¡º¸´Ù Ä¿Á³´Ù. ¹«È¿ÀÌ¹Ç·Î ÀÌÀü»óÅÂ·Î µÇµ¹¸°´Ù.
				m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case 4:
		case 18: // Crafting
		case 21:
			if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > ((m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iAngelicMag) * 2)) {
				// Á¦ÇÑÄ¡º¸´Ù Ä¿Á³´Ù. ¹«È¿ÀÌ¹Ç·Î ÀÌÀü»óÅÂ·Î µÇµ¹¸°´Ù.
				m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case 1:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 20: // Dual-Wielding
			if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > ((m_pClientList[iClientH]->m_iDex + m_pClientList[iClientH]->m_iAngelicDex) * 2)) {
				// Á¦ÇÑÄ¡º¸´Ù Ä¿Á³´Ù. ¹«È¿ÀÌ¹Ç·Î ÀÌÀü»óÅÂ·Î µÇµ¹¸°´Ù.
				m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case 2:
		case 12:
		case 14:
		case 15:
		case 19:
			if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > ((m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iAngelicInt) * 2)) {
				// Á¦ÇÑÄ¡º¸´Ù Ä¿Á³´Ù. ¹«È¿ÀÌ¹Ç·Î ÀÌÀü»óÅÂ·Î µÇµ¹¸°´Ù.
				m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case 23: // µ¶¼º ÀúÇ× 
			if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iVit * 2)) {
				// Á¦ÇÑÄ¡º¸´Ù Ä¿Á³´Ù. ¹«È¿ÀÌ¹Ç·Î ÀÌÀü»óÅÂ·Î µÇµ¹¸°´Ù.
				m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;

		case 22: // SNOOPY:Taming
			if (m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex] > (m_pClientList[iClientH]->m_iCharisma * 2)) {
				m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex]--;
				m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = iOldSSN;
			}
			else m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;

		default:
			m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] = 0;
			break;
		}

		// ¸¸¾à ¹«±â¿Í °ü·ÃÀÖ´Â ½ºÅ³ÀÌ ¿Ã¶ú°í ÇöÀç ±× ¹«±â¸¦ »ç¿ë ÁßÀÌ¶ó¸é ÇöÀç ¸íÁß·ü¿¡ 1À» ´õÇÑ´Ù. 
		if (m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] == 0) {
			// Ä«¿îÆ®°¡ 0ÀÌ¸é ½ºÅ³ÀÌ Á¤»óÀûÀ¸·Î ¿Ã¶ú´Ù´Â ÀÇ¹Ì. 
			if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1) {
				// ¾ç¼Õ¹«±â°¡ ÀåÂøµÇ¾î ÀÖ¾ú´Ù. 
				iWeaponIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
				if (m_pClientList[iClientH]->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sSkillIndex) {
					// È°·ùÀÇ »ç¿ë¿¡ ÀÇÇÑ ½ºÅ³ÀÇ »ó½ÂÀÌ¾ú´Ù. ¸íÁß·üÀ» 1 ¿Ã¸°´Ù. 
					m_pClientList[iClientH]->m_iHitRatio++;
				}
			}

			if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1) {
				// ¹«±â°¡ ÀåÂøµÇ¾î ÀÖ¾ú´Ù. 
				iWeaponIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
				if (m_pClientList[iClientH]->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sSkillIndex) {
					// °ËÀÌ³ª Mace·ùÀÇ »ç¿ë¿¡ ÀÇÇÑ ½ºÅ³ÀÇ »ó½ÂÀÌ¾ú´Ù. ¸íÁß·üÀ» 1 ¿Ã¸°´Ù. 
					m_pClientList[iClientH]->m_iHitRatio++;
				}
			}
		}

		if (m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex] == 0) {
			// SKillÀÇ ÃÑÇÕÀÌ 700À» ³ÑÀ¸¸é ´Ù¸¥ ½ºÅ³Áß ÇÏ³ª¸¦ 1 ³»¸°´Ù. 
			bCheckTotalSkillMasteryPoints(iClientH, sSkillIndex);

			// SkillÀÌ ¿Ã¶ú´Ù´Â °ÍÀ» Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¾Ë·ÁÁØ´Ù.
			//SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SKILL, sSkillIndex, m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex], NULL, NULL);
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SKILL, sSkillIndex, m_pClientList[iClientH]->m_cSkillMastery[sSkillIndex], m_pClientList[iClientH]->m_iSkillSSN[sSkillIndex], NULL);
		}
	}
}

char _tmp_cCorpseX[] = { 0,  1, 1, 1, 0, -1, -1, -1, 0, 0, 0, 0 };
char _tmp_cCorpseY[] = { -1, -1, 0, 1, 1,  1,  0, -1, 0, 0, 0 };

void CGame::ClearSkillUsingStatus(int iClientH)
{
	int i;
	short tX, fX, tY, fY;

	if (m_pClientList[iClientH] == NULL) return;

	if (m_pClientList[iClientH]->m_bSkillUsingStatus[19] == TRUE) {
		tX = m_pClientList[iClientH]->m_sX;
		tY = m_pClientList[iClientH]->m_sY;
		if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bGetMoveable(tX, tY, NULL) == FALSE) {
			fX = m_pClientList[iClientH]->m_sX + _tmp_cCorpseX[m_pClientList[iClientH]->m_cDir];
			fY = m_pClientList[iClientH]->m_sY + _tmp_cCorpseY[m_pClientList[iClientH]->m_cDir];
			if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bGetMoveable(fX, fY, NULL) == FALSE) {
				m_pClientList[iClientH]->m_cDir = iDice(1, 8);
				fX = m_pClientList[iClientH]->m_sX + _tmp_cCorpseX[m_pClientList[iClientH]->m_cDir];
				fY = m_pClientList[iClientH]->m_sY + _tmp_cCorpseY[m_pClientList[iClientH]->m_cDir];
				if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bGetMoveable(fX, fY, NULL) == FALSE) {
					return;
				}
			}
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_DAMAGEMOVE, m_pClientList[iClientH]->m_cDir, NULL, NULL, NULL);
		}
	}
	// Taming
	else if (m_pClientList[iClientH]->m_bSkillUsingStatus[22] == TRUE)
	{
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SKILLUSINGEND, NULL, NULL, NULL, NULL);
		bRemoveFromDelayEventList(iClientH, DEF_OWNERTYPE_PLAYER, DEF_DELAYEVENTTYPE_TAMING_SKILL);
	}
	for (i = 0; i < DEF_MAXSKILLTYPE; i++) {
		m_pClientList[iClientH]->m_bSkillUsingStatus[i] = FALSE;
		m_pClientList[iClientH]->m_iSkillUsingTimeID[i] = NULL;
	}

	if (m_pClientList[iClientH]->m_iAllocatedFish != NULL) {
		if (m_pFish[m_pClientList[iClientH]->m_iAllocatedFish] != NULL)
			m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_sEngagingCount--;

		m_pClientList[iClientH]->m_iAllocatedFish = NULL;
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_FISHCANCELED, NULL, NULL, NULL, NULL);
	}

}

int CGame::iCalculateUseSkillItemEffect(int iOwnerH, char cOwnerType, char cOwnerSkill, int iSkillNum, char cMapIndex, int dX, int dY)
{
	class CItem* pItem;
	char  cItemName[21];
	short lX, lY;
	int   iResult, iFish;

	switch (cOwnerType) {
	case DEF_OWNERTYPE_PLAYER:
		if (m_pClientList[iOwnerH] == NULL) return 0;
		if (m_pClientList[iOwnerH]->m_cMapIndex != cMapIndex) return 0;
		lX = m_pClientList[iOwnerH]->m_sX;
		lY = m_pClientList[iOwnerH]->m_sY;
		break;

	case DEF_OWNERTYPE_NPC:
		if (m_pNpcList[iOwnerH] == NULL) return 0;
		if (m_pNpcList[iOwnerH]->m_cMapIndex != cMapIndex) return 0;
		lX = m_pNpcList[iOwnerH]->m_sX;
		lY = m_pNpcList[iOwnerH]->m_sY;
		break;
	}

	// ½ºÅ³ »ç¿ë ¿©ºÎ ÁÖ»çÀ§¸¦ ±¼¸°´Ù. 
	if (cOwnerSkill == 0) return 0;

	// ½ºÅ³ÀÌ 100ÀÌ¶ó°í ÇØµµ °¡²û ³¬½Ã¸¦ ½ÇÆÐÇÏ°Ô ÇÏ±â À§ÇØ¼­ 1D105 
	iResult = iDice(1, 105);
	if (cOwnerSkill <= iResult)	return 0;  // ½ÇÆÐ´Ù.

	// ¶¥¿¡¼­´Â ³¬½Ã°¡ ºÒ°¡´É 
	if (m_pMapList[cMapIndex]->bGetIsWater(dX, dY) == FALSE) return 0;

	// ¼º°øÇßÀ¸¹Ç·Î ½ºÅ³ Ä«¿îÆ®¸¦ ¿Ã¸°´Ù.
	if (cOwnerType == DEF_OWNERTYPE_PLAYER)
		CalculateSSN_SkillIndex(iOwnerH, iSkillNum, 1);

	switch (m_pSkillConfigList[iSkillNum]->m_sType) {
	case DEF_SKILLEFFECTTYPE_TAMING:
		// ±æµéÀÌ±â ±â¼ú: dX, dY ºÎ±ÙÀÇ ¸ó½ºÅÍ¸¦ ±æµéÀÎ´Ù.
		_TamingHandler(iOwnerH, iSkillNum, cMapIndex, dX, dY);
		break;

	case DEF_SKILLEFFECTTYPE_GET:
		// ¾ÆÀÌÅÛÀ» ¾ò´Â ±â¼úÀÌ¾ú´Ù. 
		ZeroMemory(cItemName, sizeof(cItemName));
		switch (m_pSkillConfigList[iSkillNum]->m_sValue1) {
		case 1:
			// ±¤¹° 
			wsprintf(cItemName, "Meat");
			break;

		case 2:
			// ±ÙÃ³¿¡ ´ÙÀÌ³ª¹Í ¿ÀºêÁ§Æ® ¹°°í±â°¡ Á¸ÀçÇÑ´Ù¸é º»°Ý³¬½Ã ¸ðµå·Î µé¾î°£´Ù.
			if (cOwnerType == DEF_OWNERTYPE_PLAYER) {
				iFish = iCheckFish(iOwnerH, cMapIndex, dX, dY);
				if (iFish == NULL) wsprintf(cItemName, "Fish");
			}
			else wsprintf(cItemName, "Fish");
			break;
		}

		if (strlen(cItemName) != 0) {

			// ³¬½Ã¿¡ ¼º°øÇß´Ù¸é ¸Þ½ÃÁö¸¦ Àü¼Û.
			if (memcmp(cItemName, "Fish", 4) == 0) {
				SendNotifyMsg(NULL, iOwnerH, DEF_NOTIFY_FISHSUCCESS, NULL, NULL, NULL, NULL);
				// v1.41 ¾à°£ÀÇ °æÇèÄ¡ »ó½Â 
				GetExp(iOwnerH, iDice(1, 2));
			}

			pItem = new class CItem;
			if (pItem == NULL) return 0;
			if (_bInitItemAttr(pItem, cItemName) == TRUE) {
				// ¾ÆÀÌÅÛÀ» ³õ´Â´Ù. 
				m_pMapList[cMapIndex]->bSetItem(lX, lY, pItem);

				// ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¾ÆÀÌÅÛÀÌ ¶³¾îÁø °ÍÀ» ¾Ë¸°´Ù. 
				SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, cMapIndex,
					lX, lY, pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); //v1.4
			}
		}
		break;
	}

	return 1;
}

void CGame::UseSkillHandler(int iClientH, int iV1, int iV2, int iV3)
{
	char  cOwnerType;
	short sAttackerWeapon, sOwnerH;
	int   iResult, iPlayerSkillLevel;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;

	if ((iV1 < 0) || (iV1 >= DEF_MAXSKILLTYPE)) return;
	if (m_pSkillConfigList[iV1] == NULL) return;
	// ÀÌ¹Ì ±â¼úÀ» »ç¿ëÁßÀÌ¶óµµ ¸®ÅÏ.
	if (m_pClientList[iClientH]->m_bSkillUsingStatus[iV1] == TRUE) return;

	// ÇÃ·¹ÀÌ¾îÀÇ ±â¼ú¼öÁØ¿¡ µû¶ó ¼º°ø¿©ºÎ¸¦ °è»êÇÑ´Ù. 
	iPlayerSkillLevel = m_pClientList[iClientH]->m_cSkillMastery[iV1];
	iResult = iDice(1, 100);

	if (iResult > iPlayerSkillLevel) {
		// ½ÇÆÐ´Ù. 
		// ±â¼ú »ç¿ëÀÌ ÁßÁö µÇ¾úÀ½À» ¾Ë¸°´Ù.
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SKILLUSINGEND, NULL, NULL, NULL, NULL);
		return;
	}

	// iV1ÀÌ ±â¼ú ¹øÈ£ 
	switch (m_pSkillConfigList[iV1]->m_sType) {

	case DEF_SKILLEFFECTTYPE_TAMING: // Taming
		if (((m_pClientList[iClientH]->m_iStatus & 0x00000010) == 0)      // Invi character can not tame !
			&& (m_pClientList[iClientH]->m_bSkillUsingStatus[19] == FALSE)) // PretendCorpse character canot Tame
		{
			m_pClientList[iClientH]->m_bSkillUsingStatus[iV1] = TRUE;
			m_pClientList[iClientH]->m_iSkillUsingTimeID[iV1] = (int)timeGetTime();
			// Register delay to perform Taming
			bRegisterDelayEvent(DEF_DELAYEVENTTYPE_TAMING_SKILL // int iDelayType
				, 22 // int iEffectType
				, timeGetTime() + m_pSkillConfigList[22]->m_sValue2 * 100 // nbe of 1/10th seconds in Skill.cfg
				, iClientH // int iTargetH
				, DEF_OWNERTYPE_PLAYER // char cTargetType
				, m_pClientList[iClientH]->m_cMapIndex // char cMapIndex
				, m_pClientList[iClientH]->m_sX // int X
				, m_pClientList[iClientH]->m_sY // int Y
				, (int)m_pClientList[iClientH]->m_bSkillUsingStatus[iV1] // int iV1
				, m_pClientList[iClientH]->m_iSkillUsingTimeID[iV1] // int iV2
				, NULL); // int iV3		
		}
		else SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SKILLUSINGEND, NULL, NULL, NULL, NULL);
		break;

	case DEF_SKILLEFFECTTYPE_PRETEND:
		switch (m_pSkillConfigList[iV1]->m_sValue1) {
		case 1:
			// Á×ÀºÃ´ÇÏ±â ±â¼úÀÌ´Ù.	

			// v1.44 »çÅõÀåÀÌ¸é Á×ÀºÃ´ÇÏ±â ¸øÇÑ´Ù.
			if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFightZone == TRUE) {
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SKILLUSINGEND, NULL, NULL, NULL, NULL);
				return;
			}

			//¸¸¾à ¹Ù´Ú¿¡ ½ÃÃ¼°¡ ÀÖ´Ù¸é Á×ÀºÃ´ ÇÏ±â¸¦ ÇÒ ¼ö ¾ø´Ù. 
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetDeadOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
			if (sOwnerH != NULL) {
				// Á×ÀºÃ´ ÇÏ±â¸¦ ÇÒ ÀÚ¸®¿¡ ½ÃÃ¼°¡ ÀÖ¾î ±â¼ú »ç¿ëÀÌ ÁßÁö µÇ¾úÀ½À» ¾Ë¸°´Ù.
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SKILLUSINGEND, NULL, NULL, NULL, NULL);
				return;
			}

			// Á×ÀºÃ´ ÇÏ°íÀÚ ÇÏ´Â ÁÖº¯¿¡ ¹°Ã¼°¡ ÀÖ¾îµµ ÇÒ ¼ö ¾ø´Ù. 
			iResult = 0;
			if (m_pClientList[iClientH]->m_iAdminUserLevel <= 0) {
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY - 1);
				iResult += sOwnerH;
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY + 1);
				iResult += sOwnerH;
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX - 1, m_pClientList[iClientH]->m_sY);
				iResult += sOwnerH;
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX + 1, m_pClientList[iClientH]->m_sY);
				iResult += sOwnerH;

				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX - 1, m_pClientList[iClientH]->m_sY - 1);
				iResult += sOwnerH;
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX + 1, m_pClientList[iClientH]->m_sY - 1);
				iResult += sOwnerH;
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX - 1, m_pClientList[iClientH]->m_sY + 1);
				iResult += sOwnerH;
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, m_pClientList[iClientH]->m_sX + 1, m_pClientList[iClientH]->m_sY + 1);
				iResult += sOwnerH;
			}

			if (iResult != 0) {
				// Á×ÀºÃ´ ÇÏ±â¸¦ ÇÒ ÀÚ¸®¿¡ ½ÃÃ¼°¡ ÀÖ¾î ±â¼ú »ç¿ëÀÌ ÁßÁö µÇ¾úÀ½À» ¾Ë¸°´Ù.
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SKILLUSINGEND, NULL, NULL, NULL, NULL);
				return;
			}

			// ½ºÅ³ Ä«¿îÆ® ¿Ã¸°´Ù. <-- ³»ºÎ¿¡¼­ Ä«¿îÆÃÀ» ÇØ¾ß ¾û¶×ÇÑ ½ºÅ³ÀÌ ¿À¸£´Â °æ¿ì°¡ ¾ø´Ù. 
			CalculateSSN_SkillIndex(iClientH, iV1, 1);

			// ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô Á×´Â µ¿ÀÛ Àü¼Û.
			sAttackerWeapon = 1;
			SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDYING, 0, sAttackerWeapon, NULL);
			// Á¤»ó À§Ä¡¿¡¼­ Áö¿î´Ù.
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->ClearOwner(14, iClientH, DEF_OWNERTYPE_PLAYER, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
			// Á×Àº À§Ä¡ Ç¥½Ã¸¦ ÇÑ´Ù.
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetDeadOwner(iClientH, DEF_OWNERTYPE_PLAYER, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY);
			break;
		}
		break;

	}

	m_pClientList[iClientH]->m_bSkillUsingStatus[iV1] = TRUE;
}

void CGame::SetDownSkillIndexHandler(int iClientH, int iSkillIndex)
{
	if (m_pClientList[iClientH] == NULL) return;
	if ((iSkillIndex < 0) || (iSkillIndex >= DEF_MAXSKILLTYPE)) return;

	if (m_pClientList[iClientH]->m_cSkillMastery[iSkillIndex] > 0)
		m_pClientList[iClientH]->m_iDownSkillIndex = iSkillIndex;

	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_DOWNSKILLINDEXSET, m_pClientList[iClientH]->m_iDownSkillIndex, NULL, NULL, NULL);
}

void CGame::_TamingHandler(int iClientH, int iSkillNum, char cMapIndex, int dX, int dY)
{
	int iSkillLevel, iRange, iTamingLevel, iResult, iX, iY, iTamingType = 0, iNbeTamed = 0;
	short sOwnerH;
	char cOwnerType, cTamedString[500];
	if (m_pClientList[iClientH] == NULL) return;
	if (m_pMapList[cMapIndex] == NULL) return;
	if (m_pClientList[iClientH]->m_iAdminUserLevel == 0)
	{
		if ((m_pClientList[iClientH]->m_iStr > 100)
			&& (m_pClientList[iClientH]->m_iCharisma > 49))
		{
			iTamingType = 2; // War-GuildMaster
		}
		else if ((m_pClientList[iClientH]->m_iInt > 100)
			&& (m_pClientList[iClientH]->m_iCharisma > 49))
		{
			iTamingType = 1; // Mage-GuildMaster
		}
	}
	else iTamingType = 3; // GameMaster
	iSkillLevel = (int)m_pClientList[iClientH]->m_cSkillMastery[iSkillNum];
	iRange = 2 + iSkillLevel / 20;
	for (iX = dX - iRange; iX <= dX + iRange; iX++)
		for (iY = dY - iRange; iY <= dY + iRange; iY++)
		{
			sOwnerH = NULL;
			if ((iX > 0) && (iY > 0) && (iX < m_pMapList[cMapIndex]->m_sSizeX) && (iY < m_pMapList[cMapIndex]->m_sSizeY))
			{
				m_pMapList[cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, iX, iY);
			}
			if (sOwnerH != NULL) {
				switch (cOwnerType) {
				case DEF_OWNERTYPE_PLAYER:
					break;
				case DEF_OWNERTYPE_NPC:
					if (m_pNpcList[sOwnerH] == NULL) break;
					// 1st list for everybody			
					iTamingLevel = 201; // 201 means impossible
					switch (m_pNpcList[sOwnerH]->m_sType) {
					case 14: iTamingLevel = 35; break;	// Orc
					case 28: iTamingLevel = 65; break;	// Troll
					case 29: iTamingLevel = 100; break;	// Ogre
					case 58: iTamingLevel = 100; break;	// MG
					case 59: iTamingLevel = 120; break;	// Ettin
					case 77: iTamingLevel = 160; break;	// MasterMage-Orc 
					case 78: iTamingLevel = 170; break;	// Minos 
					}
					switch (iTamingType) {
					case 1: // Mage-GuildMaster				
						if (iTamingLevel > 199) iTamingLevel = 199;
						switch (m_pNpcList[sOwnerH]->m_sType) {
						case 12:							// Stone
						case 23: iTamingLevel = 40; break;	// Clay
						case 65: iTamingLevel = 70; break;	// IceGolem
						case 80: iTamingLevel = 80; break;	// Tentocle
						case 53: iTamingLevel = 100; break;	// BB
						case 31: iTamingLevel = 170; break;	// DD
						}
						break;
					case 2: // War-GuildMaster	
						if (iTamingLevel > 197) iTamingLevel = 197;
						switch (m_pNpcList[sOwnerH]->m_sType) {
						case 18:							// Zombie 
						case 11: iTamingLevel = 35; break;	// Skel
						case 27: iTamingLevel = 65; break;	// hh
						case 30: iTamingLevel = 120; break;	// Liche
						case 31: iTamingLevel = 170; break;	// DD
						case 32: // Uni possible for some purest Wars
							if ((m_pClientList[iClientH]->m_iPKCount == 0) // Uni
								&& (m_pClientList[iClientH]->m_iEnemyKillCount == 0)
								&& (m_pClientList[iClientH]->m_iRating > 0))
							{
								iTamingLevel = 100;
							}
							break;
						}
						break;
					case 3: // GameMaster
						iTamingLevel = 30;
						if (m_pNpcList[sOwnerH]->m_cSide == 0) iTamingLevel = 201;
						break;
					}
					iResult = iDice(2, iSkillLevel);
					if ((iResult >= iTamingLevel)
						&& (m_pNpcList[sOwnerH]->m_dwTamingTime == 0)
						&& (m_pNpcList[sOwnerH]->m_cSide != m_pClientList[iClientH]->m_cSide))
					{
						iNbeTamed++;
						// Now change npc side...
						m_pNpcList[sOwnerH]->m_cOriginalSide = m_pNpcList[sOwnerH]->m_cSide;
						switch (iTamingType) {
						default:
						case 0: // Default -> npc becomes neutral 	
						case 3: // GameMaster
							m_pNpcList[sOwnerH]->m_cSide = 0;
							break;
						case 1: // Mage-GuildMaster -> npc becomes friendly 					
							switch (m_pNpcList[sOwnerH]->m_sType) {
							case 12: // Stone
							case 23: // Clay
							case 65: // IceGolem
							case 80: // Tentocle
							case 53: // BB
							case 31: // DD						
								m_pNpcList[sOwnerH]->m_cSide = m_pClientList[iClientH]->m_cSide;
								break;
							default:
								m_pNpcList[sOwnerH]->m_cSide = 0;
								break;
							}
							break;
						case 2: // War-GuildMaster	-> npc becomes friendly
							switch (m_pNpcList[sOwnerH]->m_sType) {
							case 18: // Zombie 
							case 11: // Skel
							case 27: // hh
							case 30: // Liche
							case 31: // DD
								if (iDice(1, 95) <= iSkillLevel)
									m_pNpcList[sOwnerH]->m_cSide = m_pClientList[iClientH]->m_cSide;
								else m_pNpcList[sOwnerH]->m_cSide = 0;
								break;
							case 32: // Uni
								m_pNpcList[sOwnerH]->m_cSide = m_pClientList[iClientH]->m_cSide;
								break;
							default:
								m_pNpcList[sOwnerH]->m_cSide = 0;
								break;
							}
							break;
						}
						if (iGetFollowerNumber(sOwnerH, cOwnerType) > 5) return;
						m_pNpcList[sOwnerH]->m_dwTamingTime = timeGetTime() + 3000 * iSkillLevel + 120000;
						m_pNpcList[sOwnerH]->m_cBehavior = DEF_BEHAVIOR_MOVE;
						m_pNpcList[sOwnerH]->m_sBehaviorTurnCount = 0;
						m_pNpcList[sOwnerH]->m_iTargetIndex = NULL;
						if (m_pNpcList[sOwnerH]->m_cSide == m_pClientList[iClientH]->m_cSide)
						{
							m_pNpcList[sOwnerH]->m_cMoveType = DEF_MOVETYPE_FOLLOW;
							m_pNpcList[sOwnerH]->m_cFollowOwnerType = DEF_OWNERTYPE_PLAYER;
							m_pNpcList[sOwnerH]->m_iFollowOwnerIndex = iClientH;
						}
						SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, NULL, NULL, NULL);
						// Increase skill if not easy Taming
						if (iTamingLevel >= iSkillLevel)
						{
							CalculateSSN_SkillIndex(iClientH, 22, 1);
						}
					} // End of succes in taming 1 creature
					break;
				}
			}
		}
	if (iNbeTamed == 0)  // No creature tamed
	{
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SKILLUSINGEND, 0, NULL, NULL, NULL);
	}
	else
	{
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SKILLUSINGEND, 1, NULL, NULL, NULL);
		ZeroMemory(cTamedString, sizeof(cTamedString));
		switch (iTamingType) {
		case 2:	// War-GuildMaster
			wsprintf(cTamedString, "Undead turned: %d", iNbeTamed);
			break;
		case 1:	// Mage-GuildMaster
			wsprintf(cTamedString, "Magic things controled: %d", iNbeTamed);
			break;
		default: // GameMaster
			wsprintf(cTamedString, "Creatures neutralised: %d", iNbeTamed);
			break;
		}
		SendNotifyMsg(iClientH, iClientH, DEF_NOTIFY_NOTICEMSG, NULL, NULL, NULL, cTamedString);
		// Debug	
		wsprintf(cTamedString, "(!) Taming skill: %s is using Taming skill (%d%%) (Type %d), he tamed %d creatures."
			, m_pClientList[iClientH]->m_cCharName
			, iSkillLevel
			, iTamingType
			, iNbeTamed);
		PutLogList(cTamedString);
	}
}

void CGame::GetMagicAbilityHandler(int iClientH)
{
	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_cSkillMastery[4] != 0) return;

	m_pClientList[iClientH]->m_cSkillMastery[4] = 20;
	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SKILL, 4, m_pClientList[iClientH]->m_cSkillMastery[4], NULL, NULL);
	bCheckTotalSkillMasteryPoints(iClientH, 4);
}

BOOL CGame::bPlantSeedBag(int iMapIndex, int dX, int dY, int iItemEffectValue1, int iItemEffectValue2, int iClientH)
{
	int iNamingValue, tX, tY;
	short sOwnerH;
	char cOwnerType, cNpcName[21], cName[21], cNpcWaypointIndex[11];
	BOOL bRet;

	if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_iTotalAgriculture >= 200) {
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_NOMOREAGRICULTURE, NULL, NULL, NULL, NULL);
		return FALSE;
	}

	if (iItemEffectValue2 > m_pClientList[iClientH]->m_cSkillMastery[2]) {
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_AGRICULTURESKILLLIMIT, NULL, NULL, NULL, NULL);
		return FALSE;
	}

	iNamingValue = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->iGetEmptyNamingValue();

	if (iNamingValue != -1) {

		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);
		if (sOwnerH != NULL && sOwnerH == DEF_OWNERTYPE_NPC && m_pNpcList[sOwnerH]->m_cActionLimit == 5) {
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_AGRICULTURENOAREA, NULL, NULL, NULL, NULL);
			return FALSE;
		}
		else {
			if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bGetIsFarm(dX, dY) == FALSE) {
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_AGRICULTURENOAREA, NULL, NULL, NULL, NULL);
				return FALSE;
			}

			ZeroMemory(cNpcName, sizeof(cNpcName));
			strcpy(cNpcName, "Crops");
			ZeroMemory(cName, sizeof(cName));
			wsprintf(cName, "XX%d", iNamingValue);
			cName[0] = '_';
			cName[1] = iMapIndex + 65;

			ZeroMemory(cNpcWaypointIndex, sizeof(cNpcWaypointIndex));
			tX = dX;
			tY = dY;

			bRet = bCreateNewNpc(cNpcName, cName, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, 0, 0, DEF_MOVETYPE_RANDOM, &tX, &tY, cNpcWaypointIndex, NULL, NULL, 0, FALSE, TRUE);
			if (bRet == FALSE) {
				m_pMapList[iMapIndex]->SetNamingValueEmpty(iNamingValue);
			}
			else {
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, tX, tY);
				if (m_pNpcList[sOwnerH] == NULL) return 0;
				m_pNpcList[sOwnerH]->m_cCropType = iItemEffectValue1;
				switch (iItemEffectValue1) {
				case 1: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
				case 2: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
				case 3: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
				case 4: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
				case 5: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
				case 6: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
				case 7: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
				case 8: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
				case 9: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
				case 10: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
				case 11: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
				case 12: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
				case 13: m_pNpcList[sOwnerH]->m_cCropSkill = iItemEffectValue2; break;
				default: m_pNpcList[sOwnerH]->m_cCropSkill = 100; break;
				}
				m_pNpcList[sOwnerH]->m_sAppr2 = 1;
				SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_NPC, MSGID_EVENT_LOG, DEF_MSGTYPE_CONFIRM, NULL, NULL, NULL);
				wsprintf(G_cTxt, "(skill:%d type:%d)plant(%s) Agriculture begin(%d,%d) sum(%d)!", m_pNpcList[sOwnerH]->m_cCropSkill, m_pNpcList[sOwnerH]->m_cCropType, cNpcName, tX, tY, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_iTotalAgriculture);
				PutLogList(G_cTxt);
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CGame::_CheckFarmingAction(short sAttackerH, short sTargetH, BOOL bType)
{
	char cCropType;
	int iItemID;
	class CItem* pItem;

	iItemID = 0;
	cCropType = 0;

	cCropType = m_pNpcList[sTargetH]->m_cCropType;
	switch (cCropType) {
	case 1: GetExp(sAttackerH, iDice(3, 100) * 100); iItemID = 820; break; // WaterMelon
	case 2: GetExp(sAttackerH, iDice(3, 100) * 100); iItemID = 821; break; // Pumpkin
	case 3: GetExp(sAttackerH, iDice(4, 100) * 100); iItemID = 822; break; // Garlic
	case 4: GetExp(sAttackerH, iDice(4, 100) * 100); iItemID = 823; break; // Barley
	case 5: GetExp(sAttackerH, iDice(5, 100) * 100); iItemID = 824; break; // Carrot
	case 6: GetExp(sAttackerH, iDice(5, 100) * 100); iItemID = 825; break; // Radish
	case 7: GetExp(sAttackerH, iDice(6, 100) * 100); iItemID = 826; break; // Corn
	case 8: GetExp(sAttackerH, iDice(6, 100) * 100); iItemID = 827; break; // ChineseBellflower
	case 9: GetExp(sAttackerH, iDice(7, 100) * 100); iItemID = 828; break; // Melone
	case 10: GetExp(sAttackerH, iDice(7, 100) * 100); iItemID = 829; break; // Tommato
	case 11: GetExp(sAttackerH, iDice(8, 100) * 100); iItemID = 830; break; // Grapes
	case 12: GetExp(sAttackerH, iDice(8, 100) * 100); iItemID = 831; break; // BlueGrapes
	case 13: GetExp(sAttackerH, iDice(9, 100) * 100); iItemID = 832; break; // Mushroom
	default: GetExp(sAttackerH, iDice(1, 100) * 100); iItemID = 721; break; // Ginseng

	}

	pItem = new class CItem;
	if (_bInitItemAttr(pItem, iItemID) == FALSE) {
		delete pItem;
	}
	if (bType == 0) {
		m_pMapList[m_pClientList[sAttackerH]->m_cMapIndex]->bSetItem(m_pClientList[sAttackerH]->m_sX, m_pClientList[sAttackerH]->m_sY, pItem);
		SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[sAttackerH]->m_cMapIndex,
			m_pClientList[sAttackerH]->m_sX, m_pClientList[sAttackerH]->m_sY, pItem->m_sSprite,
			pItem->m_sSpriteFrame, pItem->m_cItemColor, FALSE);
	}
	else if (bType == 1) {
		m_pMapList[m_pNpcList[sTargetH]->m_cMapIndex]->bSetItem(m_pNpcList[sTargetH]->m_sX, m_pNpcList[sTargetH]->m_sY, pItem);
		SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pNpcList[sTargetH]->m_cMapIndex,
			m_pNpcList[sTargetH]->m_sX, m_pNpcList[sTargetH]->m_sY, pItem->m_sSprite,
			pItem->m_sSpriteFrame, pItem->m_cItemColor, FALSE);
	}

}

//LifeX Auto Skills
void CGame::AutoSkill(int iClientH)
{
	int i;

	if (m_pClientList[iClientH] == NULL) return;

	for (i = 0; i < 24; i++)
	{
		m_pClientList[iClientH]->m_cSkillMastery[i] = 100;
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SKILL, i, m_pClientList[iClientH]->m_cSkillMastery[i], NULL, NULL);
	}
}

void CGame::bCheckTotalSkillMasteryPoints(int iClientH, int iSkill)
{
	int i;
	int iRemainPoint, iTotalPoints, iWeaponIndex, iDownSkillSSN, iDownPoint;
	short sDownSkillIndex;

	if (m_pClientList[iClientH] == NULL) return;

	iTotalPoints = 0;
	for (i = 0; i < DEF_MAXSKILLTYPE; i++)
		iTotalPoints += m_pClientList[iClientH]->m_cSkillMastery[i];

	iRemainPoint = iTotalPoints - m_sCharSkillLimit;

	while (iRemainPoint > 0) {

		sDownSkillIndex = -1;
		if ((m_pClientList[iClientH]->m_iDownSkillIndex != -1)
			&& (m_pClientList[iClientH]->m_cSkillMastery[m_pClientList[iClientH]->m_iDownSkillIndex] > 0))
		{
			sDownSkillIndex = m_pClientList[iClientH]->m_iDownSkillIndex;
		}
		else
		{
			iDownSkillSSN = 99999999;
			for (i = 0; i < DEF_MAXSKILLTYPE; i++)
			{
				if ((m_pClientList[iClientH]->m_cSkillMastery[i] >= 21) && (i != iSkill)
					&& (m_pClientList[iClientH]->m_iSkillSSN[i] <= iDownSkillSSN))
				{
					iDownSkillSSN = m_pClientList[iClientH]->m_iSkillSSN[i];
					m_pClientList[iClientH]->m_iDownSkillIndex = i; // Snoopy: impose a downskill index !
					sDownSkillIndex = i; // returns the skill further from next up, over 20...
				}
			}
		}

		if (sDownSkillIndex != -1)
		{
			if (m_pClientList[iClientH]->m_cSkillMastery[sDownSkillIndex] <= 20) // v1.4
				iDownPoint = m_pClientList[iClientH]->m_cSkillMastery[sDownSkillIndex];
			else iDownPoint = 1;
			
			m_pClientList[iClientH]->m_cSkillMastery[sDownSkillIndex] -= iDownPoint; // v1.4
			m_pClientList[iClientH]->m_iSkillSSN[sDownSkillIndex] = m_iSkillSSNpoint[m_pClientList[iClientH]->m_cSkillMastery[sDownSkillIndex] + 1] - 1;
			
			iRemainPoint -= iDownPoint; // v1.4				
			if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1)
			{
				iWeaponIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
				if (m_pClientList[iClientH]->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sDownSkillIndex)
				{
					m_pClientList[iClientH]->m_iHitRatio -= iDownPoint; // v1.4
					if (m_pClientList[iClientH]->m_iHitRatio < 0) m_pClientList[iClientH]->m_iHitRatio = 0;
				}
			}
			if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1) {
				iWeaponIndex = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND];
				if (m_pClientList[iClientH]->m_pItemList[iWeaponIndex]->m_sRelatedSkill == sDownSkillIndex)
				{
					m_pClientList[iClientH]->m_iHitRatio -= iDownPoint; // v1.4
					if (m_pClientList[iClientH]->m_iHitRatio < 0) m_pClientList[iClientH]->m_iHitRatio = 0;
				}
			}
			//SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SKILL, sDownSkillIndex, m_pClientList[iClientH]->m_cSkillMastery[sDownSkillIndex], NULL, NULL);
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SKILL, sDownSkillIndex, m_pClientList[iClientH]->m_cSkillMastery[sDownSkillIndex], m_pClientList[iClientH]->m_iSkillSSN[sDownSkillIndex], NULL);
		}
		else 
		{
			return;
		}
	}
}

// 05/24/2004 - Hypnotoad - Hammer and Wand train to 100% fixed
void CGame::_CheckAttackType(int iClientH, short* spType)
{
	WORD wType;

	if (m_pClientList[iClientH] == NULL) return;
	wType = ((m_pClientList[iClientH]->m_sAppr2 & 0x0FF0) >> 4);

	switch (*spType) 
	{
	case 2:
		if (m_pClientList[iClientH]->m_cArrowIndex == -1) *spType = 0;
		if (wType < 40) *spType = 1;
		break;

	case 20:
		if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0)  *spType = 1;
		if (m_pClientList[iClientH]->m_cSkillMastery[5] < 100) *spType = 1;
		break;

	case 21:
		if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0)  *spType = 1;
		if (m_pClientList[iClientH]->m_cSkillMastery[7] < 100) *spType = 1;
		break;

	case 22:
		if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0)  *spType = 1;
		if (m_pClientList[iClientH]->m_cSkillMastery[9] < 100) *spType = 1;
		break;

	case 23:
		// Ã€Ã¥Â°Ã‹ Ã‡ÃŠÂ»Ã¬Â±Ã¢ 
		if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0)   *spType = 1;
		if (m_pClientList[iClientH]->m_cSkillMastery[8] < 100) *spType = 1;
		break;

	case 24:
		// ÂµÂµÂ³Â¢ Ã‡ÃŠÂ»Ã¬Â±Ã¢ 
		if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0)  *spType = 1;
		if (m_pClientList[iClientH]->m_cSkillMastery[10] < 100) *spType = 1;
		break;

	case 25:
		// ÃˆÂ° Ã‡ÃŠÂ»Ã¬Â±Ã¢ 
		if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0)  *spType = 2;
		if (m_pClientList[iClientH]->m_cSkillMastery[6] < 100) *spType = 2;
		if (m_pClientList[iClientH]->m_cArrowIndex == -1)      *spType = 0;
		// ÃˆÂ° Â°Ã¸Â°ÃÃ€ÃŽÂµÂ¥ Ã€Ã¥Ã‚Ã¸Ã‡Ã‘ Â¹Â«Â±Ã¢Â°Â¡ ÃˆÂ°Ã€ÃŒ Â¾Ã†Â´ÃÂ´Ã™. Ã€ÃÂ¹Ã Â°Ã¸Â°ÃÃ€Â¸Â·ÃŽ Ã€Ã¼ÃˆÂ¯.
		if (wType < 40) *spType = 1;
		break;

	case 26:
		// v2.16 2002-5-27 ÇØ¸Ó ÇÊ»ì±â 
		if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0)  *spType = 1;
		if (m_pClientList[iClientH]->m_cSkillMastery[14] < 100) *spType = 1;
		break;

	case 27:
		// v2.16 2002-5-27 ÁöÆÎÀÌ ÇÊ»ì±â 
		if (m_pClientList[iClientH]->m_iSuperAttackLeft <= 0)  *spType = 1;
		if (m_pClientList[iClientH]->m_cSkillMastery[21] < 100) *spType = 1;
		break;
	}
}