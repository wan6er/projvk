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
        constexpr BaseMultipleAttachWrapper() noexcept {}

        void clear();
        auto get_attachments() -> std::vector<_Type>&;
        auto get_attachments() const -> std::vector<_Type> CONST_REFERENCE;
        operator std::vector<_Type> CONST_REFERENCE () const;
        operator std::vector<_Type>& ();

    protected:
        void _attach(_Type CONST_REFERENCE attachment);

    private:
        std::vector<_Type> _attachments;
    };

    template<class... _Type>
    class BaseMultipleAttaches : public BaseMultipleAttachWrapper<_Type> ...
    {
    public:
        constexpr BaseMultipleAttaches() noexcept {}

        template<class __Type>
        void attaches(__Type&& arg);
        template<class __Type, class...__Args>
        void attaches(__Type&& obj, __Args&&...args);

        void clear_all();

    private:
    };


};
#include "base_multi_attach.inl"