cmake_minimum_required(VERSION 3.16)

set(PROJ_NAME "projvk")
project(${PROJ_NAME})

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "-Wall -g")
set(PROJ_BASE_DIR ${PROJECT_SOURCE_DIR})

find_package(Vulkan)
find_package(core)

# add_definitions(-DDLL_EXPORT)

# glm
set(glm_INCLUDE_DIR external/glm)

# glm
set(tinygltf_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/tinygltf)

add_subdirectory(${PROJECT_SOURCE_DIR}/core)
add_subdirectory(${PROJECT_SOURCE_DIR}/test/vk-test)