#include "matriz.h"
using namespace std;
#define DEBUG_MESSAGES_ON 0 //comentar esta linea para no hacer chequeos costosos en tiempo de ejecucion
#define EPSILON_TOLERANCIA 0.0001

/**
* 
* Operaciones entre vectores
* 
*/
void mostrar_vector(vector<double> &vec, ostream &os) {
    os.precision(5);
    os.setf(ios::fixed,ios::floatfield);
    os << "[";
    for (uint i = 0; i < vec.size(); i++) {
        os << vec[i] << (i == vec.size()-1 ? "" : ", ");
    }
    os << "]" << endl;
}

void mostrar_vector(vector<int> &vec, ostream &os) {
    os << "[";
    for (uint i = 0; i < vec.size(); i++) {
        os << vec[i] << (i == vec.size()-1 ? "" : ", ");
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

void normalizar(vector<double> &v, int p) {
    double norm = normap(v, p);
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
* MatrizDoK
*/
MatrizDoK::MatrizDoK(istream &is) {

}

double MatrizDoK::operator()(int fila, int columna) {
    if (fila >= filas() || columna >= columnas()) {
        cerr << "Se intentó acceder a una posición inexistente de una matriz DoK";
        exit(-1);    
    }
    if (_dict.count(pair<int,int>(fila, columna)) == 0) {
        return 0;
    } else {
        return _dict[pair<int,int>(fila, columna)];
    }
}

void MatrizDoK::set(int fila, int columna, double valor) {
    if (fila >= filas() || columna >= columnas()) {
        cerr << "Se intentó acceder a una posición inexistente de una matriz DoK";
        exit(-1);    
    }
    _dict[pair<int,int>(fila, columna)] = valor;
}
    
void MatrizDoK::elementos_ordenado(vector<values_with_index> &elementos) {
    for (auto it = _dict.begin(); it != _dict.end(); it++) {
        elementos.push_back(*it);
    }
    sort(elementos.begin(), elementos.end()); // funciona, ver http://www.cplusplus.com/reference/utility/pair/operators
}

void MatrizDoK::mostrar(ostream &os) {
    os.precision(5);
    os.setf(ios::fixed,ios::floatfield);

    for (int fila = 0; fila < filas(); fila++) {
        os << "[";
        for (int columna = 0; columna < columnas(); columna++) {
            // seteo la precisión en función del signo para obtener ancho fijo
            os.precision((*this)(fila, columna) < 0 ? 4 : 5);
            os << (*this)(fila, columna) << (columna == columnas()-1 ? "" : ", ");
        }
        os << "]" << endl;
    }
    os << endl;
}

void MatrizDoK::mostrar_esparsa(ostream &os) {
    os.precision(5);
    os.setf(ios::fixed,ios::floatfield);

    vector<values_with_index> elementos;
    elementos_ordenado(elementos);
    for (auto it = elementos.begin(); it != elementos.end(); it++) {
        int fila = it->first.first;
        int columna = it->first.second;
        double valor = it->second;
        os << "(" << fila << ", " << columna << ") = " << valor << endl;
    }
    os << endl;
}

/**
* MatrizCSR
*/
MatrizCSR::MatrizCSR(MatrizDoK &dok) :
        _numfilas(dok.filas()),
        _numcolumnas(dok.columnas()),
        _values(),
        _columns_index(),
        _rows_start() {
    vector <values_with_index> elementos;
    dok.elementos_ordenado(elementos);
    int ultima_fila = 0;
    _rows_start.push_back(ultima_fila);
    for (uint i = 0; i < elementos.size(); i++) {
        int fila = elementos[i].first.first;
        int columna = elementos[i].first.second;
        double valor = elementos[i].second;
        _values.push_back(valor);
        _columns_index.push_back(columna);
        for (int f = ultima_fila; f < fila; f++){
            _rows_start.push_back(i);
        }
        ultima_fila = fila;
    }
    _rows_start.push_back(elementos.size());
}

void MatrizCSR::get_fila(int fila, vector<double> &elementos, vector<int> &columnas) {
    if (fila > filas()){
        cerr << "Se intentó acceder a una posición inexistente de una matriz CSR";
        exit(-1);
    }
    for (int i = _rows_start[fila]; i < _rows_start[fila+1]; i++){
        elementos.push_back(_values[i]);
        columnas.push_back(_columns_index[i]);
    }
}

double MatrizCSR::operator()(int fila, int columna) {
    if (fila >= filas() || columna >= columnas()) {
        cerr << "Se intentó acceder a una posición inexistente de una matriz CSR";
        exit(-1);
    }
    vector<double> elementos;
    vector<int> columnas;
    get_fila(fila, elementos, columnas);
    for (uint i = 0; i < columnas.size(); i++){
        if (columna == columnas[i]){
            return elementos[i];
        }
    }
    return 0;
}

void MatrizCSR::mostrar(ostream &os) {
    os.precision(5);
    os.setf(ios::fixed,ios::floatfield);

    for (int fila = 0; fila < filas(); fila++) {
        os << "[";
        for (int columna = 0; columna < columnas(); columna++) {
            // seteo la precisión en función del signo para obtener ancho fijo
            os.precision((*this)(fila, columna) < 0 ? 4 : 5);
            os << (*this)(fila, columna) << (columna == columnas()-1 ? "" : ", ");
        }
        os << "]" << endl;
    }
    os << endl;
}

void MatrizCSR::mostrar_esparsa(ostream &os) {
    os.precision(5);
    os.setf(ios::fixed,ios::floatfield);

    for (int fila = 0; fila < filas(); fila++) {
        vector<double> elementos;
        vector<int> columnas;
        get_fila(fila, elementos, columnas);
        for (uint i = 0; i < elementos.size(); i++){
            os << "(" << fila << ", " << columnas[i] << ") = " << elementos[i] << endl;
        }
    }
    os << endl;
}