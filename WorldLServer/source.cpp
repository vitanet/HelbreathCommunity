#include <windows.h>
#include "source.h"
#include "WorldLog.h"
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include "resource.h"

#define DEF_UPPERVERSION 3
#define DEF_LOWERVERSION 51
//Default stuff
char		szAppClass[1000];
WNDCLASS	wnd;
HWND		G_hWnd;
WSADATA		wsInfo;
//MSG			msg;

//Message Processing On Screen
BOOL G_cMsgUpdated = FALSE;
char G_cMsgList[120*50];
char            G_cTxt[500];
//Main Processing Class .....WORLDLOG!!!!
class CWorldLog *G_pWorldLog;

//Timer
MMRESULT        G_mmTimer = NULL;
static HWND    List1, Button1, Button2;
static HINSTANCE BCX_hInstance;
static int     BCX_ScaleX, BCX_ScaleY;
static char    BCX_ClassName[2048];
HWND    BCX_Listbox(char*,HWND,int,int,int,int,int,int=0,int=-1);
HWND    BCX_Button(char*,HWND,int,int,int,int,int,int=0,int=-1);
HWND    BCX_Button2(char*,HWND,int,int,int,int,int,int=0,int=-1);
HWND hSv;

HINSTANCE hInst;

LRESULT CALLBACK	Server(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	Clients(HWND, UINT, WPARAM, LPARAM);

int ItemCount=0;
//Sockets
class XSocket * G_pInternalLogSock = NULL;

LRESULT CALLBACK WindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg){
		//---------Buttons---------//
	case WM_COMMAND:
		switch(wParam){
			case 1010:
					G_cMsgUpdated = TRUE;
					SendMessage(List1,(UINT)LB_RESETCONTENT, 0, 0);
					ItemCount = 0;
					PutLogList("Cleared");
					break;
			case 1011:
				 /*char pData[120];
	G_pWorldLog->UpdateNoticement(wParam);
		if (!G_bIsMainLogActive) {
			G_bIsMainLogActive = TRUE;
			G_pXWorldLogSock = new class XSocket(G_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
			G_pXWorldLogSock->bListen(G_pWorldLog->m_cWorldServerAddress, G_pWorldLog->m_iWorldServerPort, WM_WORLD_LOG_LISTENER);
			PutLogList("(!) Log-Socket Listening... Server Activated.");
			ZeroMemory(pData, sizeof(pData));
			memcpy(pData, G_pWorldLog->m_cWorldLogName, 30);
			G_pWorldLog->SendEventToMLS(MSGID_WORLDSERVERACTIVATED, 0, pData, 30, -1);
		}*/
				break;
	case IDM_SERVER:
		DialogBox(hInst, (LPCTSTR)IDD_SERVERS, G_hWnd, (DLGPROC)Server);
		break;
	case IDM_CLIENT:
		DialogBox(hInst, (LPCTSTR)IDD_SERVERS, G_hWnd, (DLGPROC)Clients);
		break;
				 }
				 break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "Quit World log server?", "WORLD-LOG", MB_YESNO + MB_ICONINFORMATION) == IDYES) {
			return DefWindowProc(hWnd, WM_CLOSE, wParam, lParam);
		}
		break;
		
	case WM_DESTROY:
		OnDestroy();
		break;

	case WM_PAINT:
		OnPaint();
		break;

	case WM_USER_TIMERSIGNAL:
		G_pWorldLog->OnTimer();
		break;

	case WM_WORLD_LOG_LISTENER:
		G_pWorldLog->bAccept(G_pXWorldLogSock, TRUE);
		break;

	case WM_INTERNAL_LOG_ACCEPT:
		G_pWorldLog->bAccept(G_pInternalLogSock, FALSE);
		break;

	case WM_KEYUP:
		OnKeyUp(wParam, lParam);
		return DefWindowProc(hWnd, WM_KEYUP, wParam, lParam);

	default: 
		if ((uMsg >= WM_ONMAINLOGSOCKETEVENT) && (uMsg <= WM_ONMAINLOGSOCKETEVENT + DEF_MAXMAINLOGSOCK)) {
			G_pWorldLog->OnMainSubLogSocketEvent(uMsg, wParam, lParam);
		}
		else if ((uMsg >= WM_ONCLIENTSOCKETEVENT) && (uMsg <= WM_ONCLIENTSOCKETEVENT + DEF_MAXCLIENTSOCK)) {
			G_pWorldLog->OnClientSubLogSocketEvent(uMsg, wParam, lParam);
		}
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	wsprintf(szAppClass, "World Log Server%d", hInstance);
	if (!InitApplication( hInstance)) return (FALSE);
	if (!InitInstance(hInstance, nCmdShow)) return (FALSE);
	Initialize();
	EventLoop();
	return 0;
}

BOOL InitApplication( HINSTANCE hInstance)
{     
 WNDCLASS  wc;

	wc.style = (CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS);
	wc.lpfnWndProc   = (WNDPROC)WindowProc;             
	wc.cbClsExtra    = 0;                            
	wc.cbWndExtra    = sizeof (int);             
	wc.hInstance     = hInstance;                    
	wc.hIcon         = NULL;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);  
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+7);	 
	wc.lpszMenuName  = (LPCSTR)IDR_MENU1;                    		 
	wc.lpszClassName = szAppClass;                   

	return (RegisterClass(&wc));
}

BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
 char cTitle[100];

	wsprintf(cTitle, "Helbreath World-Log-Server V%d.%d - KLKS and Hypnotoad", DEF_UPPERVERSION, DEF_LOWERVERSION);

	G_hWnd = CreateWindowEx(0,  // WS_EX_TOPMOST,
		szAppClass,
		cTitle,
		WS_VISIBLE | // so we don't have to call ShowWindow
		WS_POPUP |   // non-app window
		WS_CAPTION | // so our menu doesn't look ultra-goofy
		WS_SYSMENU |  // so we get an icon in the tray
		WS_MINIMIZEBOX, 
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800, //GetSystemMetrics(SM_CXSCREEN),
		400, //GetSystemMetrics(SM_CYSCREEN),
		NULL,
		NULL,
		hInstance,
		NULL );

	if (!G_hWnd) return (FALSE);
	//MAJORHG - CLEROTH - 26/03/05
    WNDCLASS Wc;
	strcpy(BCX_ClassName,"ListBox1");
	BCX_ScaleX       = 1;
	BCX_ScaleY       = 1;
	BCX_hInstance    =  hInstance;
	Wc.style         =  CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	Wc.cbClsExtra    =  0;
	Wc.cbWndExtra    =  0;
	Wc.hInstance     =  hInstance;
	Wc.hIcon         =  LoadIcon(NULL,IDI_WINLOGO);
	Wc.hCursor       =  LoadCursor(NULL,IDC_ARROW);
	Wc.hbrBackground =  (HBRUSH)(COLOR_BTNFACE+1);
	Wc.lpszMenuName  =  NULL;
	Wc.lpszClassName =  BCX_ClassName;
	RegisterClass(&Wc);
	//----------Main CMD--------//
	List1=BCX_Listbox("",G_hWnd,1009,20,45,520,200);
	Button1=BCX_Button("",G_hWnd,1010,100,245,80,20);
	Button2=BCX_Button2("",G_hWnd,1011,20,245,80,20);
	//------------------------------//

	ShowWindow(G_hWnd, nCmdShow);    
	UpdateWindow(G_hWnd);            
	return (TRUE);                 
}

int EventLoop()
{
 static unsigned short _usCnt = 0; 
 register MSG msg;

	while(1) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if(!GetMessage(&msg, NULL, 0, 0)) {
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

	G_pWorldLog = new class CWorldLog(G_hWnd);
	if (G_pWorldLog == NULL) {
		MessageBox(G_hWnd, "Init fail - Invalid WorldLServer!","ERROR",MB_OK+MB_ICONERROR);
		return;
	}
	if (G_pWorldLog->bInit() == FALSE) {
		MessageBox(G_hWnd, "Init fail - Missing required .cfg files!","ERROR",MB_OK+MB_ICONERROR);
		return;
	}

	G_pInternalLogSock = new class XSocket(G_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
	G_pInternalLogSock->bListen(G_pWorldLog->m_cWorldServerAddress, G_pWorldLog->m_iWorldServerInternalPort, WM_INTERNAL_LOG_ACCEPT);
	PutLogList("(!) Internal-log-Socket Listening...");
	G_mmTimer = _StartTimer(300);
	
}

void OnDestroy()
{
	G_pWorldLog->CleanupLogFiles();
	if (G_pWorldLog != NULL) delete G_pWorldLog;
	if (G_pXWorldLogSock != NULL) delete G_pXWorldLogSock;
	if (G_pInternalLogSock != NULL) delete G_pInternalLogSock;
	_TermWinsock();
	if (G_mmTimer != NULL) _StopTimer(G_mmTimer);
	PostQuitMessage(0);
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
 register short i;
 char * cMsg;

	hdc = BeginPaint(G_hWnd, &ps);
	SetBkMode(hdc,TRANSPARENT);
	for (i = 0; i < 20; i++) {
		cMsg = (char *)(G_cMsgList + i*120);
		TextOut(hdc, 5, 5 + 350 - i*16, cMsg, strlen(cMsg));
	}
	if (G_bIsMainLogActive == TRUE) {
		wsprintf(G_cTxt,"Total Players(%d)   Main-Log-Sock Active(%d/%d)", G_pWorldLog->m_iTotalPlayers,  G_pWorldLog->m_iActiveMainLogSock, G_pWorldLog->m_iTotalMainLogSock);
	}
	else {
		wsprintf(G_cTxt,"PRESS [HOME] KEY TO ACTIVATE WORLD SERVER!!!");
	}
	TextOut(hdc, 5, 10, G_cTxt, strlen(G_cTxt));
	if (G_pWorldLog != NULL) {
		TextOut(hdc, 575, 10, "World Name", strlen("World Name"));
		TextOut(hdc, 575, 25, "World Addr", strlen("World Addr"));
		TextOut(hdc, 575, 40, "World Port", strlen("World Port"));
		TextOut(hdc, 575, 55, "World iPort", strlen("World iPort"));
		TextOut(hdc, 575, 70, "Main Addr", strlen("Main Addr"));
		TextOut(hdc, 575, 85, "Main Port", strlen("Main Port"));
		TextOut(hdc, 665, 10, ":", 1);
		TextOut(hdc, 665, 25, ":", 1);
		TextOut(hdc, 665, 40, ":", 1);
		TextOut(hdc, 665, 55, ":", 1);
		TextOut(hdc, 665, 70, ":", 1);
		TextOut(hdc, 665, 85, ":", 1);
		TextOut(hdc, 675, 10, G_pWorldLog->m_cWorldLogName, strlen(G_pWorldLog->m_cWorldLogName));
		TextOut(hdc, 675, 25, G_pWorldLog->m_cWorldServerAddress, strlen(G_pWorldLog->m_cWorldServerAddress));
		wsprintf(G_cTxt, "%d",G_pWorldLog->m_iWorldServerPort);
		TextOut(hdc, 675, 40, G_cTxt, strlen(G_cTxt));
		wsprintf(G_cTxt, "%d",G_pWorldLog->m_iWorldServerInternalPort);
		TextOut(hdc, 675, 55, G_cTxt, strlen(G_cTxt));
		TextOut(hdc, 675, 70, G_pWorldLog->m_cMainLogAddress, strlen(G_pWorldLog->m_cMainLogAddress));
		wsprintf(G_cTxt, "%d",G_pWorldLog->m_iMainLogPort);
		TextOut(hdc, 675, 85, G_cTxt, strlen(G_cTxt));
	}
	
	EndPaint(G_hWnd, &ps);
}

void PutGameLogData(char * cMsg)
{
 FILE * pFile;
 char cBuffer[512];
 SYSTEMTIME SysTime;

	pFile = NULL;
	ZeroMemory(cBuffer, sizeof(cBuffer));
	_mkdir("GameLogData");
	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "GameLogData\\GameEvents%04d%02d%02d.log", SysTime.wYear, SysTime.wMonth, SysTime.wDay);
	pFile = fopen(cBuffer, "at");
	if (pFile == NULL) return;
	ZeroMemory(cBuffer, sizeof(cBuffer));
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cMsg);
	strcat(cBuffer, "\n");
	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);

}

void PutEventLog(char * cMsg)
{
 FILE * pFile;
 char cBuffer[512];
 SYSTEMTIME SysTime;

	pFile = fopen("Events.log", "at");
	if (pFile == NULL) return;
	ZeroMemory(cBuffer, sizeof(cBuffer));
	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cMsg);
	strcat(cBuffer, "\n");
	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}

void PutLogList(char * cMsg)
{
 char cTemp[6000];

	G_cMsgUpdated = TRUE;
/*	ZeroMemory(cTemp, sizeof(cTemp));
	memcpy((cTemp + 120), G_cMsgList, 5880);
	memcpy(cTemp, cMsg, strlen(cMsg));
	memcpy(G_cMsgList, cTemp, 6000);*/
	if (DEF_DEBUGMODE) PutEventLog(cTemp);
	SendMessage(List1,(UINT)LB_ADDSTRING,(WPARAM)0,(LPARAM)cMsg);
	SendMessage(List1,(UINT)LB_SETCURSEL,ItemCount,0);
	ItemCount++;
}
void PutLogServ(char * cMsg)
{
	G_cMsgUpdated = TRUE;
	SendDlgItemMessage(hSv, IDC_LIST1, (UINT)LB_ADDSTRING, (WPARAM)0, (LPARAM)cMsg);
	SendDlgItemMessage(hSv, IDC_LIST1,(UINT)LB_SETCURSEL,ItemCount,0);
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

void OnKeyUp(WPARAM wParam, LPARAM lParam)
{
 char pData[120];

	G_pWorldLog->UpdateNoticement(wParam);
	switch(wParam) {

	case VK_HOME:
		if (!G_bIsMainLogActive) {
			G_bIsMainLogActive = TRUE;
			G_pXWorldLogSock = new class XSocket(G_hWnd, DEF_SERVERSOCKETBLOCKLIMIT);
			G_pXWorldLogSock->bListen(G_pWorldLog->m_cWorldServerAddress, G_pWorldLog->m_iWorldServerPort, WM_WORLD_LOG_LISTENER);
			PutLogList("(!) Log-Socket Listening... Server Activated.");
			ZeroMemory(pData, sizeof(pData));
			memcpy(pData, G_pWorldLog->m_cWorldLogName, 30);
			G_pWorldLog->SendEventToMLS(MSGID_WORLDSERVERACTIVATED, 0, pData, 30, -1);
		}
		break;
	}
}
//----------------------------MAIN CMD!---------------------------------//
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
        A = CreateWindowEx(Exstyle,"Listbox",NULL,Style,
        X*BCX_ScaleX, Y*BCX_ScaleY, W*BCX_ScaleX, H*BCX_ScaleY,
        hWnd,(HMENU)id,BCX_hInstance,NULL);
        SendMessage(A,(UINT)WM_SETFONT,(WPARAM)GetStockObject
        (DEFAULT_GUI_FONT),(LPARAM)MAKELPARAM(FALSE,0));
        return A;
}
HWND BCX_Button(char* Text,HWND hWnd,int id,int X,int Y,int W,int H,int Style,int Exstyle)
{
        HWND  A;
        if (!Style)
        {
                Style = WS_CHILD | WS_VISIBLE | WS_BORDER;
        }
        A = CreateWindowEx(0,"Button","Clear",Style,
        X*BCX_ScaleX, Y*BCX_ScaleY, W*BCX_ScaleX, H*BCX_ScaleY,
        hWnd,(HMENU)id,BCX_hInstance,NULL);
        SendMessage(A,(UINT)WM_SETFONT,(WPARAM)GetStockObject
        (DEFAULT_GUI_FONT),(LPARAM)MAKELPARAM(FALSE,0));
        return A;
}
HWND BCX_Button2(char* Text,HWND hWnd,int id,int X,int Y,int W,int H,int Style,int Exstyle)
{
        HWND  A;
        if (!Style)
        {
                Style = WS_CHILD | WS_VISIBLE | WS_BORDER;
        }
        A = CreateWindowEx(0,"Button","Start",Style,
        X*BCX_ScaleX, Y*BCX_ScaleY, W*BCX_ScaleX, H*BCX_ScaleY,
        hWnd,(HMENU)id,BCX_hInstance,NULL);
        SendMessage(A,(UINT)WM_SETFONT,(WPARAM)GetStockObject
        (DEFAULT_GUI_FONT),(LPARAM)MAKELPARAM(FALSE,0));
        return A;
}
//--------------------dialog boxes---------------//
// Mesage handler for about box.
LRESULT CALLBACK Server(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	hSv = hDlg;
	switch (message)
	{
		case WM_INITDIALOG:
			G_pWorldLog->ServerList(TRUE);
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			switch(wParam){
			case IDC_REMOVE:
			char cWhat[100];
			GetDlgItemText(hDlg, IDC_EDIT1, cWhat, 100);
			if (cWhat != NULL) {
				SetDlgItemText(hDlg, IDC_EDIT1, NULL);
				SetFocus(hDlg);
				G_pWorldLog->ParseCommand(TRUE, cWhat);
			}
			break;
			}
			break;
	}
    return FALSE;
}
LRESULT CALLBACK Clients(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	hSv = hDlg;
	switch (message)
	{
		case WM_INITDIALOG:
			G_pWorldLog->ServerList(FALSE);
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			switch(wParam){

		case IDC_REMOVE:
				//GetCurSel() - selected....
				//LB_DELETESTRING
			char cWhat[100];
			GetDlgItemText(hDlg, IDC_EDIT1, cWhat, 100);
			if (cWhat != NULL) {
				SetDlgItemText(hDlg, IDC_EDIT1, NULL);
				SetFocus(hDlg);
				G_pWorldLog->ParseCommand(FALSE, cWhat);
			}
			break;

			}
			break;
	}
    return FALSE;
}