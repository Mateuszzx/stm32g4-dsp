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

# Example per-component CMakeLists.txt (place in `src/<component>/CMakeLists.txt`):
#
# # Create a library for this component
# add_library(wave_gen STATIC
#     sine_generator.c
# )

# # Add the component directory itself so #include "sine_generator.h" works
# target_include_directories(wave_gen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# # Link to stm32cubemx to get HAL, CMSIS, and DSP includes
# target_link_libraries(wave_gen PUBLIC stm32cubemx)

# # Link the main executable to this component
# target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE wave_gen)
#
# To disable a component, remove or rename its `CMakeLists.txt` and the root
# build system will skip it automatically.
