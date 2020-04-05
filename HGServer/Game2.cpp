// Game2.cpp: implementation of the CGame class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"

void CGame::RequestCheckFlag(int iClientH)
{
	if (!m_bIsCTFMode) return;
	if (m_pClientList[iClientH] == NULL) return;

	if (m_pClientList[iClientH]->m_cSide == 0) RequestTeleportHandler(iClientH, "2  ", "default", -1, -1);

	if ((m_pClientList[iClientH]->m_sX == 151) && (m_pClientList[iClientH]->m_sY == 132))//Elvine Flag Cords
	{
		if (m_pClientList[iClientH]->m_iLevel >= 120)
		{
			if (m_pClientList[iClientH]->m_cSide == 1) //Aresden Side
			{
				if ((!m_bIsAresdenFlagCaptured) && (!m_bIsElvineFlagCaptured)) //Aresden Capture Elvine Flag
				{
					PutLogList("Capture The Flag: Aresden Captured Elvine Flag!");
					SetFlagCarrierFlag(iClientH, true);

				}
			}
			else //Elvine Side
			{
				if ((m_bIsAresdenFlagCaptured) && (!m_bIsElvineFlagCaptured))
				{
					PutLogList("Capture The Flag: Elvine wins this round!");
					m_sElvineFlagCount++;
					m_bIsAresdenFlagCaptured = false;
					for (int i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != NULL) SendNotifyMsg(NULL, i, DEF_NOTIFY_ELVINEWINSROUND, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
					if (m_sElvineFlagCount == 5)
					{
						EndCaptureTheFlag(2);
					}
				}
			}

		}
		else RequestTeleportHandler(iClientH, "2  ", "elvine", -1, -1);
	}
	else if ((m_pClientList[iClientH]->m_sX == 151) && (m_pClientList[iClientH]->m_sY == 128))//Aresden Flag Cords
	{
		if (m_pClientList[iClientH]->m_iLevel >= 120)
		{
			if (m_pClientList[iClientH]->m_cSide == 2) //Elvine Side
			{
				if ((!m_bIsElvineFlagCaptured) && (!m_bIsAresdenFlagCaptured)) //Elvine Capture Aresden Flag
				{
					PutLogList("Capture The Flag: Elvine Captured Aresden Flag!");
					SetFlagCarrierFlag(iClientH, true);
				}
			}
			else //Aresden Side
			{
				if ((m_bIsElvineFlagCaptured) && (!m_bIsAresdenFlagCaptured))
				{
					PutLogList("Capture The Flag: Aresden wins this round!");
					m_sAresdenFlagCount++;
					m_bIsElvineFlagCaptured = false;
					for (int i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != NULL) SendNotifyMsg(NULL, i, DEF_NOTIFY_ARESDENWINSROUND, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
					if (m_sAresdenFlagCount == 5)
					{
						EndCaptureTheFlag(1);
					}
				}
			}
		}
		else RequestTeleportHandler(iClientH, "2  ", "aresden", -1, -1);
	}
}

void CGame::bCalculateEnduranceDecrement(short sTargetH, short sAttackerH, char cTargetType, int iArmorType)
{
	int iDownValue, iHammerChance, iItemIndex;

	iHammerChance = 100;
	iDownValue = 1;
	if (m_pClientList[sTargetH] == NULL) return;

	if ((cTargetType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH] == NULL)) return;
	if ((cTargetType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sTargetH]->m_cSide != m_pClientList[sAttackerH]->m_cSide)) 
	{
		switch (m_pClientList[sAttackerH]->m_sUsingWeaponSkill) 
		{
		case 14:
			if ((((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4) == 31) ||
				(((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4) == 32)) {
				iItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
				if ((iItemIndex != -1) && (m_pClientList[sAttackerH]->m_pItemList[iItemIndex] != NULL)) 
				{
					if (m_pClientList[sAttackerH]->m_pItemList[iItemIndex]->m_sIDnum == 761) { // BattleHammer 
						iDownValue = 30;
						break;
					}
					else if (m_pClientList[sAttackerH]->m_pItemList[iItemIndex]->m_sIDnum == 762) { // GiantBattleHammer
						iDownValue = 35;
						break;
					}
					else if (m_pClientList[sAttackerH]->m_pItemList[iItemIndex]->m_sIDnum == 843) { // BarbarianHammer
						iDownValue = 30;
						break;
					}
				}
			}
			iDownValue = 20;
			break;
		case 10:
			iDownValue = 3;
			break;
		default:
			iDownValue = 1;
			break;
		}
	}
	if (m_pClientList[sTargetH]->m_bIsSpecialAbilityEnabled == TRUE) {
		switch (m_pClientList[sTargetH]->m_iSpecialAbilityType) {
		case 52:
			iDownValue = 0;
			iHammerChance = 0;
			break;
		}
	}
	if ((m_pClientList[sTargetH]->m_cSide != 0) && (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan > 0)) {
		m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan -= iDownValue;
	}
	if (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan <= 0) {
		m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan = 0;
		SendNotifyMsg(NULL, sTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_cEquipPos, iArmorType, NULL, NULL);
		ReleaseItemHandler(sTargetH, iArmorType, TRUE);
		return;
	}
	if ((cTargetType == DEF_OWNERTYPE_PLAYER) && (m_pClientList[sAttackerH]->m_sUsingWeaponSkill == 14) && (iHammerChance == 100)) {
		if (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wMaxLifeSpan < 2000) {
			iHammerChance = iDice(6, (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wMaxLifeSpan - m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan));
		}
		else {
			iHammerChance = iDice(4, (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wMaxLifeSpan - m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan));
		}
		if ((((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4) == 31) ||
			(((m_pClientList[sAttackerH]->m_sAppr2 & 0x0FF0) >> 4) == 32)) {
			iItemIndex = m_pClientList[sAttackerH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND];
			if ((iItemIndex != -1) && (m_pClientList[sAttackerH]->m_pItemList[iItemIndex] != NULL)) {
				if (m_pClientList[sAttackerH]->m_pItemList[iItemIndex]->m_sIDnum == 761) { // BattleHammer 
					iHammerChance = iHammerChance / 2;
				}
				if (m_pClientList[sAttackerH]->m_pItemList[iItemIndex]->m_sIDnum == 762) { // GiantBattleHammer
					iHammerChance -= (5 * iHammerChance) / 8;
				}
				if (m_pClientList[sAttackerH]->m_pItemList[iItemIndex]->m_sIDnum == 843) { // BarbarianHammer
					iHammerChance = iHammerChance / 2;
				}
			}
			else {
				iHammerChance = iHammerChance / 4;
			}
			switch (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_sIDnum) {
			case 621:
			case 622:
				iHammerChance = 0;
				break;
			}
			if (m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_wCurLifeSpan > iHammerChance) {
				ReleaseItemHandler(sTargetH, iArmorType, TRUE);
				SendNotifyMsg(NULL, sTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[sTargetH]->m_pItemList[iArmorType]->m_cEquipPos, iArmorType, NULL, NULL);
			}
		}
	}
}

// Wanted System
void CGame::SetWantedFlag(short sOwnerH, char cOwnerType, int iStatus)
{
	if (cOwnerType != DEF_OWNERTYPE_PLAYER) return;
	if (m_pClientList[sOwnerH] == NULL) return;

	switch (iStatus) {
	case 1: // Set
		m_pClientList[sOwnerH]->m_iStatus = (m_pClientList[sOwnerH]->m_iStatus & 0xFFFF0FFF) | (m_pClientList[sOwnerH]->m_iWantedLevel << 12);
		break;
	default: // Remove
		m_pClientList[sOwnerH]->m_iStatus = m_pClientList[sOwnerH]->m_iStatus & 0xFFFF3FFF;
		break;
	}

	SendEventToNearClient_TypeA(sOwnerH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, NULL, NULL, NULL);
}