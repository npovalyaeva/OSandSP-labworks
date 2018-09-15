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
	wcex.style = CS_HREDRAW | CS_VREDRAW |  CS_DBLCLKS;
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


// ���������� ���������
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc; //���������� ��������� ����������
	PAINTSTRUCT ps;

	static int sx, sy;
	static HPEN rectanglePen, backgroundPen;
	static HBRUSH rectangleBrush, backgroundBrush;
	COLORREF backgroundColor = RGB(231, 231, 231), rectangleColor = RGB(1, 90, 91);

	switch (message)
	{
		case WM_CREATE:
			rectanglePen = CreatePen(PS_SOLID, 1, rectangleColor); // �������� ����
			backgroundPen = CreatePen(PS_SOLID, 1, backgroundColor);
			rectangleBrush = CreateSolidBrush(rectangleColor); // �������� �����
			backgroundBrush = CreateSolidBrush(backgroundColor);
			break;
		case WM_SIZE:
			sx = LOWORD(lParam); // ������ ����
			sy = HIWORD(lParam); // ������ ����
			break;
		case WM_PAINT: // ������� ���� � �������� �������

			//�������, ��� ��� ����� ��������������

			hdc = BeginPaint(hWnd, &ps);
			SelectObject(hdc, backgroundBrush);
			Rectangle(hdc, 0, 0, sx, sy); // (x1, y1) � ���������� ������ �������� ���� ��������������, (x2, y2) � ���������� ������� ������� ����
			SelectObject(hdc, backgroundPen);
			Rectangle(hdc, 0, 0, sx, sy);

			SelectObject(hdc, rectangleBrush);
			Rectangle(hdc, sx * 3 / 8, sy * 3 / 8, sx * 5 / 8, sy * 5 / 8); 
			SelectObject(hdc, rectanglePen);
			Rectangle(hdc, sx * 3 / 8, sy * 3 / 8, sx * 5 / 8, sy * 5 / 8);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			// �������� ��������� ��������
			DeleteObject(backgroundPen); 
			DeleteObject(rectanglePen);
			DeleteObject(backgroundBrush);
			DeleteObject(rectangleBrush);
			PostQuitMessage(0);
			break; // ���������� ���������
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}