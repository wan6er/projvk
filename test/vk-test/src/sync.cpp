#include "test_header.h"

#include "cvk/instance.h"
#include "cvk/device.h"
#include "cvk/memory.h"
#include "cvk/sync/fence.h"
#include "cvk/sync/semaphore.h"
#include "cvk/sync/sync_set.h"
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
    cvk::Device device(devices[0], device_extensions, {}, VK_QUEUE_GRAPHICS_BIT);

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

    cvk::Semaphore s1(true);
    CHECK(s1.create(device) == VK_SUCCESS);
    cvk::Semaphore s2(false);
    CHECK(s2.create(device) == VK_SUCCESS);

    cvk::Fence f1(true);
    CHECK(f1.create(device) == VK_SUCCESS);
    cvk::Fence f2(false);
    CHECK(f2.create(device) == VK_SUCCESS);

    cvk::SemaphoreSet s_set = { true, true, false };
    CHECK(s_set.create(device) == VK_SUCCESS);
    cvk::SemaphoreSet s_set1(3);
    CHECK(s_set1.create(device) == VK_SUCCESS);

    cvk::FenceSet f_set = { true, true };
    CHECK(f_set.create(device) == VK_SUCCESS);
    CHECK(f_set.wait() == VK_SUCCESS);

TEST_FUNC_END