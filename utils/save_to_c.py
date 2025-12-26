from numpy.typing import NDArray
import numpy as np

def c_array(name: str, arr: NDArray, ctype: str) -> str:
    vals = ", ".join(str(int(v)) if np.issubdtype(arr.dtype, np.integer) else f"{v:.9g}f" for v in arr)
    return f"static const {ctype} {name}[{len(arr)}] = {{ {vals} }};"

