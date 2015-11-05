#include <iostream>
#include <fstream>
#include <types.hpp>
#include <slowmotion.hpp>
#include <timing.hpp>
#include <interpolation.hpp>
#include <opencv_util.hpp>
#include <cassert>

#define CANT_ITERS_MEDICION 1
#define DEFAULT_SPLINE_BLOCK_SIZE 16

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

        cerr << "(Opcional) El quinto parámetro debe ser el tamaño del bloque de procesamiento de splines. En caso de no pasarse este argumento se tomara como valor por defecto el numero " << DEFAULT_SPLINE_BLOCK_SIZE << endl;
        exit(-1);

    } else {
        string input_path = argv[1];
        string output_path = argv[2];

        interpolation_method_t interp_method = (interpolation_method_t) atoi(argv[3]);
        assert(interp_method < 3);
        
        const uint interpol_frame_count = atoi(argv[4]);

        const uint spline_block_size = (argc > 5) ? atoi(argv[5]) : DEFAULT_SPLINE_BLOCK_SIZE;

        assert(interpol_frame_count >= 0);
        
        assert(spline_block_size >= 3);
        
        // Potencia de 2
        //assert(spline_block_size && ((spline_block_size & (spline_block_size-1)) == 0));

        // -- Leer input
        Video video_input;
        Video video_output;

        cout << "Reading input video..." << endl;
        FileToVideo(input_path.c_str(), video_input, "", false);
        cout << "Done." << endl << endl;

        cout << "Input video path: " << input_path << endl;
        cout << "Input video metadata:" << endl;
        cout << "frame_count: " << video_input.frame_count << endl;
        cout << "frame_height: " << video_input.frame_height << endl;
        cout << "frame_width: " << video_input.frame_width << endl;
        cout << "frame_rate: " << video_input.frame_rate << endl;
        cout << "------------------------------------- " << endl << endl;

        // -- Guardar video original en escala de grises
        string input_directory = input_path.substr(0, input_path.find_last_of("/"));
        string input_filename = input_path.substr(input_path.find_last_of("/"), input_path.length());
        
        string input_filename_no_extension = input_filename.substr(0, input_filename.find_last_of("."));
        string input_extension = input_filename.substr(input_filename.find_last_of("."), input_filename.length());

        string grayscale_output_path = input_directory + input_filename_no_extension + ".grayscale" + input_extension;

        cout << "Guardando video original en escala de grises en: " << grayscale_output_path << "...";
        VideoToFile(grayscale_output_path.c_str(), video_input, "", false);
        cout << "Ok!" << endl;
        cout << "------------------------------------- " << endl << endl;        

        // -- Procesamiento

        cout << "Aplicando efecto slowmotion al video utilizando el metodo de interpolacion: ";
        if (interp_method == NEAREST_NEIGHBOUR){
            cout << "NEAREST_NEIGHBOUR" << endl;
        } else if (interp_method == LINEAL){
            cout << "LINEAL" << endl;
        } else if (interp_method == SPLINE){
            cout << "SPLINE" << endl;
            cout << "Tamaño de bloque spline: " << spline_block_size << endl;
        }

        double tiempo_promedio = 0.0;
        MEDIR_TIEMPO_PROMEDIO(
            SlowMotionEffect::slowmotion(interp_method, interpol_frame_count, video_input, video_output, spline_block_size);
        , CANT_ITERS_MEDICION, &tiempo_promedio);

        cout << "Se corrio el metodo " << CANT_ITERS_MEDICION << " veces sobre el video de input." << endl;
        cout << "Tiempo consumido en promedio para procesar el video de input(sin contar tiempo de I/O): " << tiempo_promedio << " milisegundos" << endl;

        // -- Escribir output
        cout << endl << "------------------------------------- " << endl << endl;
        cout << "Writing output video..." << endl;

        VideoToFile(output_path.c_str(), video_output, "", false);

        cout << "Done." << endl << endl;
        cout << "Output video path: " << output_path << endl;
        cout << "Output video metadata:" << endl;
        cout << "frame_count: " << video_output.frame_count << endl;
        cout << "frame_height: " << video_output.frame_height << endl;
        cout << "frame_width: " << video_output.frame_width << endl;
        cout << "frame_rate: " << video_output.frame_rate << endl;
        cout << "------------------------------------- " << endl << endl;

        return 0;
    }
}
