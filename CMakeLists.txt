cmake_minimum_required(VERSION 3.16)

set(PROJ_NAME "projvk")

project(${PROJ_NAME})

set(PROJ_BASE_DIR ${PROJECT_SOURCE_DIR})
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /MT /Zc:__cplusplus")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -glldb -fsanitize=address")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
else()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib32)

endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

# find_file(CCACHE ccache)
# if(EXISTS ${CCACHE})
#     set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
#     set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
#     message("found ${CCACHE}")
# else()
#     message("cannot found ${CCACHE}")
# endif()

# set(SDL2_DIR "C:/SDL2/msvc/cmake")
# find_package(SDL2 REQUIRED)
# find_package(Vulkan REQUIRED)


find_package(core)
find_package(lockfree)

find_package(PkgConfig REQUIRED)

if(${CMAKE_HOST_UNIX})
    # find_package(XCB REQUIRED)
    pkg_check_modules(ProjDeps REQUIRED vulkan xcb)
else()
    pkg_check_modules(ProjDeps REQUIRED vulkan)
endif()


# add_definitions(-DDLL_EXPORT)

# glm
set(glm_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/glm)

# glm
set(tinygltf_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/tinygltf)

# ktx
# set(ktx_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/external/ktx/include;${PROJECT_SOURCE_DIR}/external/ktx/other_include")
add_subdirectory(${PROJECT_SOURCE_DIR}/external/ktx)

add_subdirectory(${PROJECT_SOURCE_DIR}/core)
add_subdirectory(${PROJECT_SOURCE_DIR}/lockfree)
add_subdirectory(${PROJECT_SOURCE_DIR}/test/vk-test)