#!/usr/bin/python

#input arg
#"$file"_time_consumed_med.png "$file" 0 "${3}"

import sys
import numpy as np
import matplotlib.pyplot as plt


#read input data from file
#outputFilePath = str(sys.argv[1])
#dataFile = open(sys.argv[2], 'r')
#
#x = []
#time_gauss = []
#time_lu = []
#ratio_lu_gauss = []
#
#for line in dataFile:	
#	matrix_dim = int(line.split()[0])
#	time_consumed_gauss = float(line.split()[1])
#	time_consumed_lu = float(line.split()[2])
#	
#	time_gauss.append(time_consumed_gauss)
#	time_lu.append(time_consumed_lu)
#	ratio_lu_gauss.append(time_consumed_lu/time_consumed_gauss)
#	x.append(matrix_dim)
#
#plt.gca().set_color_cycle(['red', 'blue', 'green'])
#
##print time_gauss, time_lu, ratio_lu_gauss
#
#plt.plot(x, time_gauss)
#plt.plot(x, time_lu)
#plt.plot(x, ratio_lu_gauss)
#
#plt.legend(['EG', 'LU', 'Ratio LU/EG'], loc='upper left')
##plt.yscale('log')
#
#ax = plt.gca()
#ax.set_title("Rendimiento respecto a la dimension de la matriz")    
#ax.set_xlabel('Dimension de la matriz: x = n*(m+1)')
#ax.set_ylabel('f(x) = Tiempo en microsegundos')
#
#plt.savefig(outputFilePath)
#plt.close()

import sys
import numpy as np
import matplotlib.pyplot as plt


#read input data from file
outputFilePath = str(sys.argv[1])
dataFile = open(sys.argv[2], 'r')

x = []
time = []
time_over_x = []
time_over_x_cuad = []
time_over_x_cub = []
time_over_x_cuart = []

for line in dataFile:	
	matrix_dim = int(line.split()[0])
	time_consumed = float(line.split()[1])
	
	time.append(time_consumed)
	time_over_x.append(time_consumed / float(matrix_dim))
	time_over_x_cuad.append(time_consumed / float(pow(matrix_dim, 2)))
	time_over_x_cub.append(time_consumed / float(pow(matrix_dim, 3)))
	time_over_x_cuart.append(time_consumed / float(pow(matrix_dim, 4)))
	
	x.append(matrix_dim)

plt.gca().set_color_cycle(['red', 'green', 'black', 'blue'])

#plt.plot(x, time)
#plt.plot(x, time_over_x)
#plt.plot(x, time_over_x_cuad)
plt.plot(x, time_over_x_cub)
#plt.plot(x, time_over_x_cuart)

plt.legend(['f(x)/n^3'], loc='upper right')
#plt.legend(['f(x)', 'f(x)/n', 'f(x)/n^2', 'f(x)/n^3'], loc='upper right')
#plt.yscale('log')

ax = plt.gca()
ax.set_title("Evolucion de rendimiento respecto a la dimension de la matriz")    
ax.set_xlabel('Dimension de la matriz: x = n*(m+1)')
ax.set_ylabel('f(x) = Tiempo en microsegundos')

plt.savefig(outputFilePath)
plt.close()