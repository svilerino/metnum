#ifndef MATRIX_VALUE_H
#define MATRIX_VALUE_H

#ifdef _DOK_SPARSE_VECTOR
/***************************************** IMPL MAP SPARSE VECTOR *******************************************/
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

/************************************** FIN IMP MAP SPARSE VECTOR **********************************************/
#else

/****************************************** IMPL UNORDERED_MAP PAIR *******************************************/
#include <unordered_map>
#include <utility>

/**
* Hash para pares de enteros positivos
*/
struct hash_pair{
public:
    size_t operator()(std::pair<uint,uint> p) const {
        // http://www.boost.org/doc/libs/1_53_0/doc/html/hash/reference.html#boost.hash_combine
        size_t seed = 0;
        seed ^= std::hash<uint>()(p.first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<uint>()(p.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    };
};


typedef std::pair<uint,uint> matrix_coord;

template<typename T>
using dict_of_keys = std::unordered_map<matrix_coord, T, hash_pair>;

template<typename T>
using matrix_value = std::pair<std::pair<uint, uint>, T>;

template<typename T>
using const_matrix_value = const std::pair<const std::pair<uint, uint>, T>;

/***************************************** FIN IMPL UNORDERED_MAP PAIR *****************************************/
#endif

#endif
