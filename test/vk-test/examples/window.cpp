#include "win32/win.h"
#include <iostream>

int main(int argc, char* argv[])
{
    uint32_t width = 1024;
    uint32_t height = 720;

    Windows win("model", width, height);
    win.show();
    win.invalidate();

    bool should_close = false;
    uint32_t msg = 0;
    while(!should_close) {
        std::cout << "running...\n";
        
        if (win.poll_event(msg)) {
            switch (msg)
            {
            case WM_QUIT:
                should_close = true;
                break;
            case WM_ACTIVATE:
                std::cout << "size changed : " << win.info().width << " " << win.info().height << "\n";
                break;
            }

            win.update();
        }
    }
    
    return 0;
}