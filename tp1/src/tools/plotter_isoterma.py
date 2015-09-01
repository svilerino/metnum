import sys
import math
import numpy as np
import matplotlib.pyplot as plt

isoterma = []
interna = []
externa = []
dataFile = open(sys.argv[1], 'r')

radio_interno = int(sys.argv[2])
radio_externo = int(sys.argv[3])

num_angulos = 0
for line in dataFile:
	data = line.split()
	isoterma.append(float(data[0]))
	interna.append(radio_interno)
	externa.append(radio_externo)
	num_angulos = num_angulos + 1

isoterma.append(isoterma[0])
interna.append(radio_interno)
externa.append(radio_externo)

delta_angulo = 2 * np.pi / num_angulos
angulos = np.arange(0, 2 * np.pi, delta_angulo).tolist()
angulos.append(0)

# index = 0
# for line in isoterma:
	# print math.degrees(angulos[index]), isoterma[index]
	# index = index + 1

# print isoterma
# print num_angulos
# print delta_angulo
# print angulos

ax = plt.subplot(111, polar=True, axisbg=(0.9, 0.9, 0.9))
ax.plot(angulos, interna, color='b', linewidth=4)
ax.plot(angulos, isoterma, color='r', linewidth=1)
ax.plot(angulos, externa, color='g', linewidth=3)
ax.set_yticks(range(0, int(1.5 * radio_externo), 100))
ax.grid(True)

ax.set_title("Ubicacion de la isoterma", va='bottom')
plt.savefig(sys.argv[1] + ".png")
plt.close()
