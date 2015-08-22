#include "matriz.h"
using namespace std;

/**
* 
* Operaciones entre vectores
* 
*/

void imprimir_vector(vector<double> &vec, std::ostream &os) {
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

void imprimir_sistema(Matriz &A, vector<double> &vec, std::ostream &os) {
    os.precision(5);
    os.setf(ios::fixed,ios::floatfield);

    int num_filas = A.get_filas();
    int num_columnas = A.get_columnas();

    for (int i = 0 ; i< num_filas; i++) {
        os << "[";
        for (int j = 0; j < num_columnas; j++) {
            if (A[i][j] < 0) {
                os.precision(4);  
            } else {
                os.precision(5);  
            } 
            
            if (j == num_columnas-1) {
                os << A[i][j];
            } else {
                os << A[i][j] << " ";
            }
        }
        os << " | ";
        if (vec[i] < 0) {
            os.precision(4);  
        } else {
            os.precision(5);  
        } 
        os << vec[i];
        os << "]" << endl;
    }
    os << endl;
}

void check_dimensiones(int dimA, int dimB, const char* function_name) {
    if (dimA != dimB) {
        cerr << "[Error de dimensiones en funcion: " << function_name << "]" << endl;
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

Matriz::Matriz()
{
    _matriz.clear();
    _numfilas = 0;
    _numcolumnas = 0;
}

Matriz::Matriz(int n, int m)
{
    _numfilas = n;
    _numcolumnas = m;

    vector<double> vec_fila(_numcolumnas, 0/*inicializo con ceros*/);
    _matriz.resize(_numfilas, vec_fila);
}

Matriz::Matriz(int dimension) : Matriz(dimension, dimension)// llamo al constructor de matrices rectangulares
{
    // Inicializo la diagonal con unos.
    for (int i = 0; i < dimension; i++) {
        _matriz[i][i] = 1;
    }
}

Matriz::Matriz(vector<double> &v) : Matriz(v.size(), 1)// llamo al constructor de matrices rectangulares
{
    for (int i = 0; i < _numfilas; i++) {
        _matriz[i][0] = v[i];
    }
}

// Metodo estatico(es un constructor factory porq tiene la misma firma que la que construye vectores columna)
Matriz Matriz::diagonal(vector<double> &v)
{
    Matriz res(v.size(), v.size());
    
    int sizeV = v.size();
    for (int i = 0; i<sizeV; i++) {
        res[i][i] = v[i];
    }
    return res;
}

void Matriz::cargar(std::istream &is)
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

void Matriz::mostrar(std::ostream &os)
{
    os.precision(5);
    os.setf(ios::fixed,ios::floatfield);

	for (int i = 0 ; i<_numfilas; i++) {
		os << "[";
		for (int j = 0; j < _numcolumnas; j++) {
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
        cerr << "Estas pidiendo una submatriz mas grande que la matriz original." << endl;
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

vector<double> Matriz::eliminacion_gaussiana(vector<double> &b) {
    vector<double> vecX(_numfilas, 0);
    
    // Triangular la matriz, dejando la triangular superior equivalente en this->_LU.U
    // Copio la matriz original, teniendo en cuenta que el algoritmo opera sobre la matriz ampliada
    _LU.U = _matriz;
    vector<double> bEquiv = b;

    for (int i = 0; i < _numcolumnas - 1; i++) {
        for (int j = i+1; j < _numfilas; j++) {
            double m = _LU.U[j][i] / _LU.U[i][i];
            for (int k = i; k < _numcolumnas; k++) {
                _LU.U[j][k] -= m * _LU.U[i][k];
            }
            // Tambien hay que modificar el vector b!
            bEquiv[j] -= m * bEquiv[i];
        }
    }

    cout << "Sistema original" << endl;
    imprimir_sistema(*this, b, cout);

    _matriz = _LU.U;
    b = bEquiv;
    
    cout << "Sistema equivalente triangular" << endl;
    imprimir_sistema(*this, b, cout);

    // Calculo X
    for (int i = _numfilas - 1; i >= 0; i--) {
        // Obtener suma de la fila por el b
        double sumaAcum = 0;
        for (int j = i+1; j < _numcolumnas; j++) {
            if(i<_numfilas && j < _numcolumnas)
                sumaAcum += _LU.U[i][j] * vecX[j];
        }

        // Despejar el xi
        vecX[i] = (b[i] - sumaAcum) / _LU.U[i][i];
    }

    return vecX;
}


vector <double> Matriz::resolver_sistema(vector<double> &b)
{
    if (!LU_hecha) {
        descomposicion_LU();
    }

    vector<double> y(b.size());
    vector<double> x(b.size());

    // permuto b
    vector<double> tmp(b.size());
    for (int i = 0; i<_numfilas; i++) {
        for (int j = 0; j < _numfilas; j++) {
            if (_LU.P[i][j] == 1) {
                tmp[i]=b[j];
            }
        }
    }
    b = tmp;

    // calculo Y
    for (int i = 0 ; i<_numfilas; i++) {
        double suma = 0;
        for (int j = 0; j < i; j++) {
            suma += _LU.L[i][j] * y[j];
        }
        y[i] = (b[i]-suma) / _LU.L[i][i];
    }

    // calculo X
    for (int i=_numfilas-1 ; i>=0; i--) {
        double suma=0;
        for (int j=i+1; j < _numfilas; j++) {
            suma += _LU.U[i][j] * x[j];
        }
        x[i] = (y[i]-suma) / _LU.U[i][i];
    }
    return x;
}

void Matriz::descomposicion_LU()
{
    _LU.L = _matriz;
    _LU.U = _matriz;
    _LU.P = _matriz;

    // P = ID
    for (int i = 0; i<_numfilas; i++) {
        for (int j = 0; j < _numcolumnas; j++) {
            if (i==j) {
                _LU.P[i][j]=1;
            } else {
                _LU.P[i][j]=0;
            }
        }
    }

    for (int i = 0; i<_numfilas; i++) {
        if (_LU.L[i][i]==0) {
            pivotear(i);
        }

        for (int j=i+1; j < _numcolumnas; j++) {
            for (int k=_numfilas-1; k>=i; k--) {
                if (k == i) {
                    _LU.L[j][k] = (_LU.L[j][i] / _LU.L[i][i]);
                } else {
                    _LU.L[j][k] -= (_LU.L[j][i] / _LU.L[i][i]) * _LU.L[i][k];
                }
                _LU.U[j][k] -= (_LU.U[j][i] / _LU.U[i][i]) * _LU.U[i][k];
            }
        }
    }

    // le borro la parte triangular superior a L
    for (int i = 0; i<_numfilas; i++) {
        for (int j = 0; j < _numcolumnas; j++) {
            if (j==i) _LU.L[i][i] = 1;
            if (j>i) _LU.L[i][j] = 0;
        }
    }
    LU_hecha = true;
}

void Matriz::pivotear(int i)
{
    bool encontroFilaNoNula = false;
    for (int j=i+1; (j < _numfilas && !encontroFilaNoNula); j++) {
        if (_LU.L[j][i] != 0) {
            swap(_LU.P[i], _LU.P[j]);
            swap(_LU.L[i], _LU.L[j]);
            swap(_LU.U[i], _LU.U[j]);            
            encontroFilaNoNula = true;
        }
    }
}