#include <iostream>
#include <vector>
#include <string>

#include <io.hpp>
#include <problem.hpp>
#include <csr_matrix.tpp>

typedef unsigned int uint;

int main(int argc, char** argv) {
    int result = 0;

    if(argc != 6)
    {
        std::cerr << "Los parámetros de entrada no son los correctos." << std::endl;
        std::cerr << argv[0] << " metodo prob_teletransportacion instancia archivo_entrada tolerancia" << std::endl;
        std::cerr << "metodo: 0 para pagerank | 1 para alternativa." << std::endl;
        std::cerr << "instancia: 0 para páginas web | 1 para deportes." << std::endl;

        result = 1;

    } else {
        problem_arguments args;
        args.is_pagerank = !(bool)std::stoi(argv[1]); // conversion nasty a bool negado con el 1-x (viene not pagerank en el bool)
        args.c = std::stod(argv[2],NULL);
        args.is_deportes = (bool)std::stoi(argv[3]);
        args.input_file_path = argv[4];
        args.epsilon = std::stod(argv[5],NULL);

        Problem problem(args);
    };
//    string path_timings_out = (argc > 6) ? argv[6] : "timing_results.txt";
//
//    ProblemArguments in_arg;
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
    return result;
}
