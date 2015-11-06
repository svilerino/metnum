#include <iostream>
#include <fstream>
#include <types.hpp>
#include <slowmotion.hpp>
#include <timing.hpp>
#include <interpolation.hpp>
#include <opencv_util.hpp>
#include <cassert>

#define CANT_ITERS_MEDICION 1
#define DEFAULT_FRAME_DROP 0
#define DEFAULT_SPLINE_BLOCK_SIZE 16

using namespace std;

uint calcular_frames_a_descartar(uint video_frame_count, uint frame_drop)
{
    uint frames_a_descartar = video_frame_count % (frame_drop+1);
    if(frames_a_descartar == 0)
    {
        frames_a_descartar = frame_drop;
    }
    else
    {
        frames_a_descartar --;
    }
    return frames_a_descartar;
}

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
        cerr << "(Opcional) El sexto parámetro debe ser la cantidad de frames a skipear en la lectura del video. En caso de no pasarse este argumento se tomara como valor por defecto el numero " << DEFAULT_FRAME_DROP << endl;
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

        const uint frame_drop = (argc > 6) ? atoi(argv[6]) : interpol_frame_count;
        //const uint frame_drop = (argc > 6) ? atoi(argv[6]) : DEFAULT_FRAME_DROP;

        // -- Leer input completo(sin frame_drop)

        Video video_input;
        Video video_output;

        cout << "Reading input video: " << input_path << endl;
        FileToVideo(input_path.c_str(), video_input, "", false);

        // -- Pre-Procesamiento (Hacer cantidad de frames apropiada a los videos a skipear)

        uint ultimos_frames_a_quitar = calcular_frames_a_descartar(video_input.frame_count, frame_drop);

        cout << endl << "[Warning] Se eliminan los ultimos " << ultimos_frames_a_quitar << " frames del video por cuestiones de redondeo" << endl;
        video_input.frames.erase(video_input.frames.end() - ultimos_frames_a_quitar, video_input.frames.end());
        video_input.frame_count = video_input.frames.size();

        // -- Informacion del video de entrada luego de ser procesado

        cout << "Input video metadata:" << endl;
        cout << "frame_count: " << video_input.frame_count << endl;
        cout << "frame_height: " << video_input.frame_height << endl;
        cout << "frame_width: " << video_input.frame_width << endl;
        cout << "frame_rate: " << video_input.frame_rate << endl;
        cout << "------------------------------------- " << endl << endl;

        // -- Guardar video original(con ultimos frames descartados adecuadamente) en escala de grises

        string input_directory = input_path.substr(0, input_path.find_last_of("/"));
        string input_filename = input_path.substr(input_path.find_last_of("/"), input_path.length());

        string input_filename_no_extension = input_filename.substr(0, input_filename.find_last_of("."));
        string input_extension = input_filename.substr(input_filename.find_last_of("."), input_filename.length());

        string grayscale_output_path = input_directory + input_filename_no_extension + ".grayscale.avi";

        cout << "Guardando video original(con ultimos frames descartados adecuadamente) en escala de grises en: " << grayscale_output_path << "..." << endl;
        VideoToFile(grayscale_output_path.c_str(), video_input, "", false);
        cout << "------------------------------------- " << endl << endl;

        // -- Pre-Procesamiento (Skipear frames)
        // Se puede hacer mejor borrando directo de video_input.frames en lugar de copiando lo que voy a dejar,
        // pero hay que tener cuidado con los iteradores y su invalidacion cada vez que se borra un elemento
        vector<frame_t> skipped_frames;

        for (uint cur_frame = 0; cur_frame < video_input.frame_count; cur_frame+= (frame_drop+1))
        {
            skipped_frames.push_back(video_input.frames[cur_frame]);
        }

        video_input.frames = skipped_frames;
        video_input.frame_count = skipped_frames.size();

        // -- Procesamiento (Aplicar slowmotion)

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
        cout << "Writing output video: " << output_path << endl;

        VideoToFile(output_path.c_str(), video_output, "", false);

        cout << "Output video metadata:" << endl;
        cout << "frame_count: " << video_output.frame_count << endl;
        cout << "frame_height: " << video_output.frame_height << endl;
        cout << "frame_width: " << video_output.frame_width << endl;
        cout << "frame_rate: " << video_output.frame_rate << endl;
        cout << "------------------------------------- " << endl << endl;

        return 0;
    }
}
