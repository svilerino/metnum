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