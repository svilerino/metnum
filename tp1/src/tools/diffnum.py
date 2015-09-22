import numpy
import sys

#read input data from file

for n in xrange(10,131):
	currentFilePath = sys.argv[1] + "_" + '{0:03d}'.format(n) + "_radios"+ "_" + '{0:03d}'.format(n) + "_angulos.out"
	#print currentFilePath

	lu_input = "gauss_" + currentFilePath
	gauss_input = "lu_" + currentFilePath

	lu_file = open(lu_input, 'r')
	gauss_file = open(gauss_input, 'r')

	gauss_sol = []
	lu_sol = []

	#cargo ambas soluciones a vectores
	for line in gauss_file:
		gauss_sol.append(float(line))

	for line in lu_file:
		lu_sol.append(float(line))

	#calculo norma 2 y norma inf
	#print lu_sol, gauss_sol
	diff_vec = numpy.subtract(lu_sol, gauss_sol)
	#print diff_vec
	print numpy.linalg.norm(diff_vec)
	#print numpy.linalg.norm(diff_vec, numpy.inf)
