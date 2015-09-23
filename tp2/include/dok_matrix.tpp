#ifndef DOK_TPP
#define DOK_TPP

#ifndef SPARSE_VECTOR_TPP
#include<sparse_vector.tpp>
#endif

#include <map>
#include <algorithm>
#include <iostream>
#include <cassert>

typedef unsigned int uint;

template<class T>
class DoK
{
    private:
        typedef sparse_vector<sparse_vector<T>> sparse_matrix;
        sparse_matrix matrix;

    public:
        struct matrix_value
        {
            uint row,col;
            T& val;

            matrix_value(uint row, uint col, T& val) : row(row),col(col),val(val){};
        };

        struct const_matrix_value
        {
            uint row,col;
            const T& val;

            const_matrix_value(uint row, uint col,const T& val) : row(row),col(col),val(val){};
        };


        class iterator
        {
            private:
                sparse_matrix& matrix;
                typename sparse_vector<T>::iterator it_cols;
                typename sparse_matrix::iterator it_rows;

            public:
                typedef iterator self_type;

                self_type operator++()
                {
                    if(it_rows != matrix.end())
                    {
                        ++it_cols;
                        if(it_cols == it_rows->second.end())
                        {
                            ++it_rows;
                            if(it_rows != matrix.end())
                            {
                                it_cols = it_rows->second.begin();
                            }
                        }
                    }

                    return *this;
                };
                matrix_value operator*(){
                    return matrix_value(it_rows->first,it_cols->first,it_cols->second);
                };
                //matrix_value* operator->(){return &(*this);};
                bool operator==(const self_type& it) const
                {
                    return it_cols == it.it_cols && it_rows == it.it_rows && matrix == it.matrix;
                };
                bool operator!=(const self_type& it) const {return !(*this == it);};

                iterator(sparse_matrix& sm, bool begin) : matrix(sm)
                {
                    if(begin)
                    {
                        it_rows = matrix.begin();
                        it_cols = it_rows->second.begin();
                    } else {
                        it_rows = matrix.end();
                        --it_rows;
                        it_cols = it_rows->second.end();
                        ++it_rows;
                    }
                };
        };

        class const_iterator
        {
            private:
                const sparse_matrix& matrix;
                typename sparse_vector<T>::const_iterator it_cols;
                typename sparse_matrix::const_iterator it_rows;

            public:
                typedef const_iterator self_type;

                self_type operator++()
                {
                    if(it_rows != matrix.cend())
                    {
                        ++it_cols;
                        if(it_cols == it_rows->second.cend())
                        {
                            ++it_rows;
                            if(it_rows != matrix.cend())
                            {
                                it_cols = it_rows->second.cbegin();
                            }
                        }
                    }

                    return *this;
                };
                const_matrix_value operator*(){
                    return const_matrix_value(it_rows->first,it_cols->first,it_cols->second);
                };
                //const matrix_value* operator->(){return &(*this);};
                bool operator==(const self_type& it) const
                {
                    return it_cols == it.it_cols && it_rows == it.it_rows && matrix == it.matrix;
                };
                bool operator!=(const self_type& it) const {return !(*this == it);};

                const_iterator(const sparse_matrix& sm, bool begin) : matrix(sm)
                {
                    if(begin)
                    {
                        it_rows = matrix.cbegin();
                        it_cols = it_rows->second.cbegin();
                    } else {
                        it_rows = matrix.cend();
                        --it_rows;
                        it_cols = it_rows->second.cend();
                        ++it_rows;
                    }
                };
        };


        iterator begin() {return iterator(matrix,true);};
        iterator end() {return iterator(matrix,false);};
        const_iterator cbegin() const {return const_iterator(matrix,true);};
        const_iterator cend() const {return const_iterator(matrix,false);};

        sparse_vector<T>& operator[] (uint pos){return matrix[pos];};
        const sparse_vector<T>& operator[] (uint pos) const{return matrix[pos];};
        //bool operator== (const DoK&) const;
        //bool operator!= (const DoK&) const;

        uint cols() const {
            uint max_col = 0;
            for(auto it_rows=matrix.cbegin();it_rows!=matrix.cend();++it_rows)
                max_col = std::max(max_col,it_rows->second.size());
            return max_col;
        };
        uint rows() const {return matrix.size();};

        DoK() : matrix() {};
        DoK(uint rows) : matrix(rows) {};
    };

template<class T>
std::ostream& operator<< (std::ostream& os,const DoK<T>& dok)
{
    for(auto it=dok.cbegin();it!=dok.cend();++it)
        std::cout << "[" << (*it).row << "]" << "[" << (*it).col << "] = " << (*it).val << std::endl;
    return os;
};

#endif
