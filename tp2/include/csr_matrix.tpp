#ifndef CSR_TPP
#define CSR_TPP

#include <vector>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <limits>
#include <sparse_vector.tpp>
#include <dok_matrix.tpp>
#include <matrix_value.tpp>
#include <vector_operations.tpp>

typedef enum criterio_terminacion_powermethod {CRT_K_FIXED_ITERS_LIMIT, CRT_K_ITERS_DELTA_DIFF, CRT_K_ITERS_NO_DIFF} criterio_terminacion_powermethod_t;

typedef struct power_method_stop_criteria
{
    criterio_terminacion_powermethod_t criterio;
    union Valor {
        uint cant_iters;
        double delta_diff;
    } valor;

    uint intervalo_iters_reporte;
} power_method_stop_criteria_t;

typedef unsigned int uint;
#define EMPTY_ROW_CODE std::numeric_limits<uint>::max()

template<class T>
class CSR
{
    public:
        /********************** CONSTRUCTORES *********************/
        CSR() : _numfilas(0), _numcolumnas(0), _values(), _columns_index(), _rows_start() {};
        CSR(const DoK<T>&);
        CSR(const DoK<T*>&);
        ~CSR(){for(auto it=_values.begin();it!=_values.end();++it) delete *it;};

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
                            val.val = _values[idx];
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

                iterator(typename std::vector<T*>& vals,
                         std::vector<uint>& cols_idx,
                         std::vector<uint>& rows_start,
                         uint offset) : _values(vals),
                                        _cols_idx(cols_idx),
                                        _rows_start(rows_start),
                                        idx(offset)
                {
                    if(idx < _values.size()){
                        val.val = _values[idx];
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

                typename std::vector<T*>& _values;
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
                            val.col = 0;
                            val.row = 0;
                            val.val = NULL;
                        } else {
                            val.val = _values[idx];
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

                const_iterator(const typename std::vector<T*>& vals,
                                const std::vector<uint>& cols_idx,
                                const std::vector<uint>& rows_start,
                                uint offset) : _values(vals),
                                               _cols_idx(cols_idx),
                                               _rows_start(rows_start),
                                               idx(offset)
                {
                    if(idx < _values.size()){
                        val.val = _values[idx];
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

                const typename std::vector<T*>& _values;
                const std::vector<uint>& _cols_idx;
                const std::vector<uint>& _rows_start;
                std::vector<uint>::const_iterator it_next_row_value_index;
                const_matrix_value<T> val;
                uint idx;
        };

        iterator begin() {return iterator(_values,_columns_index,_rows_start,(uint)0);};
        iterator end() {return iterator(_values,_columns_index,_rows_start,_values.size());};
        const_iterator cbegin() const {return const_iterator(_values,_columns_index,_rows_start,(uint)0);};
        const_iterator cend() const {return const_iterator(_values,_columns_index,_rows_start,_values.size());};

        /************************ GETTERS ************************/
        uint rows() const {return _numfilas;}
        uint cols() const {return _numcolumnas;}
        void get_row(uint fila, std::vector<T>& elementos, std::vector<uint>& columnas) const;
        void get_row(uint fila, std::vector<T*>& elementos, std::vector<uint>& columnas) const;

        /**
        * Operador ()
        * Permite acceder y asignar al elemento i, j de la matriz usando la notación A(i, j)
        */
        T operator()(uint fila, uint columna); //debería devolver una referencia, pero dado que a veces hay que devolver 0...

        /************************ METODOS ***********************/
        //std::vector<T> operator*(const std::vector<T>&) const;

        void prod_Ax(const std::vector<T>& x, std::vector<T>& y/*result*/, double parametro_c) const;

        void power_method(const std::vector<T>&, double, power_method_stop_criteria_t, std::vector<T>& _output, std::ofstream & reporte) const;

        /************************ OUTPUT ************************/
        /**
        * Salida
        * Imprime la matriz completa en un stream de salida.
        */
        void print_sparse(std::ostream& os);

    /************************ ATRIBUTOS ************************/
    private:
        uint _numfilas;
        uint _numcolumnas;

        typename std::vector<T*> _values;
        std::vector<uint> _columns_index;
        std::vector<uint> _rows_start;
};

/*********************************** IMPLEMENTACIONES ******************************/
template<class T>
CSR<T>::CSR(const DoK<T>& dok) :
        _numfilas(dok.rows()),
        _numcolumnas(dok.cols()),
        _values(),
        _columns_index(),
        _rows_start()
{
    uint last_row_processed = 0;

    //Inicializacion de primera fila
    auto it=dok.cbegin();
    if(it->row == 0)
    {
        _values.push_back(new T(*it->val));
        _columns_index.push_back(it->col);
        _rows_start.push_back(_values.size()-1);
        ++it;

    } else { //entonces la primera fila esta vacia
        _rows_start.push_back(EMPTY_ROW_CODE);
    };

    //Resto de las filas
    while(it!=dok.cend())
    {
        _values.push_back(new T(*it->val));
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
        ++it;
    }
    _rows_start.push_back(_values.size()); //No le sumo +1 ya que en realidad indexamos desde el 0 en lugar del 1
};

template<class T>
CSR<T>::CSR(const DoK<T*>& dok) :
        _numfilas(dok.rows()),
        _numcolumnas(dok.cols()),
        _values(),
        _columns_index(),
        _rows_start()
{
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
    while(it!=dok.cend())
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
        ++it;
    }
    _rows_start.push_back(_values.size()); //No le sumo +1 ya que en realidad indexamos desde el 0 en lugar del 1
};

template<class T>
std::vector<uint>::iterator CSR<T>::iterator::next_row_value_index(uint cur_row)
{
    assert(cur_row < _rows_start.size());
    std::vector<uint>::iterator begin = _rows_start.begin()+cur_row+1;
    while(*begin == EMPTY_ROW_CODE && begin!=_rows_start.cend()) ++begin;

    return begin;
};

template<class T>
std::vector<uint>::const_iterator CSR<T>::const_iterator::next_row_value_index(uint cur_row)
{
    assert(cur_row < _rows_start.size());
    std::vector<uint>::const_iterator begin = _rows_start.cbegin()+cur_row+1;
    while(*begin == EMPTY_ROW_CODE && begin!=_rows_start.cend()) ++begin;

    return begin;
};

template<class T>
void CSR<T>::get_row(uint fila, std::vector<T>& elementos, std::vector<uint>& columnas) const{
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
            elementos.push_back(*_values[i]);
            columnas.push_back(_columns_index[i]);
        }
    }
}

template<class T>
void CSR<T>::get_row(uint fila, std::vector<T*>& elementos, std::vector<uint>& columnas) const{
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
        ) result = *_values[it_low - _columns_index.cbegin()];
    }

    return result;
}

template<class T>
void CSR<T>::print_sparse(std::ostream& os)
{
    os.precision(5);
    os.setf(std::ios::fixed,std::ios::floatfield);

    for (uint fila = 0; fila < rows(); ++fila) {
        os << "[";
        for (uint columna = 0; columna < cols(); ++columna) {
            // seteo la precisión en función del signo para obtener ancho fijo
            os.precision((*this)(fila,columna) < 0 ? 4 : 5);
            os << (*this)(fila,columna) << (columna == cols()-1 ? "" : ", ");
        }
        os << "]" << std::endl;
    }
}

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
        os << "[" << it->row << "][" << it->col << "]: " << *it->val << std::endl;
    return os;
};

// Impl. algoritmo 1 de golub Ax
template<class T>
void CSR<T>::prod_Ax(const std::vector<T>&x, std::vector<T>& y/*resultado*/, double parametro_c) const{

    assert(_numcolumnas == x.size());// Validacion dimensiones
    assert(_numfilas == y.size());// Validacion dimensiones
    std::vector<T> cx = (x*parametro_c); // y = cx

    // y = Pt * cx
    // y_i = Producto interno <fila_i, cx> = <fila_i, y>

    // Itero sobre las filas de la matriz    
    std::vector<T> fila_actual_elementos;
    std::vector<uint> fila_actual_columnas_llenas;
    for (uint idx_fila = 0; idx_fila < _numfilas; idx_fila++)
    {
        fila_actual_elementos.clear();
        fila_actual_columnas_llenas.clear();
        get_row(idx_fila, fila_actual_elementos, fila_actual_columnas_llenas);

        // Hago el producto interno <fila_i, y> = <fila_i, cx>
        double y_i = 0;

        uint idx_elemento = 0;
        for (uint idx_col : fila_actual_columnas_llenas)
        {
            y_i += fila_actual_elementos[idx_elemento++] * cx[idx_col];
        }

        y[idx_fila] = y_i;
    }

    // Tenemos y = Pt *c*x

    double w = norma1(x, true) - norma1(y, true);//Podemos asumir que x e y tienen todas componentes >=0 y nos ahorramos el abs en la norma !

    std::vector<T> v(y.size(), (double)1/y.size());//personalization vector segun el paper de golub

    y += v*w;
};

template<class T>
void CSR<T>::power_method(const std::vector<T>& _initial_vector, double parametro_c, power_method_stop_criteria_t criterio_parada, std::vector<T>& _output_vector, std::ofstream& reporte_power_method) const {  
    int iters = 0;
    std::vector<T> eigenvec_candidate(_initial_vector);
    std::vector<T> new_eigenvec_candidate(eigenvec_candidate.size());
    double diff = 0.0;

    //std::cout << "Initial eigenvector: "<< std::endl;
    //std::cout << eigenvec_candidate << std::endl;

    // Mejor que branchear mucho adentro de las iteraciones del metodo, voy a reusar poco codigo pero hacerlo mas eficiente codeando cada version por separado

    if(criterio_parada.criterio == CRT_K_ITERS_DELTA_DIFF) {

        double epsilon_diff_corte = criterio_parada.valor.delta_diff;
        do{

            prod_Ax(eigenvec_candidate, new_eigenvec_candidate, parametro_c); //Ax

            std::vector<T> diff_vec = new_eigenvec_candidate - eigenvec_candidate;
            diff = norma1(diff_vec, false); // diff = || x_k - x_{k-1} ||

            eigenvec_candidate = new_eigenvec_candidate; // Reemplazo para proxima iteracion

            if(iters % criterio_parada.intervalo_iters_reporte == 0){
                reporte_power_method << iters << " " << diff << " " << eigenvec_candidate << std::endl;
                reporte_power_method.flush();
            }

            iters++;
        }while(diff > epsilon_diff_corte);

    } else if (criterio_parada.criterio ==  CRT_K_FIXED_ITERS_LIMIT) {

        uint cant_iters = criterio_parada.valor.cant_iters;
        for (uint iters = 0; iters <= cant_iters; iters++){

            prod_Ax(eigenvec_candidate, new_eigenvec_candidate, parametro_c); //Ax  

            std::vector<T> diff_vec = new_eigenvec_candidate - eigenvec_candidate;
            diff = norma1(diff_vec, false); // diff = || x_k - x_{k-1} ||

            eigenvec_candidate = new_eigenvec_candidate; // Reemplazo para proxima iteracion

            if(iters % criterio_parada.intervalo_iters_reporte == 0){
                reporte_power_method << iters << " " << diff << " " << eigenvec_candidate << std::endl;
                reporte_power_method.flush();
            }
        }

    } else if (criterio_parada.criterio == CRT_K_ITERS_NO_DIFF){
        assert(false && "CRT_K_ITERS_NO_DIFF sin implementar.");
    } else {
        std::cerr << "Criterio de parada erroneo en power method" << std::endl;
    }

    //Escribo la salida en el parametro de salida
    _output_vector = new_eigenvec_candidate;    
};

#endif
