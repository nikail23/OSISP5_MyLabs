#include <windows.h>

struct MyStruct
{
	LPCSTR text;
	RECT rectOptions;
};

const int columnsNumber = 5;
const int rowsNumber = 4;

const int initialWindowHeight = 450;
const int initialWindowWidth = 640;

int initialCharWidth = 12;
int initialCharHeight = 16;

HINSTANCE hinst;
POINT minTableDimensions;
RECT rect;
HDC hdc;
LOGFONT lf;

int width, height;
int maxColumnHeight;

MyStruct Table[rowsNumber][columnsNumber];

void CreateLogFont()
{
	lf.lfCharSet = DEFAULT_CHARSET;
	strcpy(lf.lfFaceName, "Courier New");
	lf.lfWeight = FW_NORMAL;
	lf.lfEscapement = false;
	lf.lfItalic = false;
	lf.lfStrikeOut = false;
	lf.lfUnderline = false;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
}

void FillTable()
{
	Table[0][0].text = "€ »ль€ ≈рмолович € »ль€ ≈рмолович € »ль€ ≈рмолович € »ль€ ≈рмолович € »ль€ ≈рмолович € »ль€ ≈рмолович ";
	Table[0][1].text = "€ »ль€ ≈рмолович € »ль€ ≈рмолович € »ль€ ≈рмолович € »ль€ ≈рмолович € »ль€ ≈рмолович € »ль€ ≈рмолович ";
	Table[0][2].text = "st hsha shts 5g h5 rhd fgdy45 t 2e rhoe jdw ofj 84w ht7 4ewgf ue i";
	Table[0][3].text = "qar at4 ey5d gud udfg hdkg ry h6hj76  ek4sjdt ejsr wjy";
	Table[0][4].text = "aw 5yh gayh5a y4";
	Table[1][0].text = "a5y ay54 wyu 6us sgf j 7idh drhgdg 8 ki8 kl ul";
	Table[1][1].text = "sry sya ryh rthr sdd hd hd hd hdfh dh fh  drfg zdg th rs6uh rart sh jursu";
	Table[1][2].text = "a5ya yty uyuk yud hd ghfdf hdf hdf ku ky7 ui ky 8kyk";
	Table[1][3].text = "sr ut7idser ytd fgd df hdfhdth t htf hf htfh tfh tfht hrd hrde hrtd htdh tdf hyki lh jlf hjdj";
	Table[1][4].text = "shsrtsr u dsgd dhdtfh d hdtfh fhtfhfh h tdftty ioyurul pk dmshg fh";
	Table[2][0].text = "arya ystwu  sedr gdrhdr sefsdr g drg drgdfegtyid z syu yukdy jssq qr tg h s rh";
	Table[2][1].text = "sst hsrj tysdg sgsg d thtgfh hjdr gd rgdrg ddrs jyt jst jst jtsy jstja se rg";
	Table[2][2].text = "ва выа ку укп ен ленгл сапртар вапр вепа ыкр ып ыо  ыкер";
	Table[2][3].text = "ва выа ку укп ен ленгл вкп ыпкыпвкп вкп  ыкр ып ыо  ыкер";
	Table[2][4].text = "ва выа ку укп ен ленгл вк пввкпвкпвк ыкр ып ыо  ыкер";
	Table[3][0].text = "ва выа ку укп ен ленгл вк п вк пф ыкр ып ыо  ыкер";
	Table[3][1].text = "ва выа ку укп ен ленгл йф а уыпауып ыуп ыкр ып ыо  ыкер";
	Table[3][2].text = "ва выа ку укп ен ленгл фцв фц ыкр ып ыо  ыкер";
	Table[3][3].text = "ва выа ку укп ен ленгл ыу уып пуы пуы аон паотп ыкр ып ыо  ыкер";
	Table[3][4].text = "ва выа ку укп ен ленгл вапа ыкр ып ыо  ыкер";
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

		if (rect.bottom > initialWindowHeight)
		{
			lf.lfHeight = initialCharHeight * rect.bottom / initialWindowHeight / 1.2;
		}
		else
		{
			lf.lfHeight = initialCharHeight * rect.bottom / initialWindowHeight;
		}
		lf.lfWidth = initialCharWidth * rect.right / initialWindowWidth;

		HFONT hFont;
		hFont = CreateFontIndirect(&lf);
		HANDLE oldFont = SelectObject(hdc, hFont);

		for (int i = 0; i < rowsNumber; i++)
		{
			for (int j = 0; j < columnsNumber; j++)
			{
				Table[i][j].rectOptions.left = j * (width / columnsNumber);
				Table[i][j].rectOptions.right = (j + 1) * (width / columnsNumber);
				Table[i][j].rectOptions.top = maxColumnHeight;
				Table[i][j].rectOptions.bottom = maxColumnHeight;
				DrawText(hdc, Table[i][j].text, -1, &Table[i][j].rectOptions, DT_WORDBREAK | DT_LEFT | DT_TOP | DT_CALCRECT);
				DrawText(hdc, Table[i][j].text, -1, &Table[i][j].rectOptions, DT_WORDBREAK | DT_LEFT | DT_TOP);
			}

			for (int j = 0; j < columnsNumber; j++)
			{
				if (Table[i][j].rectOptions.bottom > maxColumnHeight) {
					maxColumnHeight = Table[i][j].rectOptions.bottom;
				}
			}
		}

		for (int i = 0; i < columnsNumber; i++)
		{
			MoveToEx(hdc, Table[0][i].rectOptions.left, Table[0][i].rectOptions.top, NULL);
			LineTo(hdc, Table[0][i].rectOptions.left, maxColumnHeight);
		}

		for (int i = 0; i < rowsNumber; i++)
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

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	HWND hMainWnd;
	MSG msg;

	minTableDimensions.x = 480;
	minTableDimensions.y = 240;

	hinst = hInstance;
	FillTable();
	CreateLogFont();

	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbWndExtra = NULL;
	wc.cbClsExtra = NULL;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = "TableClass";
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hMainWnd = CreateWindowEx(WS_EX_CLIENTEDGE, "TableClass", "Caption", WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		initialWindowWidth,
		initialWindowHeight,
		NULL, NULL, HINSTANCE(hInstance), NULL);

	if (hMainWnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hMainWnd, nCmdShow);
	UpdateWindow(hMainWnd);

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

