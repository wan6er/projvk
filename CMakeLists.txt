cmake_minimum_required(VERSION 3.16)

set(PROJ_NAME "projvk")
project(${PROJ_NAME})

set(PROJ_BASE_DIR ${PROJECT_SOURCE_DIR})

find_package(Vulkan)
if (NOT Vulkan_FOUND)
	message(FATAL_ERROR "Could not find Vulkan library!")
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(core)

set(EXTERNAL_DIR ${PROJECT_SOURCE_DIR}/external/)

# glm
set(glm_INCLUDE_DIR ${EXTERNAL_DIR}/glm)

set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_FLAGS "-Wall -g -O0")

add_subdirectory(${PROJECT_SOURCE_DIR}/core)
add_subdirectory(${PROJECT_SOURCE_DIR}/test/vk-test)