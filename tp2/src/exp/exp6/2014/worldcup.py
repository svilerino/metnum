#!/usr/bin/python3
import os
from subprocess import call

ORDEN_OFICIAL = ["Germany", "Argentina", "Netherlands", "Brazil", "Colombia", "Belgium", "France", "Costa_Rica", "Chile", "Mexico", "Switzerland", "Uruguay", "Greece", "Algeria", "United_States", "Nigeria", "Ecuador", "Portugal", "Croatia", "Bosnia_Herzegovina", "Cote_dIvoire", "Italy", "Spain", "Russia", "Ghana", "England", "South_Korea", "Iran", "Japan", "Australia", "Honduras", "Cameroon"]


def traducir(f_input, f_output):
    equipos = {}
    resultados = []
    for linea in f_input:
        if '-' in linea and "[-" not in linea and "finals" not in linea:
            linea = linea.replace("United States", "United_States").replace("Costa Rica", "Costa_Rica").replace("South Korea", "South_Korea").replace("Bosnia-Herzegovina", "Bosnia_Herzegovina").replace("Côte d'Ivoire", "Cote_dIvoire")
            palabras = linea.split(" ")
            palabras_utiles = [palabra for palabra in palabras
                                if not any([l in palabra for l in ("(",")",".",":","/")])
                                and len(palabra) != 0]
            if "-" in palabras_utiles[3]:
                del palabras_utiles[3]
            nombre_jugador_1, resultado, nombre_jugador_2 = palabras_utiles[1:4]
            jugadores = []
            for nombre_jugador in (nombre_jugador_1, nombre_jugador_2):
                if not nombre_jugador in equipos.keys():
                    if len(equipos) > 0:
                        equipos[nombre_jugador] = max(equipos.values()) + 1
                    else:
                        equipos[nombre_jugador] = 1
                jugadores.append(equipos[nombre_jugador])
                assert(nombre_jugador in ORDEN_OFICIAL), nombre_jugador
            jugador_1, jugador_2 = jugadores
            goles_1, goles_2 = resultado.split("-")
            if goles_1 == goles_2:
                continue
            ganador = jugador_1 if goles_1 > goles_2 else jugador_2
            perdedor = jugador_2 if goles_1 > goles_2 else jugador_1
            resultados.append((ganador, max(goles_1, goles_2), perdedor, min(goles_1, goles_2)))
    print(32, len(resultados), file=f_output)
    for resultado in resultados:
        print(1, *resultado, file=f_output)
    return equipos


def parsear_e_imprimir():
    with open("worldcup-original.txt") as f_input:
        with open("worldcup.txt", "w") as f_output:
            return traducir(f_input, f_output)


def ejecutar_pagerank(c):
    with open("worldcup.in", "w") as f_output:
        print("0 %.2f 1 worldcup.txt 0.00001" % c, file=f_output)
    call(["../../../../bin/tp2", "worldcup.in", "worldcup.out"])


def parsear_salida(equipos):
    with open("worldcup.out") as f_input:
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
    return diferencia_con_ideal([equipo for puntaje, equipo in puntajes_por_equipo])


def diferencia_con_ideal(orden):
    error = 0
    for posicion, jugador in enumerate(orden):
        error += abs(posicion - ORDEN_OFICIAL.index(jugador))
    return error


def plotear_salida(res):
    import matplotlib.pyplot as plt
    c = [t[0] for t in res]
    posiciones = [t[1] for t in res]
    plt.plot(c, posiciones, 'o-')
    x1, x2, y1, y2 = plt.axis()
    plt.axis((0, x2, 0, y2))
    plt.ylabel("Diferencia entre GeM y oficial")
    plt.xlabel("Valor de c")
    plt.savefig("2014.png")


equipos = parsear_e_imprimir()
diferencias = []
for c in [i*0.1 for i in range(11)]:
    print(c)
    ejecutar_pagerank(c)
    diferencia = parsear_salida(equipos)
    diferencias.append((c, diferencia))
    print(diferencia)
plotear_salida(diferencias)
