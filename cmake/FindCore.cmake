find_path(core_INCLUDE_DIR cvk.h ${PROJECT_SOURCE_DIR}/core/include/)
find_library(core_LIBRARY core ${PROJECT_SOURCE_DIR}/lib ${PROJECT_SOURCE_DIR}/lib32)

set(core_INCLUDE_DIR ${core_INCLUDE_DIR})
set(core_LIBRARY ${core_LIBRARY})

if (core_INCLUDE_DIR AND core_LIBRARY)
    set(core_FOUND TRUE)
endif()