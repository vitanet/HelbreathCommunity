// Item.cpp: implementation of the CItem class.
//
//////////////////////////////////////////////////////////////////////

#include "Item.h"

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

CItem::CItem()
{
	ZeroMemory(m_cName, sizeof(m_cName));
	m_sSprite = 0;
	m_sSpriteFrame = 0;
											  
	m_sItemEffectValue1 = 0;
	m_sItemEffectValue2 = 0;
	m_sItemEffectValue3 = 0; 

	m_sItemEffectValue4 = 0;
	m_sItemEffectValue5 = 0;
	m_sItemEffectValue6 = 0; 

	m_dwCount = 1;
	m_sTouchEffectType   = 0;
	m_sTouchEffectValue1 = 0;
	m_sTouchEffectValue2 = 0;
	m_sTouchEffectValue3 = 0;
	
	m_cItemColor = 0;
	m_sItemSpecEffectValue1 = 0;
	m_sItemSpecEffectValue2 = 0;
	m_sItemSpecEffectValue3 = 0;

	m_sSpecialEffectValue1 = 0;
	m_sSpecialEffectValue2 = 0;
	
	m_wCurLifeSpan = 0;
	m_dwAttribute   = 0;

	m_cCategory = NULL;
	m_sIDnum    = 0;

	m_bIsForSale = FALSE;
}

CItem::~CItem()
{

}

void CGame::ItemDepleteHandler(int iClientH, short sItemIndex, BOOL bIsUseItemResult, BOOL bIsItemUsed)
{
	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == NULL) return;
	if ((bIsItemUsed == 1) ||
		(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
		(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_EAT) ||
		(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE) ||
		(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE_DEST) ||
		(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_MATERIAL) ||
		(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 380) ||
		(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 381) ||
		(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 382)) 
	{
		_bItemLog(DEF_ITEMLOG_DEPLETE, iClientH, -1, m_pClientList[iClientH]->m_pItemList[sItemIndex], FALSE);
	}
	else if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 247) ||
		(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 248)) 
	{
		_bItemLog(DEF_ITEMLOG_DEPLETE, iClientH, -1, m_pClientList[iClientH]->m_pItemList[sItemIndex], FALSE);
	}

	ReleaseItemHandler(iClientH, sItemIndex, TRUE);
	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMDEPLETED_ERASEITEM, sItemIndex, (int)bIsUseItemResult, NULL, NULL);
	delete m_pClientList[iClientH]->m_pItemList[sItemIndex];
	m_pClientList[iClientH]->m_pItemList[sItemIndex] = NULL;
	m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] = FALSE;
	m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);
	iCalcTotalWeight(iClientH);
}

void CGame::CalcTotalItemEffect(int iClientH, int iEquipItemID, BOOL bNotify)
{
	short sItemIndex;
	int  i, iPrevSAType, iTemp;
	char cEquipPos;
	double dV1, dV2, dV3;
	DWORD  dwSWEType, dwSWEValue;
	short  sTemp;
	int iShieldHPrec, iShieldSPrec, iShieldMPrec, iTotalAngelicstatas;

	iTotalAngelicstatas = m_pClientList[iClientH]->m_iAngelicStr + (16 * m_pClientList[iClientH]->m_iAngelicInt) + (256 * m_pClientList[iClientH]->m_iAngelicDex) + (16 * 256 * m_pClientList[iClientH]->m_iAngelicMag);

	iShieldHPrec = iShieldSPrec = iShieldMPrec = 0;

	if (m_pClientList[iClientH] == NULL) return;

	if ((m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1) &&
		(m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1)) {

		// �߸��� ���� ���� �����̴�. �� �� �ϳ��� ���� ���´�. 
		if (m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND]] != NULL) {
			// �Ѽ� ���� ���� ���¸� �����Ѵ�. 
			m_pClientList[iClientH]->m_bIsItemEquipped[m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND]] = FALSE;
			m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] = -1;
		}
	}

	m_pClientList[iClientH]->m_iAngelicStr = 0; // By Snoopy81
	m_pClientList[iClientH]->m_iAngelicInt = 0; // By Snoopy81
	m_pClientList[iClientH]->m_iAngelicDex = 0; // By Snoopy81
	m_pClientList[iClientH]->m_iAngelicMag = 0; // By Snoopy81 
	SetAngelFlag(iClientH, DEF_OWNERTYPE_PLAYER, 0, 0);

	if (m_pClientList[iClientH]->m_iWantedLevel > 0) SetWantedFlag(iClientH, DEF_OWNERTYPE_PLAYER, 1); // Wanted System

	m_pClientList[iClientH]->m_cAttackDiceThrow_SM = 0;
	m_pClientList[iClientH]->m_cAttackDiceRange_SM = 0;
	m_pClientList[iClientH]->m_cAttackBonus_SM = 0;

	m_pClientList[iClientH]->m_cAttackDiceThrow_L = 0;
	m_pClientList[iClientH]->m_cAttackDiceRange_L = 0;
	m_pClientList[iClientH]->m_cAttackBonus_L = 0;

	m_pClientList[iClientH]->m_iHitRatio = 0;

	m_pClientList[iClientH]->m_iDefenseRatio = (m_pClientList[iClientH]->m_iDex * 2);

	m_pClientList[iClientH]->m_iDamageAbsorption_Shield = 0;

	for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++)
		m_pClientList[iClientH]->m_iDamageAbsorption_Armor[i] = 0;

	m_pClientList[iClientH]->m_iManaSaveRatio = 0;
	m_pClientList[iClientH]->m_iAddResistMagic = 0;

	m_pClientList[iClientH]->m_iAddPhysicalDamage = 0;
	m_pClientList[iClientH]->m_iAddMagicalDamage = 0;

	m_pClientList[iClientH]->m_bIsLuckyEffect = FALSE;
	m_pClientList[iClientH]->m_iMagicDamageSaveItemIndex = -1;
	m_pClientList[iClientH]->m_iSideEffect_MaxHPdown = 0;

	m_pClientList[iClientH]->m_iAddAbsAir = 0;	// �Ӽ��� ����� ����
	m_pClientList[iClientH]->m_iAddAbsEarth = 0;
	m_pClientList[iClientH]->m_iAddAbsFire = 0;
	m_pClientList[iClientH]->m_iAddAbsWater = 0;

	m_pClientList[iClientH]->m_iCustomItemValue_Attack = 0;
	m_pClientList[iClientH]->m_iCustomItemValue_Defense = 0;

	m_pClientList[iClientH]->m_iMinAP_SM = 0;
	m_pClientList[iClientH]->m_iMinAP_L = 0;

	m_pClientList[iClientH]->m_iMaxAP_SM = 0;
	m_pClientList[iClientH]->m_iMaxAP_L = 0;

	m_pClientList[iClientH]->m_iSpecialWeaponEffectType = 0;	// ��� ������ ȿ�� ����: 0-None 1-�ʻ�������߰� 2-�ߵ�ȿ�� 3-������ 4-������
	m_pClientList[iClientH]->m_iSpecialWeaponEffectValue = 0;	// ��� ������ ȿ�� ��

	m_pClientList[iClientH]->m_iAddHP = m_pClientList[iClientH]->m_iAddSP = m_pClientList[iClientH]->m_iAddMP = 0;
	m_pClientList[iClientH]->m_iAddAR = m_pClientList[iClientH]->m_iAddPR = m_pClientList[iClientH]->m_iAddDR = 0;
	m_pClientList[iClientH]->m_iAddMR = m_pClientList[iClientH]->m_iAddAbsPD = m_pClientList[iClientH]->m_iAddAbsMD = 0;
	m_pClientList[iClientH]->m_iAddCD = m_pClientList[iClientH]->m_iAddExp = m_pClientList[iClientH]->m_iAddGold = 0;

	iPrevSAType = m_pClientList[iClientH]->m_iSpecialAbilityType;

	m_pClientList[iClientH]->m_iSpecialAbilityType = 0;
	m_pClientList[iClientH]->m_iSpecialAbilityLastSec = 0;
	m_pClientList[iClientH]->m_iSpecialAbilityEquipPos = 0;

	m_pClientList[iClientH]->m_iAddTransMana = 0;
	m_pClientList[iClientH]->m_iAddChargeCritical = 0;

	// re introduced by SNOOPY
	m_pClientList[iClientH]->m_iHitRatio_ItemEffect_SM = 0;
	m_pClientList[iClientH]->m_iHitRatio_ItemEffect_L = 0;
	m_pClientList[iClientH]->m_iMagicHitRatio_ItemEffect = 0;

	m_pClientList[iClientH]->m_iAlterItemDropIndex = -1;
	// �������� �ʾƵ� ȿ���� �ִ� ������.
	for (sItemIndex = 0; sItemIndex < DEF_MAXITEMS; sItemIndex++)
		if (m_pClientList[iClientH]->m_pItemList[sItemIndex] != NULL) {
			switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType) {
			case DEF_ITEMEFFECTTYPE_ALTERITEMDROP:
				if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan > 0) {
					// ������ �־�� ȿ���� �ִ�.
					m_pClientList[iClientH]->m_iAlterItemDropIndex = sItemIndex;
				}
				break;
			}
		}

	// Hidden bonus to hit per weapon reintroduced by SNOOPY
	for (sItemIndex = 0; sItemIndex < DEF_MAXITEMS; sItemIndex++)
		if ((m_pClientList[iClientH]->m_pItemList[sItemIndex] != NULL) &&
			(m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] == TRUE))
		{
			switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType) {
			case DEF_ITEMEFFECTTYPE_ATTACK_DEFENSE:
			case DEF_ITEMEFFECTTYPE_ATTACK_MANASAVE:
			case DEF_ITEMEFFECTTYPE_ATTACK_MAXHPDOWN:
			case DEF_ITEMEFFECTTYPE_ATTACK:
			case DEF_ITEMEFFECTTYPE_ATTACK_ARROW:
			case DEF_ITEMEFFECTTYPE_DEFENSE:
				m_pClientList[iClientH]->m_iHitRatio_ItemEffect_SM += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffectValue1;
				m_pClientList[iClientH]->m_iHitRatio_ItemEffect_L += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffectValue2;
				break;
				// Use same (L) value for both SM & L
			case DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY:
			case DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY:
				m_pClientList[iClientH]->m_iHitRatio_ItemEffect_SM += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffectValue2;
				m_pClientList[iClientH]->m_iHitRatio_ItemEffect_L += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffectValue2;
				break;
			}
		}

	// ������ �ؾ� ȿ���� �ִ� ������ 
	for (sItemIndex = 0; sItemIndex < DEF_MAXITEMS; sItemIndex++)
		if ((m_pClientList[iClientH]->m_pItemList[sItemIndex] != NULL) &&
			(m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] == TRUE)) {

			cEquipPos = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos;

			switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType) {

			case DEF_ITEMEFFECTTYPE_MAGICDAMAGESAVE:
				// ���� ������ ���� ������. �ε����� �����Ѵ�.
				m_pClientList[iClientH]->m_iMagicDamageSaveItemIndex = sItemIndex;
				break;

			case DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY:
			case DEF_ITEMEFFECTTYPE_ATTACK_DEFENSE:
			case DEF_ITEMEFFECTTYPE_ATTACK_MANASAVE:
			case DEF_ITEMEFFECTTYPE_ATTACK_MAXHPDOWN:
			case DEF_ITEMEFFECTTYPE_ATTACK:
				m_pClientList[iClientH]->m_cAttackDiceThrow_SM = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
				m_pClientList[iClientH]->m_cAttackDiceRange_SM = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
				m_pClientList[iClientH]->m_cAttackBonus_SM = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue3;
				m_pClientList[iClientH]->m_cAttackDiceThrow_L = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue4;
				m_pClientList[iClientH]->m_cAttackDiceRange_L = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5;
				m_pClientList[iClientH]->m_cAttackBonus_L = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue6;
				// Xelima Upgraded weapons
				iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
				m_pClientList[iClientH]->m_cAttackBonus_SM += iTemp;
				m_pClientList[iClientH]->m_cAttackBonus_L += iTemp;
				// Notice: +1 on Magic damage as well
				m_pClientList[iClientH]->m_iAddMagicalDamage += iTemp;

				// Add skill to HitRatio
				m_pClientList[iClientH]->m_iHitRatio += m_pClientList[iClientH]->m_cSkillMastery[m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill];
				m_pClientList[iClientH]->m_sUsingWeaponSkill = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill;

				// v1.41 Custom-Made <==> Manuf items
				if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00000001) != NULL)
				{
					m_pClientList[iClientH]->m_iCustomItemValue_Attack += (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2 / 2);
					if (m_pClientList[iClientH]->m_iCustomItemValue_Attack > 100)
						m_pClientList[iClientH]->m_iCustomItemValue_Attack = 100;
					if (m_pClientList[iClientH]->m_iCustomItemValue_Attack < -100)
						m_pClientList[iClientH]->m_iCustomItemValue_Attack = -100;

					if (m_pClientList[iClientH]->m_iCustomItemValue_Attack > 0)
					{	// Completion >100%
						dV2 = (double)m_pClientList[iClientH]->m_iCustomItemValue_Attack;
						dV1 = (dV2 / 100.0f) * (5.0f);
						m_pClientList[iClientH]->m_iMinAP_SM = m_pClientList[iClientH]->m_cAttackDiceThrow_SM +
							m_pClientList[iClientH]->m_cAttackBonus_SM + (int)dV1;
						m_pClientList[iClientH]->m_iMinAP_L = m_pClientList[iClientH]->m_cAttackDiceThrow_L +
							m_pClientList[iClientH]->m_cAttackBonus_L + (int)dV1;
						if (m_pClientList[iClientH]->m_iMinAP_SM < 1) m_pClientList[iClientH]->m_iMinAP_SM = 1;
						if (m_pClientList[iClientH]->m_iMinAP_L < 1)  m_pClientList[iClientH]->m_iMinAP_L = 1;
						if (m_pClientList[iClientH]->m_iMinAP_SM > (m_pClientList[iClientH]->m_cAttackDiceThrow_SM * m_pClientList[iClientH]->m_cAttackDiceRange_SM + m_pClientList[iClientH]->m_cAttackBonus_SM))
							m_pClientList[iClientH]->m_iMinAP_SM = (m_pClientList[iClientH]->m_cAttackDiceThrow_SM * m_pClientList[iClientH]->m_cAttackDiceRange_SM + m_pClientList[iClientH]->m_cAttackBonus_SM);
						if (m_pClientList[iClientH]->m_iMinAP_L > (m_pClientList[iClientH]->m_cAttackDiceThrow_L * m_pClientList[iClientH]->m_cAttackDiceRange_L + m_pClientList[iClientH]->m_cAttackBonus_L))
							m_pClientList[iClientH]->m_iMinAP_L = (m_pClientList[iClientH]->m_cAttackDiceThrow_L * m_pClientList[iClientH]->m_cAttackDiceRange_L + m_pClientList[iClientH]->m_cAttackBonus_L);
					}
					else if (m_pClientList[iClientH]->m_iCustomItemValue_Attack < 0)
					{	// Completion <100%
						dV2 = (double)m_pClientList[iClientH]->m_iCustomItemValue_Attack;
						dV1 = (dV2 / 100.0f) * (5.0f);
						m_pClientList[iClientH]->m_iMaxAP_SM = m_pClientList[iClientH]->m_cAttackDiceThrow_SM * m_pClientList[iClientH]->m_cAttackDiceRange_SM
							+ m_pClientList[iClientH]->m_cAttackBonus_SM + (int)dV1;
						m_pClientList[iClientH]->m_iMaxAP_L = m_pClientList[iClientH]->m_cAttackDiceThrow_L * m_pClientList[iClientH]->m_cAttackDiceRange_L
							+ m_pClientList[iClientH]->m_cAttackBonus_L + (int)dV1;
						if (m_pClientList[iClientH]->m_iMaxAP_SM < 1) m_pClientList[iClientH]->m_iMaxAP_SM = 1;
						if (m_pClientList[iClientH]->m_iMaxAP_L < 1)  m_pClientList[iClientH]->m_iMaxAP_L = 1;
						if (m_pClientList[iClientH]->m_iMaxAP_SM < (m_pClientList[iClientH]->m_cAttackDiceThrow_SM * m_pClientList[iClientH]->m_cAttackDiceRange_SM + m_pClientList[iClientH]->m_cAttackBonus_SM))
							m_pClientList[iClientH]->m_iMaxAP_SM = (m_pClientList[iClientH]->m_cAttackDiceThrow_SM * m_pClientList[iClientH]->m_cAttackDiceRange_SM + m_pClientList[iClientH]->m_cAttackBonus_SM);
						if (m_pClientList[iClientH]->m_iMaxAP_L < (m_pClientList[iClientH]->m_cAttackDiceThrow_L * m_pClientList[iClientH]->m_cAttackDiceRange_L + m_pClientList[iClientH]->m_cAttackBonus_L))
							m_pClientList[iClientH]->m_iMaxAP_L = (m_pClientList[iClientH]->m_cAttackDiceThrow_L * m_pClientList[iClientH]->m_cAttackDiceRange_L + m_pClientList[iClientH]->m_cAttackBonus_L);
					}
				}
				// Enchanted weapons
				if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00F00000) != NULL)
				{
					dwSWEType = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00F00000) >> 20;
					dwSWEValue = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x000F0000) >> 16;
					switch (dwSWEType) {
					case 2:  // Poison			-> Increase effect of 1st item, or replace a different effect
						break;
					case 1:  // Crit +			-> Increase effect of 1st item, or replace a different effect
					case 10: // CP (as wand)	-> Increase effect of 1st item, or replace a different effect
						if (m_pClientList[iClientH]->m_iSpecialWeaponEffectType == dwSWEType)
						{
							m_pClientList[iClientH]->m_iSpecialWeaponEffectValue += dwSWEValue;
						}
						else
						{
							m_pClientList[iClientH]->m_iSpecialWeaponEffectType = dwSWEType;
							m_pClientList[iClientH]->m_iSpecialWeaponEffectValue = dwSWEValue;
						}
						break;
					case 7:  // Sharp
						m_pClientList[iClientH]->m_cAttackDiceRange_SM++;
						m_pClientList[iClientH]->m_cAttackDiceRange_L++;
						break;
					case 9:  // Ancient
						m_pClientList[iClientH]->m_cAttackDiceRange_SM += 2;
						m_pClientList[iClientH]->m_cAttackDiceRange_L += 2;
						break;
					case 11: // ManaConv 
						m_pClientList[iClientH]->m_iAddTransMana += dwSWEValue;	// SNOOPY changed to 20 as for Crit increase
						if (m_pClientList[iClientH]->m_iAddTransMana > 20) m_pClientList[iClientH]->m_iAddTransMana = 20;
						break;
					case 12: // Crit Increase 
						m_pClientList[iClientH]->m_iAddChargeCritical += dwSWEValue;
						if (m_pClientList[iClientH]->m_iAddChargeCritical > 20) m_pClientList[iClientH]->m_iAddChargeCritical = 20;
						break;
					case 4:  // Magic-using weapons

						break;
					default: // All others
						m_pClientList[iClientH]->m_iSpecialWeaponEffectType = dwSWEType;
						m_pClientList[iClientH]->m_iSpecialWeaponEffectValue = dwSWEValue;
						break;
					}
				}

				// Enchanted weapon, compute stats
				if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x0000F000) != NULL)
				{
					dwSWEType = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x0000F000) >> 12;
					dwSWEValue = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00000F00) >> 8;
					switch (dwSWEType) {
					case 0:  break;
					case 1:  m_pClientList[iClientH]->m_iAddPR += (int)dwSWEValue * 7; break;
					case 2:  m_pClientList[iClientH]->m_iAddAR += (int)dwSWEValue * 7; break; // armes HitProba
					case 3:  m_pClientList[iClientH]->m_iAddDR += (int)dwSWEValue * 7; break;
					case 4:  m_pClientList[iClientH]->m_iAddHP += (int)dwSWEValue * 7; break;
					case 5:  m_pClientList[iClientH]->m_iAddSP += (int)dwSWEValue * 7; break;
					case 6:  m_pClientList[iClientH]->m_iAddMP += (int)dwSWEValue * 7; break;
					case 7:  m_pClientList[iClientH]->m_iAddMR += (int)dwSWEValue * 7; break;
					case 8:  m_pClientList[iClientH]->m_iDamageAbsorption_Armor[cEquipPos] += (int)dwSWEValue * 3; break;

					case 9:
						m_pClientList[iClientH]->m_iAddAbsMD += (int)dwSWEValue * 3;
						if (m_pClientList[iClientH]->m_iAddAbsMD > 80) m_pClientList[iClientH]->m_iAddAbsMD = 80;
						break;

					case 10: m_pClientList[iClientH]->m_iAddCD += (int)dwSWEValue; break;
					case 11: m_pClientList[iClientH]->m_iAddExp += (int)dwSWEValue * 10; break;
					case 12: m_pClientList[iClientH]->m_iAddGold += (int)dwSWEValue * 10; break;
					}
				}

				// Those Attack items where treated as a bundle above, see here special cases
				switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType) {
				case DEF_ITEMEFFECTTYPE_ATTACK: // More combo for Flamberge
					if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue == 12)
					{
						m_pClientList[iClientH]->m_iAddCD += 5;
					}
					break;

				case DEF_ITEMEFFECTTYPE_ATTACK_MAXHPDOWN:
					m_pClientList[iClientH]->m_iSideEffect_MaxHPdown = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffect;
					// Xelima Upgraded weapons
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					m_pClientList[iClientH]->m_iSideEffect_MaxHPdown -= iTemp;
					if (m_pClientList[iClientH]->m_iSideEffect_MaxHPdown < 2) m_pClientList[iClientH]->m_iSideEffect_MaxHPdown = 2;
					break;

				case DEF_ITEMEFFECTTYPE_ATTACK_MANASAVE:
					// SNOOPY: For wands, ignore m_sItemEffectValue4/5/6 for damage{
					m_pClientList[iClientH]->m_cAttackDiceThrow_L = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
					m_pClientList[iClientH]->m_cAttackDiceRange_L = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
					m_pClientList[iClientH]->m_cAttackBonus_L = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue3;
					// Compute Mana save, max=80%
					m_pClientList[iClientH]->m_iManaSaveRatio += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue4;
					if (m_pClientList[iClientH]->m_iManaSaveRatio > 80) m_pClientList[iClientH]->m_iManaSaveRatio = 80;
					//Snoopy: Reintroduced defence for staves
					m_pClientList[iClientH]->m_iDefenseRatio += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffect;
					//Snoopy: Added Magic ToHit Bonus for some wands.
					m_pClientList[iClientH]->m_iMagicHitRatio_ItemEffect += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue6;
					break;

				case DEF_ITEMEFFECTTYPE_ATTACK_DEFENSE: // PA for some weapons
					m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BODY] += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffect;
					break;

				case DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY:
					// TYpe of special weapon, Duration of the effect
					m_pClientList[iClientH]->m_iSpecialAbilityType = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffect;
					m_pClientList[iClientH]->m_iSpecialAbilityLastSec = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffectValue1;
					m_pClientList[iClientH]->m_iSpecialAbilityEquipPos = (int)cEquipPos;
					// SNOOPY: put this here for Xelima / IceElemental Bows
					m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);

					if ((bNotify == TRUE) && (iEquipItemID == (int)sItemIndex))
						SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SPECIALABILITYSTATUS, 2, m_pClientList[iClientH]->m_iSpecialAbilityType, m_pClientList[iClientH]->m_iSpecialAbilityTime, NULL);
					break;
				}
				break;

			case DEF_ITEMEFFECTTYPE_ADDEFFECT:
				switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1) {
				case 1:	// MR
					m_pClientList[iClientH]->m_iAddResistMagic += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
					// Merien Upgraded items
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					m_pClientList[iClientH]->m_iAddResistMagic += (iTemp * 10);
					break;

				case 2:	// MS
					m_pClientList[iClientH]->m_iManaSaveRatio += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
					// Merien Upgraded items
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					m_pClientList[iClientH]->m_iManaSaveRatio += iTemp;
					// MS max = 80%
					if (m_pClientList[iClientH]->m_iManaSaveRatio > 80) m_pClientList[iClientH]->m_iManaSaveRatio = 80;
					break;

				case 3:	// Adds physical damage 
					m_pClientList[iClientH]->m_iAddPhysicalDamage += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
					// Merien Upgraded items
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					m_pClientList[iClientH]->m_iAddPhysicalDamage += iTemp;
					break;

				case 4:	// Rings Defence Ratio 
					m_pClientList[iClientH]->m_iDefenseRatio += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;

					// Merien Upgraded items
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					m_pClientList[iClientH]->m_iDefenseRatio += (iTemp * 5);
					break;

				case 5:	// Lucky Gold Ring
					// SNOOPY: changed to use m_sItemEffectValue2 as Lucky % value
					//         and cumulative betxeen items
					if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2 > 0)
						m_pClientList[iClientH]->m_bIsLuckyEffect += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
					else m_pClientList[iClientH]->m_bIsLuckyEffect += 10;
					// Merien Upgraded items
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					m_pClientList[iClientH]->m_bIsLuckyEffect += (iTemp * 10);
					break;

				case 6:	// Add Magic Damage. 
					m_pClientList[iClientH]->m_iAddMagicalDamage += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					m_pClientList[iClientH]->m_iAddMagicalDamage += iTemp;
					break;

				case 7: // Lightning protection
					m_pClientList[iClientH]->m_iAddAbsAir += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					if (m_pClientList[iClientH]->m_iAddAbsAir >= 0)		m_pClientList[iClientH]->m_iAddAbsAir += (iTemp * 5);
					else												m_pClientList[iClientH]->m_iAddAbsAir -= (iTemp * 5);
					break;
				case 8: // Earth Protection
					m_pClientList[iClientH]->m_iAddAbsEarth += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					if (m_pClientList[iClientH]->m_iAddAbsEarth >= 0)	m_pClientList[iClientH]->m_iAddAbsEarth += (iTemp * 5);
					else												m_pClientList[iClientH]->m_iAddAbsEarth -= (iTemp * 5);
					break;
				case 9: // Fire Protection
					m_pClientList[iClientH]->m_iAddAbsFire += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					if (m_pClientList[iClientH]->m_iAddAbsFire >= 0)		m_pClientList[iClientH]->m_iAddAbsFire += (iTemp * 5);
					else												m_pClientList[iClientH]->m_iAddAbsFire -= (iTemp * 5);
					break;
				case 10: // Ice Protection
					m_pClientList[iClientH]->m_iAddAbsWater += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					if (m_pClientList[iClientH]->m_iAddAbsWater >= 0)	m_pClientList[iClientH]->m_iAddAbsWater += (iTemp * 5);
					else												m_pClientList[iClientH]->m_iAddAbsWater -= (iTemp * 5);
					break;

				case 11: // Poison protection
					m_pClientList[iClientH]->m_iAddPR += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					m_pClientList[iClientH]->m_iAddPR += (iTemp * 10);
					break;

				case 12: // Adds To Hit Bonus
					m_pClientList[iClientH]->m_iHitRatio += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					m_pClientList[iClientH]->m_iHitRatio += (iTemp * 5);
					break;

				case 13: // Magin Ruby  Characters Hp recovery rate(% applied) added by the purity formula.
					m_pClientList[iClientH]->m_iAddHP += (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2 / 5);
					break;

				case 14: // Magin Diamond Attack probability(physical&magic) added by the purity formula.
					m_pClientList[iClientH]->m_iAddAR += (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2 / 5);
					break;

				case 15: // Magin Emerald Magical damage decreased(% applied) by the purity formula.    
					m_pClientList[iClientH]->m_iAddMagicalDamage += (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2 / 10);
					break;

					/*
					Functions rates confirm.
					Magic Diamond:	Completion rate / 5		= Functions rate. ? Maximum 20. (not%)
					Magic Ruby:		Completion rate / 5		= Functions rate.(%) ? Maximum 20%.
					Magic Emerald:	Completion rate / 10	= Functions rate.(%) ? Maximum 10%.
					Magic Sapphire: Completion rate / 10	= Functions rate.(%) ? Maximum 10%.
					*/

					// ******* Angel Code - Begin ******* //   
				case 16: // Angel STR//AngelicPandent(STR)
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					m_pClientList[iClientH]->m_iAngelicStr = iTemp + 1;
					SetAngelFlag(iClientH, DEF_OWNERTYPE_PLAYER, 1, iTemp);
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SETTING_SUCCESS, NULL, NULL, NULL, NULL);
					break;
				case 17: // Angel DEX //AngelicPandent(DEX)
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					m_pClientList[iClientH]->m_iAngelicDex = iTemp + 1;
					SetAngelFlag(iClientH, DEF_OWNERTYPE_PLAYER, 2, iTemp);
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SETTING_SUCCESS, NULL, NULL, NULL, NULL);
					break;
				case 18: // Angel INT//AngelicPandent(INT)
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					m_pClientList[iClientH]->m_iAngelicInt = iTemp + 1;
					SetAngelFlag(iClientH, DEF_OWNERTYPE_PLAYER, 3, iTemp);
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SETTING_SUCCESS, NULL, NULL, NULL, NULL);
					break;
				case 19: // Angel MAG//AngelicPandent(MAG)
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					m_pClientList[iClientH]->m_iAngelicMag = iTemp + 1;
					SetAngelFlag(iClientH, DEF_OWNERTYPE_PLAYER, 4, iTemp);
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SETTING_SUCCESS, NULL, NULL, NULL, NULL);
					break;
				}
				if (cEquipPos == DEF_EQUIPPOS_BACK)
				{	// Snoopy Special effect capes/ mantles still have small DefRatio & PA
					m_pClientList[iClientH]->m_iDefenseRatio += 3;	// Standard DR
					m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BACK] ++;
					// Merien Upgraded diverse (capes?)
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					if (iTemp != 0)
					{
						m_pClientList[iClientH]->m_iDefenseRatio += (iTemp * 5);
						m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BACK] += iTemp;
					}
				}
				break;

			case DEF_ITEMEFFECTTYPE_ATTACK_ARROW:
				if ((m_pClientList[iClientH]->m_cArrowIndex != -1)
					&& (m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cArrowIndex] == NULL))
				{
					m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);	// ArrowIndex
				}
				else if (m_pClientList[iClientH]->m_cArrowIndex == -1)
					m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);

				if (m_pClientList[iClientH]->m_cArrowIndex == -1)
				{
					m_pClientList[iClientH]->m_cAttackDiceThrow_SM = 0;
					m_pClientList[iClientH]->m_cAttackDiceRange_SM = 0;
					m_pClientList[iClientH]->m_cAttackBonus_SM = 0;
					m_pClientList[iClientH]->m_cAttackDiceThrow_L = 0;
					m_pClientList[iClientH]->m_cAttackDiceRange_L = 0;
					m_pClientList[iClientH]->m_cAttackBonus_L = 0;
				}
				else
				{
					m_pClientList[iClientH]->m_cAttackDiceThrow_SM = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
					m_pClientList[iClientH]->m_cAttackDiceRange_SM = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
					m_pClientList[iClientH]->m_cAttackBonus_SM = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue3;
					m_pClientList[iClientH]->m_cAttackDiceThrow_L = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue4;
					m_pClientList[iClientH]->m_cAttackDiceRange_L = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5;
					m_pClientList[iClientH]->m_cAttackBonus_L = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue6;
				}
				// Xelima Upgraded bows
				iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
				m_pClientList[iClientH]->m_cAttackBonus_SM += iTemp;
				m_pClientList[iClientH]->m_cAttackBonus_L += iTemp;

				// Standard HitRatio uses Skill 
				m_pClientList[iClientH]->m_iHitRatio += m_pClientList[iClientH]->m_cSkillMastery[m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill];
				// iHitRatio3 discards skill
				// SNOOPY: Added support for enchanted bows. (1st stat)
				if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00F00000) != NULL)
				{
					dwSWEType = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00F00000) >> 20;
					dwSWEValue = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x000F0000) >> 16;
					switch (dwSWEType) {
					case 2:  // Poison			-> Increase effect of 1st item, or replace a different effect					
						break;
					case 1:  // Crit +			-> Increase effect of 1st item, or replace a different effect
					case 10: // CP (as wand)	-> Increase effect of 1st item, or replace a different effect
						if (m_pClientList[iClientH]->m_iSpecialWeaponEffectType == dwSWEType)
						{
							m_pClientList[iClientH]->m_iSpecialWeaponEffectValue += dwSWEValue;
						}
						else
						{
							m_pClientList[iClientH]->m_iSpecialWeaponEffectType = dwSWEType;
							m_pClientList[iClientH]->m_iSpecialWeaponEffectValue = dwSWEValue;
						}
						break;
					case 7:  // Sharp
						m_pClientList[iClientH]->m_cAttackDiceRange_SM++;
						m_pClientList[iClientH]->m_cAttackDiceRange_L++;
						break;
					case 9:  // Ancient
						m_pClientList[iClientH]->m_cAttackDiceRange_SM += 2;
						m_pClientList[iClientH]->m_cAttackDiceRange_L += 2;
						break;
					case 11: // ManaConv 
						m_pClientList[iClientH]->m_iAddTransMana += dwSWEValue;	// SNOOPY changed to 20 as for Crit increase
						if (m_pClientList[iClientH]->m_iAddTransMana > 20) m_pClientList[iClientH]->m_iAddTransMana = 20;
						break;
					case 12: // Crit Increase 
						m_pClientList[iClientH]->m_iAddChargeCritical += dwSWEValue;
						if (m_pClientList[iClientH]->m_iAddChargeCritical > 20) m_pClientList[iClientH]->m_iAddChargeCritical = 20;
						break;
					case 4:  // Magic-using weapons, don't overide a main weapon

						break;
					default: // All others
						m_pClientList[iClientH]->m_iSpecialWeaponEffectType = (int)dwSWEType;
						m_pClientList[iClientH]->m_iSpecialWeaponEffectValue = (int)dwSWEValue;
						break;
					}
				}

				// SNOOPY: Added support for enchanted bows. (2nd stat)
				if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x0000F000) != NULL)
				{
					dwSWEType = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x0000F000) >> 12;
					dwSWEValue = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00000F00) >> 8;
					switch (dwSWEType) {
					case 0:  break;
					case 1:  m_pClientList[iClientH]->m_iAddPR += (int)dwSWEValue * 7; break;
					case 2:  m_pClientList[iClientH]->m_iAddAR += (int)dwSWEValue * 7; break; // adds Hit Proba
					case 3:  m_pClientList[iClientH]->m_iAddDR += (int)dwSWEValue * 7; break;
					case 4:  m_pClientList[iClientH]->m_iAddHP += (int)dwSWEValue * 7; break;
					case 5:  m_pClientList[iClientH]->m_iAddSP += (int)dwSWEValue * 7; break;
					case 6:  m_pClientList[iClientH]->m_iAddMP += (int)dwSWEValue * 7; break;
					case 7:  m_pClientList[iClientH]->m_iAddMR += (int)dwSWEValue * 7; break;
					case 8:  m_pClientList[iClientH]->m_iDamageAbsorption_Armor[cEquipPos] += (int)dwSWEValue * 3; break;
					case 9:  m_pClientList[iClientH]->m_iAddAbsMD += (int)dwSWEValue * 3;
						if (m_pClientList[iClientH]->m_iAddAbsMD > 80) m_pClientList[iClientH]->m_iAddAbsMD = 80;
						break;
					case 10: m_pClientList[iClientH]->m_iAddCD += (int)dwSWEValue; break;
					case 11: m_pClientList[iClientH]->m_iAddExp += (int)dwSWEValue * 10; break;
					case 12: m_pClientList[iClientH]->m_iAddGold += (int)dwSWEValue * 10; break;
					}
				}
				break;

			case DEF_ITEMEFFECTTYPE_DEFENSE: // Notice that MainGauche is here as well .....			
			case DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY:
				// SNOOPY: 2 values for Defence ratio fot PvP mode HIGH
				m_pClientList[iClientH]->m_iDefenseRatio += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;


				// Merien Upgraded armors, add DefRatio for all defence items
				iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
				m_pClientList[iClientH]->m_iDefenseRatio += (iTemp * 5);

				// Manufs armors 
				if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00000001) != NULL)
				{
					m_pClientList[iClientH]->m_iCustomItemValue_Defense += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2;
					dV2 = (double)m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2;
					dV3 = (double)m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
					dV1 = (double)(dV2 / 100.0f) * dV3;
					dV1 = dV1 / 2.0f;
					m_pClientList[iClientH]->m_iDefenseRatio += (int)dV1;
					if (m_pClientList[iClientH]->m_iDefenseRatio <= 0) m_pClientList[iClientH]->m_iDefenseRatio = 1;
				}

				if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00F00000) != NULL)
				{
					dwSWEType = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00F00000) >> 20;
					dwSWEValue = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x000F0000) >> 16;
					switch (dwSWEType) {
					case 1:  // Crit +			-> May increase effect of 1st item or give effect if no other effect present
					case 2:  // Poison			-> May Increase effect of 1st item or give effect if no other effect present
					case 10: // CP (as wand)	-> May Increase effect of 1st item or give effect if no other effect present				
						if (m_pClientList[iClientH]->m_iSpecialWeaponEffectType == dwSWEType)
						{
							m_pClientList[iClientH]->m_iSpecialWeaponEffectValue += dwSWEValue;
						}
						else
							if (m_pClientList[iClientH]->m_iSpecialWeaponEffectType == 0)
							{
								m_pClientList[iClientH]->m_iSpecialWeaponEffectType = dwSWEType;
								m_pClientList[iClientH]->m_iSpecialWeaponEffectValue = dwSWEValue;
							}
						break;
					case 3: // Rite -> Give effect if no other effect present	
						if (m_pClientList[iClientH]->m_iSpecialWeaponEffectType == 0)
						{
							m_pClientList[iClientH]->m_iSpecialWeaponEffectType = dwSWEType;
							m_pClientList[iClientH]->m_iSpecialWeaponEffectValue = dwSWEValue;
						}
						break;
					case 7: // Sharp MG have effect in iCalculatteAttackEffect function
					case 9: // Ancient MG have effect in iCalculatteAttackEffect function
						break;
					case 11: // ManaConv  Total max 20
						m_pClientList[iClientH]->m_iAddTransMana += dwSWEValue;	// SNOOPY changed to 20 as for Crit increase
						if (m_pClientList[iClientH]->m_iAddTransMana > 20) m_pClientList[iClientH]->m_iAddTransMana = 20;
						break;
					case 12: // Crit Increase Total max 20
						m_pClientList[iClientH]->m_iAddChargeCritical += dwSWEValue;
						if (m_pClientList[iClientH]->m_iAddChargeCritical > 20) m_pClientList[iClientH]->m_iAddChargeCritical = 20;
						break;
					case 4: // ???
						break;
					default:
						break;
					}
				}
				// stated armors/shields
				if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x0000F000) != NULL)
				{
					dwSWEType = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x0000F000) >> 12;
					dwSWEValue = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00000F00) >> 8;
					// HP or Rep+ will have effect on Armor or MainGauche !
					switch (dwSWEType) {
					case 0:  break;
					case 1:  m_pClientList[iClientH]->m_iAddPR += (int)dwSWEValue * 7; break;
					case 2:  m_pClientList[iClientH]->m_iAddAR += (int)dwSWEValue * 7; break; // Armes Hit Proba
					case 3:  m_pClientList[iClientH]->m_iAddDR += (int)dwSWEValue * 7; break;
					case 4:  // HPrec
						if (cEquipPos == DEF_EQUIPPOS_LHAND)
							iShieldHPrec += (int)dwSWEValue * 7;
						else m_pClientList[iClientH]->m_iAddHP += (int)dwSWEValue * 7;
						break;
					case 5:  // SPrec
						if (cEquipPos == DEF_EQUIPPOS_LHAND)
							iShieldSPrec += (int)dwSWEValue * 7;
						else m_pClientList[iClientH]->m_iAddSP += (int)dwSWEValue * 7;
						break;
					case 6:   // MPrec
						if (cEquipPos == DEF_EQUIPPOS_LHAND)
							iShieldMPrec += (int)dwSWEValue * 7;
						else m_pClientList[iClientH]->m_iAddMP += (int)dwSWEValue * 7;
						break;
					case 7:
						m_pClientList[iClientH]->m_iAddMR += (int)dwSWEValue * 7;
						break;
					case 8: // PA
						if (cEquipPos == DEF_EQUIPPOS_LHAND)
							m_pClientList[iClientH]->m_iDamageAbsorption_Shield += (int)dwSWEValue * 3;
						else m_pClientList[iClientH]->m_iDamageAbsorption_Armor[cEquipPos] += (int)dwSWEValue * 3;
						break;
					case 9:
						m_pClientList[iClientH]->m_iAddAbsMD += (int)dwSWEValue * 3;
						if (m_pClientList[iClientH]->m_iAddAbsMD > 80) m_pClientList[iClientH]->m_iAddAbsMD = 80;
						break;
					case 10: m_pClientList[iClientH]->m_iAddCD += (int)dwSWEValue; break;
					case 11: m_pClientList[iClientH]->m_iAddExp += (int)dwSWEValue * 10; break;
					case 12: m_pClientList[iClientH]->m_iAddGold += (int)dwSWEValue * 10; break;
					}

				}
				switch (cEquipPos) { // Sub-cases of DEF_ITEMEFFECTTYPE_DEFENSE
				case DEF_EQUIPPOS_LHAND:
					if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2 != 0)
					{
						m_pClientList[iClientH]->m_iDamageAbsorption_Shield = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
					}
					else // Snoopy individual PA setting for shields...
					{
						m_pClientList[iClientH]->m_iDamageAbsorption_Shield = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1) - (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1) / 3;
					}
					// Merien Upgraded shield +1 PA
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					m_pClientList[iClientH]->m_iDamageAbsorption_Shield += iTemp;

					break;
				case DEF_EQUIPPOS_TWOHAND:

				case DEF_EQUIPPOS_RHAND:

					break;
				case DEF_EQUIPPOS_HEAD:	// PA on Head
					m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_HEAD] += (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2);
					// Merien Upgraded helmet +1 PA
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_HEAD] += iTemp;
					break;
				case DEF_EQUIPPOS_BODY:
					m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BODY] += (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2);
					// Merien Upgraded armor +1 PA
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BODY] += iTemp;
					switch (m_pClientList[iClientH]->m_cSex) {
					case 1: // Male  1:Leather, 5:Tunic
						switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) {
						case 1:  // Leather
							m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_RELEASEALL] = m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BODY] / 2;

							break;
						case 5:  // Tunic
							m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_RELEASEALL] = m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BODY] / 2;
							break;
						case 2:  // Chain
						case 3:  // Scale

							break;
						case 6: //  Robe(M)
						case 10: // eHeroRobe(M)
						case 11: // aHeroRobe(M) // Some all places PA with robes
							m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_RELEASEALL] = m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BODY] / 2;

							break;
						case 4: // Plate
						default:	// plate +				

							break;
						}
						break;
					case 2: // Women 1,2:Bodice, 3:Leather
						switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) {
						case 1:  // Bodice
						case 2:  // Bodice
							m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_RELEASEALL] = m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BODY] / 2;
							break;
						case 3:  // Leather
							m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_RELEASEALL] = m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BODY] / 2;

							break;
						case 4:  // Chain
						case 5:  // Scale
							break;
						case 7: //  Robe(M)
						case 11: // eHeroRobe(M)
						case 12: // aHeroRobe(M) // Some all places PA with robes
							m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_RELEASEALL] = m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BODY] / 2;
							break;
						case 6: // Plate
						default:

							break;
						}
						break;
					}
					break;
				case DEF_EQUIPPOS_ARMS:
					m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_ARMS] += (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2);
					// Merien Upgraded hauberks +1 PA
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_ARMS] += iTemp;
					switch (m_pClientList[iClientH]->m_cSex) {
					case 1: // Male  1:Chemise 2:Hauberk
						if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue <= 1)
						{
							m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_RELEASEALL] = m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_ARMS] / 2;
						}
						break;
					case 2: // Women 1,2:Chemise Shirt, 3:Hauberk
						if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue <= 2)
						{
							m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_RELEASEALL] = m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_ARMS] / 2;
						}
						break;
					}
					break;
				case DEF_EQUIPPOS_PANTS:
					m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_PANTS] += (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2);
					// Merien Upgraded leggings +1 PA
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_PANTS] += iTemp;
					break;
				case DEF_EQUIPPOS_LEGGINGS:
					m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_LEGGINGS] += (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2);
					// Merien Upgraded boots +1 PA
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_LEGGINGS] += iTemp;
					// Motemorphosed char is still affected by original boots
					m_pClientList[iClientH]->m_iDefenseRatio += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
					break;
				case DEF_EQUIPPOS_BACK: // Cape
					// Motemorphosed char is still affected by original cape
					m_pClientList[iClientH]->m_iDefenseRatio += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
					break;

				default:
					// SNOOPY: Kept this here so we can use Damage absorbtion for something else (ie: Cape)
					m_pClientList[iClientH]->m_iDamageAbsorption_Armor[cEquipPos] += (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2);
					// Merien Upgraded diverse (capes?)
					iTemp = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					m_pClientList[iClientH]->m_iDamageAbsorption_Armor[cEquipPos] += iTemp;
					break;
				}

				switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType) {
				case DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY:
					m_pClientList[iClientH]->m_iSpecialAbilityType = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffect;
					m_pClientList[iClientH]->m_iSpecialAbilityLastSec = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffectValue1;
					m_pClientList[iClientH]->m_iSpecialAbilityEquipPos = (int)cEquipPos;
					if ((bNotify == TRUE) && (iEquipItemID == (int)sItemIndex))
						SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SPECIALABILITYSTATUS, 2, m_pClientList[iClientH]->m_iSpecialAbilityType, m_pClientList[iClientH]->m_iSpecialAbilityTime, NULL);
					break;
				}
				break;
			}
		}

	//v1.432
	if ((iPrevSAType != 0) && (m_pClientList[iClientH]->m_iSpecialAbilityType == 0) && (bNotify == TRUE))
	{
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SPECIALABILITYSTATUS, 4, NULL, NULL, NULL);
		if (m_pClientList[iClientH]->m_bIsSpecialAbilityEnabled == TRUE)
		{
			m_pClientList[iClientH]->m_bIsSpecialAbilityEnabled = FALSE;
			m_pClientList[iClientH]->m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;
			sTemp = m_pClientList[iClientH]->m_sAppr4;
			sTemp = sTemp & 0xFF0F;
			m_pClientList[iClientH]->m_sAppr4 = sTemp;
			SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, NULL, NULL, NULL);
		}
	}
	if ((iPrevSAType != 0) && (m_pClientList[iClientH]->m_iSpecialAbilityType != 0)
		&& (iPrevSAType != m_pClientList[iClientH]->m_iSpecialAbilityType) && (bNotify == TRUE))
	{
		if (m_pClientList[iClientH]->m_bIsSpecialAbilityEnabled == TRUE)
		{
			SendNotifyMsg(NULL, i, DEF_NOTIFY_SPECIALABILITYSTATUS, 3, NULL, NULL, NULL); // 20min
			m_pClientList[iClientH]->m_bIsSpecialAbilityEnabled = FALSE;
			m_pClientList[iClientH]->m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;
			sTemp = m_pClientList[iClientH]->m_sAppr4;
			sTemp = sTemp & 0xFF0F;
			m_pClientList[iClientH]->m_sAppr4 = sTemp;
			SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, NULL, NULL, NULL);
		}
	}

	// Snoopy! Limitation of HPrec, MPrec SPrec.100% max not counting shield			
	if (m_pClientList[iClientH]->m_iAddHP > 100) m_pClientList[iClientH]->m_iAddHP = 100;
	if (m_pClientList[iClientH]->m_iAddSP > 100) m_pClientList[iClientH]->m_iAddSP = 100;
	if (m_pClientList[iClientH]->m_iAddMP > 100) m_pClientList[iClientH]->m_iAddMP = 100;
	m_pClientList[iClientH]->m_iAddHP += iShieldHPrec;
	m_pClientList[iClientH]->m_iAddSP += iShieldSPrec;
	m_pClientList[iClientH]->m_iAddMP += iShieldMPrec;
	m_pClientList[iClientH]->m_cHeroArmourBonus = _cCheckHeroItemEquipped(iClientH);

	// Snoopy: Bonus for Angels	
	m_pClientList[iClientH]->m_iDefenseRatio += m_pClientList[iClientH]->m_iAngelicDex * 2;

	if (m_pClientList[iClientH]->m_iHP > iGetMaxHP(iClientH)) m_pClientList[iClientH]->m_iHP = iGetMaxHP(iClientH, FALSE);
	if (m_pClientList[iClientH]->m_iMP > iGetMaxMP(iClientH)) m_pClientList[iClientH]->m_iMP = iGetMaxMP(iClientH);
	if (m_pClientList[iClientH]->m_iSP > iGetMaxSP(iClientH)) m_pClientList[iClientH]->m_iSP = iGetMaxSP(iClientH);

	// Maximum value for  PA, here instead of iCalculateAttackEffect
	if (m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BODY] > 60)	// Torse,  max 60
		m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BODY] = 60;
	if (m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_HEAD] > 60)	// T�te    max 60
		m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_HEAD] = 60;
	if (m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_ARMS] > 60)	// Membres max 60
		m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_ARMS] = 60;
	if (m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_PANTS] > 60)	// Membres max 60 
		m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_PANTS] = 60;
	if (m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_LEGGINGS] > 60)	// Pieds   max 60 
		m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_LEGGINGS] = 60;
	if (m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BACK] > 60)	// Dos     max 60
		m_pClientList[iClientH]->m_iDamageAbsorption_Armor[DEF_EQUIPPOS_BACK] = 60;
	if (m_pClientList[iClientH]->m_iDamageAbsorption_Shield > 60)						// Shield  max 60
		m_pClientList[iClientH]->m_iDamageAbsorption_Shield = 60;

	// Ne dire aux clients que si ca a chang�.
	if (iTotalAngelicstatas != (m_pClientList[iClientH]->m_iAngelicStr + (16 * m_pClientList[iClientH]->m_iAngelicInt) + (256 * m_pClientList[iClientH]->m_iAngelicDex) + (16 * 256 * m_pClientList[iClientH]->m_iAngelicMag)))
	{
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SETTING_SUCCESS, NULL, NULL, NULL, NULL);
	}
}

BOOL CGame::_bDepleteDestTypeItemUseEffect(int iClientH, int dX, int dY, short sItemIndex, short sDestItemID)
{
	BOOL bRet;

	// À§Ä¡¸¦ ÁöÁ¤ÇÏ°í »ç¿ë ÈÄ »ç¶óÁö´Â ¾ÆÀÌÅÛ È¿°ú Ã³¸® ºÎºÐ 
	if (m_pClientList[iClientH] == NULL) return FALSE;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return FALSE;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == NULL) return FALSE;

	switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType) {
	case DEF_ITEMEFFECTTYPE_OCCUPYFLAG:
		bRet = __bSetOccupyFlag(m_pClientList[iClientH]->m_cMapIndex, dX, dY,
			m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1,
			NULL, iClientH, NULL);
		if (bRet == TRUE) {
			GetExp(iClientH, (iDice(1, (m_pClientList[iClientH]->m_iLevel)))); // centu
		}
		else {
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_NOTFLAGSPOT, NULL, NULL, NULL, NULL);
		}
		return bRet;

		// crusade
	case DEF_ITEMEFFECTTYPE_CONSTRUCTIONKIT:
		// °Ç¼³ Å°Æ®ÀÌ´Ù. ¹Ìµé·£µå¿¡¼­¸¸ »ç¿ë °¡´ÉÇÔ. m_sItemEffectValue1: °ÇÃà¹° Á¾·ù, m_sItemEffectValue2: °ÇÃà ½Ã°£ 
		bRet = __bSetConstructionKit(m_pClientList[iClientH]->m_cMapIndex, dX, dY,                                 // 
			m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1,        //
			m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2,        //
			iClientH); // ¼³Ä¡ÀÚ 
		if (bRet == TRUE) {
			// °Ç¼³ ½ÃÀÛ	
			GetExp(iClientH, (iDice(1, (m_pClientList[iClientH]->m_iLevel)))); // centu
		}
		else {
			// °Ç¼³ ½ÇÆÐ		
			// Centuu : No se puede construir.
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CANNOTCONSTRUCT, NULL, NULL, NULL, NULL);
		}
		return bRet;

	case DEF_ITEMEFFECTTYPE_DYE:
		if ((sDestItemID >= 0) && (sDestItemID < DEF_MAXITEMS)) {
			if (m_pClientList[iClientH]->m_pItemList[sDestItemID] != NULL) {
				if ((m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cCategory == 11) ||
					(m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cCategory == 12))
				{
					m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cItemColor = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMCOLORCHANGE, sDestItemID, m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cItemColor, NULL, NULL);
					return TRUE;
				}
				else 
				{
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMCOLORCHANGE, sDestItemID, -1, NULL, NULL);
					return FALSE;
				}
			}
		}
		break;

	case DEF_ITEMEFFECTTYPE_ARMORDYE:
		if ((sDestItemID >= 0) && (sDestItemID < DEF_MAXITEMS)) {
			if (m_pClientList[iClientH]->m_pItemList[sDestItemID] != NULL) {
				if (m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cCategory == 6) {
					m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cItemColor = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMCOLORCHANGE, sDestItemID, m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cItemColor, NULL, NULL);
					return TRUE;
				}
				else {
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMCOLORCHANGE, sDestItemID, -1, NULL, NULL);
					return FALSE;
				}
			}
		}
		break;

	case DEF_ITEMEFFECTTYPE_WEAPONDYE:
		if ((sDestItemID >= 0) && (sDestItemID < DEF_MAXITEMS)) {
			if (m_pClientList[iClientH]->m_pItemList[sDestItemID] != NULL) {
				if (m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cCategory == 1) {
					m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cItemColor = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMCOLORCHANGE, sDestItemID, m_pClientList[iClientH]->m_pItemList[sDestItemID]->m_cItemColor, NULL, NULL);
					return TRUE;
				}
				else {
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMCOLORCHANGE, sDestItemID, -1, NULL, NULL);
					return FALSE;
				}
			}
		}
		break;

	case DEF_ITEMEFFECTTYPE_FARMING:
		bRet = bPlantSeedBag(m_pClientList[iClientH]->m_cMapIndex, dX, dY,
			m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1,
			m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2,
			iClientH);

		if (bRet == TRUE) {
			// °Ç¼³ ½ÃÀÛ	
			GetExp(iClientH, (iDice(1, (m_pClientList[iClientH]->m_iLevel)))); // centu
		}
		else {
			// °Ç¼³ ½ÇÆÐ	
			// Centuu : No se puede plantar.
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_NOMOREAGRICULTURE, NULL, NULL, NULL, NULL);
		}

		return bRet;

	default:
		break;
	}

	return TRUE;
}

// v1.4311-3 Ãß°¡  ÇÔ¼ö ÀÔÀå±ÇÀ» ÁÖ´Â ÇÔ¼ö   GetFightzoneTicketHandler 
void CGame::GetFightzoneTicketHandler(int iClientH)
{
	int   iRet, iEraseReq, iMonth, iDay, iHour;
	char* cp, cData[256], cItemName[21];
	class CItem* pItem;
	DWORD* dwp;
	short* sp;
	WORD* wp;

	if (m_pClientList[iClientH] == NULL) return;

	if (m_pClientList[iClientH]->m_iFightZoneTicketNumber <= 0) {
		// ÀÔÀå±ÇÀ» ´Ù »ç¿ëÇßÀ½À» ¾Ë¸°´Ù.
		// »çÅõÀå ¹øÈ£°¡ À½¼ö¸é ¿¹¾àÀº Çß´Âµ¥ ÀÔÀå±ÇÀ» ´Ù ¹ÞÀº°æ¿ì ..
		m_pClientList[iClientH]->m_iFightzoneNumber *= -1;
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_FIGHTZONERESERVE, -1, NULL, NULL, NULL);
		return;
	}

	ZeroMemory(cItemName, sizeof(cItemName));

	if (m_pClientList[iClientH]->m_iFightzoneNumber == 1)
		strcpy(cItemName, "ArenaTicket");
	else  wsprintf(cItemName, "ArenaTicket(%d)", m_pClientList[iClientH]->m_iFightzoneNumber);

	pItem = new class CItem;
	if (_bInitItemAttr(pItem, cItemName) == FALSE) {
		delete pItem;
		return;
	}

	if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == TRUE) {
		// ¿¡·¯ ¹æÁö¿ë ÄÚµå
		if (m_pClientList[iClientH]->m_iCurWeightLoad < 0) m_pClientList[iClientH]->m_iCurWeightLoad = 0;

		// ¾ÆÀÌÅÛÀ» ¹ÞÀ» ¼ö ÀÖ´Ù´Â °ÍÀÌ È®Á¤ µÇ¾úÀ¸¹Ç·Î ÀÔÀå±Ç°³¼ö¸¦ »©µµ µÈ´Ù. 
		m_pClientList[iClientH]->m_iFightZoneTicketNumber = m_pClientList[iClientH]->m_iFightZoneTicketNumber - 1;

		pItem->m_sTouchEffectType = DEF_ITET_DATE;

		iMonth = m_pClientList[iClientH]->m_iReserveTime / 10000;
		iDay = (m_pClientList[iClientH]->m_iReserveTime - iMonth * 10000) / 100;
		iHour = m_pClientList[iClientH]->m_iReserveTime - iMonth * 10000 - iDay * 100;

		pItem->m_sTouchEffectValue1 = iMonth;
		pItem->m_sTouchEffectValue2 = iDay;
		pItem->m_sTouchEffectValue3 = iHour;


		wsprintf(G_cTxt, "(*) Get FIGHTZONETICKET : Char(%s) TICKENUMBER (%d)(%d)(%d)", m_pClientList[iClientH]->m_cCharName, pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3);
		PutLogFileList(G_cTxt);
		PutLogList(G_cTxt);

		ZeroMemory(cData, sizeof(cData));

		// ¾ÆÀÌÅÛ ¾ò¾ú´Ù´Â ¸Þ½ÃÁö¸¦ Àü¼ÛÇÑ´Ù.
		dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_ITEMOBTAINED;

		cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);
		// 1°³ È¹µæÇß´Ù.
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

		// ¼ÒÁöÇ° ÃÑ Áß·® Àç °è»ê 
		iCalcTotalWeight(iClientH);

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
		// °ø°£ÀÌ ºÎÁ·ÇØ ¾ÆÀÌÅÛÀ» ¾òÀ» ¼ö ¾ø´Ù.
		delete pItem;

		// ¼ÒÁöÇ° ÃÑ Áß·® Àç °è»ê 
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
			// ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù.
			DeleteClient(iClientH, TRUE, TRUE);
			break;
		}
	}
}

//Hero Code by Zabuza - fixed by Centu
void CGame::GetHeroMantleHandler(int iClientH, int iItemID, char* pString)
{
	int   i, iNum, iRet, iEraseReq;
	char* cp, cData[256], cItemName[21];
	class CItem* pItem;
	DWORD* dwp;
	short* sp;
	WORD* wp;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_iEnemyKillCount < 100) return;
	if (m_pClientList[iClientH]->m_iContribution < 10) return;
	if (m_pClientList[iClientH]->m_cSide == 0) return;

	if (_iGetItemSpaceLeft(iClientH) == 0) {
		SendItemNotifyMsg(iClientH, DEF_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL);
		return;
	}

	//Prevents a crash if item dosent exist
	if (m_pItemConfigList[iItemID] == NULL)  return;

	switch (iItemID) {
		// Hero Cape
	case 400: //Aresden HeroCape
	case 401: //Elvine HeroCape
		if (m_pClientList[iClientH]->m_iEnemyKillCount < 300) return;
		m_pClientList[iClientH]->m_iEnemyKillCount -= 300;
		break;
	}

	// Da el item correspondiente dependiendo el sexo del personaje, centu.
	switch (m_pClientList[iClientH]->m_cSex) {
	case 1:
		switch (iItemID) {
			// Hero Helm
		case 403: //Aresden HeroHelm(M)
		case 405: //Elvine HeroHelm(M)
			if (m_pClientList[iClientH]->m_iEnemyKillCount < 150 || m_pClientList[iClientH]->m_iContribution < 20) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 150;
			m_pClientList[iClientH]->m_iContribution -= 20;
			break;
			// Hero Cap
		case 407: //Aresden HeroCap(M)
		case 409: //Elvine HeroCap(M)
			if (m_pClientList[iClientH]->m_iEnemyKillCount < 100 || m_pClientList[iClientH]->m_iContribution < 20) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 100;
			m_pClientList[iClientH]->m_iContribution -= 20;
			break;
			// Hero Armour
		case 411: //Aresden HeroArmour(M)
		case 413: //Elvine HeroArmour(M)
			if (m_pClientList[iClientH]->m_iEnemyKillCount < 300 || m_pClientList[iClientH]->m_iContribution < 30) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 300;
			m_pClientList[iClientH]->m_iContribution -= 30;
			break;
			// Hero Robe
		case 415: //Aresden HeroRobe(M)
		case 417: //Elvine HeroRobe(M)
			if (m_pClientList[iClientH]->m_iEnemyKillCount < 200 || m_pClientList[iClientH]->m_iContribution < 20) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 200;
			m_pClientList[iClientH]->m_iContribution -= 20;
			break;
			// Hero Hauberk
		case 419: //Aresden HeroHauberk(M)
		case 421: //Elvine HeroHauberk(M)
			if (m_pClientList[iClientH]->m_iEnemyKillCount < 100 || m_pClientList[iClientH]->m_iContribution < 10) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 100;
			m_pClientList[iClientH]->m_iContribution -= 10;
			break;
			// Hero Leggings
		case 423: //Aresden HeroLeggings(M)
		case 425: //Elvine HeroLeggings(M)
			if (m_pClientList[iClientH]->m_iEnemyKillCount < 150 || m_pClientList[iClientH]->m_iContribution < 15) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 150;
			m_pClientList[iClientH]->m_iContribution -= 15;
			break;
		default:
			break;
		}
		break;

	case 2:
		switch (iItemID) {
		case 404: //Aresden HeroHelm(W)
		case 406: //Elvine HeroHelm(W)
			if (m_pClientList[iClientH]->m_iEnemyKillCount < 150 || m_pClientList[iClientH]->m_iContribution < 20) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 150;
			m_pClientList[iClientH]->m_iContribution -= 20;
			break;
		case 408: //Aresden HeroCap(W)
		case 410: //Elvine HeroCap(W)
			if (m_pClientList[iClientH]->m_iEnemyKillCount < 100 || m_pClientList[iClientH]->m_iContribution < 20) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 100;
			m_pClientList[iClientH]->m_iContribution -= 20;
			break;
		case 412: //Aresden HeroArmour(W)
		case 414: //Elvine HeroArmour(W)
			if (m_pClientList[iClientH]->m_iEnemyKillCount < 300 || m_pClientList[iClientH]->m_iContribution < 30) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 300;
			m_pClientList[iClientH]->m_iContribution -= 30;
			break;
		case 416: //Aresden HeroRobe(W)
		case 418: //Elvine HeroRobe(W)
			if (m_pClientList[iClientH]->m_iEnemyKillCount < 200 || m_pClientList[iClientH]->m_iContribution < 20) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 200;
			m_pClientList[iClientH]->m_iContribution -= 20;
			break;
		case 420: //Aresden HeroHauberk(W)
		case 422: //Elvine HeroHauberk(W)
			if (m_pClientList[iClientH]->m_iEnemyKillCount < 100 || m_pClientList[iClientH]->m_iContribution < 10) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 100;
			m_pClientList[iClientH]->m_iContribution -= 10;
			break;
		case 424: //Aresden HeroLeggings(W)
		case 426: //Elvine HeroLeggings(W)
			if (m_pClientList[iClientH]->m_iEnemyKillCount < 150 || m_pClientList[iClientH]->m_iContribution < 15) return;
			m_pClientList[iClientH]->m_iEnemyKillCount -= 150;
			m_pClientList[iClientH]->m_iContribution -= 15;
			break;

		default:
			break;
		}
		break;
	}

	ZeroMemory(cItemName, sizeof(cItemName));
	memcpy(cItemName, m_pItemConfigList[iItemID]->m_cName, 20);
	// ReqPurchaseItemHandler
	iNum = 1;
	for (i = 1; i <= iNum; i++) {
		pItem = new class CItem;
		if (_bInitItemAttr(pItem, cItemName) == FALSE) {
			delete pItem;
		}
		else {
			if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == TRUE) {
				if (m_pClientList[iClientH]->m_iCurWeightLoad < 0) m_pClientList[iClientH]->m_iCurWeightLoad = 0;

				wsprintf(G_cTxt, "(*) Get HeroItem : Char(%s) Player-EK(%d) Player-Contr(%d) Hero Obtained(%s)", m_pClientList[iClientH]->m_cCharName, m_pClientList[iClientH]->m_iEnemyKillCount, m_pClientList[iClientH]->m_iContribution, cItemName);
				PutLogFileList(G_cTxt);

				pItem->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
				pItem->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
				pItem->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
				pItem->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;

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
			else {
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

void CGame::_SetItemPos(int iClientH, char* pData)
{
	char* cp, cItemIndex;
	short* sp, sX, sY;

	if (m_pClientList[iClientH] == NULL) return;

	cp = (char*)(pData + DEF_INDEX2_MSGTYPE + 2);
	cItemIndex = *cp;
	cp++;

	sp = (short*)cp;
	sX = *sp;
	cp += 2;

	sp = (short*)cp;
	sY = *sp;
	cp += 2;

	// Àß¸øµÈ ÁÂÇ¥°ª º¸Á¤ 
	if (sY < -10) sY = -10;

	if ((cItemIndex < 0) || (cItemIndex >= DEF_MAXITEMS)) return;
	if (m_pClientList[iClientH]->m_pItemList[cItemIndex] != NULL) {
		m_pClientList[iClientH]->m_ItemPosList[cItemIndex].x = sX;
		m_pClientList[iClientH]->m_ItemPosList[cItemIndex].y = sY;
	}
}

void CGame::CheckUniqueItemEquipment(int iClientH)
{
	int i, iDamage;

	if (m_pClientList[iClientH] == NULL) return;
	// v1.3 À¯´ÏÅ© ¾ÆÀÌÅÛÀÇ ÀåÂø »óÅÂ¸¦ ÆÇ´ÜÇÑ´Ù. Æ¯Á¤ÀÎ¸¸ÀÌ Âø¿ë °¡´ÉÇÑ ¾ÆÀÌÅÛÀ» ÀåÂøÇÑ °æ¿ì ¹þ°ÜÁø´Ù.

	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemList[i] != NULL) {
			if ((m_pClientList[iClientH]->m_pItemList[i]->m_sTouchEffectType == DEF_ITET_UNIQUE_OWNER) &&
				(m_pClientList[iClientH]->m_bIsItemEquipped[i] == TRUE)) {
				// Touch Effect TypeÀÌ DEF_ITET_OWNERÀÌ¸é Touch Effect Value 1, 2, 3ÀÌ ÁÖÀÎ Ä³¸¯ÅÍÀÇ °íÀ¯°ªÀ» °®´Â´Ù. 

				if ((m_pClientList[iClientH]->m_pItemList[i]->m_sTouchEffectValue1 != m_pClientList[iClientH]->m_sCharIDnum1) &&
					(m_pClientList[iClientH]->m_pItemList[i]->m_sTouchEffectValue2 != m_pClientList[iClientH]->m_sCharIDnum2) &&
					(m_pClientList[iClientH]->m_pItemList[i]->m_sTouchEffectValue3 != m_pClientList[iClientH]->m_sCharIDnum3)) {
					// ID °ªÀÌ ¸Â´Ù.

					// ÀÚ½ÅÀÇ °ÍÀÌ ¾Æ´Ï¹Ç·Î Âø¿ëÀÌ ÇØÁ¦µÈ´Ù.
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[i]->m_cEquipPos, i, NULL, NULL);
					ReleaseItemHandler(iClientH, i, TRUE);
					// v1.4 ´ë¹ÌÁö¸¦ ¾ò´Â´Ù. 
					iDamage = iDice(1, 100); // Centuu
					m_pClientList[iClientH]->m_iHP -= iDamage;
					if (m_pClientList[iClientH]->m_iHP <= 0) {
						ClientKilledHandler(iClientH, NULL, NULL, iDamage);
					}
				}
			}
		}
}

/*********************************************************************************************************************
** BOOL CGame::GetAngelHandler(int iClientH, char * pData, DWORD dwMsgSize)											**
** description   :: Reversed and coded by Snoopy																	**
*********************************************************************************************************************/
void CGame::GetAngelHandler(int iClientH, char* pData, DWORD dwMsgSize)
{
	char* cp, cData[256], cTmpName[21];
	int   iAngel, iItemNbe;
	class CItem* pItem;
	int   iRet, iEraseReq;
	short* sp;
	WORD* wp;
	int* ip;
	DWORD* dwp;
	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;
	// Centuu: Razones distintas por la cu�l no puedes obtener un angel.
	if (_iGetItemSpaceLeft(iClientH) == 0)
	{
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ANGEL_FAILED, 1, NULL, NULL, NULL); //"Impossible to get a Tutelary Angel."
		return;
	}
	if (m_pClientList[iClientH]->m_iGizonItemUpgradeLeft < 5)
	{
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ANGEL_FAILED, 2, NULL, NULL, NULL); //"You need additional Majesty Points."	
		return;
	}
	if (m_pClientList[iClientH]->m_iLevel < m_iPlayerMaxLevel)
	{
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ANGEL_FAILED, 3, NULL, NULL, NULL); //"Only Majesty characters can receive Tutelary Angel."
		return;
	}
	cp = (char*)(pData + DEF_INDEX2_MSGTYPE + 2);
	ZeroMemory(cTmpName, sizeof(cTmpName));
	strcpy(cTmpName, cp);
	cp += 20;
	ip = (int*)cp;
	iAngel = (int)*ip; // 0x00 l a i
	cp += 2;
	switch (iAngel) {
	case 1: // STR
		iItemNbe = 908;
		break;
	case 2: // DEX
		iItemNbe = 909;
		break;
	case 3: // INT
		iItemNbe = 910;
		break;
	case 4: // MAG
		iItemNbe = 911;
		break;
	default:
		PutLogList("Gail asked to create a wrong item!");
		break;
	}
	pItem = new class CItem;
	if (pItem == NULL) return;
	if ((_bInitItemAttr(pItem, iItemNbe) == TRUE))
	{
		pItem->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
		pItem->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
		pItem->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
		pItem->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;
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
			m_pClientList[iClientH]->m_iGizonItemUpgradeLeft -= 5;
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_GIZONITEMUPGRADELEFT, m_pClientList[iClientH]->m_iGizonItemUpgradeLeft, NULL, NULL, NULL);
			// Centuu : Recibe el angel.
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ANGEL_RECEIVED, NULL, NULL, NULL, NULL); //"You have received the Tutelary Angel."
		}
		else
		{
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, pItem);
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
**  void CGame::SetExchangeItem(int iClientH, int iItemIndex, int iAmount)											**
**  DESCRIPTION			:: set exchange item																		**
**  LAST_UPDATED		:: March 20, 2005; 6:31 PM; Hypnotoad														**
**	RETURN_VALUE		:: void																						**
**  NOTES				::	- make sure there that the item player places in trade exists							**
**							- stores item number in other character													**
**							- counts total item numbers sent														**
**  MODIFICATION		::	- fixed exchange bug by Rambox															**
**********************************************************************************************************************/
void CGame::SetExchangeItem(int iClientH, int iItemIndex, int iAmount)
{
	int iExH, i;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsOnServerChange == TRUE) return;
	if (m_pClientList[iClientH]->m_iExchangeCount > 8) return;	//only 4 items trade

	//no admin trade
	if ((m_bAdminSecurity == TRUE) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0)) {
		_ClearExchangeStatus(m_pClientList[iClientH]->m_iExchangeH);
		_ClearExchangeStatus(iClientH);
	}

	if ((m_pClientList[iClientH]->m_bIsExchangeMode == TRUE) && (m_pClientList[iClientH]->m_iExchangeH != NULL)) {
		iExH = m_pClientList[iClientH]->m_iExchangeH;
		if ((m_pClientList[iExH] == NULL) || (memcmp(m_pClientList[iClientH]->m_cExchangeName, m_pClientList[iExH]->m_cCharName, 10) != 0)) {
			// ��ȯ�� ������ ���ų� ��ȯ�ϰ��� �ߴ� �� ĳ���Ͱ� �ƴϴ�. 	
		}
		else {
			// ��ȯ�ϰ��� �ϴ� ���濡�� �������� �˷��ش�. 
			// �ش� �������� �����ϴ���, ������ �´��� üũ�Ѵ�. 
			if ((iItemIndex < 0) || (iItemIndex >= DEF_MAXITEMS)) return;
			if (m_pClientList[iClientH]->m_pItemList[iItemIndex] == NULL) return;
			if (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwCount < iAmount) return;

			//No Duplicate items
			for (i = 0; i < m_pClientList[iClientH]->m_iExchangeCount;i++) {
				if (m_pClientList[iClientH]->m_cExchangeItemIndex[i] == (char)iItemIndex) {
					_ClearExchangeStatus(iExH);
					_ClearExchangeStatus(iClientH);
					return;
				}
			}

			// ��ȯ�ϰ��� �ϴ� ������ �ε���, ���� ���� 
			m_pClientList[iClientH]->m_cExchangeItemIndex[m_pClientList[iClientH]->m_iExchangeCount] = (char)iItemIndex;
			m_pClientList[iClientH]->m_iExchangeItemAmount[m_pClientList[iClientH]->m_iExchangeCount] = iAmount;

			memcpy(m_pClientList[iClientH]->m_cExchangeItemName[m_pClientList[iClientH]->m_iExchangeCount], m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName, 20);



			m_pClientList[iClientH]->m_iExchangeCount++;
			SendNotifyMsg(iClientH, iClientH, DEF_NOTIFY_SETEXCHANGEITEM, iItemIndex + 1000, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName, iAmount, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan,
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan,
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 + 100,
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);

			SendNotifyMsg(iClientH, iExH, DEF_NOTIFY_SETEXCHANGEITEM, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName, iAmount, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan,
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan,
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 + 100,
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);
		}
	}
}

/*********************************************************************************************************************
**  void CGame::ConfirmExchangeItem(int iClientH)																	**
**  DESCRIPTION			:: confirms exchange																		**
**  LAST_UPDATED		:: March 20, 2005; 5:51 PM; Hypnotoad														**
**	RETURN_VALUE		:: void																						**
**  NOTES				::	- trade cancels if player changes HGServer												**
**							- trade cancels if player is disconnected or in waiting process							**
**							- trade cancels if player is trading with himself										**
**							- trade cancels if player is trading with noone											**
**							- trade cancels if player is recieves an item which hasnt been traded					**
**							- trade cancels if other player receives an item which you didn't trade					**
**							- changed weight calculations to support multiple items									**
**							- trade cancels if there is not enough item space for trade to occur					**
**  MODIFICATION		::	- GM can't trade if admin-security = true												**
**							- fixed exchange bug by Rambox															**
**********************************************************************************************************************/
void CGame::ConfirmExchangeItem(int iClientH)
{
	int iExH, i;
	int iItemWeightA, iItemWeightB, iWeightLeftA, iWeightLeftB, iAmountLeft;
	class CItem* pItemA[8], * pItemB[8], * pItemAcopy[8], * pItemBcopy[8];

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsOnServerChange == TRUE) return;
	if ((m_bAdminSecurity == TRUE) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0)) return;

	if ((m_pClientList[iClientH]->m_bIsExchangeMode == TRUE) && (m_pClientList[iClientH]->m_iExchangeH != NULL)) {
		iExH = m_pClientList[iClientH]->m_iExchangeH;

		// v1.42 ���� �̷� ��찡?
		if (iClientH == iExH) return;

		if (m_pClientList[iExH] != NULL) {
			if ((memcmp(m_pClientList[iClientH]->m_cExchangeName, m_pClientList[iExH]->m_cCharName, 10) != 0) ||
				(m_pClientList[iExH]->m_bIsExchangeMode != TRUE) ||
				(memcmp(m_pClientList[iExH]->m_cExchangeName, m_pClientList[iClientH]->m_cCharName, 10) != 0)) {
				// ��ȯ�ϰ��� �ߴ� �� ĳ���Ͱ� �ƴϴ�. ��ȯ ���´� ���.
				_ClearExchangeStatus(iClientH);
				_ClearExchangeStatus(iExH);
				return;
			}
			else {
				m_pClientList[iClientH]->m_bIsExchangeConfirm = TRUE;
				if (m_pClientList[iExH]->m_bIsExchangeConfirm == TRUE) {
					// ���浵 ��ȯ�ǻ縦 ������. ��ȯ�� ������ �� �ִ��� ����Ѵ�. ������ ����, ���� ������ �Ǵ�.

					//Check all items
					for (i = 0; i < m_pClientList[iClientH]->m_iExchangeCount; i++) {
						if ((m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]] == NULL) ||
							(memcmp(m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cName, m_pClientList[iClientH]->m_cExchangeItemName[i], 20) != 0)) {
							_ClearExchangeStatus(iClientH);
							_ClearExchangeStatus(iExH);
							return;
						}
					}
					for (i = 0; i < m_pClientList[iExH]->m_iExchangeCount; i++) {
						if ((m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]] == NULL) ||
							(memcmp(m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]]->m_cName, m_pClientList[iExH]->m_cExchangeItemName[i], 20) != 0)) {
							_ClearExchangeStatus(iClientH);
							_ClearExchangeStatus(iExH);
							return;
						}
					}

					iWeightLeftA = _iCalcMaxLoad(iClientH) - iCalcTotalWeight(iClientH);
					iWeightLeftB = _iCalcMaxLoad(iExH) - iCalcTotalWeight(iExH);

					//Calculate weight for items
					iItemWeightA = 0;
					for (i = 0; i < m_pClientList[iClientH]->m_iExchangeCount; i++) {
						iItemWeightA = iGetItemWeight(m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]],
							m_pClientList[iClientH]->m_iExchangeItemAmount[i]);
					}
					iItemWeightB = 0;
					for (i = 0; i < m_pClientList[iExH]->m_iExchangeCount; i++) {
						iItemWeightB = iGetItemWeight(m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]],
							m_pClientList[iExH]->m_iExchangeItemAmount[i]);
					}

					//See if the other person can take the item weightload
					if ((iWeightLeftA < iItemWeightB) || (iWeightLeftB < iItemWeightA)) {
						// ��ȯ�ϰ��� �ϴ� �������� ���� ���԰����� ����. ��ȯ �Ұ���. 
						_ClearExchangeStatus(iClientH);
						_ClearExchangeStatus(iExH);
						return;
					}

					for (i = 0; i < m_pClientList[iClientH]->m_iExchangeCount; i++) {
						// ���������� �ִ� �������� ���� ���� ������ ���Ѿ� ������ �ִ�.
						if ((m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
							(m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_ARROW)) {

							if (m_pClientList[iClientH]->m_iExchangeItemAmount[i] > m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_dwCount) {
								// ��ȯ�ϰ��� �ߴ� �������� �������� ����. �׵��� �پ�����.
								_ClearExchangeStatus(iClientH);
								_ClearExchangeStatus(iExH);
								return;
							}
							pItemA[i] = new class CItem;
							_bInitItemAttr(pItemA[i], m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cName);
							pItemA[i]->m_dwCount = m_pClientList[iClientH]->m_iExchangeItemAmount[i];

							// �α׸� ����� ���� ������ ���繰 
							pItemAcopy[i] = new class CItem;
							_bInitItemAttr(pItemAcopy[i], m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cName);
							bCopyItemContents(pItemAcopy[i], pItemA[i]);
							pItemAcopy[i]->m_dwCount = m_pClientList[iClientH]->m_iExchangeItemAmount[i];
						}
						else {
							pItemA[i] = (class CItem*)m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]];
							pItemA[i]->m_dwCount = m_pClientList[iClientH]->m_iExchangeItemAmount[i];

							// �α׸� ����� ���� ������ ���繰 
							pItemAcopy[i] = new class CItem;
							_bInitItemAttr(pItemAcopy[i], m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cName);
							bCopyItemContents(pItemAcopy[i], pItemA[i]);
							pItemAcopy[i]->m_dwCount = m_pClientList[iClientH]->m_iExchangeItemAmount[i];
						}
					}

					for (i = 0; i < m_pClientList[iExH]->m_iExchangeCount; i++) {
						if ((m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
							(m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_ARROW)) {

							if (m_pClientList[iExH]->m_iExchangeItemAmount[i] > m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]]->m_dwCount) {
								// ��ȯ�ϰ��� �ߴ� �������� �������� ����. �׵��� �پ�����.
								_ClearExchangeStatus(iClientH);
								_ClearExchangeStatus(iExH);
								return;
							}
							pItemB[i] = new class CItem;
							_bInitItemAttr(pItemB[i], m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]]->m_cName);
							pItemB[i]->m_dwCount = m_pClientList[iExH]->m_iExchangeItemAmount[i];

							// �α׸� ����� ���� ������ ���繰 
							pItemBcopy[i] = new class CItem;
							_bInitItemAttr(pItemBcopy[i], m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]]->m_cName);
							bCopyItemContents(pItemBcopy[i], pItemB[i]);
							pItemBcopy[i]->m_dwCount = m_pClientList[iExH]->m_iExchangeItemAmount[i];
						}
						else {
							pItemB[i] = (class CItem*)m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]];
							pItemB[i]->m_dwCount = m_pClientList[iExH]->m_iExchangeItemAmount[i];

							// �α׸� ����� ���� ������ ���繰 
							pItemBcopy[i] = new class CItem;
							_bInitItemAttr(pItemBcopy[i], m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]]->m_cName);
							bCopyItemContents(pItemBcopy[i], pItemB[i]);
							pItemBcopy[i]->m_dwCount = m_pClientList[iExH]->m_iExchangeItemAmount[i];
						}
					}

					// ���� �������� ���� �� 
					for (i = 0; i < m_pClientList[iExH]->m_iExchangeCount; i++) {
						bAddItem(iClientH, pItemB[i], NULL);
						_bItemLog(DEF_ITEMLOG_EXCHANGE, iExH, iClientH, pItemBcopy[i]);
						delete pItemBcopy[i];
						pItemBcopy[i] = NULL;
						if ((m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
							(m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_ARROW)) {
							//
							iAmountLeft = (int)m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]]->m_dwCount - m_pClientList[iExH]->m_iExchangeItemAmount[i];
							if (iAmountLeft < 0) iAmountLeft = 0;
							// v1.41 !!!
							SetItemCount(iExH, m_pClientList[iExH]->m_cExchangeItemIndex[i], iAmountLeft);

						}
						else {
							// ���� ������ �������̶�� �����Ѵ�.
							ReleaseItemHandler(iExH, m_pClientList[iExH]->m_cExchangeItemIndex[i], TRUE);
							SendNotifyMsg(NULL, iExH, DEF_NOTIFY_GIVEITEMFIN_ERASEITEM, m_pClientList[iExH]->m_cExchangeItemIndex[i], m_pClientList[iExH]->m_iExchangeItemAmount[i], NULL, m_pClientList[iClientH]->m_cCharName);
							m_pClientList[iExH]->m_pItemList[m_pClientList[iExH]->m_cExchangeItemIndex[i]] = NULL;
						}
					}

					for (i = 0; i < m_pClientList[iClientH]->m_iExchangeCount; i++) {
						bAddItem(iExH, pItemA[i], NULL);
						_bItemLog(DEF_ITEMLOG_EXCHANGE, iClientH, iExH, pItemAcopy[i]);
						delete pItemAcopy[i];
						pItemAcopy[i] = NULL;

						if ((m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
							(m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_cItemType == DEF_ITEMTYPE_ARROW)) {
							//
							iAmountLeft = (int)m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]]->m_dwCount - m_pClientList[iClientH]->m_iExchangeItemAmount[i];
							if (iAmountLeft < 0) iAmountLeft = 0;
							// v1.41 !!!
							SetItemCount(iClientH, m_pClientList[iClientH]->m_cExchangeItemIndex[i], iAmountLeft);

						}
						else {
							// ���� ������ �������̶�� �����Ѵ�.
							ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_cExchangeItemIndex[i], TRUE);
							SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_GIVEITEMFIN_ERASEITEM, m_pClientList[iClientH]->m_cExchangeItemIndex[i], m_pClientList[iClientH]->m_iExchangeItemAmount[i], NULL, m_pClientList[iExH]->m_cCharName);
							m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_cExchangeItemIndex[i]] = NULL;
						}
					}

					m_pClientList[iClientH]->m_bIsExchangeMode = FALSE;
					m_pClientList[iClientH]->m_bIsExchangeConfirm = FALSE;
					ZeroMemory(m_pClientList[iClientH]->m_cExchangeName, sizeof(m_pClientList[iClientH]->m_cExchangeName));
					m_pClientList[iClientH]->m_iExchangeH = NULL;
					m_pClientList[iClientH]->m_iExchangeCount = 0;

					m_pClientList[iExH]->m_bIsExchangeMode = FALSE;
					m_pClientList[iExH]->m_bIsExchangeConfirm = FALSE;
					ZeroMemory(m_pClientList[iExH]->m_cExchangeName, sizeof(m_pClientList[iExH]->m_cExchangeName));
					m_pClientList[iExH]->m_iExchangeH = NULL;
					m_pClientList[iExH]->m_iExchangeCount = 0;

					for (i = 0;i < 8;i++) {
						m_pClientList[iClientH]->m_cExchangeItemIndex[i] = -1;
						m_pClientList[iExH]->m_cExchangeItemIndex[i] = -1;
					}

					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_EXCHANGEITEMCOMPLETE, NULL, NULL, NULL, NULL);
					SendNotifyMsg(NULL, iExH, DEF_NOTIFY_EXCHANGEITEMCOMPLETE, NULL, NULL, NULL, NULL);

					// ���� �缳��
					iCalcTotalWeight(iClientH);
					iCalcTotalWeight(iExH);
					return;
				}
			}
		}
		else {
			// ��ȯ�� ������ ����. ��ȯ�� ��� �ȴ�.
			_ClearExchangeStatus(iClientH);
			return;
		}
	}
}

/*********************************************************************************************************************
**  void CGame::ExchangeItemHandler(int iClientH, short sItemIndex, int iAmount,									**
**									short dX, short dY, WORD wObjectID, char *pItemName)							**
**  DESCRIPTION			:: exchange item handler																	**
**  LAST_UPDATED		:: March 20, 2005; 6:31 PM; Hypnotoad														**
**	RETURN_VALUE		:: void																						**
**  NOTES				::	- stores item number																	**
**							- counts total item numbers																**
**  MODIFICATION		::	- fixed exchange bug by Rambox															**
**********************************************************************************************************************/
void CGame::ExchangeItemHandler(int iClientH, short sItemIndex, int iAmount, short dX, short dY, WORD wObjectID, char* pItemName)
{
	short sOwnerH;
	char  cOwnerType;
	int i;

	if (m_pClientList[iClientH] == NULL) return;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
	if ((m_bAdminSecurity == TRUE) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0)) return;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == NULL) return;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount < iAmount) return;
	if (m_pClientList[iClientH]->m_bIsOnServerChange == TRUE) return;
	if (m_pClientList[iClientH]->m_bIsExchangeMode == TRUE) return;
	if (wObjectID >= DEF_MAXCLIENTS) return;

	// �������� ������ ��ȯ�ϰڴٴ� �޽����� �����ߴ�. ���濡�� �˸��� ���� ��� ��ȯâ�� ������ �Ѵ�. 
	//���� dX, dY�� �ִ� ������Ʈ���� �Һ� �������� �ǳ��ش�. 
	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

	if ((sOwnerH != NULL) && (cOwnerType == DEF_OWNERTYPE_PLAYER)) {

		if ((m_bAdminSecurity == TRUE) && (m_pClientList[sOwnerH]->m_iAdminUserLevel > 0)) return;

		// v1.4 �ְ��� �� ��ü�� �´��� �Ǵ��Ѵ�.
		if (wObjectID != NULL) {
			if (wObjectID < 10000) {
				// �÷��̾� 
				if (m_pClientList[wObjectID] != NULL) {
					if ((WORD)sOwnerH != wObjectID) sOwnerH = NULL;
				}
			}
			else sOwnerH = NULL;
		}

		if ((sOwnerH == NULL) || (m_pClientList[sOwnerH] == NULL)) {
			//¾ÆÀÌÅÛ ±³È¯À» ¿äÃ»ÇÑ ÇÃ·¹ÀÌ¾î¿¡°Ô ÇØ´ç À§Ä¡¿¡ ÇÃ·¹ÀÌ¾î°¡ ¾øÀ½À» ¾Ë¸°´Ù. 
			_ClearExchangeStatus(iClientH);
		}
		else {
			if ((m_pClientList[sOwnerH]->m_bIsExchangeMode == TRUE) || (m_pClientList[sOwnerH]->m_sAppr2 & 0xF000) ||
				(m_pMapList[m_pClientList[sOwnerH]->m_cMapIndex]->m_bIsFightZone == TRUE)) {
				// »ó´ë¹æÀÌ ÀÌ¹Ì ±³È¯ ÁßÀÌ°Å³ª ÀüÅõ¸ðµå, È¤Àº »çÅõÀåÀÌ´Ù. ±³È¯¸ðµå·Î µé¾î°¥ ¼ö ¾ø´Ù. 
				_ClearExchangeStatus(iClientH);
			}
			else {
				// ±³È¯¸ðµå°¡ ½ÃÀÛµÇ¾ú´Ù. ÀÎµ¦½º, ÀÌ¸§ ÀúÀå  
				m_pClientList[iClientH]->m_bIsExchangeMode = TRUE;
				m_pClientList[iClientH]->m_iExchangeH = sOwnerH;
				ZeroMemory(m_pClientList[iClientH]->m_cExchangeName, sizeof(m_pClientList[iClientH]->m_cExchangeName));
				strcpy(m_pClientList[iClientH]->m_cExchangeName, m_pClientList[sOwnerH]->m_cCharName);

				//Clear items in the list
				m_pClientList[iClientH]->m_iExchangeCount = 0;
				m_pClientList[sOwnerH]->m_iExchangeCount = 0;
				for (i = 0; i < 8; i++) {
					//Clear the trader
					ZeroMemory(m_pClientList[iClientH]->m_cExchangeItemName[i], sizeof(m_pClientList[iClientH]->m_cExchangeItemName[i]));
					m_pClientList[iClientH]->m_cExchangeItemIndex[i] = -1;
					m_pClientList[iClientH]->m_iExchangeItemAmount[i] = 0;
					//Clear the guy we're trading with
					ZeroMemory(m_pClientList[sOwnerH]->m_cExchangeItemName[i], sizeof(m_pClientList[sOwnerH]->m_cExchangeItemName[i]));
					m_pClientList[sOwnerH]->m_cExchangeItemIndex[i] = -1;
					m_pClientList[sOwnerH]->m_iExchangeItemAmount[i] = 0;
				}

				// ±³È¯ÇÏ°íÀÚ ÇÏ´Â ¾ÆÀÌÅÛ ÀÎµ¦½º, ¼ö·® ÀúÀå 
				m_pClientList[iClientH]->m_cExchangeItemIndex[m_pClientList[iClientH]->m_iExchangeCount] = (char)sItemIndex;
				m_pClientList[iClientH]->m_iExchangeItemAmount[m_pClientList[iClientH]->m_iExchangeCount] = iAmount;

				memcpy(m_pClientList[iClientH]->m_cExchangeItemName[m_pClientList[iClientH]->m_iExchangeCount], m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName, 20);

				m_pClientList[sOwnerH]->m_bIsExchangeMode = TRUE;
				m_pClientList[sOwnerH]->m_iExchangeH = iClientH;
				ZeroMemory(m_pClientList[sOwnerH]->m_cExchangeName, sizeof(m_pClientList[sOwnerH]->m_cExchangeName));
				strcpy(m_pClientList[sOwnerH]->m_cExchangeName, m_pClientList[iClientH]->m_cCharName);

				m_pClientList[iClientH]->m_iExchangeCount++;
				SendNotifyMsg(iClientH, iClientH, DEF_NOTIFY_OPENEXCHANGEWINDOW, sItemIndex + 1000, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
					m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName, iAmount, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor,
					m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan,
					m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wMaxLifeSpan,
					m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2 + 100,
					m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute);

				SendNotifyMsg(iClientH, sOwnerH, DEF_NOTIFY_OPENEXCHANGEWINDOW, sItemIndex, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
					m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName, iAmount, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor,
					m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan,
					m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wMaxLifeSpan,
					m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2 + 100,
					m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute);
			}
		}
	}
	else {
		// NPC¿Í´Â ¹°°ÇÀ» ±³È¯ÇÒ ¼ö ¾ø´Ù.
		_ClearExchangeStatus(iClientH);
	}
}

/*********************************************************************************************************************
**  void CGame::_ClearExchangeStatus(int iToH)																		**
**  DESCRIPTION			:: clear exchange status																	**
**  LAST_UPDATED		:: March 20, 2005; 7:15 PM; Hypnotoad														**
**	RETURN_VALUE		:: void																						**
**  NOTES				::	- clears all trade variables															**
**  MODIFICATION		::	- fixed exchange bug by Rambox															**
**********************************************************************************************************************/
void CGame::_ClearExchangeStatus(int iToH)
{
	if ((iToH <= 0) || (iToH >= DEF_MAXCLIENTS)) return;
	if (m_pClientList[iToH] == NULL) return;

	if (m_pClientList[iToH]->m_cExchangeName != FALSE)
		SendNotifyMsg(NULL, iToH, DEF_NOTIFY_CANCELEXCHANGEITEM, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	m_pClientList[iToH]->m_dwInitCCTime = FALSE;
	m_pClientList[iToH]->m_iAlterItemDropIndex = 0;
	m_pClientList[iToH]->m_iExchangeH = NULL;

	m_pClientList[iToH]->m_bIsExchangeMode = FALSE;
	m_pClientList[iToH]->m_bIsExchangeConfirm = FALSE;

	ZeroMemory(m_pClientList[iToH]->m_cExchangeName, sizeof(m_pClientList[iToH]->m_cExchangeName));
}

void CGame::CancelExchangeItem(int iClientH)
{
	_ClearExchangeStatus(m_pClientList[iClientH]->m_iExchangeH);
	_ClearExchangeStatus(iClientH);
}

void CGame::RequestItemUpgradeHandler(int iClientH, int iItemIndex)
{
	int i, iItemX, iItemY, iSoM, iSoX, iSomH, iSoxH, iValue; // v2.172
	DWORD dwTemp, dwSWEType;
	double dV1, dV2, dV3;
	int sItemUpgrade = 2;

	if (m_pClientList[iClientH] == NULL) return;
	if ((iItemIndex < 0) || (iItemIndex >= DEF_MAXITEMS)) return;
	if (m_pClientList[iClientH]->m_pItemList[iItemIndex] == NULL) return;
	
	if (HeroItemChecker(m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum, 0, 0, 0) != 0) 
	{
		if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 != m_pClientList[iClientH]->m_sCharIDnum1) ||
			(m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 != m_pClientList[iClientH]->m_sCharIDnum2) ||
			(m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 != m_pClientList[iClientH]->m_sCharIDnum3)) {
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, NULL, NULL, NULL);
			return;
		}
		if (iUpgradeHeroCapeRequirements(iClientH, iItemIndex)) {
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_UPGRADEHEROCAPE, iItemIndex,
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType,
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan,
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName,
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame,
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2,
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);
			_bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, -1, m_pClientList[iClientH]->m_pItemList[iItemIndex], FALSE);
		}
		else {
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, NULL, NULL, NULL);
			_bItemLog(DEF_ITEMLOG_UPGRADEFAIL, iClientH, -1, m_pClientList[iClientH]->m_pItemList[iItemIndex], FALSE);
		}
		return;
	}

	iValue = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
	switch (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cCategory)
	{
		case 46: // Pendants are category 46
			if (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType != 1) 
			{	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, NULL, NULL, NULL);			
				return; // Pendants are type 1
			}
			if (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cEquipPos < 11) 
			{	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, NULL, NULL, NULL);			
				return; // Pendants are left finger or more
			}
			if (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemEffectType != 14) 
			{	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, NULL, NULL, NULL);			
				return; // Pendants are EffectType 14
			}
			switch (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemEffectValue1) 
			{
			default: // Other items are not upgradable
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, NULL, NULL, NULL);			
				return; // Pendants are EffectType 14

			case 16: // AngelicPandent(STR)
			case 17: // AngelicPandent(DEX)
			case 18: // AngelicPandent(INT)
			case 19: // AngelicPandent(MAG)
				if (m_pClientList[iClientH]->m_iGizonItemUpgradeLeft <= 0) 
				{	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, NULL, NULL, NULL);
					return;
				}
				if (iValue == 10)
				{	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 1, NULL, NULL, NULL);
					return;
				}
				switch (iValue) {
					case 0: sItemUpgrade = 10; break;
					case 1: sItemUpgrade = 11; break;
					case 2: sItemUpgrade = 13; break;
					case 3: sItemUpgrade = 16; break;
					case 4: sItemUpgrade = 20; break;
					case 5: sItemUpgrade = 25; break;
					case 6: sItemUpgrade = 31; break;
					case 7: sItemUpgrade = 38; break;
					case 8: sItemUpgrade = 46; break;
					case 9: sItemUpgrade = 55; break;
				}
				if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 != m_pClientList[iClientH]->m_sCharIDnum1)
					|| (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 != m_pClientList[iClientH]->m_sCharIDnum2)
					|| (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 != m_pClientList[iClientH]->m_sCharIDnum3))
				{
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, NULL, NULL, NULL);
					return;
				}
				if (( m_pClientList[iClientH]->m_iGizonItemUpgradeLeft - sItemUpgrade ) < 0) 
				{	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, NULL, NULL, NULL);
					return; 
				}
					m_pClientList[iClientH]->m_iGizonItemUpgradeLeft -= sItemUpgrade; 
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_GIZONITEMUPGRADELEFT, m_pClientList[iClientH]->m_iGizonItemUpgradeLeft, NULL, NULL, NULL);
					iValue++;
					dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
					dwTemp = dwTemp & 0x0FFFFFFF;
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28);
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
					_bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int) -1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
				
				break;
			}
			break;

	case 1:
		switch (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum)
		{
		case 709: // DarkKnightFlameberge
		case 737: // DarkKnightGiantSword
		case 745: // BlackKnightTemple
		case 989: // BlackKnightHammer
		case 990: // BlackKnightBH
		case 991: // BlackKnightBBH
			if (m_pClientList[iClientH]->m_iGizonItemUpgradeLeft <= 0) 
			{
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, NULL, NULL, NULL);
				return;
			}

			if (iValue == 15) 
			{
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 1, NULL, NULL, NULL);
				return;
			}

			sItemUpgrade = (iValue * (iValue + 6) / 8) + 2;
			if (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType != 0) 
			{
				if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 != m_pClientList[iClientH]->m_sCharIDnum1) ||
					(m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 != m_pClientList[iClientH]->m_sCharIDnum2) ||
					(m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 != m_pClientList[iClientH]->m_sCharIDnum3)) {
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, NULL, NULL, NULL);
					return;
				}
			}

			if ((m_pClientList[iClientH]->m_iGizonItemUpgradeLeft - sItemUpgrade) < 0)
			{
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, NULL, NULL, NULL);
				return;
			}

			m_pClientList[iClientH]->m_iGizonItemUpgradeLeft -= sItemUpgrade;
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_GIZONITEMUPGRADELEFT, m_pClientList[iClientH]->m_iGizonItemUpgradeLeft, NULL, NULL, NULL);

			if ((iValue == 0) && (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum == 709) )
			{ // DarkKnightFlameberge
				iItemX = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x;
				iItemY = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y;
				delete m_pClientList[iClientH]->m_pItemList[iItemIndex];
				m_pClientList[iClientH]->m_pItemList[iItemIndex] = NULL;
				m_pClientList[iClientH]->m_pItemList[iItemIndex] = new class CItem;
				m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x = iItemX;
				m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y = iItemY;
				if (_bInitItemAttr(m_pClientList[iClientH]->m_pItemList[iItemIndex], 737) == FALSE) 
				{
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
					return;
				}
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;
				iValue += 2;
				dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
				dwTemp = dwTemp & 0x0FFFFFFF;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28);
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);
				_bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
				break;
			}
			else if ((iValue >= 6) && (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum == 737))  // DarkKnightGiantSword
			{
				iItemX = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x;
				iItemY = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y;
				delete m_pClientList[iClientH]->m_pItemList[iItemIndex];
				m_pClientList[iClientH]->m_pItemList[iItemIndex] = NULL;
				m_pClientList[iClientH]->m_pItemList[iItemIndex] = new class CItem;
				m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x = iItemX;
				m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y = iItemY;
				if (_bInitItemAttr(m_pClientList[iClientH]->m_pItemList[iItemIndex], 745) == FALSE) 
				{
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
					return;
				}
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;
				iValue += 2;
				dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
				dwTemp = dwTemp & 0x0FFFFFFF;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28);
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);
				_bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
			}
			else if ((iValue >= 12) && (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum == 745))  //BlackKnightTemple
			{
				iValue += 2;
				if (iValue > 15) {
					iValue = 15;
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor = 9;
				}
				dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
				dwTemp = dwTemp & 0x0FFFFFFF;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28);
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);
				_bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
			}
			else if ((iValue == 0) && (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum == 989))
			{ // BlackKnightHammer
				iItemX = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x;
				iItemY = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y;
				delete m_pClientList[iClientH]->m_pItemList[iItemIndex];
				m_pClientList[iClientH]->m_pItemList[iItemIndex] = NULL;
				m_pClientList[iClientH]->m_pItemList[iItemIndex] = new class CItem;
				m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x = iItemX;
				m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y = iItemY;
				if (_bInitItemAttr(m_pClientList[iClientH]->m_pItemList[iItemIndex], 990) == FALSE)
				{
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
					return;
				}
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;
				iValue += 2;
				dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
				dwTemp = dwTemp & 0x0FFFFFFF;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28);
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);
				_bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
				break;
			}
			else if ((iValue >= 6) && (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum == 990))  // BlackKnightBH
			{
				iItemX = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x;
				iItemY = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y;
				delete m_pClientList[iClientH]->m_pItemList[iItemIndex];
				m_pClientList[iClientH]->m_pItemList[iItemIndex] = NULL;
				m_pClientList[iClientH]->m_pItemList[iItemIndex] = new class CItem;
				m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x = iItemX;
				m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y = iItemY;
				if (_bInitItemAttr(m_pClientList[iClientH]->m_pItemList[iItemIndex], 991) == FALSE)
				{
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
					return;
				}
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;
				iValue += 2;
				dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
				dwTemp = dwTemp & 0x0FFFFFFF;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28);
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);
				_bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
			}
			else if ((iValue >= 12) && (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum == 991))  //BlackKnightBBH
			{
				iValue += 2;
				if (iValue > 15) {
					iValue = 15;
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor = 9;
				}
				dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
				dwTemp = dwTemp & 0x0FFFFFFF;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28);
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);
				_bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
			}
			else 
			{
				iValue += 2;
				if (iValue > 15) iValue = 15;
				dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
				dwTemp = dwTemp & 0x0FFFFFFF;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28);
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
				_bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
			}
			break;

		case 717: // DarkKnightRapier
			if (m_pClientList[iClientH]->m_iGizonItemUpgradeLeft <= 0) {
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, NULL, NULL, NULL);
				return;
			}
			if (iValue == 7) {
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 1, NULL, NULL, NULL);
				return;
			}
			sItemUpgrade = (iValue * (iValue + 6) / 8) + 2;
			if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 != m_pClientList[iClientH]->m_sCharIDnum1) ||
				(m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 != m_pClientList[iClientH]->m_sCharIDnum2) ||
				(m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 != m_pClientList[iClientH]->m_sCharIDnum3)) {
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, NULL, NULL, NULL);
				return;
			}
			if ((m_pClientList[iClientH]->m_iGizonItemUpgradeLeft - sItemUpgrade) < 0) {
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, NULL, NULL, NULL);
				return;
			}
			m_pClientList[iClientH]->m_iGizonItemUpgradeLeft -= sItemUpgrade;
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_GIZONITEMUPGRADELEFT, m_pClientList[iClientH]->m_iGizonItemUpgradeLeft, NULL, NULL, NULL);
			iValue++;
			dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
			dwTemp = dwTemp & 0x0FFFFFFF;
			m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28);
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
			_bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
			break;

		default:
			if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00F00000) != NULL) {
				dwSWEType = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00F00000) >> 20;
				if (dwSWEType == 9) {
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, NULL, NULL, NULL);
					return;
				}
			}
			iSoX = iSoM = 0;
			for (i = 0; i < DEF_MAXITEMS; i++)
				if (m_pClientList[iClientH]->m_pItemList[i] != NULL) {
					switch (m_pClientList[iClientH]->m_pItemList[i]->m_sIDnum) {
					case 656: iSoX++; iSoxH = i; break;
					case 657: iSoM++; iSomH = i; break;
					}
				}
			if (iSoX > 0) {
				if (bCheckIsItemUpgradeSuccess(iClientH, iItemIndex, iSoxH) == FALSE) {
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
					iValue = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					if (iValue >= 1) ItemDepleteHandler(iClientH, iItemIndex, FALSE, TRUE);
					ItemDepleteHandler(iClientH, iSoxH, FALSE, TRUE);
					return;
				}
				if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00000001) != NULL) {
					iValue++;
					if (iValue > 10) {
						iValue = 10;
					}
					else {
						dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
						dwTemp = dwTemp & 0x0FFFFFFF;
						m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28);
						ItemDepleteHandler(iClientH, iSoxH, FALSE, TRUE);
					}
				}
				else {
					iValue++;
					if (iValue > 7) {
						iValue = 7;
					}
					else {
						dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
						dwTemp = dwTemp & 0x0FFFFFFF;
						m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28);
						ItemDepleteHandler(iClientH, iSoxH, FALSE, TRUE);
					}
				}
			}
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
			break;
		}
		return;

	case 3:
		if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00F00000) != NULL) {
			dwSWEType = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00F00000) >> 20;
			if (dwSWEType == 9) {
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, NULL, NULL, NULL);
				return;
			}
		}
		iSoX = iSoM = 0;
		for (i = 0; i < DEF_MAXITEMS; i++)
			if (m_pClientList[iClientH]->m_pItemList[i] != NULL) {
				switch (m_pClientList[iClientH]->m_pItemList[i]->m_sIDnum) {
				case 656: iSoX++; iSoxH = i; break;
				case 657: iSoM++; iSomH = i; break;
				}
			}
		if (iSoX > 0) {
			if (bCheckIsItemUpgradeSuccess(iClientH, iItemIndex, iSoxH) == FALSE) {
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
				iValue = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
				if (iValue >= 1) ItemDepleteHandler(iClientH, iItemIndex, FALSE, TRUE);
				ItemDepleteHandler(iClientH, iSoxH, FALSE, TRUE);
				return;
			}
			if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00000001) != NULL) {
				iValue++;
				if (iValue > 10) {
					iValue = 10;
				}
				else {
					dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
					dwTemp = dwTemp & 0x0FFFFFFF;
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28);
					ItemDepleteHandler(iClientH, iSoxH, FALSE, TRUE);
				}
			}
			else {
				iValue++;
				if (iValue > 7) {
					iValue = 7;
				}
				else {
					dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
					dwTemp = dwTemp & 0x0FFFFFFF;
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28);
					ItemDepleteHandler(iClientH, iSoxH, FALSE, TRUE);
				}
			}
		}
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
		return;

	case 5:
		if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00F00000) != NULL) {
			dwSWEType = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00F00000) >> 20;
			if (dwSWEType == 8) {
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, NULL, NULL, NULL);
				return;
			}
		}
		switch (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum) {
		case 623:
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, NULL, NULL, NULL);
			return;
		default:
			break;
		}
		iSoX = iSoM = 0;
		for (i = 0; i < DEF_MAXITEMS; i++)
			if (m_pClientList[iClientH]->m_pItemList[i] != NULL) {
				switch (m_pClientList[iClientH]->m_pItemList[i]->m_sIDnum) {
				case 656: iSoX++; iSoxH = i; break;
				case 657: iSoM++; iSomH = i; break;
				}
			}
		if (iSoM > 0) {
			if (bCheckIsItemUpgradeSuccess(iClientH, iItemIndex, iSomH, TRUE) == FALSE) {
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
				iValue = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
				if (iValue >= 1) ItemDepleteHandler(iClientH, iItemIndex, FALSE, TRUE);
				ItemDepleteHandler(iClientH, iSomH, FALSE, TRUE);
				return;
			}
			iValue++;
			if (iValue > 10) {
				iValue = 10;
			}
			else {
				dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
				dwTemp = dwTemp & 0x0FFFFFFF;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28);
				if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00000001) != NULL) {
					dV1 = (double)m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan;
					dV2 = 0.2f * dV1;
					dV3 = dV1 + dV2;
				}
				else {
					dV1 = (double)m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan;
					dV2 = 0.15f * dV1;
					dV3 = dV1 + dV2;
				}
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1 = (short)dV3;
				if (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1 < 0) m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1 = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan += dV2;
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CURLIFESPAN, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan, NULL, NULL);
				ItemDepleteHandler(iClientH, iSomH, FALSE, TRUE);
			}
		}
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1, NULL, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2);
		return;

	case  6:
	case 15:
		switch (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum) {
		case 621: // MerienPlateMailM
		case 622: // MerienPlateMailW 
		case 700: // SangAhHauberk 
		case 701: // SangAhFullHel
		case 702: // SangAhLeggings  
		case 704: // SangAhPlateMail 
		case 706: // DarkKnightHauberk
		case 707: // DarkKnightFullHelm 
		case 708: // DarkKnightLeggings  
		case 710: // DarkKnightPlateMail 
		case 711: // DarkMageHauberk 
		case 712: // DarkMageChainMail
		case 713: // DarkMageLeggings  
		case 716: // DarkMageLedderArmor
		case 719: // DarkMageScaleMail
		case 724: // DarkKnightHauberkW
		case 725: // DarkKnightFullHelmW 
		case 726: // DarkKnightLeggingsW 
		case 728: // DarkKnightPlateMailW 
		case 729: // DarkMageHauberkW 
		case 730: // DarkMageChainMailW 
		case 731: // DarkMageLeggingsW 
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, NULL, NULL, NULL);
			return;

		default:
			if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00F00000) != NULL) {
				dwSWEType = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00F00000) >> 20;
				if (dwSWEType == 8) {
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, NULL, NULL, NULL);
					return;
				}
			}
			iSoX = iSoM = 0;
			for (i = 0; i < DEF_MAXITEMS; i++)
				if (m_pClientList[iClientH]->m_pItemList[i] != NULL) {
					switch (m_pClientList[iClientH]->m_pItemList[i]->m_sIDnum) {
					case 656: iSoX++; iSoxH = i; break;
					case 657: iSoM++; iSomH = i; break;
					}
				}
			if (iSoM > 0) {
				if (bCheckIsItemUpgradeSuccess(iClientH, iItemIndex, iSomH, TRUE) == FALSE) {
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
					iValue = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0xF0000000) >> 28;
					if (iValue >= 1) ItemDepleteHandler(iClientH, iItemIndex, FALSE, TRUE);
					ItemDepleteHandler(iClientH, iSomH, FALSE, TRUE);
					return;
				}
				iValue++;
				if (iValue > 10) {
					iValue = 10;
				}
				else {
					dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
					dwTemp = dwTemp & 0x0FFFFFFF;
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28);
					if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00000001) != NULL) {
						dV1 = (double)m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan;
						dV2 = 0.2f * dV1;
						dV3 = dV1 + dV2;
					}
					else {
						dV1 = (double)m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan;
						dV2 = 0.15f * dV1;
						dV3 = dV1 + dV2;
					}
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1 = (short)dV3;
					if (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1 < 0) m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1 = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan;
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wMaxLifeSpan = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1;
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan += dV2;
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CURLIFESPAN, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan, NULL, NULL);
					ItemDepleteHandler(iClientH, iSomH, FALSE, TRUE);
				}
			}
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue1, NULL, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2);

			break;
		}
		return;

	case 8:
		switch (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum) {
			// centu - MagicWand(MS30-LLF) ??? 
		case 714: // DarkMageMagicStaff 
		case 732: // DarkMageMagicStaffW 
		case 738: // DarkMageMagicWand
		case 746: // BlackMageTemple
			if ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 != m_pClientList[iClientH]->m_sCharIDnum1) ||
				(m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 != m_pClientList[iClientH]->m_sCharIDnum2) ||
				(m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 != m_pClientList[iClientH]->m_sCharIDnum3)) {
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 2, NULL, NULL, NULL);
				return;
			}
			if (m_pClientList[iClientH]->m_iGizonItemUpgradeLeft <= 0) {
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, NULL, NULL, NULL);
				return;
			}
			if (iValue == 15) {
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 1, NULL, NULL, NULL);
				return;
			}
			sItemUpgrade = (iValue * (iValue + 6) / 8) + 2;
			if ((m_pClientList[iClientH]->m_iGizonItemUpgradeLeft - sItemUpgrade) < 0) {
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMUPGRADEFAIL, 3, NULL, NULL, NULL);
				return;
			}
			m_pClientList[iClientH]->m_iGizonItemUpgradeLeft -= sItemUpgrade;
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_GIZONITEMUPGRADELEFT, m_pClientList[iClientH]->m_iGizonItemUpgradeLeft, NULL, NULL, NULL);
			if (iValue == 0) {
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;
			}
			if ((iValue >= 4) && ((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum == 714) || (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum == 732))) {
				iItemX = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x;
				iItemY = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y;
				delete m_pClientList[iClientH]->m_pItemList[iItemIndex];
				m_pClientList[iClientH]->m_pItemList[iItemIndex] = NULL;
				m_pClientList[iClientH]->m_pItemList[iItemIndex] = new class CItem;
				m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x = iItemX;
				m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y = iItemY;
				if (_bInitItemAttr(m_pClientList[iClientH]->m_pItemList[iItemIndex], 738) == FALSE) { // DarkMageMagicWand
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
					return;
				}
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;
				iValue += 2;
				dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
				dwTemp = dwTemp & 0x0FFFFFFF;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28);
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);
				_bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
				break;
			}
			if ((iValue >= 6) && (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum == 738)) {
				iItemX = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x;
				iItemY = m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y;
				delete m_pClientList[iClientH]->m_pItemList[iItemIndex];
				m_pClientList[iClientH]->m_pItemList[iItemIndex] = NULL;
				m_pClientList[iClientH]->m_pItemList[iItemIndex] = new class CItem;
				m_pClientList[iClientH]->m_ItemPosList[iItemIndex].x = iItemX;
				m_pClientList[iClientH]->m_ItemPosList[iItemIndex].y = iItemY;
				if (_bInitItemAttr(m_pClientList[iClientH]->m_pItemList[iItemIndex], 746) == FALSE) { // BlackMageTemple 
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
					return;
				}
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;
				iValue += 2;
				dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
				dwTemp = dwTemp & 0x0FFFFFFF;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28);
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);
				_bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
				break;
			}
			if ((iValue >= 12) && (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum == 746)) {
				iValue += 2;
				if (iValue > 15) {
					iValue = 15;
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor = 9;
				}
				dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
				dwTemp = dwTemp & 0x0FFFFFFF;
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28);
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_GIZONITEMCANGE, iItemIndex,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemType,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_wCurLifeSpan,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cName,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSprite,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sSpriteFrame,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_cItemColor,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2,
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute);
				_bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
				break;
			}
			else {
				iValue += 2;
				if (iValue > 15) iValue = 15;
				dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
				dwTemp = dwTemp & 0x0FFFFFFF; // ºñÆ® Å¬¸®¾î 
				m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28); // ¾÷±×·¹ÀÌµåµÈ ºñÆ®°ª ÀÔ·Â
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
				_bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
				break;
			}
			break;

		default:
			// ÀÏ¹Ý ¾ÆÀÌÅÛ
			// v2.16 2002-5-21 °í±¤Çö¼öÁ¤
			iSoX = iSoM = 0;
			for (i = 0; i < DEF_MAXITEMS; i++)
				if (m_pClientList[iClientH]->m_pItemList[i] != NULL) {
					switch (m_pClientList[iClientH]->m_pItemList[i]->m_sIDnum) {
					case 656: iSoX++; iSoxH = i; break; // ½ºÅæ ¿Àºê Á¦¸®¸¶ 
					case 657: iSoM++; iSomH = i; break; // ½ºÅæ ¿Àºê ¸Þ¸®¿£ 
					}
				}
			// ½ºÅæ ¿Àºê Á¦¸®¸¶°¡ ÀÖ´Ù.
			if (iSoX > 0) {
				// ¾÷±×·¹ÀÌµå ¼º°ø È®·ü °è»ê.
				if (bCheckIsItemUpgradeSuccess(iClientH, iItemIndex, iSoxH) == FALSE) {
					// ¾÷±×·¹ÀÌµå ½ÇÆÐ 
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
					iValue = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0xF0000000) >> 28; // v2.172
					if (iValue >= 1) ItemDepleteHandler(iClientH, iItemIndex, FALSE, TRUE); // v2.172 +1 -> +2 ´Ü°è¿¡¼­ ½ÇÆÐÇÏ¸é »ç¶óÁü 
					// ½ºÅæ ¿Àºê Á¦¸®¸¶ ¾ø¾Ø´Ù.
					ItemDepleteHandler(iClientH, iSoxH, FALSE, TRUE);
					return;
				}

				iValue++;
				if (iValue > 7)
					iValue = 7;
				else {
					// ¾÷±×·¹ÀÌµå ¼º°ø. ¾ÆÀÌÅÛ Æ¯¼º ¹Ù²Ù°í
					dwTemp = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute;
					dwTemp = dwTemp & 0x0FFFFFFF; // ºñÆ® Å¬¸®¾î 
					m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute = dwTemp | (iValue << 28); // ¾÷±×·¹ÀÌµåµÈ ºñÆ®°ª ÀÔ·Â	
					// ½ºÅæ ¿Àºê Á¦¸®¸¶ ¾ø¾Ø´Ù.
					ItemDepleteHandler(iClientH, iSoxH, FALSE, TRUE);
				}
			}

			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);

			break;
		}
		break;

	default:
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMATTRIBUTECHANGE, iItemIndex, m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute, NULL, NULL);
		break;
	}
}

int CGame::HeroItemChecker(int iItemID, short sEnemyKill, char cContribution, char cSide)
{

	switch (iItemID) {

	case 400: // AresdenHeroCape 
		if (sEnemyKill != 0) sEnemyKill = 300;
		if (cContribution != 0) cContribution = 0;
		if (cSide != 0) cSide = 1;
		break;

	case 401: // ElvineHeroCape 
		if (sEnemyKill != 0) sEnemyKill = 300;
		if (cContribution != 0) cContribution = 0;
		if (cSide != 0) cSide = 2;
		break;

		// eHeroHelm
	case 403: // aHeroHelm(M)
	case 404: // aHeroHelm(W)
		if (sEnemyKill != 0) sEnemyKill = 150;
		if (cContribution != 0) cContribution = 20;
		if (cSide != 0) cSide = 1;
		break;
	case 405: // aHeroHelm(M)
	case 406: // aHeroHelm(W)
		if (sEnemyKill != 0) sEnemyKill = 150;
		if (cContribution != 0) cContribution = 20;
		if (cSide != 0) cSide = 2;
		break;

		// eHeroCap
	case 407: // aHeroCap(M)
	case 408: // aHeroCap(W)
		if (sEnemyKill != 0) sEnemyKill = 100;
		if (cContribution != 0) cContribution = 20;
		if (cSide != 0) cSide = 1;
		break;
	case 409: // aHeroCap(M)
	case 410: // aHeroCap(W)
		if (sEnemyKill != 0) sEnemyKill = 100;
		if (cContribution != 0) cContribution = 20;
		if (cSide != 0) cSide = 2;
		break;

		// eHeroArmor
	case 411: // eHeroArmor(M)
	case 412: // eHeroArmor(W)
		if (sEnemyKill != 0) sEnemyKill = 300;
		if (cContribution != 0) cContribution = 30;
		if (cSide != 0) cSide = 1;
		break;
	case 413: // eHeroArmor(M)
	case 414: // eHeroArmor(W)
		if (sEnemyKill != 0) sEnemyKill = 300;
		if (cContribution != 0) cContribution = 30;
		if (cSide != 0) cSide = 2;
		break;

		// aHeroRobe
	case 415: // aHeroRobe(M)
	case 416: // aHeroRobe(W)
		if (sEnemyKill != 0) sEnemyKill = 200;
		if (cContribution != 0) cContribution = 20;
		if (cSide != 0) cSide = 1;
		break;
	case 417: // aHeroRobe(M)
	case 418: // aHeroRobe(W)
		if (sEnemyKill != 0) sEnemyKill = 200;
		if (cContribution != 0) cContribution = 20;
		if (cSide != 0) cSide = 2;
		break;

		// aHeroHauberk
	case 419: // aHeroHauberk(M)
	case 420: // aHeroHauberk(W)
		if (sEnemyKill != 0) sEnemyKill = 100;
		if (cContribution != 0) cContribution = 10;
		if (cSide != 0) cSide = 1;
		break;
	case 421: // eHeroHauberk(M)
	case 422: // eHeroHauberk(W)
		if (sEnemyKill != 0) sEnemyKill = 100;
		if (cContribution != 0) cContribution = 10;
		if (cSide != 0) cSide = 2;
		break;

		// aHeroLeggings
	case 423: // aHeroLeggings(M)
	case 424: // aHeroLeggings(W)
		if (sEnemyKill != 0) sEnemyKill = 150;
		if (cContribution != 0) cContribution = 15;
		if (cSide != 0) cSide = 1;
		break;
	case 425: // eHeroLeggings(M)
	case 426: // eHeroLeggings(W)
		if (sEnemyKill != 0) sEnemyKill = 150;
		if (cContribution != 0) cContribution = 15;
		if (cSide != 0) cSide = 2;
		break;

	case 427: // AresdenHeroCape+1 
		if (cSide != 0) cSide = 1;
		break;

	case 428: // ElvineHeroCape+1
		if (cSide != 0) cSide = 2;
		break;

	default:
		return 0;
	}
	return iItemID;

}

BOOL CGame::iUpgradeHeroCapeRequirements(int iClientH, int iItemIndex)
{
	int iAfterItemID, iRequiredEnemyKills, iRequiredContribution, iStoneNumber, i;
	int iBeforeItemID;

	iAfterItemID = 0;
	iRequiredEnemyKills = 30;
	iRequiredContribution = 50;
	iStoneNumber = 0;
	i = 0;
	iBeforeItemID = m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum;
	if (iBeforeItemID == 400) {
		_bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)m_pClientList[iClientH]->m_pItemList[iItemIndex], FALSE);
		iAfterItemID = 427;
		iStoneNumber = 657;
	}
	else if (iBeforeItemID == 401) {
		_bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)m_pClientList[iClientH]->m_pItemList[iItemIndex], FALSE);
		iAfterItemID = 428;
		iStoneNumber = 657;
	}
	else {
		return FALSE;
	}
	if (iRequiredEnemyKills > m_pClientList[iClientH]->m_iEnemyKillCount) return FALSE;
	if (iRequiredContribution > m_pClientList[iClientH]->m_iContribution) return FALSE;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if ((m_pClientList[iClientH]->m_pItemList[i] != NULL) && (m_pClientList[iClientH]->m_pItemList[i]->m_sIDnum == iStoneNumber)) break;

	if ((i == DEF_MAXITEMS) || (iStoneNumber == 0)) return FALSE;
	if (_bInitItemAttr(m_pClientList[iClientH]->m_pItemList[iItemIndex], iAfterItemID) == FALSE) return FALSE;
	m_pClientList[iClientH]->m_iEnemyKillCount -= iRequiredEnemyKills;
	m_pClientList[iClientH]->m_iContribution -= iRequiredContribution;
	if (m_pClientList[iClientH]->m_pItemList[i] != NULL) {
		ItemDepleteHandler(iClientH, i, FALSE, TRUE);
		return TRUE;
	}
	return FALSE;
}

//50Cent - Repair All
void CGame::RequestRepairAllItemsHandler(int iClientH)
{
	int i, price;
	double d1, d2, d3;
	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;

	m_pClientList[iClientH]->totalItemRepair = 0;

	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemList[i] != NULL)
			if (((m_pClientList[iClientH]->m_pItemList[i]->m_cCategory >= 1) && (m_pClientList[iClientH]->m_pItemList[i]->m_cCategory <= 12)) ||
				((m_pClientList[iClientH]->m_pItemList[i]->m_cCategory >= 43) && (m_pClientList[iClientH]->m_pItemList[i]->m_cCategory <= 50)))
			{
				if (m_pClientList[iClientH]->m_pItemList[i]->m_wCurLifeSpan == m_pClientList[iClientH]->m_pItemList[i]->m_wMaxLifeSpan)
					continue;
				if (m_pClientList[iClientH]->m_pItemList[i]->m_wCurLifeSpan <= 0)
					price = (m_pClientList[iClientH]->m_pItemList[i]->m_wPrice / 2);
				else
				{
					d1 = (double)(m_pClientList[iClientH]->m_pItemList[i]->m_wCurLifeSpan);
					if (m_pClientList[iClientH]->m_pItemList[i]->m_wMaxLifeSpan != 0)
						d2 = (double)(m_pClientList[iClientH]->m_pItemList[i]->m_wMaxLifeSpan);
					else
						d2 = 1.0f;
					d3 = ((d1 / d2) * 0.5f);
					d2 = (double)(m_pClientList[iClientH]->m_pItemList[i]->m_wPrice);
					d3 = (d3 * d2);
					price = ((m_pClientList[iClientH]->m_pItemList[i]->m_wPrice / 2) - (short)(d3));
				}
				m_pClientList[iClientH]->m_stRepairAll[m_pClientList[iClientH]->totalItemRepair].index = i;
				m_pClientList[iClientH]->m_stRepairAll[m_pClientList[iClientH]->totalItemRepair].price = price;
				m_pClientList[iClientH]->totalItemRepair++;
			}
	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_REPAIRALLPRICES, NULL, NULL, NULL, NULL);
}

void CGame::RequestRepairAllItemsDeleteHandler(int iClientH, int index)
{
	int i;
	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;

	for (i = index; i < m_pClientList[iClientH]->totalItemRepair; i++)
		m_pClientList[iClientH]->m_stRepairAll[i] = m_pClientList[iClientH]->m_stRepairAll[i + 1];
	m_pClientList[iClientH]->totalItemRepair--;
	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_REPAIRALLPRICES, NULL, NULL, NULL, NULL);
}

void CGame::RequestRepairAllItemsConfirmHandler(int iClientH)
{
	char* cp, cData[120];
	DWORD* dwp;
	WORD* wp;
	int      iRet, i, totalPrice = 0;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;
	if (m_pClientList[iClientH]->m_pIsProcessingAllowed == FALSE) return;

	for (i = 0; i < m_pClientList[iClientH]->totalItemRepair; i++)
		totalPrice += m_pClientList[iClientH]->m_stRepairAll[i].price;

	if (dwGetItemCount(iClientH, "Gold") < (DWORD)totalPrice)
	{
		dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_NOTENOUGHGOLD;
		cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);
		*cp = 0;
		cp++;

		iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 7);
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
		for (i = 0; i < m_pClientList[iClientH]->totalItemRepair; i++)
		{
			m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_stRepairAll[i].index]->m_wCurLifeSpan = m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_stRepairAll[i].index]->m_wMaxLifeSpan;
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMREPAIRED, m_pClientList[iClientH]->m_stRepairAll[i].index, m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_stRepairAll[i].index]->m_wCurLifeSpan, NULL, NULL);
		}
		iCalcTotalWeight(SetItemCount(iClientH, "Gold", dwGetItemCount(iClientH, "Gold") - totalPrice));
	}
}


// MORLA 2.4 - Trade Items
void CGame::RequestPurchaseItemHandler2(int iClientH, char* pItemName, int iNum)
{
	class CItem* pItem;
	char* cp, cItemName[21], cData[100];
	short* sp;
	DWORD* dwp, dwItemCount;
	WORD* wp, wTempPrice;
	int   i, iRet, iEraseReq;
	int iEKCost = 0, iDPCost = 0;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;



	ZeroMemory(cData, sizeof(cData));
	ZeroMemory(cItemName, sizeof(cItemName));
	// MORLA 2.5 - Trade items x EKs
	if (memcmp(pItemName, "XelimaBlade", 11) == 0) { iDPCost = 0; iEKCost = 3500; }
	if (memcmp(pItemName, "XelimaAxe", 9) == 0) { iDPCost = 0; iEKCost = 4500; }
	if (memcmp(pItemName, "XelimaRapier", 12) == 0) { iDPCost = 0; iEKCost = 2000; }
	if (memcmp(pItemName, "KlonessBlade", 12) == 0) { iDPCost = 0; iEKCost = 3500; }
	if (memcmp(pItemName, "KlonessAxe", 10) == 0) { iDPCost = 0; iEKCost = 3500; }
	if (memcmp(pItemName, "KlonessEsterk", 13) == 0) { iDPCost = 0; iEKCost = 3000; }
	if (memcmp(pItemName, "BerserkWand(MS.20)", 18) == 0) { iDPCost = 0; iEKCost = 4500; }
	if (memcmp(pItemName, "KlonessWand(MS.20)", 18) == 0) { iDPCost = 0; iEKCost = 2000; }
	if (memcmp(pItemName, "ResurWand(MS.20)", 16) == 0) { iDPCost = 0; iEKCost = 2000; }
	if (memcmp(pItemName, "I.M.CManual", 11) == 0) { iDPCost = 0; iEKCost = 3000; }
	if (memcmp(pItemName, "MerienShield", 12) == 0) { iDPCost = 0; iEKCost = 6000; }
	if (memcmp(pItemName, "MerienPlateMailM", 16) == 0) { iDPCost = 0; iEKCost = 4000; }
	if (memcmp(pItemName, "MerienPlateMailW", 16) == 0) { iDPCost = 0; iEKCost = 4000; }
	if (memcmp(pItemName, "SwordofMedusa", 13) == 0) { iDPCost = 0; iEKCost = 5000; }
	if (memcmp(pItemName, "DemonSlayer", 11) == 0) { iDPCost = 0; iEKCost = 1000; }
	if (memcmp(pItemName, "DarkElfBow", 10) == 0) { iDPCost = 0; iEKCost = 500; }
	if (memcmp(pItemName, "GiantBattleHammer", 17) == 0) { iDPCost = 0; iEKCost = 6000; }
	if (memcmp(pItemName, "StormBringer", 12) == 0) { iDPCost = 0; iEKCost = 4000; }
	if (memcmp(pItemName, "DarkExecutor", 12) == 0) { iDPCost = 0; iEKCost = 4500; }
	if (memcmp(pItemName, "LightingBlade", 13) == 0) { iDPCost = 0; iEKCost = 4500; }
	if (memcmp(pItemName, "RingoftheAbaddon", 16) == 0) { iDPCost = 0; iEKCost = 2500; }
	if (memcmp(pItemName, "RingofArcmage", 13) == 0) { iDPCost = 0; iEKCost = 2500; }
	if (memcmp(pItemName, "NecklaceOfMerien", 16) == 0) { iDPCost = 0; iEKCost = 4000; }
	if (memcmp(pItemName, "NecklaceOfXelima", 16) == 0) { iDPCost = 0; iEKCost = 4000; }
	if (memcmp(pItemName, "KnecklaceOfMedusa", 17) == 0) { iDPCost = 0; iEKCost = 5000; }
	if (memcmp(pItemName, "KnecklaceOfIceEle", 17) == 0) { iDPCost = 0; iEKCost = 2000; }
	if (memcmp(pItemName, "HeroNeckMage(MD+35)", 19) == 0) { iDPCost = 0; iEKCost = 4000; }
	if (memcmp(pItemName, "HeroNeckWar(DM+35)", 18) == 0) { iDPCost = 0; iEKCost = 4000; }
	if (memcmp(pItemName, "RingofHeroMage", 14) == 0) { iDPCost = 0; iEKCost = 4000; }
	if (memcmp(pItemName, "RingOfHeroWar", 13) == 0) { iDPCost = 0; iEKCost = 4000; }
	if (memcmp(pItemName, "HeroWand", 8) == 0) { iDPCost = 0; iEKCost = 4000; }

	// MORLA 2.5 - Trade items x DeathMach Points
	if (memcmp(pItemName, "ZemstoneofSacrifice", 19) == 0) { iDPCost = 3; iEKCost = 0; }
	if (memcmp(pItemName, "StoneOfXelima", 13) == 0) { iDPCost = 4; iEKCost = 0; }
	if (memcmp(pItemName, "StoneOfMerien", 13) == 0) { iDPCost = 4; iEKCost = 0; }
	if (memcmp(pItemName, "RingofDemonpower", 16) == 0) { iDPCost = 10; iEKCost = 0; }
	if (memcmp(pItemName, "RingofGrandMage", 15) == 0) { iDPCost = 10; iEKCost = 0; }
	if (memcmp(pItemName, "MagicWand(MS30-LLF)", 18) == 0) { iDPCost = 12; iEKCost = 0; }
	if (memcmp(pItemName, "HuntSword", 9) == 0) { iDPCost = 100; iEKCost = 0; }
	if (memcmp(pItemName, "GladeitorHauberk", 16) == 0) { iDPCost = 50; iEKCost = 0; }
	if (memcmp(pItemName, "GladeitorBerkW", 14) == 0) { iDPCost = 50; iEKCost = 0; }
	if (memcmp(pItemName, "GladeitorHelmM", 14) == 0) { iDPCost = 60; iEKCost = 0; }
	if (memcmp(pItemName, "GladeitorHelmW", 14) == 0) { iDPCost = 60; iEKCost = 0; }
	if (memcmp(pItemName, "GladeitorHatM", 13) == 0) { iDPCost = 30; iEKCost = 0; }
	if (memcmp(pItemName, "GladeitorHatW", 13) == 0) { iDPCost = 30; iEKCost = 0; }
	if (memcmp(pItemName, "GladeitorLeggs", 14) == 0) { iDPCost = 60; iEKCost = 0; }
	if (memcmp(pItemName, "GladeitorLeggsW", 15) == 0) { iDPCost = 60; iEKCost = 0; }
	if (memcmp(pItemName, "GladeitorMailM", 14) == 0) { iDPCost = 70; iEKCost = 0; }
	if (memcmp(pItemName, "GladeitorMailW", 14) == 0) { iDPCost = 70; iEKCost = 0; }
	if (memcmp(pItemName, "GladeitorAxe", 12) == 0) { iDPCost = 80; iEKCost = 0; }
	if (memcmp(pItemName, "GladeitorWandMS70", 17) == 0) { iDPCost = 40; iEKCost = 0; }

	if ((m_pClientList[iClientH]->m_iEnemyKillCount < iEKCost)
		|| (m_pClientList[iClientH]->m_iDGPoints < iDPCost)) return;

	if ((iDPCost == 0) && (iEKCost == 0)) return;

	memcpy(cItemName, pItemName, 20);
	dwItemCount = 1;

	for (i = 1; i <= iNum; i++) {

		pItem = new class CItem;
		if (_bInitItemAttr(pItem, cItemName) == FALSE) {
			delete pItem;
		}
		else {

			if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == TRUE) {
				if (m_pClientList[iClientH]->m_iCurWeightLoad < 0) m_pClientList[iClientH]->m_iCurWeightLoad = 0;

				dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_ITEMTRADE;

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

				wp = (WORD*)cp;
				*wp = iEKCost;
				wTempPrice = iEKCost;
				cp += 2;

				if (iEraseReq == 1) delete pItem;


				m_pClientList[iClientH]->m_iEnemyKillCount -= iEKCost;
				m_pClientList[iClientH]->m_iDGPoints -= iDPCost;
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ENEMYKILLS, m_pClientList[iClientH]->m_iEnemyKillCount, NULL, NULL, NULL);
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_REPDGDEATHS, m_pClientList[iClientH]->m_iDGPoints, m_pClientList[iClientH]->m_iTotalDGKills, m_pClientList[iClientH]->m_iRating, NULL);

				iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 48);

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

//dkset by diuude and update by drawjer and modified by Ovl
void CGame::GetDkSet(int iClientH)
{
	class CItem* pItem;
	int dkM[] = { 706,707,708,710 };
	int dkW[] = { 724,725,726,728 };
	int dmM[] = { 711,712,713,913 };
	int dmW[] = { 729,730,731,914 };
	int weapon, iItemID, i, iEraseReq;
	char cTxt[120];
	DWORD  dwGoldCount = dwGetItemCount(iClientH, "Gold");

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_iLevel < m_iPlayerMaxLevel) return;
	if (dwGoldCount < m_iDKCost) 
	{
		wsprintf(cTxt, "Not enough gold. You need %d gold.", m_iDKCost);
		SendNotifyMsg(iClientH, iClientH, DEF_NOTIFY_NOTICEMSG, NULL, NULL, NULL, cTxt);
		return; // centu - el dk set cuesta gold
	}
	if (((strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "cityhall_1") == 0) || (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cName, "cityhall_2") == 0))) {
		for (i = 0; i <= 4; i++)
		{
			pItem = new class CItem;
			iItemID = -1;
			if (m_pClientList[iClientH]->m_cSex == 1)
			{
				if (m_pClientList[iClientH]->m_iStr > m_pClientList[iClientH]->m_iInt)
				{
					iItemID = dkM[i];
				}
				else
				{
					iItemID = dmM[i];
				}
			}
			else
			{
				if (m_pClientList[iClientH]->m_iStr > m_pClientList[iClientH]->m_iInt)
				{
					iItemID = dkW[i];
				}
				else
				{
					iItemID = dmW[i];
				}
			}
			if (i == 4)
			{
				if (m_pClientList[iClientH]->m_iStr > m_pClientList[iClientH]->m_iInt)
				{
					weapon = 709;
				}
				else
				{
					weapon = 714;
				}
				iItemID = weapon;
			}
			SetItemCount(iClientH, "Gold", (dwGoldCount - m_iDKCost));
			if (iItemID != -1) //if any error occures, dont crash character
			{
				_bInitItemAttr(pItem, iItemID);
				pItem->m_sTouchEffectType = DEF_ITET_UNIQUE_OWNER;
				pItem->m_sTouchEffectValue1 = m_pClientList[iClientH]->m_sCharIDnum1;
				pItem->m_sTouchEffectValue2 = m_pClientList[iClientH]->m_sCharIDnum2;
				pItem->m_sTouchEffectValue3 = m_pClientList[iClientH]->m_sCharIDnum3;
				_bAddClientItemList(iClientH, pItem, &iEraseReq);
				SendItemNotifyMsg(iClientH, DEF_NOTIFY_ITEMOBTAINED, pItem, NULL);
			}
		}
	}
}

int CGame::_iGetArrowItemIndex(int iClientH)
{
	int i;

	if (m_pClientList[iClientH] == NULL) return -1;

	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemList[i] != NULL) {

			// Arrow ¾ÆÀÌÅÛÀÌ 1°³ ÀÌ»ó ÀÖÀ¸¸é ÀÎµ¦½º °ªÀ» ¹ÝÈ¯ÇÑ´Ù.
			if ((m_pClientList[iClientH]->m_pItemList[i]->m_cItemType == DEF_ITEMTYPE_ARROW) &&
				(m_pClientList[iClientH]->m_pItemList[i]->m_dwCount > 0))
				return i;
		}

	return -1;
}

void CGame::UseItemHandler(int iClientH, short sItemIndex, short dX, short dY, short sDestItemID)
{
	int iMax, iV1, iV2, iV3, iSEV1, iEffectResult = 0;
	DWORD dwTime, dwGUID;
	short sTemp, sTmpType, sTmpAppr1;
	char cSlateType[20];
	BOOL bDepleteNow = TRUE;

	dwTime = timeGetTime();
	ZeroMemory(cSlateType, sizeof(cSlateType));

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsKilled == TRUE) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;

	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == NULL) return;


	if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE) ||
		(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_EAT)) {

		// ¾ÆÀÌÅÛÀÇ È¿°ú¿¡ ¸Â´Â Ã³¸®¸¦ ÇÑ´Ù. 
		switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType) {
		case DEF_ITEMEFFECTTYPE_WARM: // v2.172 2002-7-5 �ص� �þ�. 

			// �õ� ������ ��� �ص� �Ǿ��ٴ� �޼����� �����ش�. 
			if (m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_ICE] == 1) {

				bRemoveFromDelayEventList(iClientH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_ICE);

				// ȿ���� ������ �� �߻��� ������ �̺�Ʈ�� ����Ѵ�.
				bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_ICE, dwTime + (1 * 1000),
					iClientH, DEF_OWNERTYPE_PLAYER, NULL, NULL, NULL, 1, NULL, NULL);

			}

			m_pClientList[iClientH]->m_dwWarmEffectTime = dwTime;
			break;

			// Pandora's Box By Diuuude
		case DEF_ITEMEFFECTTYPE_PANDORA:
			int iProb, iResult1, iResult2, iPandoraItem, iEraseReq, iDamage, iDecRep, iLockTime;
			class CItem* pItem;
			/* First off all, we have to select a random event for opening box
			Effects can be :
			- OK : Restart level's Exp
			- OK : Get an Item
			- OK : Loose Rep (5 -> 50)
			- OK : Go to jail (1 to 5 mins)
			- OK : Loose HP
			- OK : Loose MP
			- OK : Loose SP
			- OK : Get some MJ Points

			*/
			iResult1 = iDice(1, 8);
			switch (iResult1) {
			case 1: // All the exp you gained this level is reseted

				m_pClientList[iClientH]->m_iExp = m_iLevelExpTable[m_pClientList[iClientH]->m_iLevel];
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_NOTICEMSG, NULL, NULL, NULL, "Doh, Pandora's Box reseted your Exp...");
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_EXP, NULL, NULL, NULL, NULL);
				break;
			case 2: // You get a random item depending on your rep
				// Check user's Rep to set a prob depending on his reps
				if (m_pClientList[iClientH]->m_iRating < -50) iProb = 500;
				if ((m_pClientList[iClientH]->m_iRating >= -50) && (m_pClientList[iClientH]->m_iRating < 0)) iProb = 400;
				if (m_pClientList[iClientH]->m_iRating == 0) iProb = 350;
				if ((m_pClientList[iClientH]->m_iRating > 0) && (m_pClientList[iClientH]->m_iRating <= 50)) iProb = 300;
				if ((m_pClientList[iClientH]->m_iRating > 50) && (m_pClientList[iClientH]->m_iRating <= 100)) iProb = 250;
				if ((m_pClientList[iClientH]->m_iRating > 100) && (m_pClientList[iClientH]->m_iRating <= 200)) iProb = 200;
				if (m_pClientList[iClientH]->m_iRating > 200) {
					iProb = 150 - (m_pClientList[iClientH]->m_iRating / 10);
				}
				iResult2 = iDice(1, iProb);
				switch (iResult2) {
					// Good Items
				case 1:
					iPandoraItem = 308; // Magic Necklace (MS10)
					break;
				case 2:
					iPandoraItem = 300; // Magic Necklace (RM10)
					break;
				case 3:
					iPandoraItem = 275; // Ogre Summon Potion
					break;
				case 4:
					iPandoraItem = 311; // Magic Necklace (DF+10)
					break;
				case 5:
					iPandoraItem = 338; // Memorial Ring
					break;
				case 6:
					iPandoraItem = 380; // Ice Storm Manual
					break;
				case 7:
					iPandoraItem = 381; // Mass Fire Strike Manual
					break;
				case 8:
					iPandoraItem = 490; // Blood Sword
					break;
				case 9:
					iPandoraItem = 618; // Dark-Elf Bow
					break;
				case 10:
					iPandoraItem = 620; // Merien Shield
					break;
				case 11:
					iPandoraItem = 636; // Ring Of Grand Mage
					break;
				case 12:
					iPandoraItem = 641; // Necklace Of Medusa
					break;
				case 13:
					iPandoraItem = 648; // Necklace Of Liche
					break;
				case 14:
					iPandoraItem = 765; // Third Memorial Ring
					break;
				case 15:
					iPandoraItem = 849; // Kloness Blade
					break;
				case 16:
					iPandoraItem = 862; // Berserk Wand (MS.10)
					break;
				case 17:
					iPandoraItem = 864; // Kloness Wand (MS.10)
					break;
				case 18:
					iPandoraItem = 866; // Resur Wand (MS.10)
					break;
				case 19:
					iPandoraItem = 291; // MagicWand (MS30-LLF)
					break;
				case 20:
					iPandoraItem = 290; // Flameberge +3 (LLF)
					break;
				case 21:
					iPandoraItem = 292; // Golden Axe (LLF)
					break;
				case 22:
					iPandoraItem = 491; // Blood Axe
					break;
				case 23:
					iPandoraItem = 492; // Blood Rapier
					break;
				case 24:
					iPandoraItem = 610; // Xelima Blade
					break;
				case 25:
					iPandoraItem = 611; // Xelima Axe
					break;
				case 26:
					iPandoraItem = 612; // Xelima Rapier
					break;
				case 27:
					iPandoraItem = 613; // Sword of Medusa
					break;
				case 28:
					iPandoraItem = 614; // Sword of Ice Elemental
					break;
				case 29:
					iPandoraItem = 616; // Demon Slayer
					break;
				case 30:
					iPandoraItem = 630; // Ring of the Xelima
					break;
				case 31:
					iPandoraItem = 633; // Ring of Demon Power
					break;
				case 32:
					iPandoraItem = 635; // Ring of Mage
					break;
				case 33:
					iPandoraItem = 636; // Ring of Arc Mage
					break;
				case 34:
					iPandoraItem = 734; // Ring of Dragon power
					break;
				case 35:
					iPandoraItem = 735; // Ring of Demon Power
					break;
				case 36:
					iPandoraItem = 844; // Black Shadow Sword
					break;
				case 37:
					iPandoraItem = 847; // Dark Executor
					break;
				case 38:
					iPandoraItem = 852; // Cancel Manual
					break;
				case 39:
					iPandoraItem = 853; // Earth Shock Wave Manual
					break;
				case 40:
					iPandoraItem = 854; // Inhibition Magic Casting Manual
					break;
				case 41:
					iPandoraItem = 858; // Necklace Of Merien
					break;
				case 42:
					iPandoraItem = 859; // Necklace Of Kloness
					break;
				case 43:
					iPandoraItem = 860; // Necklace Of Xelima
					break;
					/* Weak Items: if you add items, be sure that the weak items cases # are greater
					 than the good item cases #. It will allow players with high rating
					 to get better items.*/
				case 59:
					iPandoraItem = 4; // Dagger+1
					break;
				case 60:
					iPandoraItem = 46; // Claymore
					break;
				case 61:
					iPandoraItem = 75; // Short Bow
					break;
				case 62:
					iPandoraItem = 91; // Red Potion
					break;
				case 63:
					iPandoraItem = 93; // Blue Potion
					break;
				case 64:
					iPandoraItem = 95; // Green Potion
					break;
				case 65:
					iPandoraItem = 100; // Fish
					break;
				case 66:
					iPandoraItem = 402; // Cape
					break;
				case 67:
					iPandoraItem = 522; // Flower Pot
					break;
				case 68:
					iPandoraItem = 824; // Carrot
					break;
				case 69:
					iPandoraItem = 883; // ArmorDye(Gold)
					break;
				default:
					iPandoraItem = 0; // Nothing
					break;
				}
				if (iPandoraItem != 0) {
					pItem = new class CItem;
					_bInitItemAttr(pItem, iPandoraItem);
					_bAddClientItemList(iClientH, pItem, &iEraseReq);
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_NOTICEMSG, NULL, NULL, NULL, "Yeah, Pandora's box had an item inside !!");
					SendItemNotifyMsg(iClientH, DEF_NOTIFY_ITEMOBTAINED, pItem, NULL);
				}
				else {
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_NOTICEMSG, NULL, NULL, NULL, "Doh, Pandora's box was empty !!");
				}

				break;
			case 3: // Pandora's Box Explodes. You loose HP depending on reps
				iResult2 = iDice(2, 3);
				iDamage = (iResult2 * 100) - m_pClientList[iClientH]->m_iRating;
				m_pClientList[iClientH]->m_iLastDamage = iDamage;
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_HP, NULL, NULL, NULL, NULL);
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_NOTICEMSG, NULL, NULL, NULL, "Doh, Pandora's box exploded !!");
				SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTDAMAGE, iDamage, NULL, NULL);
				break;
			case 4: // Pandora's Box decreases your Reps
				iResult2 = iDice(1, 10);
				iDecRep = (iResult2 * 5);
				m_pClientList[iClientH]->m_iRating -= iDecRep;
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_NOTICEMSG, NULL, NULL, NULL, "Doh, Pandora's box decreased your Reputation !!");
				break;
			case 5: // Pandora's Box decreases your MP
				m_pClientList[iClientH]->m_iMP = 0;
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_MP, NULL, NULL, NULL, NULL);
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_NOTICEMSG, NULL, NULL, NULL, "Doh, Pandora's box decreased your Mana !!");
				break;
			case 6: // Pandora's Box decreases your SP
				m_pClientList[iClientH]->m_iSP = 0;
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SP, NULL, NULL, NULL, NULL);
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_NOTICEMSG, NULL, NULL, NULL, "Doh, Pandora's box decreased your Stamina !!");
				break;
			case 7: // Pandora's Box Gives you some MJ Points
				iResult2 = iDice(1, 5);
				m_pClientList[iClientH]->m_iGizonItemUpgradeLeft += iResult2;
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_NOTICEMSG, NULL, NULL, NULL, "Yeah, Your Majestic Points have been increased by Pandora's Box !!");
				break;
			case 8: // Pandora's Box send you in jail
				iResult2 = iDice(1, 5);
				iLockTime = (iResult2 * 60);
				switch (m_pClientList[iClientH]->m_cSide) {
				case 1:
					ZeroMemory(m_pClientList[iClientH]->m_cLockedMapName, sizeof(m_pClientList[iClientH]->m_cLockedMapName));
					strcpy(m_pClientList[iClientH]->m_cLockedMapName, "arejail");
					RequestTeleportHandler(iClientH, "2   ", "arejail", -1, -1);
					break;
				case 2:
					ZeroMemory(m_pClientList[iClientH]->m_cLockedMapName, sizeof(m_pClientList[iClientH]->m_cLockedMapName));
					strcpy(m_pClientList[iClientH]->m_cLockedMapName, "elvjail");;
					RequestTeleportHandler(iClientH, "2   ", "elvjail", -1, -1);
					break;
				}
				m_pClientList[iClientH]->m_iLockedMapTime = iLockTime;
				m_pClientList[iClientH]->m_iTimeLeft_ForceRecall = iLockTime;
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_NOTICEMSG, NULL, NULL, NULL, "You were busted for using illegal Pandora's Box");
				break;
			}
			break;

		case DEF_ITEMEFFECTTYPE_LOTTERY:

			break;

		case DEF_ITEMEFFECTTYPE_SLATES:
			if (m_pClientList[iClientH]->m_pItemList[sItemIndex] != NULL) {
				// Full Ancient Slate ??
				if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 867) {
					// Slates dont work on Heldenian Map
					switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2) {
					case 2: // Bezerk slate
						m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_BERSERK] = TRUE;
						SetBerserkFlag(iClientH, DEF_OWNERTYPE_PLAYER, TRUE);
						bRegisterDelayEvent(DEF_DELAYEVENTTYPE_MAGICRELEASE, DEF_MAGICTYPE_BERSERK, dwTime + (1000 * 600),
							iClientH, DEF_OWNERTYPE_PLAYER, NULL, NULL, NULL, 1, NULL, NULL);
						SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_MAGICEFFECTON, DEF_MAGICTYPE_BERSERK, 1, NULL, NULL);
						strcpy(cSlateType, "Berserk");
						break;

					case 1: // Invincible slate
						if (strlen(cSlateType) == 0) {
							strcpy(cSlateType, "Invincible");
						}
						SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SLATE_INVINCIBLE, NULL, NULL, NULL, NULL);
						break;
					case 3: // Mana slate
						if (strlen(cSlateType) == 0) {
							strcpy(cSlateType, "Mana");
						}
						SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SLATE_MANA, NULL, NULL, NULL, NULL);
						break;
					case 4: // Exp slate
						if (strlen(cSlateType) == 0) {
							strcpy(cSlateType, "Exp");
						}
						SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SLATE_EXP, NULL, NULL, NULL, NULL);
						break;
					}
					SetSlateFlag(iClientH, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2, TRUE);
					bRegisterDelayEvent(DEF_DELAYEVENTTYPE_ANCIENT_TABLET, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2,
						dwTime + (1000 * 600), iClientH, DEF_OWNERTYPE_PLAYER, NULL, NULL, NULL, 1, NULL, NULL);

				}
				if (strlen(cSlateType) > 0)
					_bItemLog(DEF_ITEMLOG_USE, iClientH, strlen(cSlateType), m_pClientList[iClientH]->m_pItemList[sItemIndex]);
			}
			break;

		case DEF_ITEMEFFECTTYPE_HP:
			iMax = iGetMaxHP(iClientH);
			if (m_pClientList[iClientH]->m_iHP < iMax) {

				if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1 == 0) {
					iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
					iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
					iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue3;
				}
				else {
					iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1;
					iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2;
					iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue3;
				}

				m_pClientList[iClientH]->m_iHP += (iDice(iV1, iV2) + iV3);
				if (m_pClientList[iClientH]->m_iHP > iMax) m_pClientList[iClientH]->m_iHP = iMax;
				if (m_pClientList[iClientH]->m_iHP <= 0)   m_pClientList[iClientH]->m_iHP = 1;

				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_HP, NULL, NULL, NULL, NULL);
			}
			break;

		case DEF_ITEMEFFECTTYPE_MP:
			iMax = iGetMaxMP(iClientH);

			if (m_pClientList[iClientH]->m_iMP < iMax) {

				if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1 == 0) {
					iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
					iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
					iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue3;
				}
				else
				{
					iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1;
					iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2;
					iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue3;
				}

				m_pClientList[iClientH]->m_iMP += (iDice(iV1, iV2) + iV3);
				if (m_pClientList[iClientH]->m_iMP > iMax)
					m_pClientList[iClientH]->m_iMP = iMax;

				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_MP, NULL, NULL, NULL, NULL);
			}
			break;

		case DEF_ITEMEFFECTTYPE_SP:
			iMax = iGetMaxSP(iClientH);

			if (m_pClientList[iClientH]->m_iSP < iMax) {

				if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1 == 0) {
					iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
					iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
					iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue3;
				}
				else {
					iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1;
					iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue2;
					iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue3;
				}

				m_pClientList[iClientH]->m_iSP += (iDice(iV1, iV2) + iV3);
				if (m_pClientList[iClientH]->m_iSP > iMax)
					m_pClientList[iClientH]->m_iSP = iMax;

				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SP, NULL, NULL, NULL, NULL);
			}

			if (m_pClientList[iClientH]->m_bIsPoisoned == TRUE) {
				// Áßµ¶µÈ »óÅÂ¿´´Ù¸é Áßµ¶À» Ç¬´Ù.
				m_pClientList[iClientH]->m_bIsPoisoned = FALSE;
				// Áßµ¶ÀÌ Ç®·ÈÀ½À» ¾Ë¸°´Ù. 
				SetPoisonFlag(iClientH, DEF_OWNERTYPE_PLAYER, FALSE); // removes poison aura when using a revitalizing potion
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_MAGICEFFECTOFF, DEF_MAGICTYPE_POISON, NULL, NULL, NULL);
			}
			break;

		case DEF_ITEMEFFECTTYPE_HPSTOCK:
			iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
			iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
			iV3 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue3;

			m_pClientList[iClientH]->m_iHPstock += iDice(iV1, iV2) + iV3;
			if (m_pClientList[iClientH]->m_iHPstock < 0)   m_pClientList[iClientH]->m_iHPstock = 0;
			if (m_pClientList[iClientH]->m_iHPstock > 500) m_pClientList[iClientH]->m_iHPstock = 500;

			// ¹è°íÇÄÀ» ÇØ°áÇÑ´Ù. 
			m_pClientList[iClientH]->m_iHungerStatus += iDice(iV1, iV2) + iV3;
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_HUNGER, m_pClientList[iClientH]->m_iHungerStatus, NULL, NULL, NULL); // MORLA2 - Muestra nuevamente
			if (m_pClientList[iClientH]->m_iHungerStatus > 100) m_pClientList[iClientH]->m_iHungerStatus = 100;
			if (m_pClientList[iClientH]->m_iHungerStatus < 0)   m_pClientList[iClientH]->m_iHungerStatus = 0;
			break;


		case DEF_ITEMEFFECTTYPE_STUDYSKILL: //Manual
			iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
			iV2 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2;
			iSEV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1;
			if (iSEV1 == 0) 
			{
				TrainSkillResponse(TRUE, iClientH, iV1, iV2);
			}
			else 
			{
				TrainSkillResponse(TRUE, iClientH, iV1, iSEV1);
			}
			break;

		case DEF_ITEMEFFECTTYPE_STUDYMAGIC:
			iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
			if (m_pMagicConfigList[iV1] != NULL)
				RequestStudyMagicHandler(iClientH, m_pMagicConfigList[iV1]->m_cName, bDepleteNow, FALSE);
			break;

			// New 15/05/2004 Changed
		case DEF_ITEMEFFECTTYPE_MAGIC:
			// Åõ¸í ¸ðµå¿´´Ù¸é ¸¶¹ý È¿°ú ¾ÆÀÌÅÛ »ç¿ë½Ã¿¡ ÇØÁ¦µÈ´Ù.

			if (m_pClientList[iClientH]->m_iAdminUserLevel == 0) {
				SetInvisibilityFlag(iClientH, DEF_OWNERTYPE_PLAYER, FALSE);

				bRemoveFromDelayEventList(iClientH, DEF_OWNERTYPE_PLAYER, DEF_MAGICTYPE_INVISIBILITY);
				m_pClientList[iClientH]->m_cMagicEffectStatus[DEF_MAGICTYPE_INVISIBILITY] = NULL;
			}


			switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1) {
			case 1:
				// Recall ¸¶¹ý È¿°ú°¡ ÀÖ´Â ¾ÆÀÌÅÛ. 
				// testcode
				RequestTeleportHandler(iClientH, "1  ");
				break;

			case 2:
				// Åõ¸í ¸¶¹ýÈ¿°ú°¡ ÀÖ´Â ¾ÆÀÌÅÛ. 
				PlayerMagicHandler(iClientH, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, 32, TRUE);
				break;

			case 3:
				// Åõ¸í ¸¶¹ý Ã£±â. »çÅõÀå ³»ºÎ¸é ¼Ò¿ë¾ø´Ù. 
				if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsFightZone == FALSE)
					PlayerMagicHandler(iClientH, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, 34, TRUE);
				break;

			case 4:
				// fixed location teleportation: ÀÔÀå±Ç µîµî
				switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2) {
				case 1:
					
						// ºí¸®µù ¾ÆÀÏ·Î °£´Ù 
						if (memcmp(m_pClientList[iClientH]->m_cMapName, "bisle", 5) != 0) {
							//v1.42
							ItemDepleteHandler(iClientH, sItemIndex, TRUE, TRUE);
							RequestTeleportHandler(iClientH, "2   ", "bisle", -1, -1);
						}
					
					break;

				//LifeX Added Reset Stat Scroll
				case 9:
					m_pClientList[iClientH]->m_iMag;
					m_pClientList[iClientH]->m_iMag = 10;
					m_pClientList[iClientH]->m_iStr;
					m_pClientList[iClientH]->m_iStr = 10;
					m_pClientList[iClientH]->m_iDex;
					m_pClientList[iClientH]->m_iDex = 10;
					m_pClientList[iClientH]->m_iCharisma;
					m_pClientList[iClientH]->m_iCharisma = 10;
					m_pClientList[iClientH]->m_iInt;
					m_pClientList[iClientH]->m_iInt = 10;
					m_pClientList[iClientH]->m_iVit;
					m_pClientList[iClientH]->m_iVit = 10;

					m_pClientList[iClientH]->m_iLU_Pool = 1432;
					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_HEAD], FALSE);
					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY], FALSE);
					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS], FALSE);
					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LEGGINGS], FALSE);
					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS], FALSE);
					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BACK], FALSE);
					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND], FALSE);
					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND], FALSE);
					ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LHAND], FALSE);
					ItemDepleteHandler(iClientH, sItemIndex, TRUE, TRUE);
					bCheckMagicInt(iClientH);
					DeleteClient(iClientH, TRUE, TRUE);
					break;

				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:
				case 19:
					// °áÅõÀåÀ¸·Î °£´Ù. 
					SYSTEMTIME SysTime;

					GetLocalTime(&SysTime);
					// v1.4311-3 º¯°æ ÀÔÀå±Ç Ã¼Å© ´Þ/³¯Â¥/½Ã°£À¸·Î Ã¼Å©ÇÑ´Ù. 
					// ÀÔÀå °¡´ÉÇÑ ½Ã°£º¸´Ù ÀÛ°Å³ª °°À¸¸é ÀÔÀå±ÇÀÌ »ç¶óÁø´Ù.
					if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sTouchEffectValue1 == SysTime.wMonth) ||
						(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sTouchEffectValue2 == SysTime.wDay) ||
						(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sTouchEffectValue3 > SysTime.wHour)) {
						// ³¯Â¥°¡ Á¤È®ÇÏÁö ¾Ê´Ù. ¾Æ¹«·± È¿°ú°¡ ¾ø°í ÀÔÀå±ÇÀº »ç¶óÁø´Ù.

						char cDestMapName[11];
						ZeroMemory(cDestMapName, sizeof(cDestMapName));
						wsprintf(cDestMapName, "fightzone%d", m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2 - 10);
						if (memcmp(m_pClientList[iClientH]->m_cMapName, cDestMapName, 10) != 0) {
							//v1.42
							ItemDepleteHandler(iClientH, sItemIndex, TRUE, TRUE);
							RequestTeleportHandler(iClientH, "2   ", cDestMapName, -1, -1);
						}
					}
					break;
				}
				break;

			case 5: // Heldenians scroll must be defined as DEF_ITEMTYPE_USE_PERM
					// Only Helnenian scrolls can be defined as DEF_ITEMTYPE_USE_PERM 
					// Scrolls not belonging to the user are depleted when entering function 
				if ((m_bIsHeldenianMode == TRUE)
					&& (m_bHeldenianWarInitiated == TRUE)
					&& (m_pMapList[m_pClientList[iClientH]->m_cMapIndex] != 0)
					&& (m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_bIsHeldenianMap == 1)
					&& (strcmp(m_pClientList[iClientH]->m_cMapName, "GodH") != 0))
				{
					memcpy(&dwGUID, &m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemSpecEffectValue1, 4);
					if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sTouchEffectType == DEF_ITET_UNIQUE_OWNER)
					{
						if (dwGUID == m_dwHeldenianGUID)
						{	// Scrolls from current Heldenian can summon
							if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue6 == -1)// -1 allows using with weapon shield equiped
								PlayerMagicHandler(iClientH, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, 31, TRUE,
									m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2);
							else
								PlayerMagicHandler(iClientH, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, 31, TRUE,
									m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2 * -1);
							m_pClientList[iClientH]->m_iWarContribution += 300;

						}
						// But older scrolls will be depleted without effect!
						ItemDepleteHandler(iClientH, sItemIndex, TRUE, TRUE);
					}
					else
					{	// Not personal scrolls can summon and deplete scroll now
						if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue6 == -1)// -1 allows using with weapon shield equiped
							PlayerMagicHandler(iClientH, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, 31, TRUE,
								m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2);
						else
							PlayerMagicHandler(iClientH, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, 31, TRUE,
								m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue2 * -1);
						ItemDepleteHandler(iClientH, sItemIndex, TRUE, TRUE);
					}
				}
				else // No permanent scroll outside Special Heldenian summon scrolls
				{
					if (m_bIsHeldenianMode == FALSE) ItemDepleteHandler(iClientH, sItemIndex, TRUE, TRUE);
				}
				break;
			}
			break;

		case DEF_ITEMEFFECTTYPE_FIRMSTAMINAR:
			m_pClientList[iClientH]->m_iTimeLeft_FirmStaminar += m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
			break;

		case DEF_ITEMEFFECTTYPE_CHANGEATTR:
			switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1) {
			case 1:
				// ¸Ó¸® »öÀ» ¹Ù²Û´Ù. 
				m_pClientList[iClientH]->m_cHairColor++;
				if (m_pClientList[iClientH]->m_cHairColor > 15) m_pClientList[iClientH]->m_cHairColor = 0;

				sTemp = (m_pClientList[iClientH]->m_cHairStyle << 8) | (m_pClientList[iClientH]->m_cHairColor << 4) | (m_pClientList[iClientH]->m_cUnderwear);
				m_pClientList[iClientH]->m_sAppr1 = sTemp;
				break;

			case 2:
				// ¸Ó¸® ½ºÅ¸ÀÏÀ» ¹Ù²Û´Ù.
				m_pClientList[iClientH]->m_cHairStyle++;
				if (m_pClientList[iClientH]->m_cHairStyle > 7) m_pClientList[iClientH]->m_cHairStyle = 0;

				sTemp = (m_pClientList[iClientH]->m_cHairStyle << 8) | (m_pClientList[iClientH]->m_cHairColor << 4) | (m_pClientList[iClientH]->m_cUnderwear);
				m_pClientList[iClientH]->m_sAppr1 = sTemp;
				break;

			case 3:
				// ÇÇºÎ»öÀ» ¹Ù²Û´Ù.
				// ³»¿ëÀ» ¹ÙÅÁÀ¸·Î Appearance¸¦ °è»ê, ÇÒ´çÇÑ´Ù.
				m_pClientList[iClientH]->m_cSkin++;
				if (m_pClientList[iClientH]->m_cSkin > 3)
					m_pClientList[iClientH]->m_cSkin = 1;

				if (m_pClientList[iClientH]->m_cSex == 1)      sTemp = 1;
				else if (m_pClientList[iClientH]->m_cSex == 2) sTemp = 4;

				switch (m_pClientList[iClientH]->m_cSkin) {
				case 2:	sTemp++; break;
				case 3:	sTemp += 2; break;
				}
				m_pClientList[iClientH]->m_sType = sTemp;
				break;

			case 4:
				// ¼ºÀüÈ¯ - ¸¸¾à ¿ÊÀ» ÀÔ°í ÀÖ´Ù¸é ½ÇÆÐÇÑ´Ù. 
				sTemp = m_pClientList[iClientH]->m_sAppr3 & 0xFF0F;
				if (sTemp == 0) {
					// sTemp°¡ 0ÀÌ ¾Æ´Ï¶ó¸é °Ñ¿Ê, ¼Ó¿Ê, ¹ÙÁöÁß ÇÑ°¡Áö¸¦ ÀÔ°í ÀÖ´Â °ÍÀÌ´Ù. ¼ºÀüÈ¯À» ÇÒ ¼ö ¾ø´Ù. 
					if (m_pClientList[iClientH]->m_cSex == 1)
						m_pClientList[iClientH]->m_cSex = 2;
					else m_pClientList[iClientH]->m_cSex = 1;

					// ³»¿ëÀ» ¹ÙÅÁÀ¸·Î Appearance¸¦ °è»ê, ÇÒ´çÇÑ´Ù.
					if (m_pClientList[iClientH]->m_cSex == 1) {
						// ³²ÀÚÀÌ´Ù. 
						sTmpType = 1;
					}
					else if (m_pClientList[iClientH]->m_cSex == 2) {
						// ¿©ÀÚÀÌ´Ù.
						sTmpType = 4;
					}

					switch (m_pClientList[iClientH]->m_cSkin) {
					case 1:
						// ¹éÀÎÀÌ¸é ±×´ë·Î.
						break;
					case 2:
						sTmpType++;
						break;
					case 3:
						sTmpType += 2;
						break;
					}

					sTmpAppr1 = (m_pClientList[iClientH]->m_cHairStyle << 8) | (m_pClientList[iClientH]->m_cHairColor << 4) | (m_pClientList[iClientH]->m_cUnderwear);
					m_pClientList[iClientH]->m_sType = sTmpType;
					m_pClientList[iClientH]->m_sAppr1 = sTmpAppr1;
					//
				}
				break;
			}

			SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, NULL, NULL, NULL);
			break;
		}
		// *** Request Teleport Handler°¡ ÀÛµ¿µÇ¸é ÀÌ¹Ì µ¥ÀÌÅÍ ÀúÀåÀÌ ¿äÃ»µÈ »óÅÂÀÌ¹Ç·Î ÀÌÈÄ¿¡ ¾ÆÀÌÅÛÀ» ¾ø¾ÖºÁ¾ß ¼Ò¿ëÀÌ ¾ø´Ù. 
		// ¾ÆÀÌÅÛÀ» ¸ÕÀú ¾ø¾Ø´Ù.
		ItemDepleteHandler(iClientH, sItemIndex, TRUE, TRUE);
	}
	else if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_DEPLETE_DEST) {
		// »ç¿ëÇÏ¸é¼­ ¸ñÇ¥ÁöÁ¡À» ÁöÁ¤ÇÏ´Â ¾ÆÀÌÅÛ.
		// dX, dYÀÇ ÁÂÇ¥°¡ À¯È¿ ¹üÀ§ ³»¿¡ ÀÖ´ÂÁö È®ÀÎÇØ¾ß ÇÑ´Ù.
		if (_bDepleteDestTypeItemUseEffect(iClientH, dX, dY, sItemIndex, sDestItemID) == TRUE)
			ItemDepleteHandler(iClientH, sItemIndex, TRUE, TRUE);
	}
	else if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW) {
		// È­»ìÀ» ÇÒ´çÇÑ´Ù. 
		m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);
	}
	else if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_PERM) {
		// ¿µ±¸È÷ ¾µ ¼ö ÀÖ´Â ¾ÆÀÌÅÛ. Áï ¾²°í³ªµµ ¾ø¾îÁöÁö ¾Ê´Â ¾ÆÀÌÅÛ. (ex: Áöµµ) 
		switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType) {
		case DEF_ITEMEFFECTTYPE_SHOWLOCATION:
			iV1 = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue1;
			switch (iV1) {
			case 1:
				// ÇöÀç ÀÚ½ÅÀÇ À§Ä¡¸¦ º¸¿©ÁØ´Ù. 
				if (strcmp(m_pClientList[iClientH]->m_cMapName, "aresden") == 0)
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 1, NULL, NULL);
				else if (strcmp(m_pClientList[iClientH]->m_cMapName, "elvine") == 0)
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 2, NULL, NULL);
				else if (strcmp(m_pClientList[iClientH]->m_cMapName, "middleland") == 0)
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 3, NULL, NULL);
				else if (strcmp(m_pClientList[iClientH]->m_cMapName, "default") == 0)
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 4, NULL, NULL);
				else if (strcmp(m_pClientList[iClientH]->m_cMapName, "huntzone2") == 0)
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 5, NULL, NULL);
				else if (strcmp(m_pClientList[iClientH]->m_cMapName, "huntzone1") == 0)
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 6, NULL, NULL);
				else if (strcmp(m_pClientList[iClientH]->m_cMapName, "huntzone4") == 0)
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 7, NULL, NULL);
				else if (strcmp(m_pClientList[iClientH]->m_cMapName, "huntzone3") == 0)
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 8, NULL, NULL);
				else if (strcmp(m_pClientList[iClientH]->m_cMapName, "arefarm") == 0)
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 9, NULL, NULL);
				else if (strcmp(m_pClientList[iClientH]->m_cMapName, "elvfarm") == 0)
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 10, NULL, NULL);
				else SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SHOWMAP, iV1, 0, NULL, NULL);
				break;
			}
			break;
		}
	}
	else if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_USE_SKILL) {
		// ±â¼ú°ú °ü·ÃµÈ ¾ÆÀÌÅÛÀ» »ç¿ëÇÑ´Ù. ¾ÆÀÌÅÛÀÇ ¼ö¸íÀ» ³·Ãß°í µô·¹ÀÌ ÀÌº¥Æ®¿¡ µî·ÏÇÑ´Ù. 

		if ((m_pClientList[iClientH]->m_pItemList[sItemIndex] == NULL) ||
			(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan <= 0) ||
			(m_pClientList[iClientH]->m_bSkillUsingStatus[m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill] == TRUE)) {
			// ¾ÆÀÌÅÛÀÇ ¼ö¸íÀÌ ´Ù Çß°Å³ª ¾ø°Å³ª °ü·Ã ½ºÅ³À» »ç¿ëÁßÀÌ¶ó¸é ¹«½Ã 
			return;
		}
		else {
			if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wMaxLifeSpan != 0) {
				// ÃÖ´ë ¼ö¸íÀÌ 0ÀÌ¸é »ç¿ëÇØµµ ¼ö¸íÀÌ ÁÙÁö ¾Ê´Â´Ù.
				m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan--;
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CURLIFESPAN, sItemIndex, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan, NULL, NULL);
				if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan <= 0) {
					// ¾ÆÀÌÅÛÀÇ ¼ö¸íÀÌ ´Ù µÇ¾ú´Ù.
					// ¾ÆÀÌÅÛÀÌ ¸Á°¡Á³´Ù´Â ¸Þ½ÃÁö <- ÀÌ°É ¹ÞÀ¸¸é ÀåÂøÈ­¸é¿¡¼­ ÇØÁ¦½ÃÄÑ¾ß ÇÑ´Ù.
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMLIFESPANEND, DEF_EQUIPPOS_NONE, sItemIndex, NULL, NULL);
				}
				else {
					// ±â¼ú »ç¿ë ½Ã°£ ID°ªÀ» ±¸ÇÑ´Ù. v1.12
					int iSkillUsingTimeID = (int)timeGetTime();

					bRegisterDelayEvent(DEF_DELAYEVENTTYPE_USEITEM_SKILL, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill,
						dwTime + m_pSkillConfigList[m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill]->m_sValue2 * 1000,
						iClientH, DEF_OWNERTYPE_PLAYER, m_pClientList[iClientH]->m_cMapIndex, dX, dY,
						m_pClientList[iClientH]->m_cSkillMastery[m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill], iSkillUsingTimeID, NULL);

					// ±â¼ú »ç¿ëÁß 
					m_pClientList[iClientH]->m_bSkillUsingStatus[m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill] = TRUE;
					m_pClientList[iClientH]->m_iSkillUsingTimeID[m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sRelatedSkill] = iSkillUsingTimeID; //v1.12
				}
			}
		}
	}
}

BOOL CGame::bSetItemToBankItem(int iClientH, short sItemIndex)
{
	int i, iRet;
	DWORD* dwp;
	WORD* wp;
	char* cp;
	short* sp;
	char cData[100];
	class CItem* pItem;

	// ¼ÒÁöÇÏ°í ÀÖ´Â ¾ÆÀÌÅÛÀ» º¸°üÇÑ´Ù.
	if (m_pClientList[iClientH] == NULL) return FALSE;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return FALSE;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == NULL) return FALSE;
	if (m_pClientList[iClientH]->m_bIsInsideWarehouse == FALSE) return FALSE;

	for (i = 0; i < DEF_MAXBANKITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemInBankList[i] == NULL) {
			// ºñ¾îÀÖ´Â À§Ä¡¸¦ Ã£¾Ò´Ù.

			m_pClientList[iClientH]->m_pItemInBankList[i] = m_pClientList[iClientH]->m_pItemList[sItemIndex];
			pItem = m_pClientList[iClientH]->m_pItemInBankList[i];
			// !!! ¾ÆÀÌÅÛÀÇ Æ÷ÀÎÅÍ¸¦ ÀÌµ¿ÇßÀ¸´Ï ±âÁ¸ÀÇ Æ÷ÀÎÅÍ´Â NULL°ªÀ¸·Î ÇÒ´ç. 
			m_pClientList[iClientH]->m_pItemList[sItemIndex] = NULL;

			// ¼ÒÁöÇ° ÃÑ Áß·® Àç °è»ê 
			iCalcTotalWeight(iClientH);

			dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_ITEMTOBANK;

			cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);

			*cp = i; // À§Ä¡ ÀúÀå 
			cp++;

			// 1°³.
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

			// v1.432
			sp = (short*)cp;
			*sp = pItem->m_sItemEffectValue2;
			cp += 2;

			// v1.42
			dwp = (DWORD*)cp;
			*dwp = pItem->m_dwAttribute;
			cp += 4;

			//SNOOPY: Completion/Purity fix ?
			sp = (short*)cp;
			*cp = pItem->m_sItemSpecEffectValue2;
			cp += 2;

			// ¾ÆÀÌÅÛ Á¤º¸ Àü¼Û 
			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 55 + 2);
			switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				// ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù. v1.41 Á¦°ÅÇÏÁö ¾Ê´Â´Ù.
				break; // v1.41 FALSE¸¦ ¹ÝÈ¯ÇÏ¸é ¾ÆÀÌÅÛÀÌ ¹Ù´Ú¿¡ º¹»çµÈ´Ù.
			}

			return TRUE;
		}

	// ¾ÆÀÌÅÛÀ» º¸°üÇÒ ¿©À¯°ø°£ÀÌ ¾ø´Ù.
	return FALSE;
}
BOOL CGame::bSetItemToBankItem(int iClientH, class CItem* pItem)
{
	int i, iRet;
	DWORD* dwp;
	WORD* wp;
	char* cp;
	short* sp;
	char cData[100];

	if (m_pClientList[iClientH] == NULL) return FALSE;
	if (pItem == NULL) return FALSE;
	if (m_pClientList[iClientH]->m_bIsInsideWarehouse == FALSE) return FALSE;

	for (i = 0; i < DEF_MAXBANKITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemInBankList[i] == NULL) {

			m_pClientList[iClientH]->m_pItemInBankList[i] = pItem;

			dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_ITEMTOBANK;

			cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);

			*cp = i; // À§Ä¡ ÀúÀå 
			cp++;

			// 1°³.
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

			// v1.432
			sp = (short*)cp;
			*sp = pItem->m_sItemEffectValue2;
			cp += 2;

			// v1.42
			dwp = (DWORD*)cp;
			*dwp = pItem->m_dwAttribute;
			cp += 4;

			//SNOOPY: Completion/Purity fix ?
			sp = (short*)cp;
			*cp = pItem->m_sItemSpecEffectValue2;
			cp += 2;

			// ¾ÆÀÌÅÛ Á¤º¸ Àü¼Û 
			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 55 + 2);
			switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				// ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù. v1.41 Á¦°ÅÇÏÁö ¾Ê´Â´Ù.
				break; // v1.41 FALSE¸¦ ¹ÝÈ¯ÇÏ¸é ¾ÆÀÌÅÛÀÌ ¹Ù´Ú¿¡ º¹»çµÈ´Ù.
			}

			return TRUE;
		}

	// ¾ÆÀÌÅÛÀ» º¸°üÇÒ ¿©À¯°ø°£ÀÌ ¾ø´Ù.
	return FALSE;
}

void CGame::ReqSellItemHandler(int iClientH, char cItemID, char cSellToWhom, int iNum, char* pItemName)
{
	char cItemCategory, cItemName[21];
	short sRemainLife;
	int   iPrice;
	double d1, d2, d3;
	BOOL   bNeutral;
	DWORD  dwSWEType, dwSWEValue, dwAddPrice1, dwAddPrice2, dwMul1, dwMul2;
	CItem* m_pGold;

	// »ç¿ëÀÚÀÇ ¾ÆÀÌÅÛ ÆÈ±â ¿ä±¸.
	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;
	if ((cItemID < 0) || (cItemID >= 50)) return;
	if (m_pClientList[iClientH]->m_pItemList[cItemID] == NULL) return;
	if (iNum <= 0) return;
	if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwCount < iNum) return;

	iCalcTotalWeight(iClientH);

	m_pGold = new class CItem;
	ZeroMemory(cItemName, sizeof(cItemName));
	wsprintf(cItemName, "Gold");
	_bInitItemAttr(m_pGold, cItemName);

	// v1.42
	bNeutral = FALSE;
	if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) bNeutral = TRUE;
	// v2.13 ¼ºÈÄ´Ï ¼öÁ¤ ¹°°ÇÀ» ¾îµð¼­³ª ÆÈ°Ô ¼öÁ¤ÇÏ¿©¼­ ÆÈ¶§´Â NPC ±¸ºÐÀÌ ÇÊ¿ä¾ø´Ù. 
	// ´Ü Ä«Å×°í¸®¸¦ ±âÁØÀ¸·Î ¾ÆÀÌÅÛ °¡°ÝÀ» °áÁ¤ÇÑ´Ù.
	switch (cSellToWhom) {
	case 15: 		// »óÁ¡ ¾ÆÁÜ¸¶ 
	case 24:        // ´ëÀå°£ ÁÖÀÎ 
		cItemCategory = m_pClientList[iClientH]->m_pItemList[cItemID]->m_cCategory;
		// 12-22 ¼ºÈÄ´Ï ¼öÁ¤ ¾îµð¼­µç ÆÈ¼ö ÀÖ°Ô ¼öÁ¤ 
		// »óÁ¡¾ÆÀÌÅÛ 
		if ((cItemCategory >= 11) && (cItemCategory <= 50)) {

			// ÀûÇÕÇÏ´Ù. ¹«Á¶°Ç ¹Ý°ª 
			iPrice = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2) * iNum;
			sRemainLife = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan;


			//v1.42 Áß¸³ÀÎ °æ¿ì ¹ÝÀÇ ¹Ý°ª.
			if (bNeutral == TRUE) iPrice = iPrice / 2;
			if (iPrice <= 0)    iPrice = 1;
			if (iPrice > 1000000) iPrice = 1000000;

			if (m_pClientList[iClientH]->m_iCurWeightLoad + iGetItemWeight(m_pGold, iPrice) > (DWORD)_iCalcMaxLoad(iClientH)) {
				// v2.12 ÆÈ °æ¿ì ¹«°Ô°¡ ÃÊ°úµÇ¾î¼­ ÆÈ ¼ö ¾ø´Ù.
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CANNOTSELLITEM, cItemID, 4, NULL, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
			}
			else SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SELLITEMPRICE, cItemID, sRemainLife, iPrice, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName, iNum);
		}
		// ´ëÀå°£ ¾ÆÀÌÅÛ
		else if ((cItemCategory >= 1) && (cItemCategory <= 10)) {
			// ¿ø·¡ ¾ÆÀÌÅÛÀÇ ¼ö¸í°ú ºñ±³ÇØ¼­ °¨°¡ »ó°¢À» °è»ê, ¾ÆÀÌÅÛÀÇ °¡°ÝÀ» ¸Å±ä´Ù.
			sRemainLife = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan;

			if (sRemainLife == 0) {
				// °íÀå³­ ¾ÆÀÌÅÛÀº ÆÈ ¼ö ¾ø´Ù
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CANNOTSELLITEM, cItemID, 2, NULL, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
			}
			else {
				d1 = (double)sRemainLife;
				if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan != 0)
					d2 = (double)m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan;
				else d2 = 1.0f;
				d3 = (d1 / d2) * 0.5f;
				d2 = (double)m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice; // ¿ø·¡ °¡°Ý 
				d3 = d3 * d2; // Ãß»êµÈ °¡°Ý 

				iPrice = (int)d3;
				iPrice = iPrice * iNum;

				dwAddPrice1 = 0;
				dwAddPrice2 = 0;
				// ¾ÆÀÌÅÛ Æ¯¼ºÄ¡¿¡ µû¸¥ °¡°Ý »ó½Â 
				if ((m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x00F00000) != NULL) {
					dwSWEType = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x00F00000) >> 20;
					dwSWEValue = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x000F0000) >> 16;

					switch (dwSWEType) {
					case 6: dwMul1 = 2; break;  // °¡º­¿î 
					case 8: dwMul1 = 2; break;  // °­È­µÈ
					case 5: dwMul1 = 3; break;  // ¹ÎÃ¸ÀÇ
					case 1: dwMul1 = 4; break;  // ÇÊ»ìÀÇ 
					case 7: dwMul1 = 5; break;  // ¿¹¸®ÇÑ
					case 2: dwMul1 = 6; break;  // Áßµ¶ÀÇ
					case 3: dwMul1 = 15; break; // Á¤ÀÇÀÇ 
					case 9: dwMul1 = 20; break; // °í´ë¹®¸í 
					default: dwMul1 = 1; break;
					}

					d1 = (double)iPrice * dwMul1;
					switch (dwSWEValue) {
					case 1: d2 = 10.0f; break;
					case 2: d2 = 20.0f; break;
					case 3: d2 = 30.0f; break;
					case 4: d2 = 35.0f; break;
					case 5: d2 = 40.0f; break;
					case 6: d2 = 50.0f; break;
					case 7: d2 = 100.0f; break;
					case 8: d2 = 200.0f; break;
					case 9: d2 = 300.0f; break;
					case 10: d2 = 400.0f; break;
					case 11: d2 = 500.0f; break;
					case 12: d2 = 700.0f; break;
					case 13: d2 = 900.0f; break;
					default: d2 = 0.0f; break;
					}
					d3 = d1 * (d2 / 100.0f);

					dwAddPrice1 = (int)(d1 + d3);
				}

				// v1.42 Èñ±Í ¾ÆÀÌÅÛÀÌ¶ó¸é Sub È¿°ú¸¦ ¼³Á¤ÇÑ´Ù. °ø°Ý¹«±â´Â 1°³¸¸ ÀåÂøµÈ´Ù°í ÇßÀ»¶§¸¸ À¯È¿ÇÔ.
				if ((m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x0000F000) != NULL) {
					dwSWEType = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x0000F000) >> 12;
					dwSWEValue = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x00000F00) >> 8;

					switch (dwSWEType) {
					case 1:
					case 12: dwMul2 = 2; break;

					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					case 7: dwMul2 = 4; break;

					case 8:
					case 9:
					case 10:
					case 11: dwMul2 = 6; break;
					}

					d1 = (double)iPrice * dwMul2;
					switch (dwSWEValue) {
					case 1: d2 = 10.0f; break;
					case 2: d2 = 20.0f; break;
					case 3: d2 = 30.0f; break;
					case 4: d2 = 35.0f; break;
					case 5: d2 = 40.0f; break;
					case 6: d2 = 50.0f; break;
					case 7: d2 = 100.0f; break;
					case 8: d2 = 200.0f; break;
					case 9: d2 = 300.0f; break;
					case 10: d2 = 400.0f; break;
					case 11: d2 = 500.0f; break;
					case 12: d2 = 700.0f; break;
					case 13: d2 = 900.0f; break;
					default: d2 = 0.0f; break;
					}
					d3 = d1 * (d2 / 100.0f);

					dwAddPrice2 = (int)(d1 + d3);
				}

				// v2.03 925 Æ¯¼ö ¾ÆÀÌÅÛ °¡°Ý °¡ÁßÄ¡¸¦ 77%¼öÁØÀ¸·Î ´Ù¿î 
				iPrice = iPrice + (dwAddPrice1 - (dwAddPrice1 / 3)) + (dwAddPrice2 - (dwAddPrice2 / 3));

				//v1.42 Áß¸³ÀÎ °æ¿ì ¹ÝÀÇ ¹Ý°ª.
				if (bNeutral == TRUE) iPrice = iPrice / 2;
				if (iPrice <= 0)    iPrice = 1;
				if (iPrice > 1000000) iPrice = 1000000;

				if (m_pClientList[iClientH]->m_iCurWeightLoad + iGetItemWeight(m_pGold, iPrice) > (DWORD)_iCalcMaxLoad(iClientH)) {
					// v2.12 ÆÈ °æ¿ì ¹«°Ô°¡ ÃÊ°úµÇ¾î¼­ ÆÈ ¼ö ¾ø´Ù.
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CANNOTSELLITEM, cItemID, 4, NULL, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
				}
				else SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SELLITEMPRICE, cItemID, sRemainLife, iPrice, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName, iNum);
			}
		}
		else SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CANNOTSELLITEM, cItemID, 1, NULL, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
		break;


	default:
		break;
	}
	if (m_pGold != NULL) delete m_pGold;
}

void CGame::ReqSellItemConfirmHandler(int iClientH, char cItemID, int iNum, char* pString)
{
	class CItem* pItemGold;
	short sRemainLife;
	int   iPrice;
	double d1, d2, d3;
	char* cp, cItemName[21], cData[120], cItemCategory;
	DWORD* dwp, dwMul1, dwMul2, dwSWEType, dwSWEValue, dwAddPrice1, dwAddPrice2;
	WORD* wp;
	int    iEraseReq, iRet;
	short* sp;
	BOOL   bNeutral;


	// ¾ÆÀÌÅÛÀ» ÆÈ°Ú´Ù´Â °ÍÀÌ °áÁ¤µÇ¾ú´Ù.
	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;
	if ((cItemID < 0) || (cItemID >= 50)) return;
	if (m_pClientList[iClientH]->m_pItemList[cItemID] == NULL) return;
	if (iNum <= 0) return;
	if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwCount < iNum) return;

	// New 18/05/2004
	if (m_pClientList[iClientH]->m_pIsProcessingAllowed == FALSE) return;

	iCalcTotalWeight(iClientH);
	cItemCategory = m_pClientList[iClientH]->m_pItemList[cItemID]->m_cCategory;

	// v1.42
	bNeutral = FALSE;
	if (memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) == 0) bNeutral = TRUE;

	iPrice = 0;
	// ¾ÆÀÌÅÛÀÇ Á¾·ù¿¡ µû¶ó °¡°Ý °è»ê.
	if ((cItemCategory >= 1) && (cItemCategory <= 10)) {
		// ¹«±â·ù´Ù
		// ¿ø·¡ ¾ÆÀÌÅÛÀÇ ¼ö¸í°ú ºñ±³ÇØ¼­ °¨°¡ »ó°¢À» °è»ê, ¾ÆÀÌÅÛÀÇ °¡°ÝÀ» ¸Å±ä´Ù.
		sRemainLife = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan;

		if (sRemainLife > 0) {
			// ¸Á°¡Áø ¾ÆÀÌÅÛÀº ÆÈÁö ¸øÇÑ´Ù.	

			d1 = (double)sRemainLife;
			if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan != 0)
				d2 = (double)m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan;
			else d2 = 1.0f;
			d3 = (d1 / d2) * 0.5f;
			d2 = (double)m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice; // ¿ø·¡ °¡°Ý 
			d3 = d3 * d2; // Ãß»êµÈ °¡°Ý 

			iPrice = (short)d3;
			iPrice = iPrice * iNum;

			dwAddPrice1 = 0;
			dwAddPrice2 = 0;
			// ¾ÆÀÌÅÛ Æ¯¼ºÄ¡¿¡ µû¸¥ °¡°Ý »ó½Â 
			if ((m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x00F00000) != NULL) {
				dwSWEType = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x00F00000) >> 20;
				dwSWEValue = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x000F0000) >> 16;

				// Èñ±Í ¾ÆÀÌÅÛ È¿°ú Á¾·ù: 
				// 0-None 1-ÇÊ»ì±â´ë¹ÌÁöÃß°¡ 2-Áßµ¶È¿°ú 3-Á¤ÀÇÀÇ 4-ÀúÁÖÀÇ 
				// 5-¹ÎÃ¸ÀÇ 6-°¡º­¿î 7-¿¹¸®ÇÑ 8-°­È­µÈ 9-°í´ë¹®¸íÀÇ
				switch (dwSWEType) {
				case 6: dwMul1 = 2; break;  // °¡º­¿î 
				case 8: dwMul1 = 2; break;  // °­È­µÈ
				case 5: dwMul1 = 3; break;  // ¹ÎÃ¸ÀÇ
				case 1: dwMul1 = 4; break;  // ÇÊ»ìÀÇ 
				case 7: dwMul1 = 5; break;  // ¿¹¸®ÇÑ
				case 2: dwMul1 = 6; break;  // Áßµ¶ÀÇ
				case 3: dwMul1 = 15; break; // Á¤ÀÇÀÇ 
				case 9: dwMul1 = 20; break; // °í´ë¹®¸í 
				default: dwMul1 = 1; break;
				}

				d1 = (double)iPrice * dwMul1;
				switch (dwSWEValue) {
				case 1: d2 = 10.0f; break;
				case 2: d2 = 20.0f; break;
				case 3: d2 = 30.0f; break;
				case 4: d2 = 35.0f; break;
				case 5: d2 = 40.0f; break;
				case 6: d2 = 50.0f; break;
				case 7: d2 = 100.0f; break;
				case 8: d2 = 200.0f; break;
				case 9: d2 = 300.0f; break;
				case 10: d2 = 400.0f; break;
				case 11: d2 = 500.0f; break;
				case 12: d2 = 700.0f; break;
				case 13: d2 = 900.0f; break;
				default: d2 = 0.0f; break;
				}
				d3 = d1 * (d2 / 100.0f);
				dwAddPrice1 = (int)(d1 + d3);
			}

			// v1.42 Èñ±Í ¾ÆÀÌÅÛÀÌ¶ó¸é Sub È¿°ú¸¦ ¼³Á¤ÇÑ´Ù. °ø°Ý¹«±â´Â 1°³¸¸ ÀåÂøµÈ´Ù°í ÇßÀ»¶§¸¸ À¯È¿ÇÔ.
			if ((m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x0000F000) != NULL) {
				dwSWEType = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x0000F000) >> 12;
				dwSWEValue = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x00000F00) >> 8;

				// Èñ±Í ¾ÆÀÌÅÛ È¿°ú Á¾·ù: 
				//Ãß°¡ µ¶¼ºÀúÇ×(1), Ãß°¡ ¸íÁß°ª(2), Ãß°¡ ¹æ¾î°ª(3), HP È¸º¹·® Ãß°¡(4), SP È¸º¹·® Ãß°¡(5)
				//MP È¸º¹·® Ãß°¡(6), Ãß°¡ ¸¶¹ýÀúÇ×(7), ¹°¸® ´ë¹ÌÁö Èí¼ö(8), ¸¶¹ý ´ë¹ÌÁö Èí¼ö(9)
				//¿¬Å¸ ´ë¹ÌÁö Ãß°¡(10), ´õ ¸¹Àº °æÇèÄ¡(11), ´õ¸¹Àº Gold(12)
				switch (dwSWEType) {
				case 1:
				case 12: dwMul2 = 2; break;

				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7: dwMul2 = 4; break;

				case 8:
				case 9:
				case 10:
				case 11: dwMul2 = 6; break;
				}

				d1 = (double)iPrice * dwMul2;
				switch (dwSWEValue) {
				case 1: d2 = 10.0f; break;
				case 2: d2 = 20.0f; break;
				case 3: d2 = 30.0f; break;
				case 4: d2 = 35.0f; break;
				case 5: d2 = 40.0f; break;
				case 6: d2 = 50.0f; break;
				case 7: d2 = 100.0f; break;
				case 8: d2 = 200.0f; break;
				case 9: d2 = 300.0f; break;
				case 10: d2 = 400.0f; break;
				case 11: d2 = 500.0f; break;
				case 12: d2 = 700.0f; break;
				case 13: d2 = 900.0f; break;
				default: d2 = 0.0f; break;
				}
				d3 = d1 * (d2 / 100.0f);
				dwAddPrice2 = (int)(d1 + d3);
			}

			iPrice = iPrice + (dwAddPrice1 - (dwAddPrice1 / 3)) + (dwAddPrice2 - (dwAddPrice2 / 3));

			//v1.42 Áß¸³ÀÎ °æ¿ì ¹ÝÀÇ ¹Ý°ª.
			if (bNeutral == TRUE) iPrice = iPrice / 2;
			if (iPrice <= 0) iPrice = 1;
			if (iPrice > 1000000) iPrice = 1000000; // New 06/05/2004

			// ¾ÆÀÌÅÛÀ» ÆÈ¾Ò´Ù´Â ¸Þ½ÃÁö Àü¼Û (´ÙÀÌ¾ó·Î±× ¹Ú½º ºñÈ°¼ºÈ­¿ë)
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMSOLD, cItemID, NULL, NULL, NULL);

			_bItemLog(DEF_ITEMLOG_SELL, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[cItemID]);

			// ÆÈ ¾ÆÀÌÅÛÀ» »èÁ¦ 
			if ((m_pClientList[iClientH]->m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
				(m_pClientList[iClientH]->m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_ARROW)) {
				// ¼ö·® °³³äÀÌ ÀÖ´Ù¸é °¹¼ö¸¦ ÁÙÀÎ´Ù.
				// v1.41 !!!
				SetItemCount(iClientH, cItemID, m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwCount - iNum);
			}
			else ItemDepleteHandler(iClientH, cItemID, FALSE, FALSE);
		}
	}
	else if ((cItemCategory >= 11) && (cItemCategory <= 50)) {
		// ½ÄÇ°, ÀâÈ­µîÀÇ ¹Ý°ª¹°°Çµé 
		iPrice = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2;
		iPrice = iPrice * iNum;

		//v1.42 Áß¸³ÀÎ °æ¿ì ¹ÝÀÇ ¹Ý°ª.
		if (bNeutral == TRUE) iPrice = iPrice / 2;
		if (iPrice <= 0) iPrice = 1;
		if (iPrice > 1000000) iPrice = 1000000; // New 06/05/2004

		// ¾ÆÀÌÅÛÀ» ÆÈ¾Ò´Ù´Â ¸Þ½ÃÁö Àü¼Û (´ÙÀÌ¾ó·Î±× ¹Ú½º ºñÈ°¼ºÈ­¿ë)
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMSOLD, cItemID, NULL, NULL, NULL);

		_bItemLog(DEF_ITEMLOG_SELL, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[cItemID]);

		// ¾ÆÀÌÅÛÀÇ Á¾·ù¿¡ µû¶ó ÀûÀýÇÑ Ã³¸®¸¦ ÇÑ´Ù.
		if ((m_pClientList[iClientH]->m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
			(m_pClientList[iClientH]->m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_ARROW)) {
			// ¼ö·® °³³äÀÌ ÀÖ´Ù¸é °¹¼ö¸¦ ÁÙÀÎ´Ù.
			// v1.41 !!!
			SetItemCount(iClientH, cItemID, m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwCount - iNum);
		}
		else ItemDepleteHandler(iClientH, cItemID, FALSE, TRUE);
	}

	// Gold¸¦ Áõ°¡½ÃÅ²´Ù. ¸¸¾à ÆÇ °¡°ÝÀÌ 0 È¤Àº ¸¶ÀÌ³Ê½ºÀÌ¸é ±ÝÀ» ÁÖÁö ¾Ê´Â´Ù.
	if (iPrice <= 0) return;

	pItemGold = new class CItem;
	ZeroMemory(cItemName, sizeof(cItemName));
	wsprintf(cItemName, "Gold");
	_bInitItemAttr(pItemGold, cItemName);

	pItemGold->m_dwCount = iPrice;

	if (_bAddClientItemList(iClientH, pItemGold, &iEraseReq) == TRUE) {
		// ¾ÆÀÌÅÛÀ» È¹µæÇß´Ù.

		dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_ITEMOBTAINED;

		cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);

		// 1°³ È¹µæÇß´Ù. <- ¿©±â¼­ 1°³¶õ Ä«¿îÆ®¸¦ ¸»ÇÏ´Â °ÍÀÌ ¾Æ´Ï´Ù
		*cp = 1;
		cp++;

		memcpy(cp, pItemGold->m_cName, 20);
		cp += 20;

		dwp = (DWORD*)cp;
		*dwp = pItemGold->m_dwCount;
		cp += 4;

		*cp = pItemGold->m_cItemType;
		cp++;

		*cp = pItemGold->m_cEquipPos;
		cp++;

		*cp = (char)0; // ¾òÀº ¾ÆÀÌÅÛÀÌ¹Ç·Î ÀåÂøµÇÁö ¾Ê¾Ò´Ù.
		cp++;

		sp = (short*)cp;
		*sp = pItemGold->m_sLevelLimit;
		cp += 2;

		*cp = pItemGold->m_cGenderLimit;
		cp++;

		wp = (WORD*)cp;
		*wp = pItemGold->m_wCurLifeSpan;
		cp += 2;

		wp = (WORD*)cp;
		*wp = pItemGold->m_wWeight;
		cp += 2;

		sp = (short*)cp;
		*sp = pItemGold->m_sSprite;
		cp += 2;

		sp = (short*)cp;
		*sp = pItemGold->m_sSpriteFrame;
		cp += 2;

		*cp = pItemGold->m_cItemColor;
		cp++;

		*cp = (char)pItemGold->m_sItemSpecEffectValue2; // v1.41 
		cp++;

		dwp = (DWORD*)cp;
		*dwp = pItemGold->m_dwAttribute;
		cp += 4;


		if (iEraseReq == 1)
			delete pItemGold;

		// ¾ÆÀÌÅÛ Á¤º¸ Àü¼Û 
		iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);

		// ¼ÒÁöÇ° ÃÑ Áß·® Àç °è»ê 
		iCalcTotalWeight(iClientH);

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
		// Áß·® ÃÊ°úµîÀÇ ¹®Á¦·Î Ãß°¡ ½ÇÆÐ.
		// ¹ÞÁö ¸øÇßÀ¸¹Ç·Î ¹Ù´Ú¿¡ ¶³¾îÁø´Ù. 
		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
			m_pClientList[iClientH]->m_sY, pItemGold);

		// ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¾ÆÀÌÅÛÀÌ ¶³¾îÁø °ÍÀ» ¾Ë¸°´Ù. 
		SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
			m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
			pItemGold->m_sSprite, pItemGold->m_sSpriteFrame, pItemGold->m_cItemColor); // v1.4 color

		// ¼ÒÁöÇ° ÃÑ Áß·® Àç °è»ê 
		iCalcTotalWeight(iClientH);

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

void CGame::ReqRepairItemHandler(int iClientH, char cItemID, char cRepairWhom, char* pString)
{
	char cItemCategory;
	short sRemainLife, sPrice;
	double d1, d2, d3;

	// ¾ÆÀÌÅÛÀ» °íÄ¡°Ú´Ù´Â ¿ä±¸.
	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;
	if ((cItemID < 0) || (cItemID >= 50)) return;
	if (m_pClientList[iClientH]->m_pItemList[cItemID] == NULL) return;

	cItemCategory = m_pClientList[iClientH]->m_pItemList[cItemID]->m_cCategory;

	// ¾ÆÀÌÅÛÀÇ Á¾·ù¿¡ µû¶ó °¡°Ý °è»ê.
	if ((cItemCategory >= 1) && (cItemCategory <= 10)) {
		// ¹«±â·ù´Ù

		// ¸¸¾à ¹«±â¸¦ ´ëÀå°£ ÁÖÀÎÀÌ ¾Æ´Ñ ÀÌ¿¡°Ô °íÃÄ´Þ¶ó°í ÇÑ´Ù¸é ÇÒ ¼ö ¾ø´Ù. 
		if (cRepairWhom != 24) {
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CANNOTREPAIRITEM, cItemID, 2, NULL, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
			return;
		}

		// ¿ø·¡ ¾ÆÀÌÅÛÀÇ ¼ö¸í°ú ºñ±³ÇØ¼­ °¨°¡ »ó°¢À» °è»ê, ¾ÆÀÌÅÛÀÇ ¼ö¸® ºñ¿ëÀ» ¸Å±ä´Ù.
		sRemainLife = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan;
		if (sRemainLife == 0) {
			// ¿ÏÀüÈ÷ ¸Á°¡Áø °ÍÀÌ¶ó¸é ¿ø·¡°¡°ÝÀÇ Àý¹ÝÀÌ µç´Ù. 
			sPrice = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2;
		}
		else {
			d1 = (double)sRemainLife;
			if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan != 0)
				d2 = (double)m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan;
			else d2 = 1.0f;
			d3 = (d1 / d2) * 0.5f;
			d2 = (double)m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice; // ¿ø·¡ °¡°Ý 
			d3 = d3 * d2; // Ãß»êµÈ °¡°Ý 

			sPrice = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2) - (short)d3;
		}

		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_REPAIRITEMPRICE, cItemID, sRemainLife, sPrice, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
	}
	else if (((cItemCategory >= 43) && (cItemCategory <= 50)) || ((cItemCategory >= 11) && (cItemCategory <= 12))) {
		// ³¬½Ã´ë, °î±ªÀÌ µî°ú °°Àº ºñ¹«±â·ù ¼ö¸®°¡´É ¾ÆÀÌÅÛ. ¿Ê, ºÎÃ÷·ù

		// ¸¸¾à »óÁ¡ ÁÖÀÎÀÌ ¾Æ´Ñ ÀÌ¿¡°Ô °íÃÄ´Þ¶ó°í ÇÑ´Ù¸é ÇÒ ¼ö ¾ø´Ù. 
		if (cRepairWhom != 15) {
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CANNOTREPAIRITEM, cItemID, 2, NULL, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
			return;
		}

		// ¿ø·¡ ¾ÆÀÌÅÛÀÇ ¼ö¸í°ú ºñ±³ÇØ¼­ °¨°¡ »ó°¢À» °è»ê, ¾ÆÀÌÅÛÀÇ ¼ö¸® ºñ¿ëÀ» ¸Å±ä´Ù.
		sRemainLife = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan;
		if (sRemainLife == 0) {
			// ¿ÏÀüÈ÷ ¸Á°¡Áø °ÍÀÌ¶ó¸é ¿ø·¡°¡°ÝÀÇ Àý¹ÝÀÌ µç´Ù. 
			sPrice = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2;
		}
		else {
			d1 = (double)sRemainLife;
			if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan != 0)
				d2 = (double)m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan;
			else d2 = 1.0f;
			d3 = (d1 / d2) * 0.5f;
			d2 = (double)m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice; // ¿ø·¡ °¡°Ý 
			d3 = d3 * d2; // Ãß»êµÈ °¡°Ý 

			sPrice = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2) - (short)d3;
		}

		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_REPAIRITEMPRICE, cItemID, sRemainLife, sPrice, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
	}
	else {
		// °íÄ¥¼ö ¾ø´Â ¾ÆÀÌÅÛÀÌ´Ù.
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CANNOTREPAIRITEM, cItemID, 1, NULL, m_pClientList[iClientH]->m_pItemList[cItemID]->m_cName);
	}
}

void CGame::ReqRepairItemCofirmHandler(int iClientH, char cItemID, char* pString)
{
	short    sRemainLife, sPrice;
	char* cp, cItemCategory, cData[120];
	double   d1, d2, d3, dV1, dV2, dV3;
	DWORD* dwp, dwGoldCount, dwSWEType, dwSWEValue;
	WORD* wp;
	int      iRet, iGoldWeight;

	// ¾ÆÀÌÅÛÀ» ¼ö¸®ÇÏ°Ú´Ù´Â °ÍÀÌ °áÁ¤µÇ¾ú´Ù.
	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;

	if ((cItemID < 0) || (cItemID >= 50)) return;
	if (m_pClientList[iClientH]->m_pItemList[cItemID] == NULL) return;

	// New 18/05/2004
	if (m_pClientList[iClientH]->m_pIsProcessingAllowed == FALSE) return;

	cItemCategory = m_pClientList[iClientH]->m_pItemList[cItemID]->m_cCategory;

	// ¾ÆÀÌÅÛÀÇ Á¾·ù¿¡ µû¶ó °¡°Ý °è»ê.
	if (((cItemCategory >= 1) && (cItemCategory <= 10)) || ((cItemCategory >= 43) && (cItemCategory <= 50)) ||
		((cItemCategory >= 11) && (cItemCategory <= 12))) {
		// ¹«±â·ù È¤Àº ³¬½Ã´ë, °î±ªÀÌ¿Í °°Àº ¾ÆÀÌÅÛ, ¿Ê, ½Å¹ß 

		// ¿ø·¡ ¾ÆÀÌÅÛÀÇ ¼ö¸í°ú ºñ±³ÇØ¼­ °¨°¡ »ó°¢À» °è»ê, ¾ÆÀÌÅÛÀÇ ¼ö¸® ºñ¿ëÀ» ¸Å±ä´Ù.
		sRemainLife = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan;
		if (sRemainLife == 0) {
			// ¿ÏÀüÈ÷ ¸Á°¡Áø °ÍÀÌ¶ó¸é ¿ø·¡°¡°ÝÀÇ Àý¹ÝÀÌ µç´Ù. 
			sPrice = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2;
		}
		else {
			d1 = (double)abs(sRemainLife);
			if (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan != 0)
				d2 = (double)abs(m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan);
			else d2 = 1.0f;
			d3 = (d1 / d2) * 0.5f;
			d2 = (double)m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice; // ¿ø·¡ °¡°Ý 
			d3 = d3 * d2; // Ãß»êµÈ °¡°Ý 

			sPrice = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_wPrice / 2) - (short)d3;
		}

		// sPrice¸¸Å­ÀÇ µ·ÀÌ µÇ¸é °íÄ¥ ¼ö ÀÖÀ¸³ª ºÎÁ·ÇÏ¸é °íÄ¥ ¼ö ¾ø´Ù. 
		dwGoldCount = dwGetItemCount(iClientH, "Gold");

		if (dwGoldCount < (DWORD)sPrice) {
			// ÇÃ·¹ÀÌ¾î°¡ °®°íÀÖ´Â Gold°¡ ¾ÆÀÌÅÛ ¼ö¸® ºñ¿ë¿¡ ºñÇØ Àû´Ù. °íÄ¥ ¼ö ¾øÀ½.
			dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_NOTENOUGHGOLD;
			cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);
			*cp = cItemID;
			cp++;

			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 7);
			switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				// ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù.
				DeleteClient(iClientH, TRUE, TRUE);
				break;
			}
			return;
		}
		else {
			//µ·ÀÌ ÃæºÐÇÏ´Ù. °íÄ¥ ¼ö ÀÖ´Ù. 

			// Centuu : Repair Fix (Recalculation of maxlifespan) - HB2
			for (int i = 0; i < DEF_MAXITEMTYPES; i++) {
				if ((m_pItemConfigList[i] != NULL) &&
					(m_pItemConfigList[i]->m_sIDnum == m_pClientList[iClientH]->m_pItemList[cItemID]->m_sIDnum)) {
					m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan = m_pItemConfigList[i]->m_wMaxLifeSpan;
					if ((m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x00F00000) != NULL) {
						dwSWEType = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x00F00000) >> 20;
						dwSWEValue = (m_pClientList[iClientH]->m_pItemList[cItemID]->m_dwAttribute & 0x000F0000) >> 16;
						switch (dwSWEType) {
						case 8: // Strong
						case 9: // Ancient
							dV2 = (double)m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan;
							dV3 = (double)(dwSWEValue * 7);
							dV1 = (dV3 / 100.0f) * dV2;
							m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan += (int)dV1;
							break;
						}
					}
				}
			}

			// ¾ÆÀÌÅÛÀÇ ¼ö¸íÀ» ´Ã¸®°í Åëº¸ÇÑ´Ù. !BUG POINT À§Ä¡°¡ Áß¿äÇÏ´Ù. ¸ÕÀú ¼ö¸íÀ» ´Ã¸®°í µ·ÀÇ Ä«¿îÆ®¸¦ ³·Ãá´Ù.
			m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan = m_pClientList[iClientH]->m_pItemList[cItemID]->m_wMaxLifeSpan;
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMREPAIRED, cItemID, m_pClientList[iClientH]->m_pItemList[cItemID]->m_wCurLifeSpan, NULL, NULL);

			iGoldWeight = SetItemCount(iClientH, "Gold", dwGoldCount - sPrice);

			// ¼ÒÁöÇ° ÃÑ Áß·® Àç °è»ê 
			iCalcTotalWeight(iClientH);


		}
	}
}

int CGame::iCalcTotalWeight(int iClientH)
{
	int i, iWeight;
	short sItemIndex;

	if (m_pClientList[iClientH] == NULL) return 0;

	m_pClientList[iClientH]->m_iAlterItemDropIndex = -1;
	// Âø¿ëÇÏÁö ¾Ê¾Æµµ È¿°ú°¡ ÀÖ´Â ¾ÆÀÌÅÛ È¿°ú. ¼ÒÁöÇÏ°í ÀÖ¾îµµ È¿°ú°¡ ÀÖ±â ¶§¹®¿¡ ¿©±â¼­ °Ë»ç.
	for (sItemIndex = 0; sItemIndex < DEF_MAXITEMS; sItemIndex++)
		if (m_pClientList[iClientH]->m_pItemList[sItemIndex] != NULL) {
			switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType) {
			case DEF_ITEMEFFECTTYPE_ALTERITEMDROP:
				if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan > 0) {
					// ¼ö¸íÀÌ ÀÖ¾î¾ß È¿°ú°¡ ÀÖ´Ù.
					m_pClientList[iClientH]->m_iAlterItemDropIndex = sItemIndex;
				}
				break;
			}
		}

	iWeight = 0;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemList[i] != NULL) {

			iWeight += iGetItemWeight(m_pClientList[iClientH]->m_pItemList[i], m_pClientList[iClientH]->m_pItemList[i]->m_dwCount);
		}

	m_pClientList[iClientH]->m_iCurWeightLoad = iWeight;

	return iWeight;
}

// 05/29/2004 - Hypnotoad - Purchase Dicount updated to take charisma into consideration
void CGame::RequestPurchaseItemHandler(int iClientH, char* pItemName, int iNum)
{
	class CItem* pItem;
	char* cp, cItemName[21], cData[100];
	short* sp;
	DWORD* dwp, dwGoldCount, dwItemCount;
	WORD* wp, wTempPrice;
	int   i, iRet, iEraseReq, iGoldWeight;
	int   iCost, iCost2, iDiscountRatio, iDiscountCost;
	double dTmp1, dTmp2, dTmp3;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;
	// ���� �������� �����ϰ��� �ϴ� ���� �ڽ��� ������ �ƴ϶�� ������ �� ����. 


	// �������� �����Ѵ�. 
	ZeroMemory(cData, sizeof(cData));
	ZeroMemory(cItemName, sizeof(cItemName));


	// �ӽ��ڵ��. 
	if (memcmp(pItemName, "10Arrows", 8) == 0) {
		strcpy(cItemName, "Arrow");
		dwItemCount = 10;
	}
	else if (memcmp(pItemName, "100Arrows", 9) == 0) {
		strcpy(cItemName, "Arrow");
		dwItemCount = 100;
	}
	else {
		memcpy(cItemName, pItemName, 20);
		dwItemCount = 1;
	}

	for (i = 1; i <= iNum; i++) {

		pItem = new class CItem;
		if (_bInitItemAttr(pItem, cItemName) == FALSE) {
			// �����ϰ��� �ϴ� �������� ������ ����Ʈ�� ����. ������ �Ұ����ϴ�.
			delete pItem;
		}
		else {

			if (pItem->m_bIsForSale == FALSE) {
				// �ǸŵǴ� �������� �ƴϴ�. �� �� ����.
				delete pItem;
				return;
			}

			pItem->m_dwCount = dwItemCount;


			//Heldenian Price Fix Thing
			if (m_pClientList[iClientH]->m_cSide == m_sLastHeldenianWinner) 
			{
				iCost = (int)((float)(pItem->m_wPrice) * 0.9f + 0.5f);
				iCost = iCost * pItem->m_dwCount;
				iCost2 = pItem->m_wPrice * pItem->m_dwCount;
			}
			else 
			{
				iCost2 = iCost = (pItem->m_wPrice * pItem->m_dwCount); //LifeX Fix gold price 11/2
			}

			// �÷��̾ ������ Gold�� �������� ��⿡ ������� �˻��Ѵ�.
			dwGoldCount = dwGetItemCount(iClientH, "Gold");

			// Charisma�� ���� ���η��� ����Ѵ�. 
			// v2.14 ī�������� 10�ΰ�� �������� ���� ���� ���� 
			iDiscountRatio = (int)((m_pClientList[iClientH]->m_iCharisma - 10) / 4);

			dTmp1 = (double)(iDiscountRatio);
			dTmp2 = dTmp1 / 100.0f;
			dTmp1 = (double)iCost;
			dTmp3 = dTmp1 * dTmp2;
			iDiscountCost = (int)dTmp3;

			//Fix For Heldenian Price =x	
			if ((iCost - iDiscountCost) <= (int)(iCost2 / 2)) 
			{
				iDiscountCost = (int)(iCost - (iCost2 / 2) + 1);
			}


			if (dwGoldCount < (DWORD)(iCost - iDiscountCost)) {
				// �÷��̾ �����ִ� Gold�� ������ ���ݿ� ���� ����. ��� ����.
				delete pItem;

				dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_NOTENOUGHGOLD;
				cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);
				*cp = -1; // -1�̸� �ǹ̾���.
				cp++;

				iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 7);
				switch (iRet) {
				case DEF_XSOCKEVENT_QUENEFULL:
				case DEF_XSOCKEVENT_SOCKETERROR:
				case DEF_XSOCKEVENT_CRITICALERROR:
				case DEF_XSOCKEVENT_SOCKETCLOSED:
					// �޽����� ������ ������ �߻��ߴٸ� �����Ѵ�.
					DeleteClient(iClientH, TRUE, TRUE);
					break;
				}
				return;
			}

			if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == TRUE) {
				// ���� ������ �ڵ�
				if (m_pClientList[iClientH]->m_iCurWeightLoad < 0) m_pClientList[iClientH]->m_iCurWeightLoad = 0;

				// ������ ��ٴ� �޽����� �����Ѵ�.
				dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
				*dwp = MSGID_NOTIFY;
				wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
				*wp = DEF_NOTIFY_ITEMPURCHASED;

				cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);
				// 1�� ȹ���ߴ�.
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

				*cp = (char)0; // ���� �������̹Ƿ� �������� �ʾҴ�.
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

				wp = (WORD*)cp;
				*wp = (iCost - iDiscountCost);
				wTempPrice = (iCost - iDiscountCost);
				cp += 2;

				if (iEraseReq == 1) delete pItem;

				// ������ ���� ���� 
				iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 48);

				// Gold�� ������ ���ҽ�Ų��. �ݵ�� ���⼭ �����ؾ� ������ �ٲ��� �ʴ´�.
				iGoldWeight = SetItemCount(iClientH, "Gold", dwGoldCount - wTempPrice);
				// ����ǰ �� �߷� �� ��� 
				iCalcTotalWeight(iClientH);



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
			else
			{
				// ������ ������ �������� ���� �� ����.
				delete pItem;

				// ����ǰ �� �߷� �� ��� 
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
					// �޽����� ������ ������ �߻��ߴٸ� �����Ѵ�.
					DeleteClient(iClientH, TRUE, TRUE);
					break;
				}
			}
		}
	}
}

void CGame::GiveItemHandler(int iClientH, short sItemIndex, int iAmount, short dX, short dY, WORD wObjectID, char* pItemName)
{
	int iRet, iEraseReq;
	short* sp, sOwnerH;
	char* cp, cOwnerType, cData[100], cCharName[21];
	DWORD* dwp;
	WORD* wp;
	class CItem* pItem;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsOnServerChange == TRUE) return;
	if ((m_bAdminSecurity == TRUE) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0)) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == NULL) return;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
	if (iAmount <= 0) return;

	// ¾ÆÀÌÅÛ ÀÌ¸§ÀÌ ÀÏÄ¡ÇÏÁö ¾Ê¾Æµµ ¹«½ÃµÈ´Ù.
	if (memcmp(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName, pItemName, 20) != 0) {
		PutLogList("(X) GiveItemHandler - Not matching Item name");
		return;
	}

	ZeroMemory(cCharName, sizeof(cCharName));

	if (((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
		(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW)) &&
		(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount > (DWORD)iAmount)) {
		// ¼Òºñ¼º ¾ÆÀÌÅÛÀÌ¾ú°í ¼ö·®¸¸Å­ °¨¼Ò½ÃÅ°°í ³²Àº °Ô ÀÖ´Ù¸é 

		pItem = new class CItem;
		if (_bInitItemAttr(pItem, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName) == FALSE) {
			// ºÐÇÒÇÏ°íÀÚ ÇÏ´Â ¾ÆÀÌÅÛÀÌ ¸®½ºÆ®¿¡ ¾ø´Â °Å´Ù. ÀÌ·± ÀÏÀº ÀÏ¾î³¯ ¼ö°¡ ¾øÁö¸¸ 
			delete pItem;
			return;
		}
		else {
			pItem->m_dwCount = iAmount;
		}

		// ¼ö·® °¨¼Ò: 0º¸´Ù Å©´Ù.
		m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount -= iAmount;

		// º¯°æµÈ ¼ö·®À» ¼³Á¤ÇÏ°í ¾Ë¸°´Ù.
		// v1.41 !!! ¾ÆÀÌÅÛ ÀÌ¸§¿¡¼­ ÀÎµ¦½º·Î º¯°æµÇ¾ú´Ù. 
		SetItemCount(iClientH, sItemIndex, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount);

		//ÀÌÁ¦ dX, dY¿¡ ÀÖ´Â ¿ÀºêÁ§Æ®¿¡°Ô ¼Òºñ¼º ¾ÆÀÌÅÛÀ» °Ç³×ÁØ´Ù. 
		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY);

		// v1.4 ÁÖ°íÀÚ ÇÑ °´Ã¼¿Í ¸Â´ÂÁö ÆÇ´ÜÇÑ´Ù.
		if (wObjectID != NULL) {
			if (wObjectID < 10000) {
				// ÇÃ·¹ÀÌ¾î 
				if ((wObjectID > 0) && (wObjectID < DEF_MAXCLIENTS)) {
					if (m_pClientList[wObjectID] != NULL) {
						if ((WORD)sOwnerH != wObjectID) sOwnerH = NULL;
					}
				}
			}
			else {
				// NPC
				if ((wObjectID - 10000 > 0) && (wObjectID - 10000 < DEF_MAXNPCS)) {
					if (m_pNpcList[wObjectID - 10000] != NULL) {
						if ((WORD)sOwnerH != (wObjectID - 10000)) sOwnerH = NULL;
					}
				}
			}
		}

		// Snoopy: Bock attemps to give item to a far character.
		if (((abs(m_pClientList[iClientH]->m_sX) - dX) > 5)
			|| ((abs(m_pClientList[iClientH]->m_sY) - dY) > 5))
		{
			sOwnerH = NULL;
		}

		if (sOwnerH == NULL) {
			// ÁÖ°íÀÚ ÇÏ´Â À§Ä¡¿¡ ¾Æ¹«µµ ¾ø´Ù.
			// ¾ÆÀÌÅÛÀ» ¼­ÀÖ´Â À§Ä¡¿¡ ¹ö¸°´Ù. 
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, pItem);

			// v1.411  
			_bItemLog(DEF_ITEMLOG_DROP, iClientH, NULL, pItem);

			// ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¾ÆÀÌÅÛÀÌ ¶³¾îÁø °ÍÀ» ¾Ë¸°´Ù. 
			SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
				m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
				pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); //v1.4 color
		}
		else {
			// ¾ÆÀÌÅÛÀ» ÁØ´Ù.
			if (cOwnerType == DEF_OWNERTYPE_PLAYER) {
				// ÇÃ·¹ÀÌ¾î¿¡°Ô ÁÖ¾ú´Ù.
				memcpy(cCharName, m_pClientList[sOwnerH]->m_cCharName, 10);

				if (sOwnerH == iClientH) {
					// ¸¸¾à ÀÚ±â ÀÚ½Å¿¡°Ô ÁÖ´Â °Å¶ó¸é ¹«½ÃÇÑ´Ù. ÇØÅ·ÀÇ ¼ÒÁö°¡ ÀÖ´Ù.
					delete pItem;
					return;
				}

				if (_bAddClientItemList(sOwnerH, pItem, &iEraseReq) == TRUE) {
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
					iRet = m_pClientList[sOwnerH]->m_pXSock->iSendMsg(cData, 53);
					switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						// ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù.
						DeleteClient(sOwnerH, TRUE, TRUE);
						break;
					}

					// v1.4 ¼ö·®´ÜÀ§ÀÇ ¾ÆÀÌÅÛÀ» Àü´ÞÇÑ °ÍÀ» ¾Ë·ÁÁØ´Ù. 
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_GIVEITEMFIN_COUNTCHANGED, sItemIndex, iAmount, NULL, cCharName);
				}
				else {
					// ¾ÆÀÌÅÛÀ» Àü´Þ¹ÞÀº Ä³¸¯ÅÍ°¡ ´õÀÌ»ó ¾ÆÀÌÅÛÀ» º¸°üÇÒ ¼ö ¾ø´Â »óÅÂÀÌ´Ù.
					// ¾ÆÀÌÅÛÀ» ¼­ÀÖ´Â À§Ä¡¿¡ ¹ö¸°´Ù. 
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
						m_pClientList[iClientH]->m_sY,
						pItem);

					// v1.411  
					_bItemLog(DEF_ITEMLOG_DROP, iClientH, NULL, pItem);

					// ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¾ÆÀÌÅÛÀÌ ¶³¾îÁø °ÍÀ» ¾Ë¸°´Ù. 
					SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
						m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
						pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); //v1.4 color

// ´õÀÌ»ó °¡Áú¼ö ¾ø´Ù´Â ¸Þ½ÃÁö¸¦ º¸³½´Ù.
					dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
					*dwp = MSGID_NOTIFY;
					wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
					*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

					iRet = m_pClientList[sOwnerH]->m_pXSock->iSendMsg(cData, 6);
					switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						// ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù.
						DeleteClient(sOwnerH, TRUE, TRUE);
						break;
					}

					// v1.4 ¼ö·®´ÜÀ§ÀÇ ¾ÆÀÌÅÛÀ» Àü´Þ¿¡ ½ÇÆÐÇßÀ½À» ¾Ë¸°´Ù.
					SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CANNOTGIVEITEM, sItemIndex, iAmount, NULL, cCharName);
				}

			}
			else {
				// NPC¿¡°Ô ¾ÆÀÌÅÛÀ» ÁÖ¾ú´Ù.
				memcpy(cCharName, m_pNpcList[sOwnerH]->m_cNpcName, 20);

				if (memcmp(m_pNpcList[sOwnerH]->m_cNpcName, "Howard", 6) == 0) {
					// NPC°¡ Ã¢°í ÁÖÀÎÀÌ¾ú´Ù¸é ¹°°ÇÀ» º¸°üÇÏ°Ú´Ù´Â ÀÇ¹ÌÀÌ´Ù. 
					if (bSetItemToBankItem(iClientH, pItem) == FALSE) {
						// ¹°°ÇÀ» ¸Ã±â´Âµ¥ ½ÇÆÐÇÏ¿´´Ù.	
						SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CANNOTITEMTOBANK, NULL, NULL, NULL, NULL);

						// ½ÇÆÐÇßÀ¸¹Ç·Î ¹Ù´Ú¿¡ ¶³±º´Ù.
						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, pItem);

						// v1.411  
						_bItemLog(DEF_ITEMLOG_DROP, iClientH, NULL, pItem);

						// ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¾ÆÀÌÅÛÀÌ ¶³¾îÁø °ÍÀ» ¾Ë¸°´Ù. 
						SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
							m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
							pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color
					}
				}
				else {
					// ÀÏ¹Ý NPC¿¡°Ô ¾ÆÀÌÅÛÀ» ÁÖ¸é ¾ÆÀÌÅÛÀ» ¼­ÀÖ´Â À§Ä¡¿¡ ¹ö·Á¾ß ÇÑ´Ù. 
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, pItem);

					// v1.411  
					_bItemLog(DEF_ITEMLOG_DROP, iClientH, NULL, pItem);

					// ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¾ÆÀÌÅÛÀÌ ¶³¾îÁø °ÍÀ» ¾Ë¸°´Ù. 
					SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
						m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
						pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color
				}
			}
		}
	}
	else {
		// ¾ÆÀÌÅÛ ÀüºÎ¸¦ ÁÖ¾ú´Ù.

		// ÀåÂøÁßÀÎ ¾ÆÀÌÅÛÀ» ÁÖ¾ú´Ù¸é ¾ÆÀÌÅÛ ÀåÂøÈ¿°ú¸¦ ÇØÁ¦ÇØ¾ß ÇÏ¹Ç·Î.
		ReleaseItemHandler(iClientH, sItemIndex, TRUE);

		// Ã³¸®µµÁß ¿¡·¯°¡ ¹ß»ýÇÒ¶§¸¦ ´ëºñÇØ¼­ -1·Î ÇÒ´çÇØ ³õ´Â´Ù.
		if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW)
			m_pClientList[iClientH]->m_cArrowIndex = -1;

		//ÀÌÁ¦ dX, dY¿¡ ÀÖ´Â ¿ÀºêÁ§Æ®¿¡°Ô ¼Òºñ¼º ¾ÆÀÌÅÛÀ» °Ç³×ÁØ´Ù. 
		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->GetOwner(&sOwnerH, &cOwnerType, dX, dY); // ¾ÆÀÌÅÛÀ» dX, dY¿¡ ÀÖ´Â Ä³¸¯ÅÍ¿¡°Ô ÁØ´Ù. ¸¸¾à ¹ÞÀ» Ä³¸¯ÅÍ°¡ ¾ÆÀÌÅÛÀ» ¹ÞÁö ¸øÇÒ »óÈ²ÀÌ¶ó¸é ¶¥¿¡ ¶³¾îÁø´Ù.  

		// v1.4 ÁÖ°íÀÚ ÇÑ °´Ã¼¿Í ¸Â´ÂÁö ÆÇ´ÜÇÑ´Ù.
		if (wObjectID != NULL) {
			if (wObjectID < 10000) {
				// ÇÃ·¹ÀÌ¾î 
				if ((wObjectID > 0) && (wObjectID < DEF_MAXCLIENTS)) {
					if (m_pClientList[wObjectID] != NULL) {
						if ((WORD)sOwnerH != wObjectID) sOwnerH = NULL;
					}
				}
			}
			else {
				// NPC
				if ((wObjectID - 10000 > 0) && (wObjectID - 10000 < DEF_MAXNPCS)) {
					if (m_pNpcList[wObjectID - 10000] != NULL) {
						if ((WORD)sOwnerH != (wObjectID - 10000)) sOwnerH = NULL;
					}
				}
			}
		}

		// Snoopy: Bock attemps to give item to a far character.
		if (((abs(m_pClientList[iClientH]->m_sX) - dX) > 5)
			|| ((abs(m_pClientList[iClientH]->m_sY) - dY) > 5))
		{
			sOwnerH = NULL;
		}

		if (sOwnerH == NULL) {
			// ¾ÆÀÌÅÛÀ» ÁÖ°íÀÚ ÇÏ´Â Àå¼Ò¿¡ Ä³¸¯ÅÍ°¡ ¾ø´Ù. 
			// ¾ÆÀÌÅÛÀ» ¼­ÀÖ´Â À§Ä¡¿¡ ¹ö¸°´Ù. 
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
				m_pClientList[iClientH]->m_sY,
				m_pClientList[iClientH]->m_pItemList[sItemIndex]);
			// v1.411  
			_bItemLog(DEF_ITEMLOG_DROP, iClientH, NULL, m_pClientList[iClientH]->m_pItemList[sItemIndex]);

			// ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¾ÆÀÌÅÛÀÌ ¶³¾îÁø °ÍÀ» ¾Ë¸°´Ù. 
			SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
				m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
				m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
				m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame,
				m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor); // v1.4 color

// ÀÌÁ¦ ¾ÆÀÌÅÛÀÌ ¶³¾îÁ³À¸¹Ç·Î ¸®½ºÆ®¿¡¼­ »èÁ¦ÇÒ°ÍÀ» Åëº¸ÇÑ´Ù.
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_DROPITEMFIN_ERASEITEM, sItemIndex, iAmount, NULL, NULL);
		}
		else {
			// ¾ÆÀÌÅÛÀ» ´Ù¸¥ Ä³¸¯ÅÍ¿¡°Ô ÁØ´Ù. @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

			if (cOwnerType == DEF_OWNERTYPE_PLAYER) {
				// ĳ���Ϳ��� �������� �־���.	
				memcpy(cCharName, m_pClientList[sOwnerH]->m_cCharName, 10);
				pItem = m_pClientList[iClientH]->m_pItemList[sItemIndex];

				// v2.03 ũ�缼�̵� ����ΰ�쵵 ��� ������ �����ϴ�.

				// v2.17 2002-7-31 �������� ������ ������ȣ�� ������ �� �ְ� �Ѵ�.
				if (pItem->m_sIDnum == 88) {

					// �÷��̾� iClientH �� sOwnerH���� ��尡�� ��û���� �־���. ���� 
					// sOwnerH�� ��帶���Ͷ�� �������� �޴� ���� �ƴ϶� Ȯ���� �� �־�� �Ѵ�.
								// v2.17 2002-7-31 �������� ������ ������ȣ�� ������ �� �ְ� �Ѵ�.
					if ((m_pClientList[iClientH]->m_iGuildRank == -1) &&
						(memcmp(m_pClientList[iClientH]->m_cLocation, "NONE", 4) != 0) &&
						(memcmp(m_pClientList[iClientH]->m_cLocation, m_pClientList[sOwnerH]->m_cLocation, 10) == 0) &&
						(m_pClientList[sOwnerH]->m_iGuildRank == 0)) {
						// ��� �������̴�.	��� �����Ϳ��Դ� ����Ȯ�� ��û �޽����� �����Ѵ�.
						SendNotifyMsg(iClientH, sOwnerH, DEF_NOTIFY_QUERY_JOINGUILDREQPERMISSION, NULL, NULL, NULL, NULL);
						// ���� �������� �� ���ο��� �������� �־����Ƿ� ����Ʈ���� �����Ұ��� �뺸�Ѵ�.
						SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_GIVEITEMFIN_ERASEITEM, sItemIndex, 1, NULL, cCharName);

						_bItemLog(DEF_ITEMLOG_DEPLETE, iClientH, (int)-1, pItem);

						goto REMOVE_ITEM_PROCEDURE;
					}
				}

				// v2.17 2002-7-31 �������� ������ ������ȣ�� ������ �� �ְ� �Ѵ�.
				if ((m_bIsCrusadeMode == FALSE) && (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 89)) {

					// �÷��̾� iClientH �� sOwnerH���� ���Ż�� ��û���� �־���. ���� 
					// sOwnerH�� ��帶�����̰� iClientH�� ��� �̸��� ���� iClientH�� �����̶�� 
					// �������� �޴� ���� �ƴ϶� Ȯ���� �� �־�� �Ѵ�.
					if ((memcmp(m_pClientList[iClientH]->m_cGuildName, m_pClientList[sOwnerH]->m_cGuildName, 20) == 0) &&
						(m_pClientList[iClientH]->m_iGuildRank != -1) &&
						(m_pClientList[sOwnerH]->m_iGuildRank == 0)) {
						// ��� �������̴�.	��� �����Ϳ��Դ� Ż��Ȯ�� ��û �޽����� �����Ѵ�.
						SendNotifyMsg(iClientH, sOwnerH, DEF_NOTIFY_QUERY_DISMISSGUILDREQPERMISSION, NULL, NULL, NULL, NULL);
						// ���� �������� �� ���ο��� �������� �־����Ƿ� ����Ʈ���� �����Ұ��� �뺸�Ѵ�.
						SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_GIVEITEMFIN_ERASEITEM, sItemIndex, 1, NULL, cCharName);

						_bItemLog(DEF_ITEMLOG_DEPLETE, iClientH, (int)-1, pItem);

						goto REMOVE_ITEM_PROCEDURE;
					}
				}

				// ÀÏ¹ÝÀûÀÎ °æ¿ì ¾ÆÀÌÅÛÀ» ±×Àú ¹ÞÀ» »Ó...
				if (_bAddClientItemList(sOwnerH, pItem, &iEraseReq) == TRUE) {

					// v1.41 Èñ±Í ¾ÆÀÌÅÛÀ» Àü´ÞÇÑ °ÍÀÌ¶ó¸é ·Î±×¸¦ ³²±ä´Ù. 
					_bItemLog(DEF_ITEMLOG_GIVE, iClientH, sOwnerH, pItem);

					// ¾ÆÀÌÅÛÀ» È¹µæÇß´Ù.
					dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
					*dwp = MSGID_NOTIFY;
					wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
					*wp = DEF_NOTIFY_ITEMOBTAINED;

					cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);

					// 1°³ È¹µæÇß´Ù.
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
					iRet = m_pClientList[sOwnerH]->m_pXSock->iSendMsg(cData, 53);
					switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						// ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù.
						DeleteClient(sOwnerH, TRUE, TRUE);
						break;
					}
				}
				else {
					// ¾ÆÀÌÅÛÀ» Àü´Þ¹ÞÀº Ä³¸¯ÅÍ°¡ ´õÀÌ»ó ¾ÆÀÌÅÛÀ» º¸°üÇÒ ¼ö ¾ø´Â »óÅÂÀÌ´Ù.
					// ¾ÆÀÌÅÛÀ» ¼­ÀÖ´Â À§Ä¡¿¡ ¹ö¸°´Ù. 
					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
						m_pClientList[iClientH]->m_sY,
						m_pClientList[iClientH]->m_pItemList[sItemIndex]);
					// v1.41 Èñ±Í ¾ÆÀÌÅÛÀ» ¶³¾î¶ß¸° °ÍÀÌ¶ó¸é ·Î±×¸¦ ³²±ä´Ù. 
					_bItemLog(DEF_ITEMLOG_DROP, iClientH, NULL, m_pClientList[iClientH]->m_pItemList[sItemIndex]);

					// ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¾ÆÀÌÅÛÀÌ ¶³¾îÁø °ÍÀ» ¾Ë¸°´Ù. 
					SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
						m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
						m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
						m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame,
						m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor); // v1.4 color

// ´õÀÌ»ó °¡Áú¼ö ¾ø´Ù´Â ¸Þ½ÃÁö¸¦ º¸³½´Ù.
					dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
					*dwp = MSGID_NOTIFY;
					wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
					*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

					iRet = m_pClientList[sOwnerH]->m_pXSock->iSendMsg(cData, 6);
					switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						// ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù.
						DeleteClient(sOwnerH, TRUE, TRUE);
						break;
					}

					// v1.4 ¾ÆÀÌÅÛ Àü´ÞÀÌ ½ÇÆÐÇßÀ½À» ¾Ë¸®´Â ¹æ¹ý 
					ZeroMemory(cCharName, sizeof(cCharName));
				}
			}
			else {
				// NPC¿¡°Ô ¾ÆÀÌÅÛÀ» ÁÖ¾ú´Ù.
				memcpy(cCharName, m_pNpcList[sOwnerH]->m_cNpcName, 20);

				if (memcmp(m_pNpcList[sOwnerH]->m_cNpcName, "Howard", 6) == 0) {
					// NPC°¡ Ã¢°í ÁÖÀÎÀÌ¾ú´Ù¸é ¹°°ÇÀ» º¸°üÇÏ°Ú´Ù´Â ÀÇ¹ÌÀÌ´Ù. 
					if (bSetItemToBankItem(iClientH, sItemIndex) == FALSE) {
						// ¾ÆÀÌÅÛÀ» º¸°üÇÏ´Âµ¥ ½ÇÆÐÇÏ¿© ¹Ù´Ú¿¡ ¶³¾îÁ³´Ù.
						SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CANNOTITEMTOBANK, NULL, NULL, NULL, NULL);

						// ½ÇÆÐÇßÀ¸¹Ç·Î ¹Ù´Ú¿¡ ¶³±º´Ù.
						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
							m_pClientList[iClientH]->m_sY,
							m_pClientList[iClientH]->m_pItemList[sItemIndex]);

						// v1.41 Èñ±Í ¾ÆÀÌÅÛÀ» ¶³¾î¶ß¸° °ÍÀÌ¶ó¸é ·Î±×¸¦ ³²±ä´Ù. 
						_bItemLog(DEF_ITEMLOG_DROP, iClientH, NULL, m_pClientList[iClientH]->m_pItemList[sItemIndex]);

						// ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¾ÆÀÌÅÛÀÌ ¶³¾îÁø °ÍÀ» ¾Ë¸°´Ù. 
						SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
							m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
							m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
							m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame,
							m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor); // v1.4 color
					}
				}
				else if (memcmp(m_pNpcList[sOwnerH]->m_cNpcName, "Kennedy", 7) == 0) {
					// ±æµå »ç¹«Àå¿¡°Ô ±æµå Å»Åð ½ÅÃ»¼­¸¦ ÁÖ¾ú´Ù¸é ±æµå¸¦ Å»ÅðÇÑ´Ù. 
					if ((m_bIsCrusadeMode == FALSE) && (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 89)) {

						if ((m_pClientList[iClientH]->m_iGuildRank != 0) && (m_pClientList[iClientH]->m_iGuildRank != -1)) {
							// Å»Åð ½ÅÃ»ÀÚ¿¡°Ô Å»Åð ¼º°øÇßÀ½À» ¾Ë¸®´Â ¸Þ½ÃÁö¸¦ º¸³»ÁØ´Ù.
							SendNotifyMsg(iClientH, iClientH, DEF_COMMONTYPE_DISMISSGUILDAPPROVE, NULL, NULL, NULL, NULL);

							// ±æµå Å»Åð. 
							ZeroMemory(m_pClientList[iClientH]->m_cGuildName, sizeof(m_pClientList[iClientH]->m_cGuildName));
							memcpy(m_pClientList[iClientH]->m_cGuildName, "NONE", 4);
							m_pClientList[iClientH]->m_iGuildRank = -1;
							m_pClientList[iClientH]->m_iGuildGUID = -1;

							// Æ¯¼ºÀÌ ¹Ù²î¹Ç·Î ¿Ü¾çÀ» »õ·Î º¸³½´Ù. 
							SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, NULL, NULL, NULL);


						}

						// ÀÌ ¾ÆÀÌÅÛÀº »èÁ¦½ÃÄÑ¾ß ÇÑ´Ù. 
						delete m_pClientList[iClientH]->m_pItemList[sItemIndex];
					}
					else {
						// ±æµå Å»Åð ½ÅÃ»¼­°¡ ¾Æ´Ï¶ó¸é ¹Ù´Ú¿¡ ¶³±º´Ù. Å©·ç¼¼ÀÌµå ¸ðµåÀÏ¶§µµ ±æµå °ü·Ã ¿¬»ê ºÒ°¡ 
						m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
							m_pClientList[iClientH]->m_sY,
							m_pClientList[iClientH]->m_pItemList[sItemIndex]);

						// v1.41 Èñ±Í ¾ÆÀÌÅÛÀ» ¶³¾î¶ß¸° °ÍÀÌ¶ó¸é ·Î±×¸¦ ³²±ä´Ù. 
						_bItemLog(DEF_ITEMLOG_DROP, iClientH, NULL, m_pClientList[iClientH]->m_pItemList[sItemIndex]);

						// ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¾ÆÀÌÅÛÀÌ ¶³¾îÁø °ÍÀ» ¾Ë¸°´Ù. 
						SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
							m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
							m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
							m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame,
							m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor); // v1.4 color

	// v1.4 ¾ÆÀÌÅÛ Àü´ÞÀÌ ½ÇÆÐÇßÀ½À» ¾Ë¸®´Â ¹æ¹ý 
						ZeroMemory(cCharName, sizeof(cCharName));

					}
				}
				else {
					// ÀÏ¹Ý NPC¿¡°Ô ¾ÆÀÌÅÛÀ» ÁÖ¸é ¾ÆÀÌÅÛÀ» ¼­ÀÖ´Â À§Ä¡¿¡ ¹ö·Á¾ß ÇÑ´Ù. 

					m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
						m_pClientList[iClientH]->m_sY,
						m_pClientList[iClientH]->m_pItemList[sItemIndex]);

					// v1.41 Èñ±Í ¾ÆÀÌÅÛÀ» ¶³¾î¶ß¸° °ÍÀÌ¶ó¸é ·Î±×¸¦ ³²±ä´Ù. 
					_bItemLog(DEF_ITEMLOG_DROP, iClientH, NULL, m_pClientList[iClientH]->m_pItemList[sItemIndex]);

					// ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¾ÆÀÌÅÛÀÌ ¶³¾îÁø °ÍÀ» ¾Ë¸°´Ù. 
					SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
						m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
						m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
						m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame,
						m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor); // v1.4 color

// v1.4 ¾ÆÀÌÅÛ Àü´ÞÀÌ ½ÇÆÐÇßÀ½À» ¾Ë¸®´Â ¹æ¹ý 
					ZeroMemory(cCharName, sizeof(cCharName));
				}
			}

			// ÀÌÁ¦ ¾ÆÀÌÅÛÀ» ÁØ º»ÀÎ¿¡°Ô ¾ÆÀÌÅÛÀ» ÁÖ¾úÀ¸¹Ç·Î ¸®½ºÆ®¿¡¼­ »èÁ¦ÇÒ°ÍÀ» Åëº¸ÇÑ´Ù.
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_GIVEITEMFIN_ERASEITEM, sItemIndex, iAmount, NULL, cCharName);
		}

	REMOVE_ITEM_PROCEDURE:;

		// ³×Æ®¿öÅ© ¿À·ù·Î Ã³¸®µµÁß Å¬¶óÀÌ¾ðÆ®°¡ Á¦°ÅµÇ¾ú´Ù¸é ´õÀÌ»ó ÁøÇàÇÒ ¼ö ¾ø´Ù. 
		if (m_pClientList[iClientH] == NULL) return;

		// ¾ÆÀÌÅÛÀ» ÁÖ°Å³ª ¹ö·ÈÀ¸¹Ç·Î Áö¿î´Ù. deleteÇØ¼­´Â ¾ÈµÈ´Ù! 
		m_pClientList[iClientH]->m_pItemList[sItemIndex] = NULL;
		m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] = FALSE;

		// È­»ì ÀÎµ¦½º¸¦ Àç ÇÒ´ç
		m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);
	}

	// ¼ÒÁöÇ° ÃÑ Áß·® Àç °è»ê 
	iCalcTotalWeight(iClientH);
}

DWORD CGame::dwGetItemCount(int iClientH, char* pName)
{
	int i;
	char cTmpName[21];

	if (m_pClientList[iClientH] == NULL) return NULL;

	ZeroMemory(cTmpName, sizeof(cTmpName));
	strcpy(cTmpName, pName);

	for (i = 0; i < DEF_MAXITEMS; i++)
		if ((m_pClientList[iClientH]->m_pItemList[i] != NULL) && (memcmp(m_pClientList[iClientH]->m_pItemList[i]->m_cName, cTmpName, 20) == 0)) {

			return m_pClientList[iClientH]->m_pItemList[i]->m_dwCount;
		}

	return 0;
}

int CGame::SetItemCount(int iClientH, char* pItemName, DWORD dwCount)
{
	int i;
	char cTmpName[21];
	WORD wWeight;

	if (m_pClientList[iClientH] == NULL) return -1;

	ZeroMemory(cTmpName, sizeof(cTmpName));
	strcpy(cTmpName, pItemName);

	for (i = 0; i < DEF_MAXITEMS; i++)
		if ((m_pClientList[iClientH]->m_pItemList[i] != NULL) && (memcmp(m_pClientList[iClientH]->m_pItemList[i]->m_cName, cTmpName, 20) == 0)) {

			wWeight = iGetItemWeight(m_pClientList[iClientH]->m_pItemList[i], 1);

			// Ä«¿îÆ®°¡ 0ÀÌ¸é ¸ðµÎ ¼Ò¸ðµÈ °ÍÀÌ¹Ç·Î ¸®½ºÆ®¿¡¼­ »èÁ¦ÇÑ´Ù.
			if (dwCount == 0) {
				ItemDepleteHandler(iClientH, i, FALSE, TRUE);
			}
			else {
				// ¾ÆÀÌÅÛÀÇ ¼ö·®ÀÌ º¯°æµÇ¾úÀ½À» ¾Ë¸°´Ù. 
				m_pClientList[iClientH]->m_pItemList[i]->m_dwCount = dwCount;
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SETITEMCOUNT, i, dwCount, (char)TRUE, NULL);
			}

			return wWeight;
		}

	return -1;
}


int CGame::SetItemCount(int iClientH, int iItemIndex, DWORD dwCount)
{
	WORD wWeight;

	if (m_pClientList[iClientH] == NULL) return -1;
	if (m_pClientList[iClientH]->m_pItemList[iItemIndex] == NULL) return -1;

	wWeight = iGetItemWeight(m_pClientList[iClientH]->m_pItemList[iItemIndex], 1);

	// Ä«¿îÆ®°¡ 0ÀÌ¸é ¸ðµÎ ¼Ò¸ðµÈ °ÍÀÌ¹Ç·Î ¸®½ºÆ®¿¡¼­ »èÁ¦ÇÑ´Ù.
	if (dwCount == 0) {
		ItemDepleteHandler(iClientH, iItemIndex, FALSE, TRUE);
	}
	else {
		// ¾ÆÀÌÅÛÀÇ ¼ö·®ÀÌ º¯°æµÇ¾úÀ½À» ¾Ë¸°´Ù. 
		m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwCount = dwCount;
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SETITEMCOUNT, iItemIndex, dwCount, (char)TRUE, NULL);
	}

	return wWeight;
}

// New 16/05/2004
void CGame::ReqCreateSlateHandler(int iClientH, char* pData)
{
	int i, iRet;
	short* sp;
	char cItemID[4], ctr[4];
	char* cp, cSlateColour, cData[120];
	BOOL bIsSlatePresent = FALSE;
	CItem* pItem;
	int iSlateType, iEraseReq;
	DWORD* dwp;
	WORD* wp;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsOnServerChange == TRUE) return;

	for (i = 0; i < 4; i++) {
		cItemID[i] = 0;
		ctr[i] = 0;
	}

	cp = (char*)pData;
	cp += 11;

	// 14% chance of creating slates
	if (iDice(1, 100) < m_sSlateSuccessRate) bIsSlatePresent = TRUE;

	try {
		// make sure slates really exist
		for (i = 0; i < 4; i++) {
			cItemID[i] = *cp;
			cp++;

			if (m_pClientList[iClientH]->m_pItemList[cItemID[i]] == NULL || cItemID[i] > DEF_MAXITEMS) {
				bIsSlatePresent = FALSE;
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SLATE_CREATEFAIL, NULL, NULL, NULL, NULL);
				return;
			}

			//No duping
			if (m_pClientList[iClientH]->m_pItemList[cItemID[i]]->m_sIDnum == 868) // LU
				ctr[0] = 1;
			else if (m_pClientList[iClientH]->m_pItemList[cItemID[i]]->m_sIDnum == 869) // LD
				ctr[1] = 1;
			else if (m_pClientList[iClientH]->m_pItemList[cItemID[i]]->m_sIDnum == 870) // RU
				ctr[2] = 1;
			else if (m_pClientList[iClientH]->m_pItemList[cItemID[i]]->m_sIDnum == 871) // RD
				ctr[3] = 1;
		}
	}
	catch (...) {
		//Crash Hacker Caught
		bIsSlatePresent = FALSE;
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SLATE_CREATEFAIL, NULL, NULL, NULL, NULL);
		wsprintf(G_cTxt, "TSearch Slate Hack: (%s) Player: (%s) - creating slates without correct item!", m_pClientList[iClientH]->m_cIPaddress, m_pClientList[iClientH]->m_cCharName);
		PutHackLogFileList(G_cTxt);
		DeleteClient(iClientH, TRUE, TRUE);
		return;
	}

	// Are all 4 slates present ??
	if (ctr[0] != 1 || ctr[1] != 1 || ctr[2] != 1 || ctr[3] != 1) {
		bIsSlatePresent = FALSE;
		return;
	}

	if (m_pClientList[iClientH]->m_iAdminUserLevel > 3) bIsSlatePresent = TRUE;

	// if we failed, kill everything
	if (!bIsSlatePresent) {
		for (i = 0; i < 4; i++) {
			if (m_pClientList[iClientH]->m_pItemList[cItemID[i]] != NULL) {
				ItemDepleteHandler(iClientH, cItemID[i], FALSE, FALSE);
			}
		}
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SLATE_CREATEFAIL, NULL, NULL, NULL, NULL);
		return;
	}

	// make the slates
	for (i = 0; i < 4; i++) {
		if (m_pClientList[iClientH]->m_pItemList[cItemID[i]] != NULL) {
			ItemDepleteHandler(iClientH, cItemID[i], FALSE, FALSE);
		}
	}

	pItem = new class CItem;

	i = iDice(1, 1000);

	if (i < 50) { // Hp slate
		iSlateType = 1;
		cSlateColour = 32;
	}
	else if (i >= 50 && i < 250) { // Bezerk slate
		iSlateType = 2;
		cSlateColour = 3;
	}
	else if (i >= 250 && i < 750) { // Exp slate
		iSlateType = 4;
		cSlateColour = 7;
	}
	else if (i >= 750 && i < 950) { // Mana slate
		iSlateType = 3;
		cSlateColour = 37;
	}
	else if (i >= 950) { // Hp slate
		iSlateType = 1;
		cSlateColour = 32;
	}

	// Notify client
	SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SLATE_CREATESUCCESS, iSlateType, NULL, NULL, NULL);

	ZeroMemory(cData, sizeof(cData));

	// Create slates
	if (_bInitItemAttr(pItem, 867) == FALSE) {
		delete pItem;
		return;
	}
	else {
		pItem->m_sTouchEffectType = DEF_ITET_ID;
		pItem->m_sTouchEffectValue1 = iDice(1, 100000);
		pItem->m_sTouchEffectValue2 = iDice(1, 100000);
		pItem->m_sTouchEffectValue3 = (short)timeGetTime();

		_bItemLog(DEF_ITEMLOG_GET, iClientH, -1, pItem);

		pItem->m_sItemSpecEffectValue2 = iSlateType;
		pItem->m_cItemColor = cSlateColour;
		if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == TRUE) {
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

			*cp = (char)0; // ���� �������̹Ƿ� �������� �ʾҴ�.
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

			if (iEraseReq == 1) delete pItem;

			// ������ ���� ���� 
			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);
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
		else {
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, pItem);
			SendEventToNearClient_TypeB(MSGID_MAGICCONFIGURATIONCONTENTS, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
				m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, pItem->m_sSprite, pItem->m_sSpriteFrame,
				pItem->m_cItemColor);
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
				// �޽����� ������ ������ �߻��ߴٸ� �����Ѵ�.
				DeleteClient(iClientH, TRUE, TRUE);
				break;
			}
		}
	}
}

void CGame::SetSlateFlag(int iClientH, short sType, bool bFlag)
{
	if (m_pClientList[iClientH] == NULL) return;

	if (sType == DEF_NOTIFY_SLATECLEAR) {
		m_pClientList[iClientH]->m_iStatus &= 0xFFBFFFFF;
		m_pClientList[iClientH]->m_iStatus &= 0xFF7FFFFF;
		m_pClientList[iClientH]->m_iStatus &= 0xFFFEFFFF;
		return;
	}

	if (bFlag == TRUE) {
		if (sType == 1) { // Invincible slate
			m_pClientList[iClientH]->m_iStatus |= 0x400000;
		}
		else if (sType == 3) { // Mana slate
			m_pClientList[iClientH]->m_iStatus |= 0x800000;
		}
		else if (sType == 4) { // Exp slate
			m_pClientList[iClientH]->m_iStatus |= 0x10000;
		}
	}
	else {
		if ((m_pClientList[iClientH]->m_iStatus & 0x400000) != 0) {
			m_pClientList[iClientH]->m_iStatus &= 0xFFBFFFFF;
		}
		else if ((m_pClientList[iClientH]->m_iStatus & 0x800000) != 0) {
			m_pClientList[iClientH]->m_iStatus &= 0xFF7FFFFF;
		}
		else if ((m_pClientList[iClientH]->m_iStatus & 0x10000) != 0) {
			m_pClientList[iClientH]->m_iStatus &= 0xFFFEFFFF;
		}
	}

	SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, NULL, NULL, NULL);
}

// v2.16 2002-5-21 °í±¤Çö Ãß°¡
BOOL CGame::bCheckIsItemUpgradeSuccess(int iClientH, int iItemIndex, int iSomH, BOOL bBonus)
{
	int iValue, iProb;

	if (m_pClientList[iClientH]->m_pItemList[iSomH] == NULL) return FALSE;
	iValue = (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x0F0000000) >> 28;

	switch (iValue) {
	case 0: iProb = 30; break;  // +1 :90%     +1~+2
	case 1: iProb = 25; break;  // +2 :80%      +3
	case 2: iProb = 20; break;  // +3 :48%      +4 
	case 3: iProb = 15; break;  // +4 :24%      +5
	case 4: iProb = 10; break;  // +5 :9.6%     +6
	case 5: iProb = 10; break;  // +6 :2.8%     +7
	case 6: iProb = 8; break;  // +7 :0.57%    +8
	case 7: iProb = 8; break;  // +8 :0.05%    +9
	case 8: iProb = 5; break;  // +9 :0.004%   +10
	case 9: iProb = 3; break;  // +10:0.00016%
	default: iProb = 1; break;
	}

	if (((m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_dwAttribute & 0x00000001) != NULL) && (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 > 100)) {
		if (iProb > 20)
			iProb += (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 / 10);
		else if (iProb > 7)
			iProb += (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 / 20);
		else
			iProb += (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sItemSpecEffectValue2 / 40);
	}
	if (bBonus == TRUE) iProb *= 2;

	iProb *= 100;


	if (iProb >= iDice(1, 10000)) {
		_bItemLog(DEF_ITEMLOG_UPGRADESUCCESS, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);
		return TRUE;
	}

	_bItemLog(DEF_ITEMLOG_UPGRADEFAIL, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[iItemIndex]);

	return FALSE;
}

/*********************************************************************************************************************
**  BOOL CGame::_bDecodeItemConfigFileContents(char * pData, DWORD dwMsgSize)										**
**  DESCRIPTION			:: decodes Item.cfg file																	**
**  LAST_UPDATED		:: March 17, 2005; 2:09 PM; Hypnotoad														**
**	RETURN_VALUE		:: BOOL																						**
**  NOTES				::	n/a																						**
**	MODIFICATION		::	n/a																						**
**********************************************************************************************************************/
BOOL CGame::_bDecodeItemConfigFileContents(char* pData, DWORD dwMsgSize)
{
	char* pContents, * token, cTxt[120];
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int  iItemConfigListIndex, iTemp;
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
					// m_sIDnum
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemIDnumber");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					iItemConfigListIndex = atoi(token);
					if (m_pItemConfigList[iItemConfigListIndex] != NULL) {
						wsprintf(cTxt, "(!!!) CRITICAL ERROR! Duplicate ItemIDnum(%d)", iItemConfigListIndex);
						PutLogList(cTxt);
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex] = new class CItem;
					m_pItemConfigList[iItemConfigListIndex]->m_sIDnum = iItemConfigListIndex;
					cReadModeB = 2;
					break;
				case 2:
					// m_cName 
					ZeroMemory(m_pItemConfigList[iItemConfigListIndex]->m_cName, sizeof(m_pItemConfigList[iItemConfigListIndex]->m_cName));
					memcpy(m_pItemConfigList[iItemConfigListIndex]->m_cName, token, strlen(token));
					cReadModeB = 3;
					break;
				case 3:
					// m_cItemType
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemType");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_cItemType = atoi(token);
					cReadModeB = 4;
					break;
				case 4:
					// m_cEquipPos
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - EquipPos");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_cEquipPos = atoi(token);
					cReadModeB = 5;
					break;
				case 5:
					// m_sItemEffectType
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectType");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectType = atoi(token);
					cReadModeB = 6;
					break;
				case 6:
					// m_sItemEffectValue1
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectValue1");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectValue1 = atoi(token);
					cReadModeB = 7;
					break;
				case 7:
					// m_sItemEffectValue2
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectValue2");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectValue2 = atoi(token);
					cReadModeB = 8;
					break;
				case 8:
					// m_sItemEffectValue3
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectValue3");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectValue3 = atoi(token);
					cReadModeB = 9;
					break;
				case 9:
					// m_sItemEffectValue4
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectValue4");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectValue4 = atoi(token);
					cReadModeB = 10;
					break;
				case 10:
					// m_sItemEffectValue5
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectValue5");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectValue5 = atoi(token);
					cReadModeB = 11;
					break;
				case 11:
					// m_sItemEffectValue6
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - ItemEffectValue6");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_sItemEffectValue6 = atoi(token);
					cReadModeB = 12;
					break;
				case 12:
					// m_wMaxLifeSpan
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - MaxLifeSpan");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_wMaxLifeSpan = (WORD)atoi(token);
					cReadModeB = 13;
					break;
				case 13:
					// m_sSpecialEffect
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - MaxFixCount");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_sSpecialEffect = atoi(token);
					cReadModeB = 14;
					break;
				case 14:
					// m_sSprite
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - Sprite");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_sSprite = atoi(token);
					cReadModeB = 15;
					break;
				case 15:
					// m_sSpriteFrame
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - SpriteFrame");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_sSpriteFrame = atoi(token);
					cReadModeB = 16;
					break;
				case 16:
					// m_wPrice
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - Price");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					iTemp = atoi(token);
					if (iTemp < 0)
						m_pItemConfigList[iItemConfigListIndex]->m_bIsForSale = FALSE;
					else m_pItemConfigList[iItemConfigListIndex]->m_bIsForSale = TRUE;

					m_pItemConfigList[iItemConfigListIndex]->m_wPrice = abs(iTemp);
					cReadModeB = 17;
					break;
				case 17:
					// m_wWeight
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - Weight");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_wWeight = atoi(token);
					cReadModeB = 18;
					break;
				case 18:
					// Appr Value
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - ApprValue");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_cApprValue = atoi(token);
					cReadModeB = 19;
					break;
				case 19:
					// m_cSpeed
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - Speed");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_cSpeed = atoi(token);
					cReadModeB = 20;
					break;

				case 20:
					// m_sLevelLimit
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - LevelLimit");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_sLevelLimit = atoi(token);
					cReadModeB = 21;
					break;

				case 21:
					// m_cGederLimit
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - GenderLimit");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_cGenderLimit = atoi(token);
					cReadModeB = 22;
					break;

				case 22:
					// m_sSpecialEffectValue1
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - SM_HitRatio");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_sSpecialEffectValue1 = atoi(token);
					cReadModeB = 23;
					break;

				case 23:
					// m_sSpecialEffectValue2
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - L_HitRatio");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_sSpecialEffectValue2 = atoi(token);
					cReadModeB = 24;
					break;

				case 24:
					// m_sRelatedSkill
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - RelatedSkill");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_sRelatedSkill = atoi(token);
					cReadModeB = 25;
					break;

				case 25:
					// m_cCategory
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - Category");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_cCategory = atoi(token);
					cReadModeB = 26;
					break;

				case 26:
					// m_cItemColor
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file error - Category");
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pItemConfigList[iItemConfigListIndex]->m_cItemColor = atoi(token);
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
			if (memcmp(token, "Item", 4) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
			}
			if (memcmp(token, "[ENDITEMLIST]", 13) == 0) {
				cReadModeA = 0;
				cReadModeB = 0;
				break;
			}
		}
		token = pStrTok->pGet();
	}

	delete pStrTok;
	delete[] pContents;
	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) CRITICAL ERROR! ITEM configuration file contents error!");
		return FALSE;
	}
	wsprintf(cTxt, "(!) ITEM(Total:%d) configuration - success!", iItemConfigListIndex);
	PutLogList(cTxt);
	m_bReceivedItemList = TRUE;
	return TRUE;
}

/*********************************************************************************************************************
**  BOOL CGame::_bInitItemAttr(class CItem * pItem, char * pItemName)												**
**  DESCRIPTION			:: initializes item variables - by ItemName													**
**  LAST_UPDATED		:: March 17, 2005; 2:10 PM; Hypnotoad														**
**	RETURN_VALUE		:: BOOL																						**
**  NOTES				::	- overloaded function - see other _bInitItemAttr										**
**	MODIFICATION		::	n/a																						**
**********************************************************************************************************************/
BOOL CGame::_bInitItemAttr(class CItem* pItem, char* pItemName)
{
	int i;
	char cTmpName[22];

	ZeroMemory(cTmpName, sizeof(cTmpName));
	strcpy(cTmpName, pItemName);

	for (i = 0; i < DEF_MAXITEMTYPES; i++)
		if (m_pItemConfigList[i] != NULL) {
			if (memcmp(cTmpName, m_pItemConfigList[i]->m_cName, 20) == 0) {
				ZeroMemory(pItem->m_cName, sizeof(pItem->m_cName));
				strcpy(pItem->m_cName, m_pItemConfigList[i]->m_cName);
				pItem->m_cItemType = m_pItemConfigList[i]->m_cItemType;
				pItem->m_cEquipPos = m_pItemConfigList[i]->m_cEquipPos;
				pItem->m_sItemEffectType = m_pItemConfigList[i]->m_sItemEffectType;
				pItem->m_sItemEffectValue1 = m_pItemConfigList[i]->m_sItemEffectValue1;
				pItem->m_sItemEffectValue2 = m_pItemConfigList[i]->m_sItemEffectValue2;
				pItem->m_sItemEffectValue3 = m_pItemConfigList[i]->m_sItemEffectValue3;
				pItem->m_sItemEffectValue4 = m_pItemConfigList[i]->m_sItemEffectValue4;
				pItem->m_sItemEffectValue5 = m_pItemConfigList[i]->m_sItemEffectValue5;
				pItem->m_sItemEffectValue6 = m_pItemConfigList[i]->m_sItemEffectValue6;
				pItem->m_wMaxLifeSpan = m_pItemConfigList[i]->m_wMaxLifeSpan;
				pItem->m_wCurLifeSpan = pItem->m_wMaxLifeSpan;
				pItem->m_sSpecialEffect = m_pItemConfigList[i]->m_sSpecialEffect;
				pItem->m_sSprite = m_pItemConfigList[i]->m_sSprite;
				pItem->m_sSpriteFrame = m_pItemConfigList[i]->m_sSpriteFrame;
				pItem->m_wPrice = m_pItemConfigList[i]->m_wPrice;
				pItem->m_wWeight = m_pItemConfigList[i]->m_wWeight;
				pItem->m_cApprValue = m_pItemConfigList[i]->m_cApprValue;
				pItem->m_cSpeed = m_pItemConfigList[i]->m_cSpeed;
				pItem->m_sLevelLimit = m_pItemConfigList[i]->m_sLevelLimit;
				pItem->m_cGenderLimit = m_pItemConfigList[i]->m_cGenderLimit;
				pItem->m_sSpecialEffectValue1 = m_pItemConfigList[i]->m_sSpecialEffectValue1;
				pItem->m_sSpecialEffectValue2 = m_pItemConfigList[i]->m_sSpecialEffectValue2;
				pItem->m_sRelatedSkill = m_pItemConfigList[i]->m_sRelatedSkill;
				pItem->m_cCategory = m_pItemConfigList[i]->m_cCategory;
				pItem->m_sIDnum = m_pItemConfigList[i]->m_sIDnum;
				pItem->m_bIsForSale = m_pItemConfigList[i]->m_bIsForSale;
				pItem->m_cItemColor = m_pItemConfigList[i]->m_cItemColor;
				return TRUE;
			}
		}
	return FALSE;
}

/*********************************************************************************************************************
**  BOOL CGame::_bInitItemAttr(class CItem * pItem, char * pItemName)												**
**  DESCRIPTION			:: initializes item variables - by ItemID													**
**  LAST_UPDATED		:: March 17, 2005; 2:10 PM; Hypnotoad														**
**	RETURN_VALUE		:: BOOL																						**
**  NOTES				::	- overloaded function - see other _bInitItemAttr										**
**	MODIFICATION		::	n/a																						**
**********************************************************************************************************************/
BOOL CGame::_bInitItemAttr(class CItem* pItem, int iItemID)
{
	int i;

	for (i = 0; i < DEF_MAXITEMTYPES; i++)
		if (m_pItemConfigList[i] != NULL) {
			if (m_pItemConfigList[i]->m_sIDnum == iItemID) {
				ZeroMemory(pItem->m_cName, sizeof(pItem->m_cName));
				strcpy(pItem->m_cName, m_pItemConfigList[i]->m_cName);
				pItem->m_cItemType = m_pItemConfigList[i]->m_cItemType;
				pItem->m_cEquipPos = m_pItemConfigList[i]->m_cEquipPos;
				pItem->m_sItemEffectType = m_pItemConfigList[i]->m_sItemEffectType;
				pItem->m_sItemEffectValue1 = m_pItemConfigList[i]->m_sItemEffectValue1;
				pItem->m_sItemEffectValue2 = m_pItemConfigList[i]->m_sItemEffectValue2;
				pItem->m_sItemEffectValue3 = m_pItemConfigList[i]->m_sItemEffectValue3;
				pItem->m_sItemEffectValue4 = m_pItemConfigList[i]->m_sItemEffectValue4;
				pItem->m_sItemEffectValue5 = m_pItemConfigList[i]->m_sItemEffectValue5;
				pItem->m_sItemEffectValue6 = m_pItemConfigList[i]->m_sItemEffectValue6;
				pItem->m_wMaxLifeSpan = m_pItemConfigList[i]->m_wMaxLifeSpan;
				pItem->m_wCurLifeSpan = pItem->m_wMaxLifeSpan;
				pItem->m_sSpecialEffect = m_pItemConfigList[i]->m_sSpecialEffect;
				pItem->m_sSprite = m_pItemConfigList[i]->m_sSprite;
				pItem->m_sSpriteFrame = m_pItemConfigList[i]->m_sSpriteFrame;
				pItem->m_wPrice = m_pItemConfigList[i]->m_wPrice;
				pItem->m_wWeight = m_pItemConfigList[i]->m_wWeight;
				pItem->m_cApprValue = m_pItemConfigList[i]->m_cApprValue;
				pItem->m_cSpeed = m_pItemConfigList[i]->m_cSpeed;
				pItem->m_sLevelLimit = m_pItemConfigList[i]->m_sLevelLimit;
				pItem->m_cGenderLimit = m_pItemConfigList[i]->m_cGenderLimit;
				pItem->m_sSpecialEffectValue1 = m_pItemConfigList[i]->m_sSpecialEffectValue1;
				pItem->m_sSpecialEffectValue2 = m_pItemConfigList[i]->m_sSpecialEffectValue2;
				pItem->m_sRelatedSkill = m_pItemConfigList[i]->m_sRelatedSkill;
				pItem->m_cCategory = m_pItemConfigList[i]->m_cCategory;
				pItem->m_sIDnum = m_pItemConfigList[i]->m_sIDnum;
				pItem->m_bIsForSale = m_pItemConfigList[i]->m_bIsForSale;
				pItem->m_cItemColor = m_pItemConfigList[i]->m_cItemColor;
				return TRUE;
			}
		}
	return FALSE;
}

int CGame::_iGetItemSpaceLeft(int iClientH)
{
	int i, iTotalItem;

	iTotalItem = 0;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemList[i] != NULL) iTotalItem++;

	return (DEF_MAXITEMS - iTotalItem);
}

BOOL CGame::bAddItem(int iClientH, CItem* pItem, char cMode)
{
	char* cp, cData[256];
	DWORD* dwp;
	WORD* wp;
	short* sp;
	int iRet, iEraseReq;


	ZeroMemory(cData, sizeof(cData));
	if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == TRUE) {
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


		if (iEraseReq == 1) {
			delete pItem;
			pItem = NULL;
		}

		// ¾ÆÀÌÅÛ Á¤º¸ Àü¼Û 
		iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);

		return TRUE;
	}
	else {
		// ¾ÆÀÌÅÛÀ» Àü´Þ¹ÞÀº Ä³¸¯ÅÍ°¡ ´õÀÌ»ó ¾ÆÀÌÅÛÀ» º¸°üÇÒ ¼ö ¾ø´Â »óÅÂÀÌ´Ù.
		// ¾ÆÀÌÅÛÀ» ¼­ÀÖ´Â À§Ä¡¿¡ ¹ö¸°´Ù. 
		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
			m_pClientList[iClientH]->m_sY,
			pItem);

		// ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¾ÆÀÌÅÛÀÌ ¶³¾îÁø °ÍÀ» ¾Ë¸°´Ù. 
		SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
			m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
			pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); //v1.4 color

// ´õÀÌ»ó °¡Áú¼ö ¾ø´Ù´Â ¸Þ½ÃÁö¸¦ º¸³½´Ù.
		dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

		iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);

		return TRUE;
	}

	return FALSE;
}

void CGame::SendItemNotifyMsg(int iClientH, WORD wMsgType, CItem* pItem, int iV1)
{
	char* cp, cData[512];
	DWORD* dwp;
	WORD* wp;
	short* sp;
	int     iRet;

	if (m_pClientList[iClientH] == NULL) return;

	dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_NOTIFY;
	wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
	*wp = wMsgType;
	cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);

	switch (wMsgType) {
	case DEF_NOTIFY_ITEMOBTAINED:
		*cp = 1; // 1°³ È¹µæÇß´Ù. <- ¿©±â¼­ 1°³¶õ ¼ö·® Ä«¿îÆ®¸¦ ¸»ÇÏ´Â °ÍÀÌ ¾Æ´Ï´Ù
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

		*cp = pItem->m_cItemColor; // v1.4
		cp++;

		*cp = (char)pItem->m_sItemSpecEffectValue2; // v1.41 
		cp++;

		dwp = (DWORD*)cp;
		*dwp = pItem->m_dwAttribute;
		cp += 4;


		iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);
		break;

	case DEF_NOTIFY_ITEMPURCHASED:
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

		wp = (WORD*)cp;
		*wp = iV1;

		iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 48);
		break;

	case DEF_NOTIFY_CANNOTCARRYMOREITEM:
		iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
		break;
	}
}

BOOL CGame::_bCheckItemReceiveCondition(int iClientH, CItem* pItem)
{
	int i;

	if (m_pClientList[iClientH] == NULL) return FALSE;


	if (m_pClientList[iClientH]->m_iCurWeightLoad + iGetItemWeight(pItem, pItem->m_dwCount) > (DWORD)_iCalcMaxLoad(iClientH))
		return FALSE;

	// ¾ÆÀÌÅÛÀ» ¹ÞÀ» ¿©À¯°ø°£ À¯¹« ÆÇ´Ü.
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemList[i] == NULL) return TRUE;

	return FALSE;
}

// New 07/05/2004
void CGame::DropItemHandler(int iClientH, short sItemIndex, int iAmount, char* pItemName, BOOL bByPlayer)
{
	class CItem* pItem;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsOnServerChange == TRUE) return;
	if ((m_bAdminSecurity == TRUE) && (m_pClientList[iClientH]->m_iAdminUserLevel > 0)) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == NULL) return;
	if ((iAmount != -1) && (iAmount < 0)) return;

	// Amount°¡ -1ÀÌ°í ¼Ò¸ðÇ°ÀÌ¸é ¼ö·®ÀÇ ÀüºÎ¸¦ ¶³¾î¶ß¸°´Ù.
	if (((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
		(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW)) &&
		(iAmount == -1))
		iAmount = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount;


	// ¾ÆÀÌÅÛ ÀÌ¸§ÀÌ ÀÏÄ¡ÇÏÁö ¾Ê¾Æµµ ¹«½Ã 
	if (memcmp(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName, pItemName, 20) != 0) return;

	if (((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
		(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW)) &&
		(((int)m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount - iAmount) > 0)) {
		// ¼Òºñ¼º ¾ÆÀÌÅÛÀÌ¾ú°í ¼ö·®¸¸Å­ °¨¼Ò½ÃÅ°°í ³²Àº °Ô ÀÖ´Ù¸é 
		pItem = new class CItem;
		if (_bInitItemAttr(pItem, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cName) == FALSE) {
			// ºÐÇÒÇÏ°íÀÚ ÇÏ´Â ¾ÆÀÌÅÛÀÌ ¸®½ºÆ®¿¡ ¾ø´Â °Å´Ù. ÀÌ·± ÀÏÀº ÀÏ¾î³¯ ¼ö°¡ ¾øÁö¸¸ 
			delete pItem;
			return;
		}
		else {
			if (iAmount <= 0) {
				// ¾ÆÀÌÅÛÀÇ ¼ö·®ÀÌ 0º¸´Ù ÀÛÀ¸¸é ¿¡·¯»óÈ². ¸®ÅÏ 
				delete pItem;
				return;
			}
			pItem->m_dwCount = (DWORD)iAmount;
		}

		// ¼ö·® °¨¼Ò 

		// ¿¡·¯. °¨¼Ò½ÃÅ°°íÀÚ ÇÏ´Â ¾çÀÌ ´õ ¸¹´Ù. 
		if ((DWORD)iAmount > m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount) {
			delete pItem;
			return;
		}

		m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount -= iAmount;

		// º¯°æµÈ ¼ö·®À» ¼³Á¤ÇÏ°í ¾Ë¸°´Ù.
		// v1.41 !!!
		SetItemCount(iClientH, sItemIndex, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwCount);

		// ¾ÆÀÌÅÛÀ» ¼­ÀÖ´Â À§Ä¡¿¡ ¹ö¸°´Ù. 
		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
			m_pClientList[iClientH]->m_sY, pItem);

		// v1.411 Èñ±Í ¾ÆÀÌÅÛÀÌ ¶³¾îÁø °ÍÀÎÁö Ã¼Å©  
		// v2.17 2002-7-31 ÇÃ·¹ÀÌ¾î°¡ Á×¾î¼­ ¶³¾îÁø°Ç ¸ðµç ·Î±×°¡ ³²´Â´Ù. 
		if (bByPlayer == TRUE)
			_bItemLog(DEF_ITEMLOG_DROP, iClientH, (int)-1, pItem);
		else
			_bItemLog(DEF_ITEMLOG_DROP, iClientH, (int)-1, pItem, TRUE);

		// ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¾ÆÀÌÅÛÀÌ ¶³¾îÁø °ÍÀ» ¾Ë¸°´Ù. 
		SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
			m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
			pItem->m_sSprite, pItem->m_sSpriteFrame, pItem->m_cItemColor); // v1.4 color

		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_DROPITEMFIN_COUNTCHANGED, sItemIndex, iAmount, NULL, NULL);
	}
	else {
		// ÀÏ¹Ý ¾ÆÀÌÅÛÀÌ³ª ¼Òºñ¼º ¾ÆÀÌÅÛÀ» ¸ðµÎ ¹ö·È´Ù.

		// ¸ÕÀú ÀåÂøµÇ¾î ÀÖ´Ù¸é ÇØÁ¦½ÃÅ²´Ù.

		ReleaseItemHandler(iClientH, sItemIndex, TRUE);

		// v2.17 ¾ÆÀÌÅÛÀÌ ÀåÂøµÇ¾î ÀÖÀ¸¸é ÇØÁ¦ÇÑ´Ù.
		if (m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] == TRUE)
			SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, NULL, NULL);

		// v1.432
		if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ALTERITEMDROP) &&
			(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan <= 0)) {
			// Èñ»ýÀÇ µ¹ÀÎ °æ¿ì ¼ö¸íÀÌ 0ÀÎ »óÅÂ·Î ¶³¾îÁö¸é »ç¶óÁø´Ù.
			delete m_pClientList[iClientH]->m_pItemList[sItemIndex];
			m_pClientList[iClientH]->m_pItemList[sItemIndex] = NULL;
		}
		else {
			// ¾ÆÀÌÅÛÀ» ¼­ÀÖ´Â À§Ä¡¿¡ ¹ö¸°´Ù. 
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(m_pClientList[iClientH]->m_sX,
				m_pClientList[iClientH]->m_sY,
				m_pClientList[iClientH]->m_pItemList[sItemIndex]);

			// v1.41 Èñ±Í ¾ÆÀÌÅÛÀ» ¶³¾î¶ß¸° °ÍÀÌ¶ó¸é ·Î±×¸¦ ³²±ä´Ù. 
			// v2.17 2002-7-31 ÇÃ·¹ÀÌ¾î°¡ Á×¾î¼­ ¶³¾îÁø°Ç ¸ðµç ·Î±×°¡ ³²´Â´Ù. 
			if (bByPlayer == TRUE)
				_bItemLog(DEF_ITEMLOG_DROP, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[sItemIndex]);
			else
				_bItemLog(DEF_ITEMLOG_DROP, iClientH, (int)-1, m_pClientList[iClientH]->m_pItemList[sItemIndex], TRUE);

			// ´Ù¸¥ Å¬¶óÀÌ¾ðÆ®¿¡°Ô ¾ÆÀÌÅÛÀÌ ¶³¾îÁø °ÍÀ» ¾Ë¸°´Ù. 
			SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_ITEMDROP, m_pClientList[iClientH]->m_cMapIndex,
				m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
				m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSprite,
				m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpriteFrame,
				m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor); //v1.4 color
		}

		// ¾ÆÀÌÅÛÀ» deleteÇÏÁö ¾Ê°í NULL·Î ÇÒ´çÇÑ´Ù. delete ÇÏÁö ¾Ê´Â ÀÌÀ¯´Â ¹Ù´Ú¿¡ ¶³¾îÁ® ÀÖ±â ¶§¹® 
		m_pClientList[iClientH]->m_pItemList[sItemIndex] = NULL;
		m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] = FALSE;

		// ÀÌÁ¦ ¾ÆÀÌÅÛÀ» ¸®½ºÆ®¿¡¼­ »èÁ¦ÇÒ°ÍÀ» Åëº¸ÇÑ´Ù.
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_DROPITEMFIN_ERASEITEM, sItemIndex, iAmount, NULL, NULL);

		// ÀÎµ¦½º°¡ ¹Ù²î¾úÀ¸¹Ç·Î ÀçÇÒ´ç
		m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);
	}

	// ¼ÒÁöÇ° ÃÑ Áß·® Àç °è»ê 
	iCalcTotalWeight(iClientH);
}

/////////////////////////////////////////////////////////////////////////////////////
//  int CGame::iClientMotion_GetItem_Handler(int iClientH, short sX, short sY, char cDir)
//  description			:: check if player is dropping item or picking up item
//  last updated		:: October 29, 2004; 7:12 PM; Hypnotoad
//	return value		:: int
/////////////////////////////////////////////////////////////////////////////////////
int CGame::iClientMotion_GetItem_Handler(int iClientH, short sX, short sY, char cDir)
{
	DWORD* dwp;
	WORD* wp;
	char* cp;
	short* sp, sRemainItemSprite, sRemainItemSpriteFrame;
	char  cRemainItemColor, cData[100];
	int   iRet, iEraseReq;
	class CItem* pItem;

	if (m_pClientList[iClientH] == NULL) return 0;
	if ((cDir <= 0) || (cDir > 8))       return 0;
	if (m_pClientList[iClientH]->m_bIsKilled == TRUE) return 0;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return 0;

	if ((sX != m_pClientList[iClientH]->m_sX) || (sY != m_pClientList[iClientH]->m_sY)) return 2;

	int iStX, iStY;
	if (m_pMapList[m_pClientList[iClientH]->m_cMapIndex] != NULL) {
		iStX = m_pClientList[iClientH]->m_sX / 20;
		iStY = m_pClientList[iClientH]->m_sY / 20;
		m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iPlayerActivity++;

		switch (m_pClientList[iClientH]->m_cSide) {
		case 0: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iNeutralActivity++; break;
		case 1: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iAresdenActivity++; break;
		case 2: m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_stTempSectorInfo[iStX][iStY].iElvineActivity++;  break;
		}
	}

	ClearSkillUsingStatus(iClientH);

	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->ClearOwner(0, iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);
	m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->SetOwner(iClientH, DEF_OWNERTYPE_PLAYER, sX, sY);

	pItem = m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->pGetItem(sX, sY, &sRemainItemSprite, &sRemainItemSpriteFrame, &cRemainItemColor);
	if (pItem != NULL) {
		if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == TRUE) {

			_bItemLog(DEF_ITEMLOG_GET, iClientH, NULL, pItem);

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

			if (iEraseReq == 1) delete pItem;

			SendEventToNearClient_TypeB(MSGID_EVENT_COMMON, DEF_COMMONTYPE_SETITEM, m_pClientList[iClientH]->m_cMapIndex,
				m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY,
				sRemainItemSprite, sRemainItemSpriteFrame, cRemainItemColor);

			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);
			switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				DeleteClient(iClientH, TRUE, TRUE);
				return 0;
			}
		}
		else
		{
			m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->bSetItem(sX, sY, pItem);

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
				return 0;
			}
		}
	}

	dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
	*dwp = MSGID_RESPONSE_MOTION;
	wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
	*wp = DEF_OBJECTMOTION_CONFIRM;

	iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 6);
	switch (iRet) {
	case DEF_XSOCKEVENT_QUENEFULL:
	case DEF_XSOCKEVENT_SOCKETERROR:
	case DEF_XSOCKEVENT_CRITICALERROR:
	case DEF_XSOCKEVENT_SOCKETCLOSED:
		DeleteClient(iClientH, TRUE, TRUE);
		return 0;
	}

	return 1;
}

BOOL CGame::_bAddClientItemList(int iClientH, class CItem* pItem, int* pDelReq)
{
	int i;

	if (m_pClientList[iClientH] == NULL) return FALSE;
	if (pItem == NULL) return FALSE;

	// ÁýÀ» ¾ÆÀÌÅÛ Áß·®°è»ê  
	if ((pItem->m_cItemType == DEF_ITEMTYPE_CONSUME) || (pItem->m_cItemType == DEF_ITEMTYPE_ARROW)) {
		// ¼ö·®°³³äÀÌ ÀÖ´Â ¾ÆÀÌÅÛ 
		if ((m_pClientList[iClientH]->m_iCurWeightLoad + iGetItemWeight(pItem, pItem->m_dwCount)) > (DWORD)_iCalcMaxLoad(iClientH))
			return FALSE;
	}
	else {
		// ¼ö·® °³³äÀÌ ¾ø´Â ¾ÆÀÌÅÛ 
		if ((m_pClientList[iClientH]->m_iCurWeightLoad + iGetItemWeight(pItem, 1)) > (DWORD)_iCalcMaxLoad(iClientH))
			return FALSE;
	}

	// ¼Òºñ¼º ¾ÆÀÌÅÛÀ» ÀÌ¹Ì ¼ÒÁöÇÏ°í ÀÖ´Ù¸é ¼ö·®¸¸ Áõ°¡½ÃÅ²´Ù. 
	if ((pItem->m_cItemType == DEF_ITEMTYPE_CONSUME) || (pItem->m_cItemType == DEF_ITEMTYPE_ARROW)) {
		for (i = 0; i < DEF_MAXITEMS; i++)
			if ((m_pClientList[iClientH]->m_pItemList[i] != NULL) &&
				(memcmp(m_pClientList[iClientH]->m_pItemList[i]->m_cName, pItem->m_cName, 20) == 0)) {
				// °°Àº ÀÌ¸§À» Ã£¾Ò´Ù. 
				m_pClientList[iClientH]->m_pItemList[i]->m_dwCount += pItem->m_dwCount;

				*pDelReq = 1;

				// ¼ÒÁöÇ° ÃÑ Áß·® Àç °è»ê 
				iCalcTotalWeight(iClientH);

				return TRUE;
			}
	}

	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemList[i] == NULL) {

			m_pClientList[iClientH]->m_pItemList[i] = pItem;
			// v1.3 ¾ÆÀÌÅÛÀÇ À§Ä¡´Â ¹«Á¶°Ç 100, 100
			m_pClientList[iClientH]->m_ItemPosList[i].x = 40;
			m_pClientList[iClientH]->m_ItemPosList[i].y = 30;

			*pDelReq = 0;

			// ¸¸¾à È­»ì·ù ¾ÆÀÌÅÛÀÌ¶ó¸é È­»ìÀ» ÇÒ´çÇÑ´Ù. 
			if (pItem->m_cItemType == DEF_ITEMTYPE_ARROW)
				m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);

			// ¼ÒÁöÇ° ÃÑ Áß·® Àç °è»ê 
			iCalcTotalWeight(iClientH);

			return TRUE;
		}

	// ´õÀÌ»ó ¾ÆÀÌÅÛÀ» ÁýÀ» °ø°£ÀÌ ¾ø´Ù.
	return FALSE;
}

BOOL CGame::bEquipItemHandler(int iClientH, short sItemIndex, BOOL bNotify)
{
	char  cEquipPos, cHeroArmorType;
	short   sSpeed;
	short sTemp;
	int iTemp;

	if (m_pClientList[iClientH] == NULL) return FALSE;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return FALSE;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == NULL) return FALSE;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType != DEF_ITEMTYPE_EQUIP) return FALSE;

	if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_wCurLifeSpan <= 0) return FALSE;

	if (((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_dwAttribute & 0x00000001) == NULL) &&
		(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sLevelLimit > m_pClientList[iClientH]->m_iLevel)) return FALSE;


	if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cGenderLimit != 0) {
		switch (m_pClientList[iClientH]->m_sType) {
		case 1:
		case 2:
		case 3:
			// ³²¼ºÀÌ´Ù.
			if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cGenderLimit != 1) return FALSE;
			break;
		case 4:
		case 5:
		case 6:
			// ¿©¼ºÀÌ´Ù.
			if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cGenderLimit != 2) return FALSE;
			break;
		}
	}

	if (iGetItemWeight(m_pClientList[iClientH]->m_pItemList[sItemIndex], 1) > (m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iAngelicStr) * 100) return FALSE;

	cEquipPos = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos;

	if ((cEquipPos == DEF_EQUIPPOS_BODY) || (cEquipPos == DEF_EQUIPPOS_LEGGINGS) ||
		(cEquipPos == DEF_EQUIPPOS_ARMS) || (cEquipPos == DEF_EQUIPPOS_HEAD)) {
		switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue4) {
		case 10: // Str Á¦ÇÑ 
			if ((m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iAngelicStr) < m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5) {
				// Å¬¶óÀÌ¾ðÆ® »ó¿¡¼­´Â Âø¿ëµÈ »óÅÂÀÌ¹Ç·Î ¹þ°Ü¾ß ÇÑ´Ù. Âø¿ëÀÌ ÇØÁ¦µÈ´Ù.
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, NULL, NULL);
				// ÇØ´ç Âø¿ë ºÎÀ§ÀÇ ¾ÆÀÌÅÛÈ¿°ú¸¦ Á¦°Å.
				ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos], TRUE);
				return FALSE;
			}
			break;
		case 11: // Dex
			if ((m_pClientList[iClientH]->m_iDex + m_pClientList[iClientH]->m_iAngelicDex) < m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5) {
				// Å¬¶óÀÌ¾ðÆ® »ó¿¡¼­´Â Âø¿ëµÈ »óÅÂÀÌ¹Ç·Î ¹þ°Ü¾ß ÇÑ´Ù. Âø¿ëÀÌ ÇØÁ¦µÈ´Ù.
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, NULL, NULL);
				// ÇØ´ç Âø¿ë ºÎÀ§ÀÇ ¾ÆÀÌÅÛÈ¿°ú¸¦ Á¦°Å.
				ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos], TRUE);
				return FALSE;
			}
			break;
		case 12: // Vit
			if (m_pClientList[iClientH]->m_iVit < m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5) {
				// Å¬¶óÀÌ¾ðÆ® »ó¿¡¼­´Â Âø¿ëµÈ »óÅÂÀÌ¹Ç·Î ¹þ°Ü¾ß ÇÑ´Ù. Âø¿ëÀÌ ÇØÁ¦µÈ´Ù.
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, NULL, NULL);
				// ÇØ´ç Âø¿ë ºÎÀ§ÀÇ ¾ÆÀÌÅÛÈ¿°ú¸¦ Á¦°Å.
				ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos], TRUE);
				return FALSE;
			}
			break;
		case 13: // Int
			if ((m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iAngelicInt) < m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5) {
				// Å¬¶óÀÌ¾ðÆ® »ó¿¡¼­´Â Âø¿ëµÈ »óÅÂÀÌ¹Ç·Î ¹þ°Ü¾ß ÇÑ´Ù. Âø¿ëÀÌ ÇØÁ¦µÈ´Ù.
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, NULL, NULL);
				// ÇØ´ç Âø¿ë ºÎÀ§ÀÇ ¾ÆÀÌÅÛÈ¿°ú¸¦ Á¦°Å.
				ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos], TRUE);
				return FALSE;
			}
			break;
		case 14: // Mag
			if ((m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iAngelicMag) < m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5) {
				// Å¬¶óÀÌ¾ðÆ® »ó¿¡¼­´Â Âø¿ëµÈ »óÅÂÀÌ¹Ç·Î ¹þ°Ü¾ß ÇÑ´Ù. Âø¿ëÀÌ ÇØÁ¦µÈ´Ù.
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, NULL, NULL);
				// ÇØ´ç Âø¿ë ºÎÀ§ÀÇ ¾ÆÀÌÅÛÈ¿°ú¸¦ Á¦°Å.
				ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos], TRUE);
				return FALSE;
			}
			break;
		case 15: // Chr
			if (m_pClientList[iClientH]->m_iCharisma < m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectValue5) {
				// Å¬¶óÀÌ¾ðÆ® »ó¿¡¼­´Â Âø¿ëµÈ »óÅÂÀÌ¹Ç·Î ¹þ°Ü¾ß ÇÑ´Ù. Âø¿ëÀÌ ÇØÁ¦µÈ´Ù.
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos, sItemIndex, NULL, NULL);
				// ÇØ´ç Âø¿ë ºÎÀ§ÀÇ ¾ÆÀÌÅÛÈ¿°ú¸¦ Á¦°Å.
				ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos], TRUE);
				return FALSE;
			}
			break;
		}
	}

	if (cEquipPos == DEF_EQUIPPOS_TWOHAND) {
		// Stormbringer
		if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 845) {
			if ((m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iAngelicInt) < 65) {
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_iSpecialAbilityEquipPos, sItemIndex, NULL, NULL);
				ReleaseItemHandler(iClientH, sItemIndex, TRUE);
				return FALSE;
			}
			else
			{
				m_pClientList[iClientH]->m_iHitRatio += 10;
			}
		}
		// Centuu : Fixed las armas Blood by KaoZureS
		else if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 490) { // Sword
			if (m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iAngelicStr < 131) {
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_iSpecialAbilityEquipPos, sItemIndex, NULL, NULL);
				ReleaseItemHandler(iClientH, sItemIndex, TRUE);
				return FALSE;
			}
			else
			{
				m_pClientList[iClientH]->m_iHitRatio += 10;
			}
		}
	}

	if (cEquipPos == DEF_EQUIPPOS_RHAND) 
	{
		// Resurrection wand(MS.10) or Resurrection wand(MS.20)
		if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 865) || (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 866)) {
			if (((m_pClientList[iClientH]->m_iInt + m_pClientList[iClientH]->m_iAngelicInt) > 99) && ((m_pClientList[iClientH]->m_iMag + m_pClientList[iClientH]->m_iAngelicMag) > 99) && (m_pClientList[iClientH]->m_iSpecialAbilityTime < 1)) {
				m_pClientList[iClientH]->m_cMagicMastery[94] = TRUE;
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_RESUR_ON, NULL, NULL, NULL, NULL);
			}
		}
		// Centuu : Fixed las armas Blood by KaoZureS
		else if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 491) { // Axe
			if (m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iAngelicStr < 61) {
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_iSpecialAbilityEquipPos, sItemIndex, NULL, NULL);
				ReleaseItemHandler(iClientH, sItemIndex, TRUE);
				return FALSE;
			}
			else
			{
				m_pClientList[iClientH]->m_iHitRatio += 10;
			}
		}
		// Centuu : Fixed las armas Blood by KaoZureS
		else if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 492) { // Rapier
			if (m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iAngelicStr < 11) {
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_iSpecialAbilityEquipPos, sItemIndex, NULL, NULL);
				ReleaseItemHandler(iClientH, sItemIndex, TRUE);
				return FALSE;
			}
			else
			{
				m_pClientList[iClientH]->m_iHitRatio += 10;
			}
		}
	}

	if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY) ||
		(m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY)) {

		if ((m_pClientList[iClientH]->m_iSpecialAbilityType != 0)) {
			// ÀÌ¹Ì Æ¯¼ö´É·ÂÀÌ ¼³Á¤µÇ¾î ÀÖ´Ù. ÀÌÀü¿¡ ÀåÂøµÇ¾î ÀÖ´Â ¾ÆÀÌÅÛ Âø¿ë ÇØÁ¦:
			// ´Ü, Âø¿ë ºÎÀ§°¡ °°´Ù¸é ÇØÁ¦ ¸Þ½ÃÁö º¸³»Áö ¾Ê´Â´Ù.
			if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos != m_pClientList[iClientH]->m_iSpecialAbilityEquipPos) {
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iClientH]->m_iSpecialAbilityEquipPos, m_pClientList[iClientH]->m_sItemEquipmentStatus[m_pClientList[iClientH]->m_iSpecialAbilityEquipPos], NULL, NULL);
				// ÇØ´ç Âø¿ë ºÎÀ§ÀÇ ¾ÆÀÌÅÛÈ¿°ú¸¦ Á¦°Å.
				ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[m_pClientList[iClientH]->m_iSpecialAbilityEquipPos], TRUE);
			}
		}
	}


	if (cEquipPos == DEF_EQUIPPOS_NONE) return FALSE;

	if (cEquipPos == DEF_EQUIPPOS_TWOHAND) {
		// ¾ç¼ÕÀ» »ç¿ëÇÏ´Â ¹«±â°¡ ÀåÂøµÇ¾î ÀÖ´Ù¸é  
		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos] != -1)
			ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos], FALSE);
		else {
			// ¿À¸¥¼Õ, ¿Þ¼Õ¿¡ ¾ÆÀÌÅÛÀÌ ÀÖ´Ù¸é ¸ðµÎ ÀåÂø ÇØÁ¦ 
			if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND] != -1)
				ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RHAND], FALSE);
			if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LHAND] != -1)
				ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LHAND], FALSE);
		}
	}
	else {
		// ¾ç¼Õ ¾ÆÀÌÅÛÀÌ ÀåÂøµÇ¾î ÀÖ´Ù¸é ÇØÁ¦½ÃÅ²´Ù.
		if ((cEquipPos == DEF_EQUIPPOS_LHAND) || (cEquipPos == DEF_EQUIPPOS_RHAND)) {
			// ¹«±â³ª ¹æÆÐ·ù¶ó¸é ¾ç¼Õ¹«±â¸¦ ÇØÁ¦ 
			if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND] != -1)
				ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_TWOHAND], FALSE);
		}

		// ÀåÂøÀ§Ä¡¿¡ ¾ÆÀÌÅÛÀÌ ÀÌ¹Ì ÀÖÀ¸¸é »èÁ¦.
		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos] != -1)
			ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos], FALSE);
	}


	if (cEquipPos == DEF_EQUIPPOS_RELEASEALL) {
		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos] != -1) {
			ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos], FALSE);
		}
		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_HEAD] != -1) {
			ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_HEAD], FALSE);
		}
		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY] != -1) {
			ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY], FALSE);
		}
		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS] != -1) {
			ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS], FALSE);
		}
		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LEGGINGS] != -1) {
			ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LEGGINGS], FALSE);
		}
		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS] != -1) {
			ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS], FALSE);
		}
		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BACK] != -1) {
			ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BACK], FALSE);
		}
	}
	else {
		if (cEquipPos == DEF_EQUIPPOS_HEAD || cEquipPos == DEF_EQUIPPOS_BODY || cEquipPos == DEF_EQUIPPOS_ARMS ||
			cEquipPos == DEF_EQUIPPOS_LEGGINGS || cEquipPos == DEF_EQUIPPOS_PANTS || cEquipPos == DEF_EQUIPPOS_BACK) {
			if (m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RELEASEALL] != -1) {
				ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_RELEASEALL], FALSE);
			}
		}
		if (m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos] != -1)
			ReleaseItemHandler(iClientH, m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos], FALSE);
	}


	m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos] = sItemIndex;
	m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] = TRUE;

	switch (cEquipPos) {

	case DEF_EQUIPPOS_HEAD:
		sTemp = m_pClientList[iClientH]->m_sAppr3;
		sTemp = sTemp & 0xFF0F;	// Åõ±¸ Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.
		sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 4); // Appr °ªÀ» ¼¼ÆÃ. 
		m_pClientList[iClientH]->m_sAppr3 = sTemp;

		iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 ÄÃ·¯°ª ¼¼ÆÃ 
		iTemp = iTemp & 0xFFFFFFF0; // ºÎÃ÷ »ö ºñÆ®¸¦ Å¬¸®¾î.				
		iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor));
		m_pClientList[iClientH]->m_iApprColor = iTemp;
		break;

	case DEF_EQUIPPOS_PANTS:
		sTemp = m_pClientList[iClientH]->m_sAppr3;
		sTemp = sTemp & 0xF0FF;	// ¹ÙÁö Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.
		sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 8); // Appr °ªÀ» ¼¼ÆÃ. 
		m_pClientList[iClientH]->m_sAppr3 = sTemp;

		iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 ÄÃ·¯°ª ¼¼ÆÃ 
		iTemp = iTemp & 0xFFFFF0FF; // ¹ÙÁö »ö ºñÆ®¸¦ Å¬¸®¾î.
		iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 8);
		m_pClientList[iClientH]->m_iApprColor = iTemp;
		break;

	case DEF_EQUIPPOS_LEGGINGS:
		sTemp = m_pClientList[iClientH]->m_sAppr4;
		sTemp = sTemp & 0x0FFF;	// ºÎÃ÷ Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.
		sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 12); // Appr °ªÀ» ¼¼ÆÃ. 
		m_pClientList[iClientH]->m_sAppr4 = sTemp;

		iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 ÄÃ·¯°ª ¼¼ÆÃ 
		iTemp = iTemp & 0xFFFFFF0F; // ºÎÃ÷ »ö ºñÆ®¸¦ Å¬¸®¾î.
		iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 4);
		m_pClientList[iClientH]->m_iApprColor = iTemp;
		break;

	case DEF_EQUIPPOS_BODY:
		sTemp = m_pClientList[iClientH]->m_sAppr3;
		sTemp = sTemp & 0x0FFF;	// °Ñ¿Ê Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.

		if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue < 100) {
			// ¿ÜÇü°ªÀÌ 100 ÀÌÇÏ. Á¤»óÀûÀÎ °ª 
			sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 12); // Appr °ªÀ» ¼¼ÆÃ. 
			m_pClientList[iClientH]->m_sAppr3 = sTemp;
		}
		else {
			// ¿ÜÇü°ªÀÌ 100 ÀÌ»óÀÌ¸é È®Àå ÇÃ·¡±×¸¦ »ç¿ëÇÑ´Ù. 
			sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue - 100) << 12); // Appr °ªÀ» ¼¼ÆÃ. 
			m_pClientList[iClientH]->m_sAppr3 = sTemp;
			// °Ñ¿Ê È®Àå Æ¯¼ºÄ¡ ºñÆ®¸¦ ¼¼Æ®.
			sTemp = m_pClientList[iClientH]->m_sAppr4;
			sTemp = sTemp | 0x080;
			m_pClientList[iClientH]->m_sAppr4 = sTemp;
		}

		iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 ÄÃ·¯°ª ¼¼ÆÃ 
		iTemp = iTemp & 0xFF0FFFFF; // °Ñ¿Ê(°©¿Ê) »ö ºñÆ®¸¦ Å¬¸®¾î.
		iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 20);
		m_pClientList[iClientH]->m_iApprColor = iTemp;
		break;

	case DEF_EQUIPPOS_ARMS:
		sTemp = m_pClientList[iClientH]->m_sAppr3;
		sTemp = sTemp & 0xFFF0;	// ¼Ó¿Ê Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.
		sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue)); // Appr °ªÀ» ¼¼ÆÃ. 
		m_pClientList[iClientH]->m_sAppr3 = sTemp;

		iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 ÄÃ·¯°ª ¼¼ÆÃ 
		iTemp = iTemp & 0xFFFFFFFF; // ¼Ó¿Ê »ö ºñÆ®¸¦ Å¬¸®¾î. // New 09/05/2004
		iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 12);
		m_pClientList[iClientH]->m_iApprColor = iTemp;
		break;

	case DEF_EQUIPPOS_LHAND:
		// ¿Þ¼Õ¿¡ ÀåÂøÇÏ´Â ¾ÆÀÌÅÛ. ¹æÆÐ·ùÀÌ´Ù. 
		sTemp = m_pClientList[iClientH]->m_sAppr2;
		sTemp = sTemp & 0xFFF0;	// ¹«±â Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.
		sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue)); // Appr °ªÀ» ¼¼ÆÃ. 
		m_pClientList[iClientH]->m_sAppr2 = sTemp;

		iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 ÄÃ·¯°ª ¼¼ÆÃ 
		iTemp = iTemp & 0xF0FFFFFF; // ¹æÆÐ »ö ºñÆ®¸¦ Å¬¸®¾î.
		iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 24);
		m_pClientList[iClientH]->m_iApprColor = iTemp;
		break;

	case DEF_EQUIPPOS_RHAND:
		// ¿À¸¥¼Õ¿¡ ÀåÂøÇÏ´Â ¾ÆÀÌÅÛ. ¹«±â·ùÀÌ´Ù. 
		sTemp = m_pClientList[iClientH]->m_sAppr2;
		sTemp = sTemp & 0xF00F;	// ¹«±â Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.
		sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 4); // Appr °ªÀ» ¼¼ÆÃ. 
		m_pClientList[iClientH]->m_sAppr2 = sTemp;

		iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 ÄÃ·¯°ª ¼¼ÆÃ 
		iTemp = iTemp & 0xFFFFFFF; // ¹«±â »ö ºñÆ®¸¦ Å¬¸®¾î.
		iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 28);
		m_pClientList[iClientH]->m_iApprColor = iTemp;

		// ¹«±â ¼Óµµ¸¦ ³ªÅ¸³»´Â StatusÀÇ ºñÆ®¸¦ ¼³Á¤ÇÑ´Ù.
		iTemp = m_pClientList[iClientH]->m_iStatus;
		iTemp = iTemp & 0xFFFFFFF0;
		sSpeed = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cSpeed);
		// Str¿¡ µû¶ó ¹«±â ¼Óµµ¸¦ ÁÙÀÎ´Ù. 
		sSpeed -= ((m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iAngelicStr) / 13);
		if (sSpeed < 0) sSpeed = 0;
		iTemp = iTemp | (int)sSpeed;
		m_pClientList[iClientH]->m_iStatus = iTemp;
		m_pClientList[iClientH]->m_iComboAttackCount = 0;
		break;

	case DEF_EQUIPPOS_TWOHAND:
		// ¾ç¼ÕÅ¸ÀÔÀÌÁö¸¸ »ç½Ç ¿À¸¥¼Õ¿¡ ÀåÂøµÈ´Ù. 
		sTemp = m_pClientList[iClientH]->m_sAppr2;
		sTemp = sTemp & 0xF00F;	// ¹«±â Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.
		sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 4); // Appr °ªÀ» ¼¼ÆÃ. 
		m_pClientList[iClientH]->m_sAppr2 = sTemp;

		iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 ÄÃ·¯°ª ¼¼ÆÃ 
		iTemp = iTemp & 0xFFFFFFF; // ¹«±â »ö ºñÆ®¸¦ Å¬¸®¾î.
		iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 28);
		m_pClientList[iClientH]->m_iApprColor = iTemp;

		// ¹«±â ¼Óµµ¸¦ ³ªÅ¸³»´Â StatusÀÇ ºñÆ®¸¦ ¼³Á¤ÇÑ´Ù.
		iTemp = m_pClientList[iClientH]->m_iStatus;
		iTemp = iTemp & 0xFFFFFFF0;
		sSpeed = (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cSpeed);
		// Str¿¡ µû¶ó ¹«±â ¼Óµµ¸¦ ÁÙÀÎ´Ù. 
		sSpeed -= ((m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iAngelicStr) / 13);
		if (sSpeed < 0) sSpeed = 0;
		iTemp = iTemp | (int)sSpeed;
		m_pClientList[iClientH]->m_iStatus = iTemp;
		m_pClientList[iClientH]->m_iComboAttackCount = 0;
		break;

	case DEF_EQUIPPOS_BACK:
		sTemp = m_pClientList[iClientH]->m_sAppr4;
		sTemp = sTemp & 0xF0FF;	// ¸ÁÅä Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.
		sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 8); // Appr °ªÀ» ¼¼ÆÃ. 
		m_pClientList[iClientH]->m_sAppr4 = sTemp;

		iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 ÄÃ·¯°ª ¼¼ÆÃ 
		iTemp = iTemp & 0xFFF0FFFF; // ¸ÁÅä »ö ºñÆ®¸¦ Å¬¸®¾î.
		iTemp = iTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemColor) << 16);
		m_pClientList[iClientH]->m_iApprColor = iTemp;
		break;

	case DEF_EQUIPPOS_RELEASEALL:
		sTemp = m_pClientList[iClientH]->m_sAppr3;
		sTemp = sTemp & 0x0FFF;	// ¸ÁÅä Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.
		sTemp = sTemp | ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cApprValue) << 12); // Appr °ªÀ» ¼¼ÆÃ. 
		m_pClientList[iClientH]->m_sAppr3 = sTemp;

		iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 ÄÃ·¯°ª ¼¼ÆÃ 
		iTemp = iTemp & 0xFFF0FFFF; // ¸ÁÅä »ö ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_iApprColor = iTemp;
		break;
	}

	if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY) {
		// °ø°Ý Æ¯¼ö ´É·Â ºñÆ® Å¬¸®¾î
		m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 & 0xFFF3;
		switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffect) {
		case 0: break;
		case 1: // °ø°Ý½Ã 50% HP °¨¼Ò
			m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 | 0x0004;
			break;

		case 2: // ³Ãµ¿ È¿°ú 
			m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 | 0x000C;
			break;

		case 3: // ¸¶ºñ È¿°ú 
			m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 | 0x0008;
			break;
		}
	}

	if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY) {
		m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 & 0xFFFC;
		switch (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sSpecialEffect) {
		case 0:
			break;
		case 50:
		case 51:
		case 52:
			m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 | 0x0002;
			break;
		default:
			if (m_pClientList[iClientH]->m_iAdminUserLevel > 0) m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 | 0x0001;

			break;
		}
	}

	cHeroArmorType = _cCheckHeroItemEquipped(iClientH);
	if (cHeroArmorType != 0x0FFFFFFFF) m_pClientList[iClientH]->m_cHeroArmourBonus = cHeroArmorType;

	SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, NULL, NULL, NULL);
	CalcTotalItemEffect(iClientH, sItemIndex, bNotify);
	return TRUE;

}

void CGame::ReleaseItemHandler(int iClientH, short sItemIndex, BOOL bNotice)
{
	char cEquipPos, cHeroArmorType;
	short  sTemp;
	int   iTemp;

	if (m_pClientList[iClientH] == NULL) return;
	if ((sItemIndex < 0) || (sItemIndex >= DEF_MAXITEMS)) return;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex] == NULL) return;
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cItemType != DEF_ITEMTYPE_EQUIP) return;

	if (m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] == FALSE) return;

	cHeroArmorType = _cCheckHeroItemEquipped(iClientH);
	if (cHeroArmorType != 0x0FFFFFFFF) m_pClientList[iClientH]->m_cHeroArmourBonus = 0;

	cEquipPos = m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_cEquipPos;
	if (cEquipPos == DEF_EQUIPPOS_RHAND) {
		if (m_pClientList[iClientH]->m_pItemList[sItemIndex] != NULL) {
			if ((m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 865) || (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sIDnum == 866)) {
				m_pClientList[iClientH]->m_cMagicMastery[94] = FALSE;
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_RESUR_OFF, NULL, NULL, NULL, NULL);
			}
		}
	}
	// ¾ÆÀÌÅÛÀÇ ÇØÁ¦À§Ä¡¿¡ ¸Â°Ô Apprº¯¼ö¸¦ Á¶Á¤ÇÑ´Ù.
	switch (cEquipPos) {
	case DEF_EQUIPPOS_RHAND:
		// ¿À¸¥¼Õ¿¡ ÀåÂøÇÏ´Â ¾ÆÀÌÅÛ. ¹«±â·ùÀÌ´Ù. 
		sTemp = m_pClientList[iClientH]->m_sAppr2;
		sTemp = sTemp & 0xF00F;	// ¹«±â Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_sAppr2 = sTemp;

		iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 ÄÃ·¯°ª ¼¼ÆÃ 
		iTemp = iTemp & 0x0FFFFFFF; // ¹«±â »ö ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_iApprColor = iTemp;

		// V1.3 ¹«±â¼Óµµ Àç°è»ê <- ¸Ç¼ÕÀÌ´Ï±ñ 0
		iTemp = m_pClientList[iClientH]->m_iStatus;
		iTemp = iTemp & 0xFFFFFFF0;
		m_pClientList[iClientH]->m_iStatus = iTemp;
		break;

	case DEF_EQUIPPOS_LHAND:
		// ¿Þ¼Õ¿¡ ÀåÂøÇÏ´Â ¾ÆÀÌÅÛ. ¹æÆÐ·ùÀÌ´Ù. 
		sTemp = m_pClientList[iClientH]->m_sAppr2;
		sTemp = sTemp & 0xFFF0;	// ¹«±â Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_sAppr2 = sTemp;

		iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 ÄÃ·¯°ª ¼¼ÆÃ 
		iTemp = iTemp & 0xF0FFFFFF; // »ö ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_iApprColor = iTemp;
		break;

	case DEF_EQUIPPOS_TWOHAND:
		// ¾ç¼Õ¿¡ ÀåÂøÇÑ´ÙÁö¸¸ »ç½Ç ¿À¸¥¼Õ¿¡¸¸ µé°í ÀÖ´Ù. ¿Þ¼ÕÀº ºñ¾îÀÖ´Â »óÅÂ.
		sTemp = m_pClientList[iClientH]->m_sAppr2;
		sTemp = sTemp & 0xF00F;	// ¹«±â Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_sAppr2 = sTemp;

		iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 ÄÃ·¯°ª ¼¼ÆÃ 
		iTemp = iTemp & 0x0FFFFFFF; // ¹«±â »ö ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_iApprColor = iTemp;
		break;

	case DEF_EQUIPPOS_BODY:
		sTemp = m_pClientList[iClientH]->m_sAppr3;
		sTemp = sTemp & 0x0FFF;	// °Ñ¿Ê Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_sAppr3 = sTemp;

		// °Ñ¿Ê È®Àå Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.
		sTemp = m_pClientList[iClientH]->m_sAppr4;
		sTemp = sTemp & 0xFF7F;
		m_pClientList[iClientH]->m_sAppr4 = sTemp;

		iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 ÄÃ·¯°ª ¼¼ÆÃ 
		iTemp = iTemp & 0xFF0FFFFF; // »ö ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_iApprColor = iTemp;
		break;

	case DEF_EQUIPPOS_BACK:
		sTemp = m_pClientList[iClientH]->m_sAppr4;
		sTemp = sTemp & 0xF0FF;	// ¸ÁÅä Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_sAppr4 = sTemp;

		iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 ÄÃ·¯°ª ¼¼ÆÃ 
		iTemp = iTemp & 0xFFF0FFFF; // »ö ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_iApprColor = iTemp;
		break;

	case DEF_EQUIPPOS_ARMS:
		sTemp = m_pClientList[iClientH]->m_sAppr3;
		sTemp = sTemp & 0xFFF0;	// °Ñ¿Ê Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_sAppr3 = sTemp;

		iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 ÄÃ·¯°ª ¼¼ÆÃ 
		iTemp = iTemp & 0xFFFF0FFF; // »ö ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_iApprColor = iTemp;
		break;

	case DEF_EQUIPPOS_PANTS:
		sTemp = m_pClientList[iClientH]->m_sAppr3;
		sTemp = sTemp & 0xF0FF;	// °Ñ¿Ê Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_sAppr3 = sTemp;

		iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 ÄÃ·¯°ª ¼¼ÆÃ 
		iTemp = iTemp & 0xFFFFF0FF; // »ö ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_iApprColor = iTemp;
		break;

	case DEF_EQUIPPOS_LEGGINGS:
		sTemp = m_pClientList[iClientH]->m_sAppr4;
		sTemp = sTemp & 0x0FFF;	// °Ñ¿Ê Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_sAppr4 = sTemp;

		iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 ÄÃ·¯°ª ¼¼ÆÃ 
		iTemp = iTemp & 0xFFFFFF0F; // »ö ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_iApprColor = iTemp;
		break;

	case DEF_EQUIPPOS_HEAD:
		sTemp = m_pClientList[iClientH]->m_sAppr3;
		sTemp = sTemp & 0xFF0F;	// Åõ±¸ Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_sAppr3 = sTemp;

		iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 ÄÃ·¯°ª ¼¼ÆÃ 
		iTemp = iTemp & 0xFFFFFFF0; // »ö ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_iApprColor = iTemp;
		break;

	case DEF_EQUIPPOS_RELEASEALL:
		sTemp = m_pClientList[iClientH]->m_sAppr3;
		sTemp = sTemp & 0x0FFF;	// ¸ÁÅä Æ¯¼ºÄ¡ ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_sAppr3 = sTemp;

		iTemp = m_pClientList[iClientH]->m_iApprColor; // v1.4 ÄÃ·¯°ª ¼¼ÆÃ 
		iTemp = iTemp & 0xFFF0FFFF; // ¸ÁÅä »ö ºñÆ®¸¦ Å¬¸®¾î.
		m_pClientList[iClientH]->m_iApprColor = iTemp;
		break;
	}

	//v1.432 Æ¯¼ö ´É·ÂÀÌ ºÎ¿©µÈ ¾ÆÀÌÅÛÀÌ¶ó¸é ÇÃ·¡±× ¼³Á¤ 
	if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY) {
		// °ø°Ý Æ¯¼ö ´É·Â ºñÆ® Å¬¸®¾î
		m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 & 0xFFF3;
	}

	if (m_pClientList[iClientH]->m_pItemList[sItemIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY) {
		// ¹æ¾î±¸ Æ¯¼ö ´É·Â ºñÆ® Å¬¸®¾î
		m_pClientList[iClientH]->m_sAppr4 = m_pClientList[iClientH]->m_sAppr4 & 0xFFFC;
	}

	m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] = FALSE;
	m_pClientList[iClientH]->m_sItemEquipmentStatus[cEquipPos] = -1;

	// ¿ÜÇüÀÌ ¹Ù²ï°ÍÀ» ¾Ë¸°´Ù.
	if (bNotice == TRUE)
		SendEventToNearClient_TypeA(iClientH, DEF_OWNERTYPE_PLAYER, MSGID_EVENT_MOTION, DEF_OBJECTNULLACTION, NULL, NULL, NULL);

	// ÀÌ ¾ÆÀÌÅÛÀ» ÇØÁ¦ÇÏ´Â °Í¿¡ µû¸£´Â Æ¯¼ºÄ¡ÀÇ º¯È­¸¦ °è»êÇÑ´Ù. 
	CalcTotalItemEffect(iClientH, sItemIndex, TRUE);
}

BOOL CGame::bPlayerItemToBank(int iClientH, short sItemIndex)
{
	int i, iIndex;

	if (m_pClientList[iClientH] == NULL) return FALSE;

	for (i = 0; i < DEF_MAXBANKITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemInBankList[i] == NULL) {
			iIndex = i;
			goto NEXT_STEP_PLTB;
		}
	// ´õÀÌ»ó ÀúÀåÇÒ °ø°£ÀÌ ¾ø´Ù. 
	return FALSE;

NEXT_STEP_PLTB:;

	// ¾ÆÀÌÅÛÀ» ÀúÀåÇÒ °ø°£ÀÌ ³²¾ÆÀÖ´Ù. 
	// ¸ÕÀú ÀåÂøµÇ¾î ÀÖ´Ù¸é ÇØÁ¦½ÃÅ²´Ù.
	ReleaseItemHandler(iClientH, sItemIndex, TRUE);

	// ¾ÆÀÌÅÛ Å¬·¡½ºÀÇ ÁÖ¼Ò¸¦ ¹Ù²Û´Ù. 
	m_pClientList[iClientH]->m_pItemInBankList[iIndex] = m_pClientList[iClientH]->m_pItemList[sItemIndex];
	// ÇÃ·¹ÀÌ¾î ¾ÆÀÌÅÛ ¸®½ºÆ®¸¦ Å¬¸®¾îÇÏ°í 
	m_pClientList[iClientH]->m_pItemList[sItemIndex] = NULL;
	m_pClientList[iClientH]->m_bIsItemEquipped[sItemIndex] = FALSE;

	// ¾ÆÀÌÅÛ ¸®½ºÆ®ÀÇ ºó °ø°£À» »èÁ¦ÇÑ´Ù.
	for (i = 1; i < DEF_MAXITEMS; i++)
		if ((m_pClientList[iClientH]->m_pItemList[i - 1] == NULL) && (m_pClientList[iClientH]->m_pItemList[i] != NULL)) {
			m_pClientList[iClientH]->m_pItemList[i - 1] = m_pClientList[iClientH]->m_pItemList[i];
			m_pClientList[iClientH]->m_bIsItemEquipped[i - 1] = m_pClientList[iClientH]->m_bIsItemEquipped[i];
			m_pClientList[iClientH]->m_pItemList[i] = NULL;
			m_pClientList[iClientH]->m_bIsItemEquipped[i] = FALSE;
		}

	return TRUE;
}

BOOL CGame::bBankItemToPlayer(int iClientH, short sItemIndex)
{
	int i, iIndex;

	if (m_pClientList[iClientH] == NULL) return FALSE;

	if (m_pClientList[iClientH]->m_pItemInBankList[sItemIndex] == NULL) return FALSE;

	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pClientList[iClientH]->m_pItemList[i] == NULL) {
			iIndex = i;
			goto NEXT_STEP_PLTB;
		}
	// ´õÀÌ»ó °®°íÀÖÀ» °ø°£ÀÌ ¾ø´Ù. 
	return FALSE;

NEXT_STEP_PLTB:;

	// ¾ÆÀÌÅÛÀ» ¼ÒÁöÇÒ °ø°£ÀÌ ³²¾ÆÀÖ´Ù. 

	// ¾ÆÀÌÅÛ Å¬·¡½ºÀÇ ÁÖ¼Ò¸¦ ¹Ù²Û´Ù. 
	m_pClientList[iClientH]->m_pItemList[iIndex] = m_pClientList[iClientH]->m_pItemInBankList[sItemIndex];

	m_pClientList[iClientH]->m_pItemInBankList[sItemIndex] = NULL;

	return TRUE;
}

void CGame::RequestRetrieveItemHandler(int iClientH, char* pData)
{
	char* cp, cBankItemIndex, cMsg[100];
	int i, j, iRet, iItemWeight;
	DWORD* dwp;
	WORD* wp;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;

	cp = (char*)(pData + DEF_INDEX2_MSGTYPE + 2);
	cBankItemIndex = *cp;

	if (m_pClientList[iClientH]->m_bIsInsideWarehouse == FALSE) return;

	if ((cBankItemIndex < 0) || (cBankItemIndex >= DEF_MAXBANKITEMS)) return;
	if (m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex] == NULL) {
		// ¿À·ù´Ù. 
		ZeroMemory(cMsg, sizeof(cMsg));

		dwp = (DWORD*)(cMsg + DEF_INDEX4_MSGID);
		*dwp = MSGID_RESPONSE_RETRIEVEITEM;
		wp = (WORD*)(cMsg + DEF_INDEX2_MSGTYPE);
		*wp = DEF_MSGTYPE_REJECT;

		iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cMsg, 8);
	}
	else {
		// Áß·®°è»ê 
		iItemWeight = iGetItemWeight(m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex], m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex]->m_dwCount);

		if ((iItemWeight + m_pClientList[iClientH]->m_iCurWeightLoad) > _iCalcMaxLoad(iClientH)) {
			// ÇÑ°èÁß·® ÃÊ°ú, ¾ÆÀÌÅÛÀ» Ã£À» ¼ö ¾ø´Ù. 
			// ½ÇÆÐ ¸Þ½ÃÁö¸¦ º¸³½´Ù.
			ZeroMemory(cMsg, sizeof(cMsg));

			// ´õÀÌ»ó °¡Áú¼ö ¾ø´Ù´Â ¸Þ½ÃÁö¸¦ º¸³½´Ù.
			dwp = (DWORD*)(cMsg + DEF_INDEX4_MSGID);
			*dwp = MSGID_NOTIFY;
			wp = (WORD*)(cMsg + DEF_INDEX2_MSGTYPE);
			*wp = DEF_NOTIFY_CANNOTCARRYMOREITEM;

			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cMsg, 6);
			switch (iRet) {
			case DEF_XSOCKEVENT_QUENEFULL:
			case DEF_XSOCKEVENT_SOCKETERROR:
			case DEF_XSOCKEVENT_CRITICALERROR:
			case DEF_XSOCKEVENT_SOCKETCLOSED:
				// ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù.
				DeleteClient(iClientH, TRUE, TRUE);
				break;
			}
			return;
		}

		//!!!
		if ((m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
			(m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW)) {
			// Áßº¹ÀÌ °¡´ÉÇÑ ¾ÆÀÌÅÛÀÌ¶ó¸é ¼ö·®¸¸ Áõ°¡½ÃÅ²´Ù.	
			for (i = 0; i < DEF_MAXITEMS; i++)
				if ((m_pClientList[iClientH]->m_pItemList[i] != NULL) &&
					(m_pClientList[iClientH]->m_pItemList[i]->m_cItemType == m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex]->m_cItemType) &&
					(memcmp(m_pClientList[iClientH]->m_pItemList[i]->m_cName, m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex]->m_cName, 20) == 0)) {
					// °°Àº Çü½ÄÀÇ ¾ÆÀÌÅÛÀ» Ã£¾Ò´Ù. ¼ö·®À» Áõ°¡½ÃÅ²´Ù.
					// v1.41 !!! 
					SetItemCount(iClientH, i, m_pClientList[iClientH]->m_pItemList[i]->m_dwCount + m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex]->m_dwCount);

					// ¹ðÅ© ¾ÆÀÌÅÛ »èÁ¦ 
					delete m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex];
					m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex] = NULL;

					// ºó °ø°£À» ¾ø¾Ø´Ù. 
					for (j = 0; j <= DEF_MAXBANKITEMS - 2; j++) {
						if ((m_pClientList[iClientH]->m_pItemInBankList[j + 1] != NULL) && (m_pClientList[iClientH]->m_pItemInBankList[j] == NULL)) {
							m_pClientList[iClientH]->m_pItemInBankList[j] = m_pClientList[iClientH]->m_pItemInBankList[j + 1];

							m_pClientList[iClientH]->m_pItemInBankList[j + 1] = NULL;
						}
					}

					// ¼º°ø ¸Þ½ÃÁö¸¦ º¸³½´Ù.
					ZeroMemory(cMsg, sizeof(cMsg));

					dwp = (DWORD*)(cMsg + DEF_INDEX4_MSGID);
					*dwp = MSGID_RESPONSE_RETRIEVEITEM;
					wp = (WORD*)(cMsg + DEF_INDEX2_MSGTYPE);
					*wp = DEF_MSGTYPE_CONFIRM;

					cp = (char*)(cMsg + DEF_INDEX2_MSGTYPE + 2);
					*cp = cBankItemIndex;
					cp++;
					*cp = i;
					cp++;

					// ¼ÒÁöÇ° ÃÑ Áß·® Àç °è»ê 
					iCalcTotalWeight(iClientH);
					// È­»ì ÇÒ´ç
					m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);

					// ¸Þ½ÃÁö Àü¼Û 
					iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cMsg, 8);
					switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						// ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù.
						DeleteClient(iClientH, TRUE, TRUE);
						break;
					}
					return;
				}

			// °°Àº ÀÌ¸§À» °®°í ÀÖ´Â ¾ÆÀÌÅÛÀÌ ¾ø´Ù. »õ·Î Ãß°¡ÇØ¾ß ÇÑ´Ù. 
			goto RRIH_NOQUANTITY;
		}
		else {
		RRIH_NOQUANTITY:;
			// ¼ö·®°³³äÀÌ ¾ø´Â ¾ÆÀÌÅÛ 
			for (i = 0; i < DEF_MAXITEMS; i++)
				if (m_pClientList[iClientH]->m_pItemList[i] == NULL) {
					// ºó °ø°£À» Ã£¾Ò´Ù. 
					// ¸ÕÀú ÁÖ¼Ò¸¦ ¿Å±ä´Ù. 
					m_pClientList[iClientH]->m_pItemList[i] = m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex];
					// v1.3 1-27 12:22
					m_pClientList[iClientH]->m_ItemPosList[i].x = 40;
					m_pClientList[iClientH]->m_ItemPosList[i].y = 30;

					m_pClientList[iClientH]->m_bIsItemEquipped[i] = FALSE;

					m_pClientList[iClientH]->m_pItemInBankList[cBankItemIndex] = NULL;

					// ºó °ø°£À» ¾ø¾Ø´Ù. 
					for (j = 0; j <= DEF_MAXBANKITEMS - 2; j++) {
						if ((m_pClientList[iClientH]->m_pItemInBankList[j + 1] != NULL) && (m_pClientList[iClientH]->m_pItemInBankList[j] == NULL)) {
							m_pClientList[iClientH]->m_pItemInBankList[j] = m_pClientList[iClientH]->m_pItemInBankList[j + 1];

							m_pClientList[iClientH]->m_pItemInBankList[j + 1] = NULL;
						}
					}

					// ¼º°ø ¸Þ½ÃÁö¸¦ º¸³½´Ù.
					ZeroMemory(cMsg, sizeof(cMsg));

					dwp = (DWORD*)(cMsg + DEF_INDEX4_MSGID);
					*dwp = MSGID_RESPONSE_RETRIEVEITEM;
					wp = (WORD*)(cMsg + DEF_INDEX2_MSGTYPE);
					*wp = DEF_MSGTYPE_CONFIRM;

					cp = (char*)(cMsg + DEF_INDEX2_MSGTYPE + 2);
					*cp = cBankItemIndex;
					cp++;
					*cp = i;
					cp++;

					// ¼ÒÁöÇ° ÃÑ Áß·® Àç °è»ê 
					iCalcTotalWeight(iClientH);

					// È­»ì ÇÒ´ç
					m_pClientList[iClientH]->m_cArrowIndex = _iGetArrowItemIndex(iClientH);

					// ¸Þ½ÃÁö Àü¼Û 
					iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cMsg, 8);
					switch (iRet) {
					case DEF_XSOCKEVENT_QUENEFULL:
					case DEF_XSOCKEVENT_SOCKETERROR:
					case DEF_XSOCKEVENT_CRITICALERROR:
					case DEF_XSOCKEVENT_SOCKETCLOSED:
						// ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù.
						DeleteClient(iClientH, TRUE, TRUE);
						break;
					}
					return;
				}
			// ¾ÆÀÌÅÛÀ» µÇÃ£À» °ø°£ÀÌ ¾ø´Ù. ¿À·ù
			ZeroMemory(cMsg, sizeof(cMsg));

			dwp = (DWORD*)(cMsg + DEF_INDEX4_MSGID);
			*dwp = MSGID_RESPONSE_RETRIEVEITEM;
			wp = (WORD*)(cMsg + DEF_INDEX2_MSGTYPE);
			*wp = DEF_MSGTYPE_REJECT;

			iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cMsg, 8);
		}
	}

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

// 05/29/2004 - Hypnotoad - Limits some items from not dropping
void CGame::_PenaltyItemDrop(int iClientH, int iTotal, BOOL bIsSAattacked)
{
	int i, j, iRemainItem;
	char cItemIndexList[DEF_MAXITEMS], cItemIndex;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;

	// SNOOPY: Lucky effect will prevent drops,  even of a ZEM.
	if ((m_pClientList[iClientH]->m_bIsLuckyEffect > 0) && ((iDice(1, 100) <= m_pClientList[iClientH]->m_bIsLuckyEffect)))
	{
		return;
	}

	if ((m_pClientList[iClientH]->m_iAlterItemDropIndex != -1) && (m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex] != NULL)) {
		// Testcode
		if (m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ALTERITEMDROP) {
			// v2.04 ½ºÅæ ¿Àºê »õÅ©¸®ÆÄÀÌ½º°¡ ¸Â´ÂÁö È®ÀÎ
			// ´ëÃ¼ÀûÀ¸·Î ¶³¾îÁö´Â ¾ÆÀÌÅÛÀÌ ÀÖ´Ù¸é ´Ù¸¥ ¾ÆÀÌÅÛÀÌ ¶³¾îÁöÁö ¾Ê°í ÀÌ ¾ÆÀÌÅÛ¸¸ ¶³¾îÁø´Ù. 
			if (m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_wCurLifeSpan > 0) {
				m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_wCurLifeSpan--;
				SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CURLIFESPAN, m_pClientList[iClientH]->m_iAlterItemDropIndex, m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_wCurLifeSpan, NULL, NULL);
			}
			DropItemHandler(iClientH, m_pClientList[iClientH]->m_iAlterItemDropIndex, -1, m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_cName);

			m_pClientList[iClientH]->m_iAlterItemDropIndex = -1;
		}
		else {
			// v2.04 testcode

			// ´Ù½Ã °Ë»ö 
			for (i = 0; i < DEF_MAXITEMS; i++)
				if ((m_pClientList[iClientH]->m_pItemList[i] != NULL) && (m_pClientList[iClientH]->m_pItemList[i]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ALTERITEMDROP)) {
					m_pClientList[iClientH]->m_iAlterItemDropIndex = i;
					if (m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_wCurLifeSpan > 0) {
						m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_wCurLifeSpan--;
						SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CURLIFESPAN, m_pClientList[iClientH]->m_iAlterItemDropIndex, m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_wCurLifeSpan, NULL, NULL);
					}
					DropItemHandler(iClientH, m_pClientList[iClientH]->m_iAlterItemDropIndex, -1, m_pClientList[iClientH]->m_pItemList[m_pClientList[iClientH]->m_iAlterItemDropIndex]->m_cName);
					m_pClientList[iClientH]->m_iAlterItemDropIndex = -1;
					break;
				}

			// ¿À·ù¹ß»ý! Èñ»ý¼®ÀÌ ¾ø´Ù. 
			goto PID_DROP;
		}
		return;
	}

PID_DROP:;

	for (i = 1; i <= iTotal; i++) {
		iRemainItem = 0;
		ZeroMemory(cItemIndexList, sizeof(cItemIndexList));

		for (j = 0; j < DEF_MAXITEMS; j++)
		{
			if (m_pClientList[iClientH]->m_pItemList[j] != NULL)
			{
				// Don't drop personal items
				if ((m_pClientList[iClientH]->m_pItemList[j]->m_sTouchEffectType != 0)
					&& (m_pClientList[iClientH]->m_pItemList[j]->m_sTouchEffectValue1 == m_pClientList[iClientH]->m_sCharIDnum1)
					&& (m_pClientList[iClientH]->m_pItemList[j]->m_sTouchEffectValue2 == m_pClientList[iClientH]->m_sCharIDnum2)
					&& (m_pClientList[iClientH]->m_pItemList[j]->m_sTouchEffectValue3 == m_pClientList[iClientH]->m_sCharIDnum3))
				{
					continue;
				}

				// Don't drop equiped activable items.
				if (((m_pClientList[iClientH]->m_pItemList[j]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY)
					|| (m_pClientList[iClientH]->m_pItemList[j]->m_sItemEffectType == DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY))
					&& (bIsSAattacked == FALSE))
				{
					continue;
				}
				cItemIndexList[iRemainItem] = j;
				iRemainItem++;
			}
		}

		if (iRemainItem == 0) return;
		cItemIndex = cItemIndexList[iDice(1, iRemainItem) - 1];

		DropItemHandler(iClientH, cItemIndex, -1, m_pClientList[iClientH]->m_pItemList[cItemIndex]->m_cName);
	}
}

void CGame::GetRewardMoneyHandler(int iClientH)
{
	int iRet, iEraseReq, iWeightLeft, iRewardGoldLeft;
	DWORD* dwp;
	WORD* wp;
	char* cp, cData[100], cItemName[21];
	class CItem* pItem;
	short* sp;

	if (m_pClientList[iClientH] == NULL) return;
	if (m_pClientList[iClientH]->m_bIsInitComplete == FALSE) return;


	// ³²Àº Áß·®À» °è»êÇÑ´Ù. 
	iWeightLeft = _iCalcMaxLoad(iClientH) - iCalcTotalWeight(iClientH);

	if (iWeightLeft <= 0) return;
	// Áß·®À» ¹ÝÀ¸·Î ³ª´«´Ù. <- Ã£Àº µ·À¸·Î ¾ÆÀÌÅÛÀ» »ì °ø°£Àº ¸¶·ÃÇØ µÖ¾ß ÇÏ¹Ç·Î.
	iWeightLeft = iWeightLeft / 2;
	if (iWeightLeft <= 0) return;

	pItem = new class CItem;
	ZeroMemory(cItemName, sizeof(cItemName));
	wsprintf(cItemName, "Gold");
	_bInitItemAttr(pItem, cItemName);

	if ((iWeightLeft / iGetItemWeight(pItem, 1)) >= m_pClientList[iClientH]->m_iRewardGold) {
		// Æ÷»ó±ÝÀ» ¸ðµÎ ¹ÞÀ» ¼ö ÀÖ´Ù. 
		pItem->m_dwCount = m_pClientList[iClientH]->m_iRewardGold;
		iRewardGoldLeft = 0;
	}
	else {
		pItem->m_dwCount = (iWeightLeft / iGetItemWeight(pItem, 1));
		iRewardGoldLeft = m_pClientList[iClientH]->m_iRewardGold - (iWeightLeft / iGetItemWeight(pItem, 1));
	}

	if (_bAddClientItemList(iClientH, pItem, &iEraseReq) == TRUE) {
		// ¾ÆÀÌÅÛÀ» È¹µæÇß´Ù.

		// ³²Àº Æ÷»ó±Ý ³»¿ª °è»ê.
		m_pClientList[iClientH]->m_iRewardGold = iRewardGoldLeft;

		dwp = (DWORD*)(cData + DEF_INDEX4_MSGID);
		*dwp = MSGID_NOTIFY;
		wp = (WORD*)(cData + DEF_INDEX2_MSGTYPE);
		*wp = DEF_NOTIFY_ITEMOBTAINED;

		cp = (char*)(cData + DEF_INDEX2_MSGTYPE + 2);

		// 1°³ È¹µæÇß´Ù. <- ¿©±â¼­ 1°³¶õ Ä«¿îÆ®¸¦ ¸»ÇÏ´Â °ÍÀÌ ¾Æ´Ï´Ù
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

		// ±× ´ÙÀ½ ³²Àº Æ÷»ó±ÝÀ» ¾Ë·ÁÁØ´Ù.
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_REWARDGOLD, NULL, NULL, NULL, NULL);

		if (iEraseReq == 1) delete pItem;

		// ¾ÆÀÌÅÛ Á¤º¸ Àü¼Û 
		iRet = m_pClientList[iClientH]->m_pXSock->iSendMsg(cData, 53);

		switch (iRet) {
		case DEF_XSOCKEVENT_QUENEFULL:
		case DEF_XSOCKEVENT_SOCKETERROR:
		case DEF_XSOCKEVENT_CRITICALERROR:
			// ¸Þ½ÃÁö¸¦ º¸³¾¶§ ¿¡·¯°¡ ¹ß»ýÇß´Ù¸é Á¦°ÅÇÑ´Ù.
			DeleteClient(iClientH, TRUE, TRUE);
			break;
		}
	}
	else {
		// ¹ÞÀ» ¼ö ¾ø´Â °æ¿ì´Â ¾Æ¹«·± Ã³¸®¸¦ ÇÏÁö ¾Ê´Â´Ù. 

		// Centuu : DEF_NOTIFY_CANNOTCARRYMOREITEM
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_CANNOTCARRYMOREITEM, NULL, NULL, NULL, NULL);
	}
}


int CGame::_iCalcMaxLoad(int iClientH)
{
	if (m_pClientList[iClientH] == NULL) return 0;

	return (((m_pClientList[iClientH]->m_iStr + m_pClientList[iClientH]->m_iAngelicStr) * 500) + ((m_pClientList[iClientH]->m_iLevel) * 500));
}

BOOL CGame::bCheckAndConvertPlusWeaponItem(int iClientH, int iItemIndex)
{
	// ÀÌ ¾ÆÀÌÅÛÀÌ +1, +2 ÀÌ¸§ÀÌ ºÙÀº ¾ÆÀÌÅÛÀÌ¶ó¸é Attribute ÇÃ·¡±×·Î Æ¯¼ºÄ¡¸¦ ÀÌµ¿½ÃÅ°°í ÀÏ¹Ý ¾ÆÀÌÅÛÀ¸·Î º¯Çü½ÃÅ²´Ù.
	if (m_pClientList[iClientH] == NULL) return FALSE;
	if (m_pClientList[iClientH]->m_pItemList[iItemIndex] == NULL) return FALSE;

	switch (m_pClientList[iClientH]->m_pItemList[iItemIndex]->m_sIDnum) {
	case 4:  // Dagger +1
	case 9:  // Short Sword +1
	case 13: // Main Gauge +1
	case 16: // Gradius +1
	case 18: // Long Sword +1
	case 19: // Long Sword +2
	case 21: // Excaliber +1
	case 24: // Sabre +1
	case 26: // Scimitar +1
	case 27: // Scimitar +2
	case 29: // Falchoin +1
	case 30: // Falchion +2
	case 32: // Esterk +1
	case 33: // Esterk +2
	case 35: // Rapier +1
	case 36: // Rapier +2
	case 39: // Broad Sword +1
	case 40: // Broad Sword +2
	case 43: // Bastad Sword +1
	case 44: // Bastad Sword +2
	case 47: // Claymore +1
	case 48: // Claymore +2
	case 51: // Great Sword +1
	case 52: // Great Sword +2
	case 55: // Flameberge +1
	case 56: // Flameberge +2
	case 60: // Light Axe +1
	case 61: // Light Axe +2
	case 63: // Tomahoc +1
	case 64: // Tomohoc +2
	case 66: // Sexon Axe +1
	case 67: // Sexon Axe +2
	case 69: // Double Axe +1
	case 70: // Double Axe +2
	case 72: // War Axe +1
	case 73: // War Axe +2

	case 580: // Battle Axe +1
	case 581: // Battle Axe +2
	case 582: // Sabre +2
		return TRUE;

	}
	return FALSE;
}

void CGame::ArmorLifeDecrement(int iAttackerH, int iTargetH, char cOwnerType, int iValue)
{
	int iTemp;

	if (m_pClientList[iAttackerH] == NULL) return;

	switch (cOwnerType) {
	case DEF_OWNERTYPE_PLAYER:
		if (m_pClientList[iTargetH] == NULL) return;
		break;

	case DEF_OWNERTYPE_NPC:
	default:
		return;
	}

	if (m_pClientList[iAttackerH]->m_cSide == m_pClientList[iTargetH]->m_cSide) return;

	switch (iDice(1, 13)) {
	case 1:
		iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY];
		if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != NULL)) {
			if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= 50;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_CURLIFESPAN, iTemp, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan, NULL, NULL);
				if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 250) {
					ReleaseItemHandler(iTargetH, iTemp, TRUE);
					SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				}
			}
			if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				ReleaseItemHandler(iTargetH, iTemp, TRUE);
			}
		}
		break;

	case 2:
		iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS];
		if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != NULL)) {


			if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= 50;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_CURLIFESPAN, iTemp, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan, NULL, NULL);
				if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 250) {
					ReleaseItemHandler(iTargetH, iTemp, TRUE);
					SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				}
			}
			if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				ReleaseItemHandler(iTargetH, iTemp, TRUE);
			}
		}
		break;

	case 3:
		iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LEGGINGS];
		if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != NULL)) {

			if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= 50;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_CURLIFESPAN, iTemp, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan, NULL, NULL);
				if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 250) {
					ReleaseItemHandler(iTargetH, iTemp, TRUE);
					SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				}
			}
			if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				ReleaseItemHandler(iTargetH, iTemp, TRUE);
			}
		}
		break;

	case 4:
		iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS];
		if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != NULL)) {

			if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= 50;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_CURLIFESPAN, iTemp, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan, NULL, NULL);
				if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 250) {
					ReleaseItemHandler(iTargetH, iTemp, TRUE);
					SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				}
			}
			if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				ReleaseItemHandler(iTargetH, iTemp, TRUE);
			}
		}
		break;

	case 5:
		iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_HEAD];
		if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != NULL)) {

			if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= 50;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_CURLIFESPAN, iTemp, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan, NULL, NULL);
				if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 250) {
					ReleaseItemHandler(iTargetH, iTemp, TRUE);
					SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				}
			}
			if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				ReleaseItemHandler(iTargetH, iTemp, TRUE);
			}
		}
		break;

	case 6:
		iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_HEAD];
		if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != NULL)) {

			if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= 50;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_CURLIFESPAN, iTemp, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan, NULL, NULL);
				if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 250) {
					ReleaseItemHandler(iTargetH, iTemp, TRUE);
					SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				}
			}
			if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				ReleaseItemHandler(iTargetH, iTemp, TRUE);
			}
		}

		iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LEGGINGS];
		if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != NULL)) {

			if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= 50;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_CURLIFESPAN, iTemp, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan, NULL, NULL);
				if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 250) {
					ReleaseItemHandler(iTargetH, iTemp, TRUE);
					SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				}
			}
			if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				ReleaseItemHandler(iTargetH, iTemp, TRUE);
			}
		}
		break;

	case 7:
		iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LEGGINGS];
		if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != NULL)) {

			if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= 50;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_CURLIFESPAN, iTemp, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan, NULL, NULL);
				if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 250) {
					ReleaseItemHandler(iTargetH, iTemp, TRUE);
					SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				}
			}
			if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				ReleaseItemHandler(iTargetH, iTemp, TRUE);
			}
		}

		iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS];
		if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != NULL)) {


			if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= 50;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_CURLIFESPAN, iTemp, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan, NULL, NULL);
				if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 250) {
					ReleaseItemHandler(iTargetH, iTemp, TRUE);
					SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				}
			}
			if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				ReleaseItemHandler(iTargetH, iTemp, TRUE);
			}
		}
		break;

	case 8:
		iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS];
		if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != NULL)) {


			if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= 50;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_CURLIFESPAN, iTemp, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan, NULL, NULL);
				if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 250) {
					ReleaseItemHandler(iTargetH, iTemp, TRUE);
					SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				}
			}
			if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				ReleaseItemHandler(iTargetH, iTemp, TRUE);
			}
		}

		iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS];
		if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != NULL)) {

			if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= 50;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_CURLIFESPAN, iTemp, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan, NULL, NULL);
				if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 250) {
					ReleaseItemHandler(iTargetH, iTemp, TRUE);
					SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				}
			}
			if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				ReleaseItemHandler(iTargetH, iTemp, TRUE);
			}
		}
		break;

	case 9:
		iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS];
		if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != NULL)) {

			if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= 50;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_CURLIFESPAN, iTemp, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan, NULL, NULL);
				if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 250) {
					ReleaseItemHandler(iTargetH, iTemp, TRUE);
					SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				}
			}
			if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				ReleaseItemHandler(iTargetH, iTemp, TRUE);
			}
		}
		break;

	case 10:
		iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS];
		if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != NULL)) {

			if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= 50;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_CURLIFESPAN, iTemp, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan, NULL, NULL);
				if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 250) {
					ReleaseItemHandler(iTargetH, iTemp, TRUE);
					SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				}
			}
			if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				ReleaseItemHandler(iTargetH, iTemp, TRUE);
			}
		}

		iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY];
		if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != NULL)) {
			if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= 50;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_CURLIFESPAN, iTemp, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan, NULL, NULL);
				if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 250) {
					ReleaseItemHandler(iTargetH, iTemp, TRUE);
					SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				}
			}
			if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				ReleaseItemHandler(iTargetH, iTemp, TRUE);
			}
		}
		break;

	case 11:
		iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY];
		if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != NULL)) {
			if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= 50;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_CURLIFESPAN, iTemp, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan, NULL, NULL);
				if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 250) {
					ReleaseItemHandler(iTargetH, iTemp, TRUE);
					SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				}
			}
			if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				ReleaseItemHandler(iTargetH, iTemp, TRUE);
			}
		}

		iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_LEGGINGS];
		if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != NULL)) {

			if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= 50;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_CURLIFESPAN, iTemp, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan, NULL, NULL);
				if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 250) {
					ReleaseItemHandler(iTargetH, iTemp, TRUE);
					SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				}
			}
			if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				ReleaseItemHandler(iTargetH, iTemp, TRUE);
			}
		}
		break;

	case 12:
		iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY];
		if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != NULL)) {
			if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= 50;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_CURLIFESPAN, iTemp, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan, NULL, NULL);
				if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 250) {
					ReleaseItemHandler(iTargetH, iTemp, TRUE);
					SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				}
			}
			if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				ReleaseItemHandler(iTargetH, iTemp, TRUE);
			}
		}
		break;

	case 13:
		iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY];
		if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != NULL)) {
			if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= 50;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_CURLIFESPAN, iTemp, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan, NULL, NULL);
				if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 250) {
					ReleaseItemHandler(iTargetH, iTemp, TRUE);
					SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				}
			}
			if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				ReleaseItemHandler(iTargetH, iTemp, TRUE);
			}
		}
		iTemp = m_pClientList[iTargetH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS];
		if ((iTemp != -1) && (m_pClientList[iTargetH]->m_pItemList[iTemp] != NULL)) {


			if ((m_pClientList[iTargetH]->m_cSide != 0) && (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan > 0)) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan -= 50;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_CURLIFESPAN, iTemp, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan, NULL, NULL);
				if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 250) {
					ReleaseItemHandler(iTargetH, iTemp, TRUE);
					SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMRELEASED, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				}
			}
			if (m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan <= 0) {
				m_pClientList[iTargetH]->m_pItemList[iTemp]->m_wCurLifeSpan = 0;
				SendNotifyMsg(NULL, iTargetH, DEF_NOTIFY_ITEMLIFESPANEND, m_pClientList[iTargetH]->m_pItemList[iTemp]->m_cEquipPos, iTemp, NULL, NULL);
				ReleaseItemHandler(iTargetH, iTemp, TRUE);
			}
		}
		break;
	}
}

char CGame::_cCheckHeroItemEquipped(int iClientH)
{
	short sHeroLeggings, sHeroHauberk, sHeroArmor, sHeroHelm;

	if (m_pClientList[iClientH] == NULL) return 0;

	sHeroHelm = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_HEAD];
	sHeroArmor = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_BODY];
	sHeroHauberk = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_ARMS];
	sHeroLeggings = m_pClientList[iClientH]->m_sItemEquipmentStatus[DEF_EQUIPPOS_PANTS];

	if ((sHeroHelm < 0) || (sHeroLeggings < 0) || (sHeroArmor < 0) || (sHeroHauberk < 0)) return 0;

	if (m_pClientList[iClientH]->m_pItemList[sHeroHelm] == NULL) return 0;
	if (m_pClientList[iClientH]->m_pItemList[sHeroLeggings] == NULL) return 0;
	if (m_pClientList[iClientH]->m_pItemList[sHeroArmor] == NULL) return 0;
	if (m_pClientList[iClientH]->m_pItemList[sHeroHauberk] == NULL) return 0;

	if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 403) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 411) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 419) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 423)) return 1;

	if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 407) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 415) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 419) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 423)) return 2;

	if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 404) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 412) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 420) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 424)) return 1;

	if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 408) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 416) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 420) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 424)) return 2;

	if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 405) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 413) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 421) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 425)) return 1;

	if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 409) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 417) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 421) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 425)) return 2;

	if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 406) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 414) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 422) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 426)) return 1;

	if ((m_pClientList[iClientH]->m_pItemList[sHeroHelm]->m_sIDnum == 410) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroArmor]->m_sIDnum == 418) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroHauberk]->m_sIDnum == 422) &&
		(m_pClientList[iClientH]->m_pItemList[sHeroLeggings]->m_sIDnum == 426)) return 2;

	return 0;
}

void CGame::_bDecodeDupItemIDFileContents(char* pData, DWORD dwMsgSize)
{
	char* pContents, * token, cTxt[120];
	char seps[] = "= \t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int  iIndex = 0;
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
					// ��ų ��ȣ 
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) ERROR! DupItemID configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return;
					}

					if (m_pDupItemIDList[atoi(token)] != NULL) {
						// �̹� �Ҵ�� ��ȣ�� �ִ�. �����̴�.
						PutLogList("(!!!) ERROR! DupItemID configuration file error - Duplicate magic number.");
						delete[] pContents;
						delete pStrTok;
						return;
					}
					m_pDupItemIDList[atoi(token)] = new class CItem;
					iIndex = atoi(token);

					cReadModeB = 2;
					break;

				case 2:
					// m_sTouchEffectType
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) ERROR! DupItemID configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return;
					}
					m_pDupItemIDList[iIndex]->m_sTouchEffectType = atoi(token);
					cReadModeB = 3;
					break;

				case 3:
					// m_sTouchEffectValue1
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) ERROR! DupItemID configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return;
					}
					m_pDupItemIDList[iIndex]->m_sTouchEffectValue1 = atoi(token);
					cReadModeB = 4;
					break;

				case 4:
					// m_sTouchEffectValue2
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) ERROR! DupItemID configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return;
					}
					m_pDupItemIDList[iIndex]->m_sTouchEffectValue2 = atoi(token);
					cReadModeB = 5;
					break;

				case 5:
					// m_sTouchEffectValue3
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) ERROR! DupItemID configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return;
					}
					m_pDupItemIDList[iIndex]->m_sTouchEffectValue3 = atoi(token);
					cReadModeB = 6;
					break;

				case 6:
					// m_wPrice
					if (_bGetIsStringIsNumber(token) == FALSE) {
						PutLogList("(!!!) ERROR! DupItemID configuration file error - Wrong Data format.");
						delete[] pContents;
						delete pStrTok;
						return;
					}
					m_pDupItemIDList[iIndex]->m_wPrice = (WORD)atoi(token);
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
			if (memcmp(token, "DupItemID", 9) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
			}

		}
		token = pStrTok->pGet();
	}

	delete pStrTok;
	delete[] pContents;

	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		PutLogList("(!!!) ERROR! DupItemID configuration file contents error!");
		return;
	}

	wsprintf(cTxt, "(!) DupItemID(Total:%d) configuration - success!", iIndex);
	PutLogList(cTxt);
}

BOOL CGame::_bCheckDupItemID(CItem* pItem)
{
	int i;

	for (i = 0; i < DEF_MAXDUPITEMID; i++)
		if (m_pDupItemIDList[i] != NULL) {
			if ((pItem->m_sTouchEffectType == m_pDupItemIDList[i]->m_sTouchEffectType) &&
				(pItem->m_sTouchEffectValue1 == m_pDupItemIDList[i]->m_sTouchEffectValue1) &&
				(pItem->m_sTouchEffectValue2 == m_pDupItemIDList[i]->m_sTouchEffectValue2) &&
				(pItem->m_sTouchEffectValue3 == m_pDupItemIDList[i]->m_sTouchEffectValue3)) {
				// °¡°Ý Á¤º¸¸¦ °»½ÅÇÏ°í ¸®ÅÏ.
				pItem->m_wPrice = m_pDupItemIDList[i]->m_wPrice;
				return TRUE;
			}
		}

	return FALSE;
}

void CGame::_AdjustRareItemValue(CItem* pItem)
{
	DWORD dwSWEType, dwSWEValue;
	double dV1, dV2, dV3;

	if ((pItem->m_dwAttribute & 0x00F00000) != NULL) {
		dwSWEType = (pItem->m_dwAttribute & 0x00F00000) >> 20;
		dwSWEValue = (pItem->m_dwAttribute & 0x000F0000) >> 16;
		// Èñ±Í ¾ÆÀÌÅÛ È¿°ú Á¾·ù: 
		// 0-None 1-ÇÊ»ì±â´ë¹ÌÁöÃß°¡ 2-Áßµ¶È¿°ú 3-Á¤ÀÇÀÇ 
		// 5-¹ÎÃ¸ÀÇ 6-°¡º­¿î 7-¿¹¸®ÇÑ 8-°­È­µÈ 9-°í´ë¹®¸íÀÇ
		if (iDice(1, 100) < m_iRareDropRate / 100) { // Centuu : Agregado para controlar el drop rare.
			switch (dwSWEType) {
			case 0:
				break;

			case 5: // ¹ÎÃ¸ÀÇ 
				pItem->m_cSpeed--;
				if (pItem->m_cSpeed < 0) pItem->m_cSpeed = 0;
				break;

			case 6: // °¡º­¿î 
				dV2 = (double)pItem->m_wWeight;
				dV3 = (double)(dwSWEValue * 4);
				dV1 = (dV3 / 100.0f) * dV2;
				pItem->m_wWeight -= (int)dV1;

				if (pItem->m_wWeight < 1) pItem->m_wWeight = 1;
				break;

			case 8: // °­È­µÈ 
			case 9: // °í´ë¹®¸íÀÇ 
				dV2 = (double)pItem->m_wMaxLifeSpan;
				dV3 = (double)(dwSWEValue * 7);
				dV1 = (dV3 / 100.0f) * dV2;
				pItem->m_wMaxLifeSpan += (int)dV1;
				break;
			}
		}
	}
}

// Item Logging
BOOL CGame::_bItemLog(int iAction, int iGiveH, int iRecvH, class CItem* pItem, BOOL bForceItemLog)
{
	char  cTxt[1024], cTemp1[120], cTemp2[120];
	int iItemCount;
	if (pItem == NULL) return FALSE;

	// !!ÁÖÀÇ ÇÑ±¹¿¡ Àû¿ëÇÒ¶§  New Item ÀÌ »ý±æ¶§´Â  iGive°¡ ³ÎÀÏ¼ö ÀÖ´Ù.
	if (m_pClientList[iGiveH]->m_cCharName == NULL) return FALSE;

	if (iAction == DEF_ITEMLOG_DUPITEMID) {
		// º¹»çµÈ ¾ÆÀÌÅÛ ÀúÀå ¿äÃ»ÀÌ´Ù. 
		if (m_pClientList[iGiveH] == NULL) return FALSE;
		if (m_pClientList[iGiveH]->m_cCharName == NULL) return FALSE;
		wsprintf(G_cTxt, "(!) Delete-DupItem(%s %d %d %d %d) Owner(%s)", pItem->m_cName, pItem->m_dwCount, pItem->m_sTouchEffectValue1,
			pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3,
			m_pClientList[iGiveH]->m_cCharName);
		bSendMsgToLS(MSGID_GAMEITEMLOG, iGiveH, NULL, G_cTxt);
		return TRUE;
	}

	ZeroMemory(cTxt, sizeof(cTxt));
	ZeroMemory(cTemp1, sizeof(cTemp1));
	ZeroMemory(cTemp2, sizeof(cTemp2));

	switch (iAction) {

	case DEF_ITEMLOG_EXCHANGE:
		if (m_pClientList[iRecvH]->m_cCharName == NULL) return FALSE;
		wsprintf(cTxt, "(%s) PC(%s)\tExchange\t%s(%d %d %d %d %x)\t%s(%d %d)\tPC(%s)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
			pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
			m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY, m_pClientList[iRecvH]->m_cCharName);
		break;

	case DEF_ITEMLOG_GIVE:
		if (m_pClientList[iRecvH]->m_cCharName == NULL) return FALSE;
		wsprintf(cTxt, "(%s) PC(%s)\tGive\t%s(%d %d %d %d %x)\t%s(%d %d)\tPC(%s)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
			pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
			m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY, m_pClientList[iRecvH]->m_cCharName);
		break;

	case DEF_ITEMLOG_DROP:
		wsprintf(cTxt, "(%s) PC(%s)\tDrop\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
			pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
			m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
		break;

	case DEF_ITEMLOG_GET:
		wsprintf(cTxt, "(%s) PC(%s)\tGet\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
			pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
			m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
		break;

	case DEF_ITEMLOG_MAKE:
		wsprintf(cTxt, "(%s) PC(%s)\tMake\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
			pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
			m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
		break;

	case DEF_ITEMLOG_DEPLETE:
		wsprintf(cTxt, "(%s) PC(%s)\tDeplete\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
			pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
			m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
		break;

	case DEF_ITEMLOG_BUY:
		iItemCount = iRecvH;
		wsprintf(cTxt, "(%s) PC(%s)\tBuy\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, iItemCount,
			pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
			m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
		break;

	case DEF_ITEMLOG_SELL:
		wsprintf(cTxt, "(%s) PC(%s)\tSell\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
			pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
			m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
		break;

	case DEF_ITEMLOG_RETRIEVE:
		wsprintf(cTxt, "(%s) PC(%s)\tRetrieve\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
			pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
			m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
		break;

	case DEF_ITEMLOG_DEPOSIT:
		wsprintf(cTxt, "(%s) PC(%s)\tDeposit\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName, pItem->m_dwCount,
			pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
			m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
		break;

	case DEF_ITEMLOG_UPGRADEFAIL:
		wsprintf(cTxt, "(%s) PC(%s)\tUpgrade Fail\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName,
			pItem->m_dwCount, pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
			m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
		break;

	case DEF_ITEMLOG_UPGRADESUCCESS:
		wsprintf(cTxt, "(%s) PC(%s)\tUpgrade Success\t%s(%d %d %d %d %x)\t%s(%d %d)", m_pClientList[iGiveH]->m_cIPaddress, m_pClientList[iGiveH]->m_cCharName, pItem->m_cName,
			pItem->m_dwCount, pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3, pItem->m_dwAttribute,
			m_pClientList[iGiveH]->m_cMapName, m_pClientList[iGiveH]->m_sX, m_pClientList[iGiveH]->m_sY);
		break;
	default:
		return FALSE;
	}
	bSendMsgToLS(MSGID_GAMEITEMLOG, iGiveH, NULL, cTxt);
	return TRUE;
}

BOOL CGame::_bItemLog(int iAction, int iClientH, char* cName, class CItem* pItem)
{
	if (pItem == NULL) return FALSE;
	if (_bCheckGoodItem(pItem) == FALSE) return FALSE;
	if (iAction != DEF_ITEMLOG_NEWGENDROP)
	{
		if (m_pClientList[iClientH] == NULL) return FALSE;
	}
	char  cTxt[200], cTemp1[120];
	//  ·Î±× ³²±ä´Ù. 
	ZeroMemory(cTxt, sizeof(cTxt));
	ZeroMemory(cTemp1, sizeof(cTemp1));
	if (m_pClientList[iClientH] != NULL) m_pClientList[iClientH]->m_pXSock->iGetPeerAddress(cTemp1);

	switch (iAction) {

	case DEF_ITEMLOG_NEWGENDROP:
		if (pItem == NULL) return FALSE;
		wsprintf(cTxt, "NPC(%s)\tDrop\t%s(%d %d %d %d)", cName, pItem->m_cName, pItem->m_dwCount,
			pItem->m_sTouchEffectValue1, pItem->m_sTouchEffectValue2, pItem->m_sTouchEffectValue3);
		break;
	case DEF_ITEMLOG_SKILLLEARN:
	case DEF_ITEMLOG_MAGICLEARN:
		if (cName == NULL) return FALSE;
		if (m_pClientList[iClientH] == NULL) return FALSE;
		wsprintf(cTxt, "PC(%s)\tLearn\t(%s)\t%s(%d %d)\t \tIP(%s)", m_pClientList[iClientH]->m_cCharName, cName,
			m_pClientList[iClientH]->m_cMapName, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, cTemp1);
		break;
	case DEF_ITEMLOG_SUMMONMONSTER:
		if (cName == NULL) return FALSE;
		if (m_pClientList[iClientH] == NULL) return FALSE;
		wsprintf(cTxt, "PC(%s)\tSummon\t(%s)\t%s(%d %d)\t \tIP(%s)", m_pClientList[iClientH]->m_cCharName, cName,
			m_pClientList[iClientH]->m_cMapName, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, cTemp1);
		break;
	case DEF_ITEMLOG_POISONED:
		if (m_pClientList[iClientH] == NULL) return FALSE;
		wsprintf(cTxt, "PC(%s)\tBe Poisoned\t \t%s(%d %d)\t \tIP(%s)", m_pClientList[iClientH]->m_cCharName,
			m_pClientList[iClientH]->m_cMapName, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, cTemp1);
		break;

	case DEF_ITEMLOG_REPAIR:
		if (cName == NULL) return FALSE;
		if (m_pClientList[iClientH] == NULL) return FALSE;
		wsprintf(cTxt, "PC(%s)\tRepair\t(%s)\t%s(%d %d)\t \tIP(%s)", m_pClientList[iClientH]->m_cCharName, cName,
			m_pClientList[iClientH]->m_cMapName, m_pClientList[iClientH]->m_sX, m_pClientList[iClientH]->m_sY, cTemp1);
		break;

	default:
		return FALSE;
	}
	bSendMsgToLS(MSGID_GAMEITEMLOG, iClientH, NULL, cTxt);
	return TRUE;
}

BOOL CGame::_bCheckGoodItem(class CItem* pItem)
{
	if (pItem == NULL) return FALSE;

	if (pItem->m_sIDnum == 90)
	{
		if (pItem->m_dwCount > 10000) return TRUE;  //Gold¿¡ ÇÑÇØ 10000¿ø ÀÌ»ó¸¸ ·Î±×¿¡ ³²±ä´Ù.
		else return FALSE;
	}
	switch (pItem->m_sIDnum) {
		// Gold Ãß°¡ 
	case 259:
	case 290:
	case 291:
	case 292:
	case 300:
	case 305:
	case 308:
	case 311:
	case 334:
	case 335:
	case 336:
	case 338:
	case 380:
	case 381:
	case 382:
	case 391:
	case 400:
	case 401:
	case 490:
	case 491:
	case 492:
	case 508:
	case 581:
	case 610:
	case 611:
	case 612:
	case 613:
	case 614:
	case 616:  // µ¥¸ó-½½·¹ÀÌ¾î
	case 618:  // ´ÙÅ©¿¤ÇÁ-º¸¿ì

	case 620:
	case 621:
	case 622:
	case 623:

	case 630:
	case 631:

	case 632:
	case 633:
	case 634:
	case 635:
	case 636:
	case 637:
	case 638:
	case 639:
	case 640:
	case 641:

	case 642:
	case 643:

	case 644:
	case 645:
	case 646:
	case 647:

	case 650:
	case 654:
	case 655:
	case 656:
	case 657:

	case 700: 	// v2.03 »ó¾î ¾ÆÀÌÅÛ 
	case 701:
	case 702:
	case 703:
	case 704:
	case 705:
	case 706:
	case 707:
	case 708:
	case 709:
	case 710:
	case 711:
	case 712:
	case 713:
	case 714:
	case 715:

	case 720:
	case 721:
	case 722:
	case 723:

	case 724:
	case 725:
	case 726:
	case 727:
	case 728:
	case 729:
	case 730:
	case 731:
	case 732:
	case 733:

	case 734:
	case 735:

	case 736:  // »õ·Î¿î »ó¾îÀÇ ÀÚÀÌ¾ðÆ® ¼Òµå
	case 737:  // »õ·Î¿î Èæ±â»çÀÇ ÀÚÀÌ¾ðÆ® ¼Òµå
	case 738:  // »õ·Î¿î Èæ¸¶¹ý»çÀÇ¸ÅÁ÷¿øµå

		return TRUE;  //Æ¯º°ÇÑ ¾ÆÀÌÅÛÀÌ±â ¶«½Ã ±â·Ï...

	default:
		// v2.17 2002-7-31 Á¦ÀÛ ¾ÆÀÌÅÛµµ ·Î±×¿¡ ³²°Ô ÇÑ´Ù.
		if ((pItem->m_dwAttribute & 0xF0F0F001) == NULL) return FALSE;  //Æ¯º°ÇÑ ¾ÆÀÌÅÛµÎ ¾Æ´Ï±¸ Æ¯¼ºÄ¡µµ ¾ø´Ù¸é º°·ç..
		else if (pItem->m_sIDnum > 30) return TRUE;  //Æ¯º°ÇÑ ¾ÆÀÌÅÛÀº ¾Æ´ÏÁö¸¸ Æ¯¼ºÄ¡°¡ ÀÖ°í ´Ü°Ë·ù°¡ ¾Æ´Ï¶ó¸é ÁÁÀº ¾ÆÅÛ..
		else return FALSE;  //Æ¯º°ÇÑ ¾ÆÀÌÅÛµÎ ¾Æ´Ï±¸ Æ¯¼ºÄ¡´Â ÀÖÁö¸¸ ´Ü°Ë·ù¶ó¸é º°·ç...
	}
}

void CGame::RequestSellItemListHandler(int iClientH, char* pData)
{
	int i, * ip, iAmount;
	char* cp, cIndex;
	struct {
		char cIndex;
		int  iAmount;
	} stTemp[12];

	if (m_pClientList[iClientH] == NULL) return;

	cp = (char*)(pData + 6);
	for (i = 0; i < 12; i++) {
		stTemp[i].cIndex = *cp;
		cp++;

		ip = (int*)cp;
		stTemp[i].iAmount = *ip;
		cp += 4;
	}

	// ³»¿ëÀ» ´Ù ÀÐ¾ú´Ù. ¼ø¼­´ë·Î ÆÈ¾ÆÄ¡¿î´Ù.
	for (i = 0; i < 12; i++) {
		cIndex = stTemp[i].cIndex;
		iAmount = stTemp[i].iAmount;

		if ((cIndex == -1) || (cIndex < 0) || (cIndex >= DEF_MAXITEMS)) return;
		if (m_pClientList[iClientH]->m_pItemList[cIndex] == NULL) return;

		// ¿¡ ÇØ´çÇÏ´Â ¾ÆÀÌÅÛÀ» ÆÇ´Ù.
		ReqSellItemConfirmHandler(iClientH, cIndex, iAmount, NULL);
		// ÀÌ ·çÆ¾À» ¼öÇàÇÑ ´ÙÀ½ Å¬¶óÀÌ¾ðÆ®°¡ »èÁ¦µÇ¾úÀ» ¼ö ÀÖÀ¸´Ï ÁÖÀÇ!

	}
}

int CGame::iGetItemWeight(CItem* pItem, int iCount)
{
	int iWeight;

	// ¾ÆÀÌÅÛÀÇ ¼ö·®¿¡ µû¸¥ ¹«°Ô¸¦ °è»êÇÑ´Ù. GoldÀÎ °æ¿ì ¹«°Ô¸¦ 20ºÐÀÇ 1·Î º¯°æ 
	iWeight = (pItem->m_wWeight);
	if (iCount < 0) iCount = 1;
	iWeight = iWeight * iCount;
	if (pItem->m_sIDnum == 90) iWeight = iWeight / 20;
	if (iWeight <= 0) iWeight = 1;

	return iWeight;
}

BOOL CGame::bCopyItemContents(CItem* pCopy, CItem* pOriginal)
{
	if (pOriginal == NULL) return FALSE;
	if (pCopy == NULL) return FALSE;

	pCopy->m_sIDnum = pOriginal->m_sIDnum;					// ¾ÆÀÌÅÛÀÇ °íÀ¯ ¹øÈ£ 
	pCopy->m_cItemType = pOriginal->m_cItemType;
	pCopy->m_cEquipPos = pOriginal->m_cEquipPos;
	pCopy->m_sItemEffectType = pOriginal->m_sItemEffectType;
	pCopy->m_sItemEffectValue1 = pOriginal->m_sItemEffectValue1;
	pCopy->m_sItemEffectValue2 = pOriginal->m_sItemEffectValue2;
	pCopy->m_sItemEffectValue3 = pOriginal->m_sItemEffectValue3;
	pCopy->m_sItemEffectValue4 = pOriginal->m_sItemEffectValue4;
	pCopy->m_sItemEffectValue5 = pOriginal->m_sItemEffectValue5;
	pCopy->m_sItemEffectValue6 = pOriginal->m_sItemEffectValue6;
	pCopy->m_wMaxLifeSpan = pOriginal->m_wMaxLifeSpan;
	pCopy->m_sSpecialEffect = pOriginal->m_sSpecialEffect;

	//v1.432 ¸íÁß·ü °¡°¨ »ç¿ë ¾ÈÇÑ´Ù. ´ë½Å Æ¯¼ö ´É·Â ¼öÄ¡°¡ µé¾î°£´Ù.
	pCopy->m_sSpecialEffectValue1 = pOriginal->m_sSpecialEffectValue1;
	pCopy->m_sSpecialEffectValue2 = pOriginal->m_sSpecialEffectValue2;

	pCopy->m_sSprite = pOriginal->m_sSprite;
	pCopy->m_sSpriteFrame = pOriginal->m_sSpriteFrame;

	pCopy->m_cApprValue = pOriginal->m_cApprValue;
	pCopy->m_cSpeed = pOriginal->m_cSpeed;

	pCopy->m_wPrice = pOriginal->m_wPrice;
	pCopy->m_wWeight = pOriginal->m_wWeight;
	pCopy->m_sLevelLimit = pOriginal->m_sLevelLimit;
	pCopy->m_cGenderLimit = pOriginal->m_cGenderLimit;

	pCopy->m_sRelatedSkill = pOriginal->m_sRelatedSkill;

	pCopy->m_cCategory = pOriginal->m_cCategory;
	pCopy->m_bIsForSale = pOriginal->m_bIsForSale;
	// 

	pCopy->m_dwCount = pOriginal->m_dwCount;
	pCopy->m_sTouchEffectType = pOriginal->m_sTouchEffectType;
	pCopy->m_sTouchEffectValue1 = pOriginal->m_sTouchEffectValue1;
	pCopy->m_sTouchEffectValue2 = pOriginal->m_sTouchEffectValue2;
	pCopy->m_sTouchEffectValue3 = pOriginal->m_sTouchEffectValue3;
	pCopy->m_cItemColor = pOriginal->m_cItemColor;
	pCopy->m_sItemSpecEffectValue1 = pOriginal->m_sItemSpecEffectValue1;
	pCopy->m_sItemSpecEffectValue2 = pOriginal->m_sItemSpecEffectValue2;
	pCopy->m_sItemSpecEffectValue3 = pOriginal->m_sItemSpecEffectValue3;
	pCopy->m_wCurLifeSpan = pOriginal->m_wCurLifeSpan;
	pCopy->m_dwAttribute = pOriginal->m_dwAttribute;

	return TRUE;
}