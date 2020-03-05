#if !defined(AFX_ACCOUNT_H__57BA71C0_B929_11D2_B143_00001C7030A6__INCLUDED_)
#define AFX_ACCOUNT_H__57BA71C0_B929_11D2_B143_00001C7030A6__INCLUDED_


#include <windows.h>

class CAccount  								 
{
public:
	CAccount(char * cDataName, char * m_cWorldName, int iClient);
	virtual ~CAccount();

	char cAccountName[11];		// 5h
	char cPassword[11];			// 10h
	int iAccountValid, iClientH;
	short m_iAccntYear, m_iAccntMonth, m_iAccntDay;
	short m_iPassYear, m_iPassMonth, m_iPassDay;
	char cWorldName[30];
	char cCharName[11];
	DWORD dAccountID;
};
class CCharacter  								 
{
public:
	CCharacter(char * cDataName, int m_iTracker);
	virtual ~CCharacter();
	char cCharacterName[11];	// 18h
	int iTracker;
};


#endif // !defined(AFX_MSG_H__57BA71C0_B929_11D2_B143_00001C7030A6__INCLUDED_)