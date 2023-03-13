#include "perf_node.h"

namespace cperf
{
    
CPERF_API auto now() -> Time {
    return std::chrono::system_clock::now();
}


void PerfNode::set_start() {
    start = now();
}

void PerfNode::set_end() {
    end = now();
}

} // namespace cperf
