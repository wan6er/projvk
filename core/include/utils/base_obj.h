#pragma once

#include <iostream>

namespace utils
{
    template<class Obj>
    class BaseObj
    {
    public:
        typedef Obj ObjType;
        BaseObj();
        BaseObj(const BaseObj& base_obj);
        ~BaseObj();

    protected:
        virtual void release() {};

        auto object() const -> const ObjType&;
        auto object() -> ObjType&;

    private:
        void add_ref();
        void red_ref();
        auto ref() const -> int*;

        int* _count = nullptr;
        ObjType _obj;
    };
};

#include "utils/base_obj.inl"