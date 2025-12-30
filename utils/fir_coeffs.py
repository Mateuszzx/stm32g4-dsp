from enum import Enum
import numpy as np
from numpy.typing import NDArray
from scipy.signal import firwin, firwin2, firls, remez, savgol_coeffs

from tools.filter_types import FilterTypes, WindowTypes, build_band_spec
from tools.save_to_c import c_array


class FIRDesignMethods(Enum):
    FIRWIN = "firwin"
    FIRWIN2 = "firwin2"
    FIRLS = "firls"
    REMEZ = "remez"
    SAVGOL = "savgol"


#######################################################################################
# FILTER CONFIGURATION
#######################################################################################
filter_type = FilterTypes.LOWPASS # Filter type: LOWPASS, HIGHPASS, BANDPASS, BANDSTOP
fir_type = FIRDesignMethods.FIRWIN # Change this to select different FIR design methods   
numtaps = 51  # Number of taps (filter order + 1)
fs = 1000  # Sampling frequency
cutoff = 40 # Cutoff frequency
window = WindowTypes.HAMMING
transition = 1.0
#######################################################################################

#######################################################################################
fir_registry = {}

def register_fir(method_enum):
    def decorator(func):
        fir_registry[method_enum] = func
        return func
    return decorator

#######################################################################################
@register_fir(FIRDesignMethods.FIRWIN)
def firwin_design(numtaps: int,
                cutoff: float,
                fs: float,
                window="hamming", filter_type=FilterTypes.LOWPASS, **kwargs):
    """Design FIR filter using the firwin method."""
    return firwin(numtaps, cutoff, fs=fs, window=window, pass_zero=filter_type.value)

@register_fir(FIRDesignMethods.FIRWIN2)
def firwin2_design(numtaps, cutoff, fs, window="hamming", filter_type=FilterTypes.LOWPASS, transition: float = 100. ):
    """Design FIR filter using the firwin2 method."""

    spec = build_band_spec(filter_type=filter_type,
                            cutoff=cutoff,
                            fs=fs,
                            transition=transition)
 
    return firwin2(numtaps, spec.freq_breaks, spec.gain_at_breaks, fs=fs, window=window)

@register_fir(FIRDesignMethods.FIRLS)
def firls_design(numtaps, cutoff, fs, window=None, filter_type=FilterTypes.LOWPASS, transition: float = 100.0):
    """Design FIR filter using the firls method."""
    spec = build_band_spec(filter_type=filter_type,
                            cutoff=cutoff,
                            fs=fs,
                            transition=transition)
 
    return firls(numtaps, spec.freq_breaks, spec.gain_at_breaks, fs=fs)

@register_fir(FIRDesignMethods.REMEZ)
def remez_design(numtaps, cutoff, fs, window=None, filter_type=FilterTypes.LOWPASS, transition: float = 100.0):
    """Design FIR filter using the remez method."""
    spec = build_band_spec(filter_type=filter_type,
                            cutoff=cutoff,
                            fs=fs,
                            transition=transition)
    return remez(numtaps, spec.remez_bands, spec.remez_desired, fs=fs)

@register_fir(FIRDesignMethods.SAVGOL)
def savgol_design(numtaps, cutoff, fs, window=None, filter_type=FilterTypes.LOWPASS, **kwargs) -> NDArray:
    if filter_type != FilterTypes.LOWPASS:
        raise ValueError("SAVGOL supports only LOWPASS-like smoothing in this design wrapper.")
    window_length = numtaps if numtaps % 2 != 0 else numtaps + 1
    polyorder = 7
    return savgol_coeffs(window_length, polyorder, deriv=0, delta=1.0)
#######################################################################################

def fir_coeffs(fir_type: FIRDesignMethods,
                numtaps: int,
                cutoff: float,
                fs: float,
                window=WindowTypes.HAMMING,
                filter_type=FilterTypes.LOWPASS,
                transition: float = 100.0) -> NDArray:
    if fir_type in fir_registry:
        return fir_registry[fir_type](numtaps, cutoff, fs, window=window.value, filter_type=filter_type, transition=transition)
    else:
        raise ValueError(f"FIR design method {fir_type} not found.")


if __name__ == "__main__":

    if fir_type in fir_registry:
        
        print(f"Generating coefficients using {fir_type.value} with {window.value} window and {filter_type.value} type...")
        
        coeffs = fir_coeffs(fir_type=fir_type,
                            numtaps=numtaps,
                            cutoff=cutoff,
                            fs=fs,
                            window=window,
                            filter_type=filter_type,
                            transition=transition)
        
        # Print C array
        print(c_array(name="fir_coeffs", arr=coeffs, ctype="float"))

        # Plot Kernel
        import matplotlib.pyplot as plt
        fig, (ax_time, ax_freq) = plt.subplots(2, 1, figsize=(10, 6))
        ax_time.stem(coeffs)
        ax_time.set_title(f"FIR Coefficients ({fir_type.value})")
        ax_time.set_xlabel("Sample Index")
        ax_time.set_ylabel("Coefficient Value")
        # Frequency Response
        w, h = np.fft.rfftfreq(2048, 1/fs), np.fft.rfft(coeffs, 2048)
        ax_freq.plot(w, 20 * np.log10(np.abs(h)))
        ax_freq.set_title("Frequency Response")
        ax_freq.set_xlabel("Frequency (Hz)")
        ax_freq.set_ylabel("Magnitude (dB)")
        ax_freq.grid()
        plt.tight_layout()
        plt.show()

    else:
        print(f"Method {fir_type} not found.")

