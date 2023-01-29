cmake_minimum_required(VERSION 3.16)

set(PROJ_NAME "projvk")

project(${PROJ_NAME})

set(PROJ_BASE_DIR ${PROJECT_SOURCE_DIR})
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)

if(MSVC)
set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od /MT /MP")
set(CMAKE_CXX_FLAGS_RELEASE "/MT /MP")
else()
set(CMAKE_CXX_FLAGS "-Wall -g")
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
else()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin32)
    endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

set(SDL2_DIR "C:/SDL2/msvc/cmake")
find_package(SDL2 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(core)

# add_definitions(-DDLL_EXPORT)

# glm
# set(glm_INCLUDE_DIR external/glm)

# glm
set(tinygltf_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/tinygltf)

# ktx
# set(ktx_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/external/ktx/include;${PROJECT_SOURCE_DIR}/external/ktx/other_include")
add_subdirectory(${PROJECT_SOURCE_DIR}/external/ktx)

add_subdirectory(${PROJECT_SOURCE_DIR}/core)
add_subdirectory(${PROJECT_SOURCE_DIR}/test/vk-test)