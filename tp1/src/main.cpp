#include <iostream>
#include "matriz.h"

using namespace std;

int main(){
//	vector<double> vec1(4, 5);
//	vector<double> vec2(4, 1);
//	vector<double> vec3(4, 10);
//	vector<double> vec4(5, 10);
//
//	//deberia fallar la suma por dimensiones
//	//sumar(vec1, vec4, vec4);
//
//	cout << normap(vec1, 1) << endl;
//	cout << normap(vec2, 2) << endl;
//	cout << normap(vec3, 3) << endl;
//
//	double prodInt = producto_interno(vec1, vec2);
//	cout << prodInt << endl;
//
//	vector<double> test(4, 5);
//
//	Matriz deCerosRectangular(3, 4);
//	deCerosRectangular.mostrar(cout);
//
//	Matriz noNulaRectangular(3, 4);
//	noNulaRectangular[2][3] = 7;
//	noNulaRectangular[1][3] = 1;
//	noNulaRectangular.mostrar(cout);
//
//	Matriz copiaDeAnterior(noNulaRectangular);
//	copiaDeAnterior[2][3] = 6;
//	noNulaRectangular.mostrar(cout);
//    copiaDeAnterior.mostrar(cout);
//
//	Matriz trasp = noNulaRectangular.traspuesta();
//	trasp.mostrar(cout);
//	
//	Matriz identidadCuadrada(4);
//	identidadCuadrada.mostrar(cout);
//
//	Matriz diagonal = Matriz::diagonal(test);
//	diagonal.mostrar(cout);
//
//	Matriz vectorcolumna = Matriz(test);
//	vectorcolumna.mostrar(cout);

	Matriz a;
	a.cargar(cin);
	vector<double> b = {1, 2 ,2}; 
	vector<double> res(b.size(), 0);
	res = a.eliminacion_gaussiana(b);
	cout << "Resultado:" << endl;
	imprimir_vector(res, cout);
    return 0;
}

