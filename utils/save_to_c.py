from numpy.typing import NDArray
import numpy as np

def c_array(name: str, arr: NDArray, ctype: str) -> str:
    vals = ", ".join(f"{v:.8f}f" for v in arr)
    return f"static const {ctype} {name}[{len(arr)}] = {{ {vals} }};"

