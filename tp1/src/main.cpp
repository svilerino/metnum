#include <vector>
#include <iostream>
#include <cstring>
#include "matriz.h"
#include "io.h"
#include "problem.h"

using namespace std;

int main(int argc, char** argv){
	if(argc < 4) {
		cout << "El ejecutable toma tres parametros por lınea de comando, que seran el archivo de entrada, el archivo de salida, y el metodo a ejecutar (0 EG, 1 LU, 2 EG_PIVOTEO_PARCIAL)." << endl;
		cout << "Como parametros adicionales se pueden pasar luego del tercer parametro: <archivo salida timing>" << endl;
		exit(-1);
	}

	string path_file_in = argv[1]; 
	
	string path_file_out = argv[2]; 


	metodo_resolucion execution_mode = (metodo_resolucion) atoi(argv[3]); 
	
	string path_timings_out = (argc > 4) ? argv[4] : "timing_results.txt";
	
	string path_isofile_out = (argc > 5) ? argv[5] : "isofile_result.txt";

	string path_seguridad_isofile_out = (argc > 6) ? argv[6] : "seguridad_isoterma_result.txt";

	ProblemArguments in_arg;
	
	ifstream input_file(path_file_in);
	if (input_file.is_open()) {
		read_args_from_stream(input_file, in_arg);
		input_file.close();
	} else {
		cerr << "Path de archivo de entrada invalido: " << path_file_in << endl; 	
		exit(-1);
	} 
	
	//mostrar_datos_entrada(in_arg, cout);

	Problem problem(in_arg);
	Results output_results;
	
	// Archivo que guarda datos de mediciones
	ofstream timing_file(path_timings_out);
	if (!timing_file.is_open()) {
		cerr << "Imposible escribir en archivo de timings: " << path_timings_out << endl;
		exit(-1);
	} 

	// Archivo que guarda datos de isoterma
	ofstream iso_file(path_isofile_out);
	if (!iso_file.is_open()) {
		cerr << "Imposible escribir en archivo de isoterma: " << path_isofile_out << endl;
		exit(-1);
	} 

	// Archivo que guarda datos de isoterma
	ofstream seguridad_file(path_seguridad_isofile_out);
	if (!seguridad_file.is_open()) {
		cerr << "Imposible escribir en archivo de seguridad isoterma: " << path_seguridad_isofile_out << endl;
		exit(-1);
	} 

	problem.resolver_instancias(output_results, timing_file, execution_mode);

	metodo_interpolacion_isoterma metodo_interpolacion = LINEAL;
	vector< vector<double> > isotermas(in_arg.num_instancias);
	problem.interpolar_isotermas(output_results, isotermas, iso_file, metodo_interpolacion);

	vector<pair<double, double> > ratio_seguridad_isotermas;
	ratio_seguridad_isotermas = problem.determinar_seguridad_isotermas(isotermas, seguridad_file);

	timing_file.close(); // Seguro esta abierto, sino hubiera ejecutado el exit(-1);
	iso_file.close(); // Seguro esta abierto, sino hubiera ejecutado el exit(-1);
	seguridad_file.close(); // Seguro esta abierto, sino hubiera ejecutado el exit(-1);

	// Escribir resultados
	ofstream output_file(path_file_out);
	if (output_file.is_open()) {
		write_results_to_stream(output_file, output_results);
		output_file.close();
	} else {
		cerr << "Imposible escribir en archivo de salida: " << path_file_out << endl;
		exit(-1);
	}
    return 0;
}