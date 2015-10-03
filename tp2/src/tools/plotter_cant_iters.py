#!/usr/bin/python
import sys
import numpy as np
import matplotlib.pyplot as plt
if (len(sys.argv) < 2):
	sys.exit("Argumentos: <datafile_path> <image_output_path>")

#read input data from file
dataFile = open(sys.argv[1], 'r')
outputFilePath = str(sys.argv[2])
eje_x = []
eje_y = []
eje_y2 = []

for line in dataFile:	
	line_values = line.split()	
	eje_x.append(float(line_values[0]))
	eje_y.append(float(line_values[1]))
	eje_y2.append(float(line_values[2]))

#plt.gca().set_color_cycle(['blue', 'red', 'green', 'yellow', 'orange', 'gray', 'black', 'purple', 'pink', 'yellow'])
plt.plot(eje_x, eje_y, linestyle='-', marker='o', color='purple')
plt.plot(eje_x, eje_y2, linestyle='--', color='red')

#plt.yscale('log')
ax = plt.gca()


plt.legend(['Tiempo', 'Promedio'], loc='lower left')
#ax.set_title("Cantidad de iteraciones")    
ax.set_xlabel('Factor de navegacion')
#ax.set_ylabel('Cantidad de iteraciones hasta converger')
ax.set_ylabel('Segundos por iteracion')
plt.savefig(outputFilePath)
plt.close()