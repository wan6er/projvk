#include "utils/cloned_obj.h"

namespace utils
{

void ClonedObj::clone()
{
    _is_cloned = true;
}

bool ClonedObj::is_cloned()
{
    return _is_cloned;
}

};