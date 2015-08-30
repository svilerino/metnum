import numpy
import matplotlib.pyplot as plt
import sys
import math

#parameters validation
if(len(sys.argv) < 3):
    print "Use with parameters: plotter.py <outputPNGPath> <inputDataFile of lines x y f(x) separated by space> <graphic_type_number> <...extra params>"
    sys.exit(1); 

#read input data from file
outputFilePath = str(sys.argv[1])
dataFile = open(sys.argv[2], 'r')
graphictype = int(sys.argv[3])

#transform input data into 2 arrays representing the points in the cartesian plane.
#para cada valor de x, en y se ponen puntitos para diferentes densidades de aristas
graphic_x = []
graphic_y = []

#en el archivo viene ordenado x y en orden relativo
#para cada valor de y con el mismo x se plotean todos los y sobre el mismo x 
#y un punto intermedio, que es la media de todos los y para el mismo x
graphic_med_x = []
graphic_med_y = []

graphic_med_y_over_n = []
graphic_med_y_over_n_square = []
graphic_med_y_over_n_cube = []
graphic_med_y_over_n_fourth = []

cant_angulos_por_radio_fijo = 0
ultima_cant_radios = -1
acum = 0

for line in dataFile:
	#parse line data
	cant_radios = float(line.split()[0])
	aristas = float(line.split()[1])
	tiempo_consumido = float(line.split()[2])
	if(ultima_cant_radios != cant_radios):
		if(ultima_cant_radios != -1):	
			graphic_med_x.append(ultima_cant_radios)
			value = acum / float(cant_angulos_por_radio_fijo)
			graphic_med_y.append(value)

			graphic_med_y_over_n.append(float(value / ultima_cant_radios))
			graphic_med_y_over_n_square.append(float(value / math.pow(ultima_cant_radios, 2)))
			graphic_med_y_over_n_cube.append(float(value / math.pow(ultima_cant_radios, 3)))
			graphic_med_y_over_n_fourth.append(float(value / math.pow(ultima_cant_radios, 4)))
			acum = 0
			cant_angulos_por_radio_fijo = 0
	
	ultima_cant_radios = cant_radios
	cant_angulos_por_radio_fijo = cant_angulos_por_radio_fijo + 1
	acum = acum + tiempo_consumido

	#append to the graphic vectors
	graphic_x.append(cant_radios)
	graphic_y.append(tiempo_consumido)#asociado tambien a la cantidad de aristas en la variable aristas

#el ultimo valor de cant_nodos no va a entrar al if de ultima_cant_radios
graphic_med_x.append(ultima_cant_radios)
value = acum / float(cant_angulos_por_radio_fijo)
graphic_med_y.append(value)

graphic_med_y_over_n.append(float(value / ultima_cant_radios))
graphic_med_y_over_n_square.append(float(value / math.pow(ultima_cant_radios, 2)))
graphic_med_y_over_n_cube.append(float(value / math.pow(ultima_cant_radios, 3)))
graphic_med_y_over_n_fourth.append(float(value / math.pow(ultima_cant_radios, 4)))


#plot the data
if(graphictype == 0):
	fit_order = int(sys.argv[4])
	if(fit_order != -1):
		print "Using Fit type: " + sys.argv[4]
		coefficients = numpy.polyfit(graphic_med_x, graphic_med_y, fit_order)
		polynomial = numpy.poly1d(coefficients)
		graphic_med_y_fit = polynomial(graphic_med_x)

		plt.plot(graphic_med_x, graphic_med_y, 'g-', graphic_med_x, graphic_med_y, 'go', graphic_x, graphic_y, 'ro', graphic_med_x, graphic_med_y_fit, 'm-.', label="")	
		plt.ylabel('\nf(x) = Verde: Tiempo consumido en promedio para las distintas cantidades de angulos en la discretizacion\nRojo: Tiempos consumidos por las distintas cantidades de angulos para una cantidad fija de radios\nMagenta:Curva fitteada orden ' + str(fit_order), fontsize=14)
	else:
		plt.plot(graphic_med_x, graphic_med_y, 'g-', graphic_med_x, graphic_med_y, 'go', graphic_x, graphic_y, 'ro', label="")	
		plt.ylabel('\nf(x) = Verde: Tiempo consumido en promedio para las distintas cantidades de angulos en la discretizacion\nRojo: Tiempos consumidos por las distintas cantidades de angulos para una cantidad fija de radios', fontsize=14)

	plt.xlabel('x = Cantidad de radios', fontsize=14)
	plt.title("Rendimiento")
	#plt.yscale('log')
	plt.grid()
	#plt.show()
	plt.savefig(outputFilePath)
else:
	if(graphictype == 1):
		plt.plot(graphic_med_x, graphic_med_y_over_n, 'g-', label="")	
		plt.xlabel('x = Cantidad de radios', fontsize=14)
		plt.ylabel('\nf(x)/x', fontsize=14)
		plt.title("Rendimiento")
		#plt.yscale('log')
		plt.grid()
		#plt.show()
		plt.savefig(outputFilePath)
	else:
		if(graphictype == 2):
			plt.plot(graphic_med_x, graphic_med_y_over_n_square, 'g-', label="")	
			plt.xlabel('x = Cantidad de radios', fontsize=14)
			plt.ylabel('\nf(x)/(n^2)', fontsize=14)
			plt.title("Rendimiento")
			#plt.yscale('log')
			plt.grid()
			#plt.show()
			plt.savefig(outputFilePath)
		else:
			if(graphictype == 3):
				plt.plot(graphic_med_x, graphic_med_y_over_n_cube, 'g-', label="")	
				plt.xlabel('x = Cantidad de radios', fontsize=14)
				plt.ylabel('\nf(x)/(n^3)', fontsize=14)
				plt.title("Rendimiento")
				#plt.yscale('log')
				plt.grid()
				#plt.show()
				plt.savefig(outputFilePath)
			else:
				if(graphictype == 4):
					plt.plot(graphic_med_x, graphic_med_y_over_n_fourth, 'g-', label="")	
					plt.xlabel('x = Cantidad de radios', fontsize=14)
					plt.ylabel('\nf(x)/(n^4)', fontsize=14)
					plt.title("Rendimiento")
					#plt.yscale('log')
					plt.grid()
					#plt.show()
					plt.savefig(outputFilePath)
plt.close()