class DoK
{
        /******************* OPERATORS y METODOS ******************/

        sparse_vector<T>& operator[] (uint pos);
        const sparse_vector<T>& operator[] (uint pos) const;

        uint cols();
        uint rows();

        /************************ OUTPUT ************************/
        /**
        * Salida
        * Imprime la matriz completa en un stream de salida.
        */
        void print_sparse(std::ostream& os) ;

        /********************* CONSTRUCTORES ********************/
        DoK() : matrix() {};
        DoK(uint r) : matrix(r),_rows(r) {};
        DoK(uint r,uint c) : matrix(r),_rows(r),_cols(c) {};
};

class CSR
{

        /************************ GETTERS ************************/
        uint rows() const;
        uint cols() const;
        void get_row(uint fila, std::vector<T>& elementos, std::vector<uint>& columnas) const;

        /**
        * Operador ()
        * Permite acceder y asignar al elemento i, j de la matriz usando la notacion A(i, j)
        */
        T operator()(uint fila, uint columna) const;

        /************************ METODOS ***********************/

        void prod_Ax(const std::vector<T>& x,
                    std::vector<T>& y/*result*/,
                    double parametro_c) const;

        void power_method(const std::vector<T>&, double _initial_vector,
                            power_method_stop_criteria_t,
                            std::vector<T>& _output) const;

        /************************ OUTPUT ************************/
        /**
        * Salida
        * Imprime la matriz completa en un stream de salida.
        */
        void print_sparse(std::ostream& os) const;
		
	/************************ CONSTRUCTORES ************************/  
	CSR(DoK<T>& dok);
};

// Impl. algoritmo 1 de golub Ax
// Nota: utilizamos operaciones sobre vectores sobrecargadas, 
// ie. Producto vector por escalar, etc
void CSR<T>::prod_Ax(const std::vector<T>& x,
                    std::vector<T>& y/*resultado*/,
                    double parametro_c) const
{
    // Itero sobre las filas de la matriz
    for (uint idx_fila = 0; idx_fila < _numfilas; idx_fila++)
    {
        std::vector<T> fila_actual_elementos;
        std::vector<uint> fila_actual_columnas_llenas;
        get_row(idx_fila, fila_actual_elementos, fila_actual_columnas_llenas);        

        // Hago el producto interno <fila_i, y> * x
        y[idx_fila] = 0;
        if(!fila_actual_elementos.empty())
        {
            for(uint idx_elem = 0;
            	idx_elem < fila_actual_elementos.size();
            	++idx_elem)
            {
                y[idx_fila] += 
                	(fila_actual_elementos[idx_elem] * 
                		x[fila_actual_columnas_llenas[idx_elem]]);
            };

            y[idx_fila] *= parametro_c;
        };
    };

    // Tenemos y = c* P^t * x

    double w = norma1(x, true) - norma1(y, true);//Podemos asumir que x e y
                                                 //tienen todas componentes >=0
                                                 //y nos ahorramos el abs en la
                                                 //norma !
    w/=(double)_numcolumnas;

    y+=w;
};

// Nota: este metodo esta recortado para ser mas legible, 
// para ver su implementacion completa, ir al codigo fuente
void CSR<T>::power_method(const std::vector<T>& _initial_vector,
                            double parametro_c,
                            power_method_stop_criteria_t criterio_parada,
                            std::vector<T>& _output_vector) const
{
    std::vector<T> eigenvec_candidate(_initial_vector);
    std::vector<T> new_eigenvec_candidate(eigenvec_candidate.size());
    double diff = 0.0;
    double epsilon_diff_corte = criterio_parada.valor.delta_diff;
    do {

        prod_Ax(eigenvec_candidate, new_eigenvec_candidate, parametro_c); //Ax

        diff = norma1(new_eigenvec_candidate-eigenvec_candidate,false);

        double norma_autovec = norma1(new_eigenvec_candidate, true);
        eigenvec_candidate = new_eigenvec_candidate/norma_autovec; // Reemplazo para proxima iteracion normalizado
        //eigenvec_candidate = new_eigenvec_candidate; // Reemplazo para proxima iteracion

    } while(diff >= epsilon_diff_corte);

    //Escribo la salida en el parametro de salida
    _output_vector = new_eigenvec_candidate;    
};