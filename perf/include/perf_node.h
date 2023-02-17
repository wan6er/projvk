#ifndef __PERF_NODE_H__
#define __PERF_NODE_H__

namespace cperf
{
    
template<typename _Key>
struct PerfNode
{
    _Key key;
    PerfNode* parent;
    PerfNode* child;

    constexpr PerfNode() noexcept : parent(nullptr), child(nullptr) {}
    constexpr PerfNode(_Key const& k) noexcept : key(k), parent(nullptr), child(nullptr) {}
};

template<typename _Key>
void append_perf_node(PerfNode<_Key>& head, PerfNode<_Key>& node)
{
    auto ptr = &head;
    while (ptr->child) {
        ptr = ptr->child;
    }

    ptr->child = &node;
    node.parent = ptr;
}

} // namespace cperf

#endif