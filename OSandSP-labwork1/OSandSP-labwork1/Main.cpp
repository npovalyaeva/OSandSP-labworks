#include <windows.h> //�������� ��� ������� ������� WinAPI
#include <tchar.h>


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR WinClassName[] = _T("MainClass");

int APIENTRY _tWinMain(HINSTANCE hInstance, // ��������� (����������) ������������ � ������ ������; ����� ����������� ������, � �������� ��������� ���������
	HINSTANCE hPrevInstance, // � ����������� �������� ������ 0
	LPTSTR lpCmdLine, // ��������� ��������� ������
	int nCmdShow) // ����� ����������� ���� (min, max, normal)
{
	WNDCLASSEX wcex; // ����� ����, ����������� ����� ��� ���� ���� ��� � ���������
			/*� ������� �� WNDCLASS, WNDCLASSEX �������� � ���� ���� cbSize, ������� ������������� ������ ���������,
			� ���� hIconSm, ������� �������� ���������� ���������� ������, ���������� � ������� ����.*/
	HWND hWnd; // ��������� �������� ���� ���������
	MSG msg; // ��������� ��� �������� ���������


		/*struct MSG
		{
			HWND hWnd; // ���������� ����
			UINT message; // ����� ���������
			WPARAM wParam; // 32-��������� ����� ��������
			LPARAM lParam; // �������������� ��������� ���������
			DWORD time; // ����� ������� ��������� � �������������
			POINT pt; // ���������� ������� (x,y)
		};
		struct POINT
		{
			LONG x,y;
		};*/


	// ����������� ������ ����
	wcex.cbSize = sizeof(WNDCLASSEX); //����������� ��� �������� ����������� ������������
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc; // ��������� �� ���������, �������������� ���������
	wcex.cbClsExtra = 0; // ����� �������������� ������ ��� �������� ������
	wcex.cbWndExtra = 0; // ����� �������������� ������ ��� ����
	wcex.hInstance = hInstance; // ��������� �������� ����������
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); // �������� ������ �� ������� ��������
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); // ����������� ������
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // ���������� ���� ����� ������
	wcex.lpszMenuName = NULL; // ��� ������ ����
	wcex.lpszClassName = WinClassName; // ������ _T() ������ ��� �������� ������ Unicode

	RegisterClassEx(&wcex); // ����������� ������ ����

	// �������� ����
	hWnd = CreateWindow(WinClassName, // ��� ������ ����
		_T("Sprite in Motion"), // ��������� ����
		WS_OVERLAPPEDWINDOW, // ����� ����
		//������� ����
		CW_USEDEFAULT, // x
		CW_USEDEFAULT, // y
		CW_USEDEFAULT, // Width
		CW_USEDEFAULT, // Height
		HWND_DESKTOP, // ���������� ������������� ����
		NULL, // ��� ����
		hInstance, // ���������� ����������
		NULL); // �������������� ���������� ���

	ShowWindow(hWnd, nCmdShow); // �������� ����
	UpdateWindow(hWnd); // ��������� ���������� ��� ������ ���������� ����

	// ���� ��������� ���������
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); // ������� ���������� ����� ������� �������
		DispatchMessage(&msg); // �������� ��������� ������� WndProc()
	}
	return 0;
}

HDC hdc; // ���������� ��������� ����������
PAINTSTRUCT ps;
RECT figureRt, backRt;
bool firstWinSizeFlag = true;
int rectX1, rectY1, rectX2, rectY2;
int windowX, windowY;
//HPEN rectanglePen, backgroundPen;
HBRUSH rectangleBrush, backgroundBrush;
COLORREF backgroundColor = RGB(231, 231, 231), rectangleColor = RGB(1, 90, 91);

void FindRectangleSize(int winX, int winY, int &x1, int &y1, int &x2, int &y2)
{
	x1 = winX * 3 / 8;
	y1 = winY * 3 / 8;
	x2 = winX * 5 / 8;
	y2 = winY * 5 / 8;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
		{
			//rectanglePen = CreatePen(PS_SOLID, 1, rectangleColor); // C������� ����
			//backgroundPen = CreatePen(PS_SOLID, 1, backgroundColor);
			rectangleBrush = CreateSolidBrush(rectangleColor); // C������� �����
			backgroundBrush = CreateSolidBrush(backgroundColor);
		}
		break;
		case WM_SIZE:
		{
			windowX = LOWORD(lParam); // ������ ����
			windowY = HIWORD(lParam); // ������ ����
			if (firstWinSizeFlag) 			
			{
				FindRectangleSize(windowX, windowY, rectX1, rectY1, rectX2, rectY2);
				firstWinSizeFlag = false;
			}
		}
		break;
		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			//SelectObject(hdc, backgroundBrush);
			//Rectangle(hdc, 0, 0, windowX, windowY); // (x1, y1) � ���������� ������ �������� ���� ��������������, (x2, y2) � ���������� ������� ������� ����
			//SelectObject(hdc, backgroundPen);
			//Rectangle(hdc, 0, 0, windowX, windowY);
			//SelectObject(hdc, rectangleBrush);
			//Rectangle(hdc, rectX1, rectY1, rectX2, rectY2);
			//SelectObject(hdc, rectanglePen);
			//Rectangle(hdc, rectX1, rectY1, rectX2, rectY2);
			SetRect(&backRt, 0, 0, windowX, windowY);
			FillRect(hdc, &backRt, backgroundBrush);
			SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
			FillRect(hdc, &figureRt, rectangleBrush);
			EndPaint(hWnd, &ps);
		}
		break;
		case WM_KEYDOWN:
		{

			//https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes

			switch (wParam)
			{
				case VK_RIGHT:
				{
					if (rectX2 < windowX)
					{
						rectX1 += 3; rectX2 += 3;
						SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
						FillRect(hdc, &figureRt, rectangleBrush);
						InvalidateRect(hWnd, NULL, TRUE);
						//UpdateWindow(hWnd);
					}
				}
				break;
				case VK_LEFT:
					if (rectX1 > 0)
					{
						rectX1 -= 3; rectX2 -= 3;
						SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
						FillRect(hdc, &figureRt, rectangleBrush);
						InvalidateRect(hWnd, NULL, TRUE);
						//UpdateWindow(hWnd);
					}
					break;
				case VK_UP:
				{
					if (rectY1 > 0)
					{
						rectY1 -= 3; rectY2 -= 3;
						SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
						FillRect(hdc, &figureRt, rectangleBrush);
						InvalidateRect(hWnd, NULL, TRUE);
						//UpdateWindow(hWnd);
					}
				}
				break;
				case VK_DOWN:
				{
					if (rectY2 < windowY)
					{
						rectY1 += 3; rectY2 += 3;
						SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
						FillRect(hdc, &figureRt, rectangleBrush);
						InvalidateRect(hWnd, NULL, TRUE);
						//UpdateWindow(hWnd);
					}
				}
				break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
		case WM_DESTROY:
		{
			// �������� ��������� ��������
			//DeleteObject(backgroundPen);
			//DeleteObject(rectanglePen);
			DeleteObject(backgroundBrush);
			DeleteObject(rectangleBrush);
			PostQuitMessage(0);
		}
		break;
		default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}