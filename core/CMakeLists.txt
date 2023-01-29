set(PROJ_NAME "core")
project(${PROJ_NAME})

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB PROJ_SRC 
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/cgltf/*.cpp
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

add_library(${PROJ_NAME} SHARED ${PROJ_SRC} ${EXTERNAL_SRC})
target_include_directories(${PROJ_NAME} PUBLIC ${Vulkan_INCLUDE_DIR} PUBLIC ${tinygltf_INCLUDE_DIR} PUBLIC ktx)
target_link_libraries(${PROJ_NAME} ${Vulkan_LIBRARY} ktx)
target_compile_definitions(${PROJ_NAME} PRIVATE BUILD_CVK PUBLIC TINYGLTF_IMPLEMENTATION PUBLIC STB_IMAGE_IMPLEMENTATION PUBLIC TINYGLTF_NO_STB_IMAGE_WRITE)