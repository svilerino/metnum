#include <vector>
#include <iostream>
#include <cstring>
#include "matriz.h"
#include "io.h"
#include "problem.h"

using namespace std;

int main(int argc, char** argv){
	if(argc != 4) {
		cout << "El ejecutable toma tres parametros por lınea de comando, que seran el archivo de entrada, el archivo de salida, y el metodo a ejectutar (0 EG, 1 LU, 2 TESTS)." << endl;
		exit(-1);
	}

	char* path_file_in = argv[1]; 
	char* path_file_out = argv[2]; 
	tipo_ejecucion execution_mode = (tipo_ejecucion) atoi(argv[3]); 

	ProblemArguments in_arg;
	
	ifstream input_file(path_file_in);
	if (input_file.is_open()) {
		read_args_from_stream(input_file, in_arg);
		input_file.close();
	} else {
		cerr << "Path de archivo de entrada invalido" << endl; 	
		exit(-1);
	} 
	
	mostrar_datos_entrada(in_arg, cout);

	Problem problem(in_arg);
	Results output_results;
	
	// Archivo que guarda datos de mediciones
	ofstream timing_file ("timing_results.txt");
	if (!timing_file.is_open()) {
		cerr << "Imposible escribir en archivo de salida" << endl;
		exit(-1);
	} 

	switch(execution_mode) {
		case ELIM_GAUSSIANA:
			problem.resolucion_gauss(output_results, timing_file);
			timing_file.close(); // Seguro esta abierto, sino hubiera ejecutado el exit(-1);
			break;
		case FACT_LU:
			problem.resolucion_lu(output_results, timing_file);
			timing_file.close(); // Seguro esta abierto, sino hubiera ejecutado el exit(-1);
			break;
	}

	ofstream output_file(path_file_out);
	if (output_file.is_open()) {
		write_results_to_stream(output_file, output_results);
		output_file.close();
	} else {
		cerr << "Imposible escribir en archivo de salida" << endl;
		exit(-1);
	}
    return 0;
}