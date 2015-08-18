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
class Matriz;

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
* Agrega funcionalidades útiles a un vector de vectores de double.
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
    Matriz (int n);
	/**
    * Constructor
    * Toma el alto (primer parámetro) y el ancho (segundo parámetro)
    * y devuelve una matriz del tamaño especificado inicializada con ceros
    * y con precisión máxima.
    */
    Matriz (int n, int m);
    /**
    * Constructor
    * Toma el alto (primer parámetro), el ancho (segundo parámetro), y la precisión
    * y devuelve una matriz del tamaño especificado inicializada con ceros.
    */
    Matriz (int n, int m, int precision);
    /**
    * Constructor a partir de un vector
    * Toma un vector no vacío de double y lo castea a matriz de R^(n x 1) (vector columna)
    * con la precisión del vector.
    */
    Matriz (vector<double> v);
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
   	void cargar (std::istream &is);

   	/*********************** OPERACIONES entre matrices *****************************/
   	/**
    * Trasponer
    * Devuelve una nueva matriz que es la una copia de la original traspuesta.
    */
    Matriz traspuesta();
    /**
    * Multiplicar por otra matriz
    * Multiplica 'this'x'otra'.
    */
	Matriz multiplicar(Matriz otra);
	/**
    * Multiplicar por escalar
    * Multiplica 'this'x't'.
    */
   	Matriz multiplicar(double t);
   	/**
    * Sumar
    * Suma dos matrices.
    */
   	Matriz sumar(Matriz otra);
   	/**
    * Restar
    * Resta 'this'-'otra'.
    */
   	Matriz restar(Matriz otra);

   	/******************* CÁLCULO DE AUTOVALORES / AUTOVECTORES **********************/

    /**
    * QR
    * Devuelve un vector de matrices con Q como primera componente y R
    * como segunda componente. Requiere que la matriz sea simétrica.
    */
    vector<Matriz> qr();
    vector<Matriz> qr_rapido();
 	// Auxiliares para el cálculo de QR y QR rápido
    vector< vector<double> > get_columnas();
    void restar_submatriz(Matriz otra);
    void multiplicar_submatriz(Matriz otra);
    void ID_restar();

    /**
    * Autovectores
    * Devuelve una matriz con filas iguales a los autovectores de this,
    * ordenados según el módulo del autovalor correspondiente de mayor a menor.
    */
    vector<Matriz> autovalores(double tol, int max_it);
    Matriz autovectores(double tol, int max_it){return autovalores(tol, max_it)[1];}

    /************************ VARIOS *****************************/

	/**
    * []
    * Permite acceder a los elementos de la matriz usando notación estándar M[i][j].
    */
	vector<double>& operator[] (int i){return matriz[i];}
    /**
    * Mostrar
    * Muestra una matriz por 'os' (que puede ser cout).
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
