#include "linux/xcb_win.h"

#if defined(linux)

#include <stdlib.h>


namespace __xcb
{
    
void get_connection(xcb_connection_t*& conn)
{
    conn = xcb_connect(NULL,NULL);
    CVK_ASSERT(!xcb_connection_has_error(conn));
}

void get_first_screen(xcb_screen_t*& screen, xcb_connection_t* conn)
{
    screen = xcb_setup_roots_iterator( xcb_get_setup(conn) ).data;
}

void generat_win_id(xcb_window_t& win, xcb_connection_t* conn)
{
    win = xcb_generate_id(conn);
}

void create_window(xcb_connection_t* conn, xcb_screen_t* screen, uint32_t win_id, uint32_t width, uint32_t height)
{
    uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t values[2];
    values[0] = screen->white_pixel;
	values[1] =
		XCB_EVENT_MASK_EXPOSURE |
		XCB_EVENT_MASK_KEY_RELEASE |
		XCB_EVENT_MASK_KEY_PRESS |
		XCB_EVENT_MASK_POINTER_MOTION |
        XCB_EVENT_MASK_RESIZE_REDIRECT|
        XCB_EVENT_MASK_FOCUS_CHANGE|
        XCB_EVENT_MASK_ENTER_WINDOW   | 
        XCB_EVENT_MASK_LEAVE_WINDOW   |
		XCB_EVENT_MASK_BUTTON_PRESS |
		XCB_EVENT_MASK_BUTTON_RELEASE;

    xcb_create_window(conn, screen->root_depth, win_id, screen->root,
                        0, 0, width, height, 10,
                        XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual,
                        mask, values);

}

void flush(xcb_connection_t* conn)
{
    xcb_flush(conn);
}

void show_window(xcb_connection_t* conn, uint32_t win_id)
{
    xcb_map_window(conn, win_id);
}

void disconnect(xcb_connection_t* conn)
{
    xcb_disconnect(conn);
}

void poll_event(xcb_generic_event_t*& event, xcb_connection_t* conn)
{
    // event = xcb_wait_for_event(conn);
    event = xcb_poll_for_event(conn);
}

int check_connection_error(xcb_connection_t* conn)
{
    return xcb_connection_has_error(conn);
}

} // namespace __xcb


XCBWindow::XCBWindow()
{
}
XCBWindow::~XCBWindow()
{
    deinit();
}

bool XCBWindow::create(std::string title, uint32_t width, uint32_t height)
{
    init();
    __xcb::create_window(_conn, _screen, _win_id, width, height);
    return true;
}

bool XCBWindow::poll_event(uint32_t& event)
{
    bool _ret = false;
    xcb_generic_event_t* event_ptr = nullptr;
    event = EVENT_NONE;
    if ((_ret = poll_event_impl(event_ptr))) {
        if (event_ptr) {
            event = event_ptr->response_type & ~0x80;
            free_event_impl(event_ptr);
        }
    }
    return _ret;
}

bool XCBWindow::show()
{
    __xcb::show_window(_conn, _win_id);
    flush();
    return true;
}

xcb_connection_t* XCBWindow::get_connection() const
{
    return _conn;
}

xcb_window_t XCBWindow::get_window() const
{
    return _win_id;
}

void XCBWindow::flush()
{
    __xcb::flush(_conn);
}

bool XCBWindow::poll_event_impl(xcb_generic_event_t*& event)
{
    __xcb::poll_event(event, _conn);
    if (!event && __xcb::check_connection_error(_conn)) {
        return false;
    }
    return true;
}

void XCBWindow::free_event_impl(xcb_generic_event_t* event)
{
    free(event);
}

void XCBWindow::init()
{
    __xcb::get_connection(_conn);
    __xcb::get_first_screen(_screen, _conn);
    __xcb::generat_win_id(_win_id, _conn);
}

void XCBWindow::deinit()
{
    if (_conn) {
        __xcb::disconnect(_conn);
    }
}


#endif
