#include "perf_nodes.h"

#include <functional>

namespace cperf
{

// void read_nodes(Map const& map, _ReadNodesCallback callback)
// {
//     using List = PerfNodeQueueRef;
//     for (auto iter = map.begin(); iter != map.end(); ++iter) {
//         List const& l = iter->second;
//         if (l->get_head().has_value()) {
//             callback(iter->first, l->get_head().value());
//         }
//     }
// }

} // namespace cperf

