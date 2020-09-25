#include <windows.h>
#include <time.h>
#include <tchar.h>
#include "Sprite.h"
#include "resource.h"

Sprite* sprite;
HDC hdcBack; // контекст граф. у-ва
HBITMAP hbmBack; // дескриптор битмапа (указатель)
HANDLE hndSprite; // void*

LRESULT /* результат обработки сообщения */ CALLBACK WndProc(HWND /* указатель окна */, UINT /* идентификатор сообщения */, WPARAM, LPARAM);
void FillWindowBackground(RECT);
WNDCLASSEX CreateWindowClassExObject(HINSTANCE);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
/*
HINSTANCE hInstance – дескриптор экземпляра приложения. 
Этот дескриптор содержит адрес начала кода программы в ее адресном пространстве. 
Дескриптор hInstance чаще всего требуется функциям, работающим с ресурсами программы.

HINSTANCE hPrevInstance – дескриптор предыдущего экземпляра приложения. 
Этот дескриптор остался от старых версий Windows - скорее всего, вам он никогда не пригодится.

LPSTR lpCmdLine – указатель на начало командной строки, введенной при запуске программы.

int nCmdShow – это значение содержит желаемый вид окна (например, свернутый или развернутый)
*/
{
	WNDCLASSEX wc = CreateWindowClassExObject(hInstance); // содержит информацию о классе окна

	if (!RegisterClassEx(&wc)) { // регистрирует класс окна для последующего использования при вызове функции CreateWindow или CreateWindowEx
		MessageBox(NULL, "Ошибка регистрации окна!", "Error!", MB_OK);
		return EXIT_FAILURE;
	}

	HWND hwnd = CreateWindowEx // Каждому окну Винда присваивает уникальный номер - Window Hundle. 
	//Это своего рода номер паспорта для окна. 
	//По этому номеру другие приложения смогут "опознать" окно и проделать с ним какие-либо операции.
	(
		WS_EX_CLIENTEDGE, 
		"WindowClass", 
		"Движение спрайта", 
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		640,
		480,
		NULL, 
		NULL, 
		hInstance, 
		NULL
	);

	if (hwnd == NULL) {
		MessageBox(NULL, "Ошибка содания окна!", "Error!", MB_OK);
		return EXIT_FAILURE;
	}

	hndSprite = LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, 0);
	if (hndSprite == NULL)
	{
		MessageBox(NULL, "Ошибка загрузки картинки!", "Error!", MB_OK);
		return EXIT_FAILURE;
	}

	RECT rect; // прямоугольник
	GetClientRect(hwnd, &rect); // извлекает координаты рабочей области окна. 
	//Рабочие координаты определяют левый верхний и нижний правый углы рабочей области
	sprite = new Sprite(50, 50, 50, 50, 4, hndSprite, rect);

	ShowWindow(hwnd, nCmdShow);
	// устанавливает состояние показа определяемого окна


	MSG msg; // Содержит информацию о сообщении из очереди сообщений потока
	while (GetMessage(&msg, NULL, 0, 0)) // извлекает сообщение из очереди сообщений вызывающего потока и помещает его в заданную структуру
	{
		TranslateMessage(&msg); // переводит сообщения виртуальных клавиш в символьные сообщения
		DispatchMessage(&msg); // распределяет сообщение оконной процедуре
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps; // содержит информации для приложения. Эта информация может быть использована для окраски клиентской области окна, которым владеет приложение.
	static RECT rect;

	switch (Message) {

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}

		case WM_KEYDOWN:
		{
			GetClientRect(hWnd, &rect);

			switch (wParam)
			{
				case VK_RIGHT:
					sprite->move(RIGHT);
					break;
				case VK_LEFT:
					sprite->move(LEFT);
					break;
				case VK_UP:
					sprite->move(UP);
					break;
				case VK_DOWN:
					sprite->move(DOWN);
					break;
			}

			InvalidateRect(hWnd, &rect, false);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			short delta = HIWORD(wParam);
			short specialKeys = LOWORD(wParam);

			if (delta > 0)
			{
				if (specialKeys & MK_SHIFT)
					sprite->move(LEFT);
				else
					sprite->setScale(delta / 100);
			}
			else if (delta < 0)
			{
				if (specialKeys & MK_SHIFT)
					sprite->move(RIGHT);
				else
					sprite->setScale(delta / 100);
			}

			InvalidateRect(hWnd, &rect, false);
			break;
		}
		case WM_PAINT:
		{
			BeginPaint(hWnd, &ps); //Функция BeginPaint автоматически заставляет 
			// регион отсечения контекста устройства исключать любую область вне обновляемого региона
			GetClientRect(hWnd, &rect); // извлекает координаты рабочей области окна
			hdcBack = CreateCompatibleDC(ps.hdc);
			//создает контекст устройства в памяти (DC), совместимый с заданным устройством. ... 
			//Если этот дескриптор равен ПУСТО (NULL), функция создает контекст устройства в памяти, 
			//совместимый с текущим экраном приложения
			hbmBack = CreateCompatibleBitmap(ps.hdc, rect.right - rect.left, rect.bottom - rect.top);
			//создает точечный рисунок, совместимый с устройством, которое связано с заданным контекстом устройства
			HBITMAP oldBmp = (HBITMAP)SelectObject(hdcBack, hbmBack);
			// Выбиpает логический объект для DC. 
			//В каждый момент вpемени может быть выбpан только один объект, котоpый должен удаляться сpазу же, 
			//как только пеpестает использоваться
			FillWindowBackground(rect);
			sprite->draw(hdcBack, hndSprite);
			BitBlt(ps.hdc, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom, hdcBack, 0, 0, SRCCOPY);
			// выполняет передачу битовых блоков данных о цвете, 
			// соответствующих прямоугольнику пикселей из заданного исходного контекста устройства
			// в целевой контекст устройства
			SelectObject(hdcBack, oldBmp);
			DeleteObject(hbmBack);
			// Эта функция возвращает ранее выбранный объект указанного типа. 
			// Приложение должно всегда заменять новый объект оригинальным объектом по умолчанию после завершения рисования новым объектом
			DeleteDC(hdcBack);
			//Удаляет контекст устpойства. 
			//Если DC является последним контекстом для устpойства, 
			// уведомляет устpойство и освобождает всю память и pесуpсы системы
			EndPaint(hWnd, &ps);
			// Функция EndPaint отмечает конец окрашивания в заданном окне.
			// Эта функция требуется для каждого вызова в функции BeginPaint, но только после того, 
			// как окрашивание завершается полностью
			break;
		}
		case WM_SIZE:
		{
			GetClientRect(hWnd, &rect);
			sprite->setWindowRect(rect);

			InvalidateRect(hWnd, NULL, false);
			break;
		}
		default:
			return DefWindowProc(hWnd, Message, wParam, lParam);
	}
	return 0;
}

WNDCLASSEX CreateWindowClassExObject(HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszClassName = "WindowClass";
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	return wc;
}

void FillWindowBackground(RECT rect) 
{
	LOGBRUSH br;
	br.lbStyle = BS_SOLID;
	br.lbColor = 0xFFFFFF;
	HBRUSH brush;
	brush = CreateBrushIndirect(&br); // Функция CreateBrushIndirect создает логическую кисть с указанным стилем, цветом и узором
	FillRect(hdcBack, &rect, brush);
}
