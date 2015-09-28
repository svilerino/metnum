#include <algorithm>
#include <cassert>
#include <iostream>
#include <functional>

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
            std::vector<double> initial_vec(csr_ptr->cols(), 1/(double)csr_ptr->cols());
            std::vector<double> res;
            std::string path_reporte_power_method = "power_method_reporte.txt";

            power_method_stop_criteria_t criterio_parada;
            criterio_parada.criterio = CRT_K_ITERS_DELTA_DIFF;
            criterio_parada.valor.delta_diff = args.epsilon;
            criterio_parada.intervalo_iters_reporte = 1;

            std::ofstream reporte_power_method(path_reporte_power_method);

            if (!reporte_power_method.is_open()) {
                std::cerr << "Imposible escribir en archivo de reporte de power method: " << path_reporte_power_method << std::endl;
                exit(-1);
            }

            reporte_power_method.precision(15);
            reporte_power_method.setf(std::ios::fixed,std::ios::floatfield);

            csr_ptr->power_method(initial_vec, args.c, criterio_parada, res, reporte_power_method);

            reporte_power_method.close(); // Seguro esta abierto, sino hubiera ejecutado el exit(-1) en el check cuando lo abro;


            /*
            std::string extracted = args.input_file_path.substr(0, args.input_file_path.find_last_of("\\."));

            std::string path_result_stream = extracted + ".out";
            std::ofstream result_stream(path_result_stream);

            if (!result_stream.is_open()) {
                std::cerr << "Imposible escribir en archivo de salida: " << path_result_stream << std::endl;
                exit(-1);
            }

            for(double val : res)
            {
                result_stream << val << std::endl;
            }

            result_stream.close(); // Seguro esta abierto, sino hubiera ejecutado el exit(-1) en el check cuando lo abro;
            */            
            imprimir_en_linea(output_file, res);
        };
    };
    output_file.close();
};
