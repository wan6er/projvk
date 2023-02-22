#pragma once

#if defined(linux)

#include "cvk/vk_header.h"
#include <xcb/xcb.h>

namespace __xcb
{
    CVK_API void get_connection(xcb_connection_t*& conn);
    CVK_API void get_first_screen(xcb_screen_t*& screen, xcb_connection_t* conn);
    CVK_API void generat_win_id(xcb_window_t& win, xcb_connection_t* conn);
    CVK_API void create_window(xcb_connection_t* conn, xcb_screen_t* screen, uint32_t win_id, uint32_t width, uint32_t height);
    CVK_API void show_window(xcb_connection_t* conn, uint32_t win_id);
    CVK_API void flush(xcb_connection_t* conn);
    CVK_API void disconnect(xcb_connection_t* conn);
    CVK_API void pull_event(xcb_generic_event_t*& event, xcb_connection_t* conn);
    // CVK_API void show_window();
} // namespace __xcb

class CVK_API XCBWindow
{
public:
    XCBWindow();
    ~XCBWindow();

    void create(uint32_t width, uint32_t height);
    void show();
    void flush();

    bool pull_event(xcb_generic_event_t*& event);
    void free_event(xcb_generic_event_t* event);

protected:
    void init();
    void deinit();

private:
    xcb_connection_t* _conn = nullptr;
    xcb_screen_t* _screen = nullptr;
    xcb_window_t _win_id = 0;

};

#endif