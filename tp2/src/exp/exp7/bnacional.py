#!/usr/bin/python3
import os
import random
from subprocess import call

ORDEN_OFICIAL = [
    "Banfield",
    "DefyJus",
    "Huracan",
    "Indepte",
    "Atl_Tuc",
    "Instituto",
    "Gsia_Jujuy",
    "Crucero",
    "S_M_SJ",
    "Union",
    "IndpRvia",
    "Sarmiento",
    "Ferro",
    "B_Unidos",
    "Sp_Belgr",
    "Aldosivi",
    "D_Haig",
    "Brown",
    "Patronato",
    "Tall_Cba",
    "AlteBrown",
    "S_Carlos"
]

def traducir(f_input, f_output):
    equipos = {}
    resultados = []
    fecha = 0
    for linea in f_input:
        linea = linea.replace(".", "_").replace(" ", "").replace("\t", " ").replace("  ", " ")
        palabras = linea.split(" ")
        if "Matchday" in palabras[0]:
            fecha += 1
        nombre_jugador_1, resultado, nombre_jugador_2 = palabras[1:4]
        jugadores = []
        for nombre_jugador in (nombre_jugador_1, nombre_jugador_2):
            if not nombre_jugador in equipos.keys():
                if len(equipos) > 0:
                    equipos[nombre_jugador] = max(equipos.values()) + 1
                else:
                    equipos[nombre_jugador] = 1
            jugadores.append(equipos[nombre_jugador])
        jugador_1, jugador_2 = jugadores
        goles_1, goles_2 = resultado.split("-")
        if goles_1 == goles_2:
            continue
        ganador = jugador_1 if goles_1 > goles_2 else jugador_2
        perdedor = jugador_2 if goles_1 > goles_2 else jugador_1
        resultados.append((fecha, ganador, max(goles_1, goles_2), perdedor, min(goles_1, goles_2)))
    print(len(equipos), len(resultados), file=f_output)
    for resultado in resultados:
        print(*resultado, file=f_output)
    return equipos


def parsear_e_imprimir():
    with open("bnacional-original.txt") as f_input:
        with open("bnacional.txt", "w") as f_output:
            return traducir(f_input, f_output)


def ejecutar_pagerank(filename):
    call(["../../../bin/tp2", "%s.in" % filename, "%s.out" % filename])


def generar_fake(filename_original, filename_fake, goles):
    with open("%s.txt" % filename_original) as f_original:
        with open("%s.txt" % filename_fake, "w") as f_fake:
            for i, linea in enumerate(f_original):
                if i == 0:
                    equipos, partidos = linea.split(" ")
                    print(equipos, int(partidos) + 1, file=f_fake)
                else:
                    print(linea, end="", file=f_fake)
            print(46, 8, goles, 20, 0, file=f_fake)


def parsear_salida(equipos, filename):
    with open("%s.out" % filename) as f_input:
        puntajes = [float(linea) for linea in f_input]
    puntajes_por_equipo = []
    for indice_1, puntaje in enumerate(puntajes):
        for equipo, indice_2 in equipos.items():
            if indice_1+1 == indice_2:
                puntajes_por_equipo.append((puntaje, equipo))
    puntajes_por_equipo.sort(key=lambda tupla: tupla[0])
    puntajes_por_equipo.reverse()
    for puntaje, equipo in puntajes_por_equipo:
        print(equipo, puntaje)
    return [equipo for puntaje, equipo in puntajes_por_equipo]


def diferencia_con_ideal(orden):
    return diferencia_entre_rankings(orden, ORDEN_OFICIAL)


def diferencia_entre_rankings(orden1, orden2):
    diferencia = 0
    for posicion, jugador in enumerate(orden1):
        diferencia += abs(posicion - orden2.index(jugador))
    return diferencia


def plotear_salida(diferencia_con_gem, diferencia_con_oficial):
    import matplotlib.pyplot as plt
    plt.plot(diferencia_con_oficial, label="vs ránking oficial")
    plt.plot(diferencia_con_gem, label="vs GeM sin partido simulado")
    plt.legend(loc=4)
    plt.ylabel("Distancia entre ránkings")
    plt.xlabel("Diferencia de goles simulada entre Villa San Carlos y Banfield")
    plt.savefig("diferencia.png")


equipos = parsear_e_imprimir()
ejecutar_pagerank("bnacional")
orden1 = parsear_salida(equipos, "bnacional")
diferencia_con_gem, diferencia_con_oficial = [], []
for goles in range(70):
    print("Goles:", goles)
    generar_fake("bnacional", "bnacional-fake", goles)
    ejecutar_pagerank("bnacional-fake")
    orden2 = parsear_salida(equipos, "bnacional-fake")
    diferencia_con_gem.append(diferencia_entre_rankings(orden1, orden2))
    print("Diferencia:", diferencia_entre_rankings(orden1, orden2))
    diferencia_con_oficial.append(diferencia_con_ideal(orden2))
plotear_salida(diferencia_con_gem, diferencia_con_oficial)
