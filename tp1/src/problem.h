#ifndef PROBLEM_H
#define PROBLEM_H
#define CANT_ITERS_MEDICION 1

#include "io.h"

typedef enum metodo_resolucion {ELIM_GAUSSIANA, FACT_LU, ELIM_GAUSSIANA_CON_PIVOTEO_PARCIAL} metodo_resolucion;

class Problem {
	// Abstrae funciones y datos útiles de un problema con varias instancias
public:
    Problem(ProblemArguments &in_args);

	/* Resolución de instancias
	* Resuelve todas las instancias del problema computando una vez la factorización LU
	* y usándola de ahí en adelante o usando eliminacion gaussiana repetidas veces.
	* Esto depende del parametro pasado del tipo metodo_resolucion
	*/
    void resolver_instancias(Results &output, ostream &timing_result_os, metodo_resolucion metodo);

private:
	/* Indice
	* Devuelve el índice (número de fila o columna) corresondiente al punto T_(j,k)
	*/
	int indice(int j, int k){return j*n+k;}
	/* Ti
	* Devuelve la termperatura interna del horno en el angulo k y la instancia i
	*/
	double Ti(int i, int k){return instancias_temp_internas[i][k];}
	/* Te
	* Devuelve la termperatura externa del horno en el angulo k y la instancia i
	*/
	double Te(int i, int k){return instancias_temp_externas[i][k];}
	/* Rj
	* Devuelve la longitud del radio j-ésimo
	*/
	double Rj(int j){return Ri+delta_r*j;}
	/* Armar matriz
	* Arma la matriz correspondiente al problema y la guarda en A
	*/
	void armar_matriz();
		/* Multiplicador
		* Auxiliar para el armado de la matriz
		* Devuelve el factor que multiplica al vecino de un punto (j, k) de la matriz
		* El vecino correspondiente se indica con s_j y s_k, pudiendo tomar cualquier par de valores en
		* {(-1, 0); (0, 0); (1, 0); (0, -1); (0, 1)}
		* j debe ser el número de radio correspondiente al punto
		* En caso de ser s_j=s_k=0, el factor es el que multiplica al punto
		*/
		double multiplicador(int s_j, int s_k, int j);
	/* Armar b
	* Arma el vector b correspondiente a una instancia del problema
	*/
	vector<double> armar_b(int instancia);

    int n;			// cantidad de angulos
    int m;			// cantidad de radios
    int dimension;	// cantidad de incógnitas (y de filas y de columnas de la matriz del problema)
    int Ri;			// radio interno
    int Re;			// radio externo
    int delta_r;	// delta radio
    int delta_t;	// delta tita
	double isoterma_buscada;
	int num_instancias;	

    Matriz A;
	vector<vector<double> > instancias_temp_internas;	// T_i(k). Dimension de los vectores internos: n, uno por cada tita
	vector<vector<double> > instancias_temp_externas;	// T_e(k). Dimension de los vectores intenros: n, uno por cada tita
};
#endif