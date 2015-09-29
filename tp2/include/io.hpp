#ifndef IO_H
#define IO_H

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <csr_matrix.tpp>

typedef struct problem_arguments
{
    bool is_pagerank,is_deportes;
    double c,epsilon;
    std::string input_file_path,output_file_path;

    //Adicionales
    criterio_terminacion_powermethod power_method_mode;//Default delta diff por catedra.
    bool random_initial_vector;
    std::string pm_reporte_path;
    std::string timing_path;

    problem_arguments(char** argv, int argc);
} problem_arguments;

typedef struct Results {

} Results;

std::ostream& operator<<(std::ostream& os, const problem_arguments& args);

CSR<double>* read_args_from_stream_pagerank(std::istream& is, const problem_arguments& args, const bool from_snap);
std::vector<std::pair<uint,uint> >* read_args_from_stream_indeg(std::istream& is, const problem_arguments& args);
void write_results_to_stream(std::ostream &os, Results output_results);

#endif
