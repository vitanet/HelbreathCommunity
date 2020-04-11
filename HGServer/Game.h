// Game.h: interface for the CGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAME_H__C3D29FC5_755B_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_GAME_H__C3D29FC5_755B_11D2_A8E6_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#pragma warning( disable: 4700 4305 4309)
/*
warning C4305: '=' : truncation from 'const int' to 'short'
warning C4309: '=' : truncation of constant value
warning C4700: local variable 'i' used without having been initialized
*/
#endif

#include <windows.h>
#include <winbase.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <memory.h>
#include <direct.h>
#include <vector>

#include "winmain.h"
#include "StrTok.h"
#include "Xsocket.h"
#include "Client.h"
#include "Npc.h"
#include "Map.h"
#include "ActionID.h"
#include "UserMessages.h"
#include "NetMessages.h"
#include "MessageIndex.h"
#include "Misc.h"
#include "Msg.h"
#include "Magic.h"
#include "Skill.h"
#include "DynamicObject.h"
#include "DelayEvent.h"
#include "Version.h"
#include "Fish.h"
#include "DynamicObject.h"
#include "DynamicObjectID.h"
#include "Potion.h"
#include "Mineral.h"
#include "Quest.h"
#include "BuildItem.h"
#include "Teleport.h"
#include "TeleportLoc.h"
#include "GlobalDef.h"
#include "TempNpcItem.h"
#include "GuildsMan.h"
#include "Party.h"
#include "Heldenian.h"
#include "Apocalypse.h"
#include "Crusade.h"

class CGame
{

public:

	CGame(HWND hWnd);
	virtual ~CGame();

	BOOL bAccept(class XSocket* pXSock);
	void DisplayInfo(HDC hdc);
	BOOL bInit();
	void Quit();
	void OnTimer(char cType);
	void ParseCommand(char* pMsg);
	void OnKeyUp(WPARAM wParam, LPARAM lParam);
	void OnKeyDown(WPARAM wParam, LPARAM lParam);
	BOOL bOnClose();
	void OnGateSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void OnMainLogSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void OnSubLogSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void OnClientSocketEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void OnStartGameSignal();

	//50Cent - Capture The Flag
	void StartCaptureTheFlag();
	void RequestCheckFlag(int iClientH);
	BOOL bCheckIfIsFlagCarrier(int iClientH);
	void SetFlagCarrierFlag(int iClientH, BOOL bFlagMode);
	void EndCaptureTheFlag(int iOption);
	void _CreateCTFGUID(int iWinnerSide);

	//50Cent - Capture The Flag
	BOOL m_bIsCTFMode;
	short m_sFlagCountWin;
	short m_sElvineFlagCount;
	short m_sAresdenFlagCount;
	BOOL m_bIsElvineFlagCaptured;
	BOOL m_bIsAresdenFlagCaptured;
	int m_iFlagCarrierIndex;

	char m_cGameServerAddrInternal[16];
	char m_cGameServerAddrExternal[16];
	int  m_iGameServerMode;
	char m_cGameServerAddr[16];
	int  m_iGameServerPort;

	class CMap* m_pMapList[DEF_MAXMAPS];

private:

	void CheckDenialServiceAttack(int iClientH, DWORD dwClientTime);

	//50Cent - Repair All
	void RequestRepairAllItemsHandler(int iClientH);
	void RequestRepairAllItemsDeleteHandler(int iClientH, int index);
	void RequestRepairAllItemsConfirmHandler(int iClientH);

	void AdminOrder_BanPj(int iClientH, char* pData, DWORD dwMsgSize);

	void HeldenianStartWarNow();
	void HeldenianEndWarNow();
	void HeldenianVictoryNow(int iSide);

	//wanted system
	void SetWantedFlag(short sOwnerH, char cOwnerType, bool iStatus);

	BOOL  iUpgradeHeroCapeRequirements(int iClientH, int iItemIndex);
	int  HeroItemChecker(int iItemID, short sEnemyKill, char cContribution, char cSide);

	// KLKS clean tiles

	void ClearMap();
	void RequestHeldenianScroll(int iClientH, char* pData, DWORD dwMsgSize);
	char cGetNextMoveArea(short sOwnerH, short sX, short sY, short dstX, short dstY, char cMapIndex, char cTurn, int* pError, char cArea);
	BOOL bGetNpcMovementArea(short sOwnerH, short pX, short pY, char cMapIndex, char cArea);
	BOOL bGetEmptyArea(short sOwnerH, char cMapIndex, short pX, short pY, char cArea);
	void CheckHeldenianResultCalculation(int iClientH);
	BOOL MobBossGenerator(int iMapIndex);

	void DoAbaddonThunderDamageHandler(char cMapIndex);
	void SendThunder(int iClient, short sX, short sY, short sV3, short sV4);


	void bReadScheduleConfigFile(char* pFn);
	void bReadApocalypseGUIDFile(char* cFn);

	void _CreateApocalypseGUID(DWORD dwApocalypseGUID);
	void LocalEndApocalypse();
	void LocalStartApocalypse(DWORD dwApocalypseGUID);
	void GlobalEndApocalypseMode(int iClientH);
	void GlobalStartApocalypseMode(int iClientH, int iMode); // Changed by SNOOPY



	// SNOOPY: Added Apocalypse functions
	void GenerateApocalypseBoss(int MapIndex);
	void OpenCloseApocalypseGate();
	void Notify_ApocalypseGateState(int iClientH);
	void Use_ApocalypseGate(int iClientH);
	void ForceOpen_ApocalypseGate();
	void ForceClose_ApocalypseGate();
	void Open_EmptyMap_Gate(int MapIndex);
	void GenerateSlime(int MapIndex);

	int iGetPlayerStatus(int iClientH, short sOwnerH);

	void AdminOrder_GetFightzoneTicket(int iClientH);


	void LocalStartHeldenianMode(short sV1, short sV2, DWORD dwHeldenianGUID);
	void GlobalStartHeldenianMode();

	void HeldenianWarStarter();
	BOOL UpdateHeldenianStatus(int iClientH);
	void _CreateHeldenianGUID(DWORD dwHeldenianGUID, int iWinnerSide);
	void ManualStartHeldenianMode(int iClientH, char* pData, DWORD dwMsgSize);
	void ManualEndHeldenianMode(int iClientH, char* pData, DWORD dwMsgSize);


	BOOL _bCheckCharacterData(int iClientH);
	void _bDecodeNpcItemConfigFileContents(char* pData, DWORD dwMsgSize);
	void GlobalUpdateConfigs(char cConfigType);
	void LocalUpdateConfigs(char cConfigType);
	void GlobalEndHeldenianMode();
	void LocalEndHeldenianMode(DWORD dwHeldenianGUID, int iWinner);


	void RemoveEventNpc(int iNpcH);
	void RemoveOccupyFlags(int iMapIndex);

	void RequestHeldenianTeleportList(int iClientH, char* pData, DWORD dwMsgSize);
	void RequestHeldenianTeleportNow(int iClientH, char* pData, DWORD dwMsgSize);

	BOOL bCheckHeldenianMap(int sAttackerH, char cType);
	void SetHeroFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void SetInhibitionCastingFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void bCalculateEnduranceDecrement(short sTargetH, short sAttackerH, char cTargetType, int iArmorType);
	char _cCheckHeroItemEquipped(int iClientH);
	BOOL bPlantSeedBag(int iMapIndex, int dX, int dY, int iItemEffectValue1, int iItemEffectValue2, int iClientH);
	void _CheckFarmingAction(short sAttackerH, short sTargetH, BOOL bType);



	void bReadHeldenianGUIDFile(char* cFn);


	// KLKS clean tiles
	void AdminOrder_CleanMap(int iClientH, char* pData, DWORD dwMsgSize);

	// Lists
	BOOL bReadBannedListConfigFile(char* pFn);
	BOOL bReadAdminListConfigFile(char* pFn);



	// Crusade
	void ManualEndCrusadeMode(int iWinnerSide); // 2.17 (x) 2.14 ( )
	void CrusadeWarStarter();
	void bReadCrusadeGUIDFile(char* cFn);
	void _CreateCrusadeGUID(DWORD dwCrusadeGUID, int iWinnerSide);
	void GlobalStartCrusadeMode();
	void GSM_SetGuildTeleportLoc(int iGuildGUID, int dX, int dY, char* pMapName);
	void SyncMiddlelandMapInfo();
	void _SendMapStatus(int iClientH);
	void MapStatusHandler(int iClientH, int iMode, char* pMapName);
	void SelectCrusadeDutyHandler(int iClientH, int iDuty);
	void RequestSummonWarUnitHandler(int iClientH, int dX, int dY, char cType, char cNum, char cMode);
	void RequestGuildTeleportHandler(int iClientH);
	void RequestSetGuildTeleportLocHandler(int iClientH, int dX, int dY, int iGuildGUID, char* pMapName);
	void MeteorStrikeHandler(int iMapIndex);
	void _LinkStrikePointMapIndex();
	void MeteorStrikeMsgHandler(char cAttackerSide);
	void _NpcBehavior_GrandMagicGenerator(int iNpcH);
	void CollectedManaHandler(WORD wAresdenMana, WORD wElvineMana);
	void SendCollectedMana();
	void _GrandMagicLaunchMsgSend(int iType, char cAttackerSide);
	void GrandMagicResultHandler(char* cMapName, int iCrashedStructureNum, int iStructureDamageAmount, int iCasualities, int iActiveStructure, int iTotalStrikePoints, char* cData);
	void CalcMeteorStrikeEffectHandler(int iMapIndex);
	void DoMeteorStrikeDamageHandler(int iMapIndex);
	void RequestSetGuildConstructLocHandler(int iClientH, int dX, int dY, int iGuildGUID, char* pMapName);
	void GSM_SetGuildConstructLoc(int iGuildGUID, int dX, int dY, char* pMapName);
	void GSM_ConstructionPoint(int iGuildGUID, int iPoint);
	void CheckCommanderConstructionPoint(int iClientH);
	void bReadCrusadeStructureConfigFile(char* cFn);

	void LocalEndCrusadeMode(int iWinnerSide);
	void LocalStartCrusadeMode(DWORD dwGuildGUID);
	void CheckCrusadeResultCalculation(int iClientH);
	BOOL _bNpcBehavior_Detector(int iNpcH);
	BOOL _bNpcBehavior_ManaCollector(int iNpcH);
	BOOL __bSetConstructionKit(int iMapIndex, int dX, int dY, int iType, int iTimeCost, int iClientH);



	// Acidx commands

	void AdminOrder_Pushplayer(int iClientH, char* pData, DWORD dwMsgSize);

	void AdminOrder_CheckRep(int iClientH, char* pData, DWORD dwMsgSize);


	void ApplyCombatKilledPenalty(int iClientH, int cPenaltyLevel, BOOL bIsSAattacked);


	void AdminOrder_ClearNpc(int iClientH);

	// Settings.cfg
	BOOL bReadSettingsConfigFile(char* cFn);



	// Daryl - AdminSettings.cfg
	BOOL bReadAdminSetConfigFile(char* cFn);


	// Hack Checks
	void bCheckClientMoveFrequency(int iClientH, DWORD dwClientTime);
	void bCheckClientMagicFrequency(int iClientH, DWORD dwClientTime);
	void bCheckClientAttackFrequency(int iClientH, DWORD dwClientTime);



	//Hypnotoad functions
	void SetDefenseShieldFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void SetMagicProtectionFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void SetProtectionFromArrowFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void SetIllusionMovementFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void SetIllusionFlag(short sOwnerH, char cOwnerType, BOOL bStatus);

	void RefreshDeathmatch(int iClientH);

	void SendThunders();

	void RequestChangePlayMode(int iClientH);
	void GetHeroMantleHandler(int iClientH, int iItemID, char* pString);

	void bCheckMagicInt(int iClientH);
	BOOL bChangeState(char cStateChange, char* cStr, char* cVit, char* cDex, char* cInt, char* cMag, char* cChar);
	void StateChangeHandler(int iClientH, char* pData, DWORD dwMsgSize);

	void SetPoisonFlag(short sOwnerH, char cOwnerType, BOOL bStatus);

	void AdminOrder_EnableAdminCommand(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_CreateItem(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_Summon(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_SummonAll(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_SummonPlayer(int iClientH, char* pData, DWORD dwMsgSize);

	void AdminOrder_UnsummonAll(int iClientH);
	void AdminOrder_SetAttackMode(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_SummonDemon(int iClientH);
	void AdminOrder_SetInvi(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_Polymorph(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_GetNpcStatus(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_CheckIP(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_CreateFish(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_Teleport(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_ReserveFightzone(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_CloseConn(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_CallGuard(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_DisconnectAll(int iClientH, char* pData, DWORD dwMsgSize);

	BOOL bCopyItemContents(class CItem* pOriginal, class CItem* pCopy);
	int  iGetMapLocationSide(char* pMapName);
	void ChatMsgHandlerGSM(int iMsgType, int iV1, char* pName, char* pData, DWORD dwMsgSize);
	void RemoveClientShortCut(int iClientH);
	BOOL bAddClientShortCut(int iClientH);

	void GSM_RequestFindCharacter(WORD wReqServerID, WORD wReqClientH, char* pName, char* pFinder); // New 16/05/2001 Changed
	void ServerStockMsgHandler(char* pData);
	void SendStockMsgToGateServer();
	BOOL bStockMsgToGateServer(char* pData, DWORD dwSize);
	void RequestHelpHandler(int iClientH);

	void CheckConnectionHandler(int iClientH, char* pData);

	void SetAngelFlag(short sOwnerH, char cOwnerType, int iStatus, int iTemp);
	void GetAngelHandler(int iClientH, char* pData, DWORD dwMsgSize);

	void ReqCreateCraftingHandler(int iClientH, char* pData);

	void RequestPurchaseItemHandler2(int iClientH, char* pItemName, int iNum);



	void PlayerOrder_ChangeCity(int iClientH, BOOL bChange);
	void RequestRango(int iClientH, int iObjectID);


	void AgingMapSectorInfo();
	void UpdateMapSectorInfo();
	BOOL bGetItemNameWhenDeleteNpc(int& iItemID, short sNpcType);
	int iGetItemWeight(class CItem* pItem, int iCount);
	void CancelQuestHandler(int iClientH);
	void ActivateSpecialAbilityHandler(int iClientH);
	void EnergySphereProcessor(BOOL bIsAdminCreate = FALSE, int iClientH = NULL);
	BOOL bCheckEnergySphereDestination(int iNpcH, short sAttackerH, char cAttackerType);
	void JoinPartyHandler(int iClientH, int iV1, char* pMemberName);
	void CreateNewPartyHandler(int iClientH);
	void _DeleteRandomOccupyFlag(int iMapIndex);
	void RequestSellItemListHandler(int iClientH, char* pData);
	void RequestRestartHandler(int iClientH);
	int iRequestPanningMapDataRequest(int iClientH, char* pData);
	void GetMagicAbilityHandler(int iClientH);
	void Effect_Damage_Spot_DamageMove(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sAtkX, short sAtkY, short sV1, short sV2, short sV3, BOOL bExp, int iAttr);
	void _TamingHandler(int iClientH, int iSkillNum, char cMapIndex, int dX, int dY);
	void RequestCheckAccountPasswordHandler(char* pData, DWORD dwMsgSize);
	int _iTalkToNpcResult_Guard(int iClientH, int* pQuestType, int* pMode, int* pRewardType, int* pRewardAmount, int* pContribution, char* pTargetName, int* pTargetType, int* pTargetCount, int* pX, int* pY, int* pRange);
	void SetIceFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void _bDecodeNoticementFileContents(char* pData, DWORD dwMsgSize);
	void RequestNoticementHandler(int iClientH, char* pData);
	void _AdjustRareItemValue(class CItem* pItem);
	BOOL _bCheckDupItemID(class CItem* pItem);
	void _bDecodeDupItemIDFileContents(char* pData, DWORD dwMsgSize);
	void NpcDeadItemGenerator(int iNpcH, short sAttackerH, char cAttackerType);
	int  iGetPlayerABSStatus(int iWhatH, int iRecvH);

	char _cGetSpecialAbility(int iKindSA);
	void BuildItemHandler(int iClientH, char* pData);
	BOOL _bDecodeBuildItemConfigFileContents(char* pData, DWORD dwMsgSize);
	BOOL _bCheckSubLogSocketIndex();
	void _CheckGateSockConnection();
	void OnSubLogRead(int iIndex);

	void _CheckStrategicPointOccupyStatus(char cMapIndex);
	void GetMapInitialPoint(int iMapIndex, short* pX, short* pY, char* pPlayerLocation = NULL);
	int  iGetMaxHP(int iClientH, BOOL bBloodEffect = TRUE);
	int  iGetMaxMP(int iClientH);
	int  iGetMaxSP(int iClientH);
	void _ClearQuestStatus(int iClientH);
	BOOL _bCheckItemReceiveCondition(int iClientH, class CItem* pItem);
	void SendItemNotifyMsg(int iClientH, WORD wMsgType, class CItem* pItem, int iV1);

	int _iTalkToNpcResult_WTower(int iClientH, int* pQuestType, int* pMode, int* pRewardType, int* pRewardAmount, int* pContribution, char* pTargetName, int* pTargetType, int* pTargetCount, int* pX, int* pY, int* pRange);
	int _iTalkToNpcResult_WHouse(int iClientH, int* pQuestType, int* pMode, int* pRewardType, int* pRewardAmount, int* pContribution, char* pTargetName, int* pTargetType, int* pTargetCount, int* pX, int* pY, int* pRange);
	int _iTalkToNpcResult_BSmith(int iClientH, int* pQuestType, int* pMode, int* pRewardType, int* pRewardAmount, int* pContribution, char* pTargetName, int* pTargetType, int* pTargetCount, int* pX, int* pY, int* pRange);
	int _iTalkToNpcResult_GShop(int iClientH, int* pQuestType, int* pMode, int* pRewardType, int* pRewardAmount, int* pContribution, char* pTargetName, int* pTargetType, int* pTargetCount, int* pX, int* pY, int* pRange);
	int _iTalkToNpcResult_GuildHall(int iClientH, int* pQuestType, int* pMode, int* pRewardType, int* pRewardAmount, int* pContribution, char* pTargetName, int* pTargetType, int* pTargetCount, int* pX, int* pY, int* pRange);
	BOOL _bCheckIsQuestCompleted(int iClientH);
	void _CheckQuestEnvironment(int iClientH);
	void _SendQuestContents(int iClientH);
	void QuestAcceptedHandler(int iClientH);
	BOOL _bDecodeQuestConfigFileContents(char* pData, DWORD dwMsgSize);

	void CancelExchangeItem(int iClientH);
	BOOL bAddItem(int iClientH, class CItem* pItem, char cMode);
	void ConfirmExchangeItem(int iClientH);
	void SetExchangeItem(int iClientH, int iItemIndex, int iAmount);
	void ExchangeItemHandler(int iClientH, short sItemIndex, int iAmount, short dX, short dY, WORD wObjectID, char* pItemName);

	void _BWM_Command_Shutup(char* pData);
	void _BWM_Init(int iClientH, char* pData);
	void CheckUniqueItemEquipment(int iClientH);
	void _SetItemPos(int iClientH, char* pData);

	BOOL _bDecodeOccupyFlagSaveFileContents(char* pData, DWORD dwMsgSize);
	void GetOccupyFlagHandler(int iClientH);
	int  _iComposeFlagStatusContents(char* pData);
	void SetSummonMobAction(int iClientH, int iMode, DWORD dwMsgSize, char* pData = NULL);
	BOOL __bSetOccupyFlag(char cMapIndex, int dX, int dY, int iSide, int iEKNum, int iClientH, BOOL bAdminFlag);
	BOOL _bDepleteDestTypeItemUseEffect(int iClientH, int dX, int dY, short sItemIndex, short sDestItemID);
	void SetDownSkillIndexHandler(int iClientH, int iSkillIndex);
	int iGetComboAttackBonus(int iSkill, int iComboCount);
	int  _iGetWeaponSkillType(int iClientH);
	void CheckFireBluring(char cMapIndex, int sX, int sY);
	void NpcTalkHandler(int iClientH, int iWho);
	void bDeleteMineral(int iIndex);
	void _CheckMiningAction(int iClientH, int dX, int dY);
	int iCreateMineral(char cMapIndex, int tX, int tY, char cLevel);
	void MineralGenerator();
	void LocalSavePlayerData(int iClientH);
	BOOL _bDecodePotionConfigFileContents(char* pData, DWORD dwMsgSize);
	void ReqCreatePotionHandler(int iClientH, char* pData);
	void _CheckAttackType(int iClientH, short* spType);

	void ForceDisconnectAccount(char* pAccountName, WORD wCount);
	void NpcRequestAssistance(int iNpcH);
	void ToggleSafeAttackModeHandler(int iClientH);
	void SetBerserkFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	void SpecialEventHandler();
	int iGetNpcRelationship_SendEvent(int iNpcH, int iOpponentH);
	int _iForcePlayerDisconect(int iNum);
	int iGetMapIndex(char* pMapName);
	int iGetNpcRelationship(int iWhatH, int iRecvH);
	int iGetPlayerRelationship(int iClientH, int iOpponentH);
	int iGetWhetherMagicBonusEffect(short sType, char cWheatherStatus);
	void WeatherProcessor();
	int _iCalcPlayerNum(char cMapIndex, short dX, short dY, char cRadius);
	void FishGenerator();
	void ReqGetFishThisTimeHandler(int iClientH);
	void FishProcessor();
	int iCheckFish(int iClientH, char cMapIndex, short dX, short dY);
	BOOL bDeleteFish(int iHandle, int iDelMode);
	int  iCreateFish(char cMapIndex, short sX, short sY, short sDifficulty, class CItem* pItem, int iDifficulty, DWORD dwLastTime);

	// v1.4311-3 변경 함수 선언  Expire -> Ban 으로 
	void UserCommand_BanGuildsman(int iClientH, char* pData, DWORD dwMsgSize);
	// v1.4311-3 추가 함수 선언 이벤트를 대비하여 운영자가 사투장을 예약해놓는다.
	long iGetExpLevel(unsigned long iExp);
	
	void ResponseSavePlayerDataReplyHandler(char* pData, DWORD dwMsgSize);
	void NoticeHandler();
	BOOL bReadNotifyMsgListFile(char* cFn);
	void SetPlayerReputation(int iClientH, char* pMsg, char cValue, DWORD dwMsgSize);
	void ShutUpPlayer(int iClientH, char* pMsg, DWORD dwMsgSize);
	void CheckDayOrNightMode();
	
	BOOL bCheckResistingPoisonSuccess(short sOwnerH, char cOwnerType);
	void PoisonEffect(int iClientH, int iV1);
	void bSetNpcAttackMode(char* cName, int iTargetH, char cTargetType, BOOL bIsPermAttack);
	BOOL _bGetIsPlayerHostile(int iClientH, int sOwnerH);
	BOOL bAnalyzeCriminalAction(int iClientH, short dX, short dY, BOOL bIsCheck = FALSE);

	int _iGetPlayerNumberOnSpot(short dX, short dY, char cMapIndex, char cRange);
	void CalcTotalItemEffect(int iClientH, int iEquipItemID, BOOL bNotify = TRUE);
	
	void GetPlayerProfile(int iClientH, char* pMsg, DWORD dwMsgSize);
	void SetPlayerProfile(int iClientH, char* pMsg, DWORD dwMsgSize);
	void ToggleWhisperPlayer(int iClientH, char* pMsg, DWORD dwMsgSize);
	void CheckAndNotifyPlayerConnection(int iClientH, char* pMsg, DWORD dwSize);
	int iCalcTotalWeight(int iClientH);
	void ReqRepairItemCofirmHandler(int iClientH, char cItemID, char* pString);
	void ReqRepairItemHandler(int iClientH, char cItemID, char cRepairWhom, char* pString);
	void ReqSellItemConfirmHandler(int iClientH, char cItemID, int iNum, char* pString);
	void ReqSellItemHandler(int iClientH, char cItemID, char cSellToWhom, int iNum, char* pItemName);
	void UseSkillHandler(int iClientH, int iV1, int iV2, int iV3);
	int  iCalculateUseSkillItemEffect(int iOwnerH, char cOwnerType, char cOwnerSkill, int iSkillNum, char cMapIndex, int dX, int dY);
	void ClearSkillUsingStatus(int iClientH);
	void DynamicObjectEffectProcessor();
	int _iGetTotalClients();
	void SendObjectMotionRejectMsg(int iClientH);
	void SetInvisibilityFlag(short sOwnerH, char cOwnerType, BOOL bStatus);
	BOOL bRemoveFromDelayEventList(int iH, char cType, int iEffectType);
	void DelayEventProcessor();
	BOOL bRegisterDelayEvent(int iDelayType, int iEffectType, DWORD dwLastTime, int iTargetH, char cTargetType, char cMapIndex, int dX, int dY, int iV1, int iV2, int iV3);
	int iGetFollowerNumber(short sOwnerH, char cOwnerType);
	int  _iCalcSkillSSNpoint(int iLevel);

	void bCheckTotalSkillMasteryPoints(int iClientH, int iSkill);
	BOOL bSetItemToBankItem(int iClientH, class CItem* pItem);
	void NpcMagicHandler(int iNpcH, short dX, short dY, short sType);
	BOOL bCheckResistingTremorSuccess(short sTargetH, char cTargetType);
	BOOL bCheckResistingIceSuccess(char cAttackerDir, short sTargetH, char cTargetType, int iHitRatio);
	BOOL bCheckResistingMagicSuccess(char cAttackerDir, short sTargetH, char cTargetType, int iHitRatio);
	void Effect_SpUp_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void Effect_SpDown_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void Effect_HpUp_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3);
	void Effect_Damage_Spot(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sV1, short sV2, short sV3, BOOL bExp, int iAttr = NULL);
	void Effect_Damage_Spot_Type2(short sAttackerH, char cAttackerType, short sTargetH, char cTargetType, short sAtkX, short sAtkY, short sV1, short sV2, short sV3, BOOL bExp, int iAttr);
	void UseItemHandler(int iClientH, short sItemIndex, short dX, short dY, short sDestItemID);
	void NpcBehavior_Stop(int iNpcH);
	void ItemDepleteHandler(int iClientH, short sItemIndex, BOOL bIsUseItemResult, BOOL bIsItemUsed);
	int _iGetArrowItemIndex(int iClientH);
	void RequestFullObjectData(int iClientH, char* pData);
	void DeleteNpc(int iNpcH);
	void CalcNextWayPointDestination(int iNpcH);
	void MobGenerator();
	void CalculateSSN_SkillIndex(int iClientH, short sSkillIndex, int iValue);
	void CalculateSSN_ItemIndex(int iClientH, short sWeaponIndex, int iValue);
	void CheckDynamicObjectList();
	int  iAddDynamicObjectList(short sOwner, char cOwnerType, short sType, char cMapIndex, short sX, short sY, DWORD dwLastTime, int iV1 = NULL);
	int _iCalcMaxLoad(int iClientH);
	void GetRewardMoneyHandler(int iClientH);
	void _PenaltyItemDrop(int iClientH, int iTotal, BOOL bIsSAattacked = FALSE);
	void EnemyKillRewardHandler(int iAttackerH, int iClientH);
	void PK_KillRewardHandler(short sAttackerH, short sVictumH);
	void ApplyPKpenalty(short sAttackerH, short sVictumH);
	BOOL bSetItemToBankItem(int iClientH, short sItemIndex);
	void RequestRetrieveItemHandler(int iClientH, char* pData);
	void RequestCivilRightHandler(int iClientH, char* pData);
	BOOL bCheckLimitedUser(int iClientH);
	void LevelUpSettingsHandler(int iClientH, char* pData, DWORD dwMsgSize);
	// v1.4311-3 선언 함수  사투장 예약 함수 선언 FightzoneReserveHandler
	void FightzoneReserveHandler(int iClientH, char* pData, DWORD dwMsgSize);
	void bCheckLevelUp(int iClientH);

	long iGetLevelExp(int iLevel);
	void TimeManaPointsUp(int iClientH);
	void TimeStaminarPointsUp(int iClientH);

	bool SpecialWeapon_DS(int iClientH);
	BOOL __bReadMapInfo(int iMapIndex);
	BOOL bBankItemToPlayer(int iClientH, short sItemIndex);
	BOOL bPlayerItemToBank(int iClientH, short sItemIndex);
	int  _iGetSkillNumber(char* pSkillName);
	void TrainSkillResponse(BOOL bSuccess, int iClientH, int iSkillNum, int iSkillLevel);
	int _iGetMagicNumber(char* pMagicName, int* pReqInt, int* pCost);
	void RequestStudyMagicHandler(int iClientH, char* pName, BOOL bSucces = FALSE, BOOL bIsPurchase = TRUE);
	BOOL _bDecodeSkillConfigFileContents(char* pData, DWORD dwMsgSize);
	BOOL _bDecodeMagicConfigFileContents(char* pData, DWORD dwMsgSize);
	void ReleaseFollowMode(short sOwnerH);
	BOOL bSetNpcFollowMode(char* pName, char* pFollowName, char cFollowOwnerType);
	void RequestTeleportHandler(int iClientH, char* pData, char* cMapName = NULL, int dX = -1, int dY = -1);
	void PlayerMagicHandler(int iClientH, int dX, int dY, short sType, BOOL bItemEffect = FALSE, int iV1 = NULL);
	int  iClientMotion_Magic_Handler(int iClientH, short sX, short sY, char cDir);
	void SendMsgToGateServer(DWORD dwMsg, int iClientH, char* pData = NULL);
	void OnGateRead();

	void ToggleCombatModeHandler(int iClientH);
	void GuildNotifyHandler(char* pData, DWORD dwMsgSize);
	void SendGuildMsg(int iClientH, WORD wNotifyMsgType, short sV1, short sV2, char* pString);

	void TimeHitPointsUp(int iClientH);

	int iDice(int iThrow, int iRange);
	BOOL _bInitNpcAttr(class CNpc* pNpc, char* pNpcName, short sClass, char cSA);
	BOOL _bDecodeNpcConfigFileContents(char* pData, DWORD dwMsgSize);
	void ReleaseItemHandler(int iClientH, short sItemIndex, BOOL bNotice);
	void ClientKilledHandler(int iClientH, int iAttackerH, char cAttackerType, short sDamage);
	int  SetItemCount(int iClientH, char* pItemName, DWORD dwCount);
	int  SetItemCount(int iClientH, int iItemIndex, DWORD dwCount);
	DWORD dwGetItemCount(int iClientH, char* pName);
	void DismissGuildRejectHandler(int iClientH, char* pName);
	void DismissGuildApproveHandler(int iClientH, char* pName);
	void JoinGuildRejectHandler(int iClientH, char* pName);
	void JoinGuildApproveHandler(int iClientH, char* pName);
	void SendNotifyMsg(int iFromH, int iToH, WORD wMsgType, DWORD sV1, DWORD sV2, DWORD sV3, char* pString, DWORD sV4 = NULL, DWORD sV5 = NULL, DWORD sV6 = NULL, DWORD sV7 = NULL, DWORD sV8 = NULL, DWORD sV9 = NULL, char* pString2 = NULL);
	void GiveItemHandler(int iClientH, short sItemIndex, int iAmount, short dX, short dY, WORD wObjectID, char* pItemName);
	void RequestPurchaseItemHandler(int iClientH, char* pItemName, int iNum);
	void ResponseDisbandGuildHandler(char* pData, DWORD dwMsgSize);
	void RequestDisbandGuildHandler(int iClientH, char* pData, DWORD dwMsgSize);
	void RequestCreateNewGuildHandler(int iClientH, char* pData, DWORD dwMsgSize);
	void ResponseCreateNewGuildHandler(char* pData, DWORD dwMsgSize);
	int  iClientMotion_Stop_Handler(int iClientH, short sX, short sY, char cDir);

	void RequestOnlines(int iClientH);

	void MultiplicadorExp(int Client, unsigned long Exp);

	BOOL bEquipItemHandler(int iClientH, short sItemIndex, BOOL bNotify = TRUE);
	BOOL _bAddClientItemList(int iClientH, class CItem* pItem, int* pDelReq);
	int  iClientMotion_GetItem_Handler(int iClientH, short sX, short sY, char cDir);
	void DropItemHandler(int iClientH, short sItemIndex, int iAmount, char* pItemName, BOOL bByPlayer = TRUE);
	void ClientCommonHandler(int iClientH, char* pData);
	BOOL __fastcall bGetMsgQuene(char* pFrom, char* pData, DWORD* pMsgSize, int* pIndex, char* pKey);
	void MsgProcess();
	BOOL __fastcall bPutMsgQuene(char cFrom, char* pData, DWORD dwMsgSize, int iIndex, char cKey);
	void NpcBehavior_Flee(int iNpcH);
	int iGetDangerValue(int iNpcH, short dX, short dY);
	void NpcBehavior_Dead(int iNpcH);
	void NpcKilledHandler(short sAttackerH, char cAttackerType, int iNpcH, short sDamage);
	int iCalculateAttackEffect(short sTargetH, char cTargetType, short sAttackerH, char cAttackerType, int tdX, int tdY, int iAttackMode, BOOL bNearAttack = FALSE, BOOL bIsDash = FALSE, BOOL bArrowUse = FALSE, BOOL bMainGaucheAttack = FALSE);

	void RemoveFromTarget(short sTargetH, char cTargetType, int iCode = NULL, int iAttackerH = NULL, char cAttackerType = 0);

	void NpcBehavior_Attack(int iNpcH);
	void TargetSearch(int iNpcH, short* pTarget, char* pTargetType);
	void NpcBehavior_Move(int iNpcH);
	BOOL bGetEmptyPosition(short* pX, short* pY, char cMapIndex);
	char cGetNextMoveDir(short sX, short sY, short dstX, short dstY, char cMapIndex, char cTurn, int* pError);
	int  iClientMotion_Attack_Handler(int iClientH, short sX, short sY, short dX, short dY, short wType, char cDir, WORD wTargetObjectID, BOOL bResponse = TRUE, BOOL bIsDash = FALSE);
	void ChatMsgHandler(int iClientH, char* pData, DWORD dwMsgSize);
	void NpcProcess();
	int bCreateNewNpc(char* pNpcName, char* pName, char* pMapName, short sClass, char cSA, char cMoveType, int* poX, int* poY, char* pWaypointList, RECT* pArea, int iSpotMobIndex, char cChangeSide, BOOL bHideGenMode, BOOL bIsSummoned = FALSE, BOOL bFirmBerserk = FALSE, BOOL bIsMaster = FALSE, int iGuildGUID = NULL);


	BOOL _bGetIsStringIsNumber(char* pStr);
	BOOL _bInitItemAttr(class CItem* pItem, char* pItemName);
	BOOL bReadProgramConfigFile(char* cFn);
	BOOL bReadProgramConfigFile2(char* cFn);

	void InitPlayerData(int iClientH, char* pData, DWORD dwSize);
	void ResponsePlayerDataHandler(char* pData, DWORD dwSize);
	BOOL bSendMsgToLS(DWORD dwMsg, int iClientH, BOOL bFlag = TRUE, char* pData = NULL);
	void OnMainLogRead();

	void CheckClientResponseTime();

	int iComposeMoveMapData(short sX, short sY, int iClientH, char cDir, char* pData);
	void SendEventToNearClient_TypeB(DWORD dwMsgID, WORD wMsgType, char cMapIndex, short sX, short sY, short sV1, short sV2, short sV3, short sV4 = NULL);
	void SendEventToNearClient_TypeA(short sOwnerH, char cOwnerType, DWORD dwMsgID, WORD wMsgType, short sV1, short sV2, short sV3);
	void DeleteClient(int iClientH, BOOL bSave, BOOL bNotify, BOOL bCountLogout = TRUE, BOOL bForceCloseConn = FALSE);
	int  iComposeInitMapData(short sX, short sY, int iClientH, char* pData);
	void RequestInitDataHandler(int iClientH, char* pData, char cKey, BOOL bIsResurrected);
	void RequestInitPlayerHandler(int iClientH, char* pData, char cKey);
	int iClientMotion_Move_Handler(int iClientH, short sX, short sY, char cDir, char cMoveType);
	void ClientMotionHandler(int iClientH, char* pData);

	void OnClientRead(int iClientH);



	// v1.4311-3 함수 선언 입장권 받는 함수 .. 
	void GetFightzoneTicketHandler(int iClientH);
	// v1.4311-3 2 시간마다 사투장 예약을 초기화 시킨다.



	void RefreshPartyStatus(int iClientH);
	void RefreshPartyCoords(int iClientH);


	void AdminOrder_BanIP(int iClientH, char* pData, DWORD dwMsgSize);

	// New 06/05/2004
	// Upgrades
	BOOL bCheckIsItemUpgradeSuccess(int iClientH, int iItemIndex, int iSomH, BOOL bBonus = FALSE);
	void RequestItemUpgradeHandler(int iClientH, int iItemIndex);



	//Party Codes
	void RequestCreatePartyHandler(int iClientH);
	void PartyOperationResultHandler(char* pData);
	void PartyOperationResult_Create(int iClientH, char* pName, int iResult, int iPartyID);
	void PartyOperationResult_Join(int iClientH, char* pName, int iResult, int iPartyID);
	void PartyOperationResult_Dismiss(int iClientH, char* pName, int iResult, int iPartyID);
	void PartyOperationResult_Delete(int iPartyID);
	void RequestJoinPartyHandler(int iClientH, char* pData, DWORD dwMsgSize);
	void RequestDismissPartyHandler(int iClientH);
	void GetPartyInfoHandler(int iClientH);
	void PartyOperationResult_Info(int iClientH, char* pName, int iTotal, char* pNameList);
	void RequestDeletePartyHandler(int iClientH);
	void RequestAcceptJoinPartyHandler(int iClientH, int iResult);
	void GetExp(int iClientH, unsigned long iExp, BOOL bIsAttackerOwn = FALSE);

	// New 07/05/2004
	// Guild Codes
	void RequestGuildNameHandler(int iClientH, int iObjectID, int iIndex);

	// Item Logs
	BOOL _bItemLog(int iAction, int iClientH, char* cName, class CItem* pItem);
	BOOL _bItemLog(int iAction, int iGiveH, int iRecvH, class CItem* pItem, BOOL bForceItemLog = FALSE);
	BOOL _bCheckGoodItem(class CItem* pItem);

	BOOL bCheckAndConvertPlusWeaponItem(int iClientH, int iItemIndex);
	void ArmorLifeDecrement(int iAttackerH, int iTargetH, char cOwnerType, int iValue);

	// MultiDrops
	BOOL bGetMultipleItemNamesWhenDeleteNpc(short sNpcType, int iProbability, int iMin, int iMax, short sBaseX, short sBaseY,
		int iItemSpreadType, int iSpreadRange,
		int* iItemIDs, POINT* BasePos, int* iNumItem);

	// Player shutup
	void GSM_RequestShutupPlayer(char* pGMName, WORD wReqServerID, WORD wReqClientH, WORD wTime, char* pPlayer);

	// PK Logs
	BOOL _bPKLog(int iAction, int iAttackerH, int iVictumH, char* pNPC);

	BOOL _bDecodeItemConfigFileContents(char* pData, DWORD dwMsgSize);
	int _iComposePlayerDataFileContents(int iClientH, char* pData);
	BOOL _bDecodePlayerDatafileContents(int iClientH, char* pData, DWORD dwSize);
	BOOL _bRegisterMap(char* pName);



	int __iSearchForQuest(int iClientH, int iWho, int* pQuestType, int* pMode, int* pRewardType, int* pRewardAmount, int* pContribution, char* pTargetName, int* pTargetType, int* pTargetCount, int* pX, int* pY, int* pRange);
	int _iTalkToNpcResult_Cityhall(int iClientH, int* pQuestType, int* pMode, int* pRewardType, int* pRewardAmount, int* pContribution, char* pTargetName, int* pTargetType, int* pTargetCount, int* pX, int* pY, int* pRange);
	void _ClearExchangeStatus(int iToH);
	int _iGetItemSpaceLeft(int iClientH);

	void AdminOrder_GoTo(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_MonsterCount(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_SetForceRecallTime(int iClientH, char* pData, DWORD dwMsgSize);
	void AdminOrder_UnsummonBoss(int iClientH);

	BOOL _bCrusadeLog(int iAction, int iClientH, int iData, char* cName);
	int iGetPlayerABSStatus(int iClientH);
	BOOL _bInitItemAttr(class CItem* pItem, int iItemID);
	void ReqCreateSlateHandler(int iClientH, char* pData);
	void SetSlateFlag(int iClientH, short sType, bool bFlag);
	void CheckForceRecallTime(int iClientH);
	void SetPlayingStatus(int iClientH);
	void ForceChangePlayMode(int iClientH, bool bNotify);

	void ShowClientMsg(int iClientH, char* pMsg);
	void RequestResurrectPlayer(int iClientH, bool bResurrect);

	void AdminOrder_SummonGuild(int iClientH, char* pData, DWORD dwMsgSize);

	void GetDkSet(int iClientH);

	void AFKChecker();


	void iRecoverFollowers(int  iClientH, BOOL bControlAll);
	void PlayerCommandAddMaster(int iClientH, char* pData, DWORD dwMsgSize);
	void PlayerOrder_DeleteGuildMaster(int iClientH, char* pData, DWORD dwMsgSize);

	void ApocalypseStarter();

	void DeathmatchGame();

	


	BOOL m_bIsGameServerRegistered;
	BOOL m_bIsSocketConnected[DEF_MAXSUBLOGSOCK];
	BOOL ReceivedAllConfig;

	int m_iNotifyCleanMap;	DWORD m_dwCleanTime;

	char m_cServerName[12];


	char m_cLogServerAddr[16];
	char m_cGateServerAddr[16];

	int  m_iLogServerPort;
	int  m_iGateServerPort;
	int  m_iWorldServerPort;

	unsigned long  m_iLimitedUserExp, m_iLevelExp20;

	class CClient* m_pClientList[DEF_MAXCLIENTS];
	class CNpc* m_pNpcList[DEF_MAXNPCS];
	
	class CNpcItem* m_pTempNpcItem[DEF_MAXNPCITEMS];
	class CDynamicObject* m_pDynamicObjectList[DEF_MAXDYNAMICOBJECTS];
	class CDelayEvent* m_pDelayEventList[DEF_MAXDELAYEVENTS];

	class CMsg* m_pMsgQuene[DEF_MSGQUENESIZE];
	int             m_iQueneHead, m_iQueneTail;
	int             m_iTotalMaps;
	class XSocket* m_pMainLogSock, * m_pGateSock;
	int				m_iGateSockConnRetryTimes;
	class CMisc     m_Misc;
	BOOL			m_bIsGameStarted;
	BOOL            m_bIsLogSockAvailable, m_bIsGateSockAvailable;
	BOOL			m_bIsItemAvailable, m_bIsBuildItemAvailable, m_bIsNpcAvailable, m_bIsMagicAvailable;
	BOOL			m_bIsSkillAvailable, m_bIsPotionAvailable, m_bIsQuestAvailable, m_bIsTeleportAvailable, m_bIsWLServerAvailable;
	class CItem* m_pItemConfigList[DEF_MAXITEMTYPES];
	class CNpc* m_pNpcConfigList[DEF_MAXNPCTYPES];
	class CMagic* m_pMagicConfigList[DEF_MAXMAGICTYPE];
	class CSkill* m_pSkillConfigList[DEF_MAXSKILLTYPE];
	class CQuest* m_pQuestConfigList[DEF_MAXQUESTTYPE];

	char            m_pMsgBuffer[DEF_MSGBUFFERSIZE + 1];

	HWND  m_hWnd;
	int   m_iTotalClients, m_iMaxClients, m_iTotalGameServerClients, m_iTotalGameServerMaxClients;
	int   m_iTotalBots, m_iMaxBots, m_iTotalGameServerBots, m_iTotalGameServerMaxBots;
	SYSTEMTIME m_MaxUserSysTime;

	BOOL  m_bF1pressed, m_bF4pressed, m_bF12pressed, m_bF5pressed;
	BOOL  m_bOnExitProcess;
	DWORD m_dwExitProcessTime;

	DWORD m_dwWhetherTime, m_dwGameTime1, m_dwGameTime2, m_dwGameTime3, m_dwGameTime4, m_dwGameTime5, m_dwGameTime6, m_dwFishTime;

	// Crusade Schedule
	BOOL m_bIsCrusadeWarStarter;
	BOOL m_bIsApocalypseStarter;
	int m_iLatestCrusadeDayOfWeek;

	int  m_cDayOrNight;		// 1이면 주간, 2면 야간 
	int   m_iSkillSSNpoint[102];

	class CMsg* m_pNoticeMsgList[DEF_MAXNOTIFYMSGS];
	int   m_iTotalNoticeMsg, m_iPrevSendNoticeMsg;
	DWORD m_dwNoticeTime, m_dwSpecialEventTime;
	BOOL  m_bIsSpecialEventTime;
	char  m_cSpecialEventType;
	//LifeX Auto Skills
    void AutoSkill(int iClientH);

	unsigned long m_iLevelExpTable[DEF_MAXEXPTABLE];	//New 22/10/14 - centu: int -> unsigned long

	class CFish* m_pFish[DEF_MAXFISHS];

	class CPotion* m_pPotionConfigList[DEF_MAXPOTIONTYPES];
	class CPotion* m_pCraftingConfigList[DEF_MAXPOTIONTYPES];

	class CTeleportDest* m_pTeleportDestination[DEF_MAXTELEPORTDESTINATION];


	BOOL  m_bIsServerShutdowned;
	char  m_cShutDownCode;
	class CMineral* m_pMineral[DEF_MAXMINERALS];

	int   m_iMiddlelandMapIndex;	// 전쟁터인 미들랜드 맵 인덱스: 이 값이 -1이면 미들랜드 맵이 없음을 의미 
	int   m_iAresdenMapIndex;		// 아레스덴 맵 인덱스 
	int	  m_iElvineMapIndex;		// 엘바인 맵 인덱스
	int   m_iBTFieldMapIndex;
	int   m_iGodHMapIndex;
	int		m_iRampartMapIndex;
	int   m_iAresdenOccupyTiles;
	int   m_iElvineOccupyTiles;
	int   m_iCurMsgs, m_iMaxMsgs;

	// v1.4311-3 변수 선언  예약된 시간을 가지는 변수 
	DWORD m_dwCanFightzoneReserveTime;

	// v1.4311-3 변수 선언 각 사투장의 예약 여부를 저장 
	int  m_iFightZoneReserve[DEF_MAXFIGHTZONE];
	// v1.4311-3 변수 선언 사투장에 들어갔을때 운영자 명령어로 강콜 않되게 함.
	int  m_iFightzoneNoForceRecall;

	

	int	  m_iStrategicStatus;

	class XSocket* m_pSubLogSock[DEF_MAXSUBLOGSOCK];
	int   m_iSubLogSockInitIndex;
	BOOL  m_bIsSubLogSockAvailable[DEF_MAXSUBLOGSOCK];
	int	  m_iCurSubLogSockIndex;
	int   m_iSubLogSockFailCount;
	int   m_iSubLogSockActiveCount;	// v1.5 현재 연결 상태가 유지되고 있는 sub-log-socket 갯수
	int   m_iAutoRebootingCount;	// v1.5 자동 재부팅 된 횟수 

	class CBuildItem* m_pBuildItemList[DEF_MAXBUILDITEMS];
	class CItem* m_pDupItemIDList[DEF_MAXDUPITEMID];

	char* m_pNoticementData;
	DWORD  m_dwNoticementDataSize;

	DWORD  m_dwMapSectorInfoTime;
	int    m_iMapSectorInfoUpdateCount;

	// Crusade 처리용
	int	   m_iCrusadeCount;			// 0이면 전면전 대기상태. 1이면 카운트 다운 시작. 
	BOOL   m_bIsCrusadeMode;		// 크루세이드 모드
	BOOL   m_bIsApocalypseMode;
	// Daryl - Chat logging option
	BOOL m_bLogChatOption;

	struct {
		char cMapName[11];			// 설치되는 맵 위치
		char cType;					// 건축물 종류. 사실 NPC 종류임.
		int  dX, dY;				// 설치되는 맵상의 위치

	} m_stCrusadeStructures[DEF_MAXCRUSADESTRUCTURES];


	int m_iCollectedMana[3];
	int m_iAresdenMana, m_iElvineMana;

	class CTeleportLoc m_pGuildTeleportLoc[DEF_MAXGUILDS];
	//

	WORD  m_wServerID_GSS;
	char  m_cGateServerStockMsg[DEF_MAXGATESERVERSTOCKMSGSIZE];
	int   m_iIndexGSS;
	char m_cApocalypseMapName[11];
	char m_cHeldenianMapName[11];
	int m_iLastCrusadeWinner; 	// New 13/05/2004
	struct {
		int iCrashedStructureNum;
		int iStructureDamageAmount;
		int iCasualties;
	} m_stMeteorStrikeResult;

	// 미들랜드의 정보를 저장하는 구조체. 미들랜드를 운용하지 않는 서버에서는 이 정보를 받아 갱신한다.
	struct {
		char cType;			// 이게 NULL이면 정의되지 않은것을 의미.
		char cSide;			// 사이드
		short sX, sY;		// 설치된 위치 
	} m_stMiddleCrusadeStructureInfo[DEF_MAXCRUSADESTRUCTURES];

	struct {
		char m_cBannedIPaddress[21];		 // 클라이언트의 IP address
	} m_stBannedList[DEF_MAXBANNED];

	struct {
		char m_cGMName[11];
	} m_stAdminList[DEF_MAXADMINS];

	// Crusade Scheduler
	struct {
		int iDay;
		int iHour;
		int iMinute;
	} m_stCrusadeWarSchedule[DEF_MAXSCHEDULE];

	struct {
		int iDay;
		int iHour;
		int iMinute;
	} m_stApocalypseScheduleStart[DEF_MAXAPOCALYPSE];

	struct {
		int iDay;
		int StartiHour;
		int StartiMinute;
	} m_stHeldenianSchedule[DEF_MAXHELDENIAN];

	

	int m_iTotalMiddleCrusadeStructures;

	// 이 변수 안에는 현재 접속한 클라이언트의 인덱스가 공백없이 저장되어 있다. 0이 나오면 리스트의 끝이라는 의미
	int m_iClientShortCut[DEF_MAXCLIENTS + 1];

	int m_iNpcConstructionPoint[DEF_MAXNPCTYPES];
	DWORD m_dwCrusadeGUID;
	int m_sLastCrusadeDate;
	int   m_iCrusadeWinnerSide;
	int   m_iPlayerMaxLevel;

	struct {
		int iTotalMembers;
		int iIndex[9];
	}m_stPartyInfo[DEF_MAXCLIENTS];

	// Daryl - Admin level adjustments
	int m_iAdminLevelWho;
	int m_iAdminLevelGMKill;
	int m_iAdminLevelGMRevive;
	int m_iAdminLevelGMCloseconn;
	int m_iAdminLevelGMCheckRep;
	int m_iAdminLevelEnergySphere;
	int m_iAdminLevelShutdown;
	int m_iAdminLevelObserver;
	int m_iAdminLevelShutup;
	int m_iAdminLevelCallGaurd;
	int m_iAdminLevelSummonDemon;
	int m_iAdminLevelSummonDeath;
	int m_iAdminLevelReserveFightzone;
	int m_iAdminLevelCreateFish;
	int m_iAdminLevelTeleport;
	int m_iAdminLevelCheckIP;
	int m_iAdminLevelPolymorph;
	int m_iAdminLevelSetInvis;
	int m_iAdminLevelSetZerk;
	int m_iAdminLevelSetIce;
	int m_iAdminLevelGetNpcStatus;
	int m_iAdminLevelSetAttackMode;
	int m_iAdminLevelUnsummonAll;
	int m_iAdminLevelUnsummonDemon;
	int m_iAdminLevelSummon;
	int m_iAdminLevelSummonAll;
	int m_iAdminLevelSummonPlayer;
	int m_iAdminLevelDisconnectAll;
	int m_iAdminLevelEnableCreateItem;
	int m_iAdminLevelCreateItem;
	int m_iAdminLevelStorm;
	int m_iAdminLevelWeather;
	int m_iAdminLevelSetStatus;
	int m_iAdminLevelGoto;
	int m_iAdminLevelMonsterCount;
	int m_iAdminLevelSetRecallTime;
	int m_iAdminLevelUnsummonBoss;
	int m_iAdminLevelClearNpc;
	int m_iAdminLevelTime;
	int m_iAdminLevelPushPlayer;
	int m_iAdminLevelSummonGuild;
	int m_iAdminLevelCheckStatus;
	int m_iAdminLevelCleanMap;

	// 09/26/2004
	short m_sSlateSuccessRate;

	// 17/05/2004
	short m_sForceRecallTime;

	// 22/05/2004
	int	 m_iPrimaryDropRate, m_iSecondaryDropRate;
	int  m_iRareDropRate;

	// 25/05/2004
	int m_iFinalShutdownCount;

	// New 06/07/2004
	BOOL m_bEnemyKillMode;
	int m_iEnemyKillAdjust, m_iRatingAdjust;
	BOOL m_bAdminSecurity;
	
	// Configurable Raid Time 
	short m_sRaidTimeMonday; 
	short m_sRaidTimeTuesday; 
	short m_sRaidTimeWednesday; 
	short m_sRaidTimeThursday; 
	short m_sRaidTimeFriday; 
	short m_sRaidTimeSaturday; 
	short m_sRaidTimeSunday; 

	BOOL m_bManualTime;
	int m_iSummonGuildCost;
	
	// Apocalypse
	BOOL	m_bIsApocalyseMode;
	BOOL	m_bIsHeldenianTeleport;

	DWORD m_dwApocalypseGUID;
	
	// Slate exploit
	int m_sCharPointLimit;

	int m_iDKCost;


	// Limit Checks
	short m_sCharStatLimit;
	BOOL m_bAllow100AllSkill;
	short m_sCharSkillLimit;
	char m_cRepDropModifier;
	char  m_cSecurityNumber[11];
	short m_sMaxPlayerLevel;
	
	BOOL var_89C, var_8A0;
	BOOL m_bReceivedItemList;

	

	// MORLA 2.2 - Deathmach Game
	int iDGtop1,iDGtop2,iDGtop3,iDGtop4,iDGtop5,iDGtop6,iDGtop7,iDGtop8,iDGtop9,iDGtop10;
	int iLastKill;
	int iTotalKills;
	bool bDeathmatch;
	DWORD dwTimeLastKill;

	// 	Heldenian
	BOOL	m_bIsHeldenianMode;			// Event has started	-> can TP on maps
	BOOL	m_bHeldenianWarInitiated;	// War has begun		-> can Fight 
	char	m_cHeldenianType;			// Type 1: (BtField) 
										// Type 2: (Casttle siege) Previous winner will defend.
	char    m_sLastHeldenianWinner;		// Last war result, allows to TP on BtField out off war time
										// Tells defender for next Type 2 Heldenian
	char	m_cHeldenianWinner;			// If victory before Event timeOut, tell who wins
	DWORD	m_dwHeldenianGUID;			// identify this event
	DWORD	m_dwHeldenianWarStartTime;	// 15min delay to start the real war
	DWORD	m_dwHeldenianFinishTime;	// 1h30 after, finish the event and compute result...	
	char	m_cHeldenianFirstDestroyedTowerWinner;
	int m_iHeldenianAresdenDead, m_iHeldenianElvineDead;
	int m_iHeldenianAresdenFlags, m_iHeldenianElvineFlags;
	int m_iHeldenianAresdenLeftTower, m_iHeldenianElvineLeftTower;


	DWORD m_dwHeldenianWarTime;
	DWORD m_dwHeldenianEndTime;
	char  m_cHeldenianSummonMode;

	DWORD m_dwApocalypseGateOpenTime;
	DWORD m_dwApocalypseGateCloseTime;

	// new March 13, 2005
	BOOL m_bIsApocalypseGateOpen;
	BOOL m_bHeldinianDuty;
	BOOL m_bHeldenianDutyMultiplyer;
	BOOL m_bIsHeldenianReady;
	BOOL m_bF10pressed;
	BOOL m_bIsApocalypseScheduleLoaded;
	BOOL m_bIsApocalypseSchedule;
	BOOL m_bIsHeldenianScheduleLoaded;
	BOOL m_bIsHeldenianSchedule;
	DWORD m_dwApocalypseStartHour;
	DWORD m_dwApocalypseStartMinute;

	// centu - settings
	int m_iClearMapTime, m_iGoldRate, m_iGuildCost, m_iExpSetting;

	bool  m_bNpcItemConfig;

	//LifeX Fix Revive 14/10/2019
	void AdminOrder_Revive(int iClientH, char * pData, DWORD dwMsgSize);
};

#endif // !defined(AFX_GAME_H__C3D29FC5_755B_11D2_A8E6_00001C7030A6__INCLUDED_)
