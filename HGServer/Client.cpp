// Client.cpp: implementation of the CClient class.
//
//////////////////////////////////////////////////////////////////////

#include "Client.h"

#pragma warning (disable : 4996 4244)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClient::CClient(HWND hWnd)
{
 int i;	

	m_pXSock = NULL;
	m_pXSock = new class XSocket(hWnd, DEF_CLIENTSOCKETBLOCKLIMIT);
	m_pXSock->bInitBufferSize(DEF_MSGBUFFERSIZE);
	ZeroMemory(m_cProfile, sizeof(m_cProfile));
	strcpy(m_cProfile, "__________");
	ZeroMemory(m_cCharName, sizeof(m_cCharName));
	ZeroMemory(m_cAccountName, sizeof(m_cAccountName));
	ZeroMemory(m_cAccountPassword, sizeof(m_cAccountPassword));
	ZeroMemory(m_cGuildName, sizeof(m_cGuildName));
	ZeroMemory(m_cLocation, sizeof(m_cLocation));
	ZeroMemory(m_cMapName, sizeof(m_cMapName)); // new
	strcpy(m_cLocation, "NONE");
	m_iGuildRank = -1;
	m_iGuildGUID = -1;
	m_bIsInitComplete = FALSE;
	m_iLU_Pool = 0; // removed
	
	m_cWarType = 0;
	m_iEnemyKillCount = 0;
	m_iMaxEK = 0;
	m_iPKCount = 0;
	m_iRewardGold = 0;
	m_iCurWeightLoad = 0;
	m_bIsSafeAttackMode  = FALSE;
	for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++) {
		m_sItemEquipmentStatus[i] = -1;
	}
	for (i = 0; i < DEF_MAXITEMS; i++) {
		m_pItemList[i]       = NULL;
		m_ItemPosList[i].x   = 40;
		m_ItemPosList[i].y   = 30;
		m_bIsItemEquipped[i] = FALSE;
	}
	m_cArrowIndex = -1;
	for (i = 0; i < DEF_MAXBANKITEMS; i++) {
		m_pItemInBankList[i] = NULL;
	}
	for (i = 0; i < DEF_MAXMAGICTYPE; i++) {
		m_cMagicMastery[i] = NULL;
	}
	for (i = 0; i < DEF_MAXSKILLTYPE; i++) {
		m_cSkillMastery[i] = NULL;
	}
	for (i = 0; i < DEF_MAXSKILLTYPE; i++) {
		m_bSkillUsingStatus[i] = FALSE;
		m_iSkillUsingTimeID[i] = NULL;
	}
	m_cMapIndex = -1;
	m_sX = -1;
	m_sY = -1;
	m_cDir = 5; 
	m_sType = 0;
	m_sOriginalType = 0;
	m_sAppr1 = 0;
	m_sAppr2 = 0;
	m_sAppr3 = 0;
	m_sAppr4 = 0;
	m_iApprColor = 0;
	m_iStatus = 0;
	m_iStatus2 = 0;
	m_cSex = 0;
	m_cSkin = 0;
	m_cHairStyle = 0;
	m_cHairColor = 0;
	m_cUnderwear = 0;
	m_cAttackDiceThrow_SM = 0;
	m_cAttackDiceRange_SM = 0;
	m_cAttackDiceThrow_L = 0;
	m_cAttackDiceRange_L = 0;
	m_cAttackBonus_SM = 0;
	m_cAttackBonus_L = 0;
	m_cSide = 0;
	m_iHitRatio = 0;
	m_iDefenseRatio = 0;
	for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++) {
		m_iDamageAbsorption_Armor[i]  = 0;
	}
	m_iDamageAbsorption_Shield = 0;
	m_iHPstock = 0;
	// <MISSING_VARIABLE_HERE> 00B4h 
	m_bIsKilled = FALSE;
	for (i = 0; i < DEF_MAXMAGICEFFECTS; i++) {
		m_cMagicEffectStatus[i]	= 0;
	}
	m_iWhisperPlayerIndex = -1;
	ZeroMemory(m_cWhisperPlayerName, sizeof(m_cWhisperPlayerName));
	m_iHungerStatus = 100;
	m_bIsWarLocation = FALSE;
	m_bIsPoisoned = FALSE;
	m_iPoisonLevel = 0;
	m_iAdminUserLevel = 0;
	m_iRating = 0;
	m_iTimeLeft_ShutUp = 0;
	m_iTimeLeft_Rating = 0;
	m_iTimeLeft_ForceRecall = 0;
	m_iTimeLeft_FirmStaminar = 0;
	m_iRecentRunTime = 0;
	m_iRecentWalkTime = 0;
	// <MISSING_VARIABLE_HERE> 0BC0h 
	m_sV1 = 0;
	m_bIsOnServerChange = FALSE;
	m_iExpStock = 0;
	m_iAllocatedFish = NULL;
	m_iFishChance = 0;

	m_iAngelicStr = 0; // By Snoopy81
	m_iAngelicInt = 0;
	m_iAngelicDex = 0;
	m_iAngelicMag = 0;

	//MORLA 2.2 - Nuevos Datos en el PJ para el Deathmach Game
	m_iDeaths = 0;
	m_iDGPoints = 0;
	m_iDGDeaths = 0;
	m_iDGKills = 0;
	m_iTotalDGDeaths = 0;
	m_iTotalDGKills = 0;

	m_iWantedLevel = 0; // Wanted System

	m_bIsPolymorph = FALSE;

	m_dwDSLAT = m_dwDSLATOld = m_dwDSLATS = 0;
	m_iDSCount = 0;

	// REINTRODUCED by SNOOPY
	m_iHitRatio_ItemEffect_SM = 0;
	m_iHitRatio_ItemEffect_L  = 0;
	m_iMagicHitRatio_ItemEffect = 0;

	ZeroMemory(m_cIPaddress, sizeof(m_cIPaddress)); 
	ZeroMemory(m_cIP2, sizeof(m_cIP2)); 

	
	m_dwAFKCheckTime = 0;

	m_bIsOnWaitingProcess = FALSE;
	m_iSuperAttackLeft = 0;
	m_iSuperAttackCount = 0;
	m_sUsingWeaponSkill = 5;
	m_iManaSaveRatio = 0;
	m_iAddResistMagic = 0;
	m_iAddPhysicalDamage = 0;
	m_iAddMagicalDamage = 0;
	m_bIsLuckyEffect = FALSE;
	m_iSideEffect_MaxHPdown = 0;
	m_iAddAbsAir = 0;
	m_iAddAbsEarth = 0;
	m_iAddAbsFire = 0;
	m_iAddAbsWater = 0;
	m_iComboAttackCount = 0;
	m_iDownSkillIndex = -1;
	m_iMagicDamageSaveItemIndex = -1;
	m_sCharIDnum1 = m_sCharIDnum2 = m_sCharIDnum3 = 0;
	m_iPartyRank = -1; // uncommented new
	m_iPartyMemberCount = 0; // uncommented new
	m_iPartyGUID = 0; // new
	for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
		m_stPartyMemberName[i].iIndex = 0;
		ZeroMemory(m_stPartyMemberName[i].cName, sizeof(m_stPartyMemberName[i].cName));
	}
	m_iAbuseCount = 0;
	m_bIsBWMonitor = FALSE;
	m_bIsForceDisconnect = FALSE; // new
	// <MISSING_VARIABLE_HERE> 0CD0h 
	m_bIsBeingResurrected = FALSE;
	m_cExchangeMode = 0;
	m_iFightZoneTicketNumber = m_iFightzoneNumber = m_iReserveTime =  0;
	// <MISSING_VARIABLE_HERE> 0B8Ch 
	// <MISSING_VARIABLE_HERE> 0B88h 
	// <MISSING_VARIABLE_HERE> 0B84h
	m_iExchangeH = NULL;
	for (i = 0; i < 4; i++) {
		m_cExchangeItemIndex[i]  = 0;
		m_iExchangeItemAmount[i] = 0; 
		m_iExchangeAlterIndex[i] = 0;
		ZeroMemory(m_cExchangeItemName[i], sizeof(m_cExchangeItemName[i]));
	}
	m_iExchangeComplete = 0;
	m_bIsExchangeConfirm = FALSE;
	m_iExchangeCount = 0;
	m_iAlterItemDropIndex = -1;
	m_iQuest = NULL;
	m_iQuestID = NULL;
	m_iAskedQuest = NULL;
	m_iCurQuestCount = NULL;
	m_iQuestRewardType = NULL;
	m_iQuestRewardAmount = NULL;
	m_iContribution = NULL;
	m_bQuestMatchFlag_Loc = FALSE;
	m_bIsQuestCompleted = FALSE;
	m_cHeroArmourBonus = 0;
	m_bIsNeutral = FALSE;
	m_bIsObserverMode = FALSE;
	m_iSpecialEventID = 200081;
	m_iSpecialWeaponEffectType = 0;
	m_iSpecialWeaponEffectValue = 0;
	m_iAddHP = m_iAddSP = m_iAddMP = 0;
	m_iAddAR = m_iAddPR = m_iAddDR = 0;
	m_iAddAbsPD = m_iAddAbsMD = 0;
	m_iAddCD = m_iAddExp = m_iAddGold = 0;
	m_iSpecialAbilityTime = DEF_SPECABLTYTIMESEC;
	m_iSpecialAbilityType = NULL;
	m_bIsSpecialAbilityEnabled = FALSE;
	m_iSpecialAbilityLastSec = 0;
	m_iSpecialAbilityEquipPos = 0;
	m_iMoveMsgRecvCount = 0;
	m_iAttackMsgRecvCount = 0;
	m_iRunMsgRecvCount = 0;
	m_iSkillMsgRecvCount = 0;
	m_bIsAdminCreateItemEnabled = FALSE; // new
	m_bIsAdminCommandEnabled = TRUE;
	m_iAutoExpAmount = 0;
	m_iWarContribution = 0;
	m_dwMoveLAT = m_dwRunLAT = m_dwAttackLAT = 0;
	m_dwInitCCTimeRcv = 0;
	m_dwInitCCTime = 0;
	ZeroMemory(m_cLockedMapName, sizeof(m_cLockedMapName));
	strcpy(m_cLockedMapName, "NONE");
	m_iLockedMapTime = NULL;
	// <MISSING_VARIABLE_HERE> 0F1Ch
	m_iCrusadeDuty  = NULL;
	m_dwCrusadeGUID = NULL;
	for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++) {
		m_stCrusadeStructureInfo[i].cType = NULL;
		m_stCrusadeStructureInfo[i].cSide = NULL;
		m_stCrusadeStructureInfo[i].sX = NULL;
		m_stCrusadeStructureInfo[i].sY = NULL;
	}	
	m_iCSIsendPoint = NULL;
	m_bIsSendingMapStatus = FALSE;
	ZeroMemory(m_cSendingMapName, sizeof(m_cSendingMapName));
	m_iConstructionPoint = NULL;
	ZeroMemory(m_cConstructMapName, sizeof(m_cConstructMapName));
	m_iConstructLocX = m_iConstructLocY = -1;
	// <MISSING_VARIABLE_HERE> 165Ch	
	m_iPartyID = 0;
	m_iPartyStatus = 0;
	m_iReqJoinPartyClientH = 0;	
	ZeroMemory(m_cReqJoinPartyName,sizeof(m_cReqJoinPartyName));
	m_dwLastActionTime = 0; // new
	m_bIsCheckingWhisperPlayer = FALSE;
	// <MISSING_VARIABLE_HERE> 1660h
	m_iGizonItemUpgradeLeft = 0;
	// <MISSING_VARIABLE_HERE> 168Ch
	// <MISSING_VARIABLE_HERE> 1684h
	// <MISSING_VARIABLE_HERE> 1688h
	m_bIsMoveBlocked = FALSE;
	// <MISSING_VARIABLE_HERE> 1694h
	m_bIsOwnLocation = FALSE;
	m_pIsProcessingAllowed = FALSE;
	// <MISSING_VARIABLE_HERE> 16A4h
	m_bIsInsideWarehouse = FALSE;
	// <MISSING_VARIABLE_HERE> 16B0h	
	m_bIsAdminOrderGoto = FALSE;
	m_bIsInsideEnemyBuilding = FALSE; // new
	m_bIsImpossibleZone = FALSE; // rename

	///////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////Modifications////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	//m_iHitRatio_ItemEffect_SM = 0;
	//m_iHitRatio_ItemEffect_L  = 0;
	m_cAura = 0;
	m_dwLogoutHackCheck = 0;
	m_iAddTransMana = 0;
	m_iAddChargeCritical = 0;
	m_bInhibition = FALSE;
	m_iPenaltyBlockYear = m_iPenaltyBlockMonth = m_iPenaltyBlockDay = 0; // v1.4
	ZeroMemory(m_cExchangeName, sizeof(m_cExchangeName));			// 교환할 대상의 이름 
	ZeroMemory(m_cExchangeItemName, sizeof(m_cExchangeItemName));	// 교환할 아이템 이름 	
	m_dwHeldenianGUID = NULL;
	m_bIsInsideWizardTower = FALSE;
	m_cHeroArmorBonus = 0;
	m_bMagicConfirm = FALSE;
	m_bMagicItem = FALSE;
	m_iSpellCount = 0;
	m_bMagicPauseTime = FALSE;
	m_bIsClientConnected = FALSE;

	m_bIsExchangeMode = FALSE;

	//50Cent - Repair All
    totalItemRepair = 0;
    for(i = 0; i < DEF_MAXITEMS; i++)
        m_stRepairAll[i].index = m_stRepairAll[i].price = 0;
	///////////////////////////////////////////////////////////////////////////////////////
}

CClient::~CClient()
{
 int i;
	
	if (m_pXSock != NULL) delete m_pXSock;
	for (i = 0; i < DEF_MAXITEMS; i++)
		if (m_pItemList[i] != NULL) {
			delete m_pItemList[i];
			m_pItemList[i] = NULL;
		}
	for(i = 0; i < DEF_MAXBANKITEMS; i++)
		if (m_pItemInBankList[i] != NULL) {
			delete m_pItemInBankList[i];
			m_pItemInBankList[i]=NULL;
		}
}

int CClient::bCreateNewParty()
{
 int i;

	if (m_iPartyRank != -1) return 0;
	m_iPartyRank = 0;
	m_iPartyMemberCount = 0;
	m_iPartyGUID = (rand() % 999999) + timeGetTime();
	for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) {
		m_stPartyMemberName[i].iIndex = 0;
		ZeroMemory(m_stPartyMemberName[i].cName, sizeof(m_stPartyMemberName[i].cName));
	}
	return 1;
}
