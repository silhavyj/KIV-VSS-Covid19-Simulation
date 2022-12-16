import json
from os import walk
import matplotlib.pyplot as plt
import numpy as np
import statistics

data = {}

def load_data(folder):
    path = f'../measured_data/self_isolation/{folder}/'
    filenames = next(walk(path), (None, None, []))[2]

    for filename in filenames:
        with open(f'../measured_data/self_isolation/{folder}/' + filename, "r") as file:
            local_data = json.load(file)
            if folder not in data:
                data[folder] = []
            data[folder].append((local_data[-1]['dead'] / 500.0) * 100.0)
    
    data[folder] = statistics.median(data[folder])
         
         
def plot_data(data):
    x = ['20', '40', '60', '80']
    y = []
    for folder in x:
        y.append(data[folder])

    fig, ax = plt.subplots()
        
    plt.ylabel("Total fatality rate [%]")
    plt.xlabel("People practising self-isolation [%]")
    plt.bar(x, y)

    ax.set_axisbelow(True)
    ax.grid(axis='y')
    plt.legend()
    plt.show()
    
plt.rcParams.update({'font.size': 16})

load_data('20')
load_data('40')
load_data('60')
load_data('80')

plot_data(data)