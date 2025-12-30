# DSP Utilities

This directory contains Python scripts to assist in the design and generation of digital filter coefficients for the STM32G4 DSP project. These tools leverage `scipy.signal` to design filters and export them as C-compatible arrays.

## Contents

- **`fir_coeffs.py`**: Design tool for Finite Impulse Response (FIR) filters.
- **`iir_coeffs.py`**: Design tool for Infinite Impulse Response (IIR) filters.
- **`requirements.txt`**: Python dependencies.
- **`tools/`**: Helper modules for filter types and C-code generation.

## Setup

Ensure you have Python installed. It is recommended to use a virtual environment.

1.  **Install dependencies**:
    ```bash
    pip install -r requirements.txt
    ```

## Usage

### Generating FIR Coefficients

1.  Open `fir_coeffs.py`.
2.  Modify the **FILTER CONFIGURATION** section at the top of the file:
    ```python
    filter_type = FilterTypes.LOWPASS
    fir_type = FIRDesignMethods.FIRWIN
    numtaps = 51      # Filter order + 1
    fs = 1000         # Sampling frequency (Hz)
    cutoff = 40       # Cutoff frequency (Hz)
    window = WindowTypes.HAMMING
    ```
3.  Run the script:
    ```bash
    python fir_coeffs.py
    ```
4.  The script will print the generated C array to the console (and optionally plot the frequency response). Copy this array into your C code (e.g., in `src/app/signal_processing_task.c`).

### Generating IIR Coefficients

1.  Open `iir_coeffs.py`.
2.  Modify the **FILTER CONFIGURATION** section:
    ```python
    filter_type = FilterTypes.LOWPASS
    design_method = IIRDesignMethods.BUTTER
    num_stages = 3    # Number of biquad stages
    fs = 1000         # Sampling frequency (Hz)
    cutoff = 450      # Cutoff frequency (Hz)
    output_format = IIROutputFormat.SOS # Recommended for stability
    ```
3.  Run the script:
    ```bash
    python iir_coeffs.py
    ```
4.  Copy the generated coefficients (SOS matrix or Numerator/Denominator) to your firmware.

## Supported Methods

- **FIR**: Window method (`firwin`), Frequency sampling (`firwin2`), Least squares (`firls`), Remez exchange (`remez`).
- **IIR**: Butterworth, Chebyshev Type I/II, Elliptic, Bessel.
