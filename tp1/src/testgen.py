import numpy
import sys
import math
import random

#parameters validation
if(len(sys.argv) < 7):
    print "Use with parameters: testgen.py <outputTestSuitePath> <r_i> <r_e> <m+1> <n> <iso> <ninst>"
    sys.exit(1); 

#read input data from file
outputFilePath = str(sys.argv[1])
r_i = float(sys.argv[2])
r_e = float(sys.argv[3])
m_1 = int(sys.argv[4])
n = int(sys.argv[5])
iso = int(sys.argv[6])
ninst = int(sys.argv[7])

with open(outputFilePath, 'w') as f:
    f.write(str(r_i) + 	" " + str(r_e) + " " + str(m_1) + " " + str(n) + " " + str(iso)+ " " + str(ninst) + "\n")

    for i in xrange(0,ninst): #Generate each instance
    	for j in xrange(0,n): #Generate n internal wall values
    		value = random.uniform(1400, 1600)
    		f.write(str(value) +" ")

    	for j in xrange(0,n):#Generate n external wall values
    		if j == (n-1):#To avoid putting an extra space in the last external value
    			value = random.uniform(50, 200)
    			f.write(str(value))
    			continue
    		value = random.uniform(50, 200)
    		f.write(str(value) + " ")
    	f.write("\n")






