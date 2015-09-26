#include <vector>
#include <iostream>
#include <cstring>
#include <dok_matrix.tpp>
#include <csr_matrix.tpp>

bool power_method_iter_fijas(CSR<double>& csr, std::vector<double>& initial_vec, uint cant_iters, double probabilidad_c, double epsilon_tolerancia, std::vector<double>& res, std::string path_reporte_power_method){
    power_method_stop_criteria_t criterio_parada;
    criterio_parada.criterio = CRT_K_FIXED_ITERS_LIMIT;
    criterio_parada.valor.cant_iters = cant_iters;
    criterio_parada.intervalo_iters_reporte = 1;

    std::ofstream reporte_power_method(path_reporte_power_method);
    
    if (!reporte_power_method.is_open()) {
        std::cerr << "Imposible escribir en archivo de reporte de power method: " << path_reporte_power_method << std::endl;
        exit(-1);
    } 

    reporte_power_method.precision(15);
    reporte_power_method.setf(std::ios::fixed,std::ios::floatfield);

    csr.power_method(initial_vec, probabilidad_c, criterio_parada, res, reporte_power_method);

    reporte_power_method.close(); // Seguro esta abierto, sino hubiera ejecutado el exit(-1) en el check cuando lo abro;

    //std::cout << std::abs(0.368 - res[0]) << std::endl;
    //std::cout << std::abs(0.142 - res[1]) << std::endl;
    //std::cout << std::abs(0.288 - res[2]) << std::endl;
    //std::cout << std::abs(0.202 - res[3]) << std::endl;
    
    assert(std::abs(res[0] - 0.368) < epsilon_tolerancia);
    assert(std::abs(res[1] - 0.142) < epsilon_tolerancia);
    assert(std::abs(res[2] - 0.288) < epsilon_tolerancia);
    assert(std::abs(res[3] - 0.202) < epsilon_tolerancia);

    return true;
}

bool power_method_delta_diff(CSR<double>& csr, std::vector<double>& initial_vec, double delta_diff, double probabilidad_c, double epsilon_tolerancia, std::vector<double>& res, std::string path_reporte_power_method){
    power_method_stop_criteria_t criterio_parada;
    criterio_parada.criterio = CRT_K_ITERS_DELTA_DIFF;
    criterio_parada.valor.delta_diff = delta_diff;
    criterio_parada.intervalo_iters_reporte = 1;
    
    std::ofstream reporte_power_method(path_reporte_power_method);
    
    if (!reporte_power_method.is_open()) {
        std::cerr << "Imposible escribir en archivo de reporte de power method: " << path_reporte_power_method << std::endl;
        exit(-1);
    } 

    reporte_power_method.precision(15);
    reporte_power_method.setf(std::ios::fixed,std::ios::floatfield);

    csr.power_method(initial_vec, probabilidad_c, criterio_parada, res, reporte_power_method);    
    
    reporte_power_method.close(); // Seguro esta abierto, sino hubiera ejecutado el exit(-1) en el check cuando lo abro;

    //std::cout << std::abs(0.368 - res[0]) << std::endl;
    //std::cout << std::abs(0.142 - res[1]) << std::endl;
    //std::cout << std::abs(0.288 - res[2]) << std::endl;
    //std::cout << std::abs(0.202 - res[3]) << std::endl;

    assert(std::abs(res[0] - 0.368) < epsilon_tolerancia);
    assert(std::abs(res[1] - 0.142) < epsilon_tolerancia);
    assert(std::abs(res[2] - 0.288) < epsilon_tolerancia);
    assert(std::abs(res[3] - 0.202) < epsilon_tolerancia);
    return true;
}

int main(int argc, char** argv) {

    // Test case: Figure 1 del paper de leise y bryan con su solucion provista en el paper

    DoK<double> dok(4);
    dok[0][0] = 0.00;
    dok[0][1] = 0.00;
    dok[0][2] = 1.00;
    dok[0][3] = 0.50;

    dok[1][0] = 1/(double)3;
    dok[1][1] = 0.00;
    dok[1][2] = 0.00;
    dok[1][3] = 0.00;

    dok[2][0] = 1/(double)3;
    dok[2][1] = 0.50;
    dok[2][2] = 0.00;
    dok[2][3] = 0.50;

    dok[3][0] = 1/(double)3;
    dok[3][1] = 0.50;
    dok[3][2] = 0.00;
    dok[3][3] = 0.00;
    
    std::cout << dok << std::endl;

    CSR<double> csr(dok);

    std::vector<double> initial_vec(csr.columnas(), 1/(double)csr.columnas());

    // Tests con distintos criterios de parada de power method
    std::vector<double> res;

    // Test con delta diff
    std::string path_reporte_power_method = "tests/reporte_power_delta_diff.out";
    std::cout << "Test con delta diff:" << std::endl;
    std::cout << "Initial eigenvector: "<< std::endl;
    std::cout << initial_vec << std::endl;
    assert(power_method_delta_diff(csr, initial_vec, 0.000000000001, 0.85, 0.001, res, path_reporte_power_method));
    std::cout << "Converged eigenvector: "<< std::endl;
    std::cout << res << std::endl << std::endl;

    // Test con iters fijas
    path_reporte_power_method = "tests/reporte_power_method_fijas.out";
    std::cout << "Test con iters fijas:" << std::endl;
    std::cout << "Initial eigenvector: "<< std::endl;
    std::cout << initial_vec << std::endl;
    assert(power_method_iter_fijas(csr, initial_vec, 25, 0.85, 0.001, res, path_reporte_power_method));
    std::cout << "Converged eigenvector: "<< std::endl;
    std::cout << res << std::endl << std::endl;
    return 0;
}
