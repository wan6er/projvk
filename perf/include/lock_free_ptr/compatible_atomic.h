#ifndef __COMPATIBLE_ATOMIC_H__
#define __COMPATIBLE_ATOMIC_H__


#if __cplusplus > 201103L

#include <atomic>

namespace utils
{

template<typename _Ty>
using Atomic = std::atomic<_Ty>;

using MemoryOrder = std::memory_order;

inline constexpr MemoryOrder MemoryOrderRelaxed = std::memory_order_relaxed;
inline constexpr MemoryOrder MemoryOrderConsume = std::memory_order_consume;
inline constexpr MemoryOrder MemoryOrderAcquire = std::memory_order_acquire;
inline constexpr MemoryOrder MemoryOrderRelease = std::memory_order_release;
inline constexpr MemoryOrder MemoryOrderAcqRel = std::memory_order_acq_rel;
inline constexpr MemoryOrder MemoryOrderSeqCst = std::memory_order_seq_cst;

}

#else

#error unsupport platform

#endif

#endif