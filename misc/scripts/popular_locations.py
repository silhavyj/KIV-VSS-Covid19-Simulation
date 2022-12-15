import json
from os import walk
import matplotlib.pyplot as plt
import numpy as np

data = {}

def load_data(folder):
    path = f'../measured_data/popular_locations/{folder}/'
    filenames = next(walk(path), (None, None, []))[2]

    for filename in filenames:
        with open(f'../measured_data/popular_locations/{folder}/' + filename, "r") as file:
            local_data = json.load(file)
            if folder not in data:
                data[folder] = {}
            for record in local_data:
                if record['time'] not in data[folder]:
                    data[folder][record['time']] = 0.0
                data[folder][record['time']] += ((record['infected'] / 500.0) / len(filenames)) * 100.0
         
         
def plot_data(data):
    labels = {
        '1250': '500',
        '1500': '1000',
        '1750': '1500',
        '2000': '2000'    
    }
    for folder in ['1250', '1500', '1750', '2000']:
        lists = sorted(data[folder].items())
        x, y = zip(*lists)
        
        plt.xlabel("time [days]")
        plt.ylabel("Infected people [%]")
        plt.plot(x, y, linestyle = 'solid', marker = 'o', markersize = 4, label=labels[folder] + "m")

    plt.grid(axis='y')
    plt.grid(axis='x')
    plt.title('Evolvement of the number of infected people with different distances between 4 popular locations')
    plt.legend()
    plt.show()
    

load_data('1250')
load_data('1500')
load_data('1750')
load_data('2000')

plot_data(data)