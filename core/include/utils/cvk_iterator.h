#pragma once

#include "cvk/vk_header.h"

namespace utils
{

    class CVK_API CvkIterator
    {
    public:
        using CvkIteratorType = VkBaseOutStructure;
        using CvkIteratorPtr = CvkIteratorType*;
        using CvkIteratorCPtr = CvkIteratorType CONST_PTR;

        constexpr CvkIterator(void* ptr) : 
            _head((CvkIteratorPtr) ptr), _ptr((CvkIteratorPtr) ptr)
        {
        }
        constexpr CvkIterator(CvkIterator CONST_REFERENCE) = default;

        bool has_next() const { return _ptr != nullptr && _ptr->pNext != nullptr; }
        auto get_next() const { return reinterpret_cast<CvkIteratorPtr>(_ptr->pNext); }
        void next() { if (has_next()) _ptr = get_next(); }

        CvkIterator& operator++();
        CvkIterator operator++(int);

        CvkIteratorPtr operator->() { return _ptr; }
        CvkIteratorCPtr operator->() const { return _ptr; }
    private:
        CvkIteratorPtr _head { nullptr };
        CvkIteratorPtr _ptr { nullptr };

    };
    
} // namespace utils
