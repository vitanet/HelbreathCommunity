// Mineral.h: interface for the CMineral class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINERAL_H__1A17B280_B21E_11D3_9DC2_00001C7030A6__INCLUDED_)
#define AFX_MINERAL_H__1A17B280_B21E_11D3_9DC2_00001C7030A6__INCLUDED_

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
#include "Quest.h"
#include "BuildItem.h"
#include "Teleport.h"
#include "TeleportLoc.h"
#include "GlobalDef.h"
#include "TempNpcItem.h"

class CMineral  
{
public:
	CMineral(char cType, char cMapIndex, int sX, int sY, int iRemain);
	virtual ~CMineral();
	
	char  m_cType;

	char  m_cMapIndex;
	int   m_sX, m_sY;
	int   m_iDifficulty;
	short m_sDynamicObjectHandle;
	
	int   m_iRemain;
};

#endif // !defined(AFX_MINERAL_H__1A17B280_B21E_11D3_9DC2_00001C7030A6__INCLUDED_)
