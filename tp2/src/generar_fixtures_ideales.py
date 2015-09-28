#!/usr/bin/python3
import sys


def generar_fecha(cant_equipos, fecha):
	for equipo in range(1, cant_equipos+1):
		rival = equipo + fecha
		if rival > cant_equipos:
			rival -= cant_equipos
		print(fecha, equipo, int(equipo < rival), rival, int(rival < equipo))


def generar_fixture_equipos(cant_equipos, cant_fechas):
	cant_partidos = cant_fechas * cant_equipos
	print(cant_equipos, cant_partidos)
	for fecha in range(1, cant_fechas+1):
		generar_fecha(cant_equipos, fecha)


if len(sys.argv) < 3:
	print ("Uso:", sys.argv[0], "cantidad_de_equipos", "cantidad_de_fechas")
	exit(-1)
cant_equipos = int(sys.argv[1])
cant_fechas = int(sys.argv[2])
if cant_fechas >= cant_equipos:
	print ("Debería haber más equipos que fechas")
	exit(-1)
generar_fixture_equipos(cant_equipos, cant_fechas)