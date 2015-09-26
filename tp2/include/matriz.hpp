#ifndef MATRIZ_H
#define MATRIZ_H

#include <stdlib.h>
#include <map>
#include <unordered_map>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <math.h>
//#include <sparse_vector.tpp>
//#include <dok_matrix.tpp>
//#include <csr_matrix.tpp>

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

#endif
