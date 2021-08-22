set(PROJ_NAME "core")
project(${PROJ_NAME})

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
else()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib32)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib32)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib32)
endif()


include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(${PROJECT_SOURCE_DIR}/src PROJ_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/cvk PROJ_cvk_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/cvk/initialize PROJ_cvk_initialize_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/utils PROJ_utils_SRC)

add_library(${PROJ_NAME} 
    ${PROJ_SRC}
    ${PROJ_cvk_SRC}
    ${PROJ_cvk_initialize_SRC}
    
    ${PROJ_utils_SRC}
)

target_include_directories(${PROJ_NAME} PRIVATE ${Vulkan_INCLUDE_DIR})
target_link_libraries(${PROJ_NAME} ${Vulkan_LIBRARY})