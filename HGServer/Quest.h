// Quest.h: interface for the CQuest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUEST_H__3E61C780_FF48_11D3_9DC2_00A0CC5B45EE__INCLUDED_)
#define AFX_QUEST_H__3E61C780_FF48_11D3_9DC2_00A0CC5B45EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

class CQuest  
{
public:
	
	char m_cSide;				// 어느 편에 속하는 임무인가? 
	
	int m_iType;				// Quest 종류 
	int m_iTargetType;			// Quest의 목표 종류. 각 Type에 따라 다른 값이 정해진다. 
	int m_iMaxCount;			// 최대 만족 횟수 

	int m_iFrom;				// Quest를 내려주는 NPC 종류 
	
	int m_iMinLevel;			// Quest를 받기 위한 최저 레벨. 
	int m_iMaxLevel;			// Quest를 받기 위한 최대 레벨 

	int m_iRequiredSkillNum;	// 조건 판단에 필요한 스킬 번호 
	int m_iRequiredSkillLevel;	// 조건 판단에 필요한 스킬 레벨 

	int m_iTimeLimit; 			// 시간 제한 잔존량 
	int m_iAssignType;			// 퀘스트 할당 타입. -1이면 아무때나. 1이면 Crusade 전면전일때만.

								// 상품 종류 및 수량. 3개 중 1개가 랜덤하게 정해짐. 0번 인덱스는 사용 안함.
	int m_iRewardType[4]; 
	int m_iRewardAmount[4];

	int m_iContribution;		// 공헌도 
	int m_iContributionLimit;	// 최대 공헌도 제한 

	int m_iResponseMode;		// 응답 모드: 0(ok) 1(Accept/Decline) 2(Next)

	char m_cTargetName[21];		// 퀘스트가 지정된 이름. 맵 혹은 캐릭터 이름  
	int  m_sX, m_sY, m_iRange;	// 퀘스트가 지정된 맵 장소 

	int  m_iQuestID;			// 퀘스트 ID. 만약 퀘스트의 내용이 변경되었을 경우를 대비한 것임.

	int  m_iReqContribution;	// 퀘스트를 맡기 위한 최저 공헌도 제한. 


	//CQuest();
	//virtual ~CQuest();

};

#endif // !defined(AFX_QUEST_H__3E61C780_FF48_11D3_9DC2_00A0CC5B45EE__INCLUDED_)
