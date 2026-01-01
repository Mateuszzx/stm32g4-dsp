# STM32G4 DSP Project

This project implements Digital Signal Processing (DSP) algorithms with CMSIS-DSP Library on the STM32G4 series microcontroller (specifically STM32G491xx), giving ready to use example. It utilizes FreeRTOS for task management and a modular CMake-based build system.

## Project Structure

- **Core/**: STM32CubeMX generated initialization code, HAL configuration, and FreeRTOS setup.
- **src/**: Main application source code and DSP modules.
  - **app/**: Application tasks (Signal Acquisition, Processing, Display).
  - **signal_processing/**: DSP libraries (FIR, IIR, FFT, Statistics, etc.).
  - **io_drivers/**: Drivers for peripherals (ADC, UART).
  - **fifo/**: Circular buffer implementation.
- **utils/**: Python scripts for designing filters and generating coefficient arrays.
- **Drivers/** & **Middlewares/**: ST HAL/LL drivers and Third-party libraries (FreeRTOS, CMSIS).

## Features

- **Real-time Processing**: Leverages FreeRTOS for deterministic task scheduling.
- **DSP Algorithms**:
  - Finite Impulse Response (FIR) Filters
  - Infinite Impulse Response (IIR) Filters (Biquad/SOS)
  - Fast Fourier Transform (FFT)
  - Moving Average Filters
  - Waveform Generation
- **Build System**: CMake integration for easy configuration and building.

## Prerequisites

- **Toolchain**: ARM GNU Toolchain (`arm-none-eabi-gcc`)
- **Build Tools**: CMake, Make or Ninja
- **Hardware**: STM32G4 Development Board (e.g., Nucleo-G491RE)

## Usage

The application is divided into tasks:
- **Acquisition Task**: Samples data from ADC.
- **Signal Processing Task**: Applies configured filters (FIR/IIR) or transforms (FFT) to the signal.
- **Display/Output Task**: Outputs the processed signal via UART or DAC.

Use the scripts in the `utils/` folder to generate filter coefficients for your specific requirements.

## Contributing

Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for details on how to contribute to this project.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

The project also uses components from STMicroelectronics (HAL, CMSIS) and FreeRTOS, which are subject to their respective licenses.

