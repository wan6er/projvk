set(PROJ_NAME "core")
project(${PROJ_NAME})


if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin32)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib32)
endif()


include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB PROJ_SRC 
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/cvk/*.cpp
    ${PROJECT_SOURCE_DIR}/src/cvk/base/*.cpp
    ${PROJECT_SOURCE_DIR}/src/cvk/image/*.cpp
    ${PROJECT_SOURCE_DIR}/src/cvk/pipe/*.cpp
    ${PROJECT_SOURCE_DIR}/src/cvk/sync/*.cpp
    ${PROJECT_SOURCE_DIR}/src/cvk/command/*.cpp
    ${PROJECT_SOURCE_DIR}/src/cvk/descriptor/*.cpp
    ${PROJECT_SOURCE_DIR}/src/cvk/initialize/*.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/*.cpp
)

if (WIN32) 
    file(GLOB PROJ_WIN32_SRC ${PROJECT_SOURCE_DIR}/src/win32/*.cpp)
    set(PROJ_SRC "${PROJ_SRC};${PROJ_WIN32_SRC}")
endif()

add_library(${PROJ_NAME} SHARED ${PROJ_SRC})
# target_include_directories(${PROJ_NAME_STATIC} PRIVATE ${vma_INCLUDE_DIR})
target_include_directories(${PROJ_NAME} PRIVATE ${Vulkan_INCLUDE_DIR})
target_link_libraries(${PROJ_NAME} ${Vulkan_LIBRARY})

message("${Vulkan_LIBRARY}")

target_compile_definitions(${PROJ_NAME} PRIVATE BUILD_CVK)

# add_library("${PROJ_NAME}_s" STATIC ${PROJ_SRC})
# # target_include_directories("${PROJ_NAME}_s" PRIVATE ${vma_INCLUDE_DIR})
# target_include_directories("${PROJ_NAME}_s" PRIVATE ${Vulkan_INCLUDE_DIR})
# target_link_libraries("${PROJ_NAME}_s" ${Vulkan_LIBRARY})