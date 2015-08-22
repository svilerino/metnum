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
    
    /*
    * Eliminacion Gaussiana: Dado un vector b, devuelve el vector de incognitas x que resuelve el sistema Ax = b
    */
    vector<double> eliminacion_gaussiana(vector<double> &b);

    /**
    * Resolver sistema
    * Dado un vector b, devuelve el vector solución del sistema Ax=b.
    * Si no está hecha ya, realiza y almacena la factorización LU para acelerar
    * futuras llamadas.
    */
    vector<double> resolver_sistema(vector <double> &b);

    /*********************** FACTORIZACIÓN ***********************/
    /**
    * Descomposición LU
    * Calcula la descomposición PLU de la matriz y la almacena en la estructura.
    */
    void descomposicion_LU();
    void pivotear(int i);

    /************************ VARIOS ************************/
	/**
    * []
    * Permite acceder a los elementos de la matriz usando notación estándar M[i][j].
    */
	vector<double>& operator[] (int i){return _matriz[i];}
    
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

    int get_filas() const;
    int get_columnas() const;

private:
    int _numfilas;
	int _numcolumnas;
    vector<vector <double> > _matriz;
    
    struct Lu{
        vector<vector <double> > P;
        vector<vector <double> > L;
        vector<vector <double> > U;
    };

    Lu _LU;
    
    bool LU_hecha; // true <=> fue calculada la factorización LU
};

void imprimir_sistema(Matriz &A, vector<double> &vec, std::ostream &os);

#endif
