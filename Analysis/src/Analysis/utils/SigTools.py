import numpy as np
from numpy import ndarray
import os
from pathlib import Path
from datetime import datetime
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
from scipy.optimize import curve_fit


# ---- Elementary Functions --------------------------------------------------

def read_spectrum(filepath : str, 
                  cut      : int = 31
                  ) -> tuple[ndarray, ndarray]:
    """
    Read the x (wavelength) and y (PE count) data from data textfile.
    """
    with open(filepath) as f:
        lines = f.readlines()[cut:]

    x = []
    y = []

    for line in lines:
        parts = line.strip().split()
        x_val = float(parts[0])
        y_val = float(parts[-1])

        x.append(x_val)
        y.append(y_val)

    return np.array(x), np.array(y)


def read_acquisition_data(filepath : str, 
                          cut      : int = 31
                          ) -> dict: 
    """

    """
    with open(filepath) as f:
        lines = f.readlines()[:cut]

        acq_dict = {}
        for line in lines:
            if ":" in line:
                key, value = line.split(":", 1)
                acq_dict[key.strip()] = value.strip()

    return acq_dict


def gen_iteration_matrix(dirpath : str
                         ) -> ndarray:
    """
    Places all the spectra from a collection into 1 matrix, where each
    row represents the PE counts from the exposure.
    """
    iteration_matrix = []
    i = 0
    while True:
        try:
            filepath = os.path.join(dirpath, f'it_{i}.txt')
            _, y     = read_spectrum(filepath)
            iteration_matrix.append(y)
            i+=1
        except:
            break

    return np.array(iteration_matrix)


def combine_iterations(iteration_matrix : ndarray, 
                       method           : str ='mean-weighted'
                       ) -> ndarray:
    """
    Combine the spectra from all the iterations from a single sample
    of a single collection, via simple averaging or weighted averaging.
    """

    if method == 'mean-weighted':

        # Set up empty array to collect values
        combined_array = []

        # Array of means on each column (wavelength bin)
        means_arr = np.mean(iteration_matrix, axis=0)
        std_arr   = np.std(iteration_matrix, axis=0)

        # Loop through the values
        cols = iteration_matrix.T
        for idx, col in enumerate(cols):
            w_sum = 0
            w_mean = 0
            for elm in col:
                d = abs(elm - means_arr[idx])
                w = np.exp(-d/std_arr[idx])
                w_mean+=elm * w
                w_sum+=w
            
            try:
                combined_array.append(w_mean/w_sum)
            except:
                combined_array.append(means_arr[idx])

    elif method == 'simple-average':
        combined_array = np.mean(iteration_matrix, axis=0)

    return np.array(combined_array)


def scale_array(array  : ndarray, 
                factor : float
                ) -> ndarray:
    """
    Divide array by some factor.
    """
    scaled_array = np.divide(array, factor)
    return scaled_array


def remove_baseline(array   : ndarray,
                    baseline: float
                    ) -> ndarray:
    """
    
    """
    sub_array = np.subtract(array, baseline)
    return sub_array

# ---- Temperature and Timeline ----------------------------------------------

def get_coll_temp_timeline(collpath : str
                           ) -> tuple[ndarray, ndarray]:

    dirnames = []
    parent = Path(collpath)
    for d in parent.iterdir():
        if d.is_dir() and any(d.iterdir()):   # at least one item inside
            dirnames.append(d.name)

    cuts = []
    timestamps = []
    temps = []
    for dir in dirnames:
        i = 0
        while True:
            try:
                filepath = os.path.join(collpath, dir, f'it_{i}.txt')
                acq_dict = read_acquisition_data(filepath)
                timestamp = acq_dict['Date and Time']
                try:
                    temp = float(acq_dict['Temperature (C)'])
                except:
                    temp = float(acq_dict['Unstabilized Temperature (C)'])     

                timestamps.append(timestamp)
                temps.append(temp)

                i+=1
            except FileNotFoundError:
                cuts.append(timestamp)
                break

    cut_times = [datetime.strptime(t, "%a %b %d %H:%M:%S.%f %Y") for t in cuts]         
    times = [datetime.strptime(t, "%a %b %d %H:%M:%S.%f %Y") for t in timestamps]

    return (np.array(times), np.array(temps), np.array(cut_times))


def plot_temp_timeline(times : ndarray, 
                       temps : ndarray, 
                       cuts  : ndarray=None, 
                       ax=None):
    """
    Plot temperature vs time. 
    Returns fig, ax so the caller controls showing/saving.
    """
    # Create figure/axis if not provided
    created_fig = False
    if ax is None:
        fig, ax = plt.subplots(figsize=(10,2))
        created_fig = True
    else:
        fig = ax.figure

    # Plot data
    ax.scatter(times, temps, s=8)

    if cuts is not None:
        for cut in cuts:
            ax.axvline(cut, color='grey', alpha=0.5)

    # Formatting
    ax.set_ylabel("EMCCD Temperature (°C)")
    ax.set_xlabel("Time")

    # Format x-axis: only hours:minutes
    ax.xaxis.set_major_formatter(mdates.DateFormatter('%H:%M'))
    ax.xaxis.set_major_locator(mdates.AutoDateLocator())
        
    if len(times) > 0:
        date_title = times[0].strftime("%a %b %d %Y")
        ax.set_title(f"Acquisition on {date_title}")

    ax.grid(alpha=0.3)

    # Return
    return fig, ax


# ---- Raman ----------------------------------------------------------------

def get_raman_gaussian(x : ndarray,
                       y : ndarray,
                       ) -> ndarray:
    """
    
    """
    def gaussian(x, a, s, m):
        return a * np.exp(-((x-m)**2)/(2*(s**2)))

    def double_gaussian(x, a1, s1, m1, a2, s2, m2):
        return gaussian(x, a1, s1, m1) + gaussian(x, a2, s2, m2)
    
    l_cut = 40
    u_cut = 350
    x_fit = x[l_cut:u_cut]
    y_fit = y[l_cut:u_cut]

    a1 = np.max(y)
    s1 = 25
    m1 = 510

    a2 = a1/60
    s2 = 2*s1
    m2 = 530

    p0 = (a1, s1, m1, a2, s2, m2)
    popt, _ = curve_fit(double_gaussian, x_fit, y_fit, p0=p0)
        
    return popt[:3]

    