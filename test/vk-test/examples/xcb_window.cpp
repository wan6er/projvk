#if defined(linux)

#include "linux/xcb_win.h"
#include <iostream>

int main(void)
{
    uint32_t done = 0;
    xcb_generic_event_t *e;

    XCBWindow win;
    win.create(100, 100);
    win.show();

    // event loop
    while (!done)
    {
        if (win.pull_event(e))
        {
            switch (e->response_type & 0x7f)
            {
            case XCB_DESTROY_NOTIFY:
                done = 1;
                break;
            case XCB_EXPOSE:
                break;
            case XCB_KEY_PRESS:
                // done = 1;
                break;
            }
            std::cout << int(e->response_type) << "\n";
            win.free_event(e);
        }
    }

}

#else

int main(void)
{
}

#endif