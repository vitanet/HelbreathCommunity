#include "Account.h"

CAccount::CAccount(char * cDataName, char * m_cWorldName, int iClient)
{
	ZeroMemory(cAccountName, sizeof(cAccountName));	
	ZeroMemory(cWorldName, sizeof(cWorldName));
	strcpy(cAccountName, cDataName);
	strcpy(cWorldName, m_cWorldName);
	ZeroMemory(cPassword,sizeof(cPassword));
	ZeroMemory(cCharName,sizeof(cCharName));
	iAccountValid = 0;
	m_iAccntYear = m_iAccntMonth = m_iAccntDay = 0;
	m_iPassYear = m_iPassMonth = m_iPassDay = 0;
	dAccountID = -1;
	iClientH = iClient;
}
CAccount::~CAccount()
{

}
CCharacter::CCharacter(char * cDataName, int m_iTracker)
{
	ZeroMemory(cCharacterName, sizeof(cCharacterName));			
	strcpy(cCharacterName, cDataName);
	iTracker = m_iTracker;
}
CCharacter::~CCharacter()
{

}