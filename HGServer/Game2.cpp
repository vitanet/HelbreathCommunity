// Game2.cpp: implementation of the CGame class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"

//LifeX Add Criticals
void CGame::GetCrits(int iClientH)
{
	char notice[100];
	int MAXCRITICAL = 1000; //Change the limit of the criticals that can be bought
	if (m_pClientList[iClientH] == NULL) return;

	int vCountBuy = (MAXCRITICAL - m_pClientList[iClientH]->m_iSuperAttackLeft);
	int vCountPrice = (vCountBuy * 50); //1 crit is 50 gold

	if (m_pClientList[iClientH]->m_iSuperAttackLeft >= MAXCRITICAL)
	{
		SendNotifyMsg(iClientH, iClientH, DEF_NOTIFY_NOTICEMSG, NULL, NULL, NULL, "Reach MAX Criticals.");
		return;
	}

	else if (dwGetItemCount(iClientH, "Gold") < vCountPrice)
	{
		wsprintf(notice, " You need %d gold to buy criticals.", vCountPrice);
		SendNotifyMsg(iClientH, iClientH, DEF_NOTIFY_NOTICEMSG, NULL, NULL, NULL, notice);
		return;
	}

	else if (dwGetItemCount(iClientH, "Gold") >= vCountPrice)
	{
		SetItemCount(iClientH, "Gold", dwGetItemCount(iClientH, "Gold") - vCountPrice);
		m_pClientList[iClientH]->m_iSuperAttackLeft = MAXCRITICAL;
		SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_SUPERATTACKLEFT, NULL, NULL, NULL, NULL);
		wsprintf(notice, " You bought %d criticals with %d gold.", vCountBuy, vCountPrice);
		SendNotifyMsg(iClientH, iClientH, DEF_NOTIFY_NOTICEMSG, NULL, NULL, NULL, notice);
		return;
	}
}