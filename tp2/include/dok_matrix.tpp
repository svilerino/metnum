#ifndef DOK_TPP
#define DOK_TPP

#include <algorithm>
#include <cassert>
#include <iostream>
#include <matrix_value.tpp>

typedef unsigned int uint;

#ifdef _DOK_SPARSE_VECTOR

/***************************************** IMPL MAP SPARSE VECTOR *******************************************/
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
        std::cout << "[" << it->row << "]" << "[" << it->col << "] = " << *it->val << std::endl;
    return os;
};

/************************************** FIN IMP MAP SPARSE VECTOR **********************************************/

#else

/****************************************** IMPL UNORDERED_MAP PAIR *******************************************/

#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

/**
* DoK
* Representa una matriz esparsa de doubles usando Dictionary of Keys
*/
template<class T>
class DoK {
    public:
        /************************ CONSTRUCTORES ************************/
        /**
        * constructor por defecto
        * Crea una matriz de 0 filas y 0 columnas
        */
        DoK() : _numfilas(0), _numcolumnas(0), _dict() {};

        /**
        * constructor de matriz 0
        */
        DoK(uint x) : _numfilas(x), _numcolumnas(x), _dict() {};

        /**
        * Constructor vacío
        * Construye una matriz llena de ceros
        */
        DoK(uint filas, uint columnas) : _numfilas(filas), _numcolumnas(columnas), _dict() {};

        /**
        * Constructor desde archivo
        * Carga la matriz de un stream de entrada.
        */
        //DoK(std::istream& is);

        /*************************** ITERADORES ****************************/
        class iterator 
        {
            public:
                typedef iterator self_type;
                typedef matrix_value<T> self_value;

                self_type operator++()
                {
                    if(it_cur_pos != _coord_not_null.end())
                    {
                        ++it_cur_pos;
                        if(it_cur_pos != _coord_not_null.end())
                        {
                            it_value = _dict_ref.find(*it_cur_pos);
                        };
                        
                    };
                    return *this;
                };

                self_value operator*(){return *it_value;};
                self_value* operator->(){return &(*it_value);}; //hago esto porque no tengo acceso a la estr interna del iterador

                bool operator==(const self_type& it) const
                {
                    return _dict_ref == it._dict_ref &&
                            _coord_not_null == it._coord_not_null &&
                            it_cur_pos - _coord_not_null.begin() == it.it_cur_pos - it._coord_not_null.begin();

                };
                bool operator!=(const self_type& it) const {return !(*this == it);};

                iterator(dict_of_keys<T>& dict,uint offset) : _dict_ref(dict),_coord_not_null(0)
                {
                    assert(_dict_ref.size() >= offset);
                    for(auto it=_dict_ref.cbegin();it!=_dict_ref.cend();++it)
                        _coord_not_null.push_back(it->first);
                    std::sort(_coord_not_null.begin(),_coord_not_null.end());
                    it_cur_pos = _coord_not_null.begin()+offset;
                    if(it_cur_pos != _coord_not_null.end())
                    {
                        it_value = _dict_ref.find(*it_cur_pos);
                    }; 
                };

            private:
                dict_of_keys<T>& _dict_ref;
                typename dict_of_keys<T>::iterator it_value;
                std::vector<matrix_coord> _coord_not_null;
                std::vector<matrix_coord>::iterator it_cur_pos;

        };

        class const_iterator
        {
            public:
                typedef const_iterator self_type;
                typedef const_matrix_value<T> self_value;

                self_type operator++()
                {
                    if(it_cur_pos != _coord_not_null.cend())
                    {
                        ++it_cur_pos;
                        if(it_cur_pos != _coord_not_null.cend())
                        {
                            it_value = _dict_ref.find(*it_cur_pos);
                        };
                        
                    };
                    return *this;
                };

                const self_value operator*(){return *it_value;};
                const self_value* operator->(){return &(*it_value);}; //hago esto porque no tengo acceso a la estr interna del iterador

                bool operator==(const self_type& it) const
                {
                    return _dict_ref == it._dict_ref &&
                            _coord_not_null == it._coord_not_null &&
                            it_cur_pos - _coord_not_null.cbegin() == it.it_cur_pos - it._coord_not_null.cbegin();

                };
                bool operator!=(const self_type& it) const {return !(*this == it);};

                const_iterator(const dict_of_keys<T>& dict,uint offset) : _dict_ref(dict),_coord_not_null(0)
                {
                    assert(_dict_ref.size() >= offset);
                    for(auto it=_dict_ref.cbegin();it!=_dict_ref.cend();++it)
                        _coord_not_null.push_back(it->first);
                    std::sort(_coord_not_null.begin(),_coord_not_null.end());
                    it_cur_pos = _coord_not_null.cbegin()+offset;
                    if(it_cur_pos != _coord_not_null.cend())
                    {
                        it_value = _dict_ref.find(*it_cur_pos);
                    }; 
                };

            private:
                const dict_of_keys<T>& _dict_ref;
                typename dict_of_keys<T>::const_iterator it_value;
                std::vector<matrix_coord> _coord_not_null;
                std::vector<matrix_coord>::const_iterator it_cur_pos;

        };



        iterator begin() {return iterator(_dict,0);};
        iterator end() {return iterator(_dict,_dict.size());};
        const_iterator cbegin() const {return const_iterator(_dict,0);};
        const_iterator cend() const {return const_iterator(_dict,_dict.size());};
        /************************ GETTERS Y SETTERS ************************/
        /**
        * Operador ()
        * Permite acceder al elemento i, j de la matriz usando la notación A(i, j)
        */
        T operator()(uint fila, uint columna); //deberia devolver T&, pero como debe ser cero a veces...
        /**
        * Set
        * Permite asignar un valor al elemento i, j de la matriz
        */
        void set(uint fila, uint columna, T valor);
        /**
        * Cantidad de filas y columnas
        */
        uint rows() {return _numfilas;}
        uint cols() {return _numcolumnas;}
        
        /************************ ELEMENTOS ORDENADOS ************************/
        /**
        * Elementos en orden
        * Recibe un vector vacío y lo llena con triplas < <uint, uint>, T> que indican 
        * los índices y valores de los elementos no nulos de la matriz, ordenados de
        * arriba hacia abajo y de izquierda a derecha.
        */
        void elementos_ordenado(std::vector<matrix_value<T>>& elements) const; 

        /************************ OUTPUT ************************/
        /**
        * Salida
        * Imprime la matriz completa en un stream de salida.
        */
        void mostrar(std::ostream& os) ;

    private:
        uint _numfilas;
        uint _numcolumnas;
        dict_of_keys<T> _dict;
};

/*
template<class T>
DoK<T>::DoK(std::istream& is)
{

};
*/

template<class T>
T DoK<T>::operator()(uint fila, uint columna) {
    if (fila >= rows() || columna >= cols())
    {
        std::cerr << "Se intentó acceder a una posición inexistente de una matriz DoK";
        exit(-1);    
    };

    if (_dict.count(std::pair<uint,uint>(fila, columna)) == 0)
    {
        return 0;
    } else {
        return _dict[std::pair<uint,uint>(fila, columna)];
    };
}

template<class T>
void DoK<T>::set(uint fila, uint columna, T valor)
{
    if (fila >= rows() || columna >= cols())
    {
        std::cerr << "Se intentó acceder a una posición inexistente de una matriz DoK";
        exit(-1);    
    };

    _dict[std::pair<uint,uint>(fila, columna)] = valor;
};
    
template<class T>
void DoK<T>::elementos_ordenado(std::vector<matrix_value<T>>& elementos) const
{
    for (auto it = _dict.cbegin(); it != _dict.cend(); ++it)
    {
        elementos.push_back(*it);
    }

    std::sort(elementos.begin(), elementos.end()); // funciona, ver http://www.cplusplus.com/reference/utility/pair/operators
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
            os.precision((*this)(fila, columna) < 0 ? 4 : 5);
            os << (*this)(fila, columna) << (columna == cols()-1 ? "" : ", ");
        }
        os << "]" << std::endl;
    }
    os << std::endl;
}

template<class T>
std::ostream& operator<< (std::ostream& os, DoK<T>& dok) {
    os.precision(5);
    os.setf(std::ios::fixed,std::ios::floatfield);

    for(auto it = dok.cbegin();it!=dok.cend();++it)
        os << "[" << it->first.first << "][" << it->first.second << "] = " << it->second << std::endl;

    return os;
}

/***************************************** FIN IMPL UNORDERED_MAP PAIR *****************************************/
#endif

#endif
