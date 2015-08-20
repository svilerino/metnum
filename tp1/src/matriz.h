#ifndef MATRIZ_H
#define MATRIZ_H

#include <stdlib.h>
#include <vector>
#include <fstream>
#include <math.h>
#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;

/**
* Resta de vectores
*/
vector<double> restar(vector<double>, vector<double>);
/**
* Normas de vectores
*/
double norma2(vector<double>);
double norma1(vector<double>);

class Matriz {
/**
* Matriz
* Representa una matriz de doubles
*/
public:
    /*********************** Constructores y cargar ***********************/
	/**
    * Constructor por defecto
    */
	Matriz(){matriz.clear();};
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
    Matriz(vector<double> v);
    /**
    * Constructor de matriz diagonal
    * Toma un vector no vacío de double y lo convierte en una matriz de (n x n) que lo
    * tiene como diagonal.
    */
    Matriz diagonal(vector<double>);
    /**
    * Cargar
    * Carga una matriz de un archivo. La primer línea del archivo debe ser,
    * en este orden, dos enteros que especifiquen el alto y el ancho.
    * A partir de la segunda línea deben estar los elementos separados por espacios.
    * No hace falta que haya saltos de línea entre filas.
    */
   	void cargar(std::istream &is);

   	/*********************** OPERACIONES entre matrices *****************************/
   	/**
    * Traspuesta
    * Devuelve una nueva matriz que es la una copia de la original traspuesta.
    */
    Matriz traspuesta();
    /**
    * Multiplicar por otra matriz
    * Multiplica 'this'x'otra' y devuleve el resultado.
    */
	Matriz multiplicar(Matriz otra);
	/**
    * Multiplicar por escalar
    * Multiplica 'this'x't' y devuelve el resultado.
    */
   	Matriz multiplicar(double t);
   	/**
    * Sumar
    * Suma dos matrices y devuelve el resultado.
    */
   	Matriz sumar(Matriz otra);
   	/**
    * Restar
    * Resta 'this'-'otra' y devuelve el resultado.
    */
   	Matriz restar(Matriz otra);

    /************************ VARIOS *****************************/

	/**
    * []
    * Permite acceder a los elementos de la matriz usando notación estándar M[i][j].
    */
	vector<double>& operator[] (int i){return matriz[i];}
    /**
    * Mostrar
    * Muestra una matriz por el stream 'os' (que puede ser cout).
    */
    void mostrar(std::ostream &os);
    /**
    * Diagonal
    * Devuelve un vector con la diagonal principal de la matriz.
    */
    vector<double> diagonal();

    /************************ Estructura interna *****************************/

    int alto;
	int ancho;

private:
    vector<vector <double> > matriz;
};

#endif
