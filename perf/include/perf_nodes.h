#pragma once

#include "perf_node.h"
#include "lock_free/sync_queue.h"
// #include "sync_ref.h"
#include "lock_free_ptr/lock_free_ptr.h"

#include <unordered_map>
#include <string_view>

namespace cperf
{
    
using PerfNodeQueue = utils::LockFreeQueue<PerfNode>;
using PerfNodeQueueRef = utils::LockFreePtr<PerfNodeQueue>;

template<typename _Key, typename _Val>
using _Map = std::unordered_map<_Key, _Val>;
using _Key = std::string_view;
using Map = _Map<_Key, PerfNodeQueueRef>;

class CPERF_API PerfNodes
{
public:
    PerfNodes() = default;
    PerfNodes(PerfNodes const&) = delete;
    PerfNodes& operator=(PerfNodes const&) = delete;

    void begin(_Key name);
    void end(_Key name);

    auto dump() -> Map;

private:
    Map _map;
    
};

template<typename _Ty>
using _ListNode = utils::_Node<_Ty>;

using _ReadNodesCallback = std::function<void(_Key, _ListNode<PerfNode> const*)>;
CPERF_API void read_nodes(Map const& map, _ReadNodesCallback callback);

} // namespace name
