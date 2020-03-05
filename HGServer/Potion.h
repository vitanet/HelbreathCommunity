// Potion.h: interface for the CPotion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POTION_H__E18064C0_AF0D_11D3_9DC2_00001C7030A6__INCLUDED_)
#define AFX_POTION_H__E18064C0_AF0D_11D3_9DC2_00001C7030A6__INCLUDED_

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
#include "Mineral.h"
#include "Quest.h"
#include "BuildItem.h"
#include "Teleport.h"
#include "TeleportLoc.h"
#include "GlobalDef.h"
#include "TempNpcItem.h"

class CPotion  
{
public:
	CPotion();
	virtual ~CPotion();

	char  m_cName[21];
	short m_sArray[12];

	int   m_iSkillLimit, m_iDifficulty;

};

#endif // !defined(AFX_POTION_H__E18064C0_AF0D_11D3_9DC2_00001C7030A6__INCLUDED_)
