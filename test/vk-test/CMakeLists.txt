set(PROJ_NAME "vk-test")
project(${PROJ_NAME})


if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJ_BASE_DIR}/bin)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJ_BASE_DIR}/bin32)
endif()

aux_source_directory(${PROJECT_SOURCE_DIR}/src PROJ_SRC)

add_executable(${PROJ_NAME} ${PROJ_SRC})

set(PROJ_LIBS "core")
set(PROJ_INCLUDE_DIR "${core_INCLUDE_DIR};${tinygltf_INCLUDE_DIR}")

target_link_libraries(${PROJ_NAME} ${PROJ_LIBS})
target_include_directories(${PROJ_NAME} PRIVATE ${PROJ_INCLUDE_DIR})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX "_d")

file(GLOB shader_files 
    ${PROJECT_SOURCE_DIR}/shader/*.vert
    ${PROJECT_SOURCE_DIR}/shader/*.frag
)
foreach(shader_file ${shader_files})
    string(REGEX REPLACE ".+/(.+)" "\\1" shader_filename ${shader_file})
    execute_process(COMMAND glslangValidator -V ${shader_file} -o "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${shader_filename}.spv" -g OUTPUT_QUIET)
    message("compiled shader ${shader_filename}.spv")
endforeach()

file(GLOB example_files ${PROJECT_SOURCE_DIR}/examples/*.cpp)
foreach(example ${example_files})
    STRING(REGEX REPLACE ".*/(.*)\\..*" "\\1" example_name ${example})
    # project(${example_name})
    add_executable(${example_name} ${example})
    target_link_libraries(${example_name} ${PROJ_LIBS})
    target_include_directories(${example_name} PRIVATE ${PROJ_INCLUDE_DIR})
    message("add execute ${example_name}")
endforeach()
