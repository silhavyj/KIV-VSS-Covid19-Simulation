import json
from os import walk
import matplotlib.pyplot as plt
import numpy as np

data = {}


def load_data(folder):
    path = f'../measured_data/{folder}/'
    filenames = next(walk(path), (None, None, []))[2]

    for filename in filenames:
        with open(f'../measured_data/{folder}/' + filename, "r") as file:
            local_data = json.load(file)
            if folder not in data:
                data[folder] = {}
            for record in local_data:
                if record['time'] not in data[folder]:
                    data[folder][record['time']] = 0.0
                data[folder][record['time']] += (record['infected'] / 500.0) / len(filenames)
         
         
def plot_data(data):
    for folder in ['20', '40', '60', '80']:
        lists = sorted(data[folder].items())
        x, y = zip(*lists)
        
        plt.xlabel("time [days]")
        plt.ylabel("Infected people [%]")
        plt.plot(x, y, linestyle = 'solid', marker = 'o', markersize = 4, label=folder + "%")

    plt.grid(axis='y')
    plt.grid(axis='x')
    plt.legend()
    plt.show()
    

load_data('20')
load_data('40')
load_data('60')
load_data('80')

plot_data(data)