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

BOOL CGame::bCheckIfIsFlagCarrier(int iClientH)
{
    if (!m_bIsCTFMode) return false;
    if (m_iFlagCarrierIndex == -1) return false;
    if (m_pClientList[iClientH] == NULL) return false;

    if (iClientH == m_iFlagCarrierIndex) return true;
    else return false;
}
void CGame::SetFlagCarrierFlag(int iClientH, BOOL bFlagMode)
{
    if (!m_bIsCTFMode) return;
    if (m_pClientList[iClientH] == NULL) return;

    if (bFlagMode)
    {
        PutLogList("Capture The Flag: New Flag Carrier!");
        m_iFlagCarrierIndex = iClientH;
        if (m_pClientList[iClientH]->m_cSide == 1)
        {
            m_bIsElvineFlagCaptured = true;
            for (int i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != NULL) SendNotifyMsg(NULL, i, DEF_NOTIFY_ARESDENCAPTUREDELVINEFLAG, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        }
        else
        {
            m_bIsAresdenFlagCaptured = true;
            for (int i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != NULL) SendNotifyMsg(NULL, i, DEF_NOTIFY_ELVINEFLAGBACKTOCH, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        }
        m_pClientList[iClientH]->m_iEnemyKillCount += 20;
        SendNotifyMsg(NULL, iClientH, DEF_NOTIFY_ENEMYKILLREWARD, iClientH, NULL, NULL, NULL);
        if (m_pClientList[iClientH]->m_cSide == 1) m_pClientList[iClientH]->m_iStatus = m_pClientList[iClientH]->m_iStatus | 0x80000; //Aresden Flag
        else if (m_pClientList[iClientH]->m_cSide == 2) m_pClientList[iClientH]->m_iStatus = m_pClientList[iClientH]->m_iStatus | 0x40000; //Elvine Flag
    }
    else
    {
        PutLogList("Capture The Flag: Flag Carrier Reset.");
        m_iFlagCarrierIndex = -1;
        if (m_pClientList[iClientH]->m_cSide == 1)
        {
            m_bIsElvineFlagCaptured = false;
            for (int i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != NULL) SendNotifyMsg(NULL, i, DEF_NOTIFY_ELVINECAPTUREDARESDENFLAG, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        }
        else
        {
            m_bIsAresdenFlagCaptured = false;
            for (int i = 0; i < DEF_MAXCLIENTS; i++) if (m_pClientList[i] != NULL) SendNotifyMsg(NULL, i, DEF_NOTIFY_ARESDENFLAGBACKTOCH, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        }
        m_pClientList[iClientH]->m_iStatus = m_pClientList[iClientH]->m_iStatus | 0x00001000;
    }
}
void CGame::EndCaptureTheFlag(int iOption)
{
    if (!m_bIsCTFMode) return;
    int i;
    switch (iOption)
    {
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

        PutLogList("(!) CTFGUID(%d) file created");
        fclose(pFile);
    }
}