import numpy as np
import pandas as pd
import plotly.graph_objects as go


directory = "out/"
file = "MNIST_loss"
# file = "XOR_loss"

data = np.genfromtxt(directory+f'{file}.txt', delimiter=";", usemask=True)

fig = go.Figure(go.Scatter(y=data))
fig.write_html(directory+f'{file}.html')

