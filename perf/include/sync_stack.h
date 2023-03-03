#ifndef __SYNC_STACK_H__
#define __SYNC_STACK_H__

#include <atomic>
#include <memory>
#include <functional>
#include <optional>

namespace utils
{

template<typename _Ty>
using _PtrType = _Ty*;
// template<typename _Ty>
// using _OwnPtrType = std::unique_ptr<_Ty>;
using _PtrDiff = std::ptrdiff_t;


template<typename _Ty>
struct _Node
{
    using ValueType = _Ty;
    using NodeType = _Node<ValueType>;
    using NodePtrType = _PtrType<NodeType>;

    _Ty val;
    NodePtrType next;

    _Node() noexcept = default;
    _Node(_Ty const& val, NodePtrType next) : val(val), next(next) {}
    _Node(_Ty const& val) : _Node(val, nullptr) {}
    _Node(_Node const& val) = default;

};

template<typename _Ty>
class _LockFreeStackFlag
{
public:
    using ValueType = _Ty;
    using NodeType = _Node<ValueType>;
    using NodePtrType = _PtrType<NodeType>;
    // using LockFreeNodePtrType = std::atomic<NodePtrType>;
    
    _PtrDiff head;
    size_t size;

    _LockFreeStackFlag() noexcept :
        head(0), size(0)
    {}
    
    _LockFreeStackFlag(_LockFreeStackFlag const& flag) noexcept = default;

    bool operator==(_LockFreeStackFlag const& right) const noexcept
    {
        return head == right.head;
    }

    auto get_head_ptr() -> NodePtrType
    {
        return reinterpret_cast<NodePtrType>(head);
    }
    
    template<typename __Ty1>
    void set(__Ty1&& headval) 
    {
        head = reinterpret_cast<_PtrDiff>(headval);
    }

protected:

};

template<typename _Ty>
class LockFreeStack
{
    using ValueType = _Ty;
    using NodeType = _Node<ValueType>;
    using NodePtrType = _PtrType<NodeType>;
    using FlagType = _LockFreeStackFlag<_Ty>;
    using LockFreeFlagType = std::atomic<FlagType>;
    using LockFreeNum = std::atomic<size_t>;

public:
    LockFreeStack() noexcept = default;
    LockFreeStack(LockFreeStack const&) = default;
    ~LockFreeStack()
    {
        clean();
    }

    void push(_Ty const& t)
    {
        auto node = new_node(t);
        auto cur_flag = _flag.load();

        while (!_flag.compare_exchange_weak(cur_flag, _unsafe_push(cur_flag, node)));

    }
    
    std::optional<ValueType> pop()
    {
        NodePtrType node = nullptr;
        auto cur_flag = _flag.load();
        while (!_flag.compare_exchange_weak(cur_flag, _announce([&]() { return _unsafe_pop(cur_flag, node); })));

        // ++_ann_lf;
        // while (!_flag.compare_exchange_weak(cur_flag, _unsafe_pop(cur_flag, node)));
        // --_ann_lf;

        if (node != nullptr) {
            auto ret = node->val;
            _wait_announce();
            del_node(node);
            return ret;
        }
        return {};
    }
    
    std::optional<NodePtrType> get_head()
    {
        auto flag = _flag.load();
        if (flag.size > 0) {
            return flag.get_head_ptr();
        }
        return {};
    }

    auto flag() const -> FlagType 
    {
        return _flag.load();
    }

    size_t size() const
    {
        return _flag.load().size;
    }

    void clean() 
    {
        while (pop().has_value());
    }

protected:
    FlagType _unsafe_push(FlagType const& flag, NodePtrType node)
    {
        FlagType ret(flag);
        if (ret.size == 0) {
            ret.set(node);
        } else {
            node->next = ret.get_head_ptr();
            ret.set(node);
        }
        ret.size++;
        return ret;
    }
    
    FlagType _unsafe_pop(FlagType const& flag, NodePtrType& node)
    {
        FlagType ret(flag);
        if (ret.size == 0) {
            // error
            node = nullptr;
        } else {
            ret.size--;
            node = ret.get_head_ptr();
            ret.set(node->next);
        }
        return ret;
    }

    auto _announce(std::function<FlagType()> func) -> FlagType
    {
        ++_ann_lf;
        auto ret = func();
        --_ann_lf;
        return ret;
    }

    void _wait_announce()
    {
        size_t Zero = 0;
        while (!_ann_lf.compare_exchange_strong(Zero, 1));
    }

    template<typename..._Args>
    NodePtrType new_node(_Args&&...args) {
        NodePtrType ptr = new NodeType(std::forward<_Args>(args)...);
        return ptr;
    }
    
    void del_node(NodePtrType node) {
        // node->next = nullptr;
        delete node;
    }

private:
    LockFreeFlagType _flag;
    LockFreeNum _ann_lf = 0;
};

} // namespace cperf


#endif