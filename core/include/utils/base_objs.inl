#pragma once

namespace utils
{

template<class Obj>
void BaseObjs<Obj>::run_loop(std::function<int(Obj&)> loop_function)
{
    std::vector<Obj>& list = object();
    int ret = 0;
    for (auto& item : list) {
        ret = loop_function(item);
        if (ret != 0) {
            break;
        }
    }
}

template<class Obj>
auto BaseObjs<Obj>::objects() const -> std::vector<Obj> CONST_REFERENCE
{
    return object();
}

template<class Obj>
auto BaseObjs<Obj>::objects() -> std::vector<Obj>&
{
    return object();
}

};