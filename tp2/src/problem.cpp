#include <algorithm>
#include <cassert>
#include <iostream>
#include <functional>
#include <chrono>

#include <timing.hpp>
#include <io.hpp>
#include <problem.hpp>
#include <csr_matrix.tpp>

#define CANT_ITERS_MEDICION 25

typedef unsigned int uint;

Problem::Problem(const problem_arguments& args) : csr_ptr(), in_deg_ptr(), args(args)
{
    std::ifstream input_file(args.input_file_path,std::ifstream::in);
    if (!input_file.is_open()) {
        std::cerr << "Path de archivo de entrada invalido: " << args.input_file_path << std::endl;
        exit(-1);

    } else if(!args.is_pagerank && args.is_deportes) {
        std::cerr << "El Metodo IN-DEG solo se puede utilizar con instancias de tipo \"Páginas Web\"" << std::endl;
        exit(-1);

    } else if(args.is_pagerank){
        csr_ptr = read_args_from_stream_pagerank(input_file,args);
        input_file.close();
        std::cout << "CSR: " << std::endl << *csr_ptr << std::endl;
        csr_ptr->print_sparse(std::cout);

    } else { //es in-deg
        in_deg_ptr = read_args_from_stream_indeg(input_file,args);
    };
}

Problem::~Problem(){if(args.is_pagerank) delete csr_ptr; else delete in_deg_ptr;};

void Problem::resolver_instancia() {
    std::ofstream output_file(args.output_file_path,std::ofstream::out);
    if(!output_file.is_open())
    {
        std::cerr << "Error al tratar de abrir el archivo de salida para escritura. Verifique la ruta y permisos: "
            << args.output_file_path << std::endl;
        exit(-1);

    } else {
        if(!args.is_pagerank) {
            //IN-DEG
            std::sort(in_deg_ptr->begin(),in_deg_ptr->end(),std::greater<std::pair<uint,uint> >());
            imprimir_en_linea(output_file, *in_deg_ptr);

        } else {
            //PAGERANK

            // --------------------------- Armo el vector inicial ---------------------------

            std::vector<double> initial_vec;

            if(args.random_initial_vector){
                // ---------- Random normalized initial vector -------------------
                unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                std::default_random_engine generator (seed);

                std::uniform_real_distribution<double> distribution(0.0, 1.0);

                for (uint i = 0; i < csr_ptr->cols(); ++i)
                {
                    double number = distribution(generator);
                    std::cout << number << std::endl;
                    initial_vec.push_back(number);
                }
                double norma = norma1(initial_vec, true);
                initial_vec/=norma; // Normalizo

            }else{
                double equiprob = 1/(double)csr_ptr->cols();
                for (uint i = 0; i < csr_ptr->cols(); ++i)
                {
                    initial_vec.push_back(equiprob);
                }
            }

            // --------------------------- Inicializo los parametros de power method ---------------------------

            std::vector<double> res;
            power_method_stop_criteria_t criterio_parada;


            criterio_parada.intervalo_iters_reporte = 1; // Flushear info de convergencia cada x iters(si es potencia de 2 el compilador puede hacer magia performante)
            criterio_parada.criterio = args.power_method_mode;


            if(criterio_parada.criterio == CRT_K_FIXED_ITERS_LIMIT){

                criterio_parada.valor.cant_iters = (uint)args.epsilon;

            }else if(criterio_parada.criterio == CRT_K_ITERS_DELTA_DIFF){

                criterio_parada.valor.delta_diff = args.epsilon;

            }else if (criterio_parada.criterio == CRT_K_ITERS_NO_DIFF){
                assert(false && "CRT_K_ITERS_NO_DIFF sin implementar.");
            } else {
                assert(false && "Criterio de parada erroneo en power method.");
            }


            // --------------------------- Abro los streams de info ---------------------------

            std::ofstream reporte_power_method(args.pm_reporte_path);

            if (!reporte_power_method.is_open()) {
                std::cerr << "Imposible escribir en archivo de convergencia de power method: " << args.pm_reporte_path << std::endl;
                exit(-1);
            }

            reporte_power_method.precision(15);
            reporte_power_method.setf(std::ios::fixed,std::ios::floatfield);


            std::ofstream timing_power_method(args.timing_path);
            if (!timing_power_method.is_open()) {
                std::cerr << "Imposible escribir en archivo de timing: " << args.timing_path << std::endl;
                exit(-1);
            }

            timing_power_method.precision(5);
            timing_power_method.setf(std::ios::fixed,std::ios::floatfield);

            // ----------- Corro power method CANT_ITERS_MEDICION veces sin imprimir nada para medir tiempo---------

            double promedio_tiempo = 0.0;

            MEDIR_TIEMPO_PROMEDIO(
                csr_ptr->power_method(initial_vec, args.c, criterio_parada, res, reporte_power_method, false/*no imprimir info*/);
            , CANT_ITERS_MEDICION, &promedio_tiempo);

            // --------------------------- Corro power method una vez mas para imprimir convergencia ---------------------------

            csr_ptr->power_method(initial_vec, args.c, criterio_parada, res, reporte_power_method, true /*imprimir info convergencia*/);

            // --------------------------- Imprimo info y cierro streams ---------------------------

            timing_power_method << args << std::endl;
            timing_power_method<< "Cantidad repeticiones medicion power_method: " << CANT_ITERS_MEDICION << std::endl;
            timing_power_method<< "Promedio tiempo microsegundos power_method: " << promedio_tiempo << std::endl;

            reporte_power_method.close(); // Seguro esta abierto, sino hubiera ejecutado el exit(-1) en el check cuando lo abro;
            timing_power_method.close(); // Seguro esta abierto, sino hubiera ejecutado el exit(-1) en el check cuando lo abro;

            imprimir_en_linea(output_file, res);
        };
    };
    output_file.close();
};
