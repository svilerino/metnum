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