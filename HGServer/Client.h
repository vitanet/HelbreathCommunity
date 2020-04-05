// Client.h: interface for the CClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENT_H__39CC7700_789F_11D2_A8E6_00001C7030A6__INCLUDED_)
#define AFX_CLIENT_H__39CC7700_789F_11D2_A8E6_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "XSocket.h"
#include "Item.h"
#include "GuildsMan.h"
#include "Magic.h"
#include "GlobalDef.h"

class CClient  
{
public:

	int  m_iAngelicStr; // By Snoopy81
	int  m_iAngelicInt; // By Snoopy81
	int  m_iAngelicDex; // By Snoopy81
	int  m_iAngelicMag; // By Snoopy81
	int m_iWantedLevel; // Wanted System

	BOOL m_bIsInsideEnemyBuilding;
	char m_cWarType;
	int m_iRecentWalkTime;
	int m_iRecentRunTime;
	short m_sV1;
	char m_cHeroArmourBonus;
	int   m_iAlterItemDropIndex;
	int bCreateNewParty();

	BOOL m_bIsExchangeMode;

	// Hack Checkers
	DWORD m_dwMagicFreqTime, m_dwMoveFreqTime, m_dwAttackFreqTime;
	BOOL m_bIsMoveBlocked, m_bMagicItem;
	DWORD dwClientTime;
	BOOL m_bMagicConfirm;
	int m_iSpellCount;
	BOOL m_bMagicPauseTime;

	CClient(HWND hWnd);
	virtual ~CClient();

	char m_cCharName[12];
	char m_cAccountName[12];
	char m_cAccountPassword[12];

	BOOL  m_bIsInitComplete;
	BOOL  m_bIsMsgSendAvailable;

	char  m_cMapName[12];	
	char  m_cMapIndex;
	short m_sX, m_sY;
	
	char  m_cGuildName[22];		// 길드의 이름 
	char  m_cLocation[12];      //  <- 맵이름이 그대로 저장된다. 소속 마을 
	int   m_iGuildRank;			// -1이면 무의미. 0이면 길드 마스터. 양수는 길드내에서의 서열 
	int   m_iGuildGUID;
	
	char  m_cDir;
	short m_sType;				// 현재 타입 <- 폴리모프 시에 변한다. 
	short m_sOriginalType;		// 오리지날 타입
	short m_sAppr1;
	short m_sAppr2;
	short m_sAppr3;
	short m_sAppr4;
	int   m_iApprColor;			// v1.4 외형 컬러테이블
	int   m_iStatus;
	int   m_iStatus2;

	DWORD m_dwTime, m_dwHPTime, m_dwMPTime, m_dwSPTime, m_dwAutoSaveTime, m_dwHungerTime, m_dwWarmEffectTime;
	// Player 특성치 

	char m_cSex, m_cSkin, m_cHairStyle, m_cHairColor, m_cUnderwear;

	int  m_iHP;						// Hit Point
	int  m_iHPstock;
	int  m_iMP;
	int  m_iSP;
	unsigned long  m_iExp;
	unsigned long m_iNextLevelExp;
	BOOL m_bIsKilled;

	int  m_iDefenseRatio;		// Defense Ratio
	int  m_iHitRatio;			// Hit Ratio

	

	int  m_iDamageAbsorption_Armor[DEF_MAXITEMEQUIPPOS];		// 갑옷 착용으로 인한 Damage 흡수 효과
	int  m_iDamageAbsorption_Shield;	// Parrying 기술 성공시의 Damage 흡수 효과 

	int  m_iLevel;
	int  m_iStr, m_iInt, m_iVit, m_iDex, m_iMag, m_iCharisma;
	int  m_iLuck; 
	int  m_iLU_Pool;
	BOOL m_bIsForceDisconnect;
	BOOL m_bIsAdminCreateItemEnabled;

	int  m_iDeaths, m_iDGPoints, m_iDGDeaths, m_iDGKills, m_iTotalDGDeaths, m_iTotalDGKills; // MORLA 2.2 - Nuevos datos
	
	char m_cAura;
	//MOG var - 3.51
	int m_iGizonItemUpgradeLeft;
	int m_iMajesticLevel;

	int m_iAddTransMana, m_iAddChargeCritical;

	int  m_iEnemyKillCount, m_iPKCount, m_iRewardGold, m_iMaxEK;
	int  m_iCurWeightLoad;		// 현재 총 소지품 무게 

	char m_cSide;				// 플레이어의 편 
	
	BOOL m_bInhibition;

	char m_cAttackDiceThrow_SM;	// 공격치 주사위 던지는 회수 
	char m_cAttackDiceRange_SM;	// 공격치 주사위 범위 
	char m_cAttackDiceThrow_L;	// 공격치 주사위 던지는 회수 
	char m_cAttackDiceRange_L;	// 공격치 주사위 범위 
	char m_cAttackBonus_SM;		// 공격 보너스
	char m_cAttackBonus_L;		// 공격 보너스

	class CItem * m_pItemList[DEF_MAXITEMS];
	POINT m_ItemPosList[DEF_MAXITEMS];
	class CItem * m_pItemInBankList[DEF_MAXBANKITEMS];
	
	BOOL  m_bIsItemEquipped[DEF_MAXITEMS];
	short m_sItemEquipmentStatus[DEF_MAXITEMEQUIPPOS];
	char  m_cArrowIndex;		// 플레이어가 활을 사용할때 화살 아이템 인덱스. 초기값은 -1(할당 안됨)

	char           m_cMagicMastery[DEF_MAXMAGICTYPE];
	unsigned char  m_cSkillMastery[DEF_MAXSKILLTYPE]; // v1.4

	int   m_iSkillSSN[DEF_MAXSKILLTYPE];
	BOOL  m_bSkillUsingStatus[DEF_MAXSKILLTYPE];
	int   m_iSkillUsingTimeID[DEF_MAXSKILLTYPE]; //v1.12

	char  m_cMagicEffectStatus[DEF_MAXMAGICEFFECTS];

	int   m_iWhisperPlayerIndex;
	char  m_cProfile[256];

	int   m_iHungerStatus;		// 배고픔 포인트. 이게 0이되면 스태미너가 오르지 않으며 체력도 절반이상 차지 않습니다. 

	DWORD m_dwWarBeginTime;		// 적국에 들어가는 순간에 기록되는 시간. 
	BOOL  m_bIsWarLocation;		// 현재 적국에 있는지를 표시 

	BOOL  m_bIsPoisoned;		// 중독되었는지의 여부 
	int   m_iPoisonLevel;       // 독의 강도 
	DWORD m_dwPoisonTime;		// 중독 시간.
	
	int   m_iPenaltyBlockYear, m_iPenaltyBlockMonth, m_iPenaltyBlockDay; // v1.4

	//v1.4311-3 추가 변수 선언 사투장 번호와 사투장 예약한 시간
	int   m_iFightzoneNumber , m_iReserveTime, m_iFightZoneTicketNumber ; 

	class XSocket * m_pXSock;

	int   m_iAdminUserLevel;     // 관리자 레벨. 0이면 무효. 번호가 올라갈 수록 권한이 커진다.
	int   m_iRating;

	int   m_iTimeLeft_ShutUp;	 // 이 값이 0이 아니면 채팅메시지가 전달되지 않는다.
	int   m_iTimeLeft_Rating;	 // 다른 플레이어의 평판을 매기기 위해 남은 시간. 0이면 사용 가능  
	int   m_iTimeLeft_ForceRecall;  // 강제 리콜되기 위해 남아있는 시간틱 
	int   m_iTimeLeft_FirmStaminar; // 스태미너가 달아 없어지지 않는 시간 텀 

	BOOL  m_bIsOnServerChange;     // 이 값이 활성화 되어 있으면 삭제시 데이터 저장 및 카운팅을 하지 않는다.

	unsigned long   m_iExpStock;			 // 쌓여있는 경험치 
	DWORD m_dwExpStockTime;		 // ExpStock 계산 시간.

	unsigned long   m_iAutoExpAmount;		 // Auto-Exp 시간 동안 얻은 경험치 
	DWORD m_dwAutoExpTime;		 // Auto-Exp 계산 시간.

	DWORD m_dwRecentAttackTime;  // 가장 최근에 공격했던 시간 

	int   m_iAllocatedFish;		 // 이 값이 0이 아니면 이벤트 낚시모드라는 이야기다. 
	int   m_iFishChance;		 // 현재 낚을 상태 

	DWORD m_dwAFKCheckTime;

	BOOL m_bIsPolymorph;
	
	char  m_cIPaddress[21];		 // 클라이언트의 IP address
	char m_cIP2[21];
	BOOL  m_bIsSafeAttackMode;

	BOOL  m_bIsOnWaitingProcess; // 텔레포트등 처리를 기다리는 상태라면 
	
	int   m_iSuperAttackLeft;	 // v1.2 필살기 사용 가능 횟수 
	int   m_iSuperAttackCount;   // v1.2 필살기 사용 가능 카운트. 이 카운트가 다 차면 필살기 사용 횟수가 늘어난다. 

	short m_sUsingWeaponSkill;	 // v1.2 현재 사용하는 무기의 스킬 인덱스 

	int   m_iManaSaveRatio;		 // v1.2 마나 절약 포인트 
	
	BOOL  m_bIsLuckyEffect;		 // v1.2 행운 효과 
	int   m_iSideEffect_MaxHPdown; // v1.4 최대 HP 절감 효과 

	int   m_iComboAttackCount;   // v1.3 연타 공격 카운트 
	int   m_iDownSkillIndex;	 // v1.3 다른 스킬이 올라갈때 내릴 스킬 인덱스 

	int   m_iMagicDamageSaveItemIndex; // v1.3 마법 공격 대미지 절감 아이템 인덱스 (보통은 -1)

	short m_sCharIDnum1, m_sCharIDnum2, m_sCharIDnum3; // v1.3 그 캐릭터가 갖는 고유값!

	int   m_iAbuseCount;		// 해킹 용의자 파악용 
	
	BOOL  m_bIsBWMonitor;		// BadWord 모니터인가?

	char  m_cExchangeMode;
	int   m_iExchangeH;
	char  m_cExchangeName[11];
	char  m_cExchangeItemName[4][21];
	char  m_iExchangeAlterIndex[4];
	char  m_cExchangeItemIndex[4];
	int   m_iExchangeItemAmount[4];
	BOOL  m_bIsExchangeConfirm;
	int   m_iExchangeComplete;
	int	  m_iExchangeCount;

	int   m_iQuest;				 // 현재 할당된 Quest 
	int   m_iQuestID;			 // 할당받은 Quest의 ID값 
	int   m_iAskedQuest;		 // 물어본 퀘스트 
	int   m_iCurQuestCount;		 // 현재 퀘스트 상태 
	
	int   m_iQuestRewardType;	 // 퀘스트 해결시 상품 종류 -> 아이템의 ID값이다.
	int   m_iQuestRewardAmount;	 // 상품 갯수 

	int   m_iContribution;		 // 도시에 대한 공헌도. 

	BOOL  m_bQuestMatchFlag_Loc; // 퀘스트 수행 조건을 판단하기 위함.
	BOOL  m_bIsQuestCompleted;   // 퀘스트가 완료되었는가? 

	int   m_iCustomItemValue_Attack;
	int   m_iCustomItemValue_Defense;

	int   m_iMinAP_SM;			// Custom-Item의 효과로 인한 최소 AP
	int   m_iMinAP_L;

	int   m_iMaxAP_SM;			// Custom-Item의 효과로 인한 최대 AP
	int   m_iMaxAP_L;

	BOOL  m_bIsNeutral;			// v1.5 중립여부를 판별하기 위한 플래그. 처리 속도를 높이기 위함이다.
	BOOL  m_bIsObserverMode;	// v1.5 관람자 모드인지 판별 

	int   m_iSpecialEventID;	// 특수 행사 참가확인용 플래그 

	int   m_iSpecialWeaponEffectType;	// 희귀 아이템 효과 종류
	int   m_iSpecialWeaponEffectValue;	// 희귀 아이템 효과 값
	// 희귀 아이템 효과 종류: 
	// 0-None 1-필살기대미지추가 2-중독효과 3-정의의  
	// 5-민첩의 6-가벼운 7-예리한 8-강화된 9-고대문명의

	// v1.42
	int   m_iAddHP, m_iAddSP, m_iAddMP; 
	int   m_iAddAR, m_iAddPR, m_iAddDR;
	int   m_iAddMR, m_iAddAbsPD, m_iAddAbsMD; 
	int   m_iAddCD, m_iAddGold;		// 고정 마법 대미지 흡수율. 반지류와는 독립적으로 계산된다.
	unsigned long m_iAddExp;

	int   m_iAddResistMagic;					// v1.2 추가 마법 저항 
	int   m_iAddPhysicalDamage;					// v1.2 고정 대미지 추가 포인트 
	int   m_iAddMagicalDamage;	

	int   m_iAddAbsAir;							// 속성별 대미지 흡수
	int   m_iAddAbsEarth;
	int   m_iAddAbsFire;
	int   m_iAddAbsWater;

	DWORD m_dwDSLAT, m_dwDSLATOld, m_dwDSLATS;
	int m_iDSCount;

	// re introduced by SNOOPY
	int  m_iHitRatio_ItemEffect_SM; // SNOOPY Added this again
	int  m_iHitRatio_ItemEffect_L;
	int  m_iMagicHitRatio_ItemEffect; // By Snoopy (for special wands)
	
	int   m_iLastDamage;

	int   m_iMoveMsgRecvCount, m_iAttackMsgRecvCount, m_iRunMsgRecvCount, m_iSkillMsgRecvCount;
	DWORD m_dwMoveLAT, m_dwRunLAT, m_dwAttackLAT;

	int   m_iSpecialAbilityTime;				// 특수 능력을 사용하기 위해서는 이 값이 0이 되어야 한다. 
	BOOL  m_bIsSpecialAbilityEnabled;			// 특수 능력이 활성화 된 상태인가?
	DWORD m_dwSpecialAbilityStartTime;			// 특수 능력을 사용하기 시작한 시간
	int   m_iSpecialAbilityLastSec;				// 특수 능력 지속 시간.

	int   m_iSpecialAbilityType;				// 할당된 특수 능력 종류
												// 공격형
												// 0:능력 없음  1:공격시 적 HP 50% 감소  2:냉동 효과  3: 마비 효과  4: 원샷 원킬  5:입힌 대미지 만큼의 HP를 얻는다.
												// 방어형
												// 50: 무기 수명 0로 만듬. 51:해당 부위 대미지 무효화  52: 모5든 부위 대미지 무효화
	int   m_iSpecialAbilityEquipPos;			// 방어구인 경우 특수효과가 적용되는 부위를 의미함.
	BOOL  m_bIsAdminCommandEnabled;

	int   m_iWarContribution;					// 전쟁 공헌도 

	DWORD m_dwSpeedHackCheckTime;				// 속도버그 검사 루틴 
	unsigned long   m_iSpeedHackCheckExp;		
	DWORD m_dwLogoutHackCheck;

	DWORD m_dwInitCCTimeRcv;
	DWORD m_dwInitCCTime;

	char  m_cLockedMapName[12];					// 갖힌 맵 이름
	int   m_iLockedMapTime;						// 이 값이 0 이상이면 어디로 텔레포트 해도 위의 맵으로 간다.

	int   m_iCrusadeDuty;						// 크루세이드에서 맡은 역할: 1-용병. 2-건설자. 3-지휘관
	DWORD m_dwCrusadeGUID;						// 크루세이드 GUID
	DWORD m_dwHeldenianGUID;
	BOOL m_bIsImpossibleZone;

	// 이 스트럭쳐는 맵의 내용을 복사하는 것이다. 한번에 보내 줄 수 없기 때문에 여러번에 걸쳐 나누어 전송한다.
	struct {
		char cType;
		char cSide;
		short sX, sY;
	} m_stCrusadeStructureInfo[DEF_MAXCRUSADESTRUCTURES];
	int m_iCSIsendPoint;

	char m_cSendingMapName[12];
	BOOL m_bIsSendingMapStatus;

	// 지휘관이 건설할 수 있는 포인트. 일반 플레이어라면 자신의 행동에 대한 누적값이다.
	int  m_iConstructionPoint;

	char m_cConstructMapName[12];
	int  m_iConstructLocX, m_iConstructLocY;
	
	// 2.06
	BOOL m_bIsPlayerCivil;
	BOOL m_bIsAttackModeChange;

	// New 06/05/2004
	// Party Stuff
	int m_iPartyID;
	int m_iPartyStatus;
	int m_iReqJoinPartyClientH;
	char m_cReqJoinPartyName[12];

	int   m_iPartyRank;										// Party내에서의 위치. -1이면 무의미. 1이면 파티 생성자. 12면 멤버 
	int   m_iPartyMemberCount;								// 파티 인원 제한용 
	int   m_iPartyGUID;										// v1.42 Party GUID
	struct {
	int  iIndex;
	char cName[11];
	} m_stPartyMemberName[DEF_MAXPARTYMEMBERS];

	// New 07/05/2004
	DWORD m_dwLastActionTime;
	int m_iDeadPenaltyTime;

	// New 16/05/2004
	char m_cWhisperPlayerName[12];
	BOOL m_bIsAdminOrderGoto;
	BOOL m_bIsInsideWarehouse;
	BOOL m_bIsInsideWizardTower;
	BOOL m_bIsInCombatantOnlyZone;
	BOOL m_bIsCheckingWhisperPlayer;
	BOOL m_bIsOwnLocation;
	BOOL m_pIsProcessingAllowed;

	// Updated 10/11/2004 - 24/05/2004
	char m_cHeroArmorBonus;
	BOOL m_bIsClientConnected;

	// New 25/05/2004
	BOOL m_bIsBeingResurrected;

	DWORD m_dwFightzoneDeadTime;
	char m_cSaveCount;

	//50Cent - Repair All
    short totalItemRepair;
    struct
    {
        char index;
        short price;
    } m_stRepairAll[DEF_MAXITEMS];
};

#endif // !defined(AFX_CLIENT_H__39CC7700_789F_11D2_A8E6_00001C7030A6__INCLUDED_)

