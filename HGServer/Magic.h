// Magic.h: interface for the CMagic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGIC_H__CD611900_EF3A_11D2_9B6C_00001C7030A6__INCLUDED_)
#define AFX_MAGIC_H__CD611900_EF3A_11D2_9B6C_00001C7030A6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

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


class CMagic  
{									 
public:
	CMagic();
	virtual ~CMagic();

	char m_cName[31];

	short m_sType;
	DWORD m_dwDelayTime, m_dwLastTime;
	short m_sValue1, m_sValue2, m_sValue3, m_sValue4, m_sValue5, m_sValue6;
	short m_sValue7, m_sValue8, m_sValue9, m_sValue10, m_sValue11, m_sValue12;
	short m_sIntLimit;
	int   m_iGoldCost;
	
	char  m_cCategory; // ¸¶aý Ál·u: RDaÝ ¸¶aý 0, °r°Ý¸¶aý 1, aclî ¸¶aý 2 
	int   m_iAttribute; // ¸¶aý LÓLs:  ¶A 1 ÇD´A 2 sN 3 a° 4  
};

#endif // !defined(AFX_MAGIC_H__CD611900_EF3A_11D2_9B6C_00001C7030A6__INCLUDED_)

