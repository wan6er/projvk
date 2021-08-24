#pragma once

template <class Obj>
utils::BaseObj<Obj>::BaseObj()
{
    std::cout << "construct()\n";
    _count = new int(1);
}

template <class Obj>
utils::BaseObj<Obj>::BaseObj(const BaseObj& base_obj)
{
    std::cout << "copy()\n";
    if (this != &base_obj) {
        object() = base_obj.object();
        _count = base_obj.ref();
        add_ref();
    }
}

template <class Obj>
utils::BaseObj<Obj>::~BaseObj()
{
    red_ref();
    if (*_count <= 0) {
        release();
        delete _count;
        _count = nullptr;
    }
}

template <class Obj>
void utils::BaseObj<Obj>::add_ref()
{
    ++*_count;
}

template <class Obj>
void utils::BaseObj<Obj>::red_ref()
{
    --*_count;
}

template <class Obj>
auto utils::BaseObj<Obj>::ref() const -> int*
{
    return _count;
}

// template <class Obj>
// auto utils::BaseObj<Obj>::ref() -> int&
// {
//     return *_count;
// }

template <class Obj>
auto utils::BaseObj<Obj>::object() const -> const ObjType &
{
    return _obj;
}

template <class Obj>
auto utils::BaseObj<Obj>::object() -> ObjType &
{
    return _obj;
}