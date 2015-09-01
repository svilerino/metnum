#include "problem.h"
#include <cassert>
#define DEBUG_MESSAGES_ON 0//comentar esta linea para no hacer chequeos costosos en tiempo de ejecucion

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

    #ifdef DEBUG_MESSAGES_ON

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

	#endif
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

vector<pair<double, double> > Problem::determinar_seguridad_isotermas(vector< vector<double> > &isotermas, ostream &seguridad_isoterma_result_os) {
	vector<pair<double, double> > res;
	for (int i = 0; i < num_instancias; i++) {
		res.push_back(determinar_seguridad_isoterma(isotermas[i], seguridad_isoterma_result_os));
	}
	return res;
}

pair<double, double> Problem::determinar_seguridad_isoterma(vector<double> &isoterma, ostream &seguridad_isoterma_result_os) {
	// maximo, promedio
	double metrica_max = 0;
	vector<double>::iterator it_max = max_element(isoterma.begin(), isoterma.end()); 
	metrica_max = (*it_max - Ri) / (Re - Ri);
	
	double metrica_promedio = 0;
	for(double d : isoterma) {
		metrica_promedio += d;
	}
	metrica_promedio = metrica_promedio/(double) isoterma.size();
	
	metrica_promedio = (metrica_promedio - Ri) / (Re - Ri);

	seguridad_isoterma_result_os << metrica_promedio << " " << metrica_max << endl;
	return make_pair(metrica_promedio, metrica_max);
}

void Problem::interpolar_isotermas(Results &output, vector< vector<double> > &out_isotermas, ostream &iso_result_os, metodo_interpolacion_isoterma metodo) {
	for (int i = 0; i < num_instancias; i++) {
		interpolar_isoterma(output.instances_solutions[i], out_isotermas[i], iso_result_os, metodo);
	}
}

void Problem::interpolar_isoterma(vector<double> &solucion, vector<double> &out_isoterma, ostream &iso_result_os, metodo_interpolacion_isoterma metodo_interpolacion) {
	// La idea es para cada angulo, encontrar el radio "mas apropiado" que indique isoterma_buscada grados.
	// Para esto podemos considerar la curva de nivel que se arma cuando fijamos el angulo y variamos el radio
	// Representaremos esta funcion (discreta) para cada angulo en el vector temp_ang.
	
	// Luego usando la stl, con las funciones lower_bound y upper_bound tomaremos los 2 puntos que encierren
    // a isoterma_buscada y procederemos a hacer la interpolacion indicada por parametro

	vector<double> temp_ang(m+1, 0);
	for (int angulo = 0; angulo < n; angulo++) {
		
		// Armo el vector con temperaturas para el angulo actual
		// Notar que estan al reves. los radios de afuera hacia adentro para que quede ordenado
		// de menor a mayor
		for (int radio = 0; radio < m+1; radio++) {
			double temperatura = solucion[indice(radio, angulo)];
			// cout << "temperatura en angulo: " << Tk(angulo) << ", Radio = " << Rj(radio) << ": " << temperatura << endl;
			// temp_ang esta ordenado de mayor a menor, lo necesitamos al reves
			temp_ang[m-radio/*m+1-1-radio*/] = temperatura;
		}

		vector<double>::iterator it_min, it_max;
		it_min = min_element(temp_ang.begin(), temp_ang.end());
		it_max = max_element(temp_ang.begin(), temp_ang.end());

		assert(*it_min <= *it_max);

		bool isotermaEnRango = true;
		double radio_estimado = 0;

		if(*it_min > isoterma_buscada){
			isotermaEnRango = false;
			radio_estimado = Ri - delta_r;
		}

		if(*it_max < isoterma_buscada){
			isotermaEnRango = false;
			radio_estimado = Re + delta_r;
		}

		if(isotermaEnRango) {
			// Busco cotas superiores e inferiores a la isoterma_buscada
			vector<double>::iterator it_low, it_up;
			// Segun la stl, it_low puede ser MAYOR O IGUAL a isoterma_buscada(una cota inferior, que puede ser el valor buscado)
			it_low = lower_bound(temp_ang.begin(), temp_ang.end(), isoterma_buscada);

			// Queremos la posicion anterior al primero que no compara menor.
			// Es decir, el ultimo que compara menor.
			if (it_low > temp_ang.begin()) {
				it_low--;
			}
			
			// Asimismo it_up nunca sera igual, solo mayor(cota superior estricta) o last si el target es el ultimo.
			it_up = upper_bound(temp_ang.begin(), temp_ang.end(), isoterma_buscada);

			// Como el vector de temperaturas esta revertido
			// Los indices de up y low estan espejados

			int idx_low = m - (it_low - temp_ang.begin());
			int idx_up = m - (it_up - temp_ang.begin());
			
			reverse(temp_ang.begin(), temp_ang.end());
			// imprimir_vector(temp_ang, cout);

			// cout << "isoterma acotada en angulo: " << Tk(angulo) << endl;

			// low y up se refieren a valores numericos
			// es decir que vale low <= isoterma_buscada <= up
			// dado que el vector esta ordenado de mayor a menor
			// en terminos de indices vale idx_up <= idx_low
			// pues las temperaturas siguen ese patron del centro al exterior del horno

			if(temp_ang[idx_low] > isoterma_buscada || isoterma_buscada > temp_ang[idx_up]) {
				imprimir_vector(temp_ang, cout);
				cout << temp_ang[idx_low] << " <= " << isoterma_buscada << " <= " << temp_ang[idx_up] << endl;

				// le hice reverse y se dieron vuelta los iters
				cout << "min: " << *it_max << endl;
				cout << "max: " << *it_min << endl;

				cout << "upper_bound at position " << idx_up << endl;
				cout << "lower_bound at position " << idx_low << endl;

				cout << "upper_bound at radio " << Rj(idx_up) << endl;
				cout << "lower_bound at radio " << Rj(idx_low) << endl;
			}

			assert(temp_ang[idx_low] <= isoterma_buscada && isoterma_buscada <= temp_ang[idx_up]);

			// Estimamos la isoterma			
			if(metodo_interpolacion == LINEAL) {
				radio_estimado = interpolacion_lineal_inversa(isoterma_buscada, Rj(idx_up), Rj(idx_low), temp_ang[idx_low], temp_ang[idx_up]);
			}
		}
		// Imprimimos por el stream para cada angulo(linea) la posicion radial de la isoterma estimada
		iso_result_os << radio_estimado << endl;
		out_isoterma.push_back(radio_estimado);
	}	
}

double Problem::interpolacion_lineal_inversa(double fx, double x1, double x2, double fx1, double fx2){
	double x=0;

	// cout << x << endl;
	// cout << x1 << endl;
	// cout << x2 << endl;
	// cout << fx1 << endl;
	// cout << fx2 << endl;

	x = x1 + ((x2 - x1) / (fx2 - fx1)) * (fx - fx1);

	return x;
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