#include <windows.h> //Содержит все базовые функции WinAPI
#include <tchar.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR WinClassName[] = _T("MainClass");

int APIENTRY _tWinMain(HINSTANCE hInstance, // Описатель (дескриптор) загруженного в память модуля; адрес виртуальной памяти, с которого загружена программа
	HINSTANCE hPrevInstance, // В современных системах всегда 0
	LPTSTR lpCmdLine, // Указатель командной строки
	int nCmdShow) // Режим отображения окна (min, max, normal)
{
	WNDCLASSEX wcex; // Класс окна, описывающий общие для всех окон вид и поведение
			/*В отличие от WNDCLASS, WNDCLASSEX включает в себя член cbSize, который устанавливает размер структуры,
			и член hIconSm, который содержит дескриптор маленького значка, связанного с классом окна.*/
	HWND hWnd; // Описатель главного окна программы
	MSG msg; // Структура для хранения сообщения


		/*struct MSG
		{
			HWND hWnd; // Дескриптор окна
			UINT message; // Номер сообщения
			WPARAM wParam; // 32-разрядные целые содержат
			LPARAM lParam; // дополнительные параметры сообщения
			DWORD time; // Время посылки сообщения в миллисекундах
			POINT pt; // Координаты курсора (x,y)
		};
		struct POINT
		{
			LONG x,y;
		};*/


	// Определение класса окна
	wcex.cbSize = sizeof(WNDCLASSEX); //Указывается для удобства последующей сериализации
	wcex.style = CS_HREDRAW | CS_VREDRAW |  CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc; // Указатель на процедуру, обрабатывающую сообщения
	wcex.cbClsExtra = 0; // Число дополнительных байтов для оконного класса
	wcex.cbWndExtra = 0; // Число дополнительных байтов для окна
	wcex.hInstance = hInstance; // Описатель текущего приложения
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Загрузка иконки из области ресурсов
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); // Стандартный курсор
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Заполнение окна белым цветом
	wcex.lpszMenuName = NULL; // Нет строки меню
	wcex.lpszClassName = WinClassName; // Макрос _T() служит для создания строки Unicode



	RegisterClassEx(&wcex); // Регистрация класса окна

	// Создание окна
	hWnd = CreateWindow(WinClassName, // Имя класса окна
		_T("Sprite in Motion"), // Заголовок окна
		WS_OVERLAPPEDWINDOW, // Стиль окна
		//Размеры окна
		CW_USEDEFAULT, // x
		CW_USEDEFAULT, // y
		CW_USEDEFAULT, // Width
		CW_USEDEFAULT, // Height
		HWND_DESKTOP, // Дескриптор родительского окна
		NULL, // Нет меню
		hInstance, // Дескриптор приложения
		NULL); // Дополнительной информации нет

	ShowWindow(hWnd, nCmdShow); // Показать окно
	UpdateWindow(hWnd); // Вызванное обновление для полной прорисовки окна

	// Цикл обработки сообщений
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); // Функция трансляции кодов нажатой клавиши
		DispatchMessage(&msg); // Посылает сообщение функции WndProc()
	}
	return 0;
}


// Обработчик сообщений
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc; //Дескриптор контекста устройства
	PAINTSTRUCT ps;

	static int sx, sy;
	static HPEN rectanglePen, backgroundPen;
	static HBRUSH rectangleBrush, backgroundBrush;
	COLORREF backgroundColor = RGB(231, 231, 231), rectangleColor = RGB(1, 90, 91);

	switch (message)
	{
		case WM_CREATE:
			rectanglePen = CreatePen(PS_SOLID, 1, rectangleColor); // Создание пера
			backgroundPen = CreatePen(PS_SOLID, 1, backgroundColor);
			rectangleBrush = CreateSolidBrush(rectangleColor); // Создание кисти
			backgroundBrush = CreateSolidBrush(backgroundColor);
			break;
		case WM_SIZE:
			sx = LOWORD(lParam); // Ширина окна
			sy = HIWORD(lParam); // Высота окна
			break;
		case WM_PAINT: // Заливка фона и создание спрайта

			//Посмоти, как это можно оптимизировать

			hdc = BeginPaint(hWnd, &ps);
			SelectObject(hdc, backgroundBrush);
			Rectangle(hdc, 0, 0, sx, sy); // (x1, y1) — координаты левого верхнего угла прямоугольника, (x2, y2) — координаты правого нижнего угла
			SelectObject(hdc, backgroundPen);
			Rectangle(hdc, 0, 0, sx, sy);

			SelectObject(hdc, rectangleBrush);
			Rectangle(hdc, sx * 3 / 8, sy * 3 / 8, sx * 5 / 8, sy * 5 / 8); 
			SelectObject(hdc, rectanglePen);
			Rectangle(hdc, sx * 3 / 8, sy * 3 / 8, sx * 5 / 8, sy * 5 / 8);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			// Удаление созданных объектов
			DeleteObject(backgroundPen); 
			DeleteObject(rectanglePen);
			DeleteObject(backgroundBrush);
			DeleteObject(rectangleBrush);
			PostQuitMessage(0);
			break; // Завершение программы
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}