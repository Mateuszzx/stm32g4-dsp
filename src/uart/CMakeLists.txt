add_library(uart_driver STATIC
    uart_driver.c
)

# Add the component directory itself so #include "sine_generator.h" works
target_include_directories(uart_driver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link to stm32cubemx to get HAL, CMSIS, and DSP includes
target_link_libraries(uart_driver PUBLIC stm32cubemx)

# Link the main executable to this component
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE uart_driver)