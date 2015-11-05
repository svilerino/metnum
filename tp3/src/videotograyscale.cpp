#include <iostream>
#include <fstream>
#include <types.hpp>
#include <slowmotion.hpp>
#include <timing.hpp>
#include <interpolation.hpp>
#include <opencv_util.hpp>
#include <cassert>

using namespace std;

int main(int argc, char** argv) {
    if(argc < 3)
    {

        cerr << "Los parámetros de entrada no son los correctos." << endl;
        cerr << "El primer parametro debe ser el path al archivo de entrada(un video)." << endl;
        cerr << "El segundo parámetro debe ser el path al archivo de salida(un video)." << endl;
        exit(-1);

    } else {
        string input_path = argv[1];
        string output_path = argv[2];

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

        cout << "Guardando video original en escala de grises en: " << output_path << "...";
        VideoToFile(output_path.c_str(), video_input, "", false);
        cout << "Ok!" << endl;
        cout << "------------------------------------- " << endl << endl;        

        return 0;
    }
}
