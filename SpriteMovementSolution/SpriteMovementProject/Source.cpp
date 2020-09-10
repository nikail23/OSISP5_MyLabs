#include <Windows.h>

const int windowWidth = 640;
const int windowHeight = 480;
const wchar_t *windowName = L"ОСИСП. Лабороторная 1.";

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
        return DefWindowProc(hWnd, uMsg, wParam, lParam); // вызывается в случае если сообщение не обрабатывается
    };
    windowcClassInfo.lpszClassName = L"MyAppClass";
    windowcClassInfo.lpszMenuName = nullptr;
    windowcClassInfo.style = CS_VREDRAW | CS_HREDRAW;
    return windowcClassInfo;
    /*
    wc.cbClsExtra = 0; Отвечают за дополнительное выделение памяти в
    wc.cbWndExtra = 0; классе нашего окна (для записи какой либо информации в окно).

    hbrBackground - Это поле принимает дескриптор кисти которая окрашивает окно.
    GetStockObject - Возвращает GDI объект который мы можем привести к типу HBRUSH.

    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);     - HANDLE курсора.
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);   - HANDLE иконки.
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION); - HANDLE иконки (отображается сверху слева в заголовке окна).

    wc.lpfnWndProc = [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
    - Эта процедура отвечает за обработку сообщений (MSG msg{};)
      Принимает 4 обязательных параметра и возвращает LRESULT (результат).
      HWND hWnd     - дескриптор окна к которому приходит сообщение.
      UINT uMsg     - код сообщения.
      WPARAM wParam - указатель в нём храниться необходимая для сообщения информация.
      LPARAM lParam - указатель в нём храниться необходимая для сообщения информация.

      wc.lpszClassName = L"MyAppClass";   - имя класса (любое).
      wc.lpszMenuName = nullptr;          - указатель на имя меню.
      wc.style = CS_VREDRAW | CS_HREDRAW; - стиль окна (2 флага по умолчанию)
    */
}

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow)
/*CALLBACK - #define для stdcall (соглашение для вызовов, вызываемый объект будет сам за собой очищать стек
 wWinMain - идентификатор
 HINSTANCE  hInstance - указатель на начало исполняемого модуля, мз сигнатуры. адрес памяти.
 HINSTANCE - не используется был акт. для 16 битных версий виндовс.
 PWSTR szCmdLine - указатель на строку UNICOD символов оканчивающихся нулём. По сути просто аргументы.
 int nCmdShow - параметр отвечающий за то как будет показываться окно (свёрнуто, развёрнуто, на весь экран и т.д.).
*/
{
    MSG message{};                             // Структура, которая содержит в себе информацию о соообщениях (между Windows и окном или между окнами).
    HWND hWindow{};                          // Дескриптор окна ( HANDLE указ. на объект ядра в котором храниться информация о нашем окне).
    WNDCLASSEX windowClass = getWindowClass(hInstance); // Эта структура отвечает за некие х-ки окна (в фигурных скобках размеры).Исп. агрегатная инициализация.

    if (!RegisterClassEx(&windowClass)) // регистрация в системе класса нашего окна.
        return EXIT_FAILURE;

    // создание окна
    if (hWindow = CreateWindow(windowClass.lpszClassName, windowName, WS_OVERLAPPEDWINDOW, 0, 0, windowWidth, windowHeight, nullptr, nullptr, windowClass.hInstance, nullptr); hWindow == INVALID_HANDLE_VALUE)
        return EXIT_FAILURE;
    /*
      wc.lpszClassName    - имя класса.
      L"Заголовок!"       - заголовок окна.
      WS_OVERLAPPEDWINDOW - стиль окна (стили посмотреть в msdn).
      0, 0,               - X и Y координаты окна (отсчитываются от левой верхней точки).
      600, 600,           - ширина, высота.
     */

    ShowWindow(hWindow, nCmdShow); // показ окна
    UpdateWindow(hWindow);         // перерисовка окна (передаётся HANDLE)

    while (GetMessage(&message, nullptr, 0, 0)) // Цикл обработки сообщений
    {
        TranslateMessage(&message); // функция расшифровывает системное сообщение
        DispatchMessage(&message);  // функция  передаёт сообщение в оконную процедуру на обработку
    }

    return static_cast<int> (message.wParam); // возвращаемое значение точки входа
    // hWnd - идентификатор окна.
    // nullptr - нулевой указатель.
}