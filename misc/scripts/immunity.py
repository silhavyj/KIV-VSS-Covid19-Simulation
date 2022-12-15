import json
from os import walk
import matplotlib.pyplot as plt
import numpy as np

data = {}

def load_data(folder):
    path = f'../measured_data/immunity/{folder}/'
    filenames = next(walk(path), (None, None, []))[2]
    count = 0

    for filename in filenames:
        with open(f'../measured_data/immunity/{folder}/' + filename, "r") as file:
            local_data = json.load(file)
            if folder not in data:
                data[folder] = {}
            for record in local_data:
                if record['time'] not in data[folder]:
                    data[folder][record['time']] = 0.0
                data[folder][record['time']] += ((record['infected'] / 500.0) / len(filenames)) * 100.0
         
         
def plot_data(data):
    for folder in ['01', '14', '28', '42']:
        lists = sorted(data[folder].items())
        x, y = zip(*lists)
        
        plt.xlabel("time [days]")
        plt.ylabel("Infected people [%]")
        plt.plot(x, y, linestyle = 'solid', label=folder + " days")

    plt.grid(axis='y')
    plt.grid(axis='x')
    plt.title('Comparision of different immunity periods')
    plt.legend()
    plt.show()
    

load_data('01')
load_data('14')
load_data('28')
load_data('42')

plot_data(data)