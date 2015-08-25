#ifndef IO_H
#define IO_H

#include <stdlib.h>
#include <vector>
#include <cstring>
#include <fstream>
#include <iostream>
#include "matriz.h"

using namespace std;

typedef enum tipo_ejecucion {ELIM_GAUSSIANA, FACT_LU} tipo_ejecucion;

typedef struct ProblemArguments {
	int radio_interno;		// R_i
	int radio_externo;		// R_e
	int cantidad_radios;	// m
	int cantidad_titas;		// n
	double isoterma_buscada;
	int num_instancias;
	// Los siguientes vectores tienen num_instancias vectores con los parametros de cada instancia
	vector<vector<double> > instancias_temp_internas;	// T_i(k). Dimension de los vectores internos: n, uno por cada tita
	vector<vector<double> > instancias_temp_externas;	// T_e(k). Dimension de los vectores intenros: n, uno por cada tita
} ProblemArguments;

typedef struct Results {
	// num_instancias vectores solucion al sistema formulado en los argumentos del problema.
	vector<vector<double> > instances_solutions;
} Results;

void mostrar_datos_entrada(ProblemArguments args, ostream &os);
void read_args_from_stream(istream &is, ProblemArguments& args);
void write_results_to_stream(ostream &os, Results output_results);

#endif