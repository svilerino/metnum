#!/usr/bin/python3
import sys
import random
from os import listdir
from os.path import isfile, join
from subprocess import call
from collections import defaultdict

def print_partido(fecha, equipo1, equipo2, f_output=sys.stdout):
    ganador = equipo1 if equipo1 < equipo2 else equipo2
    perdedor = equipo2 if equipo1 < equipo2 else equipo1
    print(fecha, ganador, 1, perdedor, 0, file=f_output)


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

def generar_fixture_equipos(cant_equipos, cant_fechas, f_output=sys.stdout):
    equipos = list(range(1, cant_equipos+1))
    enfrentamientos_restantes = [(i, j) for i in equipos for j in equipos[i:]]
    fechas = []
    for fecha in range(1, cant_fechas+1):
        if len(enfrentamientos_restantes) == 0:
            break
        fechas.append(generar_fecha(cant_equipos, fecha, enfrentamientos_restantes))
    cant_partidos = sum([len(fecha) for fecha in fechas])
    print(cant_equipos, cant_partidos, file=f_output)
    for fecha, enfrentamientos in enumerate(fechas):
        for enfrentamiento in enfrentamientos:
            print_partido(fecha+1, enfrentamiento[0], enfrentamiento[1], f_output)
    return len(enfrentamientos_restantes) == 0


def generar_archivos_input(cant_equipos):
    cant_fechas = 0
    fin = False
    while not fin:
        cant_fechas += 1
        with open("exp/exp4/fecha%s.txt" % cant_fechas, "w") as f_output:
            fin = generar_fixture_equipos(cant_equipos, cant_fechas, f_output)
    return cant_fechas


def generar_archivos_test(cant_fechas_totales, c):
    for cant_fechas in range(1, cant_fechas_totales+1):
        with open("exp/exp4/test%s.in" % cant_fechas, "w") as f_output:
            print("0 %.2f 1 exp/exp4/fecha%s.txt 0.00001" % (c, cant_fechas), file=f_output)


def ejecutar_pagerank(cant_fechas_totales):
    for cant_fechas in range(1, cant_fechas_totales+1):
        call(["../bin/tp2", "exp/exp4/test%s.in" % cant_fechas, "exp/exp4/test%s.out" % cant_fechas])


def parsear_salida_pagerank(cant_fechas_totales):
    resultado_por_cant_fechas = []
    for cant_fechas in range(1, cant_fechas_totales+1):
        with open("exp/exp4/test%s.out" % cant_fechas) as f_resultado:
            resultado_por_cant_fechas.append(f_resultado.readlines())
    res = {}
    for cant_fechas, resultado_cant_fechas in enumerate(resultado_por_cant_fechas):
        res[cant_fechas+1] = [float(line) for line in resultado_cant_fechas]
    return res


def ordenar_por_puntaje(puntaje_por_cant_fechas):
    res = {}
    for cant_fechas, puntajes in puntaje_por_cant_fechas.items():
        restantes = puntajes[:]
        ordenados = []
        while len(restantes) > 0:
            mejor_puntaje = max(restantes)
            mejores_posiciones = [posicion for posicion, puntaje in enumerate(puntajes) if puntaje == mejor_puntaje]
            competidores_empatados = [mejor_posicion+1 for mejor_posicion in mejores_posiciones]
            competidores_empatados.reverse()    # meto ruido para que el empate no termine bien ordenado
            ordenados += competidores_empatados
            restantes = [restante for restante in restantes if restante != mejor_puntaje]
        res[cant_fechas] = ordenados
    return res


def diferencia_con_ideal(orden_por_cant_fechas):
    res = defaultdict(int)
    for cant_fechas, orden in orden_por_cant_fechas.items():
        res[cant_fechas] = 0
        # sumo uno por cada número que está fuera de su lugar
        for posicion, jugador in enumerate(orden):
            if jugador != posicion + 1:
                res[cant_fechas] += 1
    return res


def imprimir_salida(cant_equipos, c, diferencias):
    with open("exp/exp4/resultados/%sequipos%.2fc.out" % (cant_equipos, c), "w") as f_output:
        for cant_fechas, diferencia in diferencias.items():
            print(cant_fechas, diferencia, file=f_output)


def main():
    if len(sys.argv) < 2:
        print("Uso:", sys.argv[0], "cantidad_de_equipos", "[c=0.85]")
        exit(-1)
    cant_equipos = int(sys.argv[1])
    if len(sys.argv) > 2:
        c = float(sys.argv[2])
    else:
        c = 0.85
    random.seed(3)
    cant_fechas_totales = generar_archivos_input(cant_equipos)
    generar_archivos_test(cant_fechas_totales, c)
    ejecutar_pagerank(cant_fechas_totales)
    puntaje_por_cant_fechas = parsear_salida_pagerank(cant_fechas_totales)
    orden_por_cant_fechas = ordenar_por_puntaje(puntaje_por_cant_fechas)
    diferencias = diferencia_con_ideal(orden_por_cant_fechas)
    imprimir_salida(cant_equipos, c, diferencias)


if __name__ == "__main__":
    main()