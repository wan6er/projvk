find_path(perf_INCLUDE_DIR "perf.h" "${PROJECT_SOURCE_DIR}/perf/include")
find_path(perf_LIBRARY "perf" "${PROJECT_SOURCE_DIR}/lib")

set(perf_INCLUDE_DIR "${perf_INCLUDE_DIR}")
set(perf_LIBRARY "${perf_LIBRARY}")

if(perf_INCLUDE_DIR AND perf_LIBRARY)
    set(perf_FOUND TRUE)
endif()
