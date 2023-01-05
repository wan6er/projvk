cmake_minimum_required(VERSION 3.16)

set(PROJ_NAME "projvk")
project(${PROJ_NAME})

set(PROJ_BASE_DIR ${PROJECT_SOURCE_DIR})

find_package(Vulkan)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")
find_package(core)

# add_definitions(-DDLL_EXPORT)

# glm
set(glm_INCLUDE_DIR external/glm)
# vma
# add_definitions(-DVMA_IMPLEMENTATION)
# set(vma_INCLUDE_DIR external/VulkanMemoryAllocator)

set(CMAKE_CXX_STANDARD 17)

# set(CMAKE_CXX_FLAGS "-Wall -g -O0")

add_subdirectory(${PROJECT_SOURCE_DIR}/core)
add_subdirectory(${PROJECT_SOURCE_DIR}/test/vk-test)