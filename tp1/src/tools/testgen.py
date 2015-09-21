import numpy
import sys
import math
import random

if(len(sys.argv) < 2):
	print "The first parameter specifies the test generation mode: 1 for the first method (varying n and m) "
	print " 2 for the second method (given fixed n and m, change the temperatures). "
	print "3 for the third method (given fixed m and temperatures, vary n) For help for each mode, run testgen.py <Mode>"
else:

	if(int(sys.argv[1]) == 1):
		#parameters validation
		if(len(sys.argv) < 11):
		    print "Mode 1: Use with parameters: testgen.py <Mode> <outputTestSuitePath#> <r_i> <r_e> <m+1_s> <m+1_e> <n_s> <n_e> <iso> <ninst>"
		    print "Where the # of radius goes from m+1_s to m+1_e and the # of angles goes from n_s to n_e"
		else:
			#read input data from file
			outputFilePath = str(sys.argv[2])
			r_i = float(sys.argv[3])
			r_e = float(sys.argv[4])
			m_1_s = int(sys.argv[5])
			m_1_e = int(sys.argv[6])
			n_s = int(sys.argv[7])
			n_e = int(sys.argv[8])
			iso = int(sys.argv[9])
			ninst = int(sys.argv[10])

			index = 0

			for m in xrange(m_1_s, m_1_e+1, 3):
				#for n in xrange(n_s, n_e+1):
				n = m
				currentFilePath = outputFilePath + "_" + '{0:03d}'.format(m) + "_radios"+ "_" + '{0:03d}'.format(n) + "_angulos.in"
				index = index + 1
				with open(currentFilePath, 'w') as f:
					f.write(str(r_i) + 	" " + str(r_e) + " " + str(m) + " " + str(n) + " " + str(iso)+ " " + str(ninst) + "\n")
					for i in xrange(0,ninst):
						for j in xrange(0,n):
							value = random.uniform(1500, 1500)
							f.write(str(value) + " ")
						for j in xrange(0,n):
							if j == (n-1):
								value = random.uniform(100, 100)
								f.write(str(value))
								continue
							value = random.uniform(100, 100)
							f.write(str(value) + " ")
						f.write("\n")

	elif(int(sys.argv[1]) == 2):
	
		#parameters validation
		if(len(sys.argv) < 9):
		    print "Mode 2: Use with parameters: testgen.py <Mode> <outputTestSuitePath> <r_i> <r_e> <m+1> <n> <iso> <ninst>"
		else:	
			#read input data from file
			outputFilePath = str(sys.argv[2])
			r_i = float(sys.argv[3])
			r_e = float(sys.argv[4])
			m_1 = int(sys.argv[5])
			n = int(sys.argv[6])
			iso = int(sys.argv[7])
			ninst = int(sys.argv[8])
			
			with open(outputFilePath, 'w') as f:
			    f.write(str(r_i) + 	" " + str(r_e) + " " + str(m_1) + " " + str(n) + " " + str(iso)+ " " + str(ninst) + "\n")
			
			    for i in xrange(0,ninst): #Generate each instance
			    	for j in xrange(0,n): #Generate n internal wall values
			    		value = random.uniform(1450, 1550)
			    		f.write(str(value) +" ")
			
			    	for j in xrange(0,n):#Generate n external wall values	
			    		if j == (n-1):#To avoid putting an extra space in the last external value
			    			value = random.uniform(50, 200)
			    			f.write(str(value))
			    			continue
			    		value = random.uniform(50, 200)
			    		f.write(str(value) + " ")
			    	f.write("\n")
	elif(int(sys.argv[1]) == 3):
		if(len(sys.argv) < 9):
			print "Mode 3: Use with parameters: testgen.py <Mode> <outputTestSuitePath#> <r_i> <r_e> <m+1_s> <m+1_e> <n> <iso>"
		else:
			outputFilePath = str(sys.argv[2])
			r_i = float(sys.argv[3])
			r_e = float(sys.argv[4])
			m_1_s = int(sys.argv[5])
			m_1_e = int(sys.argv[6])
			n = int(sys.argv[7])
			iso = int(sys.argv[8])

			int_temps = []
			ext_temps = []

			for i in xrange(0,n):
				int_value = random.uniform(1450, 1550)
				ext_value = random.uniform(50, 200)
				int_temps.append(int_value)
				ext_temps.append(ext_value)

			index = m_1_s

			for m in xrange(m_1_s, m_1_e+1):
				# print index
				currentFilePath = outputFilePath + "_" + '{0:03d}'.format(index) + "_radios.in"
				index = index + 1
				with open(currentFilePath, 'w') as f:
					f.write(str(r_i) + 	" " + str(r_e) + " " + str(m) + " " + str(n) + " " + str(iso)+ " " + "1" + "\n")
					for j in xrange(0,n):
						f.write(str(int_temps[j]) + " ")
					for k in xrange(0,n):
						f.write(str(ext_temps[k]) + " ")



	else:
		print "Invalid Mode"
	
