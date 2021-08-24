#pragma once

#include <Windows.h>

#include <iostream>
#include <string>


namespace __win
{

    LRESULT WINAPI DefaultWinProc(HWND, UINT, WPARAM, LPARAM);

    ATOM register_cls(const std::string &cls_name, HINSTANCE &hInstance, WNDPROC proc);
    void unregister_cls(const std::string &cls_name, HINSTANCE hInstance);
    HWND create_win(const std::string &name, HINSTANCE hInstance, uint32_t width, uint32_t height);
    void destroy_win(HWND hWnd);
    void adjust_screen(uint32_t &width, uint32_t &height);
};

class Windows
{
public:
    Windows(const std::string& win_name, uint32_t width, uint32_t height, WNDPROC proc = __win::DefaultWinProc);
    ~Windows();

    operator HWND() const {
        return _hWnd;
    }
    
    HINSTANCE instance() const { return _hInstance; }
    HDC dc() { return GetDC(*this); }

    void show();
    int dispatch();

    void run();

private:
    std::string _name;
    HINSTANCE _hInstance;
    HWND _hWnd;
};