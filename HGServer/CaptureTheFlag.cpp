#include "CaptureTheFlag.h"

extern void PutLogList(char* cMsg);
extern char G_cTxt[512];
extern char	G_cData50000[50000];
extern void PutLogFileList(char* cStr);
extern void PutAdminLogFileList(char* cStr);
extern void PutItemLogFileList(char* cStr);
extern void PutLogEventFileList(char* cStr);
extern void PutHackLogFileList(char* cStr);
extern void PutPvPLogFileList(char* cStr);
extern FILE* pLogFile;
extern HWND	G_hWnd;

#pragma warning (disable : 4996 6011 6001 4244 4018 6385 6386 26451 6054 4267 6053 6031)

//50Cent - Capture The Flag
void CGame::StartCaptureTheFlag()
{
    if (m_bIsCTFMode) return;

    m_sFlagCountWin = 5;

    wsprintf(G_cTxt, "Capture The Flag: Win Flag Count(%d)", m_sFlagCountWin);
    PutLogList(G_cTxt);

    m_bIsCTFMode = true;

    for (int i = 0; i < DEF_MAXCLIENTS; i++)
    {
        if (m_pClientList[i] != NULL)
        {
            SendNotifyMsg(NULL, i, DEF_NOTIFY_CAPTURETHEFLAGSTART, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        }
    }
    PutLogList("(*) Capture The Flag Started!");
}

void CGame::RequestCheckFlag(int iClientH)
{
    if (!m_bIsCTFMode) return;
    if (m_pClientList[iClientH] == NULL) return;

    if (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "elvine") == 0) 
    {
        if ((m_pClientList[iClientH]->m_sX == 148) && (m_pClientList[iClientH]->m_sY == 130))//Elvine Flag Cords
        {
            if (m_pClientList[iClientH]->m_iLevel >= 120)
            {
                if (m_pClientList[iClientH]->m_cSide == 1) //Aresden Side
                {
                    if ((!m_bIsAresdenFlagCaptured) && (!m_bIsElvineFlagCaptured)) //Aresden Capture Elvine Flag
                    {
                        SetFlagCarrierFlag(iClientH, true);
                    }
                }
                else //Elvine Side
                {
                    if ((m_bIsAresdenFlagCaptured) && (!m_bIsElvineFlagCaptured))
                    {
                        m_sElvineFlagCount++;
                        m_bIsAresdenFlagCaptured = false;
                        for (int i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != NULL) SendNotifyMsg(NULL, i, DEF_NOTIFY_ELVINEWINSROUND, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                        if (m_sElvineFlagCount == 5)
                        {
                            EndCaptureTheFlag(2);
                        }
                    }
                }
            }
        }
    }
    else if (strcmp(m_pMapList[m_pClientList[iClientH]->m_cMapIndex]->m_cLocationName, "aresden") == 0) 
    {
        if ((m_pClientList[iClientH]->m_sX == 148) && (m_pClientList[iClientH]->m_sY == 126))//Aresden Flag Cords
        {
            if (m_pClientList[iClientH]->m_iLevel >= 120)
            {
                if (m_pClientList[iClientH]->m_cSide == 2) //Elvine Side
                {
                    if ((!m_bIsElvineFlagCaptured) && (!m_bIsAresdenFlagCaptured)) //Elvine Capture Aresden Flag
                    {
                        SetFlagCarrierFlag(iClientH, true);
                    }
                }
                else //Aresden Side
                {
                    if ((m_bIsElvineFlagCaptured) && (!m_bIsAresdenFlagCaptured))
                    {
                        m_sAresdenFlagCount++;
                        m_bIsElvineFlagCaptured = false;
                        for (int i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != NULL) SendNotifyMsg(NULL, i, DEF_NOTIFY_ARESDENWINSROUND, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                        if (m_sAresdenFlagCount == 5)
                        {
                            EndCaptureTheFlag(1);
                        }
                    }
                }
            }
        }
    }
}

BOOL CGame::bCheckIfIsFlagCarrier(int iClientH)
{
    if (!m_bIsCTFMode) return false;
    if (m_pClientList[iClientH] == NULL) return false;

    if (iClientH == m_iFlagCarrierIndex) return true;
    else return false;
}
void CGame::SetFlagCarrierFlag(int iClientH, BOOL bFlagMode)
{
    int i;
    if (!m_bIsCTFMode) return;
    if (m_pClientList[iClientH] == NULL) return;

    if (bFlagMode)
    {
        m_iFlagCarrierIndex = iClientH;
        if (m_pClientList[iClientH]->m_cSide == 1)
        {
            m_bIsElvineFlagCaptured = true;
            for (i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != NULL) SendNotifyMsg(NULL, i, DEF_NOTIFY_ARESDENCAPTUREDELVINEFLAG, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        }
        else
        {
            m_bIsAresdenFlagCaptured = true;
            for (i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != NULL) SendNotifyMsg(NULL, i, DEF_NOTIFY_ELVINEFLAGBACKTOCH, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        }
        m_pClientList[iClientH]->m_iStatus = m_pClientList[iClientH]->m_iStatus | 0x00080000; // Flag
    }
    else
    {
        m_iFlagCarrierIndex = -1;
        if (m_pClientList[iClientH]->m_cSide == 1)
        {
            m_bIsElvineFlagCaptured = false;
            for (i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != NULL) SendNotifyMsg(NULL, i, DEF_NOTIFY_ELVINECAPTUREDARESDENFLAG, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        }
        else
        {
            m_bIsAresdenFlagCaptured = false;
            for (i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != NULL) SendNotifyMsg(NULL, i, DEF_NOTIFY_ARESDENFLAGBACKTOCH, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        }
        m_pClientList[iClientH]->m_iStatus = m_pClientList[iClientH]->m_iStatus | 0xFFF7FFFF;
    }
}
void CGame::EndCaptureTheFlag(int iOption)
{
    if (!m_bIsCTFMode) return;
    int i;
    switch (iOption) {
    case 0://Finished by GM
        for (i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != NULL) SendNotifyMsg(NULL, i, DEF_NOTIFY_TIECTF, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        _CreateCTFGUID(0);
        break;
    case 1://Aresden Win
        for (i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != NULL) SendNotifyMsg(NULL, i, DEF_NOTIFY_ARESDENWINCTF, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        _CreateCTFGUID(1);
        break;
    case 2://Elvine Win
        for (i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != NULL) SendNotifyMsg(NULL, i, DEF_NOTIFY_ELVINEWINCTF, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        _CreateCTFGUID(2);
        break;
    }
}
void CGame::_CreateCTFGUID(int iWinnerSide)
{
    char* cp, cTxt[256], cTemp[1024];
    FILE* pFile;

    pFile = fopen("..\\GameConfigs\\CTFGUID.Txt", "wt");
    if (pFile == NULL) {
        PutLogList("(!) Cannot create CTFGUID file");
        return;
    }
    else {
        ZeroMemory(cTemp, sizeof(cTemp));

        ZeroMemory(cTxt, sizeof(cTxt));
        wsprintf(cTxt, "winner-side = %d\n", iWinnerSide);
        strcat(cTemp, cTxt);

        cp = (char*)cTemp;
        fwrite(cp, strlen(cp), 1, pFile);

        PutLogList("(!) CTF GUID file created");
        fclose(pFile);
    }
}