import numpy as np
from datetime import date

def days_elapsed(production_date, measurement_date=None):

    given_date = date(*production_date)   # year, month, day
    today = date.today() if measurement_date is None else date(*measurement_date)

    days_since = (today - given_date).days
    return days_since


def get_activity(I0, half_life, days_elapsed):
    """
    Calculate the remaining activity of a radioactive substance.

    Parameters:
    I0 (float): Initial activity.
    half_life (float): Half-life of the substance in days.
    days_elapsed (int): Number of days elapsed.

    Returns:
    float: Remaining activity after the elapsed days.
    """
    decay_constant = np.log(2) / half_life
    remaining_activity = I0 * np.exp(-decay_constant * days_elapsed)
    return remaining_activity