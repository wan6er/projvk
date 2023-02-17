#pragma once

#include "cvk/vk_header.h"

namespace utils
{
    
    template<class _Type>
    class BaseSingleAttachWrapper
    {
    public:
        void attach(_Type CONST_REFERENCE attachment);

    protected:
        // virtual auto get_attachment() -> _Type& final;
        operator _Type& ();
        operator _Type CONST_REFERENCE () const;

    private:
        _Type _attachment = {};
    };

    template<class..._Args>
    class BaseSingleAttach : public BaseSingleAttachWrapper<_Args>...
    {
    public:
        template<class __Type>
        void attach(__Type&& attachment);

    private:

    };

};
#include "base_single_attach.inl"