from pathlib import Path

def get_project_root() -> Path:
    return Path(__file__).resolve().parent.parent.parent.parent.parent

PROJECT_ROOT = get_project_root()
DATA_DIR     = PROJECT_ROOT / "Data"
DOC_DIR      = PROJECT_ROOT / "Docs"
G4Sims_DIR   = PROJECT_ROOT / "G4Sims"

# PATHS
COLD_DATA_DIR    = "home/hargy/Scientific/DataCold/"   # <-- Adjust for local
PHOENIX_DATA_DIR = COLD_DATA_DIR + "Experiment-Data/" + "Project-Phoenix/"

# PARAMETERS
CO_60_INIT_ACTIVITY   = 37000000 # Bq (1 mCi)
CO_60_PRODUCTION_DATE = (2016, 1, 26) # year, month, day
CO_60_HALF_LIFE       = 1925.28 # days

AMBE_INIT_ACTIVITY    = 81000 # Bq (2.2 uCi)
AMBE_PRODUCTION_DATE  = (2021, 2, 9) # year, month, day
AMBE_HALF_LIFE        = 157788 # days