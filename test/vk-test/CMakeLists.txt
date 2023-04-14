set(PROJ_NAME "vk-test")
project(${PROJ_NAME})


aux_source_directory(${PROJECT_SOURCE_DIR}/src PROJ_SRC)

add_executable(${PROJ_NAME} ${PROJ_SRC})

set(PROJ_LIBS "core;lockfree")
set(PROJ_INCLUDE_DIR "${core_INCLUDE_DIR};${lockfree_INCLUDE_DIR}")
# set(PROJ_EXT_LIBS ${SDL2_LIBRARIES})
# set(PROJ_EXT_INCLUDE_DIR ${SDL2_INCLUDE_DIR})
set(PROJ_EXT_LIBS "")
set(PROJ_EXT_INCLUDE_DIR "")

if(LINUX)
    set(PROJ_EXT_LIBS "pthread;${ProjDeps_LIBRARIES}")
    set(PROJ_EXT_INCLUDE_DIR "${ProjDeps_INCLUDE_DIRS}")
    
endif()

target_link_libraries(${PROJ_NAME} ${PROJ_LIBS} ${PROJ_EXT_LIBS})
target_include_directories(${PROJ_NAME} PRIVATE ${PROJ_INCLUDE_DIR})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX "_d")

function(install_resource src_dir dst_dir res_postfix)
    file(GLOB RES_FILES "${src_dir}/${res_postfix}")
    file(COPY ${RES_FILES} DESTINATION ${dst_dir})
endfunction()

function(install_shaders src_dir dst_dir)
    install_resource(${src_dir} ${dst_dir} *.vert)
    install_resource(${src_dir} ${dst_dir} *.frag)
    file(GLOB shader_files 
        ${dst_dir}/*.vert
        ${dst_dir}/*.frag
    )
    foreach(shader_file ${shader_files})
        string(REGEX REPLACE ".+/(.+)" "\\1" shader_filename ${shader_file})
        execute_process(COMMAND glslangValidator -V ${shader_file} -o "${dst_dir}/${shader_filename}.spv" -g)
        # message("compiled shader ${shader_filename}.spv")
    endforeach()
    file(REMOVE ${shader_files})
endfunction()

install_shaders(${PROJECT_SOURCE_DIR}/shader ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shader )
install_resource(${PROJECT_SOURCE_DIR}/texture ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/texture *.ktx)
install_resource(${PROJECT_SOURCE_DIR}/models ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/model *.gltf)
install_resource(${PROJECT_SOURCE_DIR}/models ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/model *.gltf)

file(GLOB example_files ${PROJECT_SOURCE_DIR}/examples/*.cpp)
foreach(example ${example_files})
    STRING(REGEX REPLACE ".*/(.*)\\..*" "\\1" example_name ${example})
    # project(${example_name})
    add_executable(${example_name} ${example})
    target_link_libraries(${example_name} ${PROJ_LIBS} ${PROJ_EXT_LIBS})
    target_include_directories(${example_name} PRIVATE ${PROJ_INCLUDE_DIR} ${PROJ_EXT_INCLUDE_DIR})
    message("add execute ${example_name}")
endforeach()
