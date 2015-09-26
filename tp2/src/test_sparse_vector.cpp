#include <vector>
#include <iostream>
#include <cstring>
#include <sparse_vector.tpp>


int main(int argc, char** argv) {
	sparse_vector<double> vector;
	std::cout << "El size inicial es: " << vector.size() << std::endl;
	vector[9] = 1.666;
	vector[15] = 2;
	std::cout << "Nuevo size es: " << vector.size() << std::endl;
	//for (int i=0;i<vector.size();++i)
	//	cout << "[" << i << "] = " << vector[i] << std::endl;
	std::cout << vector << std::endl;

	sparse_vector<double> vector2(15);
	std::cout << "Vector == vector2: " << (vector == vector2) << std::endl;
	std::cout << "Vector != vector2: " << (vector != vector2) << std::endl;
	vector2[9] = 1.666;
	vector2[15] = 2;
	std::cout << "Vector == vector2: " << (vector == vector2) << std::endl;
	std::cout << "Vector != vector2: " << (vector != vector2) << std::endl;

	return 0;
}
