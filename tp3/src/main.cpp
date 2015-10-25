#include <iostream>
#include <fstream>
#include <io.hpp>
#include <slowmotion.hpp>
#include <timing.hpp>
#include <cassert>

#define CANT_ITERS_MEDICION 1

using namespace std;

int main(int argc, char** argv) {
    if(argc < 5)
    {

        cerr << "Los parámetros de entrada no son los correctos." << endl;
        cerr << "El primer parametro debe ser el path al archivo de entrada." << endl;
        cerr << "El segundo parámetro debe ser el path al archivo de salida." << endl;
        cerr << "\tEl formato de los archivos de entrada y salida son como se indica a continuacion" << endl;

        cerr << "\tLa primera linea tiene la cantidad de frames que tiene el video original." << endl;
        cerr << "\tLa segunda linea tiene la dimensión del frame en píxeles <alto> <ancho>." << endl;
        cerr << "\tLa tercer linea tiene el framerate(frames/sec) del video." << endl;
        cerr << "\tLas siguientes <alto> lineas son las filas del primer frame del video, conteniendo cada una, <ancho> numeros indicando el valor de cada pixel de dicha fila. " << endl;
        cerr << "\tLe siguen analogamente el resto de los frames del video de entrada. " << endl;


        cerr << "El tercer parámetro debe ser el método a ejecutar." << endl;
        cerr << "\t0.- Vecino mas cercano" << endl;
        cerr << "\t1.- Interpolacion lineal" << endl;
        cerr << "\t2.- Splines" << endl;

        cerr << "El cuarto parámetro debe ser la cantidad k de frames a agregar entre cada par de frames [i, i+1] del video original." << endl;
        exit(-1);

    } else {
        const string& input_filename = argv[1];
        const string& output_filename = argv[2];
        
        interpolation_method_t interp_method = (interpolation_method_t) atoi(argv[3]);
        assert(interp_method < 3);
        
        uint interpol_frame_count = atoi(argv[4]);

        assert(interpol_frame_count >= 0);

        // -- Leer input
        ifstream is(input_filename);
        if (!is.is_open())
        {
            cerr << "Path de archivo de entrada invalido: " << input_filename << endl;
            exit(-1);
        }

        Video video_input;
        Video video_output;
        read_video_from_args(is, video_input);

        // -- Procesamiento
        double tiempo_promedio = 0.0;
        MEDIR_TIEMPO_PROMEDIO(
            SlowMotionEffect::interpolate(interp_method, interpol_frame_count, video_input, video_output);
        , CANT_ITERS_MEDICION, &tiempo_promedio);

        // -- Escribir output
        ofstream os(output_filename);
        if (!os.is_open())
        {
            cerr << "Path de archivo de salida invalido: " << output_filename << endl;
            exit(-1);
        }

        write_video_to_stream(os, video_output);
        return 0;
    };
}
