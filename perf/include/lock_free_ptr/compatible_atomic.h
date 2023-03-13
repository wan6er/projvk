#ifndef __COMPATIBLE_ATOMIC_H__
#define __COMPATIBLE_ATOMIC_H__


#if __cplusplus > 201103L

#include <atomic>

namespace utils
{

template<typename _Ty>
using Atomic = std::atomic<_Ty>;

}

#else

#error unsupport platform

#endif

#endif