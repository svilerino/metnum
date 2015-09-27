#ifndef IO_H
#define IO_H

#include <iostream>
#include <string>
#include <vector>
#include <csr_matrix.tpp>

typedef struct problem_arguments
{
    bool is_pagerank,is_deportes;
    double c,epsilon;
    std::string input_file_path;
} problem_arguments;

typedef struct Results {

} Results;

std::ostream& operator<<(std::ostream& os, const problem_arguments& args);

CSR<double>* read_args_from_stream_pagerank(std::istream& is, const problem_arguments& args);
std::vector<std::pair<uint,uint> >* read_args_from_stream_indeg(std::istream& is, const problem_arguments& args);
void write_results_to_stream(std::ostream &os, Results output_results);

#endif
