#ifndef PROBLEM_H
#define PROBLEM_H
#define CANT_ITERS_MEDICION 1// ojo que al hacer mas de una iteracion como esta el codigo
// para lu funciona bien, para gauss no,
// hay que meter las llamadas que reemplazan A y b del sistema adentro de la medicion.

#include <math.h>
#include "io.h"
#include "timing.h"
#include <cassert>

typedef enum metodo_resolucion {ELIM_GAUSSIANA, FACT_LU, ELIM_GAUSSIANA_CON_PIVOTEO_PARCIAL} metodo_resolucion;
typedef enum metodo_interpolacion_isoterma {LINEAL} metodo_interpolacion_isoterma;

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

	void interpolar_isotermas(Results &output, ostream &iso_result_os, metodo_interpolacion_isoterma metodo);

    void mostrar_solucion(ostream &os, Results output_results);

private:
	/* interpolacion_lineal_inversa
	* Interpola linealmente el punto x entre puntos x1 y x2 dados los puntos fx, fx1 y fx2 
	*/
	double interpolacion_lineal_inversa(double fx, double x1, double x2, double fx1, double fx2);
	/* interpolar_isoterma
	* Interpola la isoterma y la imprime en el stream pasado por parametro,
	 cada linea es un angulo y el valor de cada linea es el radio donde se estima la isoterma.
	*/
    void interpolar_isoterma(vector<double> &solucion, ostream &iso_result_os, metodo_interpolacion_isoterma metodo);
	/* Punto
	* Devuelve el punto (j, k) en polares dado un indice
	*/
	pair<int, int> punto(int i){return make_pair(i / n, i % n);}
	/* Indice
	* Devuelve el índice (número de fila o columna) corresondiente al punto (j,k)
	*/
	int indice(int j, int k){return j*n+k;}
	/* Ti
	* Devuelve la temperatura interna del horno en el angulo k y la instancia i
	*/
	double Ti(int i, int k){return instancias_temp_internas[i][k];}
	/* Te
	* Devuelve la temperatura externa del horno en el angulo k y la instancia i
	*/
	double Te(int i, int k){return instancias_temp_externas[i][k];}
	/* Rj
	* Devuelve la longitud del radio j-ésimo
	*/
	double Rj(int j){return Ri+delta_r*j;}
	/* Tk
	* Devuelve la amplitud del angulo k-ésimo
	*/
	double Tk(int k){return delta_t*k;}

	/* Armar matriz
	* Arma la matriz correspondiente al problema y la guarda en A
	*/
	void armar_matriz();
	void armar_fila(int j, int k);
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
    int m;			// cantidad de radios - 1
    int dimension;	// cantidad de incógnitas (y de filas y de columnas de la matriz del problema)
    int Ri;			// radio interno
    int Re;			// radio externo
    int num_instancias;	
	double delta_r;	// delta radio
    double delta_t;	// delta tita
	double isoterma_buscada;
	
    Matriz A;
	vector<vector<double> > instancias_temp_internas;	// T_i(k). Dimension de los vectores internos: n, uno por cada tita
	vector<vector<double> > instancias_temp_externas;	// T_e(k). Dimension de los vectores intenros: n, uno por cada tita
};
#endif