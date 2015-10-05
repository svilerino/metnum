#!/usr/bin/python3
import os
from subprocess import call

ORDEN_OFICIAL = [
    "West_Germany",
    "Hungary",
    "Austria",
    "Uruguay",
    "Switzerland",
    "Brazil",
    "England",
    "Yugoslavia",
    "France",
    "Turkey",
    "Italy",
    "Belgium",
    "Mexico",
    "Czechoslovakia",
    "Scotland",
    "South_Korea"
]
REEMPLAZOS = {
    "West Germany", "West_Germany",
    "South Korea", "South_Korea"
}

def traducir(f_input, f_output):
    equipos = {}
    resultados = []
    for linea in f_input:
        if '-' in linea and "[-" not in linea and "finals" not in linea and "Group" not in linea and "Third" not in linea:
            linea = linea.replace("West Germany", "West_Germany").replace("South Korea", "South_Korea")
            palabras_utiles = [palabra for palabra in linea.split(" ")
                                if not any([l in palabra for l in ("(",")",".",":","/")])
                                and len(palabra) != 0]
            if "-" in palabras_utiles[4]:
                del palabras_utiles[4]
            nombre_jugador_1, resultado, nombre_jugador_2 = palabras_utiles[2:5]
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
            resultados.append((ganador, max(goles_1, goles_2), perdedor, min(goles_1, goles_2)))
    print(len(equipos), len(resultados), file=f_output)
    for resultado in resultados:
        print(1, *resultado, file=f_output)
    return equipos


def parsear_e_imprimir():
    with open("worldcup-original.txt") as f_input:
        with open("worldcupz.txt", "w") as f_output:
            return traducir(f_input, f_output)


def ejecutar_pagerank():
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


equipos = parsear_e_imprimir()
ejecutar_pagerank()
print(parsear_salida(equipos))