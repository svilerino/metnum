#ifndef MATRIX_VALUE_H
#define MATRIX_VALUE_H

template<typename T>
struct matrix_value
{
    uint row,col;
    T* val;

    bool operator==(const matrix_value& mv) const
    {
        return row == mv.row && col == mv.col && *val == *mv.val;
    };

    matrix_value(){};
    matrix_value(uint row, uint col, T& val) : row(row),col(col),val(val){};
};

template<typename T>
struct const_matrix_value
{
    uint row,col;
    const T* val;

    bool operator==(const const_matrix_value& mv) const
    {
        return row == mv.row && col == mv.col && *val == *mv.val;
    };

    const_matrix_value(){};
    const_matrix_value(uint row, uint col,const T& val) : row(row),col(col),val(val){};
};


#endif
