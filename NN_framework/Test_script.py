# -*- coding: utf-8 -*-
"""
Created on Mon Jan 13 16:18:12 2025

@author: Arthur
"""

import numpy as np


def write_mat(file, m):
    
    string = ''
    
    for i in range(m.shape[0]):
        for j in range(m.shape[1]):
            string += f'{m[i,j]:.9f}' + ';'
            
        string += '\n'
    string += '\n'
    
    
    with open(file, 'a') as f:
        f.write(string)
    
    return None


n = 100
size_max = 20

for i in range(n):
    
    shapes = np.random.randint(1, size_max,3)
    
    mat_1 = np.random.random([shapes[0], shapes[1]])
    mat_2 = np.random.random([shapes[1], shapes[2]])
    
    res = np.matmul(mat_1, mat_2)
    
    print(mat_1.shape, mat_2.shape, res.shape)
    
    write_mat('test_matmul.txt', mat_1)
    write_mat('test_matmul.txt', mat_2)
    write_mat('test_matmul.txt', res)
    
    