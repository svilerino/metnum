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