project(ktx)

set(KTX_DIR ${PROJ_BASE_DIR}/external/ktx)
set(KTX_SOURCES
    ${KTX_DIR}/lib/texture.c
    ${KTX_DIR}/lib/hashlist.c
    ${KTX_DIR}/lib/checkheader.c
    ${KTX_DIR}/lib/swap.c
    ${KTX_DIR}/lib/memstream.c
    ${KTX_DIR}/lib/filestream.c)

add_library(ktx STATIC ${KTX_SOURCES})
target_include_directories(ktx PUBLIC ${PROJECT_SOURCE_DIR}/include PUBLIC ${PROJECT_SOURCE_DIR}/other_include)
if(WIN32)
    target_link_libraries(ktx ${Vulkan_LIBRARY} ${WINLIBS})
else()
    # target_link_libraries(ktx ${Vulkan_LIBRARY} ${XCB_LIBRARIES} ${WAYLAND_CLIENT_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif()

# install(TARGETS ktx
#     EXPORT ktx-targets
# )