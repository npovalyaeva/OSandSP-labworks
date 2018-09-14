#include <windows.h> //�������� ��� ������� ������� WinAPI
#include <tchar.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR WinClassName[] = _T("MainClass");

int APIENTRY _tWinMain(HINSTANCE hInstance, // ��������� (����������) ������������ � ������ ������; ����� ����������� ������, � �������� ��������� ���������
	HINSTANCE hPrevInstance, // � ����������� �������� ������ 0
	LPTSTR lpCmdLine, // �������� ��������� ������
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
	//wcex.lpszClassName = WinName; // ��� ������ ����
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

	ShowWindow(hWnd, nCmdShow); //�������� ����
	UpdateWindow(hWnd); //��������� ���������� ��� ������ ���������� ����

	// ���� ��������� ���������
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);// ������� ���������� ����� ������� �������
		DispatchMessage(&msg); // �������� ��������� ������� WndProc()
	}
	return 0;
}
// ������� ������� ���������� ������������ ��������
// � �������� ��������� �� ������� ��� ������� ����������
LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{ // ���������� ���������
	switch (message)
	{
	case WM_DESTROY: PostQuitMessage(0);
		break; // ���������� ���������
		// ��������� ��������� �� ���������
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
