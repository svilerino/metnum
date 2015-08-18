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

Matriz::Matriz (int n){
    double t(52);
    t=1;
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

Matriz::Matriz (int n, int m, int prec){
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

Matriz::Matriz (vector<double> v){
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

size_t min(size_t a, size_t b){
    if (a < b)
        return a;
    else
        return b;
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
    Matriz b (ancho, alto);
    for (int i=0; i<alto; i++){
        for(int j=0; j<ancho; j++){
            b[j][i] = matriz[i][j];
        }
    }
    return b;
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

vector<Matriz> Matriz::autovalores(double tol, int max_it){
    vector<double> dant=this->diagonal();
    vector<double> d(alto);
    Matriz Qac(alto);
    Matriz A1 (alto, ancho);
    A1.matriz = this->matriz;
    vector<Matriz> QR (2);
    for (int i = 0; /*((norma1(Qac.restar(dant, d))) > tol) && */(i < max_it); i++){
        QR = A1.qr_rapido();
        A1 = QR[1].multiplicar(QR[0]);
        Qac = Qac.multiplicar(QR[0]);
        dant = d;
        d = A1.diagonal();
        cout<<i<<endl;
    }
    Matriz dRes(alto, ancho);
    Matriz vRes(alto, ancho);
    dRes = dRes.diagonal(d);
    vector<Matriz> res;
    res.push_back(dRes);
    res.push_back(Qac);
    res[0].mostrar(cout);
    res[1].mostrar(cout);
    return res;
}

vector< vector<double> > Matriz::get_columnas(){
    vector< vector<double> > columnas;
    vector<double> tmp;
    for(int j=0; j<ancho; j++){
        for (int i = 0; i < alto; i++){
        tmp.push_back(matriz[i][j]);
        }
        columnas.push_back(tmp);
        tmp.clear();
    }
    return columnas;
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

vector<Matriz> Matriz::qr(){
    vector<double> c;
    double alpha;
    double converter;
    Matriz Aux(alto, ancho);
    Matriz Q(alto);
    vector<Matriz> res;

    Matriz R(alto, ancho);
    R.matriz = this -> matriz;
    for(int i=0; i <alto-1 ; i++) {
        c = R.get_columnas()[i];
        converter = 0;
        for (int j=0; j<=i-1; j++)
            c[j]=converter;
        alpha =norma2(c);
        converter = -1;
        if (c[i] >=0)
            alpha = converter*alpha;
        c[i] = c[i]+alpha;
        converter = norma2(c);
        for (uint j=0; j<c.size(); j++)
            c[j]=c[j]/converter;
        Matriz Id(alto);
        Matriz v(c);
        converter = 2;
        Aux = v.multiplicar(v.traspuesta()).multiplicar(converter);
        Id = Id.restar(Aux);
        Matriz Idt = Id.traspuesta();
        Q = Q.multiplicar(Idt);
        R = Id.multiplicar(R);
    }
    res.push_back(Q);
    res.push_back(R);
    return res;
}

int signo(double t){
    if (t < 0) return -1;
    else return 1;
}

void normalizar(vector<double> &v){
    double norm = norma2(v);
    for (uint i = 0; i < v.size(); i++)
        v[i] = v[i] / norm;
}

vector<Matriz> Matriz::qr_rapido(){
    Matriz Q(alto);     //inicializo Q como la identidad
    Matriz R(*this);    //inicializo R como copia de mí mismo
    bool q_inicializada = false;

    for (int k = 0; k < alto - 1; k++){
        //defino el vector donde quiero poner ceros
        vector<double> v(alto - k);
        //copio la columna de mi matriz
        for (uint i = 0; i < v.size(); i++)
            v[i] = R[i+k][k];
        //modifico la primera componente
        v[0] = v[0] + signo(v[0])*norma2(v);
        //normalizo v
        normalizar(v);
        //calculo 2*v*v'*R[k..n, k..n]
        Matriz Aux(alto - k);
        for (int i = k; i < R.alto; i++)
            for (int j = k; j < R.ancho; j++){
                double acum = 0;
                for (uint z = 0; z < v.size(); z++){
                    acum = acum + v[i-k]*v[z]*R[z+k][j];
                }
                Aux[i-k][j-k] = 2*acum;
            }
        //actualizo la matriz R
        R.restar_submatriz(Aux);
        //actualizo la matriz Q
        Aux.mostrar(cout);
        Aux.ID_restar();
        Aux = Aux.traspuesta();
        Aux.mostrar(cout);
        if (!q_inicializada){
            Q = Aux;
            q_inicializada = true;
        }
        else
            Q.multiplicar_submatriz(Aux);
        Q.mostrar(cout);
    }

    vector<Matriz> res;
    res.push_back(Q);
    res.push_back(R);

    R.mostrar(cout);
    Q.mostrar(cout);

    return res;
}

void Matriz::restar_submatriz(Matriz otra){
    int dif_alto = alto - otra.alto;
    int dif_ancho = ancho - otra.ancho;
    for (int i = dif_alto; i < alto; i++)
        for (int j = dif_ancho; j < ancho; j++)
            matriz[i][j] = matriz[i][j] - otra[i - dif_alto][j - dif_ancho];
}

void Matriz::multiplicar_submatriz(Matriz otra){
    int dif = alto - otra.alto;
    Matriz Aux(otra.alto);
    for (int i = dif; i < alto; i++)
        for (int j = dif; j < ancho; j++){
            double acum = 0;
            for (int k = 0; k < dif; k++)
                acum = acum + matriz[i][k + dif] * otra[k][j - dif];
            Aux[i - dif][j - dif] = acum;
        }
    //piso la matriz
    for (int i = dif; i < alto; i++)
        for (int j = dif; j < alto; j++)
            matriz[i][j] = Aux[i - dif][j - dif];
}

void Matriz::ID_restar(){
    //computa Id - this
    for (int i = 0; i < alto; i++)
        for (int j = 0; i < ancho; i++)
            if (i == j)
                matriz[i][j] = 1 - matriz[i][j];
            else
                matriz[i][j] = - matriz[i][j];
}

