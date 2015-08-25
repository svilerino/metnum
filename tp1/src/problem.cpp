#include "problem.h"
#include "timing.h"

Problem::Problem(ProblemArguments &in_args) {
	m = in_args.cantidad_radios;
	n = in_args.cantidad_titas;
	Ri = in_args.radio_interno;
	Re = in_args.radio_externo;
	num_instancias = in_args.num_instancias;
	isoterma_buscada = in_args.isoterma_buscada;
	instancias_temp_internas = in_args.instancias_temp_internas;
	instancias_temp_externas = in_args.instancias_temp_externas;

	dimension = n*(m+1);
	delta_r = (Re-Ri)/m;
	delta_t = 2*3.14/n;
	
	armar_matriz();
}

void Problem::armar_matriz() {
	Matriz res(dimension, dimension);
	int ultimo_indice_valido = indice(m,n-1);
	// Las primeras y últimas n filas de A coinciden con la identidad
	for (int j = 0; j < n; j++) {
		res[indice(j,j)][indice(j,j)] = 1;
		int i = ultimo_indice_valido-j;
		res[indice(i,i)][indice(i,i)] = 1;
	}
	// Las demás filas tienen cinco componentes no nulas
	for (int j = 1; j < m; j++) {
		for (int k = 0; k < n-1; k++) {
			
		}
	}
	A = res; // me gustaría trabajar directamente sobre A pero desconozco cómo llamar a su constructor desde adentro de un no-constructor
	// La veo medio jodida dado que ademas las dimensiones de la matriz son dinamicas(se leen del input)
	// Seguro se puede pero esto no es tan grave porque la matriz inicial que se construye es vacia.
}

double Problem::multiplicador(int s_j, int s_k, int j) {
	double delta_r2 = pow(delta_r, 2); 	// delta_r^2
	double delta_t2 = pow(delta_t, 2); 	// delta_t^2
	double rj = Rj(j);					// rj
	double rj2 = pow(rj, 2); 			// rj^2

	if (s_j == -1 && s_k == 0) {
		// caso (j-1, k)
		return 1/delta_r2 - 1/(rj*delta_r);
	} if (s_j == 0 && s_k == 0) {
		// caso (j, k)
		return -2/delta_r2 + 1/(rj*delta_r) - 2/(rj2*delta_r2);
	} if (s_j == 1 && s_k == 0) {
		// caso (j+1, k)
		return 1/delta_r2;
	} if (s_j == 0 && s_k == -1) {
		// caso (j, k-1)
		return 1/(rj2*delta_t2);
	} if (s_j == 0 && s_k == 1) {
		// caso (j, k+1)
		return 1/(rj2*delta_t2);
	}
	cerr << "Error al llamar a multiplicador: (s_j, s_k) no está en {(-1, 0); (0, 0); (1, 0); (0, -1); (0, 1)}" << endl;
	exit(-1);
}

vector<double> Problem::armar_b(int instancia) {
	vector<double> v(dimension, 0);
	cerr << "Falta implementar Problem::armar_b!" << endl;
	exit(-1);
	return v;
}

void Problem::resolver_instancias(Results &output, ostream &timing_result_os, metodo_resolucion metodo) {
	vector<double> b;
	vector<double> res;
	SistemaEcuaciones sist_ec(A, b);
	FactorizacionLU lu;

	if(metodo == FACT_LU) {
		sist_ec.factorizar_LU(lu);
	}
	
	timing_result_os.precision(5);
    timing_result_os.setf(ios::fixed, ios::floatfield);

	for (int instancia = 0; instancia < num_instancias; instancia++) {
		// Reinicializar vector de soluciones y vector de terminos independientes
		res.resize(dimension, 0);
		b = armar_b(instancia);
		sist_ec.cambiar_b(b);

		double promedio_medicion_instancia = 0.0;

		if(metodo ==  ELIM_GAUSSIANA) {
			// Hay que poner la matriz A original sin la eli. gauss. anterior.
			sist_ec.cambiar_a(A);

		    MEDIR_TIEMPO_PROMEDIO(
				sist_ec.eliminacion_gaussiana(false/*no usar pivoteo parcial*/, res);
		    , CANT_ITERS_MEDICION, &promedio_medicion_instancia);  

		} else if (metodo == ELIM_GAUSSIANA_CON_PIVOTEO_PARCIAL) {
			// Hay que poner la matriz A original sin la eli. gauss. anterior.
			sist_ec.cambiar_a(A);

		    MEDIR_TIEMPO_PROMEDIO(
				sist_ec.eliminacion_gaussiana(true/*usar pivoteo parcial*/, res);
		    , CANT_ITERS_MEDICION, &promedio_medicion_instancia);  

		} else if (metodo == FACT_LU) {
			
		    MEDIR_TIEMPO_PROMEDIO(
				sist_ec.resolver_con_LU(lu, res);
		    , CANT_ITERS_MEDICION, &promedio_medicion_instancia);  

		}	

	    // Imprimo el tiempo consumido en el stream.
	    // La onda de esto es tener un archivo con el tiempo consumido por instancia por cada linea
	    // Despues con scripts y ploteos mostrar esta info apropiadamente en el informe.

    	timing_result_os << promedio_medicion_instancia << endl;

    	// Escribir resultados en parametro out.
    	output.instances_solutions.push_back(res);
	}
}