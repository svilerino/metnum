#include "matriz.h"
using namespace std;
#define DEBUG_MESSAGES_ON 0//comentar esta linea para no hacer chequeos costosos en tiempo de ejecucion
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

void sumar(vector<double> &a, vector<double> &b, vector<double> &res)
{
    check_dimensiones(a.size(), b.size(), __FUNCTION__);
    int sizeA = a.size();

    res.resize(sizeA);

    for (int i = 0; i < sizeA; i++) {
        res[i] = a[i] + b[i];
    }
}

void restar(vector<double> &a, vector<double> &b, vector<double> &res)
{
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

void normalizar(vector<double> &v, int norma)
{
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

int Matriz::get_filas() const
{
    return _numfilas;
}

int Matriz::get_columnas() const
{
    return _numcolumnas;
}

void Matriz::resize(int n, int m){
    _numfilas = n;
    _numcolumnas = m;

    vector<double> vec_fila(_numcolumnas, 0/*inicializo con ceros*/);
    _matriz.resize(_numfilas, vec_fila);
}

Matriz::Matriz()
{
    _matriz.clear();
    _numfilas = 0;
    _numcolumnas = 0;
}

Matriz::Matriz(int n, int m)
{
    resize(n, m);   // resize inicializa con ceros
}

Matriz::Matriz(int dimension) : Matriz(dimension, dimension) // reutilizo el constructor de matrices rectangulares
{
    // inicializo la diagonal con unos.
    for (int i = 0; i < dimension; i++) {
        _matriz[i][i] = 1;
    }
}

Matriz::Matriz(vector<double> &v) : Matriz(v.size(), 1) // reutilizo de matrices rectangulares
{
    for (int i = 0; i < _numfilas; i++) {
        _matriz[i][0] = v[i];
    }
}

// Metodo estatico (es un constructor factory porq tiene la misma firma que la que construye vectores columna)
Matriz Matriz::diagonal(vector<double> &v)
{
    Matriz res(v.size(), v.size());
    
    int sizeV = v.size();
    for (int i = 0; i<sizeV; i++) {
        res[i][i] = v[i];
    }
    return res;
}

void Matriz::cargar(istream &is)
{
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

void Matriz::mostrar(ostream &os)
{
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

void Matriz::mostrar_esparsa(ostream &os)
{
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

Matriz Matriz::traspuesta()
{
    Matriz res(_numcolumnas, _numfilas);

    for (int i = 0; i<_numfilas; i++) {
        for (int j = 0; j < _numcolumnas; j++) {
            res[j][i] = _matriz[i][j];
        }
    }
    return res;
}

Matriz Matriz::multiplicar(Matriz &a)
{
    check_dimensiones(_numcolumnas, a._numfilas, __FUNCTION__);

    Matriz b (_numfilas, a._numcolumnas);
    for (int i = 0; i < _numfilas; i++) {
        for (int j = 0; j < a._numcolumnas; j++) {
            b[i][j] = 0;
            for (int k = 0; k < _numcolumnas; k++) {
                b[i][j] = b[i][j] + _matriz[i][k] * a[k][j];
            }
        }
    }
    return b;
}

Matriz Matriz::multiplicar(double t)
{
    Matriz res(_numfilas, _numcolumnas);

    for (int i = 0; i < _numfilas; i++) {
        for (int j = 0; j < _numcolumnas; j++) {
            res[i][j] = _matriz[i][j] * t;
        }
    }
    return res;
}

Matriz Matriz::sumar(Matriz &otra)
{
    check_dimensiones(_numfilas, otra._numfilas, __FUNCTION__);
    check_dimensiones(_numcolumnas, otra._numcolumnas, __FUNCTION__);

    Matriz res(_numfilas, _numcolumnas);
    for (int i = 0; i < _numfilas; i++) {
        for (int j = 0; j < _numcolumnas; j++) {
            res[i][j] = _matriz[i][j] + otra[i][j];
        }
    }
    return res;
}

Matriz Matriz::restar(Matriz &otra)
{
    check_dimensiones(_numfilas, otra._numfilas, __FUNCTION__);
    check_dimensiones(_numcolumnas, otra._numcolumnas, __FUNCTION__);

    Matriz res(_numfilas, _numcolumnas);
    for (int i = 0; i < _numfilas; i++) {
        for (int j = 0; j < _numcolumnas; j++) {
            res[i][j] = _matriz[i][j] - otra[i][j];
        }
    }
    return res;
}

Matriz Matriz::submatriz(int y, int x, int n, int m)
{
    if (n > _numfilas || m > _numcolumnas) {
        cerr << "\e[0;31m" << "[Error] Estas pidiendo una submatriz mas grande que la matriz original." << "\e[0m[";
        exit(-1);
    }

    Matriz res(n, m);
    for (int i = 0; i<res._numfilas; i++) {
        for (int j = 0; j < res._numcolumnas; j++) {
            if ((i+y < _numfilas) && (j+x < _numcolumnas)) {
                res[i][j] = _matriz[i+y][j+x];
            }
        }
    }
    return res;
}

vector<double> Matriz::diagonal()
{
    vector<double> diag;
    for (int i = 0; i<_numfilas; i++) {
        for (int j = 0; j < _numcolumnas; j++) {
            if (i==j) {
                diag.push_back(_matriz[i][j]);
            }
        }
    }
    return diag;
}

void Matriz::intercambiar_filas(int i, int j) {
    if (i > _numfilas || j > _numfilas) {
        cerr << "\e[0;31m" << "[Error] Estas tratando de intercambiar filas con indices erroneos." << "\e[0m[";
        exit(-1);
    }
    swap(_matriz[i], _matriz[j]);
}

/**
* 
* Implementacion clase SistemaEcuaciones
* 
*/
void SistemaEcuaciones::imprimir_sistema(ostream &os) {
    os.precision(5);
    os.setf(ios::fixed,ios::floatfield);

    int num_filas = _A.get_filas();
    int num_columnas = _A.get_columnas();

    for (int i = 0 ; i< num_filas; i++) {
        os << "[";
        for (int j = 0; j < num_columnas; j++) {
            if (_A[i][j] < 0) {
                os.precision(4);  
            } else {
                os.precision(5);  
            } 
            if (j == num_columnas-1) {
                os << _A[i][j];
            } else {
                os << _A[i][j] << " ";
            }
        }
        os << " | ";
        if (_b[i] < 0) {
            os.precision(4);  
        } else {
            os.precision(5);  
        } 
        os << _b[i];
        os << "]" << endl;
    }
    os << endl;
}

void SistemaEcuaciones::eliminacion_gaussiana(bool usar_pivoteo_parcial, vector<double> &vec_sol) {
    int numfilas = _A.get_filas();
    int numcolumnas = _A.get_columnas();

    // Triangular la matriz ampliada del sistema
    for (int i = 0; i < numcolumnas - 1; i++) {
        if (usar_pivoteo_parcial) {
            pivoteo_parcial(i);
        }

        for (int j = i+1; j < numfilas; j++) {

            if (abs(_A[j][i]) < EPSILON_TOLERANCIA) {
                // Ya hay un cero allí, no hay nada que hacer
                continue;
            }

            // Calculo el coeficiente multiplicador
            double m = _A[j][i] / _A[i][i];
            
            #ifdef DEBUG_MESSAGES_ON
                if ( abs(_A[i][i]) < EPSILON_TOLERANCIA) {
                    cerr << "\e[0;31m[ERROR] DIVISION POR CERO EN PIVOTE DE GAUSS\e[0m";
                    exit(-1);
                } 
            #endif
            
            // Opero sobre la fila 
            for (int k = i; k < numcolumnas; k++) {
                _A[j][k] -= m * _A[i][k];
            }

            // Tambien hay que modificar el vector b!
            _b[j] -= m * _b[i];
        }
    }
    // Calculo el vector X de soluciones con backward substitution.
    for (int i = numfilas - 1; i >= 0; i--) {
        // Obtener suma de la fila por el b
        double sumaAcum = 0;
        for (int j = i+1; j < numcolumnas; j++) {
            if (i<numfilas && j < numcolumnas) {
                sumaAcum += _A[i][j] * vec_sol[j];
            }
        }

        // Despejar el xi
        vec_sol[i] = (_b[i] - sumaAcum) / _A[i][i];
    }
}

void SistemaEcuaciones::pivoteo_parcial(int i) {
    // Busco la maxima fila debajo de la i-esima
    int numfilas = _A.get_filas();
    int maxI = i;

    for (int j = i + 1; j < numfilas; j++) {
        if (_A[j][i] > _A[maxI][i]) {
            maxI = i;
        }
    }
    // Si es una fila distinta, las swapeo para lograr tener un pivote maximo.
    if ( i != maxI ) {
        _A.intercambiar_filas(i, maxI);
        swap(_b[i], _b[maxI]);
    }
}

void SistemaEcuaciones::cambiar_b(vector<double> & nuevo_b) {
    _b = nuevo_b;
}

void SistemaEcuaciones::cambiar_a(Matriz & nuevo_a) {
    _A = nuevo_a;
}

void SistemaEcuaciones::resolver_con_LU(FactorizacionLU& factorizacion, vector<double> &resX) {
    int sizeB = _b.size();
    vector<double> resY(sizeB);

    int numfilas = _A.get_filas();

    // Calculo resY
    for (int i = 0 ; i < numfilas; i++) {
        double suma = 0;
        for (int j = 0; j < i; j++) {
            suma += factorizacion._L[i][j] * resY[j];
        }
        resY[i] = (_b[i] - suma) / factorizacion._L[i][i];
    }

    // Calculo resX
    for (int i = numfilas - 1 ; i>=0; i--) {
        double suma = 0;
        for (int j=i+1; j < numfilas; j++) {
            suma += factorizacion._U[i][j] * resX[j];
        }
        resX[i] = (resY[i] - suma) / factorizacion._U[i][i];
    }
}

void SistemaEcuaciones::factorizar_LU(FactorizacionLU& lu) {
    int numfilas = _A.get_filas();
    int numcolumnas = _A.get_columnas();

    // L identidad
    lu._L.resize(numfilas, numcolumnas);

    for (int i = 0; i<numfilas; i++) {
        lu._L[i][i] = 1;
    }
    
    // U comienza siendo la matriz A
    lu._U = _A;

    for (int i = 0; i < numcolumnas - 1; i++) {
        for (int j = i+1; j < numfilas; j++) {

            if (abs(lu._U[j][i]) < EPSILON_TOLERANCIA) {
                // Ya hay un cero allí, no hay nada que hacer
                continue;
            }
            
            #ifdef DEBUG_MESSAGES_ON
                if ( abs(lu._U[i][i]) < EPSILON_TOLERANCIA) {
                    cerr << "\e[0;31m[ERROR] DIVISION POR CERO (U[i][i] == 0) en Factorizacion LU\e[0m";
                    exit(-1);
                }
            #endif

            // Calculo el coeficiente multiplicador para L y U
            double multiplicadorU = (lu._U[j][i] / lu._U[i][i]);

            // Opero sobre la fila 
            for (int k = i; k < numcolumnas; k++) {
                // modifico los elementos de U
                lu._U[j][k] -= multiplicadorU * lu._U[i][k];
            }

            // Guardo los elementos de L
            lu._L[j][i] = multiplicadorU;
        }
    }
}