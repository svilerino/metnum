#ifndef DOK_TPP
#define DOK_TPP

#ifndef SPARSE_VECTOR_TPP
#include<sparse_vector.tpp>
#endif

#include<map>
#include<algorithm>
#include<iostream>
#include<cassert>
#include<matrix_value.tpp>

typedef unsigned int uint;

template<class T>
class DoK
{
    private:
        typedef sparse_vector<sparse_vector<T>> sparse_matrix;
        sparse_matrix matrix;

    public:
        class iterator : public matrix_value<T>
        {
            private:
                sparse_matrix& matrix;
                typename sparse_vector<T>::iterator it_cols;
                typename sparse_matrix::iterator it_rows;
                matrix_value<T> value;

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

                    value.row = it_rows->first; value.col = it_cols->first; value.val = it_cols->second;
                    return *this;
                };
                matrix_value<T> operator*(){return value;};
                matrix_value<T>* operator->(){return &value;};
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

        class const_iterator : public const_matrix_value<T>
        {
            private:
                const sparse_matrix& matrix;
                typename sparse_vector<T>::const_iterator it_cols;
                typename sparse_matrix::const_iterator it_rows;
                const_matrix_value<T> value;

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

                    value.row = it_rows->first; value.col = it_cols->first; value.val = it_cols->second;
                    return *this;
                };
                const_matrix_value<T> operator*(){return value;};
                const_matrix_value<T>* operator->(){return &value;};
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
