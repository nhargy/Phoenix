from dataclasses import dataclass

@dataclass
class PDGParticle():
    name      : str
    label     : str
    colour    : str
    linewidth : float
    linestyle : str = "solid"

PDGParticle_dict = {

    # Leptons
    11         : PDGParticle("Electron", "e⁻", "cyan", 1, "dashdot") ,
    -11        : PDGParticle("Positron", "e⁺", "red" , 1, "dashdot") ,

    # Gamma
    22         : PDGParticle("Gamma", "γ", "gold", 0.9, "dotted") ,

    # Nucleons
    2112       : PDGParticle("Neutron", "n", "green"  , 2) ,
    2212       : PDGParticle("Proton",  "p", "maroon" , 2) ,

    # Molecules
    1000030070 : PDGParticle("Lithium-7"      , "Li7", "magenta"    , 3)   ,
    1000030060 : PDGParticle("Lithium-6"      , "Li6", "purple"     , 3)   ,
    1000090190 : PDGParticle("Fluorine-19"    , "F19", "blue"       , 4)   ,
    1000090200 : PDGParticle("Fluorine-20"    , "F20", "darkblue"   , 4.2) ,
    1000020040 : PDGParticle("Helium-4"       , "He4", "darkorange" , 1.5) ,
    1000020060 : PDGParticle("Helium-6"       , "He6", "orange"     , 1.7) ,
    1000010030 : PDGParticle("Tritium"        , "H3" , "azure"      , 1.2) ,
    1000070160 : PDGParticle("Nitrogen-16"    , "N16", "plum"       , 3.8) ,
    1000080160 : PDGParticle("Oxygen-16"      , "O16", "darkgrey"   , 3.8) ,
    1000080190 : PDGParticle("Oxygen-19"      , "O19", "silver"     , 4)   
    
}