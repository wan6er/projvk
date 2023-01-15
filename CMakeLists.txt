cmake_minimum_required(VERSION 3.16)

set(PROJ_NAME "projvk")
project(${PROJ_NAME})

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)

if(MSVC)
set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od /MT /MP")
set(CMAKE_CXX_FLAGS_RELEASE "/MT /MP")
else()
set(CMAKE_CXX_FLAGS "-Wall -g")
endif()

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