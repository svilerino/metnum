#ifndef MATRIX_VALUE_H
#define MATRIX_VALUE_H

template<typename T>
struct matrix_value
{
    uint row,col;
    T& val;

    matrix_value() : row(0),col(0),val(*new T()){delete &val;}; //grasada, todavia no se C++
    matrix_value(uint row, uint col, T& val) : row(row),col(col),val(val){};
};

template<typename T>
struct const_matrix_value
{
    uint row,col;
    T& val; //deberia ser constante, pero no le encontre solucion a la asignacion en el operador de increment de los iteradores

    const_matrix_value() : row(0),col(0),val(*new T()){delete &val;}; //grasada, todavia no se C++
    const_matrix_value(uint row, uint col,const T& val) : row(row),col(col),val(val){};
};


#endif
