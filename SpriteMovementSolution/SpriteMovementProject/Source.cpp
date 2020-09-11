#include <Windows.h>
#include <time.h>
#include <tchar.h>
#include "Sprite.h"

const int windowWidth = 640;
const int windowHeight = 480;
const int updateInterval = 40;
const char *windowName = "ОСИСП. Лабороторная 1.";
float speed = 0.05;
int timeForFrame = 40;
int ellapsedTime;

Sprite* ptrSprite;

HDC hdcBack;
HBITMAP hbmBack;
HANDLE hndSprite;

LRESULT CALLBACK handleWindowMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;

    switch (uMsg)
    {
		case WM_KEYDOWN:
		{
			int time = ellapsedTime > updateInterval ? updateInterval : ellapsedTime;
			switch (wParam)
			{
			case VK_RIGHT:
				ptrSprite->moveRight(time);
				break;
			case VK_LEFT:
				ptrSprite->moveLeft(time);
				break;
			case VK_UP:
				ptrSprite->moveUp(time);
				break;
			case VK_DOWN:
				ptrSprite->moveDown(time);
				break;
			case VK_TAB:
				ptrSprite->rotateRight(3.1415 / 180);
				break;
			}
			break;
		}
		return 0;

		case WM_MOUSEWHEEL:
		{
			short delta = HIWORD(wParam);
			short specialKeys = LOWORD(wParam);
			int time = ellapsedTime > updateInterval ? updateInterval : ellapsedTime;
			time = time * abs(delta) / 60;
			if (delta > 0)
			{
				if (specialKeys & MK_SHIFT)
					ptrSprite->moveRight(time);
				else
					ptrSprite->moveUp(time);
			}
			else if (delta < 0)
			{
				if (specialKeys & MK_SHIFT)
					ptrSprite->moveLeft(time);
				else
					ptrSprite->moveDown(time);
			}
			break;
		}
		return 0;

		case WM_PAINT:
		{
			BeginPaint(hWnd, &ps);			
			ptrSprite->draw(hWnd, hndSprite);
			EndPaint(hWnd, &ps);
			break;
		}
		return 0;

        case WM_DESTROY:
        {
            PostQuitMessage(EXIT_SUCCESS);
        }
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam); 
}

WNDCLASSEX getWindowClass(HINSTANCE hInstance) 
{
    WNDCLASSEX windowcClassInfo{sizeof(WNDCLASSEX)}; 
    windowcClassInfo.cbClsExtra = 0;
    windowcClassInfo.cbWndExtra = 0;
    windowcClassInfo.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    windowcClassInfo.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowcClassInfo.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    windowcClassInfo.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    windowcClassInfo.hInstance = hInstance;
    windowcClassInfo.lpfnWndProc = handleWindowMessages;
    windowcClassInfo.lpszClassName = "MyAppClass";
    windowcClassInfo.lpszMenuName = nullptr;
    windowcClassInfo.style = CS_VREDRAW | CS_HREDRAW;
    return windowcClassInfo;
}

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow)
{
    MSG message{};
    HWND hWindow{};
    WNDCLASSEX windowClass = getWindowClass(hInstance);

    if (!RegisterClassEx(&windowClass)) 
        return EXIT_FAILURE;

    if (hWindow = CreateWindow(windowClass.lpszClassName, windowName, WS_OVERLAPPEDWINDOW, 0, 0, windowWidth, windowHeight, nullptr, nullptr, windowClass.hInstance, nullptr); hWindow == INVALID_HANDLE_VALUE)
        return EXIT_FAILURE;

	hndSprite = LoadImage(NULL, _T("sprite.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hndSprite == NULL)
	{
		MessageBox(NULL, "Bitmap loading failed!", "Error!", MB_OK);
	}
																
	ptrSprite = new Sprite(250, 250, 100, 200, speed, hndSprite);

    ShowWindow(hWindow, nCmdShow); 
    UpdateWindow(hWindow);         

	int accumulatedTime = 0;
	int tm1 = clock();
	int tm2;

    while (GetMessage(&message, nullptr, 0, 0)) 
    {
		tm2 = clock();
		ellapsedTime = tm2 - tm1;
		accumulatedTime += ellapsedTime;
		tm1 = tm2;
		if (accumulatedTime >= timeForFrame)
		{
			InvalidateRect(hWindow, NULL, FALSE);
			accumulatedTime -= timeForFrame;
		}
        TranslateMessage(&message); 
        DispatchMessage(&message);  
    }

    return EXIT_SUCCESS; 
}