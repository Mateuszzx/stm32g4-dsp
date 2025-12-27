# Add component sources directly to the main executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    lms_filter.c
)

# Add the component directory for includes
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})