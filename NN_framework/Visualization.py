import numpy as np
import pandas as pd
import plotly.graph_objects as go

file = "error.txt"

data = np.genfromtxt(file, delimiter=";", usemask=True)

fig = go.Figure(go.Scatter(y=data))
fig.write_html('Error.html')

