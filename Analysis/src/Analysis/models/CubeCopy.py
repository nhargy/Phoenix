from dataclasses import dataclass
from typing import Tuple

@dataclass
class CubeCopy():
    name         : str
    centre_coord : Tuple[float, float, float] = (0., 0., 0.)
    edge_length  : float = 10 #mm
    alpha        : float = 0.1
    colour       : str   = "yellow"

CopyCube_dict = {
    
    0 : CubeCopy("Cube 0", (100,0,196.1) ) ,
    1 : CubeCopy("Cube 1", (0,100,196.1) ) ,
    2 : CubeCopy("Cube 2", (-100,0,196.1)) ,
    3 : CubeCopy("Cube 3", (0,-100,196.1))

}