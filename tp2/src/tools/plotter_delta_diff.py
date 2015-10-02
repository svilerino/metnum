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

diff_1 = []
diff_2 = []
diff_3 = []
diff_4 = []
diff_5 = []
diff_6 = []
diff_7 = []
diff_8 = []
diff_9 = []
diff_10 = []

iteracion = 0

for line in dataFile:	
	iteracion = iteracion + 1
	eje_x.append(iteracion)

	line_values = line.split()
	diff_1.append(float(line_values[0]))
	diff_2.append(float(line_values[1]))
	diff_3.append(float(line_values[2]))
	diff_4.append(float(line_values[3]))
	diff_5.append(float(line_values[4]))
	diff_6.append(float(line_values[5]))
	diff_7.append(float(line_values[6]))
	diff_8.append(float(line_values[7]))
	diff_9.append(float(line_values[8]))
	diff_10.append(float(line_values[9]))

#plt.gca().set_color_cycle(['blue', 'red', 'green', 'yellow', 'orange', 'gray', 'black', 'purple', 'pink', 'yellow'])

plt.plot(eje_x, diff_1, linestyle='-', marker='.', color='pink')
plt.plot(eje_x, diff_2, linestyle='-', marker='*', color='red')
plt.plot(eje_x, diff_3, linestyle='-', marker='o', color='orange')
plt.plot(eje_x, diff_4, linestyle='-', marker='D', color='yellow')
plt.plot(eje_x, diff_5, linestyle='-', marker='x', color='green')
plt.plot(eje_x, diff_6, linestyle='-', marker='+', color='lightgreen')
plt.plot(eje_x, diff_7, linestyle='-', marker='>', color='skyblue')
plt.plot(eje_x, diff_8, linestyle='-', marker='<', color='dodgerblue')
plt.plot(eje_x, diff_9, linestyle='-', marker='^', color='blue')
plt.plot(eje_x, diff_10, linestyle='-', marker='v', color='magenta')

plt.legend(['c = 0.0', 'c = 0.1', 'c = 0.2', 'c = 0.3', 'c = 0.4', 'c = 0.5', 'c = 0.6', 'c = 0.7', 'c = 0.8', 'c = 0.9'], loc='upper right')

plt.yscale('log')
plt.xscale('log')

ax = plt.gca()
ax.set_ylim([0.000001,1])
#ax.set_title("Convergencia del metodo de las potencias")    
ax.set_xlabel('Iteraciones')
ax.set_ylabel('$||x^k - x^{k-1}||_1$')

plt.savefig(outputFilePath)
plt.close()