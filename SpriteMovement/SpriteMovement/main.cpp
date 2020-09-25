#include <windows.h>
#include <time.h>
#include <tchar.h>
#include "Sprite.h"
#include "resource.h"

Sprite* sprite;
HDC hdcBack; // �������� ����. �-��
HBITMAP hbmBack; // ���������� ������� (���������)
HANDLE hndSprite; // void*

LRESULT /* ��������� ��������� ��������� */ CALLBACK WndProc(HWND /* ��������� ���� */, UINT /* ������������� ��������� */, WPARAM, LPARAM);
void FillWindowBackground(RECT);
WNDCLASSEX CreateWindowClassExObject(HINSTANCE);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
/*
HINSTANCE hInstance � ���������� ���������� ����������. 
���� ���������� �������� ����� ������ ���� ��������� � �� �������� ������������. 
���������� hInstance ���� ����� ��������� ��������, ���������� � ��������� ���������.

HINSTANCE hPrevInstance � ���������� ����������� ���������� ����������. 
���� ���������� ������� �� ������ ������ Windows - ������ �����, ��� �� ������� �� ����������.

LPSTR lpCmdLine � ��������� �� ������ ��������� ������, ��������� ��� ������� ���������.

int nCmdShow � ��� �������� �������� �������� ��� ���� (��������, ��������� ��� �����������)
*/
{
	WNDCLASSEX wc = CreateWindowClassExObject(hInstance); // �������� ���������� � ������ ����

	if (!RegisterClassEx(&wc)) { // ������������ ����� ���� ��� ������������ ������������� ��� ������ ������� CreateWindow ��� CreateWindowEx
		MessageBox(NULL, "������ ����������� ����!", "Error!", MB_OK);
		return EXIT_FAILURE;
	}

	HWND hwnd = CreateWindowEx // ������� ���� ����� ����������� ���������� ����� - Window Hundle. 
	//��� ������ ���� ����� �������� ��� ����. 
	//�� ����� ������ ������ ���������� ������ "��������" ���� � ��������� � ��� �����-���� ��������.
	(
		WS_EX_CLIENTEDGE, 
		"WindowClass", 
		"�������� �������", 
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
		MessageBox(NULL, "������ ������� ����!", "Error!", MB_OK);
		return EXIT_FAILURE;
	}

	hndSprite = LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, 0);
	if (hndSprite == NULL)
	{
		MessageBox(NULL, "������ �������� ��������!", "Error!", MB_OK);
		return EXIT_FAILURE;
	}

	RECT rect; // �������������
	GetClientRect(hwnd, &rect); // ��������� ���������� ������� ������� ����. 
	//������� ���������� ���������� ����� ������� � ������ ������ ���� ������� �������
	sprite = new Sprite(50, 50, 50, 50, 4, hndSprite, rect);

	ShowWindow(hwnd, nCmdShow);
	// ������������� ��������� ������ ������������� ����


	MSG msg; // �������� ���������� � ��������� �� ������� ��������� ������
	while (GetMessage(&msg, NULL, 0, 0)) // ��������� ��������� �� ������� ��������� ����������� ������ � �������� ��� � �������� ���������
	{
		TranslateMessage(&msg); // ��������� ��������� ����������� ������ � ���������� ���������
		DispatchMessage(&msg); // ������������ ��������� ������� ���������
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps; // �������� ���������� ��� ����������. ��� ���������� ����� ���� ������������ ��� ������� ���������� ������� ����, ������� ������� ����������.
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
			BeginPaint(hWnd, &ps); //������� BeginPaint ������������� ���������� 
			// ������ ��������� ��������� ���������� ��������� ����� ������� ��� ������������ �������
			GetClientRect(hWnd, &rect); // ��������� ���������� ������� ������� ����
			hdcBack = CreateCompatibleDC(ps.hdc);
			//������� �������� ���������� � ������ (DC), ����������� � �������� �����������. ... 
			//���� ���� ���������� ����� ����� (NULL), ������� ������� �������� ���������� � ������, 
			//����������� � ������� ������� ����������
			hbmBack = CreateCompatibleBitmap(ps.hdc, rect.right - rect.left, rect.bottom - rect.top);
			//������� �������� �������, ����������� � �����������, ������� ������� � �������� ���������� ����������
			HBITMAP oldBmp = (HBITMAP)SelectObject(hdcBack, hbmBack);
			// ����p��� ���������� ������ ��� DC. 
			//� ������ ������ �p����� ����� ���� ���p�� ������ ���� ������, ����p�� ������ ��������� �p��� ��, 
			//��� ������ ��p������ ��������������
			FillWindowBackground(rect);
			sprite->draw(hdcBack, hndSprite);
			BitBlt(ps.hdc, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom, hdcBack, 0, 0, SRCCOPY);
			// ��������� �������� ������� ������ ������ � �����, 
			// ��������������� �������������� �������� �� ��������� ��������� ��������� ����������
			// � ������� �������� ����������
			SelectObject(hdcBack, oldBmp);
			DeleteObject(hbmBack);
			// ��� ������� ���������� ����� ��������� ������ ���������� ����. 
			// ���������� ������ ������ �������� ����� ������ ������������ �������� �� ��������� ����� ���������� ��������� ����� ��������
			DeleteDC(hdcBack);
			//������� �������� ���p������. 
			//���� DC �������� ��������� ���������� ��� ���p������, 
			// ���������� ���p������ � ����������� ��� ������ � p���p�� �������
			EndPaint(hWnd, &ps);
			// ������� EndPaint �������� ����� ����������� � �������� ����.
			// ��� ������� ��������� ��� ������� ������ � ������� BeginPaint, �� ������ ����� ����, 
			// ��� ����������� ����������� ���������
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
	brush = CreateBrushIndirect(&br); // ������� CreateBrushIndirect ������� ���������� ����� � ��������� ������, ������ � ������
	FillRect(hdcBack, &rect, brush);
}
