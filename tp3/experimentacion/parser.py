#!/usr/bin/python3
import os
import sys
#import numpy as np
#import matplotlib.pyplot as plt
from collections import defaultdict, OrderedDict


def parsear_filename(filename):
    filename = filename[:-4]    # saco la extensión
    if "white_black" in filename:
        tipo_video = "Blanco a negro"
    else:
        if "camara_fija" in filename:
            tipo_video = "Cámara fija"
        elif "camara_movil" in filename:
            tipo_video = "Cámara móvil"
        else:
            raise ValueError
        tipo_video += " + "
        if "imagen_fija" in filename:
            tipo_video += "Imagen fija"
        elif "imagen_movil" in filename:
            tipo_video += "Imagen móvil"
        else:
            raise ValueError
    if "laboratorio" in filename:
        tipo_video += " (laboratorio)"
    if "resolucion_original" in filename:
        tipo_video += " (resolución original)"
    tam_bloque = None
    if "LINEAL" in filename:
        metodo = "Interpolación lineal"
    elif "SPLINE" in filename:
        metodo = "Interpolación por splines"
        if "blk" in filename:
            tam_bloque = int(filename[filename.rfind("k")+1:])
            filename = filename[:filename.rfind("-blk")]
        else:
            tam_bloque = "Todo"
    elif "VECINO" in filename:
        metodo = "Vecino más cercano"
    else:
        raise ValueError
    cant_frames_generados = int(filename[filename.rfind("k")+1:])
    experimento = (tipo_video, metodo, cant_frames_generados, tam_bloque)
    return experimento

def parsear(filename):
    experimento = parsear_filename(filename)
    with open(filename) as file_in:
        for linea in file_in:
            if "milisegundos" in linea:
                tiempo_total = float(linea.split()[-2])
            elif "frame_count" in linea:
                frame_count = int(linea.split()[-1])
            elif "frame_height" in linea:
                frame_height = int(linea.split()[-1])
            elif "frame_rate" in linea:
                frame_rate = float(linea.split()[-1]) 
    resultados = (tiempo_total, frame_count, frame_height, frame_rate)
    return experimento, resultados

def recorer_y_parsear_todo(path='.'):
    datos = OrderedDict()
    for root, subfolders, filenames in os.walk(path):
        for filename in [filename for filename in filenames if 'txt' in filename]:
            print("Parseando", filename)
            experimento, resultados = parsear(os.path.join(root, filename))
            datos[experimento] = resultados
    return datos

def procesar(datos):
    tiempos_por_tipo_video, tiempos_por_metodo = defaultdict(dict), defaultdict(dict)
    for experimento, resultados in datos.items():
        tipo_video, metodo, cant_frames_generados, tam_bloque = experimento
        tiempo_total, frame_count, frame_height, frame_rate = resultados
        if "spline" in metodo:
            metodo += " (blk = " + str(tam_bloque) + ")"
        if cant_frames_generados == 10:
            tiempos_por_tipo_video[tipo_video][metodo] = tiempo_total
        if "lineal" in metodo:
            print(abreviar(tipo_video), metodo, cant_frames_generados, str(tiempo_total).replace(".",","), sep="-")
        if cant_frames_generados == 10 and not "original" in tipo_video and not "lab" in tipo_video:
            print(abreviar(tipo_video), metodo, frame_count, str(tiempo_total).replace(".",","), sep="-")
    return tiempos_por_tipo_video

def plot_dict(dicc):
    for tipo_video in dicc.keys():
        tiempos_por_metodo = dicc[tipo_video]
        plt.bar(range(len(tiempos_por_metodo)), tiempos_por_metodo.values(), align='center')
        plt.xticks(range(len(tiempos_por_metodo)), list(tiempos_por_metodo.keys()))
        plt.show()

def abreviar(tipo_video):
    return tipo_video.replace("Cámara", "C").replace("Imagen", "I").replace("móvil", "M").replace("fija", "F").replace("(laboratorio","(lab)").replace("Blanco a negro", "BN").replace(" ","")

def main():
    if len(sys.argv) > 1:
        path = sys.argv[2]
        datos = recorer_y_parsear_todo(path)
    else:
        datos = recorer_y_parsear_todo()
    tiempos_por_tipo_video = procesar(datos)
    for tipo_video, metodos in tiempos_por_tipo_video.items():
        print(abreviar(tipo_video), end="-")
        sorted_dict = sorted(metodos.items(), key=lambda item: item[0])
        ([print(elem[0],end="-") for elem in sorted_dict])
        for metodo, tiempo in sorted_dict:
            print(str(tiempo).replace(".",","), end="-")
        print()

    #plotear(datos)

if __name__ == '__main__':
    main()