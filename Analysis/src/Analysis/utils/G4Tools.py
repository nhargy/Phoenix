import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection

from   Analysis.models.PDGParticle import PDGParticle_dict
from   Analysis.models.CubeCopy    import CopyCube_dict


def gen_run_dataframe(filepath : str, column_names : list, skiprows : int = 20) -> pd.DataFrame:
    """
    Generate a pandas DataFrame from a CSV file.

    Parameters:
    filepath (str): Path to the CSV file.
    column_names (list): List of column names for the DataFrame.
    skiprows (int): Number of rows to skip at the start of the file.

    Returns:
    pd.DataFrame: The generated DataFrame.
    """
    return pd.read_csv(filepath, skiprows=skiprows, sep=',', names=column_names)


def get_unique_event_numbers(df : pd.DataFrame) -> np.ndarray:
    """
    Get an array of unique event numbers from the DataFrame.

    Parameters:
    df (pd.DataFrame): The input DataFrame.

    Returns:
    np.ndarray: Array of unique event numbers.
    """
    return np.unique(np.array(df["fEvent"]))


def draw_cube(ax, centre=(0.,0.,0.), size = 10, color='gold', alpha=0.05, edgecolor='white'):

    cx, cy, cz = centre
    l = size/2
    
    vrt = [
        [cx+l, cy-l, cz-l],
        [cx+l, cy+l, cz-l],
        [cx+l, cy-l, cz+l],
        [cx+l, cy+l, cz+l],
        [cx-l, cy-l, cz-l],
        [cx-l, cy+l, cz-l],
        [cx-l, cy-l, cz+l],
        [cx-l, cy+l, cz+l]
    ]

    faces = [
        [vrt[0], vrt[1], vrt[3], vrt[2]],
        [vrt[1], vrt[5], vrt[7], vrt[3]],
        [vrt[5], vrt[4], vrt[6], vrt[7]],
        [vrt[0], vrt[4], vrt[6], vrt[2]],
        [vrt[2], vrt[3], vrt[7], vrt[6]],
        [vrt[0], vrt[1], vrt[5], vrt[4]]
    ]

    # Add cube as translucent surface
    cube = Poly3DCollection(faces, facecolors=color, edgecolors=edgecolor, alpha=alpha)
    ax.add_collection3d(cube)

    return cube


def draw_track(ax, track_df, arrow=True):

    intr      = str(track_df["fPostProc"])

    particle_serial  = int(track_df["fPDG"])
    particle = PDGParticle_dict[particle_serial]

    x1 = float(track_df["fX1"])
    y1 = float(track_df["fY1"])
    z1 = float(track_df["fZ1"])

    x2 = float(track_df["fX2"])
    y2 = float(track_df["fY2"])
    z2 = float(track_df["fZ2"])

    # Draw 3D line
    ax.plot([x1, x2], [y1, y2], [z1, z2], color=particle.colour, linewidth=particle.linewidth, linestyle=particle.linestyle, label = particle.label)

    # Direction vector (from start → end)
    u = x2 - x1
    v = y2 - y1
    w = z2 - z1

    if arrow==True:
        # Draw an arrow at the starting point pointing along the track
        ax.quiver(x1, y1, z1, u, v, w,
                  length=1.0,  # scales arrow size
                  normalize=True,
                  color=particle.colour,
                  arrow_length_ratio=0.4)


    if intr != "Transportation":
        if intr == "hadElastic":
            ax.scatter(x2, y2, z2, color="lawngreen", s=80, marker='o', label = intr, alpha = 0.7)
        elif intr == "neutronInelastic":
            ax.scatter(x2, y2, z2, color="fuchsia", s=80, marker='d', label = intr, alpha = 0.9)
        elif intr == "ionIoni":
            ax.scatter(x2, y2, z2, color="gold", s=90, marker='*', label = intr, alpha = 0.8)
    

def visualise_event(event_df, draw_cube_flag=True):

    # Event number
    event_num = event_df.iloc[0]["fEvent"]

    # Get copy number
    copy_num   = event_df.iloc[0]["Copy"]
    Cube       = CopyCube_dict[copy_num]
    centre     = Cube.centre_coord

    cx, cy, cz = centre
    buff       = 6
    
    # Build 3D plot
    fig = plt.figure(figsize=(7,7))
    ax = fig.add_subplot(111, projection='3d')
    #ax.set_facecolor('#f0f0f0')
    ax.set_facecolor('black')

    if draw_cube_flag==True:
        draw_cube(ax, centre = centre, color = Cube.colour, alpha=Cube.alpha)

    ax.set_box_aspect([1, 1, 1])
    
    ax.set_xlim(cx-buff, cx+buff)
    ax.set_ylim(cy-buff, cy+buff)
    ax.set_zlim(cz-buff, cz+buff)

    # Size
    track_num = len(event_df)

    for i in range(track_num):
        track_df = event_df.iloc[i]
        if i==0:
            x2 = float(track_df["fX2"])
            y2 = float(track_df["fY2"])
            z2 = float(track_df["fZ2"])
        draw_track(ax, track_df)

    ax.view_init(elev=18, azim=35)

    ax.set_title(f"Cube Copy: {copy_num},  Event Number: {event_num}", fontsize = 20)

    plt.rcParams.update({
        "text.color": "white",
        "legend.fontsize": 12,    # legend text size
        "legend.title_fontsize": 12
    })

    # AFTER all draw_track() calls, before plt.show()
    handles, labels = ax.get_legend_handles_labels()
    uniq = {}
    for h, l in zip(handles, labels):
        if l and l not in uniq:        # keep first occurrence of each non-empty label
            uniq[l] = h
    ax.legend(uniq.values(), uniq.keys(), frameon=False)



    # Hide all axes, grids, and panes
    ax.grid(False)  # no gridlines
    ax.set_axis_off()  # hides all panes, ticks, and spines
    ax.xaxis.pane.fill = False
    ax.yaxis.pane.fill = False
    ax.zaxis.pane.fill = False
    plt.tight_layout()
    plt.show()


def get_particle_edep(df, particle):
    Edep_arr = np.array(df[df["fPDG"]==particle]["fEdep"])
    Edep_tot = np.sum(Edep_arr)
    return Edep_tot
