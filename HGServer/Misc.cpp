// Misc.cpp: implementation of the CMisc class.
//
//////////////////////////////////////////////////////////////////////

#include "Misc.h"

#pragma warning (disable : 4996 6269)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMisc::CMisc()
{

}

CMisc::~CMisc()
{

}											    

void CMisc::GetMyCursorPos(short * pX, short * pY)
{
 POINT point;

	GetCursorPos(&point);
	*pX = (short)point.x;
	*pY = (short)point.y;
}


char CMisc::cGetNextMoveDir(short sX, short sY, short dX, short dY)
{
 short absX, absY;
 char  cRet = 0;

	absX = sX - dX;
	absY = sY - dY;

	if ((absX == 0) && (absY == 0)) cRet = 0;

	if (absX == 0) {
		if (absY > 0) cRet = 1;
		if (absY < 0) cRet = 5;
	}
	if (absY == 0) {
		if (absX > 0) cRet = 7;
		if (absX < 0) cRet = 3;
	}
	if ( (absX > 0)	&& (absY > 0) ) cRet = 8;
	if ( (absX < 0)	&& (absY > 0) ) cRet = 2;
	if ( (absX > 0)	&& (absY < 0) ) cRet = 6;
	if ( (absX < 0)	&& (absY < 0) ) cRet = 4;	

	return cRet;
}

void CMisc::GetPoint(int x0, int y0, int x1, int y1, int * pX, int * pY, int * pError)
{
 int dx, dy, x_inc, y_inc, error, index;
 int iResultX, iResultY, iDstCnt;

	if ((x0 == x1) && (y0 == y1)) {
		*pX = x0;
		*pY = y0;
		return;
	}

	error = *pError;

	iResultX = x0;
	iResultY = y0;
	iDstCnt  = 0;

	dx = x1-x0;
	dy = y1-y0;

	if(dx>=0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx = -dx;
	}

	if(dy>=0)
	{
		y_inc = 1;
	}
	else
	{
		y_inc = -1;
		dy = -dy;
	}

	if(dx>dy)
	{
		for(index = 0; index<=dx; index++)
		{
			error+=dy;
			if(error>dx)
			{
				error-=dx;
				iResultY += y_inc;
			}
			iResultX += x_inc;
			goto CALC_OK;
		}
	}
	else
	{
		for(index=0; index<=dy; index++)
		{
			error+=dx;
			if(error>0)
			{
				error-=dy;
				iResultX += x_inc;
			}
			iResultY += y_inc;
			goto CALC_OK;
		}
	}

CALC_OK:;

	*pX = iResultX;
	*pY = iResultY;
	*pError = error;
}


void CMisc::GetPoint2(int x0, int y0, int x1, int y1, int * pX, int * pY, int * pError, int iCount)
{
 int dx, dy, x_inc, y_inc, error, index;
 int iResultX, iResultY, iCnt = 0;


	if ((x0 == x1) && (y0 == y1)) {
		*pX = x0;
		*pY = y0;
		return;
	}

	error = *pError;

	iResultX = x0;
	iResultY = y0;

	dx = x1-x0;
	dy = y1-y0;

	if(dx>=0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx = -dx;
	}

	if(dy>=0)
	{
		y_inc = 1;
	}
	else
	{
		y_inc = -1;
		dy = -dy;
	}

	if(dx>dy)
	{
		for(index = 0; index <= dx; index++)
		{
			error += dy;
			if(error > dx)
			{
				error -= dx;
				iResultY += y_inc;
			}
			iResultX += x_inc;
			iCnt++;
			if (iCnt >= iCount)
				goto CALC_OK;
		}
	}
	else
	{
		for(index = 0; index <= dy; index++)
		{
			error += dx;
			if(error > dy)
			{
				error -= dy;
				iResultX += x_inc;
			}
			iResultY += y_inc;
			iCnt++;
			if (iCnt >= iCount)
				goto CALC_OK;
		}
	}

CALC_OK:;

	*pX = iResultX;
	*pY = iResultY;
	*pError = error;
}


void CMisc::GetDirPoint(char cDir, int * pX, int * pY)
{
	switch(cDir) {
	case 1:	*pY--; break;
	case 2:	*pY--; *pX++;	break;
	case 3:	*pX++; break;
	case 4:	*pX++; *pY++;	break;
	case 5:	*pY++; break;
	case 6:	*pX--; *pY++;	break;
	case 7:	*pX--; break;
	case 8:	*pX--; *pY--;	break;
	}

}


BOOL CMisc::bEncode(char cKey, char *pStr)
{
 int i, iLen;

	// !!
	return TRUE;

	iLen = strlen(pStr);
	for (i = 0; i <= iLen-1; i++) {
		pStr[i]  = pStr[i] ^ (cKey);
	}

	return TRUE;
}


BOOL CMisc::bDecode(char cKey, char *pStr)
{
 int i, iLen;

	// !!
	return TRUE;

	iLen = strlen(pStr);
	for (i = 0; i <= iLen-1; i++) {
		pStr[i]  = pStr[i] ^ (cKey);
	}

	return TRUE;
}


BOOL CMisc::bCheckValidName(char *pStr)
{
 int i, iLen;
 
	iLen = strlen(pStr);
	for (i = 0; i < iLen; i++) {
		// 특수 문자가 들어가 있는 경우 거부 
		if ( (pStr[i] == ',')  || (pStr[i] == '=')  || (pStr[i] == ' ') ||
			 (pStr[i] == '\n') || (pStr[i] == '\t') || /*(pStr[i] == '.') ||*/
			 (pStr[i] == '\\') || (pStr[i] == '/')  || (pStr[i] == ':') || 
			 (pStr[i] == '*')  || (pStr[i] == '?')  || (pStr[i] == '<') || 
			 (pStr[i] == '>')  || (pStr[i] == '|')  || (pStr[i] == '"') ) return FALSE;

		if ((i <= iLen-2) && ((unsigned char)pStr[i] >= 128)) {
			if (((unsigned char)pStr[i] == 164) && ((unsigned char)pStr[i+1] >= 161) && 
				((unsigned char)pStr[i+1] <= 211)) {
				// 적합	
				
			}
			else
			if (((unsigned char)pStr[i] >= 176) && ((unsigned char)pStr[i] <= 200) && 
				((unsigned char)pStr[i+1] >= 161) && ((unsigned char)pStr[i+1] <= 254)) {
				// 적합 
				
			}
			else return FALSE;
			i++; // !!! 증가시켜야만 맞다.
		}
	}

	return TRUE;
}


