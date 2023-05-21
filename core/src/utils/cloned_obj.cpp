#include "utils/cloned_obj.h"

namespace utils
{

constexpr ClonedObj::ClonedObj(bool is_cloned) :
    _is_cloned(is_cloned)
{
}

constexpr ClonedObj::ClonedObj(const ClonedObj& obj) :
    _is_cloned(obj._is_cloned)
{
}

void ClonedObj::clone()
{
    _is_cloned = true;
}

bool ClonedObj::is_cloned()
{
    return _is_cloned;
}

};