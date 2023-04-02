#if defined(linux)

#include "linux/win_xcb.h"
#include <inttypes.h>
#include <iostream>

int main(void)
{

    uint32_t e = 0;
    XCBWindow win;
    win.create("", 100, 100);
    win.show();

    while (win.poll_event(e))
    {
        if (e != XCBWindow::EVENT_NONE) {
            switch (e)
            {
            case XCB_MOTION_NOTIFY: {
                // xcb_motion_notify_event_t *motion = (xcb_motion_notify_event_t *)e;
                // printf ("Mouse moved in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
                //         motion->event, motion->event_x, motion->event_y );
                break;
            }
            case XCB_EXPOSE: {
                // auto exposed = reinterpret_cast<xcb_expose_event_t*>(e);
                // std::cout << "resize [" << exposed->width << "," << exposed->height << "]\n";
                break;
            }
            case XCB_FOCUS_IN:
                std::cout << "focus in\n";
                break;
            case XCB_FOCUS_OUT:
                std::cout << "focus out\n";
                break;
            case XCB_KEY_PRESS: {
                // auto key_event = reinterpret_cast<xcb_key_press_event_t*>(e);
                // std::cout << "key [" << (int) key_event->detail << "] pressd\n";
                break;
            }
            case XCB_KEY_RELEASE: {
                // auto key_event = reinterpret_cast<xcb_key_press_event_t*>(e);
                // std::cout << "key [" << (int) key_event->detail << "] released\n";
                break;
            }
            default:
                std::cout << e << "\n";
            }
        }

        win.free_event();
    }

}

#else

int main(void)
{
}

#endif