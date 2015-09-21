#include "matriz.h"
using namespace std;
#define DEBUG_MESSAGES_ON 0 //comentar esta linea para no hacer chequeos costosos en tiempo de ejecucion
#define EPSILON_TOLERANCIA 0.0001

/**
* 
* Operaciones entre vectores
* 
*/

void imprimir_vector(vector<double> &vec, ostream &os) {
    os.precision(5);
    os.setf(ios::fixed,ios::floatfield);
    os << "[";
    int sizeV = vec.size();
    for (int i = 0; i < sizeV; i++)
    {
        os << vec[i] << (i == sizeV-1 ? "" : ", ");
    }
    os << "]" << endl;
}

void check_dimensiones(int dimA, int dimB, const char* function_name) {
    if (dimA != dimB) {
        cerr << "\e[0;31m" << "[Error] Fallo el check de dimensiones en funcion: " << function_name << "]" << "\e[0m[";
        exit(-1);
    }
}

void sumar(vector<double> &a, vector<double> &b, vector<double> &res) {
    check_dimensiones(a.size(), b.size(), __FUNCTION__);
    int sizeA = a.size();

    res.resize(sizeA);

    for (int i = 0; i < sizeA; i++) {
        res[i] = a[i] + b[i];
    }
}

void restar(vector<double> &a, vector<double> &b, vector<double> &res) {
    check_dimensiones(a.size(), b.size(), __FUNCTION__);
    
    int sizeA = a.size();
    for (int i = 0; i < sizeA; i++) {
        res[i] = a[i] - b[i];
    }
}

double normap(vector<double> &v, int p) {
    double acum = 0;
    int sizeV = v.size();

    for (int i = 0; i<sizeV; i++) {
        acum = acum + pow(abs(v[i]), p);
    }
    return pow(acum, 1/(double)p);
}

void normalizar(vector<double> &v, int norma) {
    double norm = normap(v, norma);
    int sizeV = v.size();
    for (int i = 0; i < sizeV; i++) {
        v[i] = v[i] / norm;
    }
}

double producto_interno(vector<double>& vec1, vector<double>& vec2) {
    double res = 0;
    check_dimensiones(vec1.size(), vec2.size(), __FUNCTION__);    
    
    int sizeV = vec1.size();
    for (int i = 0; i < sizeV; i++) {
        res += (vec1[i] * vec2[i]);
    }
    return res;
}

/**
* 
* Implementacion clase Matriz
* 
*/
Matriz::Matriz() {
    _matriz.clear();
    _numfilas = 0;
    _numcolumnas = 0;
}

Matriz::Matriz(int n, int m) {
    resize(n, m);   // resize inicializa con ceros
}

void Matriz::resize(int n, int m) {
    _numfilas = n;
    _numcolumnas = m;

    vector<double> vec_fila(_numcolumnas, 0/*inicializo con ceros*/);
    _matriz.resize(_numfilas, vec_fila);
}

void Matriz::cargar(istream &is) {
	is >> _numfilas;
    is >> _numcolumnas;
    
    _matriz.clear();
    
    vector<double> fila;
	for (int i = 0; i < _numfilas; i++) {
		for (int j = 0; j < _numcolumnas; j++) {
		    double val;
			is >> val;
			fila.push_back(val);
		}
		_matriz.push_back(fila);
		fila.clear();
	}
}

void Matriz::mostrar(ostream &os) {
    os.precision(5);
    os.setf(ios::fixed,ios::floatfield);

	for (int i = 0 ; i<_numfilas; i++) {
		os << "[";
		for (int j = 0; j < _numcolumnas; j++) {
		    // seteo la precisión en función del signo para obtener ancho fijo
            if (_matriz[i][j] < 0) {
                os.precision(4);  
            } else {
                os.precision(5);  
            } 
           
            if (j == _numcolumnas-1) {
                os << _matriz[i][j];
            } else {
                os << _matriz[i][j] << ", ";
            }
        }
        os << "]" << endl;
    }
    os << endl;
}

void Matriz::mostrar_esparsa(ostream &os) {
    os.precision(5);
    os.setf(ios::fixed,ios::floatfield);

    for (int i = 0 ; i<_numfilas; i++) {
        for (int j = 0; j < _numcolumnas; j++) {
            if (_matriz[i][j] < 0) {
                os.precision(4);  
            } else if (_matriz[i][j] > 0) {
                os.precision(5);  
            } else {
                continue;
            }
            os << "(" << i << ", " << j << ") = " << _matriz[i][j] << endl;
        }
    }
    os << endl;
}
