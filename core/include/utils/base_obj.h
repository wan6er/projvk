#pragma once

#include "cvk/vk_header.h"
#include "cloned_obj.h"

#include <iostream>

namespace utils
{
    template<class Obj>
    class BaseObj : public utils::ClonedObj
    {
    public:
        typedef Obj ObjType;
        typedef BaseObj<Obj> BaseType;

        constexpr BaseObj();
        constexpr explicit BaseObj(const Obj& obj);
        constexpr explicit BaseObj(const BaseObj& base_obj);
        virtual ~BaseObj();

        void operator=(const BaseObj& base_obj);

        virtual auto count() const -> int final;
        virtual auto isolated() const -> bool final;
        virtual void count_swap(BaseObj& obj) final;

    protected:
        // virtual void release() = 0;

        auto object() const -> const ObjType&;
        auto object() -> ObjType&;

    private:
        virtual void add_ref() final;
        virtual void red_ref() final;
        virtual auto ref() const -> int* final;

        int* _count = nullptr;
        ObjType _obj;
        // std::function<void()> _release;
    };
};

#include "utils/base_obj.inl"