import numpy as np
from numpy import ndarray
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
    "axes.spines.right": True,         
    "axes.grid": False,
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
#collpath = Path(sys.argv[1])
A4       = (8.27, 11.69)
E_photon = 2.786 #eV
eV_to_J  = 1.602e-19
photons_per_mW = (1e-3) / (E_photon * eV_to_J)

# --------------- AUX FUNCTIONS ----------------------------------------------


# --------------- PAGE FUNCTIONS ---------------------------------------------

def page_title(pdf, 
               collpath: Path, 
               A4      : tuple[float ,float] = (8.27, 11.69),
               desc    : str = " "
               ) -> None:
    """
    
    """
    fig = plt.figure(figsize=A4)
    plt.axis("off")

    coll  = collpath.name
    phase = collpath.parent.name

    title = f"Report: {phase} {coll}"
    subtitle = "Project Phoenix"
    ts = datetime.now().strftime("%Y-%m-%d %H:%M")

    fig.text(0.5, 0.85, title, ha="center", va="center", fontsize=26, weight="bold")
    fig.text(0.5, 0.80, subtitle, ha="center", va="center", fontsize=14)
    fig.text(0.5, 0.75, f"Created: {ts}", ha="center", va="center", fontsize=10)

    # Short description block
    intro_text = (
        desc
    )
    fig.text(0.1, 0.6, intro_text, ha="left", va="top", fontsize=11, wrap=True)
    pdf.savefig(fig)
    plt.close(fig)
    
    return None


def page_acquisition(pdf,
                     collpath : Path
                     ) -> None:
    """
    
    """
    # ==================== #
    # === Extract Data === #
    # ==================== #

    # Acqusition data dataframe
    filepath = collpath / 'BL' / 'it_0.txt'
    acq_dict = SigTools.read_acquisition_data(filepath)

    unwanted_keys = ['Date and Time', 'Exposure Time (secs)', 'SR193i']
    for unwanted_key in unwanted_keys:
        acq_dict.pop(unwanted_key)
    try:
        acq_dict.pop('Temperature (C)')
    except:
        acq_dict.pop('Unstabilized Temperature (C)')
    acq_df = pd.DataFrame(list(acq_dict.items()), columns=[" ", "Value"])

    # Temperature timeline
    times, temps, cuts = SigTools.get_coll_temp_timeline(collpath)

    # ========================= #
    # === Setup Page Figure === #
    # ========================= #
    fig = plt.figure(figsize=A4)
    fig.suptitle("Collection Acquisition Data", fontsize = 20)

    gs = fig.add_gridspec(
        2, 1,
        left=0.15, right=0.85,
        top=0.9, bottom=0.1,
        wspace=0.25, hspace=0.3
    )

    ax1 = fig.add_subplot(gs[0])
    ax2 = fig.add_subplot(gs[1])

    # ============ #
    # === Plot === #
    # ============ #
    ax1.axis("off")
    tbl = ax1.table(
        cellText=acq_df.values,
        colLabels=acq_df.columns,
        loc="center",
        cellLoc="left",
    )
    tbl.auto_set_font_size(False)
    tbl.set_fontsize(10)
    tbl.scale(1, 1.3)

    SigTools.plot_temp_timeline(times, temps, cuts, ax=ax2)

    pdf.savefig(fig)
    plt.close(fig)

    return None


def page_exposure(pdf, 
                  collpath : Path,
                  exposure : str
                  ) -> tuple[ndarray, ndarray]:
    """
    
    """

    # Direct to baseline folder
    dirpath = collpath / exposure

    # ==================== #
    # === Extract Data === #
    # ==================== #

    # Metadata table
    it_0_filepath = dirpath / 'it_0.txt'
    x, _         = SigTools.read_spectrum(it_0_filepath)
    acq_dict = SigTools.read_acquisition_data(it_0_filepath)
    date_n_time = acq_dict['Date and Time']
    t_exp = float(acq_dict['Exposure Time (secs)'])
    gain  = int(acq_dict['Pre-Amplifier Gain'][0])
    slit  = int(acq_dict['Input Side Slit Width (um)'])
    binsize = np.round(np.mean(np.diff(x)),3)

    meta_table = {
        "Date and Time" : date_n_time,
        "Exposure Time (sec)" : t_exp,
        "Pre-Amplifier Gain" : gain,
        "Input Slit Width (μm)" : slit,
        "Bin size (nm)"         : binsize,
        "Power (mW)"            : power
    }

    meta_df = pd.DataFrame(list(meta_table.items()), columns=[" ", "Value"])

    # Iterations
    it_matrix    = SigTools.gen_iteration_matrix(dirpath)
    combined_arr = SigTools.combine_iterations(it_matrix, method='mean-weighted')

    factor   = t_exp * gain
    #norm_arr = SigTools.scale_array(combined_arr, factor)


    mean_lim = 1.2*np.nanmean(it_matrix)
    amp_lim  = 1.2*np.max(combined_arr)

    if amp_lim > mean_lim:
        y_high = amp_lim
    else:
        y_high = mean_lim

    y_low  = 250

    # ========================= #
    # === Setup Page Figure === #
    # ========================= #
    fig = plt.figure(figsize=A4)

    fig.suptitle(exposure, fontsize = 20)

    coll  = collpath.name
    phase = collpath.parent.name

    subtitle = f"{phase} {coll}"
    fig.text(0.5, 0.93, subtitle, ha="center", va="center", fontsize=14)

    gs = fig.add_gridspec(
        3, 1,
        left=0.12, right=0.88,
        top=0.95, bottom=0.12,
        wspace=0.25, hspace=0.2
    )

    ax1 = fig.add_subplot(gs[0])
    ax2 = fig.add_subplot(gs[1])
    ax3 = fig.add_subplot(gs[2])

    for ax in fig.get_axes():
        ax.grid(True, alpha=0.4)

    # ============ #
    # === Plot === #
    # ============ #

    # Plot metadata table
    ax1.axis("off")
    tbl = ax1.table(
        cellText=meta_df.values,
        colLabels=meta_df.columns,
        loc="center",
        cellLoc="left",
    )
    tbl.auto_set_font_size(False)
    tbl.set_fontsize(10)
    tbl.scale(1, 1.3)

    # Plot all iterations
    for it in it_matrix:
        ax2.plot(x, it, linewidth = 0.5)

    ax2.set_ylabel("Photo-Electron Count")

    # Right axis = scaled by (t_exp * gain):  y_scaled = y_raw / factor

    secax = ax2.secondary_yaxis(
        'right',
        functions=(lambda y: y / factor,  # left -> right
                lambda y: y * factor)  # right -> left
    )
    secax.set_ylabel(rf"Photons sec$^{-1}$")

    ax2.set_ylim(y_low, y_high)
    ax2.set_title(f'Iterations: {len(it_matrix)}')


    # Plot combined
    ax3.plot(x, combined_arr)

    ax3.set_ylabel("Photo-Electron Count")

    # Right axis = scaled by (t_exp * gain):  y_scaled = y_raw / factor

    secax = ax3.secondary_yaxis(
        'right',
        functions=(lambda y: y / factor,  # left -> right
                lambda y: y * factor)  # right -> left
    )
    secax.set_ylabel(rf"Photons sec$^{-1}$")

    ax3.set_ylim(y_low, y_high)
    ax3.set_title(f'Combined spectrum')

    pdf.savefig(fig)
    plt.close(fig)

    return x, combined_arr, t_exp, gain, binsize, date_n_time


def page_diff(pdf, dict_arr, key):

    # ==================== #
    # === Extract Data === #
    # ==================== #
    y_matrix = []
    d_arr    = []
    for dict_elm in dict_arr:

        x = dict_elm[key]['x']
        y = dict_elm[key]['y']
        t = dict_elm[key]['t']
        g = dict_elm[key]['g']
        b = dict_elm[key]['b']
        d = dict_elm[key]['d']
        p = dict_elm[key]['p']

        #x, y, t, g, b, d, p = dict_elm[key]
        #print(t, g, p)
        y_matrix.append(y)
        d_arr.append(d)

    meta_table = {
        "Exposure Time (sec)" : t,
        "Pre-Amplifier Gain"  : g,
        "Bin size (nm)"       : b,
        "Power (mW)"          : p
    }
    meta_df = pd.DataFrame(list(meta_table.items()), columns=[" ", "Value"])

    factor = t*g*p

    y_diffs = np.diff(y_matrix, axis=0)

    mean_lim = 1.5*np.nanmean(y_matrix)
    amp_lim  = 1.2*np.max(y_matrix)

    if amp_lim > mean_lim:
        y_high = amp_lim
    else:
        y_high = mean_lim

    y_low  = 250

    mean_lim = 1.2*np.nanmean(y_diffs[-1])
    amp_lim  = 1.2*np.max(y_diffs[-1])

    if amp_lim > mean_lim:
        y_high_diff = amp_lim
    else:
        y_high_diff = mean_lim

    y_low_diff = -40


    # ========================= #
    # === Setup Page Figure === #
    # ========================= #
    fig = plt.figure(figsize=A4)

    fig.suptitle(key, fontsize = 20)

    gs = fig.add_gridspec(
        3, 1,
        left=0.12, right=0.88,
        top=0.95, bottom=0.12,
        wspace=0.25, hspace=0.2
    )

    ax1 = fig.add_subplot(gs[0])
    ax2 = fig.add_subplot(gs[1])
    ax3 = fig.add_subplot(gs[2])

    for ax in fig.get_axes():
        ax.grid(True, alpha=0.4)

    # ============ #
    # === Plot === #
    # ============ #

    # Plot metadata table
    ax1.axis("off")
    tbl = ax1.table(
        cellText=meta_df.values,
        colLabels=meta_df.columns,
        loc="center",
        cellLoc="left",
    )
    tbl.auto_set_font_size(False)
    tbl.set_fontsize(10)
    tbl.scale(1, 1.3)

    # Plot all iterations
    for idx, y in enumerate(y_matrix):
        ax2.plot(x, y, linewidth = 1, label = f'Coll{idx+1}')

    ax2.set_ylabel("Photo-Electron Count")

    # Right axis = scaled by (t_exp * gain):  y_scaled = y_raw / factor

    secax = ax2.secondary_yaxis(
        'right',
        functions=(lambda y: y / factor,  # left -> right
                lambda y: y * factor)  # right -> left
    )
    secax.set_ylabel(rf"Photons sec$^{-1}$ mW$^{-1}$")

    ax2.set_ylim(y_low, y_high)
    ax2.set_title(f'Cumulative Spectra')

    ax2.legend()


    # Plot all iterations
    for idx, y_diff in enumerate(y_diffs):
        ax3.plot(x, y_diff, linewidth = 1, label = f'Coll{idx+1}')

    ax3.set_ylabel("Photo-Electron Count")

    # Right axis = scaled by (t_exp * gain):  y_scaled = y_raw / factor

    secax = ax3.secondary_yaxis(
        'right',
        functions=(lambda y: y / factor,  # left -> right
                lambda y: y * factor)  # right -> left
    )
    secax.set_ylabel(rf"Photons sec$^{-1}$ mW$^{-1}$")

    ax3.set_ylim(y_low_diff, y_high_diff)
    ax3.set_title(f'Cumulative Spectra')

    ax3.legend()

    pdf.savefig(fig)
    plt.close(fig)

    return None


# ----------------------------------------------------------------------------
# ------- BUILD PDF ----------------------------------------------------------
# ----------------------------------------------------------------------------

colls = []
for coll in sorted(phasepath.glob("Coll*")):
    colls.append(coll.name)

keys = ['BL', 'AM', 'H2O']
collpath0 = phasepath / 'Coll1'
for lif_dir in sorted(collpath0.glob("LiF*")):
    keys.append(lif_dir.name)

with PdfPages(pdf_path) as pdf:

    dict_arr = []
    for coll in colls:

        collpath = phasepath / coll

        power_path = collpath / 'power.txt'
        with open(power_path) as f:
            lines = f.readlines()

            power = float(lines[1].split(',')[1].strip())

        page_title(pdf, collpath, desc = f'Exposures: {keys}')
        page_acquisition(pdf, collpath)

        arr_dict = {}
        for key in keys:
            x, y, t, g, b, d = page_exposure(pdf, collpath, key)
            arr_dict[key] = {
                'x' : x,
                'y' : y,
                't' : t,
                'g' : g,
                'b' : b,
                'd' : d,
                'p' : power
            }

        dict_arr.append(arr_dict)

    page_title(pdf, phasepath, desc = f'Exposures: {keys}')

    for key in keys[3:]:
        page_diff(pdf, dict_arr, key)

