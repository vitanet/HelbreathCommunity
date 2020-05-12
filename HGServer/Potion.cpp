// Potion.cpp: implementation of the CPotion class.
//
//////////////////////////////////////////////////////////////////////

#include "Potion.h"

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

CPotion::CPotion()
{
 int i;
	
	ZeroMemory(m_cName, sizeof(m_cName));
	m_iSkillLimit = 0;
	m_iDifficulty = 0;

	for (i = 0; i < 12; i++)
		m_sArray[i] = -1;
}

CPotion::~CPotion()
{

}

void CGame::ReqCreatePotionHandler(int iClientH, char* pData)
{
	DWORD* dwp;
	WORD* wp;
	char* cp, cI[6], cPotionName[21], cData[120];
	int    iRet, i, j, iEraseReq, iSkillLimit, iSkillLevel, iResult, iDifficulty;
	short* sp, sItemIndex[6], sTemp;
	short  sItemNumber[6], sItemArray[12];
	BOOL   bDup, bFlag;
	class  CItem* pItem;

	if (m_pClientList[iClientH] == NULL) return;
	m_pClientList[iClientH]->m_iSkillMsgRecvCount++;

	for (i = 0; i < 6; i++) {
		cI[i] = -1;
		sItemIndex[i] = -1;
		sItemNumber[i] = 0;
	}

	cp = (char*)(pData + 11);
	cI[0] = *cp;
	cp++;
	cI[1] = *cp;
	cp++;
	cI[2] = *cp;
	cp++;
	cI[3] = *cp;
	cp++;
	cI[4] = *cp;
	cp++;
	cI[5] = *cp;
	cp++;
	// Æ÷¼ÇÀÇ Àç·á ÀÎµ¦½º¸¦ ¹Þ¾Ò´Ù. ÀÌ Àç·á°¡ Æ÷¼ÇÀ» ¸¸µé ¼ö ÀÖ´Â Á¶ÇÕÀÎÁö È®ÀÎÇÑ´Ù. 

	// µ¥ÀÌÅÍ°¡ À¯È¿ÇÑ ¾ÆÀÌÅÛ ÀÎµ¦½ºÀÎÁö Ã¼Å©ÇÑ´Ù.
	for (i = 0; i < 6; i++) {
		if (cI[i] >= DEF_MAXITEMS) return;
		if ((cI[i] >= 0) && (m_pClientList[iClientH]->m_pItemList[cI[i]] == NULL)) return;
	}

	for (i = 0; i < 6; i++)
		if (cI[i] >= 0) {
			// ¸ÕÀú ÀÌ¹Ì ÀÖ´Â ¸®½ºÆ®ÀÎÁö °Ë»ö 
			bDup = FALSE;
			for (j = 0; j < 6; j++)
				if (sItemIndex[j] == cI[i]) {
					// ÀÖ´Ù. Ä«¿îÆ® Áõ°¡ 
					sItemNumber[j]++;
					bDup = TRUE;
				}
			if (bDup == FALSE) {
				// ¾ø´Ù. »õ·Î Ãß°¡ÇÑ´Ù.
				for (j = 0; j < 6; j++)
					if (sItemIndex[j] == -1) {
						sItemIndex[j] = cI[i];
						sItemNumber[j]++;
						goto RCPH_LOOPBREAK;
					}
			RCPH_LOOPBREAK:;
			}
		}

	// ¾ÆÀÌÅÛ ¸®½ºÆ®°¡ ¸¸µé¾î Á³´Ù. ¼ÒºñµÇ´Â ¾ÆÀÌÅÛÀÌ¶ó¸é °¹¼ö¸¦ È®ÀÎÇÑ´Ù. 
	for (i = 0; i < 6; i++)
		if (sItemIndex[i] != -1) {
			if (sItemIndex[i] < 0) return;
			if ((sItemIndex[i] >= 0) && (sItemIndex[i] >= DEF_MAXITEMS)) return;
			if (m_pClientList[iClientH]->m_pItemList[sItemIndex[i]] == NULL) return;
			// ¾ÆÀÌÅÛÀÌ °¹¼ö°¡ ¿À¹öÇØµµ ¸®ÅÏ.
			if (m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_dwCount < sItemNumber[i]) return;
		}

	// ¾ÆÀÌÅÛÀ» ¾ÆÀÌÅÛ ¾ÆÀÌµð ¹øÈ£°¡ Å« ¼ø¼­ºÎÅÍ Á¤·ÄÇÑ´Ù. Bubble Sort
	bFlag = TRUE;
	while (bFlag == TRUE) {
		bFlag = FALSE;
		for (i = 0; i < 5; i++)
			if ((sItemIndex[i] != -1) && (sItemIndex[i + 1] != -1)) {
				if ((m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_sIDnum) <
					(m_pClientList[iClientH]->m_pItemList[sItemIndex[i + 1]]->m_sIDnum)) {
					// ¹Ù²Û´Ù.
					sTemp = sItemIndex[i + 1];
					sItemIndex[i + 1] = sItemIndex[i];
					sItemIndex[i] = sTemp;
					sTemp = sItemNumber[i + 1];
					sItemNumber[i + 1] = sItemNumber[i];
					sItemNumber[i] = sTemp;
					bFlag = TRUE;
				}
			}
	}

	//testcode


	// µ¥ÀÌÅÍ¿¡ ÀÌ»óÀÌ ¾øÀ½ÀÌ È®ÀÎµÇ¾ú´Ù. Æ÷¼Ç Á¦ÀÛ ¹è¿­À» ÀÛ¼ºÇÑ´Ù. 
	j = 0;
	for (i = 0; i < 6; i++) {
		if (sItemIndex[i] != -1)
			sItemArray[j] = m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_sIDnum;
		else sItemArray[j] = sItemIndex[i];
		sItemArray[j + 1] = sItemNumber[i];
		j += 2;
	}

	// sItemArrayÀÇ 12°¡Áö ¹è¿­°ú ÀÏÄ¡ÇÏ´Â Æ÷¼ÇÀ» ¸®½ºÆ®¿¡¼­ °Ë»öÇÑ´Ù. 
	ZeroMemory(cPotionName, sizeof(cPotionName));

	for (i = 0; i < DEF_MAXPOTIONTYPES; i++)
		if (m_pPotionConfigList[i] != NULL) {
			bFlag = FALSE;
			for (j = 0; j < 12; j++)
				if (m_pPotionConfigList[i]->m_sArray[j] != sItemArray[j]) bFlag = TRUE;

			if (bFlag == FALSE) {
				// Æ÷¼Ç Á¶ÇÕÀÌ ÀÏÄ¡ÇÏ´Â ¸®½ºÆ®¸¦ ¹ß°ßÇß´Ù. 
				ZeroMemory(cPotionName, sizeof(cPotionName));
				memcpy(cPotionName, m_pPotionConfigList[i]->m_cName, 20);
				iSkillLimit = m_pPotionConfigList[i]->m_iSkillLimit;
				iDifficulty = m_pPotionConfigList[i]->m_iDifficulty;
			}
		}

	// Á¶ÇÕÀÌ ÀÏÄ¡ÇÏ´Â Æ÷¼ÇÀÌ ¾øÀ¸¹Ç·Î ¹«½Ã
	if (strlen(cPotionName) == 0) {
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_NOMATCHINGPOTION, NULL, NULL, NULL, NULL);
		return;
	}

	// ÇÃ·¹ÀÌ¾îÀÇ ¿¬±Ý¼ú ½ºÅ³ ¼öÁØ
	iSkillLevel = m_pClientList[iClientH]->m_cSkillMastery[12];
	if (iSkillLimit > iSkillLevel) {
		// ÇÃ·¹ÀÌ¾îÀÇ ½ºÅ³ÀÌ ³·¾Æ ¸¸µé ¼ö ¾ø´Â Æ÷¼ÇÀÌ´Ù.
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_LOWPOTIONSKILL, NULL, NULL, NULL, cPotionName);
		return;
	}

	// Æ÷¼ÇÀÇ Á¦ÀÛ ³­ÀÌµµ¿¡ µû¶ó ¼º°ø·üÀ» Á¶Á¤ÇÑ´Ù.
	iSkillLevel -= iDifficulty;
	if (iSkillLevel <= 0) iSkillLevel = 1;

	iResult = iDice(1, 100);
	if (iResult > iSkillLevel) {
		// ½ºÅ³ ½ÇÆÐ 
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_POTIONFAIL, NULL, NULL, NULL, cPotionName);
		return;
	}

	// ¸ðµç Á¶°ÇÀÌ ¸¸Á·µÇ¾î Æ÷¼ÇÀ» ¸¸µé ¼ö ÀÖ´Ù. ½ºÅ³Ä«¿îÆ®µµ Áõ°¡ 
	CalculateSSN_SkillIndex(iClientH, 12, 1);

	// ¸¸Á·ÇÏ´Â Æ÷¼Ç Á¶ÇÕÀÌ ÀÖ´Ù¸é strlen(cPotionName)ÀÌ 0ÀÌ ¾Æ´Ï´Ù. ÀÌ¹Ì °Ë»çÇßÁö¸¸ 
	if (strlen(cPotionName) != 0) {
		pItem = NULL;
		pItem = new class CItem;
		if (pItem == NULL) return;

		// Æ÷¼ÇÀ» Á¦ÀÛÇßÀ¸¹Ç·Î Àç·á¸¦ ¾ø¾Ø´Ù.
		for (i = 0; i < 6; i++)
			if (sItemIndex[i] != -1) {
				if (m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME)
					// v1.41 !!!
					SetItemCount(iClientH, sItemIndex[i], m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_dwCount - sItemNumber[i]);
				else ItemDepleteHandler(iClientH, sItemIndex[i], FALSE, TRUE);
			}

		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_POTIONSUCCESS, NULL, NULL, NULL, cPotionName);
		// v1.41 ¼Ò·®ÀÇ °æÇèÄ¡ Áõ°¡ 
		GetExp(iClientH, iDice(1, (iDifficulty / 3)));

		if ((_bInitItemAttr(pItem, cPotionName) == TRUE)) {
			// Æ÷¼Ç ¾ÆÀÌÅÛÀÌ ¸¸µé¾îÁ³´Ù. ÇÃ·¹ÀÌ¾îÀÇ ¾ÆÀÌÅÛ ¸®½ºÆ®¿¡ µî·ÏÇÑ´Ù. ¸¸¾à °ø°£ÀÌ ¾ø°Å³ª 
			// ¹«°Ô°¡ ¸ðÀÚ¶ó¸é ¹ß ¹Ø¿¡ ¶³¾îÁ®¾ß ÇÑ´Ù.
			if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == TRUE) {
				ZeroMemory(cData, sizeof(cData));
				// ¾ÆÀÌÅÛÀ» È¹µæÇß´Ù.
				dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_ITEMOBTAINED;

				cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);

				// 1°³ È¹µæÇß´Ù. Amount°¡ ¾Æ´Ï´Ù!
				*cp = 1;
				cp++;

				memcpy(cp, pItem->m_cName, 20);
				cp += 20;

				dwp = (DWORD*)cp;
				*dwp = pItem->m_dwCount;	// ¼ö·®À» ÀÔ·Â 
				cp += 4;

				*cp = pItem->m_cItemType;
				cp++;

				*cp = pItem->m_cEquipPos;
				cp++;

				*cp = (char)0; // ¾òÀº ¾ÆÀÌÅÛÀÌ¹Ç·Î ÀåÂøµÇÁö ¾Ê¾Ò´Ù.
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

				// ¾ÆÀÌÅÛ Á¤º¸ Àü¼Û 
				iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);
				switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					// ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù.
					DeleteClient(iClientH, TRUE, TRUE);
					break;
				}

			}
			else {
				// ´õÀÌ»ó ¾ÆÀÌÅÛÀ» º¸°üÇÒ ¼ö ¾ø´Â »óÅÂÀÌ´Ù.
				// ¾ÆÀÌÅÛÀ» ¼­ÀÖ´Â À§Ä¡¿¡ ¹ö¸°´Ù. 
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
					m_pClientList[iClientH]->m_sY, pItem);

				// ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¾ÆÀÌÅÛÀÌ ¶³¾îÁø °ÍÀ» ¾Ë¸°´Ù. 
				SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
					m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
					pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4

// ´õÀÌ»ó °¡Áú¼ö ¾ø´Ù´Â ¸Þ½ÃÁö¸¦ º¸³½´Ù.
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
					// ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù.
					DeleteClient(iClientH, TRUE, TRUE);
					break;
				}
			}
		}
		else {
			delete pItem;
			pItem = NULL;
		}
	}
}

BOOL CGame::_bDecodePotionConfigFileContents(char* pData, DWORD dwMsgSize)
{
	char* pContents, * token, cTxt[120];
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int  iPotionConfigListIndex = 0;
	int  iCraftingConfigListIndex = 0;
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
					// ���� ��ȣ 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! POTION configuration file error - Wrong Data format(1).");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					if (m_pPotionConfigList[atoi(token)] != NULL) {
						// �̹� �Ҵ�� ��ȣ�� �ִ�. �����̴�.
						PutLogList("(!!!) CRITICAL ERROR! POTION configuration file error - Duplicate potion number.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pPotionConfigList[atoi(token)] = new class CPotion;
					iPotionConfigListIndex = atoi(token);

					cReadModeB = 2;
					break;

				case 2:
					// ���� �̸� 
					ZeroMemory(m_pPotionConfigList[iPotionConfigListIndex]->m_cName, sizeof(m_pPotionConfigList[iPotionConfigListIndex]->m_cName));
					memcpy(m_pPotionConfigList[iPotionConfigListIndex]->m_cName, token, strlen(token));
					cReadModeB = 3;
					break;

				default:
					// ���� ���� m_sArray[0~10]
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pPotionConfigList[iPotionConfigListIndex]->m_sArray[cReadModeB - 3] = atoi(token);
					cReadModeB++;
					break;

				case 14:
					// ������ m_sArray[11]
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pPotionConfigList[iPotionConfigListIndex]->m_sArray[11] = atoi(token);
					cReadModeB = 15;
					break;

				case 15:
					// ��ų ����ġ 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pPotionConfigList[iPotionConfigListIndex]->m_iSkillLimit = atoi(token);
					cReadModeB = 16;
					break;

				case 16:
					// ���̵�
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! MAGIC configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pPotionConfigList[iPotionConfigListIndex]->m_iDifficulty = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;
				}
				break;

			case 2: // Crafting
				switch (cReadModeB) {
				case 1:	// 
					if (_bGetIsStringIsNumber(token) == FALSE)
					{
						PutLogList("(!!!) CRITICAL ERROR! CRAFTING configuration file error - Wrong Data format(1).");
						delete[] pContents; delete pStrTok; return FALSE;
					}
					if (m_pCraftingConfigList[atoi(token)] != NULL)
					{
						PutLogList("(!!!) CRITICAL ERROR! CRAFTING configuration file error - Duplicate crafting number.");
						delete[] pContents; delete pStrTok; return FALSE;
					}
					m_pCraftingConfigList[atoi(token)] = new class CPotion;
					iCraftingConfigListIndex = atoi(token);
					cReadModeB = 2;
					break;

				case 2:
					ZeroMemory(m_pCraftingConfigList[iCraftingConfigListIndex]->m_cName, sizeof(m_pCraftingConfigList[iCraftingConfigListIndex]->m_cName));
					memcpy(m_pCraftingConfigList[iCraftingConfigListIndex]->m_cName, token, strlen(token));
					cReadModeB = 3;
					break;

				default: // m_sArray[0~10]
					if (_bGetIsStringIsNumber(token) == FALSE)
					{
						PutLogList("(!!!) CRITICAL ERROR! CRAFTING configuration file error - Wrong Data format.");
						delete[] pContents; delete pStrTok; return FALSE;
					}
					m_pCraftingConfigList[iCraftingConfigListIndex]->m_sArray[cReadModeB - 3] = atoi(token);
					cReadModeB++;
					break;

				case 14: // m_sArray[11]
					if (_bGetIsStringIsNumber(token) == FALSE)
					{
						PutLogList("(!!!) CRITICAL ERROR! CRAFTING configuration file error - Wrong Data format.");
						delete[] pContents; delete pStrTok; return FALSE;
					}
					m_pCraftingConfigList[iCraftingConfigListIndex]->m_sArray[11] = atoi(token);
					cReadModeB = 15;
					break;

				case 15: // 
					if (_bGetIsStringIsNumber(token) == FALSE)
					{
						PutLogList("(!!!) CRITICAL ERROR! CRAFTING configuration file error - Wrong Data format.");
						delete[] pContents; delete pStrTok; return FALSE;
					}
					m_pCraftingConfigList[iCraftingConfigListIndex]->m_iSkillLimit = atoi(token);
					cReadModeB = 16;
					break;

				case 16:// 
					if (_bGetIsStringIsNumber(token) == FALSE)
					{
						PutLogList("(!!!) CRITICAL ERROR! CRAFTING configuration file error - Wrong Data format.");
						delete[] pContents; delete pStrTok; return FALSE;
					}
					m_pCraftingConfigList[iCraftingConfigListIndex]->m_iDifficulty = atoi(token);
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
			if (memcmp(token, "potion", 5) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
			}

			if (memcmp(token, "crafting", 8) == 0)
			{
				cReadModeA = 2;
				cReadModeB = 1;
			}

		}
		token = pStrTok->pGet();
	}

	delete pStrTok;
	delete[] pContents;

	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) CRITICAL ERROR! POTION configuration file contents error!");
		return FALSE;
	}

	wsprintf(cTxt, "(!) POTION(Total:%d) configuration - success!", iPotionConfigListIndex);
	PutLogList(cTxt);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
///		Crafting functions copyed on Alchemy functions...
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CGame::ReqCreateCraftingHandler(int iClientH, char* pData)
{
	DWORD* dwp;
	WORD* wp;
	char* cp, cI[6], cCraftingName[21], cData[120];
	int    iRet, i, j, iEraseReq, iRiskLevel, iDifficulty, iNeededContrib = 0;
	short* sp, sTemp;
	short  sItemIndex[6], sItemPurity[6], sItemNumber[6], sItemArray[12];
	BOOL   bDup, bFlag, bNeedLog;
	class  CItem* pItem;

	if (m_pClientList[iClientH] == NULL) return;

	m_pClientList[iClientH]->m_iSkillMsgRecvCount++;

	for (i = 0; i < 6; i++)
	{
		cI[i] = -1;
		sItemIndex[i] = -1;
		sItemNumber[i] = 0;
		sItemPurity[i] = -1;
	}
	cp = (char*)(pData + 11);
	cp += 20;
	cI[0] = *cp;
	cp++;
	cI[1] = *cp;
	cp++;
	cI[2] = *cp;
	cp++;
	cI[3] = *cp;
	cp++;
	cI[4] = *cp;
	cp++;
	cI[5] = *cp;
	cp++;

	for (i = 0; i < 6; i++)
	{
		if (cI[i] >= DEF_MAXITEMS) return;
		if ((cI[i] >= 0) && (m_pClientList[iClientH]->m_pItemList[cI[i]] == NULL)) return;
	}

	for (i = 0; i < 6; i++)
		if (cI[i] >= 0)
		{
			bDup = FALSE;
			for (j = 0; j < 6; j++)
				if (sItemIndex[j] == cI[i])
				{
					sItemNumber[j]++;
					bDup = TRUE;
				}
			if (bDup == FALSE)
			{
				for (j = 0; j < 6; j++)
					if (sItemIndex[j] == -1)
					{
						sItemIndex[j] = cI[i];
						sItemNumber[j]++;
						goto RCPH_LOOPBREAK;
					}
			RCPH_LOOPBREAK:;
			}
		}


	for (i = 0; i < 6; i++)
		if (sItemIndex[i] != -1)
		{
			if (sItemIndex[i] < 0) return;
			if ((sItemIndex[i] >= 0) && (sItemIndex[i] >= DEF_MAXITEMS)) return;
			if (m_pClientList[iClientH]->m_pItemList[sItemIndex[i]] == NULL) return;
			if (m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_dwCount < sItemNumber[i]) return;
			sItemPurity[i] = m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_sItemSpecEffectValue2;
			if ((m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_NONE)
				&& (m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_sSprite == 6)
				&& (m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_sSpriteFrame == 129))
			{
				sItemPurity[i] = 100; // Merien stones considered 100% purity.
			}
			if (m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME)
			{
				sItemPurity[i] = -1; // Diamonds / Emeralds.etc.. never have purity
			}
			if (sItemNumber[i] > 1) // No purity for stacked items
			{
				sItemPurity[i] = -1;
			}


			if ((m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_EQUIP)
				&& (m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_cEquipPos == DEF_EQUIPPOS_NECK))
			{
				iNeededContrib = 10; // Necks Crafting requires 10 contrib
			}
		}

	// Bubble Sort
	bFlag = TRUE;
	while (bFlag == TRUE)
	{
		bFlag = FALSE;
		for (i = 0; i < 5; i++)
			if ((sItemIndex[i] != -1) && (sItemIndex[i + 1] != -1))
			{
				if ((m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_sIDnum) < (m_pClientList[iClientH]->m_pItemList[sItemIndex[i + 1]]->m_sIDnum))
				{
					sTemp = sItemIndex[i + 1];
					sItemIndex[i + 1] = sItemIndex[i];
					sItemIndex[i] = sTemp;
					sTemp = sItemPurity[i + 1];
					sItemPurity[i + 1] = sItemPurity[i];
					sItemPurity[i] = sTemp;
					sTemp = sItemNumber[i + 1];
					sItemNumber[i + 1] = sItemNumber[i];
					sItemNumber[i] = sTemp;
					bFlag = TRUE;
				}
			}
	}
	j = 0;
	for (i = 0; i < 6; i++)
	{
		if (sItemIndex[i] != -1)
			sItemArray[j] = m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_sIDnum;
		else sItemArray[j] = sItemIndex[i];
		sItemArray[j + 1] = sItemNumber[i];
		j += 2;
	}

	// Search Crafting you wanna build
	ZeroMemory(cCraftingName, sizeof(cCraftingName));
	for (i = 0; i < DEF_MAXPOTIONTYPES; i++) {
		if (m_pCraftingConfigList[i] != NULL)
		{
			bFlag = FALSE;
			for (j = 0; j < 12; j++)
			{
				if (m_pCraftingConfigList[i]->m_sArray[j] != sItemArray[j]) bFlag = TRUE; // one item mismatch	
			}
			if (bFlag == FALSE) // good Crafting receipe
			{
				ZeroMemory(cCraftingName, sizeof(cCraftingName));
				memcpy(cCraftingName, m_pCraftingConfigList[i]->m_cName, 20);
				iRiskLevel = m_pCraftingConfigList[i]->m_iSkillLimit;			// % to loose item if crafting fails
				iDifficulty = m_pCraftingConfigList[i]->m_iDifficulty;
			}
		}
	}

	// Check if recipe is OK
	if (strlen(cCraftingName) == 0)
	{
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CRAFTING_FAIL, 1, NULL, NULL, NULL); // "There is not enough material"
		return;
	}
	// Check for Contribution
	if (m_pClientList[iClientH]->m_iContribution < iNeededContrib)
	{
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CRAFTING_FAIL, 2, NULL, NULL, NULL); // "There is not enough Contribution Point"	
		return;
	}
	// Check possible Failure
	if (iDice(1, 100) < iDifficulty)
	{
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CRAFTING_FAIL, 3, NULL, NULL, NULL); // "Crafting failed"
	// Remove parts...
		pItem = NULL;
		pItem = new class CItem;
		if (pItem == NULL) return;
		for (i = 0; i < 6; i++) {
			if (sItemIndex[i] != -1)
			{	// Deplete any Merien Stone
				if ((m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_NONE)
					&& (m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_sSprite == 6)
					&& (m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_sSpriteFrame == 129))
				{
					ItemDepleteHandler(iClientH, sItemIndex[i], FALSE, TRUE);
				}
				else {
					// Risk to deplete any other items (not stackable ones) // DEF_ITEMTYPE_CONSUME
					if ((m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_EQUIP)
						|| (m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_MATERIAL))
					{
						if (iDice(1, 100) < iRiskLevel)
						{
							ItemDepleteHandler(iClientH, sItemIndex[i], FALSE, TRUE);
						}
					}
				}
			}
		}
		return;
	}

	// Purity
	int iPurity, iTot = 0, iCount = 0;
	for (i = 0; i < 6; i++)
	{
		if (sItemIndex[i] != -1)
		{
			if (sItemPurity[i] != -1)
			{
				iTot += sItemPurity[i];
				iCount++;
			}
		}
	}
	if (iCount == 0)
	{
		iPurity = 20 + iDice(1, 80);			// Wares have random purity (20%..100%)
		bNeedLog = FALSE;
	}
	else
	{
		iPurity = iTot / iCount;
		iTot = (iPurity * 4) / 5;
		iCount = iPurity - iTot;
		iPurity = iTot + iDice(1, iCount);	// Jewel completion depends off Wares purity
		bNeedLog = TRUE;
	}
	if (iNeededContrib != 0)
	{
		iPurity = 0;						// Necks require contribution but no purity/completion
		bNeedLog = TRUE;
	}

	CalculateSSN_SkillIndex(iClientH, 18, 1);

	if (strlen(cCraftingName) != 0)
	{
		pItem = NULL;
		pItem = new class CItem;
		if (pItem == NULL) return;
		for (i = 0; i < 6; i++)
		{
			if (sItemIndex[i] != -1)
			{
				if (m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME)
				{
					SetItemCount(iClientH, sItemIndex[i],
						m_pClientList[iClientH]->m_pItemList[sItemIndex[i]]->m_dwCount - sItemNumber[i]);
				}
				else // So if item is not Type 5 (stackable items), you deplete item
				{
					ItemDepleteHandler(iClientH, sItemIndex[i], FALSE, TRUE);
				}
			}
		}
		if (iNeededContrib != 0)
		{
			m_pClientList[iClientH]->m_iContribution -= iNeededContrib;
			// No known msg to send info to client, so client will compute shown Contrib himself.		
		}

		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CRAFTING_SUCCESS, NULL, NULL, NULL, NULL);

		GetExp(iClientH, iDice(1, iDice(2, 100)));

		if ((_bInitItemAttr(pItem, cCraftingName) == TRUE))
		{	// // Snoopy: Added Purity to Oils/Elixirs
			if (iPurity != 0)
			{
				pItem->m_sItemSpecEffectValue2 = iPurity;
				pItem->m_dwAttribute = 1;
			}
			pItem->m_sTouchEffectType = DEF_ITET_ID;
			pItem->m_sTouchEffectValue1 = iDice(1, 100000);
			pItem->m_sTouchEffectValue2 = iDice(1, 100000);
			pItem->m_sItemSpecEffectValue1 = 300; // centu - Fix Endurance Crafting by Rambox
			SYSTEMTIME SysTime;
			char cTemp[256];
			GetLocalTime(&SysTime);
			ZeroMemory(cTemp, sizeof(cTemp));
			wsprintf(cTemp, "%d%2d", (short)SysTime.wMonth, (short)SysTime.wDay);
			pItem->m_sTouchEffectValue3 = atoi(cTemp);

			// SNOOPY log anything above WAREs
			if (bNeedLog)
			{
				wsprintf(G_cTxt, "PC(%s) Crafting (%s) Purity(%d)"
					, m_pClientList[iClientH]->m_cCharName
					, pItem->m_cName
					, pItem->m_sItemSpecEffectValue2);
				PutLogList(G_cTxt);
			}
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
					pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor);

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
}