#include <iostream>
#include <fstream>
#include <types.hpp>
#include <slowmotion.hpp>
#include <timing.hpp>
#include <opencv_util.hpp>
#include <cassert>

#define CANT_ITERS_MEDICION 1

using namespace std;

int main(int argc, char** argv) {
    if(argc < 5)
    {

        cerr << "Los parámetros de entrada no son los correctos." << endl;
        cerr << "El primer parametro debe ser el path al archivo de entrada(un video)." << endl;
        cerr << "El segundo parámetro debe ser el path al archivo de salida(un video)." << endl;
        
        cerr << "El tercer parámetro debe ser el método a ejecutar." << endl;
        cerr << "\t0.- Vecino mas cercano" << endl;
        cerr << "\t1.- Interpolacion lineal" << endl;
        cerr << "\t2.- Splines" << endl;

        cerr << "El cuarto parámetro debe ser la cantidad k de frames a agregar entre cada par de frames [i, i+1] del video original." << endl;
        exit(-1);

    } else {
        const char* input_filename = argv[1];
        const char* output_filename = argv[2];
        
        interpolation_method_t interp_method = (interpolation_method_t) atoi(argv[3]);
        assert(interp_method < 3);
        
        uint interpol_frame_count = atoi(argv[4]);

        assert(interpol_frame_count >= 0);

        // -- Leer input
        Video video_input;
        Video video_output;

        cout << "Reading input video..." << endl;
        FileToVideo(input_filename, video_input);
        cout << "Done." << endl << endl;

        cout << "Input video metadata:" << endl;
        cout << "frame_count: " << video_input.frame_count << endl;
        cout << "frame_height: " << video_input.frame_height << endl;
        cout << "frame_width: " << video_input.frame_width << endl;
        cout << "frame_rate: " << video_input.frame_rate << endl;
        cout << "------------------------------------- " << endl << endl;

        // -- Procesamiento

        cout << "Aplicando efecto slowmotion al video utilizando el metodo de interpolacion: ";
        if (interp_method == NEAREST_NEIGHBOUR){
            cout << "NEAREST_NEIGHBOUR" << endl;
        } else if (interp_method == LINEAL){
            cout << "LINEAL" << endl;
        } else if (interp_method == SPLINE){
            cout << "SPLINE" << endl;
        }


        double tiempo_promedio = 0.0;
        MEDIR_TIEMPO_PROMEDIO(
            SlowMotionEffect::slowmotion(interp_method, interpol_frame_count, video_input, video_output);
        , CANT_ITERS_MEDICION, &tiempo_promedio);

        cout << "Microsegundos consumidos para procesar el video de input(sin contar tiempo de I/O): " << tiempo_promedio << endl;

        // -- Escribir output
        cout << endl << "------------------------------------- " << endl << endl;
        cout << "Writing output video..." << endl;

        VideoToFile(output_filename, video_output);

        cout << "Done." << endl << endl;
        cout << "Output video metadata:" << endl;
        cout << "frame_count: " << video_output.frame_count << endl;
        cout << "frame_height: " << video_output.frame_height << endl;
        cout << "frame_width: " << video_output.frame_width << endl;
        cout << "frame_rate: " << video_output.frame_rate << endl;
        cout << "------------------------------------- " << endl << endl;

        return 0;
    }
}
