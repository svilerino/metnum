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

eje_y_diff_1 = []
eje_y_diff_2 = []
eje_y_diff_3 = []
eje_y_diff_4 = []

iteracion = 0

for line in dataFile:	
	iteracion = iteracion + 1
	line_values = line.split()

	diff_1 = float(line_values[0])
	if(len(line_values) > 1):
		diff_2 = float(line_values[1])
	else:
		diff_2 = 0

	if(len(line_values) > 2):
		diff_3 = float(line_values[2])
	else:
		diff_3 = 0

	if(len(line_values) > 3):
		diff_4 = float(line_values[3])
	else:
		diff_4 = 0

	eje_x.append(iteracion)
	eje_y_diff_1.append(diff_1)
	eje_y_diff_2.append(diff_2)
	eje_y_diff_3.append(diff_3)
	eje_y_diff_4.append(diff_4)

plt.gca().set_color_cycle(['blue', 'red', 'green'])

plt.plot(eje_x, eje_y_diff_1)
plt.plot(eje_x, eje_y_diff_2)
plt.plot(eje_x, eje_y_diff_3)
plt.plot(eje_x, eje_y_diff_4)
#plt.legend(['Test1', 'Test2', 'Test3', 'Test4'], loc='upper right')

plt.yscale('log')

ax = plt.gca()
ax.set_title("Convergencia del metodo de las potencias")    
ax.set_xlabel('Iteraciones')
ax.set_ylabel('Distancia manhattan de la solucion entre iteraciones')

plt.savefig(outputFilePath)
plt.close()

#import sys
#import numpy as np
#import matplotlib.pyplot as plt
#import math
#
#
##read input data from file
#outputFilePath = str(sys.argv[1])
#dataFile = open(sys.argv[2], 'r')
#
#
#
#
#
#
#x = []
#time = []
#time_over_logx = []
#time_over_x = []
#time_over_x_cuad = []
#time_over_x_cub = []
#time_over_x_cuart = []
#
#for line in dataFile:	
#	matrix_dim = int(line.split()[0])
#	time_consumed = float(line.split()[1])
#	
#	if(matrix_dim > 1 ):
#		time.append(time_consumed)
#		time_over_logx.append(time_consumed / math.log10(matrix_dim))
#		time_over_x.append(time_consumed / float(matrix_dim))
#		time_over_x_cuad.append(time_consumed / float(pow(matrix_dim, 2)))
#		time_over_x_cub.append(time_consumed / float(pow(matrix_dim, 3)))
#		time_over_x_cuart.append(time_consumed / float(pow(matrix_dim, 4)))
#		
#		x.append(matrix_dim)
#
##plt.plot(x, time, 'b-', x, time, 'b*')
#
#plt.gca().set_color_cycle(['blue', 'green', 'black', 'red'])
#
#fit_order = 1
#coefficients = np.polyfit(x, time, fit_order)
#polynomial = np.poly1d(coefficients)
#y_fit = polynomial(x)
#
#plt.plot(x, time)
#plt.plot(x, y_fit)
#
##plt.plot(x, time_over_logx)
##plt.plot(x, time_over_x)
##plt.plot(x, time_over_x_cuad)
##plt.plot(x, time_over_x_cub)
##plt.plot(x, time_over_x_cuart)
#
#plt.legend(['f(x)', 'fitteo lineal'], loc='upper right')
##plt.legend(['f(x)', 'f(x)/n', 'f(x)/n^2', 'f(x)/n^3'], loc='upper right')
##plt.yscale('log')
#plt.grid()
#
#ax = plt.gca()
#ax.set_title("Disipacion del calor en una direccion fija")    
#ax.set_xlabel('Distancia al centro del horno')
#ax.set_ylabel('f(x) = Temperatura en grados')
#
#plt.savefig(outputFilePath)
#plt.close()