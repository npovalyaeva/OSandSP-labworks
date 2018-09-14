#include <windows.h> //Содержит все базовые функции WinAPI
#include <tchar.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR WinClassName[] = _T("MainClass");

int APIENTRY _tWinMain(HINSTANCE hInstance, // Описатель (дескриптор) загруженного в память модуля; адрес виртуальной памяти, с которого загружена программа
	HINSTANCE hPrevInstance, // В современных системах всегда 0
	LPTSTR lpCmdLine, // казатель командной строки
	int nCmdShow) // Режим отображения окна (min, max, normal)
{
	WNDCLASSEX wcex; // Класс окна, описывающий общие для всех окон вид и поведение
			/*В отличие от WNDCLASS, WNDCLASSEX включает в себя член cbSize, который устанавливает размер структуры,
			и член hIconSm, который содержит дескриптор маленького значка, связанного с классом окна.*/
	HWND hWnd; // Описатель главного окна программы
	MSG msg; // Структура для хранения сообщения

	// Определение класса окна
	wcex.cbSize = sizeof(WNDCLASSEX); //Указывается для удобства последующей сериализации
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc; // Указатель на процедуру, обрабатывающую сообщения
	//wcex.lpszClassName = WinName; // Имя класса окна
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

	ShowWindow(hWnd, nCmdShow); //Показать окно
	UpdateWindow(hWnd); //Вызванное обновление для полной прорисовки окна

	// Цикл обработки сообщений
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);// Функция трансляции кодов нажатой клавиши
		DispatchMessage(&msg); // Посылает сообщение функции WndProc()
	}
	return 0;
}
// Оконная функция вызывается операционной системой
// и получает сообщения из очереди для данного приложения
LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{ // Обработчик сообщений
	switch (message)
	{
	case WM_DESTROY: PostQuitMessage(0);
		break; // Завершение программы
		// Обработка сообщения по умолчанию
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
