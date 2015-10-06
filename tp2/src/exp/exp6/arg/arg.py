#!/usr/bin/python3
import os
import random
from subprocess import call

EQUIPOS = {
    1: "Aldosivi",
    2: "Argentinos Juniors",
    3: "Arsenal",
    4: "Atlético de Rafaela",
    5: "Banfield",
    6: "Belgrano",
    7: "Boca Juniors",
    8: "Colón",
    9: "Crucero del Norte",
    10: "Defensa y Justicia",
    11: "Estudiantes (LP)",
    12: "Gimnasia y Esgrima (LP)",
    13: "Godoy Cruz",
    14: "Huracán",
    15: "Independiente",
    16: "Lanús",
    17: "Newell's Old Boys",
    18: "Nueva Chicago",
    19: "Olimpo",
    20: "Quilmes",
    21: "Racing Club",
    22: "River Plate",
    23: "Rosario Central",
    24: "San Lorenzo",
    25: "San Martín (SJ)",
    26: "Sarmiento",
    27: "Temperley",
    28: "Tigre",
    29: "Unión",
    30: "Vélez Sarsfield",
}

def parsear_y_limpiar():
    with open("arg-original.txt") as f_input:
        with open("arg.txt", "w") as f_output:
            resultados = []
            empates = []
            for numlinea, linea in enumerate(f_input):
                if numlinea == 0:
                    cant_equipos, _ = linea.split(" ")
                else:
                    fecha, jugador1, goles1, jugador2, goles2 = map(int, linea.split(" "))
                    if goles1 != goles2:
                        ganador, goles_ganador = (jugador1, goles1) if goles1 > goles2 else (jugador2, goles2)
                        perdedor, goles_perdedor = (jugador1, goles1) if goles1 < goles2 else (jugador2, goles2)
                        resultados.append((fecha, ganador, goles_ganador, perdedor, goles_perdedor))
                    else:
                        empates.append((fecha, jugador1, goles1, jugador2, goles2))
            print(cant_equipos, len(resultados), file=f_output)
            for resultado in resultados:
                print(*resultado, file=f_output)
    return resultados+empates


def ejecutar_pagerank():
    call(["../../../../bin/tp2", "arg.in", "arg.out" ])


def parsear_salida(equipos):
    with open("arg.out") as f_input:
        puntajes = [float(linea) for linea in f_input]
    puntajes_por_equipo = []
    for indice_1, puntaje in enumerate(puntajes):
        for indice_2, equipo in equipos.items():
            if indice_1+1 == indice_2:
                puntajes_por_equipo.append((puntaje, equipo))
    puntajes_por_equipo.sort(key=lambda tupla: tupla[0])
    puntajes_por_equipo.reverse()
    for puntaje, equipo in puntajes_por_equipo:
        print(equipo, puntaje)
    return [equipo for puntaje, equipo in puntajes_por_equipo]


def diferencia_entre_rankings(orden1, orden2):
    diferencia = 0
    for posicion, jugador in enumerate(orden1):
        diferencia += abs(posicion - orden2.index(jugador))
    return diferencia


def puntaje_estandar(cant_equipos, resultados):
    puntajes = [0 for _ in range(cant_equipos)]
    for resultado in resultados:
        _, jugador1, goles1, jugador2, goles2 = resultado
        if goles1 > goles2:
            puntajes[jugador1-1] += 3
        else:
            puntajes[jugador1-1] += 1
            puntajes[jugador2-1] += 1
    ordenados = [(puntaje, equipo+1) for equipo, puntaje in enumerate(puntajes)]
    ordenados.sort(key=lambda tupla: tupla[0])
    ordenados.reverse()
    puntajes_por_equipo = [(puntaje, EQUIPOS[equipo]) for puntaje, equipo in ordenados]
    for puntaje, equipo in puntajes_por_equipo:
        print(equipo, puntaje)
    return [equipo for puntaje, equipo in puntajes_por_equipo]


def plotear_salida(diferencias):
    import matplotlib.pyplot as plt
    plt.plot(diferencias, label="vs ránking oficial")
    plt.legend(loc=4)
    plt.ylabel("Distancia entre ránkings")
    plt.xlabel("Valor de c")
    plt.savefig("diferencia.png")


def generar_archivos_test(c):
    with open("arg.in", "w") as f_output:
        print("0 %.2f 1 arg.txt 0.00001" % c, file=f_output)


def plotear_salida(res):
    import matplotlib.pyplot as plt
    c = [t[0] for t in res]
    posiciones = [t[1] for t in res]
    plt.plot(c, posiciones, 'o-')
    x1, x2, y1, y2 = plt.axis()
    plt.axis((0, x2, 0, y2))
    plt.ylabel("Diferencia entre GeM y oficial")
    plt.xlabel("Valor de c")
    plt.savefig("arg.png")


resultados = parsear_y_limpiar()
diferencias = []
for c in [i*0.1 for i in range(11)]:
    generar_archivos_test(c)
    ejecutar_pagerank()
    orden_gem = parsear_salida(EQUIPOS)
    orden_oficial = puntaje_estandar(len(orden_gem), resultados)
    diferencia = diferencia_entre_rankings(orden_gem, orden_oficial)
    print(diferencia)
    diferencias.append((c, diferencia))
plotear_salida(diferencias)
print(diferencias)