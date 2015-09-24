#ifndef MATRIX_VALUE_H
#define MATRIX_VALUE_H

template<typename T>
struct matrix_value
{
    uint row,col;
    T* val;

    matrix_value(){};
    matrix_value(uint row, uint col, T& val) : row(row),col(col),val(val){};
};

template<typename T>
struct const_matrix_value
{
    uint row,col;
    const T* val;

    const_matrix_value(){};
    const_matrix_value(uint row, uint col,const T& val) : row(row),col(col),val(val){};
};


#endif
