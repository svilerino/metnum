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