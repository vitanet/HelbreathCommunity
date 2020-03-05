#include "Map.h"

CMap::CMap(int iClientH, char *pData)
{
	ZeroMemory(m_cMapName, sizeof(m_cMapName));
	iIndex = -1; // default if not registered
	strcpy(m_cMapName, pData);
	iIndex = iClientH;						// 0BE1Ch->10h
}
CMap::~CMap()
{

}