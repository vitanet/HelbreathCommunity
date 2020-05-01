// OnlineUser.h: interface for the OnlineUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ONLINEUSER_H__D500A2E3_0662_4156_814E_0F39112419BF__INCLUDED_)
#define AFX_ONLINEUSER_H__D500A2E3_0662_4156_814E_0F39112419BF__INCLUDED_

#include <windows.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class OnlineUser  
{
public:
	OnlineUser();
	virtual ~OnlineUser();

	char m_cName[11];
	char m_cGuildName[21];

};

#endif // !defined(AFX_ONLINEUSER_H__D500A2E3_0662_4156_814E_0F39112419BF__INCLUDED_)
