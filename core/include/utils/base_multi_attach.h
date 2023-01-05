#pragma once

#include "cvk/vk_header.h"

#include <string>
#include <vector>
#include <functional>

namespace utils
{
    template<class _Type>
    class BaseMultipleAttachWrapper
    {
    public:
        constexpr BaseMultipleAttachWrapper() noexcept = default;

        void clear();
        virtual auto get_attachments() -> std::vector<_Type>& final;
        operator std::vector<_Type> CONST_REFERENCE () const;

    protected:
        void _attach(_Type CONST_REFERENCE attachment);

    private:
        std::vector<_Type> _attachments;
    };

    template<class... _Type>
    class BaseMultipleAttaches : public BaseMultipleAttachWrapper<_Type> ...
    {
    public:

        template<class __Type>
        void attaches(__Type&& arg);
        template<class __Type, class...__Args>
        void attaches(__Type&& obj, __Args&&...args);

        void clear_all();

    private:
        template<class __Type>
        void _clear_recursive(__Type&& obj);
        template<class __Type, class...__Args>
        void _clear_recursive(__Type&& obj, __Args&&...args);

        template<class __Type>
        auto remove_clr() -> std::remove_const_t<std::remove_reference_t<__Type>>;
    };


};
#include "base_multi_attach.inl"