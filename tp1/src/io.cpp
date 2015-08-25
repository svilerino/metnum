#include "io.h"
using namespace std;

void read_args_from_stream(istream &is, ProblemArguments &args) {
	memset(&args, 0, sizeof(ProblemArguments));

	is >> args.radio_interno;
	is >> args.radio_externo;
	is >> args.cantidad_radios;
	is >> args.cantidad_titas;
	is >> args.isoterma_buscada;
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

void write_results_to_stream(ostream &os, Results output_results) {
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

void resolucion_gauss(ProblemArguments in_args, Results &output, ostream &timing_result_os) {
	// Armar la matriz y resolver el sistema usando eliminacion gaussiana
	
}

void resolucion_lu(ProblemArguments in_args, Results &output, ostream &timing_result_os) {
// Armar la matriz y resolver el sistema usando LU(ver tests.hpp para ver como)
}

void mostrar_datos_entrada(ProblemArguments args, ostream &os) {
	os << "radio_interno: " << args.radio_interno << endl;
	os << "radio_externo: " << args.radio_externo << endl;
	os << "cantidad_radios: " << args.cantidad_radios << endl;
	os << "cantidad_titas: " << args.cantidad_titas << endl;
	os << "isoterma_buscada: " << args.isoterma_buscada << endl;
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