#include <Windows.h>

const int windowWidth = 640;
const int windowHeight = 480;
const wchar_t *windowName = L"�����. ������������ 1.";

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
    windowcClassInfo.lpfnWndProc = [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        switch (uMsg)
        {
            case WM_DESTROY:
            {
                PostQuitMessage(EXIT_SUCCESS);
            }
            return 0;
        }
        return DefWindowProc(hWnd, uMsg, wParam, lParam); // ���������� � ������ ���� ��������� �� ��������������
    };
    windowcClassInfo.lpszClassName = L"MyAppClass";
    windowcClassInfo.lpszMenuName = nullptr;
    windowcClassInfo.style = CS_VREDRAW | CS_HREDRAW;
    return windowcClassInfo;
    /*
    wc.cbClsExtra = 0; �������� �� �������������� ��������� ������ �
    wc.cbWndExtra = 0; ������ ������ ���� (��� ������ ����� ���� ���������� � ����).

    hbrBackground - ��� ���� ��������� ���������� ����� ������� ���������� ����.
    GetStockObject - ���������� GDI ������ ������� �� ����� �������� � ���� HBRUSH.

    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);     - HANDLE �������.
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);   - HANDLE ������.
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION); - HANDLE ������ (������������ ������ ����� � ��������� ����).

    wc.lpfnWndProc = [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
    - ��� ��������� �������� �� ��������� ��������� (MSG msg{};)
      ��������� 4 ������������ ��������� � ���������� LRESULT (���������).
      HWND hWnd     - ���������� ���� � �������� �������� ���������.
      UINT uMsg     - ��� ���������.
      WPARAM wParam - ��������� � �� ��������� ����������� ��� ��������� ����������.
      LPARAM lParam - ��������� � �� ��������� ����������� ��� ��������� ����������.

      wc.lpszClassName = L"MyAppClass";   - ��� ������ (�����).
      wc.lpszMenuName = nullptr;          - ��������� �� ��� ����.
      wc.style = CS_VREDRAW | CS_HREDRAW; - ����� ���� (2 ����� �� ���������)
    */
}

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow)
/*CALLBACK - #define ��� stdcall (���������� ��� �������, ���������� ������ ����� ��� �� ����� ������� ����
 wWinMain - �������������
 HINSTANCE  hInstance - ��������� �� ������ ������������ ������, �� ���������. ����� ������.
 HINSTANCE - �� ������������ ��� ���. ��� 16 ������ ������ �������.
 PWSTR szCmdLine - ��������� �� ������ UNICOD �������� �������������� ����. �� ���� ������ ���������.
 int nCmdShow - �������� ���������� �� �� ��� ����� ������������ ���� (�������, ���������, �� ���� ����� � �.�.).
*/
{
    MSG message{};                             // ���������, ������� �������� � ���� ���������� � ����������� (����� Windows � ����� ��� ����� ������).
    HWND hWindow{};                          // ���������� ���� ( HANDLE ����. �� ������ ���� � ������� ��������� ���������� � ����� ����).
    WNDCLASSEX windowClass = getWindowClass(hInstance); // ��� ��������� �������� �� ����� �-�� ���� (� �������� ������� �������).���. ���������� �������������.

    if (!RegisterClassEx(&windowClass)) // ����������� � ������� ������ ������ ����.
        return EXIT_FAILURE;

    // �������� ����
    if (hWindow = CreateWindow(windowClass.lpszClassName, windowName, WS_OVERLAPPEDWINDOW, 0, 0, windowWidth, windowHeight, nullptr, nullptr, windowClass.hInstance, nullptr); hWindow == INVALID_HANDLE_VALUE)
        return EXIT_FAILURE;
    /*
      wc.lpszClassName    - ��� ������.
      L"���������!"       - ��������� ����.
      WS_OVERLAPPEDWINDOW - ����� ���� (����� ���������� � msdn).
      0, 0,               - X � Y ���������� ���� (������������� �� ����� ������� �����).
      600, 600,           - ������, ������.
     */

    ShowWindow(hWindow, nCmdShow); // ����� ����
    UpdateWindow(hWindow);         // ����������� ���� (��������� HANDLE)

    while (GetMessage(&message, nullptr, 0, 0)) // ���� ��������� ���������
    {
        TranslateMessage(&message); // ������� �������������� ��������� ���������
        DispatchMessage(&message);  // �������  ������� ��������� � ������� ��������� �� ���������
    }

    return static_cast<int> (message.wParam); // ������������ �������� ����� �����
    // hWnd - ������������� ����.
    // nullptr - ������� ���������.
}