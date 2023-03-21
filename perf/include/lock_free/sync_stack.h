#ifndef __SYNC_STACK_H__
#define __SYNC_STACK_H__

#include "sync_structure_base.h"
#include "lock_free_ptr/lock_free_ptr.h"

namespace utils
{

template<typename _Ty>
class LockFreeStack : public __SyncStructureBase
{
    using ValueType = _Ty;
    using NodeType = _Node<ValueType>;
    using NodePtr = LockFreePtr<NodeType>;
    using LockFreeSize = Atomic<size_t>;

public:
    using SharedNodePtr = SharedPtr<NodeType>;

    LockFreeStack() noexcept : _size(0) {}
    LockFreeStack(LockFreeStack const&) = default;
    ~LockFreeStack()
    {
        clean();
    }

    void push(_Ty const& t)
    {
        auto node = make_shared<NodeType>(t);
        head.load(node->next, MemoryOrderRelaxed);
        while (!head.compare_exchange_weak(node->next, node));
        _size++;
    }

    SharedNodePtr pop()
    {
        auto _s = _size.load(MemoryOrderRelaxed);
        while (_s != 0 && !_size.compare_exchange_weak(_s, _s - 1));
        if (_s == 0) {
            return nullptr;
        }
        
        // static std::mutex mtx;
        SharedNodePtr node; 
        head.load(node, MemoryOrderRelaxed);
        while (!head.compare_exchange_weak(node, node->next));
        return node;
    }

    SharedNodePtr top() 
    {
        SharedNodePtr node;
        head.load(node, MemoryOrderRelaxed);
        return node;    
    }

    size_t size() const
    {
        return _size;
    }

    void clean() 
    {
        while (!pop().empty());
    }

private:
    NodePtr head;
    LockFreeSize _size;

};

} // namespace cperf


#endif