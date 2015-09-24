#ifndef CSR_TPP
#define CSR_TPP

#include<vector>
#include<algorithm>
#include<ostream>
#include<sparse_vector.tpp>
#include<dok_matrix.tpp>
#include<matrix_value.tpp>

typedef unsigned int uint;

template<class T>
class CSR
{
    public:
        /********************** CONSTRUCTORES *********************/
        CSR() : _numfilas(0), _numcolumnas(0), _values(), _columns_index(), _rows_start() {};
        CSR(const DoK<T>&);

        /************************ ITERADORES *********************/
        class iterator : public matrix_value<T>
        {
            private:
                typename std::vector<T>& _values;
                std::vector<uint>& _cols_idx;
                std::vector<uint>& _rows_start;
                matrix_value<T> val;
                uint idx;

            public:
                typedef iterator self_type;

                self_type operator++()
                {
                    if(idx < _values.size())
                    {
                        ++idx;
                        if (idx == _values.size())
                        {
                            val.col = 0;
                            val.row = 0;
                            val.val = 0;
                        } else {
                            val.val = _values[idx];
                            val.col = _cols_idx[idx];
                            if(_rows_start[val.row+1] == idx) ++val.row;
                        }
                    }
                    return *this;
                };

                matrix_value<T> operator*(){return val;};

                matrix_value<T>* operator->(){return &val;};

                bool operator==(const self_type& it) const
                {
                    return idx == it.idx &&
                            val == it.val &&
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
                    val.val = _values[idx];
                    val.col = _cols_idx[idx];
                    std::vector<uint>::const_iterator it = 
                        std::lower_bound(_rows_start.cbegin(),_rows_start.cend(),idx);
                    val.row = it - _rows_start.cbegin();
                    if(*it != idx) --val.row;
                };

        };

        class const_iterator : public const_matrix_value<T>
        {
            private:
                const typename std::vector<T>& _values;
                const std::vector<uint>& _cols_idx;
                const std::vector<uint>& _rows_start;
                const_matrix_value<T> val;
                uint idx;

            public:
                typedef const_iterator self_type;

                self_type operator++()
                {
                    if(idx < _values.size())
                    {
                        ++idx;
                        if (idx == _values.size())
                        {
                            val.col = 0;
                            val.row = 0;
                            val.val = 0;
                        } else {
                            val.val = _values[idx];
                            val.col = _cols_idx[idx];
                            if(_rows_start[val.row+1] == idx) ++val.row;
                        }
                    }
                    return *this;
                };

                const_matrix_value<T> operator*(){return val;};

                const_matrix_value<T>* operator->(){return &val;};

                bool operator==(const self_type& it) const
                {
                    return idx == it.idx &&
                            val == it.val &&
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
                    val.val = _values[idx];
                    val.col = _cols_idx[idx];
                    std::vector<uint>::const_iterator it = 
                        std::lower_bound(_rows_start.cbegin(),_rows_start.cend(),idx);
                    val.row = it - _rows_start.cbegin();
                    if(*it != idx) --val.row;
                };

        };

        iterator begin() {return const_iterator(_values,_columns_index,_rows_start,0);};
        iterator end() {return const_iterator(_values,_columns_index,_rows_start,_values.size());};
        const_iterator cbegin() const {return const_iterator(_values,_columns_index,_rows_start,0);};
        const_iterator cend() const {return const_iterator(_values,_columns_index,_rows_start,_values.size());};

        /************************ GETTERS ************************/
        //void get_fila(uint fila, vector<T> &elementos, vector<uint> &columnas) ;
        uint filas() const {return _numfilas;}
        uint columnas() const {return _numcolumnas;}

        /**
        * Operador ()
        * Permite acceder y asignar al elemento i, j de la matriz usando la notación A(i, j)
        */
        T operator()(uint fila, uint columna) ;

	/************************ METODOS ***********************/
	std::vector<T>& operator*(const std::vector<T>&) const;
	void operator*(std::vector<T>&) const;

	std::vector<T>& power_method(const std::vector<T>&) const;
	void power_method(std::vector<T>&) const;

        /************************ OUTPUT ************************/
        /**
        * Salida
        * Imprime la matriz completa en un stream de salida.
        */
        //void mostrar(ostream &os) ;

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
    uint not_null_counter = 0,last_row_value = 0;
    for(auto it=dok.cbegin();it!=dok.cend();++it)
    {
        _values.push_back(it->val);
        _columns_index.push_back(it->col);
        if(last_row_value < it->row) 
        {
            for(++last_row_value;last_row_value<it->row;++last_row_value) //inserto 0 en caso de filas nulas
                _rows_start.push_back(0);
            _rows_start.push_back(_values.size()-1);
        }
        ++not_null_counter;
    }
    _rows_start.push_back(not_null_counter+1);
}

/*void MatrizCSR::get_fila(int fila, vector<double> &elementos, vector<int> &columnas) {
    if (fila > filas()){
        std::cerr << "Se intentó acceder a una posición inexistente de una matriz CSR";
        exit(-1);
    }
    for (int i = _rows_start[fila]; i < _rows_start[fila+1]; i++){
        elementos.push_back(_values[i]);
        columnas.push_back(_columns_index[i]);
    }
}*/

template<class T>
T CSR<T>::operator()(uint fila, uint columna) {
    if (fila >= _numfilas || columna >= _numcolumnas) {
        std::cerr << "Se intentó acceder a una posición inexistente de una matriz CSR" << std::endl;
        exit(-1);
    }

    T result = 0;
    uint first_row_value_index = _rows_start[fila];
    uint next_row_value_index = _rows_start[fila+1]; //no se indefine por el ultimo valor agregado a _rows_start

    std::vector<uint>::const_iterator it_low = 
        std::lower_bound(_columns_index.cbegin()+first_row_value_index,
                         _columns_index.cbegin()+next_row_value_index,
                         columna);
    if(*it_low == columna) result = _values[it_low - _columns_index.cbegin()];
    
    return result;
}

/*
void CSR<T>::mostrar(ostream &os) {
    os.precision(5);
    os.setf(ios::fixed,ios::floatfield);

    for (int fila = 0; fila < filas(); fila++) {
        os << "[";
        for (int columna = 0; columna < columnas(); columna++) {
            // seteo la precisión en función del signo para obtener ancho fijo
            os.precision((*this)(fila, columna) < 0 ? 4 : 5);
            os << (*this)(fila, columna) << (columna == columnas()-1 ? "" : ", ");
        }
        os << "]" << endl;
    }
    os << endl;
}
*/

/*
void MatrizCSR::mostrar_esparsa(ostream &os) {
    os.precision(5);
    os.setf(ios::fixed,ios::floatfield);

    for (int fila = 0; fila < filas(); fila++) {
        vector<double> elementos;
        vector<int> columnas;
        get_fila(fila, elementos, columnas);
        for (uint i = 0; i < elementos.size(); i++){
            os << "(" << fila << ", " << columnas[i] << ") = " << elementos[i] << endl;
        }
    }
    os << endl;
}
*/

template<class T>
std::vector<T>& CSR<T>::operator*(const std::vector<T>& x) const {};

template<class T>
void CSR<T>::operator*(std::vector<T>& x) const {};

template<class T>
std::vector<T>& CSR<T>::power_method(const std::vector<T>& x) const {};

template<class T>
void CSR<T>::power_method(std::vector<T>& x) const {};

template<class T>
std::ostream& operator<< (std::ostream& os,CSR<T>& csr){
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

    return os;
};

#endif
