cmake_minimum_required(VERSION 3.8)

set(PROJ_NAME "lockfree")

project(${PROJ_NAME})

set(CMAKE_CXX_FLAGS "-g")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
# message("${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB PROJ_SRC 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/thread_pool/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/lock_free_ptr/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/lock_free/*.cpp"
)

add_library(${PROJ_NAME} SHARED ${PROJ_SRC})
target_compile_definitions(${PROJ_NAME} PRIVATE CLF_BUILD)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    target_link_libraries(${PROJ_NAME} atomic)
endif()
