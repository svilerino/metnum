#ifndef TESTS_H
#define TESTS_H

#include "matriz.h"

void run_test(){
    MatrizDoK A(4, 4);
    A.set(1, 0, 5.0);
    A.set(1, 1, 8.0);
    A.set(2, 2, 3.0);
    A.set(3, 1, 6.0);
    A.mostrar(cout);
    A.mostrar_esparsa(cout);

    MatrizCSR B(A);
    B.mostrar(cout);
    B.mostrar_esparsa(cout);
}
#endif
