#include<matriz.hpp>
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
