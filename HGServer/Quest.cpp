#include "Quest.h"

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

BOOL CGame::_bDecodeQuestConfigFileContents(char* pData, DWORD dwMsgSize)
{
	char* pContents, * token, cTxt[120];
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int  iQuestConfigListIndex = 0;
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
					// Äù½ºÆ® ¹øÈ£ 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}

					if (m_pQuestConfigList[atoi(token)] != NULL) {
						// ÀÌ¹Ì ÇÒ´çµÈ ¹øÈ£°¡ ÀÖ´Ù. ¿¡·¯ÀÌ´Ù.
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Duplicate quest number.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[atoi(token)] = new class CQuest;
					iQuestConfigListIndex = atoi(token);

					cReadModeB = 2;
					break;

				case 2:
					// Äù½ºÆ® »çÀÌµå  
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_cSide = atoi(token);
					cReadModeB = 3;
					break;

				case 3:
					// Äù½ºÆ® Á¾·ù  
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iType = atoi(token);
					cReadModeB = 4;
					break;

				case 4:
					// TargetType
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iTargetType = atoi(token);
					cReadModeB = 5;
					break;

				case 5:
					// MaxCount
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iMaxCount = atoi(token);
					cReadModeB = 6;
					break;

				case 6:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iFrom = atoi(token);
					cReadModeB = 7;
					break;

				case 7:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iMinLevel = atoi(token);
					cReadModeB = 8;
					break;

				case 8:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iMaxLevel = atoi(token);
					cReadModeB = 9;
					break;

				case 9:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iRequiredSkillNum = atoi(token);
					cReadModeB = 10;
					break;

				case 10:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iRequiredSkillLevel = atoi(token);
					cReadModeB = 11;
					break;

				case 11:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iTimeLimit = atoi(token);
					cReadModeB = 12;
					break;

				case 12:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iAssignType = atoi(token);
					cReadModeB = 13;
					break;

				case 13:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardType[1] = atoi(token);
					cReadModeB = 14;
					break;

				case 14:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardAmount[1] = atoi(token);
					cReadModeB = 15;
					break;

				case 15:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardType[2] = atoi(token);
					cReadModeB = 16;
					break;

				case 16:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardAmount[2] = atoi(token);
					cReadModeB = 17;
					break;

				case 17:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardType[3] = atoi(token);
					cReadModeB = 18;
					break;

				case 18:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iRewardAmount[3] = atoi(token);
					cReadModeB = 19;
					break;

				case 19:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iContribution = atoi(token);
					cReadModeB = 20;
					break;

				case 20:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iContributionLimit = atoi(token);
					cReadModeB = 21;
					break;

				case 21:
					// 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iResponseMode = atoi(token);
					cReadModeB = 22;
					break;

				case 22:
					ZeroMemory(m_pQuestConfigList[iQuestConfigListIndex]->m_cTargetName, sizeof(m_pQuestConfigList[iQuestConfigListIndex]->m_cTargetName));
					strcpy(m_pQuestConfigList[iQuestConfigListIndex]->m_cTargetName, token);
					cReadModeB = 23;
					break;

				case 23:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_sX = atoi(token);
					cReadModeB = 24;
					break;

				case 24:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_sY = atoi(token);
					cReadModeB = 25;
					break;

				case 25:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iRange = atoi(token);
					cReadModeB = 26;
					break;

				case 26:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iQuestID = atoi(token);
					cReadModeB = 27;
					break;

				case 27:
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pQuestConfigList[iQuestConfigListIndex]->m_iReqContribution = atoi(token);
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
			if (memcmp(token, "quest", 5) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
			}
		}
		token = pStrTok->pGet();
	}

	delete pStrTok;
	delete[] pContents;

	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) CRITICAL ERROR! QUEST configuration file contents error!");
		return FALSE;
	}

	wsprintf(cTxt, "(!) QUEST(Total:%d) configuration - success!", iQuestConfigListIndex);
	PutLogList(cTxt);

	return TRUE;
}


int CGame::__iSearchForQuest(int iClientH, int iWho, int* pQuestType, int* pMode, int* pRewardType, int* pRewardAmount, int* pContribution, char* pTargetName, int* pTargetType, int* pTargetCount, int* pX, int* pY, int* pRange)
{
	int i, iQuestList[DEF_MAXQUESTTYPE], iIndex, iQuest, iReward, iQuestIndex;

	if (m_pClientList[iClientH] == NULL) return -1;

	// ÀûÇÕÇÑ ¹Ì¼Ç ¹è¿­ ÃÊ±âÈ­.
	iIndex = 0;
	for (i = 0; i < DEF_MAXQUESTTYPE; i++)
		iQuestList[i] = -1;

	for (i = 1; i < DEF_MAXQUESTTYPE; i++)
		if (m_pQuestConfigList[i] != NULL) {

			if (m_pQuestConfigList[i]->m_iFrom != iWho) goto SFQ_SKIP;
			if (m_pQuestConfigList[i]->m_cSide != m_pClientList[iClientH]->m_cSide) goto SFQ_SKIP;
			if (m_pQuestConfigList[i]->m_iMinLevel > m_pClientList[iClientH]->m_iLevel) goto SFQ_SKIP;
			if (m_pQuestConfigList[i]->m_iMaxLevel < m_pClientList[iClientH]->m_iLevel) goto SFQ_SKIP;
			if (m_pQuestConfigList[i]->m_iReqContribution > m_pClientList[iClientH]->m_iContribution) goto SFQ_SKIP;

			if (m_pQuestConfigList[i]->m_iRequiredSkillNum != -1) {
				// °í·ÁÇØ¾ß ÇÒ ½ºÅ³ÀÌ ÀÖ´Ù. 
				if (m_pClientList[iClientH]->m_cSkillMastery[m_pQuestConfigList[i]->m_iRequiredSkillNum] <
					m_pQuestConfigList[i]->m_iRequiredSkillLevel) goto SFQ_SKIP;
			}

			// Å©·ç¼¼ÀÌµå ¸ðµåÀÏ¶§´Â ÇÒ´ç Å¸ÀÔÀÌ 1ÀÎ Äù½ºÆ®¸¸ ºÎ¿©µÈ´Ù.
			if ((m_bIsCrusadeMode == TRUE) && (m_pQuestConfigList[i]->m_iAssignType != 1)) goto SFQ_SKIP;
			// Å©·ç¼¼ÀÌµå ¸ðµå°¡ ¾Æ´Ò¶§´Â ÇÒ´ç Å¸ÀÔÀÌ 1Àº ¼±ÅÃµÇÁö ¾Ê´Â´Ù.
			if ((m_bIsCrusadeMode == FALSE) && (m_pQuestConfigList[i]->m_iAssignType == 1)) goto SFQ_SKIP;

			if (m_pQuestConfigList[i]->m_iContributionLimit < m_pClientList[iClientH]->m_iContribution) goto SFQ_SKIP;

			// ¿©±â±îÁö ¿À¸é Á¶°Ç¿¡ ÇÕ´çÇÑ Äù½ºÆ®´Ù. µî·ÏÇÑ´Ù. 
			iQuestList[iIndex] = i;
			iIndex++;

		SFQ_SKIP:;
		}

	// iIndex°³ ¸¸Å­ÀÇ Á¶°Ç¿¡ ¸¸Á·ÇÏ´Â Äù½ºÆ®¸¦ Ã£¾Ò´Ù. ±× Áß¿¡¼­ ·£´ýÇÏ°Ô 1°³¸¦ ¼±ÅÃÇÑ´Ù.
	if (iIndex == 0) return -1;
	iQuest = (iDice(1, iIndex)) - 1;
	iQuestIndex = iQuestList[iQuest];
	// Äù½ºÆ® ÀÎµ¦½º 
	iReward = iDice(1, 3);
	*pMode = m_pQuestConfigList[iQuestIndex]->m_iResponseMode;					// Äù½ºÆ® ÀÀ´ä ¸ðµå 
	*pRewardType = m_pQuestConfigList[iQuestIndex]->m_iRewardType[iReward];	// Äù½ºÆ® »óÇ° Á¾·ù 
	// SNOOPY: Added a trap for item non existing on the server...
	if (m_pItemConfigList[m_pQuestConfigList[iQuestIndex]->m_iRewardType[iReward]] == NULL)
	{
		*pRewardAmount = -1; // Replaces by Experience				
	}
	else *pRewardAmount = m_pQuestConfigList[iQuestIndex]->m_iRewardAmount[iReward];	// Äù½ºÆ® »óÇ° °¹¼ö 
	*pContribution = m_pQuestConfigList[iQuestIndex]->m_iContribution;			// Äù½ºÆ® ÇØ°á½Ã ¿Ã¶ó°¡´Â °øÇåµµ 

	strcpy(pTargetName, m_pQuestConfigList[iQuestIndex]->m_cTargetName);			// Äù½ºÆ® ¸Ê ÀÌ¸§ ¼ö·Ï 
	*pX = m_pQuestConfigList[iQuestIndex]->m_sX;
	*pY = m_pQuestConfigList[iQuestIndex]->m_sY;
	*pRange = m_pQuestConfigList[iQuestIndex]->m_iRange;

	*pTargetType = m_pQuestConfigList[iQuestIndex]->m_iTargetType;
	*pTargetCount = m_pQuestConfigList[iQuestIndex]->m_iMaxCount;
	*pQuestType = m_pQuestConfigList[iQuestIndex]->m_iType;

	return iQuestIndex;
}

// New 14/05/2004
void CGame::QuestAcceptedHandler(int iClientH)
{
	int iIndex, cQuestRemain;

	if (m_pClientList[iClientH] == NULL) return;

	// Does the quest exist ??
	if (m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest] == NULL) return;

	if (m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest]->m_iAssignType == 1) {
		// Àü¸éÀü¿ë Äù½ºÆ®ÀÌ´Ù. 
		switch (m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest]->m_iType) {
		case 10: // ÅÚ·¹Æ÷Æ®¿ë 1È¸¼º Äù½ºÆ®. ÅÚ·¹Æ÷Æ®ÇÏ°í ³ª¸é ³¡ÀÌ´Ù. ´õÀÌ»óÀÇ Äù½ºÆ® ¿©ºÎ°¡ ¾øÀ½.
			_ClearQuestStatus(iClientH);
			RequestTeleportHandler(iClientH, "2   ", m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest]->m_cTargetName,
				m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest]->m_sX, m_pQuestConfigList[m_pClientList[iClientH]->m_iAskedQuest]->m_sY);
			return;
		}
	}

	// ÇÃ·¹ÀÌ¾î°¡ Äù½ºÆ®¸¦ ¼ö¶ôÇß´Ù. Äù½ºÆ® ¹øÈ£, ID°ª ÇÒ´ç.
	m_pClientList[iClientH]->m_iQuest = m_pClientList[iClientH]->m_iAskedQuest;
	iIndex = m_pClientList[iClientH]->m_iQuest;
	m_pClientList[iClientH]->m_iQuestID = m_pQuestConfigList[iIndex]->m_iQuestID;
	m_pClientList[iClientH]->m_iCurQuestCount = 0;
	m_pClientList[iClientH]->m_bIsQuestCompleted = FALSE;

	_CheckQuestEnvironment(iClientH);
	_SendQuestContents(iClientH);

	if (m_pClientList[iClientH]->m_iQuest != NULL)
	{
		cQuestRemain = (m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest]->m_iMaxCount - m_pClientList[iClientH]->m_iCurQuestCount);
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_QUESTCOUNTER, cQuestRemain, NULL, NULL, NULL);
		_bCheckIsQuestCompleted(iClientH);
	}
}


void CGame::_SendQuestContents(int iClientH)
{
	int iWho, iIndex, iQuestType, iContribution, iTargetType, iTargetCount, iX, iY, iRange, iQuestCompleted;
	char cTargetName[21];

	if (m_pClientList[iClientH] == NULL) return;

	iIndex = m_pClientList[iClientH]->m_iQuest;
	if (iIndex == NULL) {
		// ÇÒ´çµÈ Quest°¡ ¾ø´Ù. 
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_QUESTCONTENTS, NULL, NULL, NULL, NULL,
			NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	}
	else {
		// ÇÒ´çµÈ Quest ³»¿ëÀ» º¸³»ÁØ´Ù.
		iWho = m_pQuestConfigList[iIndex]->m_iFrom;
		iQuestType = m_pQuestConfigList[iIndex]->m_iType;
		iContribution = m_pQuestConfigList[iIndex]->m_iContribution;
		iTargetType = m_pQuestConfigList[iIndex]->m_iTargetType;
		iTargetCount = m_pQuestConfigList[iIndex]->m_iMaxCount;
		iX = m_pQuestConfigList[iIndex]->m_sX;
		iY = m_pQuestConfigList[iIndex]->m_sY;
		iRange = m_pQuestConfigList[iIndex]->m_iRange;
		ZeroMemory(cTargetName, sizeof(cTargetName));
		memcpy(cTargetName, m_pQuestConfigList[iIndex]->m_cTargetName, 20);
		iQuestCompleted = (int)m_pClientList[iClientH]->m_bIsQuestCompleted;

		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_QUESTCONTENTS, iWho, iQuestType, iContribution, NULL,
			iTargetType, iTargetCount, iX, iY, iRange, iQuestCompleted, cTargetName);
	}
}

void CGame::_CheckQuestEnvironment(int iClientH)
{
	int iIndex;
	char cTargetName[21];

	if (m_pClientList[iClientH] == NULL) return;

	iIndex = m_pClientList[iClientH]->m_iQuest;
	if (iIndex == NULL) return;
	if (m_pQuestConfigList[iIndex] == NULL) return;

	if (iIndex >= 35 && iIndex <= 40) {
		m_pClientList[iClientH]->m_iQuest = 0;
		m_pClientList[iClientH]->m_iQuestID = NULL;
		m_pClientList[iClientH]->m_iQuestRewardAmount = NULL;
		m_pClientList[iClientH]->m_iQuestRewardType = NULL;
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_QUESTABORTED, NULL, NULL, NULL, NULL);
		return;
	}

	if (m_pQuestConfigList[iIndex]->m_iQuestID != m_pClientList[iClientH]->m_iQuestID) {
		// Äù½ºÆ® ID °ªÀÌ ´Ù¸£´Ù. ÇÒ´ç¹Þ¾Ò´ø ±× Äù½ºÆ®°¡ ¾Æ´Ï´Ù. Äù½ºÆ®´Â Ãë¼ÒµÈ´Ù.
		m_pClientList[iClientH]->m_iQuest = NULL;
		m_pClientList[iClientH]->m_iQuestID = NULL;
		m_pClientList[iClientH]->m_iQuestRewardAmount = NULL;
		m_pClientList[iClientH]->m_iQuestRewardType = NULL;

		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_QUESTABORTED, NULL, NULL, NULL, NULL);
		return;
	}

	switch (m_pQuestConfigList[iIndex]->m_iType) {
	case DEF_QUESTTYPE_MONSTERHUNT:
	case DEF_QUESTTYPE_GOPLACE:
		// ¸ó½ºÅÍ »ç³ÉÀÇ °æ¿ì ¸Å¹ø ¸ó½ºÅÍ »ç³É½Ã ¸Ê ÀÌ¸§À» ºñ±³ÇÏ´Â °ÍÀº ºñÈ¿À²ÀûÀÌ¹Ç·Î ¹Ì¸® ¸Ê ÀÌ¸§À» ºñ±³, ÇÃ·¡±×¸¦ ¼³Á¤ÇØ ³õ´Â´Ù. 
		ZeroMemory(cTargetName, sizeof(cTargetName));
		memcpy(cTargetName, m_pQuestConfigList[iIndex]->m_cTargetName, 20);
		if (memcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, cTargetName, 10) == 0)
			m_pClientList[iClientH]->m_bQuestMatchFlag_Loc = TRUE;
		else m_pClientList[iClientH]->m_bQuestMatchFlag_Loc = FALSE;
		break;
	}

}

BOOL CGame::_bCheckIsQuestCompleted(int iClientH)
{
	int iQuestIndex;

	// Äù½ºÆ®°¡ Á¾·áµÇ¾ú´ÂÁö ÆÇ´ÜÇÑ ÈÄ ¸Þ½ÃÁö Ã³¸®.
	if (m_pClientList[iClientH] == NULL) return FALSE;
	if (m_pClientList[iClientH]->m_bIsQuestCompleted == TRUE) return FALSE;
	iQuestIndex = m_pClientList[iClientH]->m_iQuest;
	if (iQuestIndex == NULL) return FALSE;

	if (m_pQuestConfigList[iQuestIndex] != NULL) {
		switch (m_pQuestConfigList[iQuestIndex]->m_iType) {
		case DEF_QUESTTYPE_MONSTERHUNT:
			if ((m_pClientList[iClientH]->m_bQuestMatchFlag_Loc == TRUE) &&
				(m_pClientList[iClientH]->m_iCurQuestCount >= m_pQuestConfigList[iQuestIndex]->m_iMaxCount)) {
				// ¸ó½ºÅÍ »ç³É Äù½ºÆ®°¡ ¿Ï·áµÇ¾ú´Ù. ¿Ï·áµÇ¾úÀ¸¸é ÀÇ·ÚÀÎ¿¡°Ô µ¹¾Æ°¡ ´ëÈ­¸¦ ÇÏ¸é Æ÷»ó¹Þ´Â´Ù. 
				m_pClientList[iClientH]->m_bIsQuestCompleted = TRUE;
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_QUESTCOMPLETED, NULL, NULL, NULL, NULL);
				return TRUE;
			}
			break;

		case DEF_QUESTTYPE_GOPLACE:
			if ((m_pClientList[iClientH]->m_bQuestMatchFlag_Loc == TRUE) && // ¸ÊÀº ¸Â°í
				(m_pClientList[iClientH]->m_sX >= m_pQuestConfigList[iQuestIndex]->m_sX - m_pQuestConfigList[iQuestIndex]->m_iRange) && // ÁÂÇ¥µµ ÀÌ³»ÀÌ¸é 
				(m_pClientList[iClientH]->m_sX <= m_pQuestConfigList[iQuestIndex]->m_sX + m_pQuestConfigList[iQuestIndex]->m_iRange) &&
				(m_pClientList[iClientH]->m_sY >= m_pQuestConfigList[iQuestIndex]->m_sY - m_pQuestConfigList[iQuestIndex]->m_iRange) &&
				(m_pClientList[iClientH]->m_sY <= m_pQuestConfigList[iQuestIndex]->m_sY + m_pQuestConfigList[iQuestIndex]->m_iRange)) {
				// Æ¯Á¤ Àå¼Ò Ã£¾Æ°¡±â Äù½ºÆ® ¿Ï·á.
				m_pClientList[iClientH]->m_bIsQuestCompleted = TRUE;
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_QUESTCOMPLETED, NULL, NULL, NULL, NULL);
				return TRUE;
			}
			break;
		}
	}

	return FALSE;
}

int CGame::_iTalkToNpcResult_Cityhall(int iClientH, int* pQuestType, int* pMode, int* pRewardType, int* pRewardAmount, int* pContribution, char* pTargetName, int* pTargetType, int* pTargetCount, int* pX, int* pY, int* pRange)
{
	int iQuest, iEraseReq;
	class CItem* pItem;
	unsigned long iExp;

	// Return Code
	// -1  : ÇöÀç ÀÓ¹« ¼öÇàÁß
	// -2  : ¸¶À»ÀÌ ´Ù¸£°Å³ª Áß¸³Àº ÀÓ¹«¸¦ ¾òÀ» ¼ö ¾ø´Ù.    
	// -3  : ¹üÁËÀÚ´Â ÀÓ¹«¸¦ ¾òÀ» ¼ö ¾ø´Ù.
	// -4  : ÇöÀç Á¶°Ç¿¡ ¸Â´Â ÀÓ¹«°¡ ¾ø´Ù. ÀÏ»óÀûÀÎ ´ëÈ­³ª Á¤º¸ Ãâ·Â 

	// ½ÃÃ» ÇàÁ¤°ü¿¡°Ô ¸»À» °É¾ú´Ù. ·¹º§°ú Äù½ºÆ® »óÅÂ¿¡ µû¶ó ÀÓ¹«¸¦ ÇÒ´çÇÑ´Ù. 
	if (m_pClientList[iClientH] == NULL) return 0;

	if (m_pClientList[iClientH]->m_iQuest != NULL) {
		if (m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest] == NULL) return -4;
		else if (m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest]->m_iFrom == 4) {
			// ½ÃÃ» ÇàÁ¤°ü¿¡°Ô ¸ÃÀº ÀÓ¹«´Ù. ÀÓ¹«°¡ ³¡³µ´Ù¸é ¿©±â¼­ Ã³¸®ÇÏ°í -5¸¦ ¸®ÅÏÇÑ´Ù.
			if (m_pClientList[iClientH]->m_bIsQuestCompleted == TRUE) {
				// Æ÷»óÀ» ÇÒ ¼ö ¾ø´Ù¸é(»óÇ°À» µé ¼ö ¾ø´Â »óÈ²ÀÌ¶ó´ø°¡) ÀÓ¹« Á¾·á Ã³¸®¸¦ ÇÏÁö ¾Ê´Â´Ù.
				// ¸ðµç Á¶°ÇÀÌ ¸¸Á·µÇ¸é Ã³¸®ÇÏ°í -5¸¦ ¸®ÅÏ.
				if ((m_pClientList[iClientH]->m_iQuestRewardType > 0) &&
					(m_pItemConfigList[m_pClientList[iClientH]->m_iQuestRewardType] != NULL)) {
					// Äù½ºÆ®ÀÇ ´ñ°¡°¡ ¾ÆÀÌÅÛÀÌ´Ù. ¾ÆÀÌÅÛÀ» ¹ÞÀ» ¼ö ÀÖ´ÂÁöÀÇ ¿©ºÎ¸¦ ÆÇ´ÜÇÑ´Ù.
					pItem = new class CItem;
					_bInitItemAttr(pItem, m_pItemConfigList[m_pClientList[iClientH]->m_iQuestRewardType]->m_cName);
					pItem->m_dwCount = m_pClientList[iClientH]->m_iQuestRewardAmount;
					if (_bCheckItemReceiveCondition(iClientH, pItem) == TRUE) {
						// Äù½ºÆ® ¾ÆÀÌÅÛÀ» ¹ÞÀ» ¼ö ÀÖ´Â Á¶°ÇÀÌ´Ù. ¼ö¿©ÇÑ´Ù.
						_bAddClientItemList(iClientH, pItem, &iEraseReq);
						SendItemNotifyMsg(iClientH, DEF_NOTIFY_ITEMOBTAINED, pItem, NULL);
						if (iEraseReq == 1) delete pItem;

						// °øÇåµµ »ó½Â 
						m_pClientList[iClientH]->m_iContribution += m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest]->m_iContribution;

						// Æ÷»óÀÌ Àü´ÞµÇ¾ú´Ù´Â ¸Þ½ÃÁö Àü¼Û ÈÄ ¸®ÅÏ. Äù½ºÆ® »óÅÂ Å¬¸®¾î.
						SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_QUESTREWARD, 4, 1, m_pClientList[iClientH]->m_iQuestRewardAmount,
							m_pItemConfigList[m_pClientList[iClientH]->m_iQuestRewardType]->m_cName, m_pClientList[iClientH]->m_iContribution);

						_ClearQuestStatus(iClientH);
						return -5;
					}
					else {
						delete pItem;
						// ¾ÆÀÌÅÛÀ» ¹ÞÀ» ¼ö ¾ø´Ù´Â ¸Þ½ÃÁö Àü¼Û ÈÄ ¸®ÅÏ
						SendItemNotifyMsg(iClientH, DEF_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);

						SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_QUESTREWARD, 4, 0, m_pClientList[iClientH]->m_iQuestRewardAmount,
							m_pItemConfigList[m_pClientList[iClientH]->m_iQuestRewardType]->m_cName, m_pClientList[iClientH]->m_iContribution);

						return -5;
					}
				}
				else if (m_pClientList[iClientH]->m_iQuestRewardType == -1) {
					// Äù½ºÆ®ÀÇ ´ñ°¡°¡ °æÇèÄ¡ÀÌ´Ù. 
					GetExp(iClientH, iDice(1, m_pClientList[iClientH]->m_iQuestRewardAmount));
					// °øÇåµµ »ó½Â 
					m_pClientList[iClientH]->m_iContribution += m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest]->m_iContribution;

					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_QUESTREWARD, 4, 1, m_pClientList[iClientH]->m_iQuestRewardAmount,
						"°æÇèÄ¡              ", m_pClientList[iClientH]->m_iContribution);

					// Äù½ºÆ® »óÅÂ¸¦ Å¬¸®¾îÇÑ´Ù.
					_ClearQuestStatus(iClientH);
					return -5;
				}
				else if (m_pClientList[iClientH]->m_iQuestRewardType == -2) {
					// Äù½ºÆ®ÀÇ ´ñ°¡´Â ºñ·Ê °æÇèÄ¡ Àû¿¡°Ô Á×¾úÀ»¶§ °æÇèÄ¡ * m_pClientList[iClientH]->m_iQuestRewardAmount ¸¸Å­ÀÇ °æÇèÄ¡¸¦ ¾ò´Â´Ù.
					//***
					iExp = (iDice(1, (10 * (m_pClientList[iClientH]->m_iLevel))) * m_pClientList[iClientH]->m_iQuestRewardAmount);

					GetExp(iClientH, iDice(1, iExp));
					// °øÇåµµ »ó½Â 
					m_pClientList[iClientH]->m_iContribution += m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest]->m_iContribution;

					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_QUESTREWARD, 4, 1, iExp,
						"°æÇèÄ¡              ", m_pClientList[iClientH]->m_iContribution);

					// Äù½ºÆ® »óÅÂ¸¦ Å¬¸®¾îÇÑ´Ù.
					_ClearQuestStatus(iClientH);
					return -5;
				}
				else {
					// Äù½ºÆ®ÀÇ ´ñ°¡´Â ¾ø´Ù. °øÇåµµ¸¸ ¿À¸¥´Ù.
					// °øÇåµµ »ó½Â 
					m_pClientList[iClientH]->m_iContribution += m_pQuestConfigList[m_pClientList[iClientH]->m_iQuest]->m_iContribution;

					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_QUESTREWARD, 4, 1, 0,
						"                     ", m_pClientList[iClientH]->m_iContribution);

					// Äù½ºÆ® »óÅÂ¸¦ Å¬¸®¾îÇÑ´Ù.
					_ClearQuestStatus(iClientH);
					return -5;
				}
			}
			else return -1;
		}

		return -4;	// ´Ù¸¥ NPC¿¡°Ô¼­ ¾òÀº Äù½ºÆ®¸¦ ¼öÇàÁßÀÌ´Ù. Áï Á¶°Ç¿¡ ¸Â´Â ÀÓ¹«°¡ ¾ø´Ù. ÀÏ»óÀûÀÎ ´ëÈ­¸¦ Ãâ·ÂÇÑ´Ù.
	}

	// ¾Æ¹«·± ÀÓ¹«¸¦ ¸Â°í ÀÖÁö ¾Ê´Ù. ÀÓ¹«¸¦ ¸ÃÀ» ¼ö ÀÖ´Â Á¶°ÇÀÎÁö ÆÇº°ÇÑ´Ù. 
	if (memcmp(m_pClientList[iClientH]->m_cLocation, m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, 10) == 0) {
		// ¸¶À» ¼Ò¼ÓÀÌ °°´Ù. 
		// ¹üÁËÀÚ¶ó¸é ÀÓ¹«¸¦ ¸ÃÀ» ¼ö ¾ø´Ù. 
		if (m_pClientList[iClientH]->m_iPKCount > 0) return -3;

		iQuest = __iSearchForQuest(iClientH, 4, pQuestType, pMode, pRewardType, pRewardAmount, pContribution, pTargetName, pTargetType, pTargetCount, pX, pY, pRange);
		if (iQuest <= 0) return -4;

		// ÀÓ¹« Ã£¾Ò´Ù. 
		return iQuest;
	}
	else return -2; // ¸¶À»ÀÌ ´Ù¸£°Å³ª ¸¶À» ¼Ò¼ÓÀÌ ¾Æ´Ï´Ù.

	return -4;
}

void CGame::_ClearQuestStatus(int iClientH)
{
	if (m_pClientList[iClientH] == NULL) return;

	m_pClientList[iClientH]->m_iQuest = NULL;
	m_pClientList[iClientH]->m_iQuestID = NULL;
	m_pClientList[iClientH]->m_iQuestRewardType = NULL;
	m_pClientList[iClientH]->m_iQuestRewardAmount = NULL;
	m_pClientList[iClientH]->m_bIsQuestCompleted = FALSE;
}

void CGame::CancelQuestHandler(int iClientH)
{
	if (m_pClientList[iClientH] == NULL) return;

	// Äù½ºÆ® »óÅÂ¸¦ Å¬¸®¾îÇÑ´Ù.
	_ClearQuestStatus(iClientH);
	// Äù½ºÆ®°¡ Ãë¼ÒµÇ¾úÀ½À» ¾Ë·ÁÁØ´Ù.
	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_QUESTABORTED, NULL, NULL, NULL, NULL);
}

