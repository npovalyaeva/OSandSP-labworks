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
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
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

HDC hdc; // Дескриптор контекста устройства
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
			//rectanglePen = CreatePen(PS_SOLID, 1, rectangleColor); // Cоздание пера
			//backgroundPen = CreatePen(PS_SOLID, 1, backgroundColor);
			rectangleBrush = CreateSolidBrush(rectangleColor); // Cоздание кисти
			backgroundBrush = CreateSolidBrush(backgroundColor);
		}
		break;
		case WM_SIZE:
		{
			windowX = LOWORD(lParam); // Ширина окна
			windowY = HIWORD(lParam); // Высота окна
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
			//Rectangle(hdc, 0, 0, windowX, windowY); // (x1, y1) — координаты левого верхнего угла прямоугольника, (x2, y2) — координаты правого нижнего угла
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
			// Удаление созданных объектов
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