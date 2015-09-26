#include <vector>
#include <iostream>
#include <cstring>
#include <dok_matrix.tpp>
#include <csr_matrix.tpp>


int main(int argc, char** argv) {
    DoK<double> dok1(4);
    dok1[0][0] = 0.00;
    dok1[0][1] = 0.00;
    dok1[0][2] = 1.00;
    dok1[0][3] = 0.50;

    dok1[1][0] = 1/(double)3;
    dok1[1][1] = 0.00;
    dok1[1][2] = 0.00;
    dok1[1][3] = 0.00;

    dok1[2][0] = 1/(double)3;
    dok1[2][1] = 0.50;
    dok1[2][2] = 0.00;
    dok1[2][3] = 0.50;

    dok1[3][0] = 1/(double)3;
    dok1[3][1] = 0.50;
    dok1[3][2] = 0.00;
    dok1[3][3] = 0.00;
    
    std::cout << dok1 << std::endl;

    CSR<double> csr1;
    CSR<double> csr2(dok1);

    std::vector<double> initial_vec(csr2.columnas(), 1/(double)csr2.columnas());
    std::vector<double> res;
    csr2.power_method(initial_vec, 0.85, res);

    return 0;
}
