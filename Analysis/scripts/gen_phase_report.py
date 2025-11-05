import numpy as np
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec
import pandas as pd
import json
from matplotlib.backends.backend_pdf import PdfPages
from datetime import datetime
import sys
import os
from pathlib import Path


import Analysis.config.config          as cfg
from   Analysis.utils              import G4Tools, XTools, SigTools
from   Analysis.models.PDGParticle import PDGParticle_dict

import matplotlib as mpl
from cycler import cycler

mpl.rcParams.update({
    # Typography
    "font.family": "serif",
    "font.size": 10,
    "axes.titlesize": 12,
    "axes.labelsize": 11,
    "legend.fontsize": 9,
    "figure.titlesize": 14,

    # Lines & markers — thin, crisp, old-school
    "lines.linewidth": 1.0,
    "lines.markersize": 4,
    "lines.markeredgewidth": 0.8,

    # Axes styling
    "axes.spines.top": True,
    "axes.spines.right": True,         # old papers usually had boxed axes
    "axes.grid": False,                 # if you like grids, make them faint (see below)
    "axes.prop_cycle": cycler(color=[
        "#1f77b4",  # muted blue
        "#d62728",  # muted red
        "#2ca02c",  # muted green
        "#9467bd",  # purple
        "#8c564b",  # brown
        "#e377c2",  # rose
        "#7f7f7f",  # gray
        "#bcbd22",  # olive
        "#17becf",  # teal
    ]),

    # Ticks — inward, minor ticks on
    "xtick.direction": "in",
    "ytick.direction": "in",
    "xtick.minor.visible": True,
    "ytick.minor.visible": True,
    "xtick.major.size": 4,
    "ytick.major.size": 4,
    "xtick.minor.size": 2,
    "ytick.minor.size": 2,

    # Figure
    "figure.dpi": 150,
    "savefig.dpi": 300,
    "savefig.facecolor": "white",

})


# --------------- CONFIG -----------------------------------------------------
pdf_path = "report1.pdf"
phasepath = Path(sys.argv[1])
A4       = (8.27, 11.69)
E_photon = 2.786 #eV
eV_to_J  = 1.602e-19
photons_per_mW = (1e-3) / (E_photon * eV_to_J)