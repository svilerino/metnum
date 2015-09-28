#include <algorithm>
#include <cassert>
#include <iostream>
#include <functional>
#include <chrono>

#include <io.hpp>
#include <problem.hpp>
#include <csr_matrix.tpp>

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
        //std::cout << "CSR: " << std::endl << *csr_ptr << std::endl;
        //csr_ptr->print_sparse(std::cout);

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

            std::vector<double> res;

            power_method_stop_criteria_t criterio_parada;
            criterio_parada.criterio = CRT_K_ITERS_DELTA_DIFF;
            criterio_parada.valor.delta_diff = args.epsilon;
            criterio_parada.intervalo_iters_reporte = 1;

            std::ofstream reporte_power_method(args.pm_reporte_path);

            if (!reporte_power_method.is_open()) {
                std::cerr << "Imposible escribir en archivo de reporte de power method: " << args.pm_reporte_path << std::endl;
                exit(-1);
            }

            reporte_power_method.precision(15);
            reporte_power_method.setf(std::ios::fixed,std::ios::floatfield);

            csr_ptr->power_method(initial_vec, args.c, criterio_parada, res, reporte_power_method);

            reporte_power_method.close(); // Seguro esta abierto, sino hubiera ejecutado el exit(-1) en el check cuando lo abro;
            imprimir_en_linea(output_file, res);
        };
    };
    output_file.close();
};
