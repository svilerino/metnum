#!/usr/bin/python3
import sys
import random
from subprocess import call
from collections import defaultdict


def generar_partido(equipo1, equipo2):
    # 1 le gana a todos salvo a 2, que pierde con todos salvo con 1
    equipos = (equipo1, equipo2)
    if 1 in equipos and 2 in equipos:
        return 2, 1, 1, 0
    if 1 in equipos:
        return 1, 1, max(equipos), 0
    if 2 in equipos:
        return max(equipos), 1, 2, 0
    ganador = random.choice(equipos)
    perdedor = equipo1 if ganador == equipo2 else equipo2
    return ganador, 1, perdedor, 0


def generar_archivos_input(cant_equipos):
    with open("exp8.txt", "w") as f_output:
        equipos = list(range(1, cant_equipos+1))
        enfrentamientos_restantes = [(i, j) for i in equipos for j in equipos[i:]]
        print(len(equipos), len(enfrentamientos_restantes), file=f_output)
        for enfrentamiento in enfrentamientos_restantes:
            ganador, goles_ganador, perdedor, goles_perdedor = generar_partido(enfrentamiento[0], enfrentamiento[1])
            print(1, ganador, goles_ganador, perdedor, goles_perdedor, file=f_output)
    

def generar_archivos_test(c):
    with open("exp8.in", "w") as f_output:
        print("0 %.2f 1 exp8.txt 0.00001" % c, file=f_output)


def ejecutar_pagerank():
    call(["../../../bin/tp2", "exp8.in", "exp8.out"])


def parsear_salida():
    with open("exp8.out") as f_input:
        puntajes = [float(linea) for linea in f_input]
    puntajes_por_equipo = []
    for equipo, puntaje in enumerate(puntajes):
        puntajes_por_equipo.append((puntaje, equipo+1))
    puntajes_por_equipo.sort(key=lambda tupla: tupla[0])
    puntajes_por_equipo.reverse()
    return puntajes_por_equipo


def plotear_salida(res):
    import matplotlib.pyplot as plt
    c = [t[0] for t in res]
    posiciones = [t[1] for t in res]
    plt.plot(c, posiciones, 'o-')
    plt.ylabel("Posición de B")
    plt.xlabel("Valor de c")
    plt.savefig("exp8.png")


def main():
    if len(sys.argv) < 2:
        print("Uso:", sys.argv[0], "cantidad_de_equipos")
        exit(-1)
    cant_equipos = int(sys.argv[1])
    random.seed(5)
    generar_archivos_input(cant_equipos)
    resultados = []
    for c in [i*0.1 for i in range(11)]:
        generar_archivos_test(c)
        ejecutar_pagerank()
        resultado = parsear_salida()
        posicion_2 = [equipo for puntaje, equipo in resultado].index(2) + 1
        resultados.append((c, posicion_2))
    plotear_salida(resultados)


if __name__ == "__main__":
    main()