#include "perf_nodes.h"


namespace cperf
{

void PerfNodes::begin(_Key name)
{
    PerfNode node;
    node.set_start();
    if (_map.find(name) == _map.end()) {
        _map.emplace(name, utils::make_ptr<PerfNodeQueue>());
    }
    _map[name]->push(node);
}

void PerfNodes::end(_Key name)
{
    PerfNode& node = _map[name]->get_head().value()->val;
    node.set_end();
}

auto PerfNodes::dump() -> Map
{
    Map _ret;
    _ret.swap(_map);
    return _ret;
}

} // namespace cperf

