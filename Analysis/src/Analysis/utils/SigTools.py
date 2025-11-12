import numpy as np
from numpy import ndarray
import os
from pathlib import Path
from datetime import datetime
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
from scipy.optimize import curve_fit
import pandas as pd


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

def gaussian(x, a, s, m):
    return a * np.exp(-((x-m)**2)/(2*(s**2)))

def double_gaussian(x, a1, s1, m1, a2, s2, m2):
    return gaussian(x, a1, s1, m1) + gaussian(x, a2, s2, m2)

def get_raman_gaussian(x : ndarray,
                       y : ndarray,
                       ) -> ndarray:
    """
    
    """
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


"""
From now, functions beginning with gen (generate) take in a path to 
a folder or file as an argument, whil proc (process) take in the generated
dataframes as arguments.
"""

# ---- Generate DFs ----------------------------------------------------------

def get_colls_and_samples(phasepath, toignore = ['power.txt', 'SP_B2_35']):
    colls = []
    for collpath in sorted(phasepath.glob('Coll*')):
        coll = str(collpath).split('/')[-1]
        colls.append(coll)

    samples = []
    for samplepath in sorted(collpath.iterdir()):
        sample = str(samplepath).split('/')[-1]
        samples.append(sample)

    for ig in toignore:
        samples.remove(ig) 

    return colls, samples

def gen_df_meta(phasepath: Path) -> pd.DataFrame:
    # You mentioned this exists already
    colls, samples = get_colls_and_samples(phasepath)

    index_tuples = []

    # Metadata containers (camera/acquisition, no SR193i block)
    dt_vals        = []  # Date and Time
    swver_vals     = []  # Software Version
    temp_vals      = []  # Temperature (or Unstabilized)
    model_vals     = []  # Model
    dtype_vals     = []  # Data Type
    acqmode_vals   = []  # Acquisition Mode
    trigmode_vals  = []  # Trigger Mode
    texp_vals      = []  # Exposure Time (secs)
    readout_vals   = []  # Readout Mode
    hbin_vals      = []  # Horizontal binning
    edr_vals       = []  # Extended Dynamic Range
    hflip_vals     = []  # Horizontally flipped
    vshift_vals    = []  # Vertical Shift Speed (usecs)
    pxrate_vals    = []  # Pixel Readout Rate (MHz)
    bclamp_vals    = []  # Baseline Clamp
    clkamp_vals    = []  # Clock Amplitude
    outamp_vals    = []  # Output Amplifier
    cam_sn_vals    = []  # Serial Number (camera)
    gain_vals      = []  # Pre-Amplifier Gain
    snfilter_vals  = []  # Spurious Noise Filter Mode
    photonpc_vals  = []  # Photon counted
    avgfilter_vals = []  # Data Averaging Filter Mode

    pow_vals       = []  # Laser power (from power.txt)

    for sample in samples:
        for coll in colls:
            dirpath = phasepath / coll / sample

            # Read power for this collection
            power_path = phasepath / coll / 'power.txt'
            with open(power_path) as f:
                lines = f.readlines()
                power = float(lines[1].split(',')[1].strip())

            # For each iteration file inside sample/coll
            for it, itpath in enumerate(sorted(dirpath.iterdir(),
                                               key=lambda p: int(p.stem.split('_')[1]))):

                acq_data = read_acquisition_data(itpath)

                # Index = one row per spectrum (no wavelength here)
                index_tuples.append((sample, coll, it))

                # Core metadata (no SR193i)
                dt_vals.append(acq_data.get('Date and Time'))

                swver_vals.append(acq_data.get('Software Version'))

                # Temperature: stabilized or unstabilized
                if 'Temperature (C)' in acq_data:
                    temp_vals.append(acq_data['Temperature (C)'])
                elif 'Unstabilized Temperature (C)' in acq_data:
                    temp_vals.append(acq_data['Unstabilized Temperature (C)'])
                else:
                    temp_vals.append(np.nan)

                model_vals.append(acq_data.get('Model'))
                dtype_vals.append(acq_data.get('Data Type'))
                acqmode_vals.append(acq_data.get('Acquisition Mode'))
                trigmode_vals.append(acq_data.get('Trigger Mode'))
                texp_vals.append(float(acq_data.get('Exposure Time (secs)')))
                readout_vals.append(acq_data.get('Readout Mode'))
                hbin_vals.append(acq_data.get('Horizontal binning'))
                edr_vals.append(acq_data.get('Extended Dynamic Range'))
                hflip_vals.append(acq_data.get('Horizontally flipped'))
                vshift_vals.append(acq_data.get('Vertical Shift Speed (usecs)'))
                pxrate_vals.append(acq_data.get('Pixel Readout Rate (MHz)'))
                bclamp_vals.append(acq_data.get('Baseline Clamp'))
                clkamp_vals.append(acq_data.get('Clock Amplitude'))
                outamp_vals.append(acq_data.get('Output Amplifier'))
                cam_sn_vals.append(acq_data.get('Serial Number'))

                gain_val = int(acq_data['Pre-Amplifier Gain'][0])
                gain_vals.append(gain_val)

                snfilter_vals.append(acq_data.get('Spurious Noise Filter Mode'))
                photonpc_vals.append(acq_data.get('Photon counted'))
                avgfilter_vals.append(acq_data.get('Data Averaging Filter Mode'))

                pow_vals.append(float(power))


    # Build MultiIndex
    index = pd.MultiIndex.from_tuples(
        index_tuples,
        names=['Sample', 'Collection', 'Iteration']
    )

    # Build DataFrame (one row per spectrum)
    df_meta = pd.DataFrame(
        {
            'DateTime':                 dt_vals,
            'SoftwareVersion':          swver_vals,
            'Temperature':            temp_vals,
            'Model':                    model_vals,
            'DataType':                 dtype_vals,
            'AcquisitionMode':          acqmode_vals,
            'TriggerMode':              trigmode_vals,
            'Exposure_s':               texp_vals,
            'ReadoutMode':              readout_vals,
            'HorizontalBinning':        hbin_vals,
            'ExtendedDynamicRange':     edr_vals,
            'HorizontallyFlipped':      hflip_vals,
            'VertShiftSpeed_us':        vshift_vals,
            'PixelReadoutRate_MHz':     pxrate_vals,
            'BaselineClamp':            bclamp_vals,
            'ClockAmplitude':           clkamp_vals,
            'OutputAmplifier':          outamp_vals,
            'CameraSerialNumber':       cam_sn_vals,
            'PreAmpGain':               gain_vals,
            'SpuriousNoiseFilterMode':  snfilter_vals,
            'PhotonCounted':            photonpc_vals,
            'DataAveragingFilterMode':  avgfilter_vals,
            'Power_mW':                 pow_vals,
        },
        index=index
    )

    return df_meta


def gen_df_raw(phasepath, toignore = ['power.txt', 'SP_B2_35']):

    colls, samples = get_colls_and_samples(phasepath)

    index_tuples  = []
    y_vals        = []

    for sample in samples:
        for coll in colls:
            dirpath =  phasepath / coll / sample

            for it, itpath in enumerate(sorted(dirpath.iterdir(), key=lambda p: int(p.stem.split('_')[1]))):
                wls, y_spectrum = read_spectrum(itpath)

                for wl_idx, wl_val in enumerate(wls):
                    index_tuples.append((sample, coll, it, wl_val))
                    y_vals.append(y_spectrum[wl_idx])

    index = pd.MultiIndex.from_tuples(
    index_tuples,
    names=['Sample', 'Collection', 'Iteration', 'Wavelength']
    )

    # Build DataFrame with x, y columns
    df = pd.DataFrame({'Spectrum': y_vals}, index=index)

    return df

def gen_df_comb(phasepath, toignore = ['power.txt', 'SP_B2_35']):

    colls, samples = get_colls_and_samples(phasepath)

    index_tuples  = []
    y_vals        = []

    for sample in samples:
        for coll in colls:
            dirpath =  phasepath / coll / sample

            it_matrix = []
            for it, itpath in enumerate(sorted(dirpath.iterdir(), key=lambda p: int(p.stem.split('_')[1]))):
                wls, y_spectrum = read_spectrum(itpath)
                it_matrix.append(y_spectrum)

            y_combined = combine_iterations(np.array(it_matrix))
            for wl_idx, wl_val in enumerate(wls):
                index_tuples.append((sample, coll, wl_val))
                y_vals.append(y_combined[wl_idx])

    index = pd.MultiIndex.from_tuples(
    index_tuples,
    names=['Sample', 'Collection', 'Wavelength']
    )

    # Build DataFrame with x, y columns
    df = pd.DataFrame({'Spectrum': y_vals}, index=index)
    
    return df

# ---- Higher level DF operations --------------------------------------------

def get_wavelengths(df):
    wls   = df.index.get_level_values('Wavelength').unique().to_numpy()
    return wls


def proc_raman_df(df_comb):

    df_h2o = df_comb.loc['H2O']
    df_bl  = df_comb.loc['BL']

    colls = df_comb.index.get_level_values('Collection').unique()
    wls   = df_comb.index.get_level_values('Wavelength').unique().to_numpy()

    index_tuples  = []
    popts         = []
    signals       = []

    for coll in colls:
        y = df_h2o.loc[coll]['Spectrum'].to_numpy()

        # Get mean baseline of collection to remove for fitting
        y_bl = df_bl.loc[coll]['Spectrum'].to_numpy()
        bl_mean = np.mean(y_bl)
        y = np.subtract(y, bl_mean)

        popt = get_raman_gaussian(wls, y)
        index_tuples.append((coll,))
        popts.append(popt)

        a, s, m = popt
        signal = a * s * np.sqrt(2 * np.pi)

        #signal = np.trapezoid(gaussian(wls, *popt) , x=wls)
        signals.append(signal)

    index = pd.MultiIndex.from_tuples(
    index_tuples,
    names=['Collection']
    )

    df = pd.DataFrame({'signal' : signals, 'popt': popts}, index=index)

    return df


def get_norm_factor(meta_df,
                    idx,
                    normby = ['Exposure_s', 'PreAmpGain', 'Power_mW']):

    factor = 1
    for key in normby:
        val = meta_df.loc[idx][key]
        factor = factor*val

    return factor


def proc_diff_df(comb_df):

    

    return df
