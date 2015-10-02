#!/usr/bin/python3
import sys
import random
from subprocess import call
from collections import defaultdict


def generar_partido(equipo1, equipo2):
    ganador = equipo1 if equipo1 < equipo2 else equipo2
    perdedor = equipo2 if equipo1 < equipo2 else equipo1
    return ganador, 1, perdedor, 0


def evaluar_enfrentamientos_posibles(enfrentamientos_restantes, equipos_jugando_hoy):
    res = []
    for enfrentamiento in enfrentamientos_restantes:
        if (not enfrentamiento[0] in equipos_jugando_hoy) and (not enfrentamiento[1] in equipos_jugando_hoy):
            res.append(enfrentamiento)
    return res


def generar_fecha(cant_equipos, fecha, enfrentamientos_restantes):
    enfrentamientos_fecha = []
    equipos_jugando_hoy = []
    enfrentamientos_posibles_hoy = evaluar_enfrentamientos_posibles(enfrentamientos_restantes, equipos_jugando_hoy)
    while len(enfrentamientos_posibles_hoy) > 0:
        enfrentamiento_nuevo = random.choice(enfrentamientos_posibles_hoy)
        equipos_jugando_hoy.append(enfrentamiento_nuevo[0])
        equipos_jugando_hoy.append(enfrentamiento_nuevo[1])
        enfrentamientos_restantes.remove(enfrentamiento_nuevo)
        enfrentamientos_fecha.append(enfrentamiento_nuevo)
        enfrentamientos_posibles_hoy = evaluar_enfrentamientos_posibles(enfrentamientos_restantes, equipos_jugando_hoy)
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
    resultados = defaultdict(list)
    for fecha, enfrentamientos in enumerate(fechas):
        for enfrentamiento in enfrentamientos:
            ganador, goles_ganador, perdedor, goles_perdedor = resultado = generar_partido(enfrentamiento[0], enfrentamiento[1])
            resultados[fecha].append(resultado)
            print(fecha+1, ganador, goles_ganador, perdedor, goles_perdedor, file=f_output)
    return len(enfrentamientos_restantes) == 0, resultados


def generar_archivos_input(cant_equipos):
    cant_fechas = 0
    fin = False
    while not fin:
        cant_fechas += 1
        random.seed(1)
        with open("exp/exp5/fecha%s.txt" % cant_fechas, "w") as f_output:
            fin, resultados_por_fecha = generar_fixture_equipos(cant_equipos, cant_fechas, f_output)
    return resultados_por_fecha


def generar_archivos_test(cant_fechas_totales, c):
    for cant_fechas in range(1, cant_fechas_totales+1):
        with open("exp/exp5/test%s.in" % cant_fechas, "w") as f_output:
            print("0 %.2f 1 exp/exp5/fecha%s.txt 0.00001" % (c, cant_fechas), file=f_output)


def ejecutar_pagerank(cant_fechas_totales):
    for cant_fechas in range(1, cant_fechas_totales+1):
        call(["../bin/tp2", "exp/exp5/test%s.in" % cant_fechas, "exp/exp5/test%s.out" % cant_fechas])


def parsear_salida_pagerank(cant_fechas_totales):
    resultado_por_cant_fechas = []
    for cant_fechas in range(1, cant_fechas_totales+1):
        with open("exp/exp5/test%s.out" % cant_fechas) as f_resultado:
            resultado_por_cant_fechas.append(f_resultado.readlines())
    res = {}
    for cant_fechas, resultado_cant_fechas in enumerate(resultado_por_cant_fechas):
        res[cant_fechas+1] = [float(line) for line in resultado_cant_fechas]
    return ordenar_por_puntaje(res)


def puntaje_estandar(resultados_por_fecha, cant_equipos):
    res = {}
    res[1] = [0 for _ in range(cant_equipos)]
    for fecha, resultados in resultados_por_fecha.items():
        fecha += 1  # ajuste de índices
        if fecha > 1:
            res[fecha] = res[fecha-1][:]
        for ganador, _, perdedor, _ in resultados:
            ganador -= 1
            perdedor -= 1
            res[fecha][ganador] += 3
    return ordenar_por_puntaje(res)


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


def diferencia_con_ideal(orden_por_fecha):
    res = defaultdict(int)
    for cant_fechas, orden in orden_por_fecha.items():
        res[cant_fechas] = 0
        # sumo la distancia por cada jugador a su posición ideal 
        for posicion, jugador in enumerate(orden):
            res[cant_fechas] += abs(jugador - (posicion + 1))
    return res


def imprimir_salida(cant_equipos, c, diferencias):
    with open("exp/exp5/resultados/%sequipos%.2fc.res" % (cant_equipos, c), "w") as f_output:
        for cant_fechas, diferencia in diferencias.items():
            print(cant_fechas, diferencia, file=f_output)


def plotear_salida(cant_equipos, c, diferencias_pagerank, diferencias_estandar):
    import matplotlib.pyplot as plt
    plt.plot(list(diferencias_pagerank.values()), label="Método GeM (c=%.2f)" % c)
    plt.plot(list(diferencias_estandar.values()), label="Método estándar")
    plt.ylabel("Diferencia con el ranking ideal")
    plt.xlabel("Cantidad de fechas consideradas")
    plt.legend()
    plt.savefig("exp/exp5/resultados/%sequipos%.2fc.png" % (cant_equipos, c))


def main():
    if len(sys.argv) < 2:
        print("Uso:", sys.argv[0], "cantidad_de_equipos", "[c=0.85]")
        exit(-1)
    cant_equipos = int(sys.argv[1])
    if len(sys.argv) > 2:
        c = float(sys.argv[2])
    else:
        c = 0.85
    
    resultados_por_fecha = generar_archivos_input(cant_equipos)
    cant_fechas_totales = len(resultados_por_fecha)
    generar_archivos_test(cant_fechas_totales, c)
    
    ejecutar_pagerank(cant_fechas_totales)
    
    orden_por_fecha_pagerank = parsear_salida_pagerank(cant_fechas_totales)
    diferencias_pagerank = diferencia_con_ideal(orden_por_fecha_pagerank)
    
    orden_por_fecha_estandar = puntaje_estandar(resultados_por_fecha, cant_equipos)
    diferencias_estandar = diferencia_con_ideal(orden_por_fecha_estandar)

    imprimir_salida(cant_equipos, c, diferencias_pagerank)
    plotear_salida(cant_equipos, c, diferencias_pagerank, diferencias_estandar)


if __name__ == "__main__":
    main()