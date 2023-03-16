#ifndef __SYNC_QUEUE_H__
#define __SYNC_QUEUE_H__

#include "sync_structure_base.h"
#include "lock_free_ptr/lock_free_ptr.h"

#include <thread>
#include <iostream>

namespace utils
{

template<typename _Ty>
class LockFreeQueue : public __SyncStructureBase
{
    using ValueType = _Ty;
    using NodeType = _Node<ValueType>;
    using SharedNodePtr = SharedPtr<NodeType>;
    using LockFreeNodePtr = LockFreePtr<NodeType>;
    using LockFreeSize = Atomic<size_t>;

public:
    LockFreeQueue() noexcept = default;
    
    LockFreeQueue(LockFreeQueue const&) = default; 

    ~LockFreeQueue()
    {
        // clean();
    }

    void push(_Ty const& t)
    {
        auto node = make_shared<NodeType>(t);
        auto _s = _size.fetch_add(1, MemoryOrderRelease);
        if (_s == 0) {

        }
    }
    
    SharedNodePtr pop()
    {
        auto _s = _size.load(MemoryOrderRelaxed);
        while (_s != 0 && !_size.compare_exchange_weak(_s, _s - 1));
        if (_s == 0) {
            return nullptr;
        }

        SharedNodePtr node = head.load(MemoryOrderRelaxed);
        while (!head.compare_exchange_weak(node, node->next));
        return node;
    }

    SharedNodePtr top() 
    {
        return head.load(MemoryOrderRelaxed);    
    }

    size_t size() const
    {
        return _size;
    }

private:
    LockFreeNodePtr head = nullptr;
    LockFreeNodePtr tail = nullptr;
    LockFreeSize _size = 0;

};

} // namespace cperf


#endif