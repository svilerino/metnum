#include <algorithm>
#include <cassert>
#include <iostream>
#include <functional>

#include <io.hpp>
#include <problem.hpp>
#include <csr_matrix.tpp>

typedef unsigned int uint;

Problem::Problem(const problem_arguments& args) : csr_ptr(), in_deg_ptr(), is_pagerank(args.is_pagerank)
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
        std::sort(in_deg_ptr->begin(),in_deg_ptr->end(),std::greater<std::pair<uint,uint> >());
        std::cout << *in_deg_ptr << std::endl;
    };
}

Problem::~Problem(){if(is_pagerank) delete csr_ptr; else delete in_deg_ptr;};

void Problem::resolver_instancias(Results &output, std::ostream &timing_result_os) {

}
