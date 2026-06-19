set(PROJ_NAME "framework")
project(${PROJ_NAME})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/render FRAMEWORK_SRC)

add_library(${PROJ_NAME} STATIC ${FRAMEWORK_SRC})
target_include_directories(${PROJ_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/include)
target_link_libraries(${PROJ_NAME} PUBLIC core)
