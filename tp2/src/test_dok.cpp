#include<vector>
#include<iostream>
#include<cstring>
#include<matriz.hpp>


int main(int argc, char** argv) {
	DoK<double> dok1(10);
	dok1[9][9] = 2.22;
	dok1[2][5] = 2.22;
	dok1[3][2] = 2.22;
	dok1[5][5] = 1.66;

	std::cout << "Dok1 Filas: " << dok1.rows() << std::endl;
	std::cout << "Dok1 Cols: " << dok1.cols() << std::endl;
	//std::cout << "dok1[0][9]: " << dok1[0][9] << std::endl;
	std::cout << "dok1[2][5]: " << dok1[2][5] << std::endl;
	//std::cout << "Tamaños: " << dok1.matrix.size() << std::endl;
	std::cout << dok1 << std::endl;

	return 0;
}
