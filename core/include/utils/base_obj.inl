#pragma once

template <class Obj>
constexpr utils::BaseObj<Obj>::BaseObj() :
    _obj()
{
    _count = new int(1);
}

// template <class Obj>
// utils::BaseObj<Obj>::BaseObj(std::function<void()> release) : 
//     _obj(),
//     _release(release)
// {
//     _count = new int(1);
// }

template <class Obj>
constexpr utils::BaseObj<Obj>::BaseObj(const Obj& obj) : 
    _obj(obj)
    // _release(release)
{
    utils::ClonedObj::clone();
    _count = new int(1);
}

template <class Obj>
constexpr utils::BaseObj<Obj>::BaseObj(const BaseObj& base_obj) :
    utils::ClonedObj(base_obj)
    // _release(base_obj._release)
{
    // std::cout << "copy()\n";
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
        // release();
        // _release();
        delete _count;
        _count = nullptr;
    }
}

template <class Obj>
auto utils::BaseObj<Obj>::count() const -> int
{
    return *_count;
}

template <class Obj>
auto utils::BaseObj<Obj>::isolated() const -> bool
{
    return *_count == 1 ? true : false;
}

template <class Obj>
void utils::BaseObj<Obj>::operator=(const BaseObj& base_obj)
{
    if (this != &base_obj) {
        object() = base_obj.object();
        _count = base_obj.ref();
        add_ref();
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