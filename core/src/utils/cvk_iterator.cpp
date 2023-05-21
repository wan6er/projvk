#include "utils/cvk_iterator.h"

namespace utils
{
    constexpr CvkIterator::CvkIterator(void* ptr)
    {
        // _head = reinterpret_cast<CvkIteratorPtr>(ptr);
        // _ptr = reinterpret_cast<CvkIteratorPtr>(ptr);
        _head = (CvkIteratorPtr) ptr;
        _ptr = (CvkIteratorPtr) ptr;
    }

    CvkIterator& CvkIterator::operator++() { 
        next();
        return *this; 
    }

    CvkIterator CvkIterator::operator++(int) { 
        CvkIterator ret(*this);
        next();
        return ret; 
    }
    
} // namespace utils
