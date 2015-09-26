#include <vector>
#include <iostream>
#include <cstring>
#include <matriz.hpp>
#include <io.hpp>
#include <problem.hpp>

using namespace std;

int main(int argc, char** argv) {

// Comento todo esto para que no chille con unused variables el make


//    bool is_pagerank = 1-atoi(argv[1]); // conversion nasty a bool negado con el 1-x (viene not pagerank en el bool)
//    double prob_teletransportation_c = strtod(argv[2]);
//    bool is_deportes = atoi(argv[3]);
//    std::string path_file_in = argv[4];
//    double pow_method_tolerance = strtod(argv[5]);
//    string path_timings_out = (argc > 6) ? argv[6] : "timing_results.txt";    
//
//    ProblemArguments in_arg;
//    ifstream input_file(path_file_in);
//    if (input_file.is_open()) {
//        read_args_from_stream(input_file, in_arg);
//        input_file.close();
//    } else {
//        cerr << "Path de archivo de entrada invalido: " << path_file_in << endl;
//        exit(-1);
//    }
//
//    
//    //mostrar_datos_entrada(in_arg, cout);
//
//    Problem problem(in_arg);
//    Results output_results;
//
//    // Archivo que guarda datos de mediciones
//    ofstream timing_file(path_timings_out);
//    if (!timing_file.is_open()) {
//        cerr << "Imposible escribir en archivo de timings: " << path_timings_out << endl;
//        exit(-1);
//    }
//
//    problem.resolver_instancias(output_results, timing_file);
//
//    timing_file.close(); // Seguro esta abierto, sino hubiera ejecutado el exit(-1);
//
//    // Escribir resultados
//    ofstream output_file(path_file_out);
//    if (output_file.is_open()) {
//        write_results_to_stream(output_file, output_results);
//        output_file.close();
//    } else {
//        cerr << "Imposible escribir en archivo de salida: " << path_file_out << endl;
//        exit(-1);
//    }
//
    return 0;
}
