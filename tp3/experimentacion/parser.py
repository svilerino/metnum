#!/usr/bin/python3
import os
import sys

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
    experimento = tipo_video, metodo, cant_frames_generados, tam_bloque
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
    datos = {}
    for root, subfolders, filenames in os.walk(path):
        for filename in [filename for filename in filenames if 'txt' in filename]:
            print("Parseando", filename)
            experimento, resultados = parsear(os.path.join(root, filename))
            datos[experimento] = resultados
    print(datos)
    for experimento, resultados in datos.items():
        # do stuff
        pass

def main():
    if len(sys.argv) > 1:
        path = sys.argv[2]
        recorer_y_parsear_todo(path)
    else:
        recorer_y_parsear_todo()

if __name__ == '__main__':
    main()