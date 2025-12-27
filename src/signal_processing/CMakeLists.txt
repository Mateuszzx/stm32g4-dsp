# Add each child directory that provides a CMakeLists.txt
file(GLOB CHILDREN RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
foreach(_child ${CHILDREN})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${_child} AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_child}/CMakeLists.txt)
        message(STATUS "Adding src component: src/${_child}")
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${_child})
    endif()
endforeach()
