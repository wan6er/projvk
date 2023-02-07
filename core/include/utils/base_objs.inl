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

template<class Obj>
void BaseObjs<Obj>::resize(size_t size)
{
    objects().resize(size);
}

template<class Obj>
void BaseObjs<Obj>::reserve(size_t size)
{
    objects().reserve(size);
}

template<class Obj>
auto BaseObjs<Obj>::operator[](size_t index) -> Obj&
{
    return objects()[index];
}

template<class Obj>
auto BaseObjs<Obj>::operator[](size_t index) const -> Obj CONST_REFERENCE
{
    return objects()[index];
}

};