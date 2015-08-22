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
void imprimir_vector(vector<double> &vec, std::ostream &os);
void check_dimensiones(int dimA, int dimB, const char* function_name);
void sumar(vector<double> &a, vector<double> &b, vector<double> &res);
void restar(vector<double> &a, vector<double> &b, vector<double> &res);
double producto_interno(vector<double>&, vector<double>&);
double normap(vector<double>&, int p);
void normalizar(vector<double> &v, int norma);

class Matriz {
/**
* Matriz
* Representa una matriz de doubles
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
    * Constructor a partir de un vector
    * Toma un vector no vacío de doubles y lo castea a matriz de R^(n x 1)
    * (vector columna).
    */
    Matriz(vector<double> &v);
    
    /**
    * Constructor de matriz diagonal
    * Toma un vector no vacío de double y lo convierte en una matriz de (n x n) que lo
    * tiene como diagonal.
    */
    static Matriz diagonal(vector<double> &v);
    
    /**
    * Cargar
    * Carga una matriz de un archivo. La primer línea del archivo debe ser,
    * en este orden, dos enteros que especifiquen el alto y el ancho.
    * A partir de la segunda línea deben estar los elementos separados por espacios.
    * No hace falta que haya saltos de línea entre filas.
    */
   	void cargar(std::istream &is);

   	/*********************** OPERACIONES ***********************/
   	/**
    * Traspuesta
    * Devuelve una nueva matriz que es la una copia de la original traspuesta.
    */
    Matriz traspuesta();
    
    /**
    * Multiplicar por otra matriz
    * Multiplica 'this'x'otra' y devuleve el resultado.
    */
	Matriz multiplicar(Matriz &otra);
	
    /**
    * Multiplicar por escalar
    * Multiplica 'this'x't' y devuelve el resultado.
    */
   	Matriz multiplicar(double t);
   	
    /**
    * Sumar
    * Suma 'this'+'otra' y devuelve el resultado.
    */
   	Matriz sumar(Matriz &otra);
   	
    /**
    * Restar
    * Resta 'this'-'otra' y devuelve el resultado.
    */
   	Matriz restar(Matriz &otra);
    
    /**
    * Submatriz
    * Devuelve la submatriz de alto n y ancho m que tiene por primer elemento
    * this[i][j].
    */
    Matriz submatriz(int i, int j, int n, int m);

    /************************ VARIOS ************************/
	/**
    * []
    * Permite acceder a los elementos de la matriz usando notación estándar M[i][j].
    */
	vector<double>& operator[] (int i){return _matriz[i];}
    
    /**
    * Mostrar
    * Muestra una matriz por el stream 'os'.
    */
    void mostrar(std::ostream &os);
    
    /**
    * Diagonal
    * Devuelve un vector con la diagonal principal de la matriz.
    */
    vector<double> diagonal();

    /**
    * Intercambiar Filas
    * Intercambia las filas i y j de la matriz
    */

    void intercambiar_filas(int i, int j);
    /************************ Estructura interna *****************************/

    int get_filas() const;
    int get_columnas() const;

private:
    int _numfilas;
	int _numcolumnas;
    vector<vector <double> > _matriz;

};

//void InicializarLU(int filas, int columnas, FactorizacionLU& lu);
struct FactorizacionLU {
    Matriz L;
    Matriz U;
};

class SistemaEcuaciones {
public:
    SistemaEcuaciones(Matriz &A, vector<double> &b);

    /**
    * Muestra el sistema de ecuaciones por el stream 'os'.
    */
    void imprimir_sistema(std::ostream &os);

    /*
    *   Eliminacion Gaussiana
        Dado un vector b, Devuelve el vector de incognitas x que resuelve el sistema Ax = b
        Tiene costo cubico
    */
    vector<double> eliminacion_gaussiana(bool usar_pivoteo_parcial);

    /*
    *   Devuelve la Factorizacion LU de la Matriz A del sistema
        Tiene costo cubico
    */
    FactorizacionLU factorizar_LU();// Asumiendo que en este tp son sistemas que admiten LU.

    /*
    *   Resuelve el sistema en tiempo cuadratico teniendo la factorizacion LU.
    */
    vector<double> resolver_con_LU(FactorizacionLU& lu);

    /*
    *   Cambia el vector de terminos independientes
    */
    void cambiar_b(vector<double> & nuevo_b);

private:
    Matriz _A;
    vector<double> _b;

    void pivoteo_parcial(int i);
};

#endif