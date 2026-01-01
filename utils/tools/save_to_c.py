from numpy.typing import NDArray
import numpy as np

def c_array(name: str, arr: NDArray, ctype: str) -> str:
    """
    Docstring for c_array function.
    
    :param name: Function name
    :type name: str
    :param arr: Array of coefficients
    :type arr: NDArray
    :param ctype: C data type
    :type ctype: str
    :return: C array as a string
    :rtype: str
    """
    vals = ", ".join(f"{v:.8f}f" for v in arr)
    return f"static const {ctype} {name}[{len(arr)}] = {{ {vals} }};"

