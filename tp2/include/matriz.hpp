#ifndef MATRIZ_H
#define MATRIZ_H

#include<stdlib.h>
#include<map>
#include<unordered_map>
#include<fstream>
#include<cmath>
#include<iomanip>
#include<iostream>
#include<algorithm>
#include<math.h>
#include<sparse_vector.tpp>
#include<dok_matrix.tpp>

using namespace std;

/**
* Operaciones de vectores
*/
void mostrar_vector(vector<double> &vec, ostream &os);
void mostrar_vector(vector<int> &vec, ostream &os);
void check_dimensiones(int dimA, int dimB,  char* function_name);
void sumar(vector<double> &a, vector<double> &b, vector<double> &res);
void restar(vector<double> &a, vector<double> &b, vector<double> &res);
double producto_interno(vector<double>&, vector<double>&);
double normap(vector<double>&, int p);
void normalizar(vector<double> &v, int p);

/**
* Hash para pares de enteros
*/
struct hash_pair{
public:
    size_t operator()(pair<int, int> p) const {
        // http://www.boost.org/doc/libs/1_53_0/doc/html/hash/reference.html#boost.hash_combine
        size_t seed = 0;
        seed ^= hash<int>()(p.first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash<int>()(p.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    };
};

/**
* Typedefs útiles
*/
typedef pair<pair<int, int>, double> values_with_index;
typedef unordered_map<pair<int, int>, double, hash_pair> dict_of_keys;
typedef unsigned int uint;


/**
* Dictionary of Keys
*/
class MatrizDoK {
public:
    MatrizDoK() : _numfilas(0), _numcolumnas(0), _dict() {}
    MatrizDoK(int filas, int columnas) : _numfilas(filas), _numcolumnas(columnas), _dict() {}
    MatrizDoK(istream &is);

    /**
    * Operador ()
    * Permite acceder al elemento i, j de la matriz usando la notación A(i, j)
    */
    double operator()(int fila, int columna) ;
    /**
    * Set
    * Permite asignar un valor al elemento i, j de la matriz
    */
    void set(int fila, int columna, double valor);
    /**
    * Cantidad de filas y columnas
    */
    int filas() {return _numfilas;}
    int columnas() {return _numcolumnas;}

    /************************ ELEMENTOS ORDENADOS ************************/
    /**
    * Elementos en orden
    * Recibe un vector vacío y lo llena con triplas < <int, int>, double> que indican
    * los índices y valores de los elementos no nulos de la matriz, ordenados de
    * arriba hacia abajo y de izquierda a derecha.
    */
    void elementos_ordenado(vector<values_with_index> &elements) ;

    /************************ OUTPUT ************************/
    /**
    * Salida
    * Imprime la matriz completa en un stream de salida.
    */
    void mostrar(ostream &os) ;
    /**
    * Salida comprimida
    * Imprime los elementos no nulos de la matriz en un stream de salida.
    */
    void mostrar_esparsa(ostream &os) ;

private:
    int _numfilas;
    int _numcolumnas;
    dict_of_keys _dict;
};

/**
* MatrizCSR
* Representa una matriz esparsa de doubles usando Compressed Sparse Row
* La matriz es inmutable: una vez ruida no se puede modificar
*/
class MatrizCSR{
public:
    /************************ RUCTORES ************************/
    /**
    * ructor por defecto
    */
    MatrizCSR() : _numfilas(0), _numcolumnas(0), _values(), _columns_index(), _rows_start() {}
    /**
    * ructor a partir de Matriz DoK
    */
    MatrizCSR(MatrizDoK &dok);

    /************************ GETTERS ************************/
    /**
    * get_fila
    * Carga en 'elementos' y en 'columnas' los valores e índices de columna de la fila 'fila'
    */
    void get_fila(int fila, vector<double> &elementos, vector<int> &columnas) ;
    /**
    * Operador ()
    * Permite acceder al elemento i, j de la matriz usando la notación A(i, j)
    */
    double operator()(int fila, int columna) ;
    /**
    * Cantidad de filas y columnas
    */
    int filas() {return _numfilas;}
    int columnas() {return _numcolumnas;}

    /************************ OUTPUT ************************/
    /**
    * Salida
    * Imprime la matriz completa en un stream de salida.
    */
    void mostrar(ostream &os) ;
    /**
    * Salida comprimida
    * Imprime los elementos no nulos de la matriz en un stream de salida.
    */
    void mostrar_esparsa(ostream &os) ;

private:
    int _numfilas;
    int _numcolumnas;
    vector<double> _values;
    vector<int> _columns_index;
    vector<int> _rows_start;
};

#endif
