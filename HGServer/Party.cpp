#include "Party.h"

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

Party::Party()
{}

Party::~Party()
{}

void CGame::CreateNewPartyHandler(int iClientH)
{
	int bFlag;

	if (m_pClientList[iClientH] == NULL) return;

	bFlag = m_pClientList[iClientH]->bCreateNewParty();
	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_RESPONSE_CREATENEWPARTY, bFlag, NULL, NULL, NULL);
}

void CGame::JoinPartyHandler(int iClientH, int iV1, char* pMemberName)
{
	char* cp, cData[120];
	short sAppr2;
	DWORD* dwp;
	WORD* wp;
	int i;

	if (m_pClientList[iClientH] == NULL) return;
	if ((m_bAdminSecurity == TRUE) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0)) return;

	switch (iV1) {
	case 0: // ÆÄÆ¼ Å»Åð ½ÅÃ»
		RequestDeletePartyHandler(iClientH);
		break;

	case 1: // ÆÄÆ¼ °¡ÀÔ ½ÅÃ»


		if ((m_pClientList[iClientH]->m_iPartyID != NULL) || (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_NULL)) {
			// ÀÌ¹Ì ÆÄÆ¼¿¡ °¡ÀÔÇØ ÀÖ´Ù¸é ÆÄÆ¼ °¡ÀÔ ½ÅÃ»À» ÇÒ ¼ö ¾ø´Ù.
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_PARTY, 7, 0, NULL, NULL);
			m_pClientList[iClientH]->m_iReqJoinPartyClientH = NULL;
			ZeroMemory(m_pClientList[iClientH]->m_cReqJoinPartyName, sizeof(m_pClientList[iClientH]->m_cReqJoinPartyName));
			m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;

			break;
		}

		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != NULL) && (strcmp(m_pClientList[i]->m_cCharName, pMemberName) == 0)) {
				// °°Àº ÀÌ¸§À» °¡Áø ÇÃ·¹ÀÌ¾î¸¦ Ã£¾Ò´Ù.
				sAppr2 = (short)((m_pClientList[i]->m_sAppr2 & 0xF000) >> 12);
				if (sAppr2 != 0) {
					// ÀüÅõ ¸ðµå »óÅÂÀÇ »ó´ë¿¡°Ô´Â ÆÄÆ¼ °¡ÀÔ ½ÅÃ»À» ÇÒ ¼ö ¾ø´Ù.
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_PARTY, 7, 0, NULL, NULL);

				}
				else if (m_pClientList[i]->m_cSide != m_pClientList[iClientH]->m_cSide) {
					// ÆíÀÌ ´Ù¸£¸é ÆÄÆ¼¿¡ µé ¼ö ¾ø´Ù.
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_PARTY, 7, 0, NULL, NULL);

				}
				else if (m_pClientList[i]->m_iPartyStatus == DEF_PARTYSTATUS_PROCESSING) {
					// ÆÄÆ¼ °¡ÀÔÀ» ½ÅÃ»ÇÑ ´ë»óÀÌ ÀÌ¹Ì ´Ù¸¥ ÆÄÆ¼ °¡ÀÔ °ü·Ã Ã³¸®¸¦ ÇÏ°í ÀÖ´Ù. ½ÅÃ» ºÒ°¡.
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_PARTY, 7, 0, NULL, NULL);


					m_pClientList[iClientH]->m_iReqJoinPartyClientH = NULL;
					ZeroMemory(m_pClientList[iClientH]->m_cReqJoinPartyName, sizeof(m_pClientList[iClientH]->m_cReqJoinPartyName));
					m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
				}
				else {
					// °¡ÀÔ ½ÂÀÎ ¿©ºÎ¸¦ ¹¯´Â´Ù.
					m_pClientList[i]->m_iReqJoinPartyClientH = iClientH;
					ZeroMemory(m_pClientList[i]->m_cReqJoinPartyName, sizeof(m_pClientList[i]->m_cReqJoinPartyName));
					strcpy(m_pClientList[i]->m_cReqJoinPartyName, m_pClientList[iClientH]->m_cCharName);
					SendNotifyMsg(NULL, i, DEF_NOTIFY_QUERY_JOINPARTY, NULL, NULL, NULL, m_pClientList[iClientH]->m_cCharName);

					// ½ÅÃ»ÇÑ Ãø¿¡´Â ÆÄÆ¼ ¸¶½ºÅÍ°¡ µÉ Ä³¸¯ÅÍÀÇ ÀÎµ¦½º¸¦ ³Ö¾îÁÜ. Ãë¼ÒÇÒ¶§ Ã³¸®ÇÏ±â À§ÇÔÀÓ.
					m_pClientList[iClientH]->m_iReqJoinPartyClientH = i;
					ZeroMemory(m_pClientList[iClientH]->m_cReqJoinPartyName, sizeof(m_pClientList[iClientH]->m_cReqJoinPartyName));
					strcpy(m_pClientList[iClientH]->m_cReqJoinPartyName, m_pClientList[i]->m_cCharName);
					// ÆÄÆ¼ »óÅÂ ¼¼Æ®
					m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_PROCESSING;
				}
				break;
			}
		break;

	case 2: // ÆÄÆ¼ ¸â¹ö È®ÀÎ ¸í·É 
		if (m_pClientList[iClientH]->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM)
		{
			ZeroMemory(cData, sizeof(cData));
			cp = (char*)cData;
			dwp = (DWORD*)cp;
			*dwp = MSGID_PARTYOPERATION;
			cp += 4;
			wp = (WORD*)cp;
			*wp = 6; // ÆÄÆ¼ ¸â¹ö ¸®½ºÆ® ¿äÃ»
			cp += 2;
			wp = (WORD*)cp;
			*wp = iClientH;
			cp += 2;
			memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
			cp += 10;
			wp = (WORD*)cp;
			*wp = m_pClientList[iClientH]->m_iPartyID;
			cp += 2;
			SendMsgToGateServer(MSGID_PARTYOPERATION, iClientH, cData);
		}

		RefreshPartyStatus(iClientH);
		break;
	}
}

// New 06/05/2004
// Party Code
void CGame::RequestCreatePartyHandler(int iClientH)
{
	char* cp, cData[120];
	DWORD* dwp;
	WORD* wp;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;

	if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_NULL) {
		// ÆÄÆ¼ »óÅÂ°¡ ÀÌ¹Ì Á¸ÀçÇÏ¸é ÆÄÆ¼¸¦ ¸¸µé ¼ö ¾ø´Ù.
		return;
	}

	m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_PROCESSING;

	// Gate Server¿¡ ÆÄÆ¼ »ý¼ºÈÄ PartyID¸¦ ¾Ë·ÁÁÙ °ÍÀ» ¿äÃ»ÇÑ´Ù. 
	ZeroMemory(cData, sizeof(cData));
	cp = (char*)cData;

	dwp = (DWORD*)cp;
	*dwp = MSGID_PARTYOPERATION;
	cp += 4;
	wp = (WORD*)cp;
	*wp = 1; // 1, request
	cp += 2;

	wp = (WORD*)cp;
	*wp = iClientH;
	cp += 2;

	memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
	cp += 10;

	SendMsgToGateServer(MSGID_PARTYOPERATION, iClientH, cData);


}

// Last Updated October 28, 2004 - 3.51 translation
void CGame::PartyOperationResultHandler(char* pData)
{
	char* cp, cResult, cName[12];
	WORD* wp;
	int i, iClientH, iPartyID, iTotal;

	cp = (char*)(pData + 4);
	wp = (WORD*)cp;
	cp += 2;

	switch (*wp) {
	case 1:
		cResult = *cp;
		cp++;

		wp = (WORD*)cp;
		iClientH = (int)*wp;
		cp += 2;

		ZeroMemory(cName, sizeof(cName));
		memcpy(cName, cp, 10);
		cp += 10;

		wp = (WORD*)cp;
		iPartyID = (int)*wp;
		cp += 2;

		PartyOperationResult_Create(iClientH, cName, cResult, iPartyID);


		break;

	case 2:
		wp = (WORD*)cp;
		iPartyID = *wp;
		cp += 2;

		PartyOperationResult_Delete(iPartyID);


		break;

	case 3:
		wp = (WORD*)cp;
		iClientH = *wp;
		cp += 2;

		ZeroMemory(cName, sizeof(cName));
		memcpy(cName, cp, 10);
		cp += 10;

		// Strange bug here, <0 and above positive integer!!!!
		if ((iClientH < 1) || (iClientH > DEF_MAXCLIENTS)) return;

		if (m_pClientList[iClientH] == NULL) return;
		if (strcmp(m_pClientList[iClientH]->m_cCharName, cName) != 0) return;

		for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
			if (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] == iClientH) {
				m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] = 0;
				m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers--;


				break;
			}

		for (i = 0; i < DEF_MAXPARTYMEMBERS - 1; i++)
			if ((m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] == 0) && (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i + 1] != 0)) {
				m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] = m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i + 1];
				m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i + 1] = 0;
			}

		m_pClientList[iClientH]->m_iPartyID = NULL;
		m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;



		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_PARTY, 8, 0, NULL, NULL);
		break;

	case 4:
		cResult = *cp;
		cp++;

		wp = (WORD*)cp;
		iClientH = (int)*wp;
		cp += 2;

		ZeroMemory(cName, sizeof(cName));
		memcpy(cName, cp, 10);
		cp += 10;

		wp = (WORD*)cp;
		iPartyID = (int)*wp;
		cp += 2;

		PartyOperationResult_Join(iClientH, cName, cResult, iPartyID);


		break;

	case 5:
		wp = (WORD*)cp;
		iClientH = (int)*wp;
		cp += 2;

		ZeroMemory(cName, sizeof(cName));
		memcpy(cName, cp, 10);
		cp += 10;

		wp = (WORD*)cp;
		iTotal = (int)*wp;
		cp += 2;

		PartyOperationResult_Info(iClientH, cName, iTotal, cp);


		break;

	case 6:
		cResult = *cp;
		cp++;

		wp = (WORD*)cp;
		iClientH = (int)*wp;
		cp += 2;

		ZeroMemory(cName, sizeof(cName));
		memcpy(cName, cp, 10);
		cp += 10;

		wp = (WORD*)cp;
		iPartyID = (int)*wp;
		cp += 2;

		PartyOperationResult_Dismiss(iClientH, cName, cResult, iPartyID);


		break;
	}
}


void CGame::PartyOperationResult_Create(int iClientH, char* pName, int iResult, int iPartyID)
{
	char* cp, cData[120];
	DWORD* dwp;
	WORD* wp;
	int i;

	if (m_pClientList[iClientH] == NULL) return;
	if (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0) return;

	// Prevent creation of a n� 0 party
	if ((iPartyID == 0) && (iResult != 0)) return;

	switch (iResult) {
	case 0: // ÆÄÆ¼ »ý¼º ½ÇÆÐ 
		if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) return;
		if (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0) return;

		m_pClientList[iClientH]->m_iPartyID = NULL;
		m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
		m_pClientList[iClientH]->m_iReqJoinPartyClientH = NULL;

		// SNOOPY added this for safety
		ZeroMemory(m_pClientList[iClientH]->m_cReqJoinPartyName, sizeof(m_pClientList[iClientH]->m_cReqJoinPartyName));

		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_PARTY, 1, 0, NULL, NULL);
		break;

	case 1: // ÆÄÆ¼ »ý¼º ¼º°ø 
		if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) return;
		if (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0) return;

		m_pClientList[iClientH]->m_iPartyID = iPartyID;
		m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_CONFIRM;
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_PARTY, 1, 1, NULL, NULL);

		// °ÔÀÓ ¼­¹öÀÇ ÆÄÆ¼ ¸®½ºÆ®¿¡ µî·Ï.
		for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
			if (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] == 0) {
				m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] = iClientH;
				m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers++;

				break;
			}

		if ((m_pClientList[iClientH]->m_iReqJoinPartyClientH != NULL) && (strlen(m_pClientList[iClientH]->m_cReqJoinPartyName) != NULL)) {
			ZeroMemory(cData, sizeof(cData));

			cp = (char*)cData;

			dwp = (DWORD*)cp;
			*dwp = MSGID_PARTYOPERATION;
			cp += 4;

			wp = (WORD*)cp;
			*wp = 3;
			cp += 2;

			wp = (WORD*)cp;
			*wp = m_pClientList[iClientH]->m_iReqJoinPartyClientH;
			cp += 2;

			memcpy(cp, m_pClientList[iClientH]->m_cReqJoinPartyName, 10);
			cp += 10;

			wp = (WORD*)cp;
			*wp = m_pClientList[iClientH]->m_iPartyID;
			cp += 2;

			SendMsgToGateServer(MSGID_PARTYOPERATION, iClientH, cData);
			m_pClientList[iClientH]->m_iReqJoinPartyClientH = NULL;
			ZeroMemory(m_pClientList[iClientH]->m_cReqJoinPartyName, sizeof(m_pClientList[iClientH]->m_cReqJoinPartyName));
		}
		break;
	}
}

// Last Updated October 28, 2004 - 3.51 translation
void CGame::PartyOperationResult_Join(int iClientH, char* pName, int iResult, int iPartyID)
{
	int i;

	if (m_pClientList[iClientH] == NULL) return;

	// Prevent creation of a n� 0 party
	if ((iPartyID == 0) && (iResult != 0)) return;

	switch (iResult) {
	case 0:
		if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) return;
		if (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0) return;

		m_pClientList[iClientH]->m_iPartyID = NULL;
		m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_PARTY, 4, 0, NULL, pName);

		m_pClientList[iClientH]->m_iReqJoinPartyClientH = NULL;
		ZeroMemory(m_pClientList[iClientH]->m_cReqJoinPartyName, sizeof(m_pClientList[iClientH]->m_cReqJoinPartyName));
		break;

	case 1:
		if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) return;
		if (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0) return;

		m_pClientList[iClientH]->m_iPartyID = iPartyID;
		m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_CONFIRM;
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_PARTY, 4, 1, NULL, pName);

		m_pClientList[iClientH]->m_iReqJoinPartyClientH = NULL;
		ZeroMemory(m_pClientList[iClientH]->m_cReqJoinPartyName, sizeof(m_pClientList[iClientH]->m_cReqJoinPartyName));

		for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
			if (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] == 0) {
				m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] = iClientH;
				m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers++;


				break;
			}

		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((i != iClientH) && (m_pClientList[i] != NULL) && (m_pClientList[i]->m_iPartyID != NULL) && (m_pClientList[i]->m_iPartyID == iPartyID)) {
				SendNotifyMsg(NULL, i, DEF_NOTIFY_PARTY, 4, 1, NULL, pName);
			}
		break;
	}
}

void CGame::PartyOperationResult_Dismiss(int iClientH, char* pName, int iResult, int iPartyID)
{
	int i;
	// iClientH´Â ÀÌ¹Ì »ç¶óÁö°í ¾ø´Â Ä³¸¯ÅÍÀÏ¼öµµ ÀÖ´Ù.

	// Prevent dismissing th 0 party
	if ((iPartyID == 0) && (iResult != 0)) return;

	switch (iResult) {
	case 0: // ÆÄÆ¼ Å»Åð ½ÇÆÐ ? ÀÌ·± ÀÏÀÌ?
		break;

	case 1: // ÆÄÆ¼ Å»Åð ¼º°ø 
		if (iClientH == NULL) {
			// iClientH °¡ NULLÀÌ¸é ¼­¹ö ÀÌµ¿Áß Á¢¼Ó Á¾·áµÇ¾î °­Á¦ Á¦°Å ¿äÃ»µÈ °ÍÀÓ.
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if ((m_pClientList[i] != NULL) && (strcmp(m_pClientList[i]->m_cCharName, pName) == 0)) {
					iClientH = i;
					break;
				}

			// °ÔÀÓ ¼­¹öÀÇ ÆÄÆ¼ ¸®½ºÆ®¿¡¼­ ÇØÁ¦.
			for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
				if (m_stPartyInfo[iPartyID].iIndex[i] == iClientH) {
					m_stPartyInfo[iPartyID].iIndex[i] = 0;
					m_stPartyInfo[iPartyID].iTotalMembers--;

					break;
				}

			// ¸®½ºÆ® ÀÎµ¦½ºÀÇ ºó°ø°£À» Á¦°ÅÇÑ´Ù.
			for (i = 0; i < DEF_MAXPARTYMEMBERS - 1; i++)
				if ((m_stPartyInfo[iPartyID].iIndex[i] == 0) && (m_stPartyInfo[iPartyID].iIndex[i + 1] != 0)) {
					m_stPartyInfo[iPartyID].iIndex[i] = m_stPartyInfo[iPartyID].iIndex[i + 1];
					m_stPartyInfo[iPartyID].iIndex[i + 1] = 0;
				}

			if (m_pClientList[iClientH] != NULL) {
				m_pClientList[iClientH]->m_iPartyID = NULL;
				m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
				m_pClientList[iClientH]->m_iReqJoinPartyClientH = NULL;

				// SNOOPY: Added safety here
				ZeroMemory(m_pClientList[iClientH]->m_cReqJoinPartyName, sizeof(m_pClientList[iClientH]->m_cReqJoinPartyName));
			}

			// ¸ðµç ÆÄÆ¼¿øµé¿¡°Ô ¾Ë·ÁÁØ´Ù.
			for (i = 1; i < DEF_MAXCLIENTS; i++)
				if ((m_pClientList[i] != NULL) && (m_pClientList[i]->m_iPartyID != NULL) && (m_pClientList[i]->m_iPartyID == iPartyID)) {
					SendNotifyMsg(NULL, i, DEF_NOTIFY_PARTY, 6, 1, NULL, pName);
				}
			break;
		}

		if ((m_pClientList[iClientH] != NULL) && (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING)) return;
		if ((m_pClientList[iClientH] != NULL) && (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0)) return;

		// ¸ðµç ÆÄÆ¼¿øµé¿¡°Ô ¾Ë·ÁÁØ´Ù.
		for (i = 1; i < DEF_MAXCLIENTS; i++)
			if ((m_pClientList[i] != NULL) && (m_pClientList[i]->m_iPartyID != NULL) && (m_pClientList[i]->m_iPartyID == iPartyID)) {
				SendNotifyMsg(NULL, i, DEF_NOTIFY_PARTY, 6, 1, NULL, pName);
			}

		// °ÔÀÓ ¼­¹öÀÇ ÆÄÆ¼ ¸®½ºÆ®¿¡¼­ ÇØÁ¦.
		for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
			if (m_stPartyInfo[iPartyID].iIndex[i] == iClientH) {
				m_stPartyInfo[iPartyID].iIndex[i] = 0;
				m_stPartyInfo[iPartyID].iTotalMembers--;

				break;
			}
		// ¸®½ºÆ® ÀÎµ¦½ºÀÇ ºó°ø°£À» Á¦°ÅÇÑ´Ù.
		for (i = 0; i < DEF_MAXPARTYMEMBERS - 1; i++)
			if ((m_stPartyInfo[iPartyID].iIndex[i] == 0) && (m_stPartyInfo[iPartyID].iIndex[i + 1] != 0)) {
				m_stPartyInfo[iPartyID].iIndex[i] = m_stPartyInfo[iPartyID].iIndex[i + 1];
				m_stPartyInfo[iPartyID].iIndex[i + 1] = 0;
			}

		if (m_pClientList[iClientH] != NULL) {
			m_pClientList[iClientH]->m_iPartyID = NULL;
			m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
			m_pClientList[iClientH]->m_iReqJoinPartyClientH = NULL;
			// SNOOPY: Added safety here
			ZeroMemory(m_pClientList[iClientH]->m_cReqJoinPartyName, sizeof(m_pClientList[iClientH]->m_cReqJoinPartyName));
		}
		break;
	}
}

void CGame::PartyOperationResult_Delete(int iPartyID)
{
	int i;

	for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
	{
		m_stPartyInfo[iPartyID].iIndex[i] = 0;
		m_stPartyInfo[iPartyID].iTotalMembers = 0;
	}

	// ÆÄÆ¼ ÇØ»êµÊ 
	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != NULL) && (m_pClientList[i]->m_iPartyID == iPartyID)) {
			SendNotifyMsg(NULL, i, DEF_NOTIFY_PARTY, 2, 0, NULL, NULL);
			m_pClientList[i]->m_iPartyID = NULL;
			m_pClientList[i]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
			m_pClientList[i]->m_iReqJoinPartyClientH = NULL;

			// SNOOPY: Added safety here
			ZeroMemory(m_pClientList[i]->m_cReqJoinPartyName, sizeof(m_pClientList[i]->m_cReqJoinPartyName));

		}
}



void CGame::RequestJoinPartyHandler(int iClientH, char* pData, DWORD dwMsgSize)
{
	char   seps[] = "= \t\n";
	char* cp, * token, cBuff[256], cData[120], cName[12];
	class  CStrTok* pStrTok;
	DWORD* dwp;
	WORD* wp;
	int i;

	// ÆÄÆ¼ °¡ÀÔ ¿©ºÎ¸¦ ¹¯°í ÀÖ´Ù.
	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_NULL) return;
	if ((dwMsgSize) <= 0) return;
	if ((m_bAdminSecurity == TRUE) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0)) return;

	ZeroMemory(cBuff, sizeof(cBuff));
	memcpy(cBuff, pData, dwMsgSize);

	pStrTok = new class CStrTok(cBuff, seps);
	token = pStrTok->pGet();

	token = pStrTok->pGet();
	if (token != NULL) {
		ZeroMemory(cName, sizeof(cName));
		strcpy(cName, token);
	}
	else return;

	for (i = 1; i < DEF_MAXCLIENTS; i++)
		if ((m_pClientList[i] != NULL) && (strcmp(m_pClientList[i]->m_cCharName, cName) == 0)) {
			// °ÔÀÌÆ® ¼­¹ö·Î ÆÄÆ¼ ¸â¹ö °¡ÀÔÀ» ½ÅÃ»ÇÑ´Ù.
			if ((m_pClientList[i]->m_iPartyID == NULL) || (m_pClientList[i]->m_iPartyStatus != DEF_PARTYSTATUS_CONFIRM)) {
				// ÆÄÆ¼¿¡ °¡ÀÔÇÏ°íÀÚ ÇÑ Ä³¸¯ÅÍ°¡ ÆÄÆ¼ ¸¶½ºÅÍ°¡ ¾Æ´Ï´Ù.
				return;
			}

			ZeroMemory(cData, sizeof(cData));

			cp = (char*)cData;
			dwp = (DWORD*)cp;
			*dwp = MSGID_PARTYOPERATION;
			cp += 4;
			wp = (WORD*)cp;
			*wp = 3; // ÆÄÆ¼ ¸â¹ö Ãß°¡ ¿äÃ»
			cp += 2;
			wp = (WORD*)cp;
			*wp = iClientH;
			cp += 2;
			memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
			cp += 10;
			wp = (WORD*)cp;
			*wp = m_pClientList[i]->m_iPartyID;
			cp += 2;
			SendMsgToGateServer(MSGID_PARTYOPERATION, iClientH, cData);
			return;
		}

	// °¡ÀÔÇÏ°íÀÚ ÇÏ´Â ÆÄÆ¼ ¸¶½ºÅÍ°¡ ÇöÀç ¼­¹ö¿¡ Á¸ÀçÇÏÁö ¾Ê´Â´Ù.
	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_PLAYERNOTONGAME, NULL, NULL, NULL, cName);
}



void CGame::RequestDismissPartyHandler(int iClientH)
{
	char* cp, cData[120];
	DWORD* dwp;
	WORD* wp;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_CONFIRM) return;

	ZeroMemory(cData, sizeof(cData));
	cp = (char*)cData;
	dwp = (DWORD*)cp;
	*dwp = MSGID_PARTYOPERATION;
	cp += 4;
	wp = (WORD*)cp;
	*wp = 4; // ¸â¹ö Á¦°Å ¿äÃ»
	cp += 2;
	wp = (WORD*)cp;
	*wp = iClientH;
	cp += 2;
	memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
	cp += 10;
	wp = (WORD*)cp;
	*wp = m_pClientList[iClientH]->m_iPartyID;
	cp += 2;
	SendMsgToGateServer(MSGID_PARTYOPERATION, iClientH, cData);

	m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_PROCESSING;
}


void CGame::GetPartyInfoHandler(int iClientH)
{
	char* cp, cData[120];
	DWORD* dwp;
	WORD* wp;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_CONFIRM) return;

	ZeroMemory(cData, sizeof(cData));
	cp = (char*)cData;
	dwp = (DWORD*)cp;
	*dwp = MSGID_PARTYOPERATION;
	cp += 4;
	wp = (WORD*)cp;
	*wp = 5; // ÆÄÆ¼ Á¤º¸ ¿äÃ»
	cp += 2;
	wp = (WORD*)cp;
	*wp = iClientH;
	cp += 2;
	memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
	cp += 10;
	wp = (WORD*)cp;
	*wp = m_pClientList[iClientH]->m_iPartyID;
	cp += 2;
	SendMsgToGateServer(MSGID_PARTYOPERATION, iClientH, cData);
}


void CGame::PartyOperationResult_Info(int iClientH, char* pName, int iTotal, char* pNameList)
{
	if (m_pClientList[iClientH] == NULL) return;
	if (strcmp(m_pClientList[iClientH]->m_cCharName, pName) != 0) return;
	if (m_pClientList[iClientH]->m_iPartyStatus != DEF_PARTYSTATUS_CONFIRM) return;

	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_PARTY, 5, 1, iTotal, pNameList);
}

void CGame::RequestDeletePartyHandler(int iClientH)
{
	char* cp, cData[120];
	DWORD* dwp;
	WORD* wp;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_iPartyID != NULL) {
		ZeroMemory(cData, sizeof(cData));
		cp = (char*)cData;
		dwp = (DWORD*)cp;
		*dwp = MSGID_PARTYOPERATION;
		cp += 4;
		wp = (WORD*)cp;
		*wp = 4; // ¸â¹ö Á¦°Å ¿äÃ»
		cp += 2;
		wp = (WORD*)cp;
		*wp = iClientH;
		cp += 2;
		memcpy(cp, m_pClientList[iClientH]->m_cCharName, 10);
		cp += 10;
		wp = (WORD*)cp;
		*wp = m_pClientList[iClientH]->m_iPartyID;
		cp += 2;
		SendMsgToGateServer(MSGID_PARTYOPERATION, iClientH, cData);
		// »óÅÂ º¯È¯
		m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_PROCESSING;
	}
}

void CGame::RequestAcceptJoinPartyHandler(int iClientH, int iResult)
{
	char* cp, cData[120];
	DWORD* dwp;
	WORD* wp;
	int iH;

	if (m_pClientList[iClientH] == NULL) return;

	switch (iResult) {
	case 0: // ÆÄÆ¼ °¡ÀÔ ½ÅÃ» °ÅºÎµÇ¾úÀ½À» ¾Ë·ÁÁØ´Ù.
		iH = m_pClientList[iClientH]->m_iReqJoinPartyClientH;
		if (m_pClientList[iH] == NULL) {
			// °¡ÀÔÀ» ½ÅÃ»Çß´ø ÇÃ·¹ÀÌ¾î°¡ ¼­¹ö»ó¿¡ ¾ø´Ù.
			return;
		}
		if (strcmp(m_pClientList[iH]->m_cCharName, m_pClientList[iClientH]->m_cReqJoinPartyName) != 0) {
			// °¡ÀÔÀ» ½ÅÃ»Çß´ø ÇÃ·¹ÀÌ¾î°¡ ¼­¹ö»ó¿¡ ¾ø´Ù.
			return;
		}
		if (m_pClientList[iH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) {
			// °¡ÀÔÀ» ½ÅÃ»Çß´ø ÇÃ·¹ÀÌ¾îÀÇ »óÅÂ°¡ º¯°æµÇ¾ú´Ù.
			return;
		}
		if ((m_pClientList[iH]->m_iReqJoinPartyClientH != iClientH) || (strcmp(m_pClientList[iH]->m_cReqJoinPartyName, m_pClientList[iClientH]->m_cCharName) != 0)) {
			// °¡ÀÔÀ» ½ÅÃ»Çß´ø ±×¶§ ±× ÇÃ·¹ÀÌ¾î°¡ ¾Æ´Ï´Ù.
			return;
		}

		SendNotifyMsg(NULL, iH, DEF_NOTIFY_PARTY, 7, 0, NULL, NULL);


		m_pClientList[iH]->m_iPartyID = NULL;
		m_pClientList[iH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
		m_pClientList[iH]->m_iReqJoinPartyClientH = NULL;
		ZeroMemory(m_pClientList[iH]->m_cReqJoinPartyName, sizeof(m_pClientList[iH]->m_cReqJoinPartyName));

		m_pClientList[iClientH]->m_iReqJoinPartyClientH = NULL;
		ZeroMemory(m_pClientList[iClientH]->m_cReqJoinPartyName, sizeof(m_pClientList[iClientH]->m_cReqJoinPartyName));
		break;

	case 1: // ÆÄÆ¼ °¡ÀÔ ½ÅÃ» ½ÂÀÎ
		if ((m_pClientList[iClientH]->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM) && (m_pClientList[iClientH]->m_iPartyID != NULL)) {
			iH = m_pClientList[iClientH]->m_iReqJoinPartyClientH;
			if (m_pClientList[iH] == NULL) {
				// °¡ÀÔÀ» ½ÅÃ»Çß´ø ÇÃ·¹ÀÌ¾î°¡ ¼­¹ö»ó¿¡ ¾ø´Ù.
				return;
			}
			if (strcmp(m_pClientList[iH]->m_cCharName, m_pClientList[iClientH]->m_cReqJoinPartyName) != 0) {
				// °¡ÀÔÀ» ½ÅÃ»Çß´ø ÇÃ·¹ÀÌ¾î°¡ ¼­¹ö»ó¿¡ ¾ø´Ù.
				return;
			}
			if (m_pClientList[iH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) {
				// °¡ÀÔÀ» ½ÅÃ»Çß´ø ÇÃ·¹ÀÌ¾îÀÇ »óÅÂ°¡ º¯°æµÇ¾ú´Ù.
				return;
			}
			if ((m_pClientList[iH]->m_iReqJoinPartyClientH != iClientH) || (strcmp(m_pClientList[iH]->m_cReqJoinPartyName, m_pClientList[iClientH]->m_cCharName) != 0)) {
				// °¡ÀÔÀ» ½ÅÃ»Çß´ø ±×¶§ ±× ÇÃ·¹ÀÌ¾î°¡ ¾Æ´Ï´Ù.
				return;
			}

			// °¡ÀÔ Ã³¸® ÇÑ´Ù.
			ZeroMemory(cData, sizeof(cData));
			cp = (char*)cData;
			dwp = (DWORD*)cp;
			*dwp = MSGID_PARTYOPERATION;
			cp += 4;
			wp = (WORD*)cp;
			*wp = 3; // ÆÄÆ¼ ¸â¹ö Ãß°¡ ¿äÃ»
			cp += 2;
			wp = (WORD*)cp;
			*wp = m_pClientList[iClientH]->m_iReqJoinPartyClientH;
			cp += 2;
			memcpy(cp, m_pClientList[iClientH]->m_cReqJoinPartyName, 10);
			cp += 10;
			wp = (WORD*)cp;
			*wp = m_pClientList[iClientH]->m_iPartyID;
			cp += 2;
			SendMsgToGateServer(MSGID_PARTYOPERATION, iClientH, cData);
		}
		else {
			iH = m_pClientList[iClientH]->m_iReqJoinPartyClientH;
			if (m_pClientList[iH] == NULL) {
				// °¡ÀÔÀ» ½ÅÃ»Çß´ø ÇÃ·¹ÀÌ¾î°¡ ¼­¹ö»ó¿¡ ¾ø´Ù.
				return;
			}
			if (strcmp(m_pClientList[iH]->m_cCharName, m_pClientList[iClientH]->m_cReqJoinPartyName) != 0) {
				// °¡ÀÔÀ» ½ÅÃ»Çß´ø ÇÃ·¹ÀÌ¾î°¡ ¼­¹ö»ó¿¡ ¾ø´Ù.
				return;
			}
			if (m_pClientList[iH]->m_iPartyStatus != DEF_PARTYSTATUS_PROCESSING) {
				// °¡ÀÔÀ» ½ÅÃ»Çß´ø ÇÃ·¹ÀÌ¾îÀÇ »óÅÂ°¡ º¯°æµÇ¾ú´Ù.
				return;
			}
			if ((m_pClientList[iH]->m_iReqJoinPartyClientH != iClientH) || (strcmp(m_pClientList[iH]->m_cReqJoinPartyName, m_pClientList[iClientH]->m_cCharName) != 0)) {
				// °¡ÀÔÀ» ½ÅÃ»Çß´ø ±×¶§ ±× ÇÃ·¹ÀÌ¾î°¡ ¾Æ´Ï´Ù.
				return;
			}

			if (m_pClientList[iClientH]->m_iPartyStatus == DEF_PARTYSTATUS_NULL) {
				// ½ÅÃ» ¹ÞÀº ÇÃ·¹ÀÌ¾î°¡ ¾Æ¹«·± ÆÄÆ¼¿¡µµ °¡ÀÔÇÏ°í ÀÖÁö ¾Ê´Ù. ÀÌ Ä³¸¯ÅÍ¸¦ ÆÄÆ¼ ¸¶½ºÅÍ·Î ½ÅÃ»ÇÑ´Ù.
				RequestCreatePartyHandler(iClientH);
			}
		}
		break;

	case 2: // ÆÄÆ¼ °¡ÀÔ ½ÅÃ» Ãë¼Ò
		// ÀÌ ¸Þ½ÃÁö¸¦ ¹Þ±â Àü¿¡ ÀÌ¹Ì ÆÄÆ¼¿¡ °¡ÀÔµÇ¾ú´Ù¸é Å»Åð Ã³¸®¸¦ ÇÑ´Ù.
		if ((m_pClientList[iClientH]->m_iPartyID != NULL) && (m_pClientList[iClientH]->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM)) {
			// ¸â¹ö Á¦°Å¸¦ ¿äÃ»ÇÑ´Ù.
			RequestDismissPartyHandler(iClientH);
		}
		else {
			// °¡ÀÔ ½ÅÃ» »óÅÂ¿´´ø °ªµéÀ» ÃÊ±âÈ­ ÇÑ´Ù.
			iH = m_pClientList[iClientH]->m_iReqJoinPartyClientH;

			// ½ÅÃ»Çß´ø Ä³¸¯ÅÍ°¡ Á¢¼ÓÀ» ²÷Àº »óÅÂ¶ó¸é NULLÀÏ ¼ö ÀÖ´Ù.
			if ((m_pClientList[iH] != NULL) && (m_pClientList[iH]->m_iReqJoinPartyClientH == iClientH) &&
				(strcmp(m_pClientList[iH]->m_cReqJoinPartyName, m_pClientList[iClientH]->m_cCharName) == 0)) {
				// ½ÅÃ»Çß´ø Ä³¸¯ÅÍÀÇ ÆÄÆ¼ ¾ÆÀÌµð¿Í »óÅÂ´Â °Çµå¸®Áö ¾Ê´Â´Ù.
				m_pClientList[iH]->m_iReqJoinPartyClientH = NULL;
				ZeroMemory(m_pClientList[iH]->m_cReqJoinPartyName, sizeof(m_pClientList[iH]->m_cReqJoinPartyName));
			}

			m_pClientList[iClientH]->m_iPartyID = NULL;
			m_pClientList[iClientH]->m_iPartyStatus = DEF_PARTYSTATUS_NULL;
			m_pClientList[iClientH]->m_iReqJoinPartyClientH = NULL;
			ZeroMemory(m_pClientList[iClientH]->m_cReqJoinPartyName, sizeof(m_pClientList[iClientH]->m_cReqJoinPartyName));
		}
		break;
	}
}

//New Party Status - ZeroEoyPnk - 06/09/2010
void CGame::RefreshPartyStatus(int iClientH)
{
	int i, HPParty, NotifyClient, PartyId, x, MaxHpParty, TotalMembers, PartySex, MPParty, MaxMpParty;

	NotifyClient = 0;
	HPParty = 0;
	PartySex = 0;
	MaxHpParty = 0;
	TotalMembers = 0;
	MPParty = 0;
	MaxMpParty = 0;
	x = 0;

	if (m_pClientList[iClientH] == NULL) return;

	if ((m_pClientList[iClientH]->m_iPartyID != NULL) && (m_pClientList[iClientH]->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM))
	{
		HPParty = m_pClientList[iClientH]->m_iHP;
		MaxHpParty = iGetMaxHP(iClientH, FALSE);
		MPParty = m_pClientList[iClientH]->m_iMP;
		MaxMpParty = iGetMaxMP(iClientH);
		PartySex = m_pClientList[iClientH]->m_cSex;
		TotalMembers = m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers;
		for (i = 0; i < m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers; i++)
		{
			x++;
			if (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] == iClientH)
			{
				PartyId = x;
			}
		}
		for (i = 0; i < m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers; i++)
		{
			NotifyClient = m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i];
			if (m_pClientList[NotifyClient] != NULL)
			{
				SendNotifyMsg(NULL, NotifyClient, DEF_SEND_PARTYHP, PartyId, HPParty, MaxHpParty, NULL, PartySex, MPParty, MaxMpParty);
			}
		}
	}
}

//LifeX Party Cords
void CGame::RefreshPartyCoords(int iClientH)
{
	int i, x, iH, PartyId;
	char MapName[12];
	int Party_sX, Party_sY;

	iH = 0;
	PartyId = 0;
	Party_sX = 0;
	Party_sY = 0;
	x = 0;

	if (m_pClientList[iClientH] == NULL) return;

	if ((m_pClientList[iClientH]->m_iPartyID != NULL) && (m_pClientList[iClientH]->m_iPartyStatus == DEF_PARTYSTATUS_CONFIRM))
	{
		Party_sX = m_pClientList[iClientH]->m_sX;
		Party_sY = m_pClientList[iClientH]->m_sY;
		ZeroMemory(MapName, sizeof(MapName));
		strcpy(MapName, m_pClientList[iClientH]->m_cMapName);

		for (i = 0; i < m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers; i++)
		{
			x++;
			if (m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i] == iClientH)
			{
				PartyId = x;
			}
		}

		for (i = 0; i < m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iTotalMembers; i++)
		{
			iH = m_stPartyInfo[m_pClientList[iClientH]->m_iPartyID].iIndex[i];

			if (m_pClientList[iH] != NULL)
			{
				SendNotifyMsg(NULL, iH, DEF_SEND_PARTYCOORDS, PartyId, Party_sX, Party_sY, MapName, NULL, NULL);
			}
		}
	}
}