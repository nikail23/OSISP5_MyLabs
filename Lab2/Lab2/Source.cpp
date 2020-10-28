#include <windows.h>

struct TableCell
{
	LPCSTR text;
	RECT rectOptions;
};

const int COLUMNS_NUMBER = 5;
const int ROWS_NUMBER = 5;
const int INITIAL_WINDOW_HEIGHT = 450;
const int INITIAL_WINDOW_WIDTH = 640;
const int INITIAL_CHAR_WIDTH = 12;
const int INITIAL_CHAR_HEIGHT = 16;

POINT minTableDimensions;
RECT rect;
HDC hdc;
LOGFONT logfont;

int width, height;
int maxColumnHeight;

TableCell Table[ROWS_NUMBER][COLUMNS_NUMBER];

void CreateLogFont()
{
	logfont.lfCharSet = DEFAULT_CHARSET;
	strcpy_s(logfont.lfFaceName, "Courier New");
	logfont.lfWeight = FW_BOLD;
	logfont.lfEscapement = false;
	logfont.lfItalic = false;
	logfont.lfStrikeOut = false;
	logfont.lfUnderline = false;
	logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
}

void FillTable()
{
	Table[0][0].text = "я Илья Ермолович я Илья Ермолович";
	Table[0][1].text = "я Илья Ермолович я Илья Ермолович";
	Table[0][2].text = "я Илья Ермолович я Илья Ермолович";
	Table[0][3].text = "я Илья Ермолович я Илья Ермолович";
	Table[0][4].text = "я Илья Ермолович я Илья Ермолович";
	Table[1][0].text = "я Илья Ермолович я Илья Ермолович";
	Table[1][1].text = "я Илья Ермолович я Илья Ермолович";
	Table[1][2].text = "я Илья Ермолович я Илья Ермолович";
	Table[1][3].text = "я Илья Ермолович я Илья Ермолович";
	Table[1][4].text = "я Илья Ермолович я Илья Ермолович";
	Table[2][0].text = "я Илья Ермолович я Илья Ермолович я Илья Ермолович";
	Table[2][1].text = "я Илья Ермолович я Илья Ермолович я Илья Ермолович";
	Table[2][2].text = "я Илья Ермолович я Илья Ермолович я Илья Ермолович";
	Table[2][3].text = "я Илья Ермолович я Илья Ермолович я Илья Ермолович";
	Table[2][4].text = "я Илья Ермолович я Илья Ермолович я Илья Ермолович";
	Table[3][0].text = "я Илья Ермолович я Илья Ермолович я Илья Ермолович";
	Table[3][1].text = "я Илья Ермолович я Илья Ермолович я Илья Ермолович";
	Table[3][2].text = "я Илья Ермолович я Илья Ермолович я Илья Ермолович";
	Table[3][3].text = "я Илья Ермолович я Илья Ермолович я Илья Ермолович";
	Table[3][4].text = "я Илья Ермолович я Илья Ермолович я Илья Ермолович";
	Table[4][0].text = "я Илья Ермолович я Илья Ермолович я Илья Ермолович";
	Table[4][1].text = "я Илья Ермолович я Илья Ермолович я Илья Ермолович";
	Table[4][2].text = "я Илья Ермолович я Илья Ермолович я Илья Ермолович";
	Table[4][3].text = "я Илья Ермолович я Илья Ермолович я Илья Ермолович";
	Table[4][4].text = "я Илья Ермолович я Илья Ермолович я Илья Ермолович";
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	switch (Message)
	{
		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize = minTableDimensions;
			break;
		case WM_SIZE:
		{
			hdc = GetDC(hwnd);
			RECT rect;
			GetClientRect(hwnd, &rect);
			width = rect.right;
			height = rect.bottom;
			maxColumnHeight = 0;

			logfont.lfHeight = INITIAL_CHAR_HEIGHT * height / INITIAL_WINDOW_HEIGHT;
			logfont.lfWidth = INITIAL_CHAR_WIDTH * width / INITIAL_WINDOW_WIDTH;

			HFONT hFont = CreateFontIndirect(&logfont);
			HANDLE oldFont = SelectObject(hdc, hFont);

			for (int i = 0; i < ROWS_NUMBER; i++)
			{
				for (int j = 0; j < COLUMNS_NUMBER; j++)
				{
					Table[i][j].rectOptions.left = j * (width / COLUMNS_NUMBER);
					Table[i][j].rectOptions.right = (j + 1) * (width / COLUMNS_NUMBER);
					Table[i][j].rectOptions.top = maxColumnHeight;
					Table[i][j].rectOptions.bottom = maxColumnHeight;
					DrawText(hdc, Table[i][j].text, -1, &Table[i][j].rectOptions, DT_WORDBREAK | DT_LEFT | DT_TOP | DT_CALCRECT);
					DrawText(hdc, Table[i][j].text, -1, &Table[i][j].rectOptions, DT_WORDBREAK | DT_LEFT | DT_TOP);
				}

				for (int j = 0; j < COLUMNS_NUMBER; j++)
				{
					if (Table[i][j].rectOptions.bottom > maxColumnHeight) {
						maxColumnHeight = Table[i][j].rectOptions.bottom;
					}
				}
			}

			for (int i = 0; i < COLUMNS_NUMBER; i++)
			{
				MoveToEx(hdc, Table[0][i].rectOptions.left, Table[0][i].rectOptions.top, NULL);
				LineTo(hdc, Table[0][i].rectOptions.left, maxColumnHeight);
			}

			for (int i = 0; i < ROWS_NUMBER; i++)
			{
				MoveToEx(hdc, Table[i][0].rectOptions.left, Table[i][0].rectOptions.top, NULL);
				LineTo(hdc, width, Table[i][0].rectOptions.top);
			}

			MoveToEx(hdc, width - 1, 0, NULL);
			LineTo(hdc, width - 1, maxColumnHeight);
			MoveToEx(hdc, 0, maxColumnHeight, NULL);
			LineTo(hdc, width, maxColumnHeight);

			SelectObject(hdc, oldFont);
			DeleteObject(hFont);
			ReleaseDC(hwnd, hdc);
			break;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
		}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX windowClass;
	HWND hMainWnd;
	MSG msg;

	minTableDimensions.x = 480;
	minTableDimensions.y = 240;

	FillTable();
	CreateLogFont();

	memset(&windowClass, 0, sizeof(windowClass));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = WndProc;
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.cbWndExtra = NULL;
	windowClass.cbClsExtra = NULL;
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClass.lpszClassName = "TableClass";
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&windowClass)) {
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return EXIT_FAILURE;
	}

	hMainWnd = CreateWindowEx(WS_EX_CLIENTEDGE, "TableClass", "Lab2", WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		INITIAL_WINDOW_WIDTH,
		INITIAL_WINDOW_HEIGHT,
		NULL, NULL, HINSTANCE(hInstance), NULL);

	if (hMainWnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return EXIT_FAILURE;
	}

	ShowWindow(hMainWnd, nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}