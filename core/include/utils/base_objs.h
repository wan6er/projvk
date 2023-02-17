#pragma once

#include "cvk/vk_header.h"
#include "base_obj.h"

#include <vector>
#include <functional>

namespace utils
{
    template<class Obj>
    class BaseObjs : public BaseObj<std::vector<Obj>>
    {
    public:
        void resize(size_t size);
        void reserve(size_t size);
        auto operator[](size_t index) -> Obj&;
        auto operator[](size_t index) const -> Obj CONST_REFERENCE;
        inline size_t size() const { return object().size(); }

    protected:
        void run_loop(std::function<int(Obj&)> loop_function);
        auto objects() const -> std::vector<Obj> CONST_REFERENCE;
        auto objects() -> std::vector<Obj>&;

    private:
        using BaseObj<std::vector<Obj>>::object;
    };
};

#include "utils/base_objs.inl"
