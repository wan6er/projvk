set(PROJ_NAME "vk-test")
project(${PROJ_NAME})


if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJ_BASE_DIR}/bin)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJ_BASE_DIR}/bin32)
endif()

aux_source_directory(${PROJECT_SOURCE_DIR}/src PROJ_SRC)

add_executable(${PROJ_NAME} ${PROJ_SRC})

target_link_libraries(${PROJ_NAME} core)
target_include_directories(${PROJ_NAME} PRIVATE ${vma_INCLUDE_DIR})
target_include_directories(${PROJ_NAME} PRIVATE ${core_INCLUDE_DIR})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX "_d")

file(GLOB shader_files 
    ${PROJECT_SOURCE_DIR}/shader/*.vert
    ${PROJECT_SOURCE_DIR}/shader/*.frag
)
foreach(shader_file ${shader_files})
    string(REPLACE "/" ";" shader_file_list ${shader_file})
    list(GET shader_file_list -1 shader_filename)
    execute_process(COMMAND glslangValidator -V ${shader_file} -o "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${shader_filename}.spv" -g)
    message("compiled shader ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${shader_filename}.spv")

endforeach()
