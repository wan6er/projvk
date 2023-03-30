#ifndef __SYNC_QUEUE_H__
#define __SYNC_QUEUE_H__

#include "sync_structure_base.h"
#include "lock_free_ptr/lock_free_ptrs.h"

#include <thread>
#include <iostream>

namespace utils
{

template<typename _Ty>
class LockFreeQueue : public __SyncStructureBase
{
    constexpr static unsigned int HEADI = 0;
    constexpr static unsigned int TAILI = 1;

    using ValueType = _Ty;
    using NodeType = _Node<ValueType>;
    using LockFreeNodePtrs = LockFreePtrs<NodeType, 2>;
    using LockFreeSize = Atomic<size_t>;

public:
    using SharedNodePtr = SharedPtr<NodeType>;
    using SharedNodePtrs = SharedPtrs<NodeType, 2>;

    LockFreeQueue() noexcept = default;
    
    LockFreeQueue(LockFreeQueue const&) = default; 

    ~LockFreeQueue()
    {
        clean();
    }

    void push(_Ty const& t) 
    {
        auto node = make_cshared<NodeType>(t);

        SharedNodePtrs _ptrs, _next, _none;

        ptrs.load(_ptrs);

        do {
            _next = _ptrs;
            _next.swap(TAILI, node.get_ptr());
            if (_next[HEADI] == nullptr) {
                _next.swap(HEADI, node.get_ptr());
            }
        } while (!ptrs.compare_exchange_weak(_ptrs, _next));

        if (_ptrs[TAILI] != nullptr) {
            (*_ptrs[TAILI])->next = node;
        }

        _size.fetch_add(1);

    }
    
    SharedNodePtr pop()
    {
        bool poped = false;
        SharedNodePtr _ret;
        SharedNodePtrs _ptrs, _next;
        ptrs.load(_ptrs);
        do {
            _next = _ptrs;
            if (_next[HEADI] != nullptr) {
                auto _next_ptr = (*_next[HEADI])->next.get_ptr();
                _next.swap(HEADI, _next_ptr);
                if (_next_ptr == nullptr) {
                    if (_ptrs[HEADI] != _ptrs[TAILI]) {
                        break;
                    }
                    _next.swap(TAILI, nullptr);
                }
            } else {
                break;
            }
        } while (!(poped = ptrs.compare_exchange_weak(_ptrs, _next)));

        if (poped) {
            _size.fetch_sub(1);
            _ret.swap(_ptrs[HEADI]);
        }
        return _ret;
    }

    SharedNodePtr top() 
    {
        SharedNodePtrs _ptrs;
        ptrs.load(_ptrs);
        return SharedNodePtr(_ptrs[HEADI]);
    }

    size_t size() const
    {
        // SharedNodePtrs _ptrs;
        // ptrs.load(_ptrs);
        // return size_t(_ptrs[SIZEI]);
        return _size;
    }

    void clean()
    {
        auto item = pop();
        while (!item.empty()) {
            item = pop();
        }
    }

protected:

private:
    LockFreeNodePtrs ptrs;
    LockFreeSize _size = 0;

};

} // namespace cperf


#endif