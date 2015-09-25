#include "../include/vector_operations.tpp"
#include <iostream>
#include <chrono>

//reset && g++ -std=c++11 -O2 -ftree-vectorize -mavx -ftree-vectorizer-verbose=1 -o test test_timing_vector_op.cpp && ./test 

using namespace std;

#define NUM_ELEMENTS 3000000

inline std::vector<double> sumar(std::vector<double> a, std::vector<double>& b)
{
    assert(a.size() == b.size());

    std::vector<double> result(a.size(),0);
    std::transform(a.cbegin(),a.cend(),b.cbegin(),result.begin(),std::plus<double>());

    return result;
};

void sumas (){

	cout << "Sumas:" << endl;

	vector<double> vecA(NUM_ELEMENTS, 1);
	vector<double> vecB(NUM_ELEMENTS, 1);
	vector<double> vecC(NUM_ELEMENTS, 0);

    chrono::time_point<chrono::high_resolution_clock> start, end;
    int elapsed = 0;


    start = chrono::high_resolution_clock::now();
		vecC = sumar(vecA, vecB);
    end = chrono::high_resolution_clock::now();
    elapsed = chrono::duration_cast<chrono::microseconds>(end-start).count();
    
    cout << "\tFuncion sumar: " << elapsed << endl;

    start = chrono::high_resolution_clock::now();
		size_t size = vecA.size();
		for (int i = 0; i < size; ++i)
		{
			vecC[i] = vecA[i] + vecB[i];
		}
    end = chrono::high_resolution_clock::now();
    elapsed = chrono::duration_cast<chrono::microseconds>(end-start).count();
    
    cout << "\tFor directo sumar: " << elapsed << endl;

    start = chrono::high_resolution_clock::now();
		vecC = vecA + vecB;
    end = chrono::high_resolution_clock::now();
    elapsed = chrono::duration_cast<chrono::microseconds>(end-start).count();
    
    cout << "\tOperador +: " << elapsed << endl;
}

void norma1(){
	cout << "Norma 1:" << endl;

	vector<double> vecA(150*NUM_ELEMENTS, 1);

    chrono::time_point<chrono::high_resolution_clock> start, end;
    int elapsed = 0;
    double norm = 0;

    start = chrono::high_resolution_clock::now();
		norm = norma1(vecA, false);
    end = chrono::high_resolution_clock::now();
    elapsed = chrono::duration_cast<chrono::microseconds>(end-start).count();
    
    cout << "\tFuncion norma1: " << elapsed << endl;

    start = chrono::high_resolution_clock::now();
		norm = norma1(vecA, true);
    end = chrono::high_resolution_clock::now();
    elapsed = chrono::duration_cast<chrono::microseconds>(end-start).count();

    
    cout << "\tFuncion norma1_allpos: " << elapsed << endl;

    //std::cout << norm << std::endl;
}

int main(){
	sumas();
	norma1();
	return 0;
}
