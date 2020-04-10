// Mineral.cpp: implementation of the CMineral class.
//
//////////////////////////////////////////////////////////////////////

#include "Mineral.h"

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

CMineral::CMineral(char cType, char cMapIndex, int sX, int sY, int iRemain)
{
	m_cType = cType;
	m_cMapIndex = cMapIndex;
	m_sX = sX;
	m_sY = sY;;
    m_iRemain = iRemain;
	m_iDifficulty = 0;
}

CMineral::~CMineral()
{

}

BOOL CGame::_bDecodeBuildItemConfigFileContents(char* pData, DWORD dwMsgSize)
{
	char* pContents, * token, cTxt[120];
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int  i, iIndex = 0;
	class CStrTok* pStrTok;
	class CItem* pItem;

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
					// Â¾Ã†Ã€ÃŒÃ…Ã› Ã€ÃŒÂ¸Â§ 
					ZeroMemory(m_pBuildItemList[iIndex]->m_cName, sizeof(m_pBuildItemList[iIndex]->m_cName));
					memcpy(m_pBuildItemList[iIndex]->m_cName, token, strlen(token));

					cReadModeB = 2;
					break;

				case 2:
					// Â½ÂºÃ…Â³ ÃÂ¦Ã‡Ã‘Ã„Â¡ 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format(1).");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					m_pBuildItemList[iIndex]->m_iSkillLimit = atoi(token);
					cReadModeB = 3;
					break;

				case 3:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemID[0] = atoi(token);
					cReadModeB = 4;
					break;

				case 4:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemCount[0] = atoi(token);
					cReadModeB = 5;
					break;

				case 5:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemValue[0] = atoi(token);
					cReadModeB = 6;
					break;

				case 6:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemID[1] = atoi(token);
					cReadModeB = 7;
					break;

				case 7:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemCount[1] = atoi(token);
					cReadModeB = 8;
					break;

				case 8:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemValue[1] = atoi(token);
					cReadModeB = 9;
					break;

				case 9:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemID[2] = atoi(token);
					cReadModeB = 10;
					break;

				case 10:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemCount[2] = atoi(token);
					cReadModeB = 11;
					break;

				case 11:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemValue[2] = atoi(token);
					cReadModeB = 12;
					break;


				case 12:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemID[3] = atoi(token);
					cReadModeB = 13;
					break;

				case 13:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemCount[3] = atoi(token);
					cReadModeB = 14;
					break;

				case 14:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemValue[3] = atoi(token);
					cReadModeB = 15;
					break;

				case 15:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemID[4] = atoi(token);
					cReadModeB = 16;
					break;

				case 16:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pBuildItemList[iIndex]->m_iMaterialItemCount[4] = atoi(token);

					cReadModeB = 17;
					break;

				case 17:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemValue[4] = atoi(token);

					cReadModeB = 18;
					break;


				case 18:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemID[5] = atoi(token);
					cReadModeB = 19;
					break;

				case 19:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pBuildItemList[iIndex]->m_iMaterialItemCount[5] = atoi(token);

					cReadModeB = 20;
					break;

				case 20:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					m_pBuildItemList[iIndex]->m_iMaterialItemValue[5] = atoi(token);

					cReadModeB = 21;
					break;



				case 21:
					// m_iAverageValue
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					m_pBuildItemList[iIndex]->m_iAverageValue = atoi(token);

					cReadModeB = 22;
					break;

				case 22:
					// m_iMaxSkill
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					m_pBuildItemList[iIndex]->m_iMaxSkill = atoi(token);

					cReadModeB = 23;
					break;

				case 23:
					// m_wAttribute
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					m_pBuildItemList[iIndex]->m_wAttribute = atoi(token);

					cReadModeA = 0;
					cReadModeB = 0;

					pItem = new class CItem;
					if (_bInitItemAttr(pItem, m_pBuildItemList[iIndex]->m_cName) == TRUE) {
						// Â¾Ã†Ã€ÃŒÃ…Ã›Ã€Ã‡ ÃÂ¸Ã€Ã§Â°Â¡ ÃˆÂ®Ã€ÃŽÂµÃ‡Â¾ÃºÂ´Ã™.
						m_pBuildItemList[iIndex]->m_sItemID = pItem->m_sIDnum;

						// ÃƒÃ–Â´Ã« Â°Â¡ÃÃŸÃ„Â¡Â°Âª Â°Ã¨Â»Ãª
						for (i = 0; i < 6; i++)
							m_pBuildItemList[iIndex]->m_iMaxValue += (m_pBuildItemList[iIndex]->m_iMaterialItemValue[i] * 100);

						iIndex++;
					}
					else {
						// Ã€ÃŒÂ·Â± Ã€ÃŒÂ¸Â§Ã€Â» Â°Â¡ÃÃ¸ Â¾Ã†Ã€ÃŒÃ…Ã›Ã€ÃŒ ÃÂ¸Ã€Ã§Ã‡ÃÃÃ¶ Â¾ÃŠÂ´Ã‚Â´Ã™. Â¿Â¡Â·Â¯ 
						wsprintf(G_cTxt, "(!!!) CRITICAL ERROR! BuildItem configuration file error - Not Existing Item(%s)", m_pBuildItemList[iIndex]->m_cName);
						PutLogList(G_cTxt);

						delete m_pBuildItemList[iIndex];
						m_pBuildItemList[iIndex] = NULL;

						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					delete pItem;
					break;
				}
				break;

			default:
				break;
			}
		}
		else {
			if (memcmp(token, "BuildItem", 9) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
				m_pBuildItemList[iIndex] = new class CBuildItem;
			}

		}
		token = pStrTok->pGet();
	}

	delete pStrTok;
	delete[] pContents;

	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) CRITICAL ERROR! BuildItem configuration file contents error!");
		return FALSE;
	}

	wsprintf(cTxt, "(!) BuildItem(Total:%d) configuration - success!", iIndex);
	PutLogList(cTxt);

	return TRUE;
}

// Centu : agregados items con atributos
void CGame::BuildItemHandler(int iClientH, char* pData)
{
	char* cp, cName[21], cElementItemID[6], cColor;
	int    i, x, z, iMatch, iCount, iPlayerSkillLevel, iResult, iTotalValue, iResultValue, iTemp, iItemCount[DEF_MAXITEMS];
	class  CItem* pItem;
	BOOL   bFlag, bItemFlag[6];
	double dV1, dV2, dV3;
	DWORD  dwTemp, dwTemp2, dwType, dwValue;
	WORD   wTemp;
	short sMagicLevel = 0;

	// »ç¿ëÀÚ°¡ ¾ÆÀÌÅÛ Á¦ÀÛÀ» ½ÅÃ»Çß´Ù.
	if (m_pClientList[iClientH] == NULL) return;
	m_pClientList[iClientH]->m_iSkillMsgRecvCount++;

	cp = (char*)(pData + 11);
	ZeroMemory(cName, sizeof(cName));
	memcpy(cName, cp, 20);
	cp += 20;

	ZeroMemory(cElementItemID, sizeof(cElementItemID));
	cElementItemID[0] = *cp;
	cp++;
	cElementItemID[1] = *cp;
	cp++;
	cElementItemID[2] = *cp;
	cp++;
	cElementItemID[3] = *cp;
	cp++;
	cElementItemID[4] = *cp;
	cp++;
	cElementItemID[5] = *cp;
	cp++;

	// ¾ÕºÎºÐÀÇ ºó°ø°£À» ¾ø¾Ø´Ù.
	bFlag = TRUE;
	while (bFlag == TRUE) {
		bFlag = FALSE;
		for (i = 0; i <= 4; i++)
			if ((cElementItemID[i] == -1) && (cElementItemID[i + 1] != -1)) {
				cElementItemID[i] = cElementItemID[i + 1];
				cElementItemID[i + 1] = -1;
				bFlag = TRUE;
			}
	}

	for (i = 0; i < 6; i++) bItemFlag[i] = FALSE;

	iPlayerSkillLevel = m_pClientList[iClientH]->m_cSkillMastery[13];
	iResult = iDice(1, 100);

	if (iResult > iPlayerSkillLevel) {
		// ½ÇÆÐ´Ù.
		// ¾ÆÀÌÅÛ Á¦ÀÛ¿¡ ½ÇÆÐÇÏ¿´´Ù.
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_BUILDITEMFAIL, NULL, NULL, NULL, NULL);
		return;
	}

	// °¢°¢ÀÇ ¾ÆÀÌÅÛÀÌ ´Ù Á¸ÀçÇÏ´ÂÁö °Ë»ç.
	for (i = 0; i < 6; i++)
		if (cElementItemID[i] != -1) {
			// Àß¸øµÈ Item ID°ªÀÌ´Ù. ¹«½Ã
			if ((cElementItemID[i] < 0) || (cElementItemID[i] > DEF_MAXITEMS)) return;
			if (m_pClientList[iClientH]->m_pItemList[cElementItemID[i]] == NULL) return;
		}

	// ÇØ´ç ÀÌ¸§À» °¡Áø ¾ÆÀÌÅÛÀ» ºôµå ¾ÆÀÌÅÛ ¸®½ºÆ®¿¡¼* Ã£´Â´Ù.
	for (i = 0; i < DEF_MAXBUILDITEMS; i++)
		if (m_pBuildItemList[i] != NULL) {
			if (memcmp(m_pBuildItemList[i]->m_cName, cName, 20) == 0) {
				// ¾ÆÀÌÅÛÀ» Ã£¾Ò´Ù. Á¦ÀÛ Á¶°Ç°ú ¾ÆÀÌÅÛÀÌ ÀÏÄ¡ÇÏ´ÂÁö °Ë»ç.

				// ½ºÅ³ÀÌ ³·¾Æ¼* Á¦ÀÛ ºÒ°¡´É. ¿ø·¡ Å¬¶óÀÌ¾ðÆ®¿¡¼* °É·¯ Áø´Ù.
				if (m_pBuildItemList[i]->m_iSkillLimit > m_pClientList[iClientH]->m_cSkillMastery[13]) return;

				for (x = 0; x < DEF_MAXITEMS; x++)
					if (m_pClientList[iClientH]->m_pItemList[x] != NULL)
						iItemCount[x] = m_pClientList[iClientH]->m_pItemList[x]->m_dwCount;
					else iItemCount[x] = 0;

				// ÇØ´ç ºôµå ¾ÆÀÌÅÛÀÇ Àç·á°¡ ÃæºÐÇÑÁö¸¦ °Ë»çÇÑ´Ù. ´õºÒ¾î ºôµå ¾ÆÀÌÅÛÀÇ °¡ÁßÄ¡ °ªµµ °è»ê.
				iMatch = 0;
				iTotalValue = 0;

				for (x = 0; x < 6; x++) {
					if (m_pBuildItemList[i]->m_iMaterialItemCount[x] == 0) {
						iMatch++;
					}
					else {
						for (z = 0; z < 6; z++)
							if ((cElementItemID[z] != -1) && (bItemFlag[z] == FALSE)) {

								if ((m_pClientList[iClientH]->m_pItemList[cElementItemID[z]]->m_sIDnum == m_pBuildItemList[i]->m_iMaterialItemID[x]) &&
									(m_pClientList[iClientH]->m_pItemList[cElementItemID[z]]->m_dwCount >= m_pBuildItemList[i]->m_iMaterialItemCount[x]) &&
									(iItemCount[cElementItemID[z]] > 0)) {
									// Àç·áÁß ÇÏ³ª¸¦ Ã£¾Ò´Ù.
									iTemp = m_pClientList[iClientH]->m_pItemList[cElementItemID[z]]->m_sItemSpecEffectValue2;
									if (iTemp > m_pClientList[iClientH]->m_cSkillMastery[13]) {
										// Àç·áÀÇ ¼øµµ°¡ ³» ½ºÅ³ ¼öÁØÀ» »óÈ¸ÇÑ´Ù¸é ¼øµµ¸¦ ³·Ãá´Ù.
										iTemp = iTemp - (iTemp - m_pClientList[iClientH]->m_cSkillMastery[13]) / 2;
									}

									iTotalValue += (iTemp * m_pBuildItemList[i]->m_iMaterialItemValue[x]);
									iItemCount[cElementItemID[z]] -= m_pBuildItemList[i]->m_iMaterialItemCount[x];
									iMatch++;
									bItemFlag[z] = TRUE;

									goto BIH_LOOPBREAK;
								}
							}
					BIH_LOOPBREAK:;
					}
				}

				// ¿©±â¼* iMatch°¡ 6ÀÌ¸é ¾ÆÀÌÅÛ Á¦ÀÛ Á¶°ÇÀÌ ¸¸Á·µÈ °ÍÀÌ´Ù.
				if (iMatch != 6) {
					// Àç·á ºÎÁ·. ¾ÆÀÌÅÛ Á¦ÀÛ ºÒ°¡
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_BUILDITEMFAIL, NULL, NULL, NULL, NULL);
					return;
				}

				// ¼øµµ °è»ê
				dV2 = (double)m_pBuildItemList[i]->m_iMaxValue;
				if (iTotalValue <= 0)
					dV3 = 1.0f;
				else dV3 = (double)iTotalValue;
				dV1 = (double)(dV3 / dV2) * 100.0f;

				// ¹éºÐ·ü·Î ¹Ù²ï °ª.
				iTotalValue = (int)dV1;

				// ¾ÆÀÌÅÛ »ý¼º
				pItem = new class CItem;
				if (_bInitItemAttr(pItem, m_pBuildItemList[i]->m_cName) == FALSE) {
					delete pItem;
					return;
				}

				// ¾ÆÀÌÅÛÀÌ Custom-MadeÀÓÀ» ³ªÅ¸³»´Â ÇÃ·¡±×¸¦ ÀÔ·Â
				dwTemp = pItem->m_dwAttribute;
				dwTemp = dwTemp & 0xFFFFFFFE;
				dwTemp = dwTemp | 0x00000001;
				pItem->m_dwAttribute = dwTemp;

				if (pItem->m_cItemType == DEF_ITEMTYPE_MATERIAL) {
					// ¸¸µé¾îÁø ¾ÆÀÌÅÛÀÌ ¹«±â Á¦ÀÛ Àç·á(À×°÷)ÀÌ¶ó¸é ±â¼ú ¼öÁØ¿¡ µû¸¥ ¼øµµ¸¦ ÀÔ·ÂÇÑ´Ù.
					iTemp = iDice(1, (iPlayerSkillLevel / 2) + 1) - 1;
					pItem->m_sItemSpecEffectValue2 = (iPlayerSkillLevel / 2) + iTemp;
					// v2.15 Á¦ÀÛ ¾ÆÀÌÅÛ¿¡ ¾ÆÀÌÅÛ °íÀ¯¹øÈ£ Ãß°¡
					pItem->m_sTouchEffectType = DEF_ITET_ID;
					pItem->m_sTouchEffectValue1 = iDice(1, 100000);
					pItem->m_sTouchEffectValue2 = iDice(1, 100000);
					pItem->m_sTouchEffectValue3 = timeGetTime();

				}
				else {
					// ¹«±â Á¦ÀÛ Àç·á°¡ ¾Æ´Ï°í ¹«±â³ª ¹æ¾î±¸·ù¶ó¸é
					// ¾ÆÀÌÅÛ ¼Ó¼º°ª ÀÔ·Â
					dwTemp = pItem->m_dwAttribute;
					dwTemp = dwTemp & 0x0000FFFF;

					dwTemp2 = (WORD)m_pBuildItemList[i]->m_wAttribute;
					dwTemp2 = dwTemp2 << 16;

					dwTemp = dwTemp | dwTemp2;
					pItem->m_dwAttribute = dwTemp;

					iResultValue = (iTotalValue - m_pBuildItemList[i]->m_iAverageValue);
					//Àç·á ¼øµµ¿¡ µû¸¥ ¼º´É ÀÔ·Â: SpecEffectValue1Àº ¼ö¸í, SpecEffectValue2´Â ¼º´É °¡ÁßÄ¡

					// 1. ¼º´É °¡ÁßÄ¡ °è»ê(¹éºÐÀ²)
					if (iResultValue > 0) {
						dV2 = (double)iResultValue;
						dV3 = (double)(100 - m_pBuildItemList[i]->m_iAverageValue);
						dV1 = (dV2 / dV3) * 100.0f;
						pItem->m_sItemSpecEffectValue2 = (int)dV1;
					}
					else if (iResultValue < 0) {
						dV2 = (double)(iResultValue);
						dV3 = (double)(m_pBuildItemList[i]->m_iAverageValue);
						dV1 = (dV2 / dV3) * 100.0f;
						pItem->m_sItemSpecEffectValue2 = (int)dV1;
					}
					else pItem->m_sItemSpecEffectValue2 = 0;

					// 2. »õ·Î¿î ÃÖ´ë ¼ö¸í °è»ê
					dV2 = (double)pItem->m_sItemSpecEffectValue2;
					dV3 = (double)pItem->m_wMaxLifeSpan;
					dV1 = (dV2 / 100.0f) * dV3;

					iTemp = (int)pItem->m_wMaxLifeSpan;
					iTemp += (int)dV1;

					// v2.15 Á¦ÀÛ ¾ÆÀÌÅÛ¿¡ ¾ÆÀÌÅÛ °íÀ¯¹øÈ£ Ãß°¡
					pItem->m_sTouchEffectType = DEF_ITET_ID;
					pItem->m_sTouchEffectValue1 = iDice(1, 100000);
					pItem->m_sTouchEffectValue2 = iDice(1, 100000);
					pItem->m_sTouchEffectValue3 = timeGetTime();

					if (iTemp <= 0)
						wTemp = 1;
					else wTemp = (WORD)iTemp;

					if (wTemp <= pItem->m_wMaxLifeSpan * 2) {
						// ¿¡·¯·Î ÀÎÇØ ³Ê¹« ¸¹Àº ¼ö¸í°ªÀÌ ³ª¿ÈÀ» ¸·±âÀ§ÇÔ
						pItem->m_wMaxLifeSpan = wTemp;
						pItem->m_sItemSpecEffectValue1 = (short)wTemp;
						pItem->m_wCurLifeSpan = pItem->m_wMaxLifeSpan;
					}
					else pItem->m_sItemSpecEffectValue1 = (short)pItem->m_wMaxLifeSpan;

					//Custom-ItemÀº »ö»óÀÌ 2¹ø OGEID.

					if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK) {
						// Â°Ã¸Â°Ã? Â¹Â«Â±Ã¢Â·Ã¹Â¿Â¡ ÂºÃ™Ã€Â» Â¼Ã¶ Ã€Ã–Â´Ã‚ Ã?Â¢ÂµÃŽÂ»Ã§Â¸Â¦ Â¼Â±Ã…Ãƒ
						// Â°Â¡ÂºÂ*Â¿Ã®(3%) Â°Â*ÃˆÂ*ÂµÃˆ(7%) Ã‡ÃŠÂ»Ã¬Ã€Ã‡(15%) Â¹ÃŽÃƒÂ¸Ã€Ã‡(20%) Ã?Â¤Ã€Ã‡Ã€Ã‡(20%) Ã?ÃŸÂµÂ¶Ã€Ã‡(16%) Â¿Â¹Â¸Â®Ã‡Ã‘(16%) Â°Ã*Â´Ã«Â¹Â®Â¸Ã*Ã€Ã‡(3%)
						iResult = iDice(1, 11500);
						if ((iResult >= 1) && (iResult <= 299)) {
							dwType = 10;
							cColor = 5;
						}
						else if ((iResult >= 300) && (iResult <= 999)) {
							dwType = 12;
							cColor = 5;
						}
						else if ((iResult >= 1000) && (iResult <= 2499)) {
							dwType = 7;
							cColor = 6;
						}
						else if ((iResult >= 2500) && (iResult <= 4499)) {
							dwType = 7;
							cColor = 6;
						}
						else if ((iResult >= 4500) && (iResult <= 6499)) {
							dwType = 7;
							cColor = 6;
						}
						else if ((iResult >= 6500) && (iResult <= 8099)) {
							dwType = 7;
							cColor = 6;
						}
						else if ((iResult >= 8100) && (iResult <= 9699)) {
							dwType = 9;
							cColor = 8;
						}
						else if ((iResult >= 9700) && (iResult <= 9999)) {
							dwType = 9;
							cColor = 8;
						}
						else if ((iResult >= 10000) && (iResult <= 11499)) {
							dwType = 15; // Magic
							cColor = 10; // Black Color
						}

						// Â¾Ã†Ã€ÃŒÃ…Ã› Â»Ã¶Â»Ã³ Ã€Ã”Â·Ã‚
						pItem->m_cItemColor = cColor;

						// Â°Â¡ÂºÂ*Â¿Ã®, Â°Â*ÃˆÂ*ÂµÃˆ, Ã‡ÃŠÂ»Ã¬Ã€Ã‡, Â¹ÃŽÃƒÂ¸Ã€Ã‡, Ã?Â¤Ã€Ã‡Ã€Ã‡, Ã?ÃŸÂµÂ¶Ã€Ã‡, Â¿Â¹Â¸Â®Ã‡Ã‘, Â°Ã*Â´Ã«Â¹Â®Â¸Ã*Ã€Ã‡
						// Â¾Ã†Ã€ÃŒÃ…Ã› Main Ã†Â¯Â¼ÂºÃ„Â¡ Ã?Â¤ÂµÂµÂ°Âª Ã€Ã”Â·Ã‚

						// Â¾Ã†Ã€ÃŒÃ…Ã› Sub Ã†Â¯Â¼ÂºÃ„Â¡ Ã?Â¤ÂµÂµÂ°Âª Ã€Ã”Â·Ã‚
						if (dwType == 15) {
							iResult = iDice(1, 30060);
							if ((iResult >= 1) && (iResult < 10000))           dwValue = 1;  // Fire-Strike
							else if ((iResult >= 10000) && (iResult < 17400))  dwValue = 2;  // Lightning
							else if ((iResult >= 17400) && (iResult < 22400))  dwValue = 3;  // Chill-Wind
							else if ((iResult >= 22400) && (iResult < 25400))  dwValue = 4;  // Ice-Strike
							else if ((iResult >= 25400) && (iResult < 27400))  dwValue = 5;  // Energy-Strike
							else if ((iResult >= 27400) && (iResult < 28400))  dwValue = 6;  // Mass-Fire-Strike
							else if ((iResult >= 28400) && (iResult < 28900))  dwValue = 7;  // Mass-Chill-Wind
							else if ((iResult >= 28900) && (iResult < 29300))  dwValue = 8;  // Earthworm-Strike
							else if ((iResult >= 29300) && (iResult < 29600))  dwValue = 9;  // Bloody-Shock-Wave
							else if ((iResult >= 29600) && (iResult < 29800))  dwValue = 10; // Mass-Ice-Strike
							else if ((iResult >= 29800) && (iResult < 29900))  dwValue = 11; // Lightning-Strike
							else if ((iResult >= 29900) && (iResult < 29970))  dwValue = 12; // Ice-Whirlwind
							else if ((iResult >= 29970) && (iResult < 30000))  dwValue = 13; // Meteor-Strike
							else if ((iResult >= 30000) && (iResult < 30025))  dwValue = 14; // Mass-Magic-Missile
							else if ((iResult >= 30025) && (iResult < 30045))  dwValue = 15; // Blizzard
							else if ((iResult >= 30045) && (iResult <= 30060))  dwValue = 16; // Earth-Shock-Wave
							sMagicLevel = 1;
						}
						else {
							iResult = iDice(1, 30000);
							if ((iResult >= 1) && (iResult < 10000))           dwValue = 1;  // 10000/29348 = 34%
							else if ((iResult >= 10000) && (iResult < 17400))  dwValue = 2;  // 6600/29348 = 22.4%
							else if ((iResult >= 17400) && (iResult < 22400))  dwValue = 3;  // 4356/29348 = 14.8%
							else if ((iResult >= 22400) && (iResult < 25400))  dwValue = 4;  // 2874/29348 = 9.7%
							else if ((iResult >= 25400) && (iResult < 27400))  dwValue = 5;  // 1897/29348 = 6.4%
							else if ((iResult >= 27400) && (iResult < 28400))  dwValue = 6;  // 1252/29348 = 4.2%
							else if ((iResult >= 28400) && (iResult < 28900))  dwValue = 7;  // 826/29348 = 2.8%
							else if ((iResult >= 28900) && (iResult < 29300))  dwValue = 8;  // 545/29348 = 1.85%
							else if ((iResult >= 29300) && (iResult < 29600))  dwValue = 9;  // 360/29348 = 1.2%
							else if ((iResult >= 29600) && (iResult < 29800))  dwValue = 10; // 237/29348 = 0.8%
							else if ((iResult >= 29800) && (iResult < 29900))  dwValue = 11; // 156/29348 = 0.5%
							else if ((iResult >= 29900) && (iResult < 29970))  dwValue = 12; // 103/29348 = 0.3%
							else if ((iResult >= 29970) && (iResult <= 30000))  dwValue = 13; // 68/29348 = 0.1%
							else dwValue = 1; // v2.03 906
							sMagicLevel = 0;
						}

						// Â¹Â«Â±Ã¢Ã€Ã‡ Main Ã†Â¯Â¼ÂºÃ„Â¡Â¿Â¡ ÂµÃ»Â¶Ã³ Â¼Ã¶Ã„Â¡ ÂºÂ¸Ã?Â¤
						switch (dwType) {
						case 1: // Ã‡ÃŠÂ»Ã¬ Ã…Â¸Â°Ã? ÃƒÃ–Ã€Ãº +5
							if (dwValue <= 5) dwValue = 5;
							break;
						case 2: // Ã?ÃŸÂµÂ¶ Ã…Â¸Â°Ã? ÃƒÃ–Ã€Ãº +20
							if (dwValue <= 4) dwValue = 4;
							break;
						case 6: // Â°Ã¦Â·Â®ÃˆÂ* ÃƒÃ–Ã€Ãº  +16%
							if (dwValue <= 4) dwValue = 4;
							break;
						case 8: // Â¼Ã¶Â¸Ã* ÃƒÃ–Ã€Ãº +14%               
							if (dwValue <= 2) dwValue = 2;
							break;

						}
						// v2.03 912 Â¾Ã†Ã€ÃŒÃ…Ã› Â¼Ã¶Ã?Ã˜Ã€ÃŒ 2Ã€ÃŒÃ‡Ã?Ã€Ã?Â¶Â§ Ã†Â¯Â¼ÂºÃ„Â¡ ÃƒÃ–Â´Ã«Â°ÂªÃ€Âº 7
						if (dwValue > 7) dwValue = 7;

						// Â¾Ã†Ã€ÃŒÃ…Ã› Main Ã†Â¯Â¼ÂºÃ„Â¡, Â°Âª Ã€Ã”Â·Ã‚
						pItem->m_dwAttribute = NULL;
						dwType = dwType << 20;
						dwValue = dwValue << 16;
						pItem->m_dwAttribute = pItem->m_dwAttribute | dwType | dwValue;
					}

					if (pItem->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE) {
						// Â¹Ã¦Â¾Ã®Â±Â¸Â¿Â¡ Â¸Ã‚Â´Ã‚ Ã?Â¢ÂµÃŽÂ»Ã§Â¿Ã? Ã†Â¯Â¼ÂºÃ„Â¡Â¸Â¦ Â¼Â±Ã…Ãƒ, Ã‡Ã’Â´Ã§Ã‡Ã‘Â´Ã™.

						// Â°Â*ÃˆÂ*ÂµÃˆ(60%) Â°Â¡ÂºÂ*Â¿Ã® (30%) Â¸Â¶Â³Âª ÂºÂ¯ÃˆÂ¯Ã€Ã‡(5%) Ã‡ÃŠÂ»Ã¬ ÃƒÃ¦Ã€Ã¼Ã€Ã‡(5%)
						iResult = iDice(1, 10000);//AQUI OGEID
						if ((iResult >= 1) && (iResult <= 5999))          dwType = 8;
						else if ((iResult >= 6000) && (iResult <= 8999))  dwType = 6;
						else if ((iResult >= 9000) && (iResult <= 9554))  dwType = 11; //dwType = 11;
						else if ((iResult >= 9555) && (iResult <= 10000)) dwType = 12; //dwType = 12;

						// Â¾Ã†Ã€ÃŒÃ…Ã› Sub Ã†Â¯Â¼ÂºÃ„Â¡ Ã?Â¤ÂµÂµÂ°Âª Ã€Ã”Â·Ã‚
						iResult = iDice(1, 30000);
						if ((iResult >= 1) && (iResult < 10000))           dwValue = 1;  // 10000/29348 = 34%
						else if ((iResult >= 10000) && (iResult < 17400))  dwValue = 1;  // 6600/29348 = 22.4%
						else if ((iResult >= 17400) && (iResult < 22400))  dwValue = 1;  // 4356/29348 = 14.8%
						else if ((iResult >= 22400) && (iResult < 25400))  dwValue = 2;  // 2874/29348 = 9.7%
						else if ((iResult >= 25400) && (iResult < 27400))  dwValue = 2;  // 1897/29348 = 6.4%
						else if ((iResult >= 27400) && (iResult < 28400))  dwValue = 2;  // 1252/29348 = 4.2%
						else if ((iResult >= 28400) && (iResult < 28900))  dwValue = 4;  // 826/29348 = 2.8%
						else if ((iResult >= 28900) && (iResult < 29300))  dwValue = 4;  // 545/29348 = 1.85%
						else if ((iResult >= 29300) && (iResult < 29600))  dwValue = 4;  // 360/29348 = 1.2%
						else if ((iResult >= 29600) && (iResult < 29800))  dwValue = 6; // 237/29348 = 0.8%
						else if ((iResult >= 29800) && (iResult < 29900))  dwValue = 6; // 156/29348 = 0.5%
						else if ((iResult >= 29900) && (iResult < 29970))  dwValue = 10; // 103/29348 = 0.3%
						else if ((iResult >= 29970) && (iResult <= 30000))  dwValue = 10; // 68/29348 = 0.1%
						else dwValue = 1; // v2.03 906

						// Â¹Ã¦Â¾Ã®Â±Â¸Ã€Ã‡ Main Ã†Â¯Â¼ÂºÃ„Â¡Â¿Â¡ ÂµÃ»Â¶Ã³ Â¼Ã¶Ã„Â¡ ÂºÂ¸Ã?Â¤
						switch (dwType) {
						case 6: // Â°Ã¦Â·Â®ÃˆÂ* ÃƒÃ–Ã€Ãº +16%
							if (dwValue <= 4) dwValue = 4;
							break;
						case 8: // Â¼Ã¶Â¸Ã* ÃƒÃ–Ã€Ãº +14%
							if (dwValue <= 2) dwValue = 2;
							break;

						case 11:
						case 12:
							// v2.04
							dwValue = (dwValue + 1) / 2;
							if (dwValue < 1) dwValue = 1;
							if (dwValue > 2) dwValue = 2;
							break;
						}
						// v2.03 912 Â¾Ã†Ã€ÃŒÃ…Ã› Â¼Ã¶Ã?Ã˜Ã€ÃŒ 2Ã€ÃŒÃ‡Ã?Ã€Ã?Â¶Â§ Ã†Â¯Â¼ÂºÃ„Â¡ ÃƒÃ–Â´Ã«Â°ÂªÃ€Âº 7
						if (dwValue > 7) dwValue = 7;

						// Â¹Ã¦Â¾Ã®Â±Â¸ Main Ã†Â¯Â¼ÂºÃ„Â¡, Â°Âª Ã€Ã”Â·Ã‚
						pItem->m_dwAttribute = NULL;
						dwType = dwType << 20;
						dwValue = dwValue << 16;
						pItem->m_dwAttribute = pItem->m_dwAttribute | dwType | dwValue;

						// Â¹Ã¦Â¾Ã®Â±Â¸ Sub Ã†Â¯Â¼ÂºÃ„Â¡Â°Â¡ Ã€Ã”Â·Ã‚ÂµÃ‰ ÃˆÂ®Â·Ã¼Ã€Âº 40%
						if (iDice(1, 10000) <= m_iRareDropRate) {

							// ÃˆÃ±Â±Ã? Â¾Ã†Ã€ÃŒÃ…Ã› Sub Ã†Â¯Â¼ÂºÃ„Â¡ ÃˆÂ¿Â°Ãº Ã?Â¾Â·Ã¹:
							//ÃƒÃŸÂ°Â¡ ÂµÂ¶Â¼ÂºÃ€ÃºÃ‡Ã—(1), ÃƒÃŸÂ°Â¡ Â¸Ã*Ã?ÃŸÂ°Âª(2), ÃƒÃŸÂ°Â¡ Â¹Ã¦Â¾Ã®Â°Âª(3), HP ÃˆÂ¸ÂºÂ¹Â·Â® ÃƒÃŸÂ°Â¡(4), SP ÃˆÂ¸ÂºÂ¹Â·Â® ÃƒÃŸÂ°Â¡(5)
							//MP ÃˆÂ¸ÂºÂ¹Â·Â® ÃƒÃŸÂ°Â¡(6), ÃƒÃŸÂ°Â¡ Â¸Â¶Â¹Ã½Ã€ÃºÃ‡Ã—(7), Â¹Â°Â¸Â® Â´Ã«Â¹ÃŒÃ?Ã¶ ÃˆÃ*Â¼Ã¶(8), Â¸Â¶Â¹Ã½ Â´Ã«Â¹ÃŒÃ?Ã¶ ÃˆÃ*Â¼Ã¶(9)
							//Â¿Â¬Ã…Â¸ Â´Ã«Â¹ÃŒÃ?Ã¶ ÃƒÃŸÂ°Â¡(10), Â´Ãµ Â¸Â¹Ã€Âº Â°Ã¦Ã‡Ã¨Ã„Â¡(11), Â´ÃµÂ¸Â¹Ã€Âº Gold(12)

						   // Â¹Ã¦Â¾Ã®Â±Â¸ Ã€ÃŒÂ±Ã¢ Â¶Â§Â¹Â®Â¿Â¡
							// ÃƒÃŸÂ°Â¡Â¹Ã¦Â¾Ã®Â°Âª(10%) ÃƒÃŸÂ°Â¡ÂµÂ¶Â¼ÂºÃ€ÃºÃ‡Ã—(30%)    SPÃˆÂ¸ÂºÂ¹ÃƒÃŸÂ°Â¡(15%)  HPÃˆÂ¸ÂºÂ¹ÃƒÃŸÂ°Â¡(10%)
							// MPÃˆÂ¸ÂºÂ¹ ÃƒÃŸÂ°Â¡(10%) ÃƒÃŸÂ°Â¡Â¸Â¶Â¹Ã½Ã€ÃºÃ‡Ã—(15%) Â¹Â°Â¸Â®Â´Ã«Â¹ÃŒÃ?Ã¶ÃˆÃ*Â¼Ã¶(3%) Â¸Â¶Â¹Ã½Â´Ã«Â¹ÃŒÃ?Ã¶ÃˆÃ*Â¼Ã¶(3%)
							iResult = iDice(1, 10000);
							if ((iResult >= 1) && (iResult <= 999)) {
								dwType = 8;
								cColor = 15;
							}
							else if ((iResult >= 1000) && (iResult <= 3999)) {
								dwType = 8;
								cColor = 15;
							}
							else if ((iResult >= 4000) && (iResult <= 5499)) {
								dwType = 8;
								cColor = 15;
							}
							else if ((iResult >= 5500) && (iResult <= 6499)) {
								dwType = 9;
								cColor = 10;
							}
							else if ((iResult >= 6500) && (iResult <= 7499)) {
								dwType = 9;
								cColor = 10;
							}
							else if ((iResult >= 7500) && (iResult <= 9399)) {
								dwType = 9;
								cColor = 10;
							}
							else if ((iResult >= 9400) && (iResult <= 9799)) {
								dwType = 8;
								cColor = 15;
							}
							else if ((iResult >= 9800) && (iResult <= 10000)) {
								dwType = 9;
								cColor = 10;
							}

							pItem->m_cItemColor = cColor;

							// Â¾Ã†Ã€ÃŒÃ…Ã› Sub Ã†Â¯Â¼ÂºÃ„Â¡ Ã?Â¤ÂµÂµÂ°Âª Ã€Ã”Â·Ã‚
							iResult = iDice(1, 30000);
							if ((iResult >= 1) && (iResult < 10000))           dwValue = 1;  // 10000/29348 = 34%
							else if ((iResult >= 10000) && (iResult < 17400))  dwValue = 1;  // 6600/29348 = 22.4%
							else if ((iResult >= 17400) && (iResult < 22400))  dwValue = 1;  // 4356/29348 = 14.8%
							else if ((iResult >= 22400) && (iResult < 25400))  dwValue = 2;  // 2874/29348 = 9.7%
							else if ((iResult >= 25400) && (iResult < 27400))  dwValue = 2;  // 1897/29348 = 6.4%
							else if ((iResult >= 27400) && (iResult < 28400))  dwValue = 2;  // 1252/29348 = 4.2%
							else if ((iResult >= 28400) && (iResult < 28900))  dwValue = 2;  // 826/29348 = 2.8%
							else if ((iResult >= 28900) && (iResult < 29300))  dwValue = 4;  // 545/29348 = 1.85%
							else if ((iResult >= 29300) && (iResult < 29600))  dwValue = 6;  // 360/29348 = 1.2%
							else if ((iResult >= 29600) && (iResult < 29800))  dwValue = 6; // 237/29348 = 0.8%
							else if ((iResult >= 29800) && (iResult < 29900))  dwValue = 8; // 156/29348 = 0.5%
							else if ((iResult >= 29900) && (iResult < 29970))  dwValue = 8; // 103/29348 = 0.3%
							else if ((iResult >= 29970) && (iResult <= 30000))  dwValue = 8; // 68/29348 = 0.1%
							else dwValue = 1; // v2.03 906

							// Â¹Ã¦Â¾Ã®Â±Â¸Ã€Ã‡ Sub Ã†Â¯Â¼ÂºÃ„Â¡Â¿Â¡ ÂµÃ»Â¶Ã³ Â¼Ã¶Ã„Â¡ ÂºÂ¸Ã?Â¤
							switch (dwType) {
							case 1: // ÂµÂ¶Â¼ÂºÃ€ÃºÃ‡Ã— ÃƒÃ–Ã€Ãº +21%
							case 3: // Â¹Â°Â¸Â®Â¹Ã¦Â¾Ã® ÃƒÃ–Ã€Ãº +21%
							case 7: // Â¸Â¶Â¹Ã½ Ã€ÃºÃ‡Ã— ÃƒÃ–Ã€Ãº +21%
							case 8: // Â¹Â°Â¸Â®ÃˆÃ*Â¼Ã¶ ÃƒÃ–Ã€Ãº +9%
							case 9: // Â¸Â¶Â¹Ã½ÃˆÃ*Â¼Ã¶ ÃƒÃ–Ã€Ãº +9%
								if (dwValue <= 3) dwValue = 3;
								break;
							}
							// v2.03 912 Â¾Ã†Ã€ÃŒÃ…Ã› Â¼Ã¶Ã?Ã˜Ã€ÃŒ 2Ã€ÃŒÃ‡Ã?Ã€Ã?Â¶Â§ Ã†Â¯Â¼ÂºÃ„Â¡ ÃƒÃ–Â´Ã«Â°ÂªÃ€Âº 7
							if (dwValue > 7) dwValue = 7;

							// Â¾Ã†Ã€ÃŒÃ…Ã› Sub Ã†Â¯Â¼ÂºÃ„Â¡ Ã€Ã”Â·Ã‚
							dwType = dwType << 12;
							dwValue = dwValue << 8;
							pItem->m_dwAttribute = pItem->m_dwAttribute | dwType | dwValue;
						}
					}
				}
				//testcode
				wsprintf(G_cTxt, "Custom-Item(%s) Value(%d) Life(%d/%d)", pItem->m_cName, pItem->m_sItemSpecEffectValue2, pItem->m_wCurLifeSpan, pItem->m_wMaxLifeSpan);
				PutLogList(G_cTxt);

				// ¾ÆÀÌÅÛ Àü´Þ
				bAddItem(iClientH, pItem, NULL);
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_BUILDITEMSUCCESS, pItem->m_sItemSpecEffectValue2, pItem->m_cItemType, NULL, NULL); // Integer¸¦ Àü´ÞÇÏ±â À§ÇØ

#ifdef DEF_TAIWANLOG
		 // v1.41 Èñ±Í ¾ÆÀÌÅÛÀÌ¶ó¸é ·Î±×¸¦ ³²±ä´Ù.
				_bItemLog(DEF_ITEMLOG_MAKE, iClientH, (int)-1, pItem);
#endif   

				// ¸¶Áö¸·À¸·Î Àç·á°¡ µÇ´Â ¾ÆÀÌÅÛÀ» ¾ø¾Ø´Ù.
				for (x = 0; x < 6; x++)
					if (cElementItemID[x] != -1) {
						if (m_pClientList[iClientH]->m_pItemList[cElementItemID[x]] == NULL) {
							// ### BUG POINT!!!
							// ¹ö±×ÀÇ ¿øÀÎÀ» ¹àÈù´Ù.
							wsprintf(G_cTxt, "(?) Char(%s) ElementItemID(%d)", m_pClientList[iClientH]->m_cCharName, cElementItemID[x]);
							PutLogFileList(G_cTxt);
						}
						else {
							iCount = m_pClientList[iClientH]->m_pItemList[cElementItemID[x]]->m_dwCount - m_pBuildItemList[i]->m_iMaterialItemCount[x];
							if (iCount < 0) iCount = 0;
							SetItemCount(iClientH, cElementItemID[x], iCount);
						}
					}

				// ÀÌÁ¦ ½ºÅ³À» Ä«¿îÆ® ÇÑ´Ù. ´Ü ¾ÆÀÌÅÛÀÇ ÃÖ´ë ¼ºÀå ½ºÅ³ ÇÑµµ ³»¿¡¼*¸¸ °¡´ÉÇÏ´Ù. 
				if (m_pBuildItemList[i]->m_iMaxSkill > m_pClientList[iClientH]->m_cSkillMastery[13])
					CalculateSSN_SkillIndex(iClientH, 13, 1);

				// v1.41 ¼Ò·®ÀÇ °æÇèÄ¡ Áõ°¡
				GetExp(iClientH, iDice(1, (m_pBuildItemList[i]->m_iSkillLimit / 4)));

				break;
			}
		}
}

void CGame::MineralGenerator()
{
	int i, iP, tX, tY, iRet;

	for (i = 0; i < DEF_MAXMAPS; i++) {
		if ((iDice(1, 4) == 1) && (m_pMapList[i] != NULL) &&
			(m_pMapList[i]->m_bMineralGenerator == TRUE) &&
			(m_pMapList[i]->m_iCurMineral < m_pMapList[i]->m_iMaxMineral)) {

			iP = iDice(1, m_pMapList[i]->m_iTotalMineralPoint) - 1;
			if ((m_pMapList[i]->m_MineralPointList[iP].x == -1) || (m_pMapList[i]->m_MineralPointList[iP].y == -1)) break;

			tX = m_pMapList[i]->m_MineralPointList[iP].x;
			tY = m_pMapList[i]->m_MineralPointList[iP].y;

			iRet = iCreateMineral(i, tX, tY, m_pMapList[i]->m_cMineralGeneratorLevel);
		}
	}
}

int CGame::iCreateMineral(char cMapIndex, int tX, int tY, char cLevel)
{
	int i, iDynamicHandle, iMineralType;

	if ((cMapIndex < 0) || (cMapIndex >= DEF_MAXMAPS)) return NULL;
	if (m_pMapList[cMapIndex] == NULL) return NULL;

	for (i = 1; i < DEF_MAXMINERALS; i++)
		if (m_pMineral[i] == NULL) {
			// ºó °ø°£¿¡ ±¤¹°µ¢ÀÌ¸¦ ¸¸µç´Ù.
			iMineralType = iDice(1, cLevel);
			m_pMineral[i] = new class CMineral(iMineralType, cMapIndex, tX, tY, 1);
			if (m_pMineral[i] == NULL) return NULL;

			iDynamicHandle = NULL;
			switch (iMineralType) {
			default:
				iDynamicHandle = iAddDynamicObjectList(NULL, NULL, DEF_DYNAMICOBJECT_MINERAL1, cMapIndex, tX, tY, NULL, i);
				break;

			case 5: // º¸¼®·ù 
			case 6:
				iDynamicHandle = iAddDynamicObjectList(NULL, NULL, DEF_DYNAMICOBJECT_MINERAL2, cMapIndex, tX, tY, NULL, i);
				break;
			}

			if (iDynamicHandle == NULL) {
				delete m_pMineral[i];
				m_pMineral[i] = NULL;
				return NULL;
			}
			m_pMineral[i]->m_sDynamicObjectHandle = iDynamicHandle;
			m_pMineral[i]->m_cMapIndex = cMapIndex;

			// ³­ÀÌµµ¿Í ¼ö·®À» ÀÔ·ÂÇÑ´Ù.
			switch (iMineralType) {
			case 1: m_pMineral[i]->m_iDifficulty = 10; m_pMineral[i]->m_iRemain = 20; break;
			case 2: m_pMineral[i]->m_iDifficulty = 15; m_pMineral[i]->m_iRemain = 15; break;
			case 3: m_pMineral[i]->m_iDifficulty = 20; m_pMineral[i]->m_iRemain = 10; break;
			case 4: m_pMineral[i]->m_iDifficulty = 50; m_pMineral[i]->m_iRemain = 8; break;
			case 5: m_pMineral[i]->m_iDifficulty = 70; m_pMineral[i]->m_iRemain = 6; break;
			case 6: m_pMineral[i]->m_iDifficulty = 90; m_pMineral[i]->m_iRemain = 4; break;
			default: m_pMineral[i]->m_iDifficulty = 10; m_pMineral[i]->m_iRemain = 20; break;
			}

			// ¸ÊÀÇ ¹Ì³×¶ö ¼ö Áõ°¡
			m_pMapList[cMapIndex]->m_iCurMineral++;

			return i;
		}

	return NULL;
}

void CGame::_CheckMiningAction(int iClientH, int dX, int dY)
{
	short sType;
	DWORD dwRegisterTime;
	int   iDynamicIndex, iSkillLevel, iResult, iItemID;
	class CItem* pItem;
	WORD  wWeaponType;

	if (m_pClientList[iClientH] == NULL)  return;

	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bGetDynamicObject(dX, dY, &sType, &dwRegisterTime, &iDynamicIndex);

	switch (sType) {
	case DEF_DYNAMICOBJECT_MINERAL1:
	case DEF_DYNAMICOBJECT_MINERAL2:
		// ±¤¹°Ã¤Ãë¸¦ ½ÃµµÇÑ Ä³¸¯ÅÍÀÇ Á¶°ÇÀ» »ìÇÉ´Ù. °î±ªÀÌ¸¦ µé°í ÀÖ´Â°¡? ±¤¹° Ã¤Ãë ½ºÅ³Àº? 
		// °î±ªÀÌ¿¡ ÇØ´çÇÏ´Â ¿ÜÇüÀÌ¸é Åë°ú 
		wWeaponType = ((m_pClientList[iClientH]->m_sAppr2 & 0x0FF0) >> 4);
		if (wWeaponType != 25) {
			// ÇÈ¾×½º¸¦ µé°í ÀÖ´Ù. ±¤¹° Ã¤Ãë °¡´É 
			return;
		}

		// ÀüÅõ ¸ð¼ÇÀÌ ¾Æ´Ï´õ¶óµµ ¸®ÅÏ 
		if ((m_pClientList[iClientH]->m_sAppr2 & 0xF000) == 0) return;

		iSkillLevel = m_pClientList[iClientH]->m_cSkillMastery[0];
		if (iSkillLevel == 0) break;

		if (m_pDynamicObjectList[iDynamicIndex] == NULL) break;
		// Ä³´Â ±¤¹°ÀÇ ³­ÀÌµµ¸¸Å­ ½ºÅ³À» ³·Ãá´Ù.
		iSkillLevel -= m_pMineral[m_pDynamicObjectList[iDynamicIndex]->m_iV1]->m_iDifficulty;
		if (iSkillLevel <= 0) iSkillLevel = 1;

		iResult = iDice(1, 100);
		if (iResult <= iSkillLevel) {
			// È®·üÀº ¼º°øÇß´Ù. ±¤¹°Ã¤Ãë ±â¼ú Áõ°¡ 
			CalculateSSN_SkillIndex(iClientH, 0, 1);

			// ÇÃ·¹ÀÌ¾îÀÇ ¹ß ¹Ø¿¡ ±¤¹°À» ¶³¾î¶ß¸°´Ù. 
			switch (m_pMineral[m_pDynamicObjectList[iDynamicIndex]->m_iV1]->m_cType) {
			case 1:
				switch (iDice(1, 5)) {
				case 1:
				case 2:
				case 3:
					iItemID = 355; // Coal
					GetExp(iClientH, iDice(1, iDice(1, 3)));
					break;
				case 4:
					iItemID = 357; // IronOre
					GetExp(iClientH, iDice(1, iDice(1, 3)));
					break;
				case 5:
					iItemID = 507; // BlondeStone
					GetExp(iClientH, iDice(1, iDice(1, 3)));
					break;
				}
				break;

			case 2:
				switch (iDice(1, 5)) {
				case 1:
				case 2:
					iItemID = 355; // Coal
					GetExp(iClientH, iDice(1, iDice(1, 3)));
					break;
				case 3:
				case 4:
					iItemID = 357; // IronOre
					GetExp(iClientH, iDice(1, iDice(1, 3)));
					break;
				case 5:
					if (iDice(1, 3) == 2) {
						iItemID = 356; // SilverNugget
						GetExp(iClientH, iDice(1, iDice(1, 4)));
					}
					else {
						iItemID = 507; // BlondeStone
						GetExp(iClientH, iDice(1, iDice(1, 3)));
					}
					break;
				}
				break;

			case 3:
				switch (iDice(1, 6)) {
				case 1:
					iItemID = 355; // Coal
					GetExp(iClientH, iDice(1, iDice(1, 3)));
					break;
				case 2:
				case 3:
				case 4:
				case 5:
					iItemID = 357; // IronOre
					GetExp(iClientH, iDice(1, iDice(1, 3)));
					break;
				case 6:
					if (iDice(1, 8) == 3) {
						if (iDice(1, 2) == 1) {
							iItemID = 356; // SilverNugget
							GetExp(iClientH, iDice(1, iDice(1, 4)));
						}
						else {
							iItemID = 357; // IronOre
							GetExp(iClientH, iDice(1, iDice(1, 3)));
						}
					}
					else {
						iItemID = 357; // IronOre
						GetExp(iClientH, iDice(1, iDice(1, 3)));
					}
					break;
				}
				break;

			case 4:
				switch (iDice(1, 6)) {
				case 1:
					iItemID = 355; // Coal
					GetExp(iClientH, iDice(1, iDice(1, 3)));
					break;
				case 2:
					if (iDice(1, 3) == 2) {
						iItemID = 356; // SilverNugget
						GetExp(iClientH, iDice(1, iDice(1, 4)));
					}
					break;
				case 3:
				case 4:
				case 5:
					iItemID = 357; // IronOre
					GetExp(iClientH, iDice(1, iDice(1, 3)));
					break;
				case 6:
					if (iDice(1, 8) == 3) {
						if (iDice(1, 4) == 3) {
							if (iDice(1, 4) < 3) {
								iItemID = 508; // Mithral
								GetExp(iClientH, iDice(1, iDice(1, 15)));
							}
							else {
								iItemID = 354; // GoldNugget
								GetExp(iClientH, iDice(1, iDice(1, 5)));
							}
						}
						else {
							iItemID = 356; // SilverNugget
							GetExp(iClientH, iDice(1, iDice(1, 4)));
						}
					}
					else {
						if (iDice(1, 2) == 1) {
							iItemID = 354; // GoldNugget
							GetExp(iClientH, iDice(1, iDice(1, 5)));
						}
						else {
							iItemID = 357;  // IronOre
							GetExp(iClientH, iDice(1, iDice(1, 3)));
						}
					}
					break;
				}
				break;

			case 5:
				switch (iDice(1, 19)) {
				case 3:
					iItemID = 352; // Sapphire
					GetExp(iClientH, iDice(1, iDice(2, 3)));
					break;
				default:
					iItemID = 358; // Crystal
					GetExp(iClientH, iDice(1, iDice(2, 3)));
					break;
				}
				break;

			case 6:
				switch (iDice(1, 5)) {
				case 1:
					if (iDice(1, 6) == 3) {
						iItemID = 353; // Emerald 
						GetExp(iClientH, iDice(1, iDice(2, 4)));
					}
					else {
						iItemID = 358; // Crystal
						GetExp(iClientH, iDice(1, iDice(2, 3)));
					}
					break;
				case 2:
					if (iDice(1, 6) == 3) {
						iItemID = 352; // Saphire
						GetExp(iClientH, iDice(1, iDice(2, 4)));
					}
					else {
						iItemID = 358; // Crystal
						GetExp(iClientH, iDice(1, iDice(2, 3)));
					}
					break;
				case 3:
					if (iDice(1, 6) == 3) {
						iItemID = 351; // Ruby
						GetExp(iClientH, iDice(1, iDice(2, 4)));
					}
					else {
						iItemID = 358; // Crystal 
						GetExp(iClientH, iDice(1, iDice(2, 3)));
					}
					break;
				case 4:
					iItemID = 358; // Crystal 
					GetExp(iClientH, iDice(1, iDice(2, 3)));
					break;
				case 5:
					if (iDice(1, 12) == 3) {
						iItemID = 350; // Diamond
						GetExp(iClientH, iDice(1, iDice(2, 5)));
					}
					else {
						iItemID = 358; // Crystal
						GetExp(iClientH, iDice(1, iDice(2, 3)));
					}
					break;
				}
				break;

			}

			pItem = new class CItem;
			if (_bInitItemAttr(pItem, iItemID) == FALSE) {
				delete pItem;
			}
			else {
				// ¾ÆÀÌÅÛÀ» ¼­ÀÖ´Â À§Ä¡¿¡ ¹ö¸°´Ù. 
				m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
					m_pClientList[iClientH]->m_sY, pItem);
				// ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¾ÆÀÌÅÛÀÌ ¶³¾îÁø °ÍÀ» ¾Ë¸°´Ù. 
				SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
					m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
					pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4

			}

			// ±¤¹°ÀÇ ³²Àº ·®À» ÁÙÀÌ°í 0ÀÎ°æ¿ì »èÁ¦ÇÑ´Ù.
			m_pMineral[m_pDynamicObjectList[iDynamicIndex]->m_iV1]->m_iRemain--;
			if (m_pMineral[m_pDynamicObjectList[iDynamicIndex]->m_iV1]->m_iRemain <= 0) {
				// ±¤¹°ÀÌ ¸ðµÎ ¼Ò¸ðµÇ¾ú´Ù. Delete Mineral 
				bDeleteMineral(m_pDynamicObjectList[iDynamicIndex]->m_iV1);

				// µ¿Àû °´Ã¼ »èÁ¦
				delete m_pDynamicObjectList[iDynamicIndex];
				m_pDynamicObjectList[iDynamicIndex] = NULL;
			}
		}
		break;

	default:
		break;
	}
}

void CGame::bDeleteMineral(int iIndex)
{
	int iDynamicIndex;
	DWORD dwTime;

	dwTime = timeGetTime();

	if (m_pMineral[iIndex] == NULL) return;
	iDynamicIndex = m_pMineral[iIndex]->m_sDynamicObjectHandle;
	if (m_pDynamicObjectList[iDynamicIndex] == NULL) return;

	SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_REJECT, m_pDynamicObjectList[iDynamicIndex]->m_cMapIndex,
		m_pDynamicObjectList[iDynamicIndex]->m_sX, m_pDynamicObjectList[iDynamicIndex]->m_sY,
		m_pDynamicObjectList[iDynamicIndex]->m_sType, iDynamicIndex, NULL);
	// ¸Ê¿¡¼­ ±¤¹° µ¿Àû °´Ã¼¸¦ »èÁ¦ÇÑ´Ù.
	m_pMapList[m_pDynamicObjectList[iDynamicIndex]->m_cMapIndex]->SetDynamicObject(NULL, NULL, m_pDynamicObjectList[iDynamicIndex]->m_sX, m_pDynamicObjectList[iDynamicIndex]->m_sY, dwTime);
	// ±¤¹°ÀÌ »ç¶óÁ³À¸¹Ç·Î ÀÌµ¿ÀÌ °¡´ÉÇÏ°Ô ÇÑ´Ù. 
	m_pMapList[m_pMineral[iIndex]->m_cMapIndex]->SetTempMoveAllowedFlag(m_pDynamicObjectList[iDynamicIndex]->m_sX, m_pDynamicObjectList[iDynamicIndex]->m_sY, TRUE);

	// ±¤¹° °³Ã¼ ¼ö °¨¼Ò 
	m_pMapList[m_pMineral[iIndex]->m_cMapIndex]->m_iCurMineral--;

	// ±¤¹° °´Ã¼ »èÁ¦ 
	delete m_pMineral[iIndex];
	m_pMineral[iIndex] = NULL;
}