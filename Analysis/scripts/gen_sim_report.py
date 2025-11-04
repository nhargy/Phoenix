import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import json
from matplotlib.backends.backend_pdf import PdfPages
from datetime import datetime

import Analysis.config.config          as cfg
from   Analysis.utils              import G4Tools, XTools
from   Analysis.models.PDGParticle import PDGParticle_dict

# Columns names from expeceted csv file
column_names = ["fEvent", "fEntry", "fPreProc", "fPostProc", "fTrackID", 
                "fParentID", "fPDG", "fKinetic", "fEdep", 
                "fX1", "fY1", "fZ1", "fX2", "fY2", "fZ2", "Copy"]

# Open json config file for this script and extract values
try:
    with open('gen_sim_report.json', 'r') as file:
        content = json.load(file)
        data_dir_filepath   = content["data_dir_filepath"]
        report_dir_savepath = content["report_dir_savepath"]

except FileNotFoundError:
    print("Error: 'gen_sim_report.json' not found. Please create the file.")
except json.JSONDecodeError:
    print("Error: Invalid JSON format in 'gen_sim_report.json'.")

run_data_filepath     = f"{data_dir_filepath}/output0_nt_Hits.csv"
gen_neutrons_filepath = f"{data_dir_filepath}/generated_neutrons.csv"
gen_gammas_filepath   = f"{data_dir_filepath}/generated_gammas.csv"
geo_params_filepath   = f"{data_dir_filepath}/geo_params.csv"

# Create PDF
pdf_path = f"{report_dir_savepath}/report1.pdf"
pdf = PdfPages(pdf_path)

# ---- Page 1: Title page ----
fig = plt.figure(figsize=(8.27, 11.69))  # A4 portrait in inches (approx)
plt.axis("off")

title = "Simulation Report"
subtitle = data_dir_filepath
ts = datetime.now().strftime("%Y-%m-%d %H:%M")

fig.text(0.5, 0.85, title, ha="center", va="center", fontsize=26, weight="bold")
fig.text(0.5, 0.80, subtitle, ha="center", va="center", fontsize=14)
fig.text(0.5, 0.75, f"Created: {ts}", ha="center", va="center", fontsize=10)

# Short description block
intro_text = (
    "Sim report."
)
fig.text(0.1, 0.6, intro_text, ha="left", va="top", fontsize=11, wrap=True)
pdf.savefig(fig)
plt.close(fig)

# Generate the run DataFrame
df = G4Tools.gen_run_dataframe(run_data_filepath, column_names=column_names)
df_n = pd.read_csv(gen_neutrons_filepath)
df_g = pd.read_csv(gen_gammas_filepath)
df_p = pd.read_csv(geo_params_filepath)

fig,(ax1, ax2, ax3, ax4) = plt.subplots(ncols = 1, nrows=4, figsize=(8,8))

fig.suptitle("Primaries Characteristion", fontsize = 20)

energies_neutrons = np.array(df_n["E_MeV"])
energies_gammas   = np.array(df_g["E_MeV"])

dir_x_neutrons = np.array(df_n["dir_x"])
dir_y_neutrons = np.array(df_n["dir_y"])
dir_z_neutrons = np.array(df_n["dir_z"])

# Summary stats
stats = {
    "# Primaries": len(energies_neutrons),
    "# Gammas"   : len(energies_gammas),
    "Gamma percentage": np.round((len(energies_gammas)/len(energies_neutrons))*100,2)
}
stats_df = pd.DataFrame(list(stats.items()), columns=[" ", "Value"])

ax1.axis("off")
tbl = ax1.table(
    cellText=stats_df.values,
    colLabels=stats_df.columns,
    loc="center",
    cellLoc="left",
)
tbl.auto_set_font_size(False)
tbl.set_fontsize(10)
tbl.scale(1, 1.3)

bins = 72
ax2.hist(energies_neutrons, 
         color = 'green',
         edgecolor = 'grey',
         bins = bins, 
         density = False)
ax2.grid("on", alpha = 0.25)

ax2.set_xlabel("Energy [MeV]")
ax2.set_ylabel("Counts")
ax2.set_title("Primary Neutron Energy Spectrum")

ax3.hist(energies_gammas, 
         color = 'gold',
         edgecolor = 'grey',
         bins = bins, 
         density = False)
ax3.grid("on", alpha = 0.25)

ax3.set_xlabel("Energy [MeV]")
ax3.set_ylabel("Counts")
ax3.set_title("Primary Gamma Energy Spectrum")

ax4.hist(dir_x_neutrons, 
         edgecolor = 'grey',
         alpha = 0.5,
         bins = bins, 
         density = False,
         label = "x")

ax4.hist(dir_y_neutrons, 
         edgecolor = 'grey',
         alpha = 0.6,
         bins = bins, 
         density = False,
         label = "y")

ax4.hist(dir_z_neutrons, 
         edgecolor = 'grey',
         alpha = 0.7,
         bins = bins, 
         density = False,
         label = "z")
ax4.grid("on", alpha = 0.25)

ax4.set_xlabel("Vector Displacement [mm]")
ax4.set_ylabel("Counts")
ax4.set_title("Primary Direction Spectrum")

plt.tight_layout()
plt.legend()

pdf.savefig(fig)
plt.close(fig)


# - 2nd Page: geoParams
fig, ax = plt.subplots(ncols = 1, nrows=1, figsize=(8,8))

ax.axis("off")
tbl = ax.table(
    cellText=df_p,
    loc="center",
    cellLoc="left",
)
tbl.auto_set_font_size(False)
tbl.set_fontsize(10)
tbl.scale(1, 1.3)

plt.tight_layout()
plt.legend()

pdf.savefig(fig)
plt.close(fig)

pdf.close()