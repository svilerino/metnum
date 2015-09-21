#ifndef MATRIZ_H
#define MATRIZ_H

#include <stdlib.h>
#include <vector>
#include <fstream>
#include <math.h>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <algorithm>

using namespace std;

/**
* Operaciones de vectores
*/
void imprimir_vector(vector<double> &vec, ostream &os);
void check_dimensiones(int dimA, int dimB, const char* function_name);
void sumar(vector<double> &a, vector<double> &b, vector<double> &res);
void restar(vector<double> &a, vector<double> &b, vector<double> &res);
double producto_interno(vector<double>&, vector<double>&);
double normap(vector<double>&, int p);
void normalizar(vector<double> &v, int norma);

class Matriz {
/**
* Matriz
* Representa una matriz de doubles usando vector de vectores
*/
public:
    /*********************** CONSTRUCTORES ***********************/
	/**
    * Constructor por defecto
    */
	Matriz();
	
    /**
    * Constructor
    * Construye la matriz identidad de n*n.
    */
    Matriz(int n);
	
    /**
    * Constructor
    * Toma el alto (primer parámetro) y el ancho (segundo parámetro)
    * y devuelve una matriz del tamaño especificado inicializada con ceros.
    */
    Matriz(int n, int m);

    /**
    * Cargar
    * Carga una matriz de un archivo. La primer línea del archivo debe ser,
    * en este orden, dos enteros que especifiquen el alto y el ancho.
    * A partir de la segunda línea deben estar los elementos separados por espacios.
    * No hace falta que haya saltos de línea entre filas.
    */
   	void cargar(istream &is);

    /**
    * Resize
    * Borra la matriz y crea una nueva de alto n y ancho m.
    */
    void resize(int n, int m);

    /************************ VARIOS ************************/
	/**
    * []
    * Permite acceder a los elementos de la matriz usando notación estándar M[i][j].
    */
    vector<double>& operator[](int i){return _matriz[i];}
    
    /**
    * Mostrar
    * Muestra una matriz por el stream 'os'.
    */
    void mostrar(ostream &os);
    
    /**
    * Mostrar esparsa
    * Muestra los valores no nulos de una matriz por el stream 'os'.
    */
    void mostrar_esparsa(ostream &os);
    /************************ Estructura interna *****************************/

    int get_filas() const {return _numfilas;}
    int get_columnas() const {return _numcolumnas;}

private:
    int _numfilas;
	int _numcolumnas;
    vector<vector <double> > _matriz;
};

#endif
