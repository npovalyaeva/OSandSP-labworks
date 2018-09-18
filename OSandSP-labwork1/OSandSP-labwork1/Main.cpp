#include <windows.h> //Содержит все базовые функции WinAPI
#include <tchar.h>

#pragma comment(lib,"msimg32")

#define REBOUND_COUNT 7 // Количество отскоков, которые выполнит спрайт, когда дойдет до границы окна
#define REBOUND_STEP 7 // Длина одного отскока
#define RECT_STEP 3 // На сколько пикселей сдвинется спрайт во время одной из манипуляций: нажатии кнопки на клавиатуре, прокрутки колесика 

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

	// Определение класса окна
	wcex.cbSize = sizeof(WNDCLASSEX); //Указывается для удобства последующей сериализации
	wcex.style = CS_HREDRAW;
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
HBITMAP hBitmap;
BITMAP bm;
HDC memBit;
bool firstWinSizeFlag = true, movingFlag = false;

bool modeFlag = true; // Режим отображения: true - загруженная .bmp, false - нарисованный спрайт

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

void DrawSprite(HWND hWnd)
{
	SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
	FillRect(hdc, &figureRt, rectangleBrush);
	InvalidateRect(hWnd, NULL, TRUE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
		{
			hBitmap = (HBITMAP)LoadImage(NULL, _T("D:\\Git\\OSandSP-labworks\\OSandSP-labwork1\\fallen-leaf.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			if (hBitmap == NULL)
			{
				MessageBox(hWnd, _T("Файл не найден"), _T("Загрузка изображения"),
					MB_OK | MB_ICONHAND);
			}
			GetObject(hBitmap, sizeof(bm), &bm);
			hdc = GetDC(hWnd);
			memBit = CreateCompatibleDC(hdc);
			SelectObject(memBit, hBitmap);
			ReleaseDC(hWnd, hdc);

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
				rectangleWidth = rectX2 - rectX1; // Ширина созданного спрайта
				rectangleHeight = rectY2 - rectY1; // Высота созданного спрайта
			}
		}
		break;
		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			SetRect(&backRt, 0, 0, windowX, windowY);
			FillRect(hdc, &backRt, backgroundBrush);
			if (!modeFlag)
			{
				SetRect(&figureRt, rectX1, rectY1, rectX2, rectY2);
				FillRect(hdc, &figureRt, rectangleBrush);
			}
			else
			{
				TransparentBlt(hdc, rectX1, rectY1, bm.bmWidth, bm.bmHeight, memBit, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));
				rectX2 = rectX1 + bm.bmWidth;
				rectY2 = rectY1 + bm.bmHeight;
			}
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
						DrawSprite(hWnd);
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
						DrawSprite(hWnd);
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
						DrawSprite(hWnd);
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
						DrawSprite(hWnd);
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
		case WM_MOUSEWHEEL: // Обработка прокрутки колесика
		{
			wheelScrolling = GET_WHEEL_DELTA_WPARAM(wParam); // Интервал, на который прокрутилось колесико
			// Положительное значение указывает, что колесико вращалось вперед, в сторону от пользователя; отрицательное значение указывает, что колесико вращалось назад, к пользователю.
			if (GetAsyncKeyState(VK_SHIFT)) // Нажата ли клавиша SHIFT
			{
				if ((wheelScrolling > 0) && (rectX1 > 0))
				{ // Прокрутка влево
					rectX1 -= RECT_STEP; 
					rectX2 -= RECT_STEP;
					DrawSprite(hWnd);
				}
				if ((wheelScrolling < 0) && (rectX2 < windowX))
				{ // Прокрутка вправо
					rectX1 += RECT_STEP; 
					rectX2 += RECT_STEP;
					DrawSprite(hWnd);
				}
			}
			else
			{
				if ((wheelScrolling > 0) && (rectY1 > 0))
				{ // Прокрутка вверх
					rectY1 -= RECT_STEP; rectY2 -= RECT_STEP;
					DrawSprite(hWnd);
				}
				if ((wheelScrolling < 0) && (rectY2 < windowY))
				{ // Прокрутка вниз
					rectY1 += RECT_STEP; rectY2 += RECT_STEP;
					DrawSprite(hWnd);
				}
			}
		}
		break;
		case WM_LBUTTONDOWN:
		{
			mouseInitialX = LOWORD(lParam); // Начальные координаты курсора
			mouseInitialY = HIWORD(lParam);
			if ((mouseInitialX >= rectX1) && (mouseInitialX <= rectX2) && (mouseInitialY >= rectY1) && (mouseInitialY <= rectY2)) // Принадлежит ли курсор спрайту
			{
				movingFlag = true; // Установить флаг движения спрайта в TRUE
			}
		}
		break;
		case WM_MOUSEMOVE:
		{
			if (movingFlag)
			{
				mouseCurrentX = LOWORD(lParam); // Текущие координаты курсора
				mouseCurrentY = HIWORD(lParam);

				rectX1 += (mouseCurrentX - mouseInitialX); // Координаты спрайта после перемещения курсора
				rectY1 += (mouseCurrentY - mouseInitialY);
				rectX2 = rectX1 + rectangleWidth;
				rectY2 = rectY1 + rectangleHeight;

				DrawSprite(hWnd);

				mouseInitialX = mouseCurrentX;
				mouseInitialY = mouseCurrentY;
			}
		}
		break;
		case WM_TIMER: // Таймер отвечает за реализацию отскока от границы окна во время движения спрайта
		{
			switch (wParam)
			{
				case 1: // Отскок влево
				{
					if (timeCounter != 0)
					{
						rectX1 -= REBOUND_STEP; 
						rectX2 -= REBOUND_STEP;
						DrawSprite(hWnd);
						timeCounter--;
					}
					else
						KillTimer(hWnd, 1);
				}
				break;
				case 2: // Отскок вправо
				{
					if (timeCounter != 0)
					{
						rectX1 += REBOUND_STEP; 
						rectX2 += REBOUND_STEP;
						DrawSprite(hWnd);
						timeCounter--;
					}
					else
						KillTimer(hWnd, 2);
				}
				break;
				case 3: // Отскок вниз
				{
					if (timeCounter != 0)
					{
						rectY1 += REBOUND_STEP; 
						rectY2 += REBOUND_STEP;
						DrawSprite(hWnd);
						timeCounter--;
					}
					else
						KillTimer(hWnd, 3);
				}
				break;
				case 4: // Отскок вверх
				{
					if (timeCounter != 0)
					{
						rectY1 -= REBOUND_STEP; 
						rectY2 -= REBOUND_STEP;
						DrawSprite(hWnd);
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
			movingFlag = false; // Установить флаг движения спрайта в FALSE
		}
		break;
		case WM_DESTROY:
		{
			// Удаление созданных объектов
			DeleteObject(backgroundBrush);
			DeleteObject(rectangleBrush);
			PostQuitMessage(0);
		}
		break;
		default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


