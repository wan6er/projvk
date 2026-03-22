#pragma once

namespace utils
{
    


template <class Obj>
constexpr BaseObj<Obj>::BaseObj() :
    _obj()
{
    _count = new int(1);
}


template <class Obj>
constexpr BaseObj<Obj>::BaseObj(const Obj& obj) : 
    ClonedObj(true),
    _obj(obj)
{
    // ClonedObj::clone();
    _count = new int(1);
}

template <class Obj>
constexpr BaseObj<Obj>::BaseObj(const BaseObj& base_obj) :
    ClonedObj(base_obj)
{
    if (this != &base_obj) {
        object() = base_obj.object();
        _count = base_obj.ref();
        add_ref();
    }
}

template <class Obj>
BaseObj<Obj>::~BaseObj()
{
    red_ref();
    if (*_count <= 0) {
        delete _count;
        _count = nullptr;
    }
}

template <class Obj>
auto BaseObj<Obj>::count() const -> int
{
    return *_count;
}

template <class Obj>
void BaseObj<Obj>::count_swap(BaseObj& obj)
{
    auto _temp_c = obj._count;
    obj._count = this->_count;
    this->_count = _temp_c;
}

template <class Obj>
auto BaseObj<Obj>::isolated() const -> bool
{
    return *_count == 1 ? true : false;
}

template <class Obj>
void BaseObj<Obj>::operator=(const BaseObj& base_obj)
{
    if (this != &base_obj) {
        // Release current shared counter before rebinding to another object.
        if (_count != nullptr) {
            red_ref();
            if (*_count <= 0) {
                delete _count;
                _count = nullptr;
            }
        }

        object() = base_obj.object();
        _count = base_obj.ref();
        add_ref();
    }
}

template <class Obj>
void BaseObj<Obj>::add_ref()
{
    ++*_count;
}

template <class Obj>
void BaseObj<Obj>::red_ref()
{
    --*_count;
}

template <class Obj>
auto BaseObj<Obj>::ref() const -> int*
{
    return _count;
}

// template <class Obj>
// auto BaseObj<Obj>::ref() -> int&
// {
//     return *_count;
// }

template <class Obj>
auto BaseObj<Obj>::object() const -> const ObjType &
{
    return _obj;
}

template <class Obj>
auto BaseObj<Obj>::object() -> ObjType &
{
    return _obj;
}

} // namespace utils
