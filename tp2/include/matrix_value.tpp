#ifndef MATRIX_VALUE_H
#define MATRIX_VALUE_H

#include <iostream>
#include <vector>

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
    matrix_value(uint row, uint col, T* val) : row(row),col(col),val(val){};
    matrix_value(const matrix_value& mv) : row(mv.row),col(mv.col),val(mv.val){};
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
    const_matrix_value(uint row, uint col,const T* val) : row(row),col(col),val(val){};
    const_matrix_value(const const_matrix_value& mv) : row(mv.row),col(mv.col),val(mv.val){};
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const matrix_value<T>& mv)
{
    os << "(" << mv.row << ", " << mv.col << ", " << mv.val << ")";
    return os;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<matrix_value<T>>& vmv)
{
    os << "(";
    for(auto it=vmv.cbegin();it!=vmv.end()-1;++it)
        os << *it << ", ";

    os << vmv.back() << ")";
    return os;
}

#endif
