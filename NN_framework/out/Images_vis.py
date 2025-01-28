# -*- coding: utf-8 -*-
"""
Created on Mon Jan 27 11:17:09 2025

@author: Arthur
"""
import numpy as np
import plotly.graph_objects as go
import plotly.express as px

data = np.genfromtxt('X_mnist.txt', delimiter=";")

for i in range(data.shape[1]):
    im = data[:,i]
    im = im.reshape([28,28])
    
    fig = px.imshow(im)
    fig.write_image(f'mnist_images/image_{i}.png')