#include "matriz.h"
using namespace std;

Matriz::Matriz(int n, int m){
    matriz.clear();
    alto = n;
    ancho = m;
    vector<double> p;
    double t = 0;
    for (int i = 0; i < ancho; i++)
        p.push_back(t);
    for (int i = 0; i < alto; i++)
        matriz.push_back(p);
}

Matriz::Matriz(int n){
    double t=1;
    Matriz a(n,n);
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            if (i==j) a[i][j] = t;
        }
    }
    alto = n;
    ancho = n;
    matriz = a.matriz;
}

Matriz::Matriz(vector<double> v){
    Matriz res(v.size(),1);
    for (uint i=0; i<v.size(); i++){
        res[i][0] = v[i];
    }
    alto = res.alto;
    ancho = res.ancho;
    matriz = res.matriz;
}

void Matriz::cargar(std::istream &is){
    matriz.clear();
	vector<double> p;
	is >> alto;
	is >> ancho;
	for (int i = 0; i < alto; i++){
		for (int j = 0; j < ancho; j++){
		    double t;
			is >> t;
			p.push_back(t);
		}
		matriz.push_back(p);
		p.clear();
	}
}

void Matriz::mostrar(std::ostream &os){
    os.precision(5);
    os.setf(ios::fixed,ios::floatfield);
	for (int i=0 ; i<alto; i++){
		os << "[";
		for (int j=0; j<ancho; j++){
		    if (matriz[i][j] < 0) os.precision(4);
		    else os.precision(5);
		    if (j == ancho-1) os << matriz[i][j];
			else os << matriz[i][j] << ",";
		}
        os << "]" << endl;
	}
	os << endl;
}

Matriz Matriz::multiplicar(Matriz a){
    if (ancho!=(a.alto)) cerr << "Estás tratando de multiplicar matrices incompatibles" << endl;
    Matriz b (alto, (a.ancho));
    for (int i = 0; i < alto; i++){
        for(int j = 0; j < a.ancho; j++){
            b[i][j] = 0;
            for(int k = 0; k < ancho; k++){
                b[i][j] = b[i][j] + matriz[i][k] * a[k][j];
            }
        }
    }
    return b;
}

Matriz Matriz::multiplicar(double t){
    Matriz res(alto, ancho);
    for (int i = 0; i < alto; i++)
        for (int j = 0; j < ancho; j++)
            res[i][j] = matriz[i][j]*t;
    return res;
}

Matriz Matriz::sumar(Matriz otra){
    if (alto != otra.alto or ancho != otra.ancho) cerr << "Estás tratando de sumar matrices incompatibles" << endl;
    Matriz res(alto, ancho);
    for (int i = 0; i < alto; i++)
        for (int j = 0; j < ancho; j++)
            res[i][j] = matriz[i][j] + otra[i][j];
    return res;
}

Matriz Matriz::restar(Matriz otra){
    if (alto != otra.alto or ancho != otra.ancho) cerr << "Estás tratando de restar matrices incompatibles" << endl;
    Matriz res(alto, ancho);
    for (int i = 0; i < alto; i++)
        for (int j = 0; j < ancho; j++)
            res[i][j] = matriz[i][j] - otra[i][j];
    return res;
}

Matriz Matriz::traspuesta(){
    Matriz res(ancho, alto);
    for (int i=0; i<alto; i++){
        for(int j=0; j<ancho; j++){
            res[j][i] = matriz[i][j];
        }
    }
    return res;
}

vector<double> Matriz::diagonal(){
    vector<double> diag;
    for(int i=0; i<alto; i++){
        for (int j=0; j<ancho; j++){
            if (i==j) diag.push_back(matriz[i][j]);
        }
    }
    return diag;
}

vector<double> restar(vector<double> a, vector<double> b){
    vector<double> res (a.size());
    if (a.size()!=b.size()) cerr<<"Estas tratando de restar dos vectores incompatibles";
    for (uint i=0; i<a.size(); i++){
        res[i] = a[i] - b[i];
    }
    return res;
}

Matriz Matriz::diagonal(vector<double> v){
    Matriz res(v.size(), v.size());
    for(uint i=0; i<v.size(); i++){
        res[i][i] = v[i];
    }
    return res;
}

double norma1(vector<double> v){
    float acum = 0;
    for (uint i=0; i<v.size(); i++){
        acum = acum + v[i];
    }
    double t = acum;
    return t;
}

double norma2(vector<double> v){
    int tam = v.size();
    float acum = 0;
    for (int i=0; i<tam; i++){
        acum = acum + v[i]*v[i];
    }
    double t = sqrt(acum);
    return t;
}

void normalizar(vector<double> &v){
    double norm = norma2(v);
    for (uint i = 0; i < v.size(); i++)
        v[i] = v[i] / norm;
}