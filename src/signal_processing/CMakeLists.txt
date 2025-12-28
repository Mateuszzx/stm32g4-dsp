# Options to enable/disable specific DSP modules

option(DSP_ENABLE_ADAPTIVE_FILTERS "Enable Adaptive Filters" OFF)
option(DSP_ENABLE_FIR "Enable FIR Filters" OFF)
option(DSP_ENABLE_IIR "Enable IIR Filters" ON)
option(DSP_ENABLE_MOVING_AVERAGE "Enable Moving Average Filters" OFF)
option(DSP_ENABLE_STATISTICS "Enable Statistics Calculation" OFF)
option(DSP_ENABLE_WAVEFORM_GENERATOR "Enable Waveform Generator" ON)
option(DSP_ENABLE_FFT "Enable FFT Module" ON)


if(DSP_ENABLE_FFT AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/fft/CMakeLists.txt)
    add_subdirectory(fft)
endif()

if(DSP_ENABLE_ADAPTIVE_FILTERS AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/adaptive_filters/CMakeLists.txt)
    add_subdirectory(adaptive_filters)
endif()

if(DSP_ENABLE_FIR AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/fir/CMakeLists.txt)
    add_subdirectory(fir)
endif()

if(DSP_ENABLE_IIR AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/iir/CMakeLists.txt)
    add_subdirectory(iir)
endif()

if(DSP_ENABLE_MOVING_AVERAGE AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/moving_average/CMakeLists.txt)
    add_subdirectory(moving_average)
endif()

if(DSP_ENABLE_STATISTICS AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/statistics_calculation/CMakeLists.txt)
    add_subdirectory(statistics_calculation)
endif()

if(DSP_ENABLE_WAVEFORM_GENERATOR AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/waveform_generator/CMakeLists.txt)
    add_subdirectory(waveform_generator)
endif()
