#include <vector>
#include <iostream>
#include <cstring>
#include "matriz.h"
#include "io.h"
#include "problem.h"

using namespace std;

int main(int argc, char** argv){
	if(argc != 4) {
		cout << "El ejecutable toma tres parametros por lınea de comando, que seran el archivo de entrada, el archivo de salida, y el metodo a ejectutar (0 EG, 1 LU, 2 EG_PIVOTEO_PARCIAL)." << endl;
		exit(-1);
	}

	char* path_file_in = argv[1]; 
	char* path_file_out = argv[2]; 
	metodo_resolucion execution_mode = (metodo_resolucion) atoi(argv[3]); 

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
	ofstream timing_file("timing_results.txt");
	if (!timing_file.is_open()) {
		cerr << "Imposible escribir en archivo de salida" << endl;
		exit(-1);
	} 

	problem.resolver_instancias(output_results, timing_file, execution_mode);

	timing_file.close(); // Seguro esta abierto, sino hubiera ejecutado el exit(-1);

	// Escribir resultados
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