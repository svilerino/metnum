#ifndef CSR_TPP
#define CSR_TPP

#include<vector>
#include <cassert>
#include<algorithm>
#include<ostream>
#include<limits>
#include<sparse_vector.tpp>
#include<dok_matrix.tpp>
#include<matrix_value.tpp>
#include<vector_operations.tpp>

typedef unsigned int uint;
#define EMPTY_ROW_CODE std::numeric_limits<uint>::max()

template<class T>
class CSR
{
    public:
        /********************** CONSTRUCTORES *********************/
        CSR() : _numfilas(0), _numcolumnas(0), _values(), _columns_index(), _rows_start() {};
        CSR(const DoK<T>&);

        /************************ ITERADORES *********************/
        class iterator
        {
            public:
                typedef iterator self_type;
                typedef matrix_value<T> self_value;

                self_type operator++()
                {
                    if(idx < _values.size())
                    {
                        ++idx;
                        if (idx == _values.size())
                        {
                            val.col = 0;
                            val.row = 0;
                            val.val = NULL;
                        } else {
                            val.val = &_values[idx];
                            val.col = _cols_idx[idx];

                            if(idx == *it_next_row_value_index) //llegue a la siguiente fila
                            {
                                val.row = it_next_row_value_index - _rows_start.cbegin();
                                it_next_row_value_index = next_row_value_index(val.row);
                            }
                        }
                    }
                    return *this;
                };

                self_value operator*(){return val;};

                self_value* operator->(){return &val;};

                bool operator==(const self_type& it) const
                {
                    return idx == it.idx &&
                            val.row == it.val.row &&
                            val.col == it.val.col &&
                            val.val == it.val.val &&
                            _rows_start == it._rows_start &&
                            _cols_idx == it._cols_idx &&
                            _values == it._values;
                };

                bool operator!=(const self_type& it) const {return !(*this == it);};

                iterator(typename std::vector<T>& vals,
                         std::vector<uint>& cols_idx,
                         std::vector<uint>& rows_start,
                         uint offset) : _values(vals),
                                        _cols_idx(cols_idx),
                                        _rows_start(rows_start),
                                        idx(offset)
                {
                    if(idx < _values.size()){
                        val.val = &_values[idx];
                        val.col = _cols_idx[idx];

                        //calculo la primer fila
                        auto it_row = std::find_if(_rows_start.begin(),
                                                    _rows_start.end(),
                                                    [this](uint index){return index != EMPTY_ROW_CODE
                                                                        && index >= idx;});
                        if(*it_row != idx)
                        {
                            for(--it_row;
                                *it_row == EMPTY_ROW_CODE;
                                --it_row){};
                        };
                        val.row = it_row - _rows_start.begin();
                        it_next_row_value_index = next_row_value_index(val.row);

                    } else {
                            val.col = 0;
                            val.row = 0;
                            val.val = NULL;
                            idx = _values.size();
                    }
                };

            private:
                std::vector<uint>::iterator next_row_value_index(uint cur_row);

                typename std::vector<T>& _values;
                std::vector<uint>& _cols_idx;
                std::vector<uint>& _rows_start;
                std::vector<uint>::iterator it_next_row_value_index;
                matrix_value<T> val;
                uint idx;
        };

        class const_iterator
        {
            public:
                typedef const_iterator self_type;
                typedef const_matrix_value<T> self_value;

                self_type operator++()
                {
                    if(idx < _values.size())
                    {
                        ++idx;
                        if (idx == _values.size())
                        {
                    #ifdef _DOK_SPARSE_VECTOR
                            val.col = 0;
                            val.row = 0;
                            val.val = NULL;
                        } else {
                            val.val = &_values[idx];
                            val.col = _cols_idx[idx];
                            if(idx == *it_next_row_value_index) //llegue a la siguiente fila
                            {
                                val.row = it_next_row_value_index - _rows_start.cbegin();
                                it_next_row_value_index = next_row_value_index(val.row);
                            }
                        }
                    #else
                            val.first.second = 0;
                            val.first.first = 0;
                            val.second = 0;
                        } else {
                            val.second = _values[idx];
                            val.first.second = _cols_idx[idx];

                            if(idx == *it_next_row_value_index) //llegue a la siguiente fila
                            {
                                val.first.first = it_next_row_value_index - _rows_start.cbegin();
                                it_next_row_value_index = next_row_value_index(val.first.first);
                            }
                        }
                    #endif
                    }
                    return *this;
                };

                const self_value operator*() const {return val;};

                const self_value* operator->() const {return &val;};

                bool operator==(const self_type& it) const
                {
                    return idx == it.idx &&
                            val.row == it.val.row &&
                            val.col == it.val.col &&
                            val.val == it.val.val &&
                            _rows_start == it._rows_start &&
                            _cols_idx == it._cols_idx &&
                            _values == it._values;
                };

                bool operator!=(const self_type& it) const {return !(*this == it);};

                const_iterator(const typename std::vector<T>& vals,
                                const std::vector<uint>& cols_idx,
                                const std::vector<uint>& rows_start,
                                uint offset) : _values(vals),
                                               _cols_idx(cols_idx),
                                               _rows_start(rows_start),
                                               idx(offset)
                {
                    if(idx < _values.size()){
                        val.val = &_values[idx];
                        val.col = _cols_idx[idx];

                        //calculo la primer fila
                        auto it_row = std::find_if(_rows_start.cbegin(),
                                                    _rows_start.cend(),
                                                    [this](uint index){return index != EMPTY_ROW_CODE
                                                                        && index >= idx;});
                        if(*it_row != idx)
                        {
                            for(--it_row;
                                *it_row == EMPTY_ROW_CODE;
                                --it_row){};
                        };
                        val.row = it_row - _rows_start.cbegin();
                        it_next_row_value_index = next_row_value_index(val.row);

                    } else {
                            val.col = 0;
                            val.row = 0;
                            val.val = NULL;
                            idx = _values.size();
                    }
                };

            private:
                std::vector<uint>::const_iterator next_row_value_index(uint cur_row);

                const typename std::vector<T>& _values;
                const std::vector<uint>& _cols_idx;
                const std::vector<uint>& _rows_start;
                std::vector<uint>::const_iterator it_next_row_value_index;
                const_matrix_value<T> val;
                uint idx;
        };

        iterator begin() {return iterator(_values,_columns_index,_rows_start,0);};
        iterator end() {return iterator(_values,_columns_index,_rows_start,_values.size());};
        const_iterator cbegin() const {return const_iterator(_values,_columns_index,_rows_start,0);};
        const_iterator cend() const {return const_iterator(_values,_columns_index,_rows_start,_values.size());};

        /************************ GETTERS ************************/
        uint filas() const {return _numfilas;}
        uint columnas() const {return _numcolumnas;}
        void get_row(uint fila, std::vector<T>& elementos, std::vector<uint>& columnas);

        /**
        * Operador ()
        * Permite acceder y asignar al elemento i, j de la matriz usando la notación A(i, j)
        */
        T operator()(uint fila, uint columna); //debería devolver una referencia, pero dado que a veces hay que devolver 0...

        /************************ METODOS ***********************/
        std::vector<T>& operator*(const std::vector<T>&) const;
        void operator*(std::vector<T>&) const;

        std::vector<T>& power_method(const std::vector<T>&, double) const;
        void power_method(std::vector<T>&, double) const;

        /************************ OUTPUT ************************/
        /**
        * Salida
        * Imprime la matriz completa en un stream de salida.
        */
        void show(std::ostream& os);

    /************************ ATRIBUTOS ************************/
    //private:
        uint _numfilas;
        uint _numcolumnas;

        typename std::vector<T> _values;
        std::vector<uint> _columns_index;
        std::vector<uint> _rows_start;
};

template<class T>
CSR<T>::CSR(const DoK<T>& dok) :
        _numfilas(dok.rows()),
        _numcolumnas(dok.cols()),
        _values(),
        _columns_index(),
        _rows_start()
{
    #ifdef _DOK_SPARSE_VECTOR
    uint last_row_processed = 0;

    //Inicializacion de primera fila
    auto it=dok.cbegin();
    if(it->row == 0)
    {
        _values.push_back(*it->val);
        _columns_index.push_back(it->col);
        _rows_start.push_back(_values.size()-1);
        ++it;

    } else { //entonces la primera fila esta vacia
        _rows_start.push_back(EMPTY_ROW_CODE);
    };

    //Resto de las filas
    for(;it!=dok.cend();++it)
    {
        _values.push_back(*it->val);
        _columns_index.push_back(it->col);
        if(last_row_processed == it->row-1) //puede haber overflow, por eso "=="
        {
            _rows_start.push_back(_values.size()-1);
            ++last_row_processed;

        } else if(last_row_processed != it->row){ //entonces hay filas vacias 
            for(++last_row_processed;last_row_processed<it->row;++last_row_processed) //inserto oo en caso de filas nulas
                _rows_start.push_back(EMPTY_ROW_CODE);
            _rows_start.push_back(_values.size()-1);
        };
    }
    _rows_start.push_back(_values.size()); //No le sumo +1 ya que en realidad indexamos desde el 0 en lugar del 1

    #else //DoK hecho con unordered_map
    uint last_row_processed = 0;

    //Inicializacion de primera fila
    auto it=dok.cbegin();
    if(it->first.first == 0)
    {
        _values.push_back(it->second);
        _columns_index.push_back(it->first.second);
        _rows_start.push_back(_values.size()-1);
        ++it;

    } else { //entonces la primera fila esta vacia
        _rows_start.push_back(EMPTY_ROW_CODE);
    };

    //Resto de las filas
    for(;it!=dok.cend();++it)
    {
        _values.push_back(it->second);
        _columns_index.push_back(it->first.second);
        if(last_row_processed == it->first.first-1) //puede haber overflow, por eso "=="
        {
            _rows_start.push_back(_values.size()-1);
            ++last_row_processed;

        } else if(last_row_processed != it->first.first){ //entonces hay filas vacias 
            for(++last_row_processed;last_row_processed<it->first.first;++last_row_processed) //inserto oo en caso de filas nulas
                _rows_start.push_back(EMPTY_ROW_CODE);
            _rows_start.push_back(_values.size()-1);
        };
    }

    #endif
};

template<class T>
std::vector<uint>::iterator CSR<T>::iterator::next_row_value_index(uint cur_row)
{
    assert(idx < _rows_start.size());
    std::vector<uint>::iterator begin = _rows_start.begin()+cur_row+1;
    for(;*begin == EMPTY_ROW_CODE && begin!=_rows_start.end();++begin){};

    return begin;
};

template<class T>
std::vector<uint>::const_iterator CSR<T>::const_iterator::next_row_value_index(uint cur_row)
{
    assert(idx < _rows_start.size());
    std::vector<uint>::const_iterator begin = _rows_start.cbegin()+cur_row+1;
    for(;*begin == EMPTY_ROW_CODE && begin!=_rows_start.cend();++begin){};

    return begin;
};

template<class T>
void CSR<T>::get_row(uint fila, std::vector<T>& elementos, std::vector<uint>& columnas) {
    if (fila > _numfilas){
        std::cerr << "Se intentó acceder a una posición inexistente de una matriz CSR";
        exit(-1);
    }

    uint first_row_value_index = _rows_start[fila];
    if(first_row_value_index != EMPTY_ROW_CODE)
    {
        //la siguiente variable no se indefine por el ultimo valor agregado a _rows_start, siguiendo la convencion CSR
        uint next_row_value_index = *std::find_if(_rows_start.cbegin()+fila+1,
                                                    _rows_start.cend(),
                                                    [](uint index){return index != EMPTY_ROW_CODE;});

        for (uint i = first_row_value_index; i < next_row_value_index; ++i){
            elementos.push_back(_values[i]);
            columnas.push_back(_columns_index[i]);
        }
    }
}

template<class T>
T CSR<T>::operator()(uint fila, uint columna) {
    if (fila >= _numfilas || columna >= _numcolumnas) {
        std::cerr << "Se intentó acceder a una posición inexistente de una matriz CSR" << std::endl;
        exit(-1);
    }

    T result = T();
    uint first_row_value_index = _rows_start[fila];
    if(first_row_value_index != EMPTY_ROW_CODE)
    {
        //la siguiente variable no se indefine por el ultimo valor agregado a _rows_start, siguiendo la convencion CSR
        uint next_row_value_index = *std::find_if(_rows_start.cbegin()+fila+1,
                                                    _rows_start.cend(),
                                                    [](uint index){return index != EMPTY_ROW_CODE;});

        auto it_next_row_value_index = _columns_index.cbegin()+next_row_value_index;
        auto it_low = std::lower_bound(_columns_index.cbegin()+first_row_value_index,
                                        it_next_row_value_index,
                                        columna);

        if(it_low != it_next_row_value_index //necesario pues trabajamos con rangos parciales,
                                            // con lo cual podria ser que si la columna buscada
                                            // corresponda a un cero, it_low apunte a la primer
                                            // columna de la fila siguiente en lugar de cend().
            && *it_low == columna
        ) result = _values[it_low - _columns_index.cbegin()];
    }

    return result;
}

template<class T>
void CSR<T>::show(std::ostream& os)
{
    //os.precision(5);
    //os.setf(ios::fixed,ios::floatfield);
}

template<class T>
std::vector<T>& CSR<T>::operator*(const std::vector<T>& x) const {};

template<class T>
void CSR<T>::operator*(std::vector<T>& x) const {};

template<class T>
std::vector<T>& CSR<T>::power_method(const std::vector<T>& x, double prob) const {};

template<class T>
void CSR<T>::power_method(std::vector<T>& x, double prob) const {};

template<class T>
std::ostream& operator<< (std::ostream& os,const CSR<T>& csr)
{
    //os.precision(5);
    //os.setf(ios::fixed,ios::floatfield);

    /*
    os << "Values: ";
    for(auto it=csr._values.cbegin();it!=csr._values.cend();++it)
        os << *it << " ";
    os << std::endl;

    os << "Columns: ";
    for(auto it=csr._columns_index.cbegin();it!=csr._columns_index.cend();++it)
        os << *it << " ";
    os << std::endl;

    os << "Rows: ";
    for(auto it=csr._rows_start.cbegin();it!=csr._rows_start.cend();++it)
        os << *it << " ";
    os << std::endl;
    */

    for(auto it=csr.cbegin();it!=csr.cend();++it)
        #ifdef _DOK_SPARSE_VECTOR
        os << "[" << it->row << "][" << it->col << "]: " << *it->val << std::endl;
        #else
        os << "[" << it->first.first << "][" << it->first.second << "]: " << it->second << std::endl;
        #endif
    return os;
};

#endif
