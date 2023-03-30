#include "win32/win.h"
#include "clog.h"

#if defined(WIN32)

#include <Windows.h>


LRESULT WINAPI __win::DefaultWinProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    WinInfo*info = (WinInfo*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    switch (wMsg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_SIZE:
        if (info) {
            info->width = LOWORD(lParam);
            info->height = HIWORD(lParam);
            PostMessage(hWnd, WM_ACTIVATE, wParam, lParam);
        }
        break;
    }
    return DefWindowProc(hWnd, wMsg, wParam, lParam);
}

Windows::Windows()
{
}

Windows::~Windows()
{
    __win::destroy_win(_hWnd);
    __win::unregister_cls(_name, _hInstance);
}

bool Windows::create(std::string title, uint32_t width, uint32_t height)
{
    _hInstance = GetModuleHandle(title.c_str());
    if (!__win::register_cls(title, _hInstance, __win::DefaultWinProc)) {
        // CLogE("Could not register window class %s", win_name.c_str());
        return false;
    }

    __win::adjust_screen(width, height);

    _hWnd = __win::create_win(title, _hInstance, width, height);
    if (!_hWnd) {
        // CLogE("Could not create window %s", win_name.c_str());
        return false;
    }

    SetWindowLongPtr(_hWnd, GWLP_USERDATA, (LONG_PTR)&_info);
    return true;
}

void Windows::run(std::function<void()> process)
{
    Windows::show();
    while(!Windows::dispatch(process));
}

bool Windows::show()
{
    ShowWindow(_hWnd, 1);
    UpdateWindow(_hWnd);
    return true;
}

bool Windows::poll_event(uint32_t& message)
{
    bool has_event = PeekMessage(&_msg, 0, 0, 0, PM_REMOVE);
    if (has_event) {
        message = _msg.message;
        if (message == WM_QUIT) {
            return false;
        }
    }
    return true;
}

void Windows::free_event()
{
    update();
}

void Windows::update()
{
    TranslateMessage(&_msg);
    DispatchMessage(&_msg);
}

int Windows::dispatch(std::function<void()> process)
{
    // MSG msg = {};
    uint32_t msg = 0;
    if (poll_event(msg)) {
        if (msg == WM_QUIT)
            return -1;
        process();
        update();
    }
    return 0;
}

auto Windows::info() const -> WinInfo CONST_REFERENCE
{
    return _info;
}

void __win::adjust_screen(uint32_t& width, uint32_t& height)
{
	::RECT screen =
	{ 0, 0, (LONG)width, (LONG)height };
	AdjustWindowRect(&screen, WS_OVERLAPPEDWINDOW, 0);

	width = static_cast<uint32_t>(screen.right - screen.left);
	height = static_cast<uint32_t>(screen.bottom - screen.top);
}

void Windows::invalidate() const
{
    InvalidateRect(_hWnd, nullptr, true);
}

ATOM __win::register_cls(const std::string& cls_name, HINSTANCE& hInstance, WNDPROC proc)
{
    WNDCLASSEX cls = {};
    cls.cbSize = sizeof(WNDCLASSEX);
    cls.style = CS_HREDRAW | CS_VREDRAW;
    cls.hInstance = hInstance;
    cls.lpszClassName = cls_name.c_str();
    cls.lpfnWndProc = proc;
    return RegisterClassEx(&cls);
}

void __win::unregister_cls(const std::string &cls_name, HINSTANCE hInstance)
{
    UnregisterClass(cls_name.c_str(), hInstance);
}

HWND __win::create_win(const std::string& name, HINSTANCE hInstance, uint32_t width, uint32_t height)
{
    return CreateWindowEx(0,
                          name.c_str(),
                          name.c_str(),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          width,
                          height,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);
}

void __win::destroy_win(HWND hWnd)
{
    DestroyWindow(hWnd);
}

#endif