#include <vector>
#include <iostream>
#include <cstring>
#include <dok_matrix.tpp>
#include <csr_matrix.tpp>


int main(int argc, char** argv) {

    // Test case: Figure 1 del paper de leise y bryan con su solucion provista en el paper

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

    double epsilon_tolerancia = 0.000000001;
    assert(abs(res[0] - 0.368) < epsilon_tolerancia);
    assert(abs(res[1] - 0.142) < epsilon_tolerancia);
    assert(abs(res[2] - 0.288) < epsilon_tolerancia);
    assert(abs(res[3] - 0.202) < epsilon_tolerancia);


    return 0;
}
