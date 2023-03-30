#ifndef __BASE_WINDOW_H__
#define __BASE_WINDOW_H__

#include <string>

namespace __base
{

class Window
{
public:

    enum {
        EVENT_NONE = ~0x0
    };

    virtual bool create(std::string title, uint32_t width, uint32_t height) = 0;
    virtual bool poll_event(uint32_t& event) = 0;
    virtual void free_event() = 0;
    virtual bool show() = 0;

};
    
} // namespace __base


#endif