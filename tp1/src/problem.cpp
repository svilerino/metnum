#include "problem.h"

Problem::Problem(ProblemArguments &in_args) {
	m = in_args.cantidad_radios-1;
	n = in_args.cantidad_titas;
	Ri = in_args.radio_interno;
	Re = in_args.radio_externo;
	num_instancias = in_args.num_instancias;
	isoterma_buscada = in_args.isoterma_buscada;
	instancias_temp_internas = in_args.instancias_temp_internas;
	instancias_temp_externas = in_args.instancias_temp_externas;

	dimension = n*(m+1);
	delta_r = ((double)Re-(double)Ri)/(double)m;
	delta_t = 2.0*M_PI/(double)n;
	
	armar_matriz();
	//A.mostrar_esparsa(cout);
}

void Problem::armar_matriz() {
	A.resize(dimension, dimension);
	// Las primeras y últimas n filas de A coinciden con la identidad
	for (int k = 0; k < n; k++) {
		A[indice(0,k)][indice(0,k)] = 1;
		A[indice(m,k)][indice(m,k)] = 1;
	}
	// Las demás filas tienen cinco componentes no nulas
	for (int j = 1; j < m; j++) {
		for (int k = 0; k < n; k++) {
			armar_fila(j, k);
		}
	}

	// Evaluo que sea diagonal dominante por filas
	for (int i = 0; i < dimension; i++){
		double suma = 0;
		for (int j = 0; j < dimension; j++){
			if (j != i) suma += abs(A[i][j]); 
		}
		if (abs(suma) > abs(A[i][i])+0.00000001) cerr << "No es diagonal dominante por filas" << endl;
	}


	for (int i = 0; i < 1000; ++i) {
		int res = indice(punto(i).first, punto(i).second);
		// cout << res << " " << i << endl;
		assert(res == i);
	}
}

void Problem::armar_fila(int j, int k) {
	int S_PUNTOS[5][2] = {{-1, 0}, {0, 0}, {1, 0}, {0, -1}, {0, 1}}; // tiene los vecinos y el punto principal
	//cout << "j, k: " << j << ", " << k << endl;
	int indice_fila = indice(j, k);
	//cout << "indice_fila: " << indice_fila << endl;
	for (int num_punto = 0; num_punto < 5; num_punto++){
		int s_j = S_PUNTOS[num_punto][0];
		int s_k = S_PUNTOS[num_punto][1];
		int j_punto = j+s_j;
		int k_punto = (k+s_k == -1) ? n-1 : (k+s_k)%n;		// el operador % no se lleva bien con números negativos
		int indice_columna = indice(j_punto, k_punto);
		double valor = multiplicador(s_j, s_k, j);
		//cout << endl << j_punto << " " << k_punto << endl;
		//cout << indice_fila << ", " << indice_columna << " = " << valor << endl;
		A[indice_fila][indice_columna] = valor; 
	}
}

double Problem::multiplicador(int s_j, int s_k, int j) {
	double delta_r2 = pow(delta_r, 2);	// delta_r^2
	double delta_t2 = pow(delta_t, 2);	// delta_t^2
	double rj = Rj(j);					// rj
	double rj2 = pow(rj, 2);			// rj^2
	if (s_j == -1 && s_k == 0) {
		// caso (j-1, k)
		return 1/delta_r2 - 1/(rj*delta_r);
	} if (s_j == 0 && s_k == 0) {
		// caso (j, k)
		return -2/delta_r2 + 1/(rj*delta_r) - 2/(rj2*delta_t2);
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
	vector<double> b(dimension, 0);
	// Las primeras y últimas n componentes del vector son las únicas no-nulas
	for (int k = 0; k < n; k++) {
		b[indice(0, k)] = Ti(instancia, k);
		b[indice(m, k)] = Te(instancia, k);
	}
	return b;
}

void Problem::resolver_instancias(Results &output, ostream &timing_result_os, metodo_resolucion metodo) {
	vector<double> b;
	vector<double> res;
	SistemaEcuaciones sist_ec(A, b);
	FactorizacionLU lu;

	double promedio_preprocesamiento = 0.0;

	if(metodo == FACT_LU) {
		MEDIR_TIEMPO_PROMEDIO(
				sist_ec.factorizar_LU(lu);
		    , CANT_ITERS_MEDICION, &promedio_preprocesamiento);
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

		// Si se uso factorizacion LU, le agrego ademas el costo de obtener la factorizacion LUpromedio_medicion_instancia		
		// La primera instancia se lleva de bonus el costo de crear LU si corresponde
		promedio_medicion_instancia += (instancia == 0) * promedio_preprocesamiento;

		// cant_radios cant_titas dimension_matriz num_instancias metodo_utilizado promedio_medicion
		//timing_result_os << n << " " << (m+1) << " " << dimension << " " << num_instancias << " " << metodo << " " << promedio_medicion_instancia;
		timing_result_os << n << " " << (m+1) << " " << promedio_medicion_instancia;

    	if(instancia < num_instancias - 1){
    		timing_result_os << endl;// La medicion esta en microsegundos !
    	}

    	// Escribir resultados en parametro out.
    	output.instances_solutions.push_back(res);
	}
}