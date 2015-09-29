#!/usr/bin/python3
import sys
import random
from subprocess import call


def print_partido(fecha, equipo1, equipo2, output=sys.stdout):
    ganador = equipo1 if equipo1 < equipo2 else equipo2
    perdedor = equipo2 if equipo1 < equipo2 else equipo1
    print(fecha, ganador, 1, perdedor, 0, file=output)


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

def generar_fixture_equipos(cant_equipos, cant_fechas, output=sys.stdout):
    equipos = list(range(1, cant_equipos+1))
    enfrentamientos_restantes = [(i, j) for i in equipos for j in equipos[i:]]
    fechas = []
    for fecha in range(1, cant_fechas+1):
        if len(enfrentamientos_restantes) == 0:
            break
        fechas.append(generar_fecha(cant_equipos, fecha, enfrentamientos_restantes))
    cant_partidos = sum([len(fecha) for fecha in fechas])
    print(cant_equipos, cant_partidos, file=output)
    for fecha, enfrentamientos in enumerate(fechas):
        for enfrentamiento in enfrentamientos:
            print_partido(fecha+1, enfrentamiento[0], enfrentamiento[1], output)
    return len(enfrentamientos_restantes) == 0


def generar_archivos_input(cant_equipos):
    cant_fechas = 0
    fin = False
    while not fin:
        cant_fechas += 1
        with open("exp/exp4/fecha%s.txt" % cant_fechas, "w") as output:
            fin = generar_fixture_equipos(cant_equipos, cant_fechas, output)
    return cant_fechas

def ejecutar_pagerank(cant_fechas):
    for fechas in range(1, cant_fechas+1):
        call(["../bin/tp2", "exp/exp4/test%s.in" % fechas, "exp/exp4/test%s.out" % fechas])


def main():
    if len(sys.argv) < 2:
        print("Uso:", sys.argv[0], "cantidad_de_equipos")
        exit(-1)
    cant_equipos = int(sys.argv[1])
    random.seed(3)
    cant_fechas = generar_archivos_input(cant_equipos)
    ejecutar_pagerank(cant_fechas)


if __name__ == "__main__":
    main()