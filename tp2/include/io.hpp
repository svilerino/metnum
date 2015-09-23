#ifndef IO_H
#define IO_H

#include<stdlib.h>
#include<vector>
#include<cstring>
#include<fstream>
#include<iostream>
#include<matriz.hpp>

using namespace std;

typedef struct ProblemArguments {

} ProblemArguments;

typedef struct Results {

} Results;

void mostrar_datos_entrada(ProblemArguments args, ostream &os);
void read_args_from_stream(istream &is, ProblemArguments& args);
void write_results_to_stream(ostream &os, Results output_results);

#endif
