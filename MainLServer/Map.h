#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "NetMessages.h"


class CMap  
{
public:
	CMap (int iClientH, char *pData);
	virtual ~CMap ();
	char  m_cMapName[31];
	int	  iIndex; //HG server ID
};