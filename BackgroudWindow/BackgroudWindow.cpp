// BackgroudWindow.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "BackgroudWindow.h"

#define MAX_LOADSTRING 100
#pragma comment (lib, "Gdiplus.lib") 
#pragma comment (lib, "comctl32")
// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
int count = 0;
bool isPaint = true;
RECT rect;
HWND hwnd;
HWND hwndPB;
int SWidth = 0;
int SHeight = 0;
int pStat = 0;
LPWSTR bgImgPath = NULL;
LPWSTR logoImgPath = NULL;
wchar_t wstr[50] = { 0 };

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return 1;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return 1;
}

LRESULT CALLBACK MessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return 1;
}

wchar_t* char2wchar(const char* str, size_t len)
{
	memset(wstr, 0, 50*sizeof(wchar_t));
	mbstowcs_s(NULL, wstr, str, len + 1);
	return wstr;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	int argc = 0;
	LPWSTR* lpwArgv = NULL;
	lpwArgv = CommandLineToArgvW(lpCmdLine, &argc);
	if (argc == 2) {
		bgImgPath = lpwArgv[0];
		logoImgPath = lpwArgv[1];
	}
	else {
		//MessageBoxA(NULL, "需要两个参数，背景图片和logo图片", "错误", MB_OK);
		char path[] = "E:\default_bk.jpg";
		wchar_t wpath[20];
		mbstowcs_s(NULL, wpath, path, strlen(path) + 1);
		bgImgPath = wpath;
		char logoPath[] = "E:\glogo.png";
		wchar_t wlogopath[20];
		mbstowcs_s(NULL, wlogopath, logoPath, strlen(logoPath) + 1);
		logoImgPath = wlogopath;
		//return 0;
	}
	

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	ULONG_PTR token;
	Gdiplus::GdiplusStartupInput input = { 0 };
	input.GdiplusVersion = 1;
	Gdiplus::GdiplusStartup(&token, &input, NULL);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BACKGROUDWINDOW, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
	
	//SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, hInstance, GetCurrentThreadId());
	//SetWindowsHookEx(WH_MSGFILTER, MessageProc, hInstance, GetCurrentThreadId());
    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BACKGROUDWINDOW));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
	Gdiplus::GdiplusShutdown(token);
    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BACKGROUDWINDOW));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW +1);
	wcex.lpszMenuName = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中
   SWidth = GetSystemMetrics(SM_CXSCREEN);
   SHeight = GetSystemMetrics(SM_CYSCREEN);
   HWND hWnd = CreateWindowExW(WS_EX_NOACTIVATE, szWindowClass, szTitle, WS_POPUP,
      0, 0, SWidth, SHeight, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   hwnd = hWnd;
   SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
   ShowWindow(hWnd, SW_SHOWNOACTIVATE);
   //UpdateWindow(hWnd);
   
   return TRUE;
}

void drawWord(HDC hdc, LPCSTR str)
{	
	HFONT hFont = CreateFont(70, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, MAC_CHARSET, OUT_STRING_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("微软雅黑"));
	
	SelectObject(hdc, hFont);
	SetRect(&rect, (SWidth*75)/100, (SHeight*84)/100, (SWidth*75)/100+450, (SHeight*84)/100+70);
	
	SetTextColor(hdc, RGB(0, 230, 30));
	//FillRect(hdc, &rect, brush);
	SetBkMode(hdc, TRANSPARENT);
	DrawTextA(hdc, str, -1, &rect, DT_LEFT);
}

void drawOutWord(HDC hdc, LPCSTR str)
{
	HFONT hFont = CreateFont(70, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, MAC_CHARSET, OUT_STRING_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Impact"));

	SelectObject(hdc, hFont);
	SetTextColor(hdc, RGB(0, 230, 30));
	//FillRect(hdc, &rect, brush);
	SetBkMode(hdc, TRANSPARENT);
	TextOutA(hdc, (SWidth * 75) / 100, (SHeight * 84) / 100, str, strlen(str));
}

void drawString(HDC hdc, WCHAR* str)
{
	Gdiplus::Graphics graphics(hdc);
	Gdiplus::SolidBrush brush(Gdiplus::Color(255, 0, 255, 0));
	Gdiplus::FontFamily fontFamily(L"Times New Roman");
	Gdiplus::Font font(&fontFamily, 45, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::StringFormat sf;
	sf.SetAlignment(Gdiplus::StringAlignmentNear);
	//Gdiplus::PointF pointF((SWidth * 0.76f), (SHeight * 0.842f));
	Gdiplus::RectF grect(SWidth * 0.76f, SHeight * 0.842f, 400.0, 70.0);
	//SetRect(&rect, SWidth * 0.76f, SHeight * 0.842f, SWidth * 0.76f + 400.0, SHeight * 0.842f + 70.0);
	graphics.DrawString(str, -1, &font, grect, &sf, &brush);
	//graphics.Clear(Gdiplus::Color(0, 255, 255, 0));
}

void drawLogo(HDC hdc)
{
	Gdiplus::Graphics graphics(hdc);
	Gdiplus::Image image(logoImgPath);
	Gdiplus::Rect destRect(65, 65, 400, 100);
	graphics.DrawImage(&image, destRect);
}

void drawBackground(HDC hdc)
{
	Gdiplus::Graphics graphics(hdc);
	Gdiplus::Image image(bgImgPath);
	Gdiplus::Rect destRect(0, 0, SWidth, SHeight);
	graphics.DrawImage(&image, destRect);
}

void drawText(HDC hdc)
{
	count++;
	if (count % 4 == 1) {
		drawWord(hdc, "游戏玩命加载中.");
	}
	else if (count % 4 == 2) {
		drawWord(hdc, "游戏玩命加载中..");
	}
	else if (count % 4 == 3) {
		drawWord(hdc, "游戏玩命加载中...");
	}
	else {
		count = 0;
		drawWord(hdc, "游戏玩命加载中");
	}
}

void drawTextString(HDC hdc)
{
	count++;
	if (count % 4 == 1) {
		wchar_t str[] = L"游戏玩命加载中.";
		drawString(hdc, str);
		isPaint = false;
	}
	else if (count % 4 == 2) {
		wchar_t str[] = L"游戏玩命加载中..";
		drawString(hdc, str);
		isPaint = false;
	}
	else if (count % 4 == 3) {
		wchar_t str[] = L"游戏玩命加载中...";
		drawString(hdc, str);
		isPaint = true;
	}
	else {
		count = 0;
		wchar_t str[] = L"游戏玩命加载中";
		drawString(hdc, str);
		isPaint = false;
	}
}

void paint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	HDC mdc = CreateCompatibleDC(hdc);
	HBITMAP hBMMem = CreateCompatibleBitmap(hdc, SWidth, SHeight);
	SelectObject(mdc, hBMMem);
	drawBackground(mdc);
	drawLogo(mdc);
	drawText(mdc);
	BitBlt(hdc, 0, 0, SWidth, SHeight, mdc, 0, 0, SRCCOPY);
	DeleteObject(hBMMem);
	DeleteDC(mdc);
	DeleteDC(hdc);
	EndPaint(hWnd, &ps);

	InvalidateRect(hWnd, &rect, false);
	Sleep(1000);
	//RedrawWindow(hWnd, NULL, NULL, RDW_INTERNALPAINT|RDW_INVALIDATE|RDW_UPDATENOW);
}

void rePaint(HWND hWnd) {
	HDC hdc = GetDC(hWnd);
	HDC mdc = CreateCompatibleDC(hdc);
	HBITMAP hBMMem = CreateCompatibleBitmap(hdc, SWidth, SHeight);
	SelectObject(mdc, hBMMem);
	drawBackground(mdc);
	BitBlt(hdc, 0, 0, SWidth, SHeight, mdc, 0, 0, SRCCOPY);
	DeleteObject(hBMMem);
	DeleteDC(mdc);
	ReleaseDC(hWnd, hdc);
}

void drawDyWord(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	//ps.rcPaint = rect;
	drawBackground(hdc);
	count++;
	if (count % 4 == 1) {
		drawWord(hdc, "游戏玩命加载中.");
	}
	else if (count % 4 == 2) {
		drawWord(hdc, "游戏玩命加载中..");
	}
	else if (count % 4 == 3) {
		drawWord(hdc, "游戏玩命加载中...");
	}
	else {
		count = 0;
		drawWord(hdc, "游戏玩命加载中");
	}
	EndPaint(hWnd, &ps);
	Sleep(1000);
	InvalidateRect(hWnd, &rect, false);
	//RedrawWindow(hWnd, &rect, NULL, RDW_INTERNALPAINT | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}

DWORD WINAPI PBThreadProc(LPVOID lpParameter)
{
	HWND hwndpb = (HWND)lpParameter;
	PBRANGE range;
	long long cur = 0;
	SendMessage(hwndpb, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
	SendMessage(hwndpb, PBM_GETRANGE, TRUE, (LPARAM)&range);
	//SendMessage(hwndpb, PBM_SETSTEP, 1, 0);
	while (1) {
		SendMessage(hwndpb, PBM_DELTAPOS, (range.iHigh - range.iLow)/100, 0);
		cur = SendMessage(hwndpb, PBM_GETPOS, 0, 0);
		if (cur < 1) {
			HDC hdc = GetDC(hwndpb);
			drawBackground(hdc);
		}
		if (cur > 0 && cur < 50) {
			Sleep(400);
		}
		else if (cur >= 50 && cur < 90) {
			Sleep(1500);
		}
		else if(cur >= 90 && cur < 98){
			Sleep(7500);
		}
		else {
			Sleep(20000);
		}
		if (cur == range.iHigh) {
			//SendMessage(hwndpb, PBM_SETPOS, range.iLow, 0);
			isPaint = false;
			rePaint(hwnd);
			return 0;
		}
	}
}

DWORD WINAPI PaintProc(LPVOID lpParrameter)
{
	HWND hWnd = (HWND)lpParrameter;
	while (1) {
		HDC hdc = GetDC(hWnd);
		HDC mdc = CreateCompatibleDC(hdc);
		HBITMAP hBMMem = CreateCompatibleBitmap(hdc, SWidth, SHeight);
		SelectObject(mdc, hBMMem);
		drawBackground(mdc);
		drawLogo(mdc);
		drawText(mdc);
		BitBlt(hdc, 0, 0, SWidth, SHeight, mdc, 0, 0, SRCCOPY);
		DeleteObject(hBMMem);
		DeleteDC(mdc);
		ReleaseDC(hWnd, hdc);
		Sleep(1000);
		InvalidateRect(hWnd, &rect, false);
	}
	return 0;
}
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char buff[50] = { 0 };
	sprintf_s(buff, "@@@  %ud  %ud %ld\n", message, wParam, lParam);
	OutputDebugStringA(buff);
    switch (message)
    {
    case WM_COMMAND:
        {
            //int wmId = LOWORD(wParam);
            //// 分析菜单选择:
            //switch (wmId)
            //{
            //case IDM_ABOUT:
            //    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            //    break;
            //case IDM_EXIT:
            //    DestroyWindow(hWnd);
            //    break;
            //default:
            //    return DefWindowProc(hWnd, message, wParam, lParam);
            //}
        }
        break;
	case WM_CREATE:
	{
		SetWindowsHookEx(WH_MOUSE, MouseProc, GetModuleHandle(NULL), GetCurrentThreadId());
		SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, GetModuleHandle(NULL), GetCurrentThreadId());
		InitCommonControls();
		hwndPB = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE| PBS_SMOOTH, 0, 0, 0, 0,
			hWnd, 0, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		CreateThread(NULL, 0, PBThreadProc, hwndPB, 0, 0);
		//CreateThread(NULL, 0, PaintProc, hWnd, 0, 0);
		return 0;
	}
	case WM_SIZE:
	{
		RECT rc;
		SetRect(&rc, (SWidth*281)/1000, (SHeight*74)/100, (SWidth*713)/1000, (SHeight*111)/100);
		MoveWindow(hwndPB, rc.left, rc.top, rc.right - rc.left, (rc.bottom - rc.top) / 10, false);
		return 0;
	}
	case WM_PAINT:
	{
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		//FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		if (isPaint) {
			paint(hWnd);
			SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
		}
	}
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_ERASEBKGND:
		return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}