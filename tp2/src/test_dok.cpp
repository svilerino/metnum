#include <vector>
#include <iostream>
#include <cstring>
#include <dok_matrix.tpp>


int main(int argc, char** argv) {
    DoK<double> dok1(10);
    dok1[9][9] = 9.99;
    dok1[0][0] = 6.66;
    dok1[2][5] = 2.55;
    dok1[5][6] = 5.66;
    dok1[5][8] = 5.88;
    dok1[3][2] = 3.22;
    dok1[5][5] = 5.55;

    std::cout << "Dok1 Filas: " << dok1.rows() << std::endl;
    std::cout << "Dok1 Cols: " << dok1.cols() << std::endl;

    //std::cout << "dok1[0][9]: " << dok1[0][9] << std::endl;
    std::cout << "dok1[2][5]: " << dok1[2][5] << std::endl;
    std::cout << "dok1[5][5]: " << dok1[5][5] << std::endl;
    //std::cout << "Tamaños: " << dok1.matrix.size() << std::endl;
    *dok1.begin()->val = 0.11;

    std::cout << dok1 << std::endl << std::endl;
    std::cout << "Y ahora la matriz esparsa" << std::endl;
    dok1.print_sparse(std::cout);

    return 0;
}
