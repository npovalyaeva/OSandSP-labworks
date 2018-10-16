#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <fstream>

#define CLASS_NAME "MainClass" 
#define WINDOW_NAME "Multiplication Table"
#define MENU_CHANGESIZE_TITLE L"Change Size"
#define MENU_ABOUT_TITLE L"About"
#define MENU_ABOUT_TEXT L"Subject: Operating systems and system programming;\r\nAuthor: Nadya Povalyaeva, 651001;\r\nControl: Evgeny Bazylev."

#define FONT_NAME L"Courier New"

#define FILE_PATH "D:\\Git\\OSandSP-labworks\\OSandSP-labwork2\\table.txt"
#define FILE_MODE "r"
#define OPENFILE_ERROR_MESSAGE_CAPTION _T("Error")
#define OPENFILE_ERROR_MESSAGE_TEXT _T("File not found")

#define FILE_TABLE_ROWS 21
#define FILE_TABLE_COLUMNS 21

#define COLOR_BACKGROUND RGB(241, 242, 240);
#define COLOR_TABLE RGB(191, 186, 190);
#define COLOR_TABLENUMBER RGB(85, 138, 134);
#define COLOR_TABLEACCENT RGB(166, 62, 20);

HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

BOOL ReadFile();
VOID AddMenu(HWND);
VOID InitializeFont();
VOID PaintTable(HWND);
VOID FindTableParameters(int*, int*, int*, int*);
VOID FillTable(int*, int*, int*, int*);
BOOL CheckUserSize();

TCHAR WinClassName[] = _T(CLASS_NAME);
FILE *file;
HDC hdc;
PAINTSTRUCT ps;
RECT cellRt;
HBRUSH backgroundBrush, tableBrush;
COLORREF backgroundColor = COLOR_BACKGROUND;
COLORREF tableColor = COLOR_TABLE;
COLORREF tableNumberColor = COLOR_TABLENUMBER;
COLORREF tableAccentNumberColor = COLOR_TABLEACCENT;
int windowWidth, windowHeight;
HMENU hMenu;
int rows = 10, columns = 10;
BOOL isRowsNum, isColumnsNum;
int table[FILE_TABLE_ROWS][FILE_TABLE_COLUMNS];
LOGFONT lf;
HFONT hFont;

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
	wcex.hbrBackground = CreateSolidBrush(backgroundColor);
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

BOOL ReadFile()
{
	std::ifstream in(FILE_PATH);
	if (!in.is_open())
		return false;
	for (int i = 0; i < FILE_TABLE_ROWS; i++)
		for (int j = 0; j < FILE_TABLE_COLUMNS; j++)
			in >> table[i][j];
	in.close();
	return true;
}

VOID AddMenu(HWND hWnd)
{
	hMenu = CreateMenu();
	AppendMenu(hMenu, MF_STRING, 1, MENU_CHANGESIZE_TITLE);
	AppendMenu(hMenu, MF_STRING, 2, MENU_ABOUT_TITLE);
	SetMenu(hWnd, hMenu);
}

VOID InitializeFont()
{
	lf.lfHeight = 25; // Высота символьной ячейки шрифта в логических единицах
	lf.lfWidth = 0; // Ширина символьной ячейки шрифта в логических единицах
	lf.lfEscapement = 0; // Угол между вектором наклона и осью X устройства в десятых долях градусов
	lf.lfOrientation = 0; // Угол между основной линией каждого символа и осью X устройства в десятых долях градусов
	lf.lfWeight = FW_NORMAL; // Толщина шрифта в диапазоне 0..1000
	lf.lfItalic = 0; // Курсивный шрифт
	lf.lfUnderline = 0; // Подчеркнутый шрифт
	lf.lfStrikeOut = 0; // Зачеркнутый шрифт
	lf.lfCharSet = ANSI_CHARSET; // Набор символов
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS; // Точность вывода
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS; // Точность отсечения
	lf.lfQuality = PROOF_QUALITY; // Качество вывода
	lf.lfPitchAndFamily = VARIABLE_PITCH | FF_MODERN; // Ширина символов и семейство шрифтов
	wcscpy_s(lf.lfFaceName, FONT_NAME); // Название гарнитуры шрифта
	hFont = CreateFontIndirect(&lf);
}

VOID PaintTable(HWND hWnd)
{
	int cellWidth, cellHeight, borderWidth, borderHeight;
	HBRUSH tableBrush;
	COLORREF tableColor = COLOR_TABLE;

	FindTableParameters(&cellWidth, &cellHeight, &borderWidth, &borderHeight);
	tableBrush = CreateSolidBrush(tableColor);

	for (int i = 0; i <= rows; i++)
		for (int j = 0; j <= columns; j++) 
		{
			SetRect(&cellRt, borderWidth + cellWidth * j, borderHeight + cellHeight * i, borderWidth + cellWidth * (j + 1), borderHeight + cellHeight * (i + 1));
			FrameRect(hdc, &cellRt, tableBrush);
		}
	FillTable(&cellWidth, &cellHeight, &borderWidth, &borderHeight);
}

VOID FindTableParameters(int* cellWidth, int* cellHeight, int* borderWidth, int* borderHeight)
{
	*cellHeight = (int)ceil(windowHeight / (rows + 1));
	*cellWidth = (int)ceil(windowWidth / (columns + 1));
	*borderHeight = (windowHeight - (rows + 1) * *cellHeight) / 2;
	*borderWidth = (windowWidth - (columns + 1) * *cellWidth) / 2;
}

VOID FillTable(int* cellWidth, int* cellHeight, int* borderWidth, int* borderHeight)
{
	wchar_t str[4];
	SelectObject(hdc, hFont);
	SetBkColor(hdc, backgroundColor);
	for (int i = 0; i <= rows; i++)
		for (int j = 0; j <= columns; j++)
		{
			if ((i == j) || (i == 0) || (j == 0))
				SetTextColor(hdc, tableAccentNumberColor);
			else
				SetTextColor(hdc, tableNumberColor);
			_itow_s(table[i][j], str, 10);
			TextOut(hdc, *borderWidth + *cellWidth * j + 3, *borderHeight + *cellHeight * i + 1, str, _tcsclen(str));
		}
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
		if (!ReadFile()) {
			MessageBox(hWnd, OPENFILE_ERROR_MESSAGE_TEXT, OPENFILE_ERROR_MESSAGE_CAPTION, MB_OK | MB_ICONHAND);
			ExitProcess(0);
		}
		AddMenu(hWnd);
		InitializeFont(); 
		backgroundBrush = CreateSolidBrush(backgroundColor);
		tableBrush = CreateSolidBrush(tableColor);
		break;
	case WM_SIZE:
		windowWidth = LOWORD(lParam);
		windowHeight = HIWORD(lParam);
		InvalidateRect(hWnd, NULL, TRUE);
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
		PaintTable(hWnd);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		DeleteObject(backgroundBrush);
		DeleteObject(tableBrush);
		DeleteObject(hFont);
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