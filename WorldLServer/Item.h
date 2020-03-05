#if !defined(AFX_ITEM_H__A9554BE2_A96B_11D2_B143_00001C7030A7__INCLUDED_)
#define AFX_ITEM_H__A9554BE2_A96B_11D2_B143_00001C7030A7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "Item.h"
#include "NetMessages.h"

class CItem  
{
public:
		
	CItem();
	virtual ~CItem();
	
	char  m_cItemName[30];		// 4h

};

#endif
