find_path(lockfree_INCLUDE_DIR "lockfree.h" "${PROJECT_SOURCE_DIR}/lockfree/include")
find_path(lockfree_LIBRARY "lockfree" "${PROJECT_SOURCE_DIR}/lib")

set(lockfree_INCLUDE_DIR "${lockfree_INCLUDE_DIR}")
set(lockfree_LIBRARY "${lockfree_LIBRARY}")

if(lockfree_INCLUDE_DIR AND lockfree_LIBRARY)
    set(lockfree_FOUND TRUE)
endif()
