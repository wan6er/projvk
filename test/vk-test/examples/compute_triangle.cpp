#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/shader.h"
#include "cvk/buffer.h"
#include "cvk/descriptor.h"
#include "cvk/image.h"
#include "cvk/semaphore.h"
#include "cvk/fence.h"
#include "cvk/command_pool.h"
#include "cvk/command_buffer.h"
#include "cvk/pipeline_compute.h"

#include "utils/file.h"

#include "glm/glm.hpp"

#include <fstream>

int main()
{

    std::vector<std::string> instance_extensions = {
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
        // VK_KHR_MAINTENANCE_4_EXTENSION_NAME
    };
    std::vector<std::string> instance_layers = {
        "VK_LAYER_KHRONOS_validation"
    };

    cvk::Instance instance(instance_extensions, instance_layers);
    std::vector<VkPhysicalDevice>&& devices = instance.get_all_physical_device();
    cvk::Device device(devices[0]);
    device.add_extensions(
        VK_KHR_MAINTENANCE_4_EXTENSION_NAME
    );
    device.add_device_features(
        cvk::DeviceFeatureMaintenance4()
    );
    device.create(VK_QUEUE_COMPUTE_BIT);

    uint32_t compindex = 0;
    __cvk::get_queue_family_index(device.get_physical_device(), VK_QUEUE_COMPUTE_BIT, compindex);

    cvk::Queue queue(device, compindex);
    cvk::Fence fence(device);
    CVK_ASSERT(fence.create() == VK_SUCCESS);

    uint32_t width = 1024;
    uint32_t height = 720;

    cvk::Sampler sampler(device);
    sampler.create();
    cvk::StorageImage2DReadable storage_image(device);
    CVK_ASSERT(storage_image.create(device.get_memory_properties(), VK_FORMAT_B8G8R8A8_UNORM, width, height) == VK_SUCCESS);

    struct UBO {
        glm::vec4 src;
    } ubo_obj;
    cvk::UniformBufferWritable ubo(device);
    CVK_ASSERT(ubo.create(device.get_memory_properties(), sizeof(UBO)) == VK_SUCCESS);
    ubo.upload(&ubo_obj, sizeof(UBO));

    cvk::Descriptor descriptor(device);
    descriptor.add_layout()
        .set(0, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, VK_SHADER_STAGE_COMPUTE_BIT, 1)
        .set(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1);
    CVK_ASSERT(descriptor.create() == VK_SUCCESS);
    descriptor[0].write(0, storage_image.get_descriptor_info(VK_IMAGE_LAYOUT_GENERAL, sampler));
    descriptor[0].write(1, ubo.get_descriptor_info());

    cvk::Shader comp_shader(device, utils::load_file("shader/compute_basic.comp.spv"));
    CVK_ASSERT(comp_shader.create() == VK_SUCCESS);

    cvk::PipelineLayout layout(device);
    layout.attaches(descriptor[0].get_layout());
    CVK_ASSERT(layout.create() == VK_SUCCESS);
    cvk::ComputePipeline compute(device);
    CVK_ASSERT(compute.create(layout, comp_shader) == VK_SUCCESS);

    cvk::CommandPool cmdpool(device, compindex);
    CVK_ASSERT(cmdpool.create() == VK_SUCCESS);
    cvk::CommandBuffer cmdbuf(device, cmdpool);
    CVK_ASSERT(cmdbuf.create() == VK_SUCCESS);

    cmdbuf.begin();

    cmdbuf.cmd().set_image_layout_barrier(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL, storage_image, storage_image.get_subresource_range());

    cmdbuf.cmd().bind_descriptor_sets(VK_PIPELINE_BIND_POINT_COMPUTE, layout, { descriptor[0] });
    cmdbuf.cmd().bind_pipeline(VK_PIPELINE_BIND_POINT_COMPUTE, compute);
    cmdbuf.cmd().dispatch(width / 16, height / 16);

    cmdbuf.end();

    queue.submit({ cmdbuf }, fence);
    fence.wait();
    fence.reset();

    uint8_t* imgptr = nullptr;
    storage_image.map(imgptr);

    std::ofstream fs("comp_triangle.ppm");
    std::cout.rdbuf(fs.rdbuf());

    std::cout << "P3\n" << width << " " << height << "\n255\n";

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            int u8r = imgptr[2];
            int u8g = imgptr[1];
            int u8b = imgptr[0];
            std::cout << u8r << " " << u8g << " " << u8b << "\n";
            imgptr += 4;
        }
    }

    storage_image.unmap();

}