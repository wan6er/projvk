#include "test_header.h"

#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/memory.h"
#include "cvk/fence.h"
#include "cvk/semaphore.h"
#include "cvk/initialize/sync_initialize.h"

TEST_FUNC_BEGIN("sync")

    
    std::vector<std::string> instance_extensions = {
    };
    std::vector<std::string> instance_layers = {
    };
    std::vector<std::string> device_extensions = {
    };

    cvk::Instance instance(instance_extensions, instance_layers);
    std::vector<VkPhysicalDevice>&& devices = instance.get_all_physical_device();
    VkPhysicalDeviceFeatures device_features = {};
    cvk::Device device(devices[0], device_extensions, device_features, VK_QUEUE_GRAPHICS_BIT);

    VkFence fence;
    CHECK(__cvk::create_fence(device, fence) == VK_SUCCESS);
    __cvk::destroy_fence(device, fence);
    CHECK(__cvk::create_fence(device, fence, true) == VK_SUCCESS);
    __cvk::destroy_fence(device, fence);
    VkSemaphore semaphore;
    CHECK(__cvk::create_semaphore(device, semaphore) == VK_SUCCESS);
    __cvk::destroy_semaphore(device, semaphore);
    CHECK(__cvk::create_semaphore(device, semaphore, true) == VK_SUCCESS);
    __cvk::destroy_semaphore(device, semaphore);

    cvk::Semaphore s1(device, true);
    CHECK(s1.create() == VK_SUCCESS);
    cvk::Semaphore s2(device, false);
    CHECK(s2.create() == VK_SUCCESS);

    cvk::Fence f1(device, true);
    CHECK(f1.create() == VK_SUCCESS);
    cvk::Fence f2(device, false);
    CHECK(f2.create() == VK_SUCCESS);

    cvk::SemaphoreSet s_set(device);
    s_set = { true, true, false };
    CHECK(s_set.create() == VK_SUCCESS);
    cvk::SemaphoreSet s_set1(device, 3);
    CHECK(s_set1.create() == VK_SUCCESS);

    cvk::FenceSet f_set(device);
    f_set = { true, true };
    CHECK(f_set.create() == VK_SUCCESS);
    CHECK(f_set.wait() == VK_SUCCESS);

TEST_FUNC_END