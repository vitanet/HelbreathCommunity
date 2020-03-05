#if !defined(AFX_SQL_H__A9554BE2_A96B_11D2_B143_00001C7030A7__INCLUDED_)
#define AFX_SQL_H__A9554BE2_A96B_11D2_B143_00001C7030A7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <windows.h>

class CSql
{
public:
		
	CSql();
	virtual ~CSql();
	void CreateNewCharacter();
	void Bank();
	void CharacterInfo();
	void Guild();
};

#endif
