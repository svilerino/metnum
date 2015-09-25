#include<vector>
#include<iostream>
#include<cstring>
#include<dok_matrix.tpp>
#include<csr_matrix.tpp>


int main(int argc, char** argv) {
    DoK<double> dok1(10);
    //dok1[0][0] = 6.66;
    dok1[2][5] = 2.22;
    dok1[3][2] = 3.33;
    dok1[5][5] = 5.55;
    dok1[5][6] = 5.66;
    dok1[5][8] = 5.88;
    dok1[9][9] = 9.99;
    /*
    for(auto it=dok1.begin();it!=dok1.end();++it)
        std::cout << "dok1[" << it->row << "][" << it->col << "] = " << *it->val << std::endl;
    */
    std::cout << dok1 << std::endl;

    CSR<double> csr1;
    CSR<double> csr2(dok1);

    /*
    std::cout << "Filas CSR1: " << csr1.filas() << std::endl;
    std::cout << "Columnas CSR1: " << csr1.columnas() << std::endl;
    std::cout << "CSR1[0][0]: " << csr1(0,0) << std::endl;
    std::cout << "Filas CSR2: " << csr2.filas() << std::endl;
    std::cout << "Columnas CSR2: " << csr2.columnas() << std::endl;
    /
    for(uint i = 0;i<csr2.filas();++i)
        for(uint j=0;j<csr2.columnas();++j)
            std::cout << "CSR2[" << i << "][" << j << "]: " << csr2(i,j) << std::endl;

    /
    std::cout << "CSR2[0][0]: " << csr2(0,0) << std::endl;
    std::cout << "CSR2[0][5]: " << csr2(0,5) << std::endl;
    std::cout << "CSR2[2][5]: " << csr2(2,5) << std::endl;
    std::cout << "CSR2[1][4]: " << csr2(1,4) << std::endl;
    std::cout << "CSR2[3][2]: " << csr2(3,2) << std::endl;
    std::cout << "CSR2[5][5]: " << csr2(5,5) << std::endl;
    std::cout << "CSR2[9][9]: " << csr2(9,9) << std::endl;

    auto it = csr2.begin();
    std::cout << it->row << " " << it->col << " " << *it->val << std::endl;
    ++it;
    std::cout << it->row << " " << it->col << " " << *it->val << std::endl;
    */
    std::cout << csr2 << std::endl;

    std::vector<double> a(10,6.66);
    std::vector<double> b(10,2.22);
    a+=b;
    std::cout << a << std::endl;
    a*=2.5;
    std::cout << a << std::endl;
    b = a*2.5;
    std::cout << b << std::endl;
    std::vector<double> c = a+b;
    std::cout << c << std::endl;
    c = a-b;
    std::cout << c << std::endl << std::endl;

    std::vector<uint> d(0);
    a.erase(a.begin(),a.end());
    csr2.get_row((uint)0,a,d);
    std::cout << "Elementos: " << a << std::endl;
    std::cout << "Columnas : " << d << std::endl;

    a.erase(a.begin(),a.end());
    d.erase(d.begin(),d.end());
    csr2.get_row((uint)5,a,d);
    std::cout << "Elementos: " << a << std::endl;
    std::cout << "Columnas : " << d << std::endl;

    return 0;
}
