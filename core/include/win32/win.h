#pragma once

#include "cvk/vk_header.h"

#include <Windows.h>

#include <iostream>
#include <string>
#include <functional>

namespace __win
{

    CVK_API LRESULT WINAPI DefaultWinProc(HWND, UINT, WPARAM, LPARAM);

    CVK_API ATOM register_cls(const std::string &cls_name, HINSTANCE &hInstance, WNDPROC proc);
    CVK_API void unregister_cls(const std::string &cls_name, HINSTANCE hInstance);
    CVK_API HWND create_win(const std::string &name, HINSTANCE hInstance, uint32_t width, uint32_t height);
    CVK_API void destroy_win(HWND hWnd);
    CVK_API void adjust_screen(uint32_t &width, uint32_t &height);
};

struct CVK_API WinInfo
{
    uint32_t width;
    uint32_t height;
    bool size_changed;
};

class CVK_API Windows
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
    int dispatch(std::function<void()> process);

    bool poll_event(uint32_t& message);
    void update();

    void run(std::function<void()> process);
    void invalidate() const;

    auto info() const -> WinInfo CONST_REFERENCE;

private:
    std::string _name;
    HINSTANCE _hInstance;
    HWND _hWnd;
    WinInfo _info;
    MSG _msg;

};