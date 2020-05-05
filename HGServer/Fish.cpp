// Fish.cpp: implementation of the CFish class.
//
//////////////////////////////////////////////////////////////////////

#include "Fish.h"

#pragma warning (disable : 4996 6011 6001 4244 4018 6385 6386 26451 6054 4267 6053 6031)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFish::CFish(char cMapIndex, short sX, short sY, short sType, class CItem * pItem, int iDifficulty)
{
	m_cMapIndex		= cMapIndex;
	m_sX			= sX;
	m_sY			= sY;
	m_sType			= sType;
	m_pItem			= pItem;

	m_sEngagingCount = 0;
	m_iDifficulty    = iDifficulty;
	
	if (m_iDifficulty <= 0) 
		m_iDifficulty = 1;
}

CFish::~CFish()
{
	if (m_pItem != NULL) delete m_pItem;
}

int CGame::iCreateFish(char cMapIndex, short sX, short sY, short sType, class CItem* pItem, int iDifficulty, DWORD dwLastTime)
{
	register int i, iDynamicHandle;

	// Â¸ÃŠÃ€Ã‡ Ã€Â§Ã„Â¡Â°Â¡ Â¹Â°Ã€ÃÃÃ¶ ÃˆÂ®Ã€ÃÃ‡Ã‘Â´Ã™. 
	if ((cMapIndex < 0) || (cMapIndex >= DEF_MAXMAPS)) return NULL;
	if (m_pMapList[cMapIndex] == NULL) return NULL;
	if (m_pMapList[cMapIndex]->bGetIsWater(sX, sY) == FALSE) return NULL;

	for (i = 0; i < DEF_MAXFISHS; i++)
		if (m_pFish[i] == NULL) {
			// ÂºÃ³ Â°Ã¸Â°Â£Â¿Â¡ Â¹Â°Â°Ã­Â±Ã¢Â¸Â¦ Â¸Â¸ÂµÃ§Â´Ã™.
			m_pFish[i] = new class CFish(cMapIndex, sX, sY, sType, pItem, iDifficulty);
			if (m_pFish[i] == NULL) return NULL;

			// Dynamic ObjectÂ¸Â¦ Â¹ÃŸÂ»Ã½Â½ÃƒÃ…Â²Â´Ã™. OwnerÂ¿Â¡ Fish Ã€ÃÂµÂ¦Â½ÂºÂ¸Â¦ Â³Ã–Â´Ã‚Â´Ã™.
			switch (pItem->m_sIDnum) {
			case 101: // Â»Â¡Â°Â­Â¹Â°Â°Ã­Â±Ã¢
			case 102: // ÃƒÃŠÂ·ÃÂ¹Â°Â°Ã­Â±Ã¢
			case 103: // Â³Ã«Â¶Ã»Â¹Â°Â°Ã­Â±Ã¢
			case 570:
			case 571:
			case 572:
			case 573:
			case 574:
			case 575:
			case 576:
			case 577:
				iDynamicHandle = iAddDynamicObjectList(i, NULL, DEF_DYNAMICOBJECT_FISH, cMapIndex, sX, sY, dwLastTime);
				break;
			default:
				// Â¹Â°Â°Ã­Â±Ã¢Â°Â¡ Â¾Ã†Â´Ã‘ Â´Ã™Â¸Â¥ Â¾Ã†Ã€ÃŒÃ…Ã› 
				iDynamicHandle = iAddDynamicObjectList(i, NULL, DEF_DYNAMICOBJECT_FISHOBJECT, cMapIndex, sX, sY, dwLastTime);
				break;
			}

			if (iDynamicHandle == NULL) {
				delete m_pFish[i];
				m_pFish[i] = NULL;
				return NULL;
			}
			m_pFish[i]->m_sDynamicObjectHandle = iDynamicHandle;
			m_pMapList[cMapIndex]->m_iCurFish++;

			return i;
		}

	return NULL;
}


BOOL CGame::bDeleteFish(int iHandle, int iDelMode)
{
	int i, iH;
	DWORD dwTime;

	if (m_pFish[iHandle] == NULL) return FALSE;

	dwTime = timeGetTime();

	// Â¿Â¬Â°Ã¼ÂµÃ‡Â¾Ã® Ã€Ã–Â´Ã‚ DynamicObjectÂ¸Â¦ Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
	iH = m_pFish[iHandle]->m_sDynamicObjectHandle;

	if (m_pDynamicObjectList[iH] != NULL) {
		SendEventToNearClient_TypeB(MSGID_DYNAMICOBJECT, DEF_MSGTYPE_REJECT, m_pDynamicObjectList[iH]->m_cMapIndex, m_pDynamicObjectList[iH]->m_sX, m_pDynamicObjectList[iH]->m_sY, m_pDynamicObjectList[iH]->m_sType, iH, NULL);
		// Â¸ÃŠÂ¿Â¡Â¼Â­ Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
		m_pMapList[m_pDynamicObjectList[iH]->m_cMapIndex]->SetDynamicObject(NULL, NULL, m_pDynamicObjectList[iH]->m_sX, m_pDynamicObjectList[iH]->m_sY, dwTime);
		m_pMapList[m_pDynamicObjectList[iH]->m_cMapIndex]->m_iCurFish--;

		delete m_pDynamicObjectList[iH];
		m_pDynamicObjectList[iH] = NULL;
	}

	// Ã€ÃŒ Â¹Â°Â°Ã­Â±Ã¢Â¿Ã Â¿Â¬Â°Ã¡ÂµÃ‡Â¾Ã® Ã€Ã–Â´Ã‚ Ã‡ÃƒÂ·Â¹Ã€ÃŒÂ¾Ã®ÂµÃ©Â¿Â¡Â°Ã” Â¹Â°Â°Ã­Â±Ã¢Â°Â¡ Â»Ã§Â¶Ã³ÃÂ® Â³Â¬Â½ÃƒÂ°Â¡ ÃƒÃ«Â¼Ã’ÂµÃ‡Â¾ÃºÃ€Â½Ã€Â» Â¾Ã‹Â·ÃÃÃ˜Â´Ã™. 
	for (i = 1; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != NULL) && (m_pClientList[i]->m_bIsInitComplete == TRUE) &&
			(m_pClientList[i]->m_iAllocatedFish == iHandle)) {
			// Â¸ÃÂ½ÃƒÃÃ¶ Ã€Ã¼Â¼Ã› 
			SendNotifyMsg(NULL, i, DEF_NOTIFY_FISHCANCELED, iDelMode, NULL, NULL, NULL);
			ClearSkillUsingStatus(i); // v1.4 Â³Â¬Â½Ãƒ Â½ÂºÃ…Â³Ã€Â» Ã…Â¬Â¸Â®Â¾Ã®Ã‡Ã‘Â´Ã™.
		}
	}

	// Â»Ã¨ÃÂ¦Ã‡Ã‘Â´Ã™.
	delete m_pFish[iHandle];
	m_pFish[iHandle] = NULL;

	return TRUE;
}


int CGame::iCheckFish(int iClientH, char cMapIndex, short dX, short dY)
{
	int i;
	short sDistX, sDistY;

	if (m_pClientList[iClientH] == NULL) return 0;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return 0;

	if ((cMapIndex < 0) || (cMapIndex >= DEF_MAXMAPS)) return 0;

	// Â¸ÃŠÃ€Ã‡ Ã†Â¯ÃÂ¤ Ã€Â§Ã„Â¡ Â³Â»Â¿Â¡ Â¹Â°Â°Ã­Â±Ã¢ Â´Ã™Ã€ÃŒÂ³ÂªÂ¹Ã Â¿Ã€ÂºÃªÃÂ§Ã†Â®Â°Â¡ Ã€Ã–Â´Ã‚ÃÃ¶ Ã†Ã‡Â´ÃœÃ‡Ã‘Â´Ã™. 
	for (i = 1; i < DEF_MAXDYNAMICOBJECTS; i++)
		if (m_pDynamicObjectList[i] != NULL) {
			sDistX = abs(m_pDynamicObjectList[i]->m_sX - dX);
			sDistY = abs(m_pDynamicObjectList[i]->m_sY - dY);

			if ((m_pDynamicObjectList[i]->m_cMapIndex == cMapIndex) &&
				((m_pDynamicObjectList[i]->m_sType == DEF_DYNAMICOBJECT_FISH) || (m_pDynamicObjectList[i]->m_sType == DEF_DYNAMICOBJECT_FISHOBJECT)) &&
				(sDistX <= 2) && (sDistY <= 2)) {
				// Â¹Â°Â°Ã­Â±Ã¢ Â´Ã™Ã€ÃŒÂ³ÂªÂ¹Ã Â¿Ã€ÂºÃªÃÂ§Ã†Â®Â¸Â¦ ÃƒÂ£Â¾Ã’Â´Ã™. Ã€ÃŒÃÂ¦ Ã€ÃŒ Â´Ã™Ã€ÃŒÂ³ÂªÂ¹Ã Â¿Ã€ÂºÃªÃÂ§Ã†Â® Ã€ÃÂµÂ¦Â½ÂºÂ¸Â¦ Â¼Ã’Ã€Â¯Ã‡Ã‘ FishÃ€Ã‡ Ã€ÃÂµÂ¦Â½ÂºÂ¸Â¦ Â¹ÃÃˆÂ¯Ã‡Ã‘Â´Ã™. 

				if (m_pFish[m_pDynamicObjectList[i]->m_sOwner] == NULL) return 0;
				if (m_pFish[m_pDynamicObjectList[i]->m_sOwner]->m_sEngagingCount >= DEF_MAXENGAGINGFISH) return 0;

				// Ã€ÃŒÂ¹ÃŒ Â³Â¬Â½ÃƒÂ¸Ã°ÂµÃ¥Â¿Â¡ ÂµÃ©Â¾Ã®Â°Â¡ Ã€Ã–Â´Ã‚ Ã„Â³Â¸Â¯Ã…ÃÂ´Ã‚ ÃÃŸÂºÂ¹ ÂµÂ¿Ã€Ã›Ã€ÃŒ ÂºÃ’Â°Â¡Â´Ã‰.
				if (m_pClientList[iClientH]->m_iAllocatedFish != NULL) return 0;
				if (m_pClientList[iClientH]->m_cMapIndex != cMapIndex) return 0;
				// Ã€ÃŒÃÂ¦ Ã„Â³Â¸Â¯Ã…ÃÂ¿Â¡Â°Ã” Â³Â¬Â½Ãƒ Â¸Ã°ÂµÃ¥Â¸Â¦ Â½ÃƒÃ€Ã›Ã‡Ã’ Â°ÃÃ€Â» Ã‡Ã’Â´Ã§Ã‡Ã‘Â´Ã™.
				m_pClientList[iClientH]->m_iAllocatedFish = m_pDynamicObjectList[i]->m_sOwner;
				m_pClientList[iClientH]->m_iFishChance = 1;
				// Ã€ÃŒ Ã„Â³Â¸Â¯Ã…ÃÂ´Ã‚ Â³Â¬Â½Ãƒ Â½ÂºÃ…Â³Ã€Â» Â»Ã§Â¿Ã«ÃÃŸÃ€Ã“Ã€Â» Â¼Â³ÃÂ¤.
				m_pClientList[iClientH]->m_bSkillUsingStatus[1] = TRUE;

				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_EVENTFISHMODE, (m_pFish[m_pDynamicObjectList[i]->m_sOwner]->m_pItem->m_wPrice / 2), m_pFish[m_pDynamicObjectList[i]->m_sOwner]->m_pItem->m_sSprite,
					m_pFish[m_pDynamicObjectList[i]->m_sOwner]->m_pItem->m_sSpriteFrame, m_pFish[m_pDynamicObjectList[i]->m_sOwner]->m_pItem->m_cName);

				// Ã„Â«Â¿Ã®Ã†Â® ÃÃµÂ°Â¡ 
				m_pFish[m_pDynamicObjectList[i]->m_sOwner]->m_sEngagingCount++;

				return i;
			}
		}

	return 0;
}

void CGame::FishProcessor()
{
	int i, iSkillLevel, iResult, iChangeValue;

	// ÀÌº¥Æ® ³¬½Ã ¸ğµå°¡ ÇÒ´çµÈ ÇÃ·¹ÀÌ¾îµéÀ» Ã³¸®ÇÑ´Ù.
	for (i = 1; i < DEF_MAXCLIENTS; i++) {
		if ((m_pClientList[i] != NULL) && (m_pClientList[i]->m_bIsInitComplete == TRUE) &&
			(m_pClientList[i]->m_iAllocatedFish != NULL)) {

			if (m_pFish[m_pClientList[i]->m_iAllocatedFish] == NULL) continue;

			// ÀÌ ÇÃ·¹ÀÌ¾î¿¡°Ô ÇÒ´çµÈ ¹°°í±â°¡ ÀÖ´Ù. ³¬À» È®·üÀ» Àç°è»êÇÏ¿© Åëº¸ÇØ ÁØ´Ù. 
			// ³¬½Ã ±â¼úÀÇ ½ºÅ³ 
			iSkillLevel = m_pClientList[i]->m_cSkillMastery[1];
			// ¹°°í±âÀÇ ³­ÀÌµµ¿¡ µû¶ó ½ºÅ³ ·¹º§ÀÌ ¶³¾îÁø´Ù. 
			iSkillLevel -= m_pFish[m_pClientList[i]->m_iAllocatedFish]->m_iDifficulty;
			if (iSkillLevel <= 0) iSkillLevel = 1;

			iChangeValue = iSkillLevel / 10;
			if (iChangeValue <= 0) iChangeValue = 1;
			iChangeValue = iDice(1, iChangeValue);

			iResult = iDice(1, 100);
			if (iSkillLevel > iResult) {
				// ½ºÅ³ ÁÖ»çÀ§ ±¼¸² ¼º°ø. ³¬À» È®·ü Áõ°¡.
				m_pClientList[i]->m_iFishChance += iChangeValue;
				if (m_pClientList[i]->m_iFishChance > 99) m_pClientList[i]->m_iFishChance = 99;

				SendNotifyMsg(NULL, i, DEF_NOTIFY_FISHCHANCE, m_pClientList[i]->m_iFishChance, NULL, NULL, NULL);
			}
			else {
				// ½ºÅ³ ÁÖ»çÀ§ ±¼¸² ½ÇÆĞ. ³¬À» È®·ü °¨¼Ò.	
				m_pClientList[i]->m_iFishChance -= iChangeValue;
				if (m_pClientList[i]->m_iFishChance < 1) m_pClientList[i]->m_iFishChance = 1;

				SendNotifyMsg(NULL, i, DEF_NOTIFY_FISHCHANCE, m_pClientList[i]->m_iFishChance, NULL, NULL, NULL);
			}
		}
	}
}

void CGame::ReqGetFishThisTimeHandler(int iClientH)
{
	int iResult, iFishH;
	class CItem* pItem;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;
	if (m_pClientList[iClientH]->m_iAllocatedFish == NULL) return;
	if (m_pFish[m_pClientList[iClientH]->m_iAllocatedFish] == NULL) return;

	// ³¬½Ã ½ºÅ³ »ç¿ëÁßÀÓÀ» ÇØÁ¦½ÃÅ²´Ù.
	m_pClientList[iClientH]->m_bSkillUsingStatus[1] = FALSE;

	iResult = iDice(1, 100);
	if (m_pClientList[iClientH]->m_iFishChance >= iResult) {
		// ÀÌ ¹°°í±â¸¦ ³¬´Âµ¥ ¼º°øÇÏ¿´´Ù!

		// °æÇèÄ¡ Áõ°¡ 
		GetExp(iClientH, iDice(m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_iDifficulty, 5));
		// ½ºÅ³ Áõ°¡ 
		CalculateSSN_SkillIndex(iClientH, 1, m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_iDifficulty);

		// ¾ÆÀÌÅÛ Æ÷ÀÎÅÍ¸¦ ¾ò¾î¿Â´Ù.
		pItem = m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_pItem;
		m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_pItem = NULL;

		// ³¬Àº ¾ÆÀÌÅÛÀ» ¹Ù´Ú¿¡ ¶³¾î¶ß¸°´Ù.
		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
			m_pClientList[iClientH]->m_sY,
			pItem);

		// ´Ù¸¥ Å¬¶óÀÌ¾ğÆ®¿¡°Ô ¾ÆÀÌÅÛÀÌ ¶³¾îÁø °ÍÀ» ¾Ë¸°´Ù. 
		SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
			m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
			pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color

// ³¬½Ã ¼º°ø ¸Ş½ÃÁö Àü¼Û 
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_FISHSUCCESS, NULL, NULL, NULL, NULL);
		iFishH = m_pClientList[iClientH]->m_iAllocatedFish;
		m_pClientList[iClientH]->m_iAllocatedFish = NULL;

		// ¹°°í±â¸¦ Áö¿ì°í ¸®ÅÏ 
		bDeleteFish(iFishH, 1); // <- ¿©±â¼­ ´Ù¸¥ ³¬½Ã²Ûµé¿¡°Ô ¸Ş½ÃÁö°¡ Àü¼ÛµÉ °ÍÀÌ´Ù.
		return;
	}

	// ³¬´Âµ¥ ½ÇÆĞ! 
	m_pFish[m_pClientList[iClientH]->m_iAllocatedFish]->m_sEngagingCount--;
	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_FISHFAIL, NULL, NULL, NULL, NULL);

	m_pClientList[iClientH]->m_iAllocatedFish = NULL;
}


void CGame::FishGenerator()
{
	int i, iP, tX, tY, iRet;
	char  cItemName[21];
	short sDifficulty;
	DWORD dwLastTime;
	class CItem* pItem;

	for (i = 0; i < DEF_MAXMAPS; i++) {
		if ((iDice(1, 10) == 5) && (m_pMapList[i] != NULL) &&
			(m_pMapList[i]->m_iCurFish < m_pMapList[i]->m_iMaxFish)) {

			iP = iDice(1, m_pMapList[i]->m_iTotalFishPoint) - 1;
			if ((m_pMapList[i]->m_FishPointList[iP].x == -1) || (m_pMapList[i]->m_FishPointList[iP].y == -1)) break;

			tX = m_pMapList[i]->m_FishPointList[iP].x + (iDice(1, 3) - 2);
			tY = m_pMapList[i]->m_FishPointList[iP].y + (iDice(1, 3) - 2);

			pItem = new class CItem;
			if (pItem == NULL) break;

			// Â¸Â¸ÂµÃ© Â¹Â°Â°Ã­Â±Ã¢ ÃÂ¾Â·Ã¹Â¿Ã Â³Â­Ã€ÃŒÂµÂµ, ÃÃ¶Â¼Ã“ Â½ÃƒÂ°Â£Ã€Â» Â°Ã¡ÃÂ¤Ã‡Ã‘Â´Ã™. 
			ZeroMemory(cItemName, sizeof(cItemName));
			switch (iDice(1, 9)) {
			case 1:   strcpy(cItemName, "RedCarp"); sDifficulty = iDice(1, 10) + 20; break;
			case 2:   strcpy(cItemName, "GreenCarp"); sDifficulty = iDice(1, 5) + 10; break;
			case 3:   strcpy(cItemName, "GoldCarp"); sDifficulty = iDice(1, 10) + 1;  break;
			case 4:   strcpy(cItemName, "CrucianCarp"); sDifficulty = 1;  break;
			case 5:   strcpy(cItemName, "BlueSeaBream"); sDifficulty = iDice(1, 15) + 1;  break;
			case 6:   strcpy(cItemName, "RedSeaBream"); sDifficulty = iDice(1, 18) + 1;  break;
			case 7:   strcpy(cItemName, "Salmon"); sDifficulty = iDice(1, 12) + 1;  break;
			case 8:   strcpy(cItemName, "GrayMullet"); sDifficulty = iDice(1, 10) + 1;  break;
			case 9:
				// Â°Â¡Â²Ã» Â°Â¡Â´Ã™ Â³Â¬Ã€Â» Â¼Ã¶ Ã€Ã–Â´Ã‚ Ã†Â¯Â¼Ã¶ Â¾Ã†Ã€ÃŒÃ…Ã› 
				switch (iDice(1, 150)) {
				case 1:
				case 2:
				case 3:
					strcpy(cItemName, "PowerGreenPotion");
					sDifficulty = iDice(5, 4) + 30;
					break;

				case 10:
				case 11:
					strcpy(cItemName, "SuperGreenPotion");
					sDifficulty = iDice(5, 4) + 50;
					break;

				case 20:
					strcpy(cItemName, "Dagger+2");
					sDifficulty = iDice(5, 4) + 30;
					break;

				case 30:
					strcpy(cItemName, "LongSword+2");
					sDifficulty = iDice(5, 4) + 40;
					break;

				case 40:
					strcpy(cItemName, "Scimitar+2");
					sDifficulty = iDice(5, 4) + 50;
					break;

				case 50:
					strcpy(cItemName, "Rapier+2");
					sDifficulty = iDice(5, 4) + 60;
					break;

				case 60:
					strcpy(cItemName, "Flameberge+2");
					sDifficulty = iDice(5, 4) + 60;
					break;

				case 70:
					strcpy(cItemName, "WarAxe+2");
					sDifficulty = iDice(5, 4) + 50;
					break;

				case 90:
					strcpy(cItemName, "Ruby");
					sDifficulty = iDice(5, 4) + 40;
					break;

				case 95:
					strcpy(cItemName, "Diamond");
					sDifficulty = iDice(5, 4) + 40;
					break;
				}
				break;
			}
			dwLastTime = (60000 * 10) + (iDice(1, 3) - 1) * (60000 * 10);

			if (_bInitItemAttr(pItem, cItemName) == TRUE) {
				iRet = iCreateFish(i, tX, tY, 1, pItem, sDifficulty, dwLastTime);
			}
			else {
				delete pItem;
				pItem = NULL;
			}
		}
	}
}