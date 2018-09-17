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

	// ����������� ������ ����
	wcex.cbSize = sizeof(WNDCLASSEX); //����������� ��� �������� ����������� ������������
	wcex.style = CS_HREDRAW | CS_VREDRAW;
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

#define REBOUND_COUNT 7 // ���������� ��������, ������� �������� ������, ����� ������ �� ������� ����
#define REBOUND_STEP 7 // ����� ������ �������
#define RECT_STEP 3 // �� ������� �������� ��������� ������ �� ����� ����� �� �����������: ������� ������ �� ����������, ��������� �������� 


HDC hdc; // ���������� ��������� ����������
PAINTSTRUCT ps;
RECT figureRt, backRt;
bool firstWinSizeFlag = true, movingFlag = false;
int timeCounter;
int rectX1, rectY1, rectX2, rectY2;
int rectangleWidth, rectangleHeight;
int windowX, windowY;
int mouseCurrentX, mouseCurrentY, mouseInitialX, mouseInitialY;
HBRUSH rectangleBrush, backgroundBrush;
COLORREF backgroundColor = RGB(231, 231, 231), rectangleColor = RGB(1, 90, 91);
int wheelScrolling;


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
				rectangleWidth = rectX2 - rectX1; // ������ ���������� �������
				rectangleHeight = rectY2 - rectY1; // ������ ���������� �������
			}
		}
		break;
		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			SetRect(&backRt, 0, 0, windowX, windowY);
			FillRect(hdc, &backRt, backgroundBrush);
			SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
			FillRect(hdc, &figureRt, rectangleBrush);
			EndPaint(hWnd, &ps);
		}
		break;
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_RIGHT:
				{
					if (rectX2 < windowX)
					{
						rectX1 += RECT_STEP; 
						rectX2 += RECT_STEP;
						SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
						FillRect(hdc, &figureRt, rectangleBrush);
						InvalidateRect(hWnd, NULL, TRUE);
					}
					if (rectX2 >= windowX)
					{
						SetTimer(hWnd, 1, 10, NULL);
						timeCounter = REBOUND_COUNT;
					}
				}
				break;
				case VK_LEFT:
				{
					if (rectX1 > 0)
					{
						rectX1 -= RECT_STEP; 
						rectX2 -= RECT_STEP;
						SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
						FillRect(hdc, &figureRt, rectangleBrush);
						InvalidateRect(hWnd, NULL, TRUE);
					}
					if (rectX1 <= 0)
					{
						SetTimer(hWnd, 2, 10, NULL);
						timeCounter = REBOUND_COUNT;
					}
				}
				break;
				case VK_UP:
				{
					if (rectY1 > 0)
					{
						rectY1 -= RECT_STEP; 
						rectY2 -= RECT_STEP;
						SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
						FillRect(hdc, &figureRt, rectangleBrush);
						InvalidateRect(hWnd, NULL, TRUE);
					}
					if (rectY1 <= 0)
					{
						SetTimer(hWnd, 3, 10, NULL);
						timeCounter = REBOUND_COUNT;
					}

				}
				break;
				case VK_DOWN:
				{
					if (rectY2 < windowY)
					{
						rectY1 += RECT_STEP; 
						rectY2 += RECT_STEP;
						SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
						FillRect(hdc, &figureRt, rectangleBrush);
						InvalidateRect(hWnd, NULL, TRUE);
					}
					if (rectY2 >= windowY)
					{
						SetTimer(hWnd, 4, 10, NULL);
						timeCounter = REBOUND_COUNT;
					}
				}
				break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
		case WM_MOUSEWHEEL: // ��������� ��������� ��������
		{
			wheelScrolling = GET_WHEEL_DELTA_WPARAM(wParam); // ��������, �� ������� ������������ ��������
			// ������������� �������� ���������, ��� �������� ��������� ������, � ������� �� ������������; ������������� �������� ���������, ��� �������� ��������� �����, � ������������.
			if (GetAsyncKeyState(VK_SHIFT)) // ������ �� ������� SHIFT
			{
				if ((wheelScrolling > 0) && (rectX1 > 0))
				{ // ��������� �����
					rectX1 -= RECT_STEP; 
					rectX2 -= RECT_STEP;
					SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
					FillRect(hdc, &figureRt, rectangleBrush);
					InvalidateRect(hWnd, NULL, TRUE);
				}
				if ((wheelScrolling < 0) && (rectX2 < windowX))
				{ // ��������� ������
					rectX1 += RECT_STEP; 
					rectX2 += RECT_STEP;
					SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
					FillRect(hdc, &figureRt, rectangleBrush);
					InvalidateRect(hWnd, NULL, TRUE);
				}
			}
			else
			{
				if ((wheelScrolling > 0) && (rectY1 > 0))
				{ // ��������� �����
					rectY1 -= RECT_STEP; rectY2 -= RECT_STEP;
					SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
					FillRect(hdc, &figureRt, rectangleBrush);
					InvalidateRect(hWnd, NULL, TRUE);
				}
				if ((wheelScrolling < 0) && (rectY2 < windowY))
				{ // ��������� ����
					rectY1 += RECT_STEP; rectY2 += RECT_STEP;
					SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
					FillRect(hdc, &figureRt, rectangleBrush);
					InvalidateRect(hWnd, NULL, TRUE);
				}
			}
			
		}
		break;
		case WM_LBUTTONDOWN:
		{
			mouseInitialX = LOWORD(lParam); // ��������� ���������� �������
			mouseInitialY = HIWORD(lParam);
			if ((mouseInitialX >= rectX1) && (mouseInitialX <= rectX2) && (mouseInitialY >= rectY1) && (mouseInitialY <= rectY2)) // ����������� �� ������ �������
			{
				movingFlag = true; // ���������� ���� �������� ������� � TRUE
			}
		}
		break;
		case WM_MOUSEMOVE:
		{
			if (movingFlag)
			{
				mouseCurrentX = LOWORD(lParam); // ������� ���������� �������
				mouseCurrentY = HIWORD(lParam);

				rectX1 += (mouseCurrentX - mouseInitialX); // ���������� ������� ����� ����������� �������
				rectY1 += (mouseCurrentY - mouseInitialY);
				rectX2 = rectX1 + rectangleWidth;
				rectY2 = rectY1 + rectangleHeight;

				SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
				FillRect(hdc, &figureRt, rectangleBrush);
				InvalidateRect(hWnd, NULL, TRUE);

				mouseInitialX = mouseCurrentX;
				mouseInitialY = mouseCurrentY;
			}
		}
		break;
		case WM_TIMER: // ������ �������� �� ���������� ������� �� ������� ���� �� ����� �������� �������
		{
			switch (wParam)
			{
				case 1: // ������ �����
				{
					if (timeCounter != 0)
					{
						rectX1 -= REBOUND_STEP; rectX2 -= REBOUND_STEP;
						SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
						FillRect(hdc, &figureRt, rectangleBrush);
						InvalidateRect(hWnd, NULL, TRUE);
						timeCounter--;
					}
					else
						KillTimer(hWnd, 1);
				}
				break;
				case 2: // ������ ������
				{
					if (timeCounter != 0)
					{
						rectX1 += REBOUND_STEP; rectX2 += REBOUND_STEP;
						SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
						FillRect(hdc, &figureRt, rectangleBrush);
						InvalidateRect(hWnd, NULL, TRUE);
						timeCounter--;
					}
					else
						KillTimer(hWnd, 2);
				}
				break;
				case 3: // ������ ����
				{
					if (timeCounter != 0)
					{
						rectY1 += REBOUND_STEP; rectY2 += REBOUND_STEP;
						SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
						FillRect(hdc, &figureRt, rectangleBrush);
						InvalidateRect(hWnd, NULL, TRUE);
						timeCounter--;
					}
					else
						KillTimer(hWnd, 3);
				}
				break;
				case 4: // ������ �����
				{
					if (timeCounter != 0)
					{
						rectY1 -= REBOUND_STEP; rectY2 -= REBOUND_STEP;
						SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
						FillRect(hdc, &figureRt, rectangleBrush);
						InvalidateRect(hWnd, NULL, TRUE);
						timeCounter--;
					}
					else
						KillTimer(hWnd, 4);
				}
				break;
			}

		}
		break;
		case WM_LBUTTONUP:
		{
			movingFlag = false; // ���������� ���� �������� ������� � FALSE
		}
		break;
		case WM_DESTROY:
		{
			// �������� ��������� ��������
			DeleteObject(backgroundBrush);
			DeleteObject(rectangleBrush);
			PostQuitMessage(0);
		}
		break;
		default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}