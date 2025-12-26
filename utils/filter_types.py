from __future__ import annotations
from enum import Enum
from dataclasses import dataclass

class FilterTypes(Enum):
    LOWPASS = "lowpass"
    HIGHPASS = "highpass"
    BANDPASS = "bandpass"
    BANDSTOP = "bandstop"

class WindowTypes(Enum):
    HAMMING = "hamming"
    HANN = "hann"
    BLACKMAN = "blackman"
    BARTLETT = "bartlett"


Number = int | float
Cutoff = Number | tuple[Number, Number]

@dataclass(frozen=True)
class BandSpec:
    # For firwin2 / firls (piecewise-linear amplitude): breakpoints + gain at each breakpoint
    freq_breaks: list[float]
    gain_at_breaks: list[float]

    # For remez: bands are pairs (edges), desired is per-band
    remez_bands: list[float]
    remez_desired: list[float]


def _clamp(x: float, lo: float, hi: float) -> float:
    return max(lo, min(hi, x))

def _validate_cutoff(filter_type, cutoff: Cutoff, nyq: float) -> tuple[float, float] | float:
    if filter_type in (FilterTypes.LOWPASS, FilterTypes.HIGHPASS):
        if isinstance(cutoff, (tuple, list)):
            raise ValueError(f"{filter_type.value}: cutoff must be scalar Hz, got {cutoff}")
        fc = float(cutoff)
        if not (0.0 < fc < nyq):
            raise ValueError(f"{filter_type.value}: cutoff must be in (0, nyq), got {fc} (nyq={nyq})")
        return fc

    # band filters
    if not isinstance(cutoff, (tuple, list)) or len(cutoff) != 2:
        raise ValueError(f"{filter_type.value}: cutoff must be (f1, f2) Hz, got {cutoff}")
    f1, f2 = float(cutoff[0]), float(cutoff[1])
    if not (0.0 < f1 < f2 < nyq):
        raise ValueError(f"{filter_type.value}: require 0 < f1 < f2 < nyq, got {(f1, f2)} (nyq={nyq})")
    return (f1, f2)

def build_band_spec(
    *,
    filter_type: FilterTypes,
    cutoff: Cutoff,
    fs: float,
    transition: float = 1000.0,
) -> BandSpec:
    nyq = fs / 2.0
    tw = float(transition)
    if tw <= 0:
        raise ValueError("transition must be > 0 Hz")

    c = _validate_cutoff(filter_type, cutoff, nyq)

    if filter_type == FilterTypes.LOWPASS:
        fp = float(c)
        fsb = _clamp(fp + tw, 0.0, nyq)
        freq = [0.0, fp, fsb, nyq]
        gain = [1.0, 1.0, 0.0, 0.0]
        remez_bands = [0.0, fp, fsb, nyq]
        remez_desired = [1.0, 0.0]
        return BandSpec(freq, gain, remez_bands, remez_desired)

    if filter_type == FilterTypes.HIGHPASS:
        fp = float(c)
        fsb = _clamp(fp - tw, 0.0, nyq)
        # Ensure monotonic increasing edges: [0, fsb, fp, nyq]
        if fsb >= fp:
            fsb = max(0.0, fp * 0.9)
        freq = [0.0, fsb, fp, nyq]
        gain = [0.0, 0.0, 1.0, 1.0]
        remez_bands = [0.0, fsb, fp, nyq]
        remez_desired = [0.0, 1.0]
        return BandSpec(freq, gain, remez_bands, remez_desired)

    f1, f2 = c  # type: ignore[misc]

    if filter_type == FilterTypes.BANDPASS:
        f1s = _clamp(f1 - tw, 0.0, nyq)
        f2s = _clamp(f2 + tw, 0.0, nyq)
        if not (0.0 <= f1s < f1 < f2 < f2s <= nyq):
            raise ValueError("Invalid bandpass edges after applying transition; adjust cutoff/transition.")
        freq = [0.0, f1s, f1, f2, f2s, nyq]
        gain = [0.0, 0.0, 1.0, 1.0, 0.0, 0.0]
        remez_bands = [0.0, f1s, f1, f2, f2s, nyq]
        remez_desired = [0.0, 1.0, 0.0]
        return BandSpec(freq, gain, remez_bands, remez_desired)

    if filter_type == FilterTypes.BANDSTOP:
        f1s = _clamp(f1 - tw, 0.0, nyq)
        f2s = _clamp(f2 + tw, 0.0, nyq)
        if not (0.0 <= f1s < f1 < f2 < f2s <= nyq):
            raise ValueError("Invalid bandstop edges after applying transition; adjust cutoff/transition.")
        freq = [0.0, f1s, f1, f2, f2s, nyq]
        gain = [1.0, 1.0, 0.0, 0.0, 1.0, 1.0]
        remez_bands = [0.0, f1s, f1, f2, f2s, nyq]
        remez_desired = [1.0, 0.0, 1.0]
        return BandSpec(freq, gain, remez_bands, remez_desired)

    raise ValueError(f"Unsupported FilterTypes: {filter_type}")
