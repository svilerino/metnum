#include <vector>
#include <iostream>
#include <cstring>
#include "matriz.h"

using namespace std;

typedef enum tipo_ejecucion {ELIM_GAUSSIANA, FACT_LU} tipo_ejecucion;

typedef struct ProblemArguments {
	int radio_interno;
	int radio_externo;
	int cantidad_radios;
	int cantidad_titas;
	double isoterma;
	int num_instancias;
	// Los siguientes vectores tienen ninst vectores con los parametros de cada instancia
	vector<vector<double> > instancias_temp_internas;//m, uno por cada tita
	vector<vector<double> > instancias_temp_externas;//m, uno por cada tita
} ProblemArguments;

typedef struct Results {
	//ninst vectores solucion al sistema formulado en los argumentos del problema.
	vector<vector<double> > instances_solutions;
} Results;

void mostrar_datos_entrada(ProblemArguments args, std::ostream &os);
void read_args_from_stream(std::istream &is, ProblemArguments& args);
void write_results_to_stream(std::ostream &os, Results output_results);

void resolucion_gauss(ProblemArguments in_args, Results &output, std::ostream &timing_result_os);
void resolucion_lu(ProblemArguments in_args, Results &output, std::ostream &timing_result_os);

int main(int argc, char** argv){
	if(argc != 4) {
		cout << "El ejecutable tomara tres parametros por lınea de comando, que seran el archivo de entrada, el archivo de salida, y el metodo a ejectutar (0 EG, 1 LU, 2 TESTS)." << endl;
		exit(-1);
	}

	char* path_file_in = argv[1]; 
	char* path_file_out = argv[2]; 
	tipo_ejecucion execution_mode = (tipo_ejecucion) atoi(argv[3]); 


	ProblemArguments in_arg;
	memset(&in_arg, 0, sizeof(ProblemArguments));

	
	ifstream input_file (path_file_in);
	if (input_file.is_open()) {
		 read_args_from_stream(input_file, in_arg);
		input_file.close();
	} else {
		cerr << "Path de archivo de entrada invalido" << endl; 	
		exit(-1);
	} 
	
	mostrar_datos_entrada(in_arg, cout);

	Results output_results;
	memset(&output_results, 0, sizeof(Results));

	// Archivo que guarda datos de mediciones
	ofstream timing_file ("timing_results.txt");
	if (!timing_file.is_open()) {
		cerr << "Imposible escribir en archivo de salida" << endl;
		exit(-1);
	} 

	switch(execution_mode) {
		case ELIM_GAUSSIANA:
			resolucion_gauss(in_arg, output_results, timing_file);
			timing_file.close();// Seguro esta abierto sino hubiera ejecutado el exit(-1);
			break;
		case FACT_LU:
			resolucion_lu(in_arg, output_results, timing_file);
			timing_file.close();// Seguro esta abierto sino hubiera ejecutado el exit(-1);
			break;
	}

	ofstream output_file (path_file_out);
	if (output_file.is_open()) {
		write_results_to_stream(output_file, output_results);
		output_file.close();
	} else {
		cerr << "Imposible escribir en archivo de salida" << endl;
		exit(-1);
	}
    return 0;
}

void read_args_from_stream(std::istream &is, ProblemArguments &args) {
	memset(&args, 0, sizeof(ProblemArguments));

	is >> args.radio_interno;
	is >> args.radio_externo;
	is >> args.cantidad_radios;
	is >> args.cantidad_titas;
	is >> args.isoterma;
	is >> args.num_instancias;

	for (int i = 0; i < args.num_instancias; i++) {
		vector<double> temperaturas_internas;
		for(int j = 0; j < args.cantidad_titas ; j++) {
			double temp = 0;
			is >> temp;
			temperaturas_internas.push_back(temp);
		}		
		args.instancias_temp_internas.push_back(temperaturas_internas);
		
		vector<double> temperaturas_externas;
		for(int j = 0; j < args.cantidad_titas ; j++) {
			double temp = 0;
			is >> temp;
			temperaturas_externas.push_back(temp);
		}
		args.instancias_temp_externas.push_back(temperaturas_externas);
	}
}

void write_results_to_stream(std::ostream &os, Results output_results) {
	os.precision(5);
	os.setf(ios::fixed,ios::floatfield);
	
	int ninst = output_results.instances_solutions.size();

	for (int i = 0; i < ninst; i++) {
		int solution_dim = output_results.instances_solutions[i].size();
		for (int j = 0; j < solution_dim; j++) {
			os << output_results.instances_solutions[i][j] << endl;
		}	
	}
}

void resolucion_gauss(ProblemArguments in_args, Results &output, std::ostream &timing_result_os) {
// Armar la matriz y resolver el sistema usando eliminacion gaussiana(ver tests.hpp para ver como)
}

void resolucion_lu(ProblemArguments in_args, Results &output, std::ostream &timing_result_os) {
// Armar la matriz y resolver el sistema usando LU(ver tests.hpp para ver como)
}

void mostrar_datos_entrada(ProblemArguments args, std::ostream &os) {
	os << "radio_interno: " << args.radio_interno << endl;
	os << "radio_externo: " << args.radio_externo << endl;
	os << "cantidad_radios: " << args.cantidad_radios << endl;
	os << "cantidad_titas: " << args.cantidad_titas << endl;
	os << "isoterma: " << args.isoterma << endl;
	os << "num_instancias: " << args.num_instancias << endl;
	
	// Los siguientes vectores tienen ninst vectores con los parametros de cada instancia
	for (int i = 0; i < args.num_instancias; i++) {
		cout << "Instancia " << i << "-esima" << endl;
		cout << "Temperaturas internas: ";
		imprimir_vector(args.instancias_temp_internas[i], cout);
		
		cout << endl << "Temperaturas externas: ";
		imprimir_vector(args.instancias_temp_externas[i], cout);
		cout << endl;
	}
}