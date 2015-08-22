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

void Matriz::intercambiar_filas(int i, int j) {
    if (i > _numfilas || j > _numfilas) {
        cerr << "Estas tratando de intercambiar filas con indices erroneos." << endl;
        exit(-1);
    }

    swap(_matriz[i], _matriz[j]);
}


//void InicializarLU(int filas, int columnas, FactorizacionLU& lu) {
    // lu.L es lower(tiene la parte superior con ceros y unos en la diagonal)
    // lu.U es upper(tiene la parte inferior con ceros)
    // Voy a asumir que las diagonales de ambas pueden tener valores
    
    // La matriz L va a ser del tipo
    // a11  0   0   0  ... 0
    // a21 a22  0   0  ... 0
    // a31 a32 a33  0  ... 0
    // a41 a42 a43 a44 ... 0
    // ...
    // an1 an2 an3 an4 ... ann

    // La matriz U va a ser del tipo
    // a11 a12 a13 a14 ... a1n
    //  0  a22 a23 a24 ... a1n
    //  0   0  a33 a34 ... a1n
    //  0   0   0  a44 ... a1n
    // ...
    //  0   0   0   0  ... ann

    // El modo de representar esto eficientemente en memoria lo voy a plantear asi...
    // Las matrices se representan como vectores de vectores de doubles, entonces...
    // Para la matriz L:
    // la fila i-esima va a tener n-i columnas y respetando los limites obvios de indexacion se indexa igual.

    // Para la matriz U:
    // la fila i-esima va a tener i columnas pero para indexarla se debe hacer lo siguiente.
    // U[i][j] va a ser equivalente a U[i][j]
//}

/**
* 
* Implementacion clase SistemaEcuaciones
* 
*/

SistemaEcuaciones::SistemaEcuaciones(Matriz &A, vector<double> &b) {
    _A = A;
    _b = b;
}

void SistemaEcuaciones::imprimir_sistema(std::ostream &os) {
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

vector<double> SistemaEcuaciones::eliminacion_gaussiana(bool usar_pivoteo_parcial) {
    cout << "Sistema original" << endl;
    imprimir_sistema(cout);

    int numfilas = _A.get_filas();
    int numcolumnas = _A.get_columnas();

    vector<double> vecSol(numfilas, 0);
    
    // Triangular la matriz ampliada del sistema
    for (int i = 0; i < numcolumnas - 1; i++) {
        if(usar_pivoteo_parcial)
        {
            pivoteo_parcial(i);
        }

        for (int j = i+1; j < numfilas; j++) {
            // Calculo el coeficiente multiplicador
            double m = _A[j][i] / _A[i][i];
            
            // Opero sobre la fila 
            for (int k = i; k < numcolumnas; k++) {
                _A[j][k] -= m * _A[i][k];
            }
            // Tambien hay que modificar el vector b!
            _b[j] -= m * _b[i];
        }
    }
    
    cout << "Sistema equivalente triangular" << endl;
    imprimir_sistema(cout);

    // Calculo el vector X de soluciones con backward substitution.
    for (int i = numfilas - 1; i >= 0; i--) {
        // Obtener suma de la fila por el b
        double sumaAcum = 0;
        for (int j = i+1; j < numcolumnas; j++) {
            if(i<numfilas && j < numcolumnas)
                sumaAcum += _A[i][j] * vecSol[j];
        }

        // Despejar el xi
        vecSol[i] = (_b[i] - sumaAcum) / _A[i][i];
    }

    return vecSol;
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
    if( i != maxI ) {
        _A.intercambiar_filas(i, maxI);
        swap(_b[i], _b[maxI]);
    }
}

void SistemaEcuaciones::cambiar_b(vector<double> & nuevo_b) {
    _b = nuevo_b;
}

vector<double> SistemaEcuaciones::resolver_con_LU(FactorizacionLU& lu) {
    vector<double> res;
    return res;
}

FactorizacionLU SistemaEcuaciones::factorizar_LU() {
    FactorizacionLU lu;

    return lu;
}

//vector <double> Matriz::resolver_sistema(vector<double> &b)
//{
//    if (!LU_hecha) {
//        descomposicion_LU();
//    }
//
//    vector<double> y(b.size());
//    vector<double> x(b.size());
//
//    // permuto b
//    vector<double> tmp(b.size());
//    for (int i = 0; i<_numfilas; i++) {
//        for (int j = 0; j < _numfilas; j++) {
//            if (_LU.P[i][j] == 1) {
//                tmp[i]=b[j];
//            }
//        }
//    }
//    b = tmp;
//
//    // calculo Y
//    for (int i = 0 ; i<_numfilas; i++) {
//        double suma = 0;
//        for (int j = 0; j < i; j++) {
//            suma += _LU.L[i][j] * y[j];
//        }
//        y[i] = (b[i]-suma) / _LU.L[i][i];
//    }
//
//    // calculo X
//    for (int i=_numfilas-1 ; i>=0; i--) {
//        double suma=0;
//        for (int j=i+1; j < _numfilas; j++) {
//            suma += _LU.U[i][j] * x[j];
//        }
//        x[i] = (y[i]-suma) / _LU.U[i][i];
//    }
//    return x;
//}

//void Matriz::descomposicion_LU()
//{
//    _LU.L = _matriz;
//    _LU.U = _matriz;
//    _LU.P = _matriz;
//
//    // P = ID
//    for (int i = 0; i<_numfilas; i++) {
//        for (int j = 0; j < _numcolumnas; j++) {
//            if (i==j) {
//                _LU.P[i][j]=1;
//            } else {
//                _LU.P[i][j]=0;
//            }
//        }
//    }
//
//    for (int i = 0; i<_numfilas; i++) {
//        if (_LU.L[i][i]==0) {
//            pivotear(i);
//        }
//
//        for (int j=i+1; j < _numcolumnas; j++) {
//            for (int k=_numfilas-1; k>=i; k--) {
//                if (k == i) {
//                    _LU.L[j][k] = (_LU.L[j][i] / _LU.L[i][i]);
//                } else {
//                    _LU.L[j][k] -= (_LU.L[j][i] / _LU.L[i][i]) * _LU.L[i][k];
//                }
//                _LU.U[j][k] -= (_LU.U[j][i] / _LU.U[i][i]) * _LU.U[i][k];
//            }
//        }
//    }
//
//    // le borro la parte triangular superior a L
//    for (int i = 0; i<_numfilas; i++) {
//        for (int j = 0; j < _numcolumnas; j++) {
//            if (j==i) _LU.L[i][i] = 1;
//            if (j>i) _LU.L[i][j] = 0;
//        }
//    }
//    LU_hecha = true;
//}

//void Matriz::pivotear(int i)
//{
//    bool encontroFilaNoNula = false;
//    for (int j=i+1; (j < _numfilas && !encontroFilaNoNula); j++) {
//        if (_LU.L[j][i] != 0) {
//            swap(_LU.P[i], _LU.P[j]);
//            swap(_LU.L[i], _LU.L[j]);
//            swap(_LU.U[i], _LU.U[j]);            
//            encontroFilaNoNula = true;
//        }
//    }
//}
