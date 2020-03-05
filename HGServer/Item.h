// Item.h: interface for the CItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_)
#define AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_

#include <windows.h>

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Game.h"
#include "StrTok.h"
#include "TeleportLoc.h"
#include "GlobalDef.h"
#include "OccupyFlag.h"
#include "Tile.h"
#include "StrategicPoint.h"

class CItem  
{
public:
	CItem();
	virtual ~CItem();

	char  m_cName[21];
	
	short m_sIDnum;					// 아이템의 고유 번호
	char  m_cItemType;
	char  m_cEquipPos;
	short m_sItemEffectType;     
	short m_sItemEffectValue1, m_sItemEffectValue2, m_sItemEffectValue3; 
	short m_sItemEffectValue4, m_sItemEffectValue5, m_sItemEffectValue6; 
	WORD  m_wMaxLifeSpan;
	short m_sSpecialEffect;
	
	//v1.432 명중률 가감 사용 안한다. 대신 특수 능력 수치가 들어간다.
	short m_sSpecialEffectValue1, m_sSpecialEffectValue2; 

	short m_sSprite;
	short m_sSpriteFrame;

	char  m_cApprValue;
	char  m_cSpeed;

	DWORD m_wPrice; 
	WORD  m_wWeight;
	short m_sLevelLimit;	
	char  m_cGenderLimit;

	short m_sRelatedSkill;
	
	char  m_cCategory;
	BOOL  m_bIsForSale;

	DWORD m_dwCount;
	short m_sTouchEffectType;
	short m_sTouchEffectValue1, m_sTouchEffectValue2, m_sTouchEffectValue3;
	char  m_cItemColor; // v1.4 에서 아이템 색으로 변경되었다. 
	short m_sItemSpecEffectValue1, m_sItemSpecEffectValue2, m_sItemSpecEffectValue3;
	short m_wCurLifeSpan;
	DWORD m_dwAttribute;				// aaaa bbbb cccc dddd eeee ffff xxxx xxx1 
										// 1: Custom-Made Item flag 
										// a: Item 속성 종류 
										// b: Item 속성 정도
										// c: 특수 아이템 성질 Flag 
										// d: 특수 아이템 성질 정도 
										// e: 특수 아이템 추가 성질 Flag
										// f: 특수 아이템 추가 성질 정도 
										 

};

/*
아이템 정의파일에 필요한 내용 - 게임 서버용

char cItemType
 - 아이템의 종류. 장착되는 것인지 소모되는 것인지에 대한 정의

char cEquipPos
 - 장착되는 아이템이라면 그 위치 

short sItemEffectType     
  - 아이템의 효과 종류.  

short sItemEffectValue1, 2, 3 ... 
 - 아이템 효과의 수치 1, 2, 3...

WORD wMaxLifeSpan
 - 아이템의 최대 수명 

short sMaxFixCount
 - 최대 고칠수 있는 횟수

short sSprite
short sSpriteFrame
 - 아이템 스프라이트 번호 

WORD  wPrice 
 - 아이템 가격 
WORD  wWeight
 - 아이템 무게 
short sLevelLimit
 - 아이템 레벨 제한 
char  cGenderLimit
 - 아이템 성별 제한 

short m_SM_HitRatio, m_sL_HitRatio
 - 목표별 명중률 가감치 (무기일 경우)

short sRelatedSkill
 - 연결된 Skill

플레이어 데이터 파일에 저장되어야 할 정보 - 로그 서버용 

DWORD dwCount
 - 아이템의 갯수. (같은 종류의 화살이나 포션, Gold등만이 의미를 갖는다)

WORD wCurLifeSpan
 - 현재 남은 수명 

short sCurFixCount
 - 현재까지 수리한 횟수 

short sTouchEffectType
 - 아이템을 처음 건드렸을때 작동되는 효과 종류 

short sTouchEffectValue1, 2, 3...
 - 아이템 접촉 효과의 수치 

short sItemSpecEffectType
 - 아이템의 특수효과 종류 (ex:마법이 걸린 무기라면 공격력이나 방어력에 영향이 있을수도)

short sItemSpecEffectValue1, 2, 3...
 - 아이템 특수효과의 수치 1, 2, 3...


옷 아이템인 경우(Equip Position이 Body, Leggings, Arms인 경우)
sItemEffectValue4: 옷의 특정 특성치 제한 종류를 나타낸다. 

  10: Str
  11: Dex
  12: Vit
  13: Int
  14: Mag
  15: Chr

sItemEffectValue5: 옷의 특정 특성치 제한 수준



*/

#endif // !defined(AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_)
