#include<vector>
#include<iostream>
#include<cstring>
#include<dok_matrix.tpp>


int main(int argc, char** argv) {
	DoK<double> dok1(10);
    #ifdef _DOK_SPARSE_VECTOR
        dok1[9][9] = 2.22;
        dok1[0][0] = 6.66;
        dok1[2][5] = 2.22;
        dok1[5][6] = 5.66;
        dok1[5][8] = 5.88;
        dok1[3][2] = 2.22;
        dok1[5][5] = 1.66;
    #else
        dok1.set(9,9,9.99);
        dok1.set(2,5,2.22);
        dok1.set(3,2,3.33);
        dok1.set(5,5,5.55);
        dok1.set(5,6,5.66);
        dok1.set(5,8,5.88);
        dok1.set(0,0,6.66);
    #endif

	std::cout << "Dok1 Filas: " << dok1.rows() << std::endl;
	std::cout << "Dok1 Cols: " << dok1.cols() << std::endl;

    #ifdef _DOK_SPARSE_VECTOR
        std::cout << "dok1[0][9]: " << dok1[0][9] << std::endl;
        std::cout << "dok1[2][5]: " << dok1[2][5] << std::endl;
        //std::cout << "Tamaños: " << dok1.matrix.size() << std::endl;
    #else
        std::cout << "dok1[0][9]: " << dok1(0,9) << std::endl;
        std::cout << "dok1[2][5]: " << dok1(2,5) << std::endl;
    #endif

	std::cout << dok1 << std::endl;
    dok1.mostrar(std::cout);

	return 0;
}
