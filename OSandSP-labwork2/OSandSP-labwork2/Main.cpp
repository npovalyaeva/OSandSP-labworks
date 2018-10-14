#include <windows.h>
#include <tchar.h>
#include "resource.h"

#define CLASS_NAME "MainClass" 
#define WINDOW_NAME "Multiplication Table"
#define MENU_CHANGESIZE_TITLE L"Change Size"
#define MENU_ABOUT_TITLE L"About"
#define MENU_ABOUT_TEXT L"Subject: Operating systems and system programming;\r\nAuthor: Nadya Povalyaeva, 651001;\r\nControl: Evgeny Bazylev."

#define COLOR_BACKGROUND RGB(241, 242, 240);
#define COLOR_TABLE RGB(191, 186, 190);
#define COLOR_TABLENUMBER RGB(85, 138, 134);
#define COLOR_TABLEACCENT RGB(166, 62, 20);

HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

VOID AddMenu(HWND);

TCHAR WinClassName[] = _T(CLASS_NAME);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex; HWND hWnd; MSG msg;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = WinClassName;

	RegisterClassEx(&wcex);

	hWnd = CreateWindow(WinClassName, _T(WINDOW_NAME), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, HWND_DESKTOP, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

HDC hdc;
PAINTSTRUCT ps;
RECT backRt;
HBRUSH backgroundBrush;
COLORREF backgroundColor = COLOR_BACKGROUND;
int windowX, windowY;
HMENU hMenu;
int rows, columns;
BOOL isRowsNum, isColumnsNum;


VOID AddMenu(HWND hWnd)
{
	hMenu = CreateMenu();
	AppendMenu(hMenu, MF_STRING, 1, MENU_CHANGESIZE_TITLE);
	AppendMenu(hMenu, MF_STRING, 2, MENU_ABOUT_TITLE);
	SetMenu(hWnd, hMenu);
}

BOOL CheckUserSize()
{
	if (isRowsNum && isColumnsNum)
		return ((rows >= 2 && rows <= 20) && (columns >= 2 && columns <= 20)) ? true : false;
	else return false;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		AddMenu(hWnd);
		backgroundBrush = CreateSolidBrush(backgroundColor);
		break;
	case WM_SIZE:
		windowX = LOWORD(lParam);
		windowY = HIWORD(lParam);
		break;
	case WM_COMMAND:
		switch (wParam) {
		case 1: // Change size
			DialogBox(hInst, MAKEINTRESOURCE(IDD_SIZEDIALOG), hWnd, DlgProc);
			break;
		case 2: // About
			MessageBox(hWnd, MENU_ABOUT_TEXT, MENU_ABOUT_TITLE, MB_OK);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetRect(&backRt, 0, 0, windowX, windowY);
		FillRect(hdc, &backRt, backgroundBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEditRows, hEditColumns, hErrorMessage;
	bool flag = true;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hErrorMessage = GetDlgItem(hDlg, IDC_ERROR);
		SetDlgItemInt(hDlg, IDC_EDITROWS, 10, FALSE);
		SetDlgItemInt(hDlg, IDC_EDITCOLUMNS, 10, FALSE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			rows = GetDlgItemInt(hDlg, IDC_EDITROWS, &isRowsNum, FALSE);
			columns = GetDlgItemInt(hDlg, IDC_EDITCOLUMNS, &isColumnsNum, FALSE);
			if (CheckUserSize())
			{
				InvalidateRect(GetParent(hDlg), NULL, TRUE);
				EndDialog(hDlg, 0);
			}
			else
				ShowWindow(hErrorMessage, SW_SHOW);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			//ExitProcess(0);
			break;
		}
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		EndDialog(hDlg, 0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

