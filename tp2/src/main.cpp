#include <iostream>
#include <vector>
#include <string>

#include <io.hpp>
#include <problem.hpp>
#include <csr_matrix.tpp>

typedef unsigned int uint;

int main(int argc, char** argv) {
    if(argc < 3)
    {
        std::cerr << "Los parámetros de entrada no son los correctos." << std::endl;
        std::cerr << "El primer parametro debe ser un archivo con una linea conteniendo los siguientes valores: " << std::endl;
        std::cerr << "\tmetodo prob_teletransportacion instancia archivo_entrada tolerancia" << std::endl;
        std::cerr << "\tmetodo: 0 para pagerank | 1 para alternativa." << std::endl;
        std::cerr << "\tinstancia: 0 para páginas web | 1 para deportes." << std::endl;
        std::cerr << "El segundo parámetro debe ser un archivo de salida." << std::endl;


        std::cerr << std::endl << "Opcionalmente se pueden pasar mas argumentos, de no pasarlos, se asumira su valor por default." << std::endl;


        std::cerr << "El tercer parámetro indica el metodo de corte del metodo de la potencia." << std::endl;
        
        std::cerr << "\t0 - Cantidad de iteraciones fijas. En este caso el valor <tolerancia> del archivo de input se considerara un entero sin signo expresando la cantidad de iters." << std::endl;
        std::cerr << "\t1 - Iterar hasta diferencia menor a <tolerancia>" << std::endl;

        std::cerr << "Por default se considera el caso 1 para legacy con la catedra." << std::endl << std::endl;

        std::cerr << "El cuarto parametro indica como será el vector inicial del método de la potencia" << std::endl;

        std::cerr << "\t 0 - Vector equiprobable de Rn - (1/n, ..., 1/n)" << std::endl;
        std::cerr << "\t 1 - Vector aleatorio de Rn - X = (x1, ..., xn) normalizado tal que norma1(X) = 1" << std::endl;

        std::cerr << "Por default se considera el caso 0." << std::endl << std::endl;

        std::cerr << "El quinto parametro indica el path de salida de la informacion de convergencia del metodo de la potencia." << std::endl;
        std::cerr << "Por default se considera el caso mismo path y nombre del archivo de salida pero con extension .convergence" << std::endl << std::endl;

        std::cerr << "El sexto parametro indica el path de salida de la medicion de tiempo del metodo de la potencia." << std::endl;
        std::cerr << "Por default se considera el caso mismo path y nombre del archivo de salida pero con extension .timing" << std::endl << std::endl;

        return -1;

    } else {
        problem_arguments args(argv, argc);
        //std::cout << args << std::endl;
        Problem problem(args);
        problem.resolver_instancia();

        return 0;
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
    return 0;
}
