#include "cvk/descriptor/descriptor_set_writer.h"

namespace cvk
{

DescriptorSetWriter::DescriptorSetWriter(size_t size)
{
    objects().resize(size);
}

std::vector<VkWriteDescriptorSet> DescriptorSetWriter::get_infos() const
{
    std::vector<VkWriteDescriptorSet> infos;
    infos.reserve(objects().size());
    for (auto CONST_REFERENCE obj : objects()) {
        infos.emplace_back(obj);
    }
    return infos;
}

void DescriptorSetWriter::update(VkDevice device)
{
    auto infos = get_infos();
    __cvk::update_descriptor_set(device, infos);
}
// void DescriptorSetWriter::update(VkDevice device)
// {
//     run_loop([&] (auto& write) -> int {
//         write.update(device);
//         return 0;
//     });
// }

} // namespace cvk
