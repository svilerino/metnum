#!/usr/bin/python3
import sys
import random

def print_partido(fecha, equipo1, equipo2):
    ganador = equipo1 if equipo1 < equipo2 else equipo2
    perdedor = equipo2 if equipo1 < equipo2 else equipo1
    print(fecha, ganador, 1, perdedor, 0)


def generar_fecha(cant_equipos, fecha, enfrentamientos_restantes):
    enfrentamientos_fecha = []
    equipos_jugando_hoy = []
    enfrentamientos_posibles = enfrentamientos_restantes
    while len(enfrentamientos_posibles) > 0:
        enfrentamiento_nuevo = random.choice(enfrentamientos_posibles)
        equipos_jugando_hoy.append(enfrentamiento_nuevo[0])
        equipos_jugando_hoy.append(enfrentamiento_nuevo[1])
        enfrentamientos_restantes.remove(enfrentamiento_nuevo)
        enfrentamientos_fecha.append(enfrentamiento_nuevo)
        enfrentamientos_posibles = [enfrentamiento for enfrentamiento in enfrentamientos_restantes
                                    if not (enfrentamiento[0] in equipos_jugando_hoy
                                    or enfrentamiento[1] in equipos_jugando_hoy)]
    return enfrentamientos_fecha

def generar_fixture_equipos(cant_equipos, cant_fechas=None):
    equipos = list(range(1, cant_equipos+1))
    enfrentamientos_restantes = [(i, j) for i in equipos for j in equipos[i:]]
    if cant_fechas is None:
        cant_fechas = int(len(enfrentamientos_restantes) / 2)
    fechas = []
    for fecha in range(1, cant_fechas+1):
        if len(enfrentamientos_restantes) == 0:
            break
        fechas.append(generar_fecha(cant_equipos, fecha, enfrentamientos_restantes))
    cant_partidos = sum([len(fecha) for fecha in fechas])
    print(cant_equipos, cant_partidos)
    for fecha, enfrentamientos in enumerate(fechas):
        for enfrentamiento in enfrentamientos:
            print_partido(fecha+1, enfrentamiento[0], enfrentamiento[1])



if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Uso:", sys.argv[0], "cantidad_de_equipos", "[cantidad_de_fechas] (si se omite, se jugarán todos los partidos)")
        exit(-1)
    cant_equipos = int(sys.argv[1])
    random.seed(3)
    if len(sys.argv) > 2:
        cant_fechas = int(sys.argv[2])
        generar_fixture_equipos(cant_equipos, cant_fechas)
    else:
        generar_fixture_equipos(cant_equipos)