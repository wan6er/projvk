#pragma once

#if defined(linux)

#include "cvk/vk_header.h"
#include "base/base_window.h"

namespace __xcb
{
    CVK_API void get_connection(xcb_connection_t*& conn);
    CVK_API void get_first_screen(xcb_screen_t*& screen, xcb_connection_t* conn);
    CVK_API void generat_win_id(xcb_window_t& win, xcb_connection_t* conn);
    CVK_API void create_window(xcb_connection_t* conn, xcb_screen_t* screen, uint32_t win_id, uint32_t width, uint32_t height);
    CVK_API void show_window(xcb_connection_t* conn, uint32_t win_id);
    CVK_API void set_title(xcb_connection_t* conn, uint32_t win_id, std::string const& title);
    CVK_API void flush(xcb_connection_t* conn);
    CVK_API void disconnect(xcb_connection_t* conn);
    CVK_API void poll_event(xcb_generic_event_t*& event, xcb_connection_t* conn);
    CVK_API int check_connection_error(xcb_connection_t* conn);
    // CVK_API void show_window();
} // namespace __xcb

class CVK_API XCBWindow : public __base::Window
{
public:
    XCBWindow();
    ~XCBWindow();

    bool create(std::string title, uint32_t width, uint32_t height);
    bool poll_event(uint32_t& event);
    void free_event();
    bool show();

    void flush();

    xcb_connection_t* get_connection() const;
    xcb_window_t get_window() const;

protected:
    void init();
    void deinit();

    bool poll_event_impl(xcb_generic_event_t*& event);
    void free_event_impl(xcb_generic_event_t* event);

private:
    xcb_connection_t* _conn = nullptr;
    xcb_screen_t* _screen = nullptr;
    xcb_generic_event_t* _event_ptr = nullptr;
    xcb_window_t _win_id = 0;

};

#endif