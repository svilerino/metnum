#ifndef DOK_TPP
#define DOK_TPP

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <matrix_value.tpp>
#include <unordered_map>
#include <utility>
#include <vector>

typedef unsigned int uint;

#include <map>
#include <sparse_vector.tpp>

template<class T>
class DoK
{
    public:
        typedef sparse_vector<sparse_vector<T>> sparse_matrix;

        /********************* ITERADORES *********************/
        class iterator
        {
            public:
                typedef iterator self_type;
                typedef matrix_value<T> self_value;

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

                    value.row = it_rows->first; value.col = it_cols->first; value.val = &it_cols->second;
                    return *this;
                };
                self_value operator*(){return value;};
                self_value* operator->(){return &value;};
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
                        value.row = it_rows->first; value.col = it_cols->first; value.val = &it_cols->second;
                    } else {
                        it_rows = matrix.end();
                        --it_rows;
                        it_cols = it_rows->second.end();
                        ++it_rows;
                    };
                };

            private:
                sparse_matrix& matrix;
                typename sparse_vector<T>::iterator it_cols;
                typename sparse_matrix::iterator it_rows;
                matrix_value<T> value;
        };

        class const_iterator
        {
            public:
                typedef const_iterator self_type;
                typedef const_matrix_value<T> self_value;

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

                    value.row = it_rows->first; value.col = it_cols->first; value.val = &it_cols->second;
                    return *this;
                };
                const self_value operator*() const {return value;};
                const self_value* operator->() const {return &value;};
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
                        value.row = it_rows->first; value.col = it_cols->first; value.val = &it_cols->second;
                    } else {
                        it_rows = matrix.cend();
                        --it_rows;
                        it_cols = it_rows->second.cend();
                        ++it_rows;
                    }
                };

            private:
                const sparse_matrix& matrix;
                typename sparse_vector<T>::const_iterator it_cols;
                typename sparse_matrix::const_iterator it_rows;
                const_matrix_value<T> value;
        };


        iterator begin() {return iterator(matrix,true);};
        iterator end() {return iterator(matrix,false);};
        const_iterator cbegin() const {return const_iterator(matrix,true);};
        const_iterator cend() const {return const_iterator(matrix,false);};

        /******************* OPERATORS y METODOS ******************/

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

        /************************ OUTPUT ************************/
        /**
        * Salida
        * Imprime la matriz completa en un stream de salida.
        */
        void mostrar(std::ostream& os) ;


        /********************* CONSTRUCTORES ********************/
        DoK() : matrix() {};
        DoK(uint rows) : matrix(rows) {};

    private:
        sparse_matrix matrix;
};

template<class T>
void DoK<T>::mostrar(std::ostream& os)
{
    os.precision(5);
    os.setf(std::ios::fixed,std::ios::floatfield);

    for (uint fila = 0; fila < rows(); ++fila) {
        os << "[";
        for (uint columna = 0; columna < cols(); ++columna) {
            // seteo la precisión en función del signo para obtener ancho fijo
            os.precision((*this)[fila][columna] < 0 ? 4 : 5);
            os << (*this)[fila][columna] << (columna == cols()-1 ? "" : ", ");
        }
        os << "]" << std::endl;
    }
    os << std::endl;
}

template<class T>
std::ostream& operator<< (std::ostream& os,const DoK<T>& dok)
{
    for(auto it=dok.cbegin();it!=dok.cend();++it)
        os << "[" << it->row << "]" << "[" << it->col << "] = " << *it->val << std::endl;
    return os;
};

#endif
