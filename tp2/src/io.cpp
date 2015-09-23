#include<io.hpp>

void read_args_from_stream(std::istream &is, ProblemArguments &args) {
}

void write_results_to_stream(std::ostream &os, Results output_results) {
    os.precision(5);
    os.setf(std::ios::fixed,std::ios::floatfield);
}

void mostrar_datos_entrada(ProblemArguments args, std::ostream &os) {

}
