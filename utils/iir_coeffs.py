from enum import Enum
import numpy as np
from numpy.typing import NDArray
from scipy.signal import iirfilter, sosfreqz, freqz

from filter_types import FilterTypes, _validate_cutoff
from save_to_c import c_array


class IIRDesignMethods(Enum):
    BUTTER = "butter"
    CHEBY1 = "cheby1"
    CHEBY2 = "cheby2"
    ELLIP = "ellip"
    BESSEL = "bessel"

class IIROutputFormat(Enum):
    SOS = "sos"
    BA = "ba"

#######################################################################################
# FILTER CONFIGURATION
#######################################################################################
filter_type = FilterTypes.LOWPASS # Filter type: LOWPASS, HIGHPASS, BANDPASS, BANDSTOP
design_method = IIRDesignMethods.BUTTER # Design method
num_stages = 3 # Number of biquad stages
order = 2 * num_stages # Calculate order based on stages (2 poles per stage)
fs = 1000  # Sampling frequency
cutoff = 50  # Cutoff frequency (scalar for LP/HP, tuple for BP/BS)
rp = 1.0   # Passband ripple in dB (for cheby1, ellip)
rs = 40.0  # Stopband attenuation in dB (for cheby2, ellip)
output_format = IIROutputFormat.SOS # Output format: SOS (Second-Order Sections) or BA (Numerator/Denominator)
# output_format = IIROutputFormat.BA # Output format: SOS (Second-Order Sections) or BA (Numerator/Denominator)
#######################################################################################

def iir_design(method: IIRDesignMethods,
               order: int,
               cutoff: float | tuple[float, float],
               fs: float,
               filter_type: FilterTypes = FilterTypes.LOWPASS,
               rp: float = 1.0,
               rs: float = 40.0,
               output: IIROutputFormat = IIROutputFormat.SOS):
    
    # Validate and get cutoff in Hz
    nyq = 0.5 * fs
    c = _validate_cutoff(filter_type, cutoff, nyq)
    
    # Normalize cutoff for scipy (0 to 1 where 1 is Nyquist)
    if isinstance(c, (list, tuple)):
        Wn = [freq / nyq for freq in c]
    else:
        Wn = c / nyq

    return iirfilter(N=order, Wn=Wn, rp=rp, rs=rs, btype=filter_type.value, 
                     analog=False, ftype=method.value, output=output.value)


if __name__ == "__main__":
    print(f"Generating IIR coefficients using {design_method.value} ({filter_type.value})...")
    
    try:
        coeffs = iir_design(method=design_method,
                            order=order,
                            cutoff=cutoff,
                            fs=fs,
                            filter_type=filter_type,
                            rp=rp,
                            rs=rs,
                            output=output_format)

        import matplotlib.pyplot as plt
        fig, (ax_time, ax_freq) = plt.subplots(2, 1, figsize=(10, 6))

        ax_time.set_title(f"IIR Filter Coefficients ({design_method.value}, order={order})")
        if output_format == IIROutputFormat.SOS:
            ax_time.stem(coeffs[:, :3].flatten(), label="SOS B Coeffs")
        else:
            b, a = coeffs
            ax_time.stem(b, label="B Coeffs", linefmt='C0-', markerfmt='C0o')
            ax_time.stem(a, label="A Coeffs", linefmt='C1-', markerfmt='C1x')

        ax_time.set_xlabel("Coefficient Index")
        ax_time.set_ylabel("Coefficient Value")
        ax_time.legend()
        ax_time.grid()

        if output_format == IIROutputFormat.SOS:
            print(f"#define IIR_NUM_STAGES {len(coeffs)}")
            # Flatten for C array
            flat_sos = coeffs.flatten()
            # print(c_array("iir_sos_coeffs", flat_sos, "float"))

            # Generate ARM CMSIS-DSP specific format
            # Order: {b10, b11, b12, a11, a12, b20, b21, b22, a21, a22, ...}
            # where a coefficients are negated.
            arm_coeffs = []
            for section in coeffs:
                b0, b1, b2, a0, a1, a2 = section
                # Normalize by a0 if it's not 1.0 (though scipy usually returns a0=1.0)
                if a0 != 1.0:
                    b0 /= a0
                    b1 /= a0
                    b2 /= a0
                    a1 /= a0
                    a2 /= a0
                
                # Append in ARM order: b0, b1, b2, -a1, -a2
                arm_coeffs.extend([b0, b1, b2, -a1, -a2])
            
            print(c_array("lowpass_coeffs", np.array(arm_coeffs), "float32_t"))
            
            # Frequency Response
            w, h = sosfreqz(coeffs, worN=2048, fs=fs)
            
        else:
            b, a = coeffs
            print(c_array("iir_coeffs_b", b, "float"))
            print(c_array("iir_coeffs_a", a, "float"))
            
            # Frequency Response
            w, h = freqz(b, a, worN=2048, fs=fs)

        ax_freq.plot(w, 20 * np.log10(np.abs(h)))
        ax_freq.set_title(f"IIR Frequency Response ({design_method.value}, order={order})")
        ax_freq.set_xlabel("Frequency (Hz)")
        ax_freq.set_ylabel("Magnitude (dB)")
        ax_freq.grid()
        ax_freq.axvline(cutoff if isinstance(cutoff, (int, float)) else cutoff[0], color='r', linestyle='--', alpha=0.5)
        if isinstance(cutoff, (tuple, list)):
             ax_freq.axvline(cutoff[1], color='r', linestyle='--', alpha=0.5)

        plt.tight_layout()
        plt.show()

    except Exception as e:
        print(f"Error generating coefficients: {e}")
