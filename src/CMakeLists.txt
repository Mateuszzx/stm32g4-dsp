# `src/CMakeLists.txt`
# This file is included by the root CMakeLists. It is responsible for:
# - Adding optional top-level `driver.c` to the main target
# - Adding each component subdirectory under `src/` that contains its own
#   `CMakeLists.txt` (making components optional)

# Add top-level driver file if present
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/driver.c)
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/driver.c)
endif()


# Add each child directory that provides a CMakeLists.txt
file(GLOB CHILDREN RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
foreach(_child ${CHILDREN})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${_child} AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_child}/CMakeLists.txt)
        message(STATUS "Adding src component: src/${_child}")
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${_child})
    endif()
endforeach()
