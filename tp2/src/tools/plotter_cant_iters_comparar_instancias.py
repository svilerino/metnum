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
eje_y3 = []

for line in dataFile:	
	line_values = line.split()	
	eje_x.append(float(line_values[0]))
	eje_y.append(float(line_values[1]))
	eje_y2.append(float(line_values[2]))
	eje_y3.append(float(line_values[3]))

#plt.gca().set_color_cycle(['blue', 'red', 'green', 'yellow', 'orange', 'gray', 'black', 'purple', 'pink', 'yellow'])
plt.plot(eje_x, eje_y, linestyle='-', marker='o', color='green')
plt.plot(eje_x, eje_y2, linestyle='-', marker='*', color='red')
plt.plot(eje_x, eje_y3, linestyle='-', marker='d', color='blue')

#iters notredame berkstan stanford
plt.legend(['NotreDame', 'BerkStan', 'Stanford'], loc='upper left')

#plt.yscale('log')
ax = plt.gca()

#ax.set_title("Cantidad de iteraciones")    
ax.set_xlabel('Factor de navegacion $ \\alpha $')
ax.set_ylabel('Cantidad de iteraciones hasta converger')
plt.savefig(outputFilePath)
plt.close()