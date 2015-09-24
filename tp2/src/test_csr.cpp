#include<vector>
#include<iostream>
#include<cstring>
#include<dok_matrix.tpp>
#include<csr_matrix.tpp>


int main(int argc, char** argv) {
    DoK<double> dok1(10);
    dok1[2][5] = 2.22;
    dok1[3][2] = 2.22;
    dok1[5][5] = 1.66;
    dok1[9][9] = 2.22;
    for(auto it=dok1.begin();it!=dok1.end();++it)
        std::cout << "dok1[" << it->row << "][" << it->col << "] = " << *it->val << std::endl;

    CSR<double> csr1;
    CSR<double> csr2(dok1);

    std::cout << "Filas CSR1: " << csr1.filas() << std::endl;
    std::cout << "Columnas CSR1: " << csr1.columnas() << std::endl;
    //std::cout << "CSR1[0][0]: " << csr1(0,0) << std::endl;
    std::cout << "Filas CSR2: " << csr2.filas() << std::endl;
    std::cout << "Columnas CSR2: " << csr2.columnas() << std::endl;
    //std::cout << "CSR2[0][0]: " << csr2(0,0) << std::endl;
    //std::cout << "CSR2[2][5]: " << csr2(2,5) << std::endl;
    //std::cout << "CSR2[1][4]: " << csr2(1,4) << std::endl;
    std::cout << csr2 << std::endl;

	return 0;
}
