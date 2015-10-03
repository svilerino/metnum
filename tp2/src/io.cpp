#include <io.hpp>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <string>

#include <csr_matrix.tpp>
#include <dok_matrix.tpp>

problem_arguments::problem_arguments(char** argv, int argc)
{
    const std::string& input_filename = argv[1];
    const std::string& output_filename = argv[2];
    std::ifstream is(input_filename,std::ifstream::in);
    if (!is.is_open())
    {
        std::cerr << "Path de archivo de entrada invalido: " << input_filename << std::endl;
        exit(-1);
    } else is >> is_pagerank >> c >> is_deportes >> input_file_path >> epsilon;

    output_file_path = output_filename;
    is_pagerank = !is_pagerank;


    //  Parametros adicionales
    std::string extracted_test_name = output_filename.substr(0, output_filename.find_last_of("\\."));

    // Modo de corte power_method
    if(argc > 3){
        if(atoi(argv[3]) == 0){
            power_method_mode = CRT_K_FIXED_ITERS_LIMIT;
        }else if(atoi(argv[3]) == 1){
            power_method_mode = CRT_K_ITERS_DELTA_DIFF; // fixed iters(interpreta el argumento de la tolerancia como cantidad de iters en int)
        }
    }else {
        power_method_mode = CRT_K_ITERS_DELTA_DIFF; // fixed iters(interpreta el argumento de la tolerancia como cantidad de iters en int)
    }

    // Vector inicial aleatorio para power method / false => equiprobable en R**n
    random_initial_vector = (argc > 4) ? atoi(argv[4]) : false;

    // Path archivo de reporte
    pm_reporte_path = (argc > 5) ? argv[5] : (extracted_test_name + ".convergence");

    // Path archivo de timing
    timing_path = (argc > 6) ? argv[6] : (extracted_test_name + ".timing");
};

CSR<double>* read_args_from_stream_pagerank(std::istream& is,const problem_arguments& args)
{
    uint nodes,edges;
    DoK<double>* dok_ptr;
    if(!args.is_deportes)
    {
        is.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        is.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        is.ignore(std::numeric_limits<std::streamsize>::max(),':');
        is >> nodes;
        is.ignore(std::numeric_limits<std::streamsize>::max(),':');
        is >> edges;
        is.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        is.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

#ifdef _INPUT_FROM_ZERO
        std::cerr << "[_INPUT_FROM_ZERO mode: asume que los nodos comienzan en uno]" << std::endl;
#endif
        dok_ptr = new DoK<double>(nodes, nodes);
        assert(dok_ptr != nullptr);

        std::vector<double> degs(nodes, 0);
        while(edges > 0)
        {
            uint row,col;
            is >> row >> std::skipws >> col;
#ifdef _INPUT_FROM_ZERO
            assert(row < nodes);
            assert(col < nodes);
            ++degs[row];
            (*dok_ptr)[col][row] = 1; //de momento, guardo el link
#else
            assert(row-1 < nodes);
            assert(col-1 < nodes);
            ++degs[row-1];
            (*dok_ptr)[col-1][row-1] = 1; //de momento, guardo el link
#endif
            --edges;
        };
        for(auto it=degs.begin();it!=degs.end();++it) *it = (double)1/(*it);
        for(auto it=dok_ptr->begin();it!=dok_ptr->end();++it) *it->val = degs[it->col];

    } else {
        is >> nodes >> edges;
        dok_ptr = new DoK<double>(nodes,nodes);
        assert(dok_ptr != nullptr);
        std::vector<uint> diff_goles_partidos_perdidos(nodes,0);
        while(edges > 0)
        {
            uint fecha,team_win,goals_win,team_loser,goals_loser;
            is >> fecha >> team_win >> goals_win >> team_loser >> goals_loser;
            --team_win;--team_loser;
            double goals_diff = goals_win-goals_loser;
            diff_goles_partidos_perdidos[team_loser]+=goals_diff;
            (*dok_ptr)[team_win][team_loser]+=goals_diff;
            --edges;
        };
        for(auto it=dok_ptr->begin();it!=dok_ptr->end();++it)
            *it->val/=diff_goles_partidos_perdidos[it->col];
    };

    CSR<double>* result = new CSR<double>(*dok_ptr);

    delete dok_ptr;
    return result;
};

std::vector<std::pair<uint,uint> >* read_args_from_stream_indeg(std::istream& is,const problem_arguments& args)
{
    uint nodes,edges;
    is.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    is.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    is.ignore(std::numeric_limits<std::streamsize>::max(),':');
    is >> nodes;
    is.ignore(std::numeric_limits<std::streamsize>::max(),':');
    is >> edges;
    is.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    is.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

    std::vector<std::pair<uint,uint> >* degs_ptr = new std::vector<std::pair<uint,uint> >(nodes);
    assert(degs_ptr != nullptr);
    for(uint i=1;i<=nodes;++i) (*degs_ptr)[i-1].second = i;
    while(edges > 0)
    {
        uint row,col;
        is >> row >> col;
        ++(*degs_ptr)[col-1].first;
        --edges;
    };

    return degs_ptr;
};
void write_results_to_stream(std::ostream &os, Results output_results) {
    os.precision(5);
    os.setf(std::ios::fixed,std::ios::floatfield);
}

std::ostream& operator<<(std::ostream& os, const problem_arguments& args)
{
    os << "Método: "; if(args.is_pagerank) os << "PageRank"; else os << "Alternativo"; os << std::endl;
    os << "Factor de navegacion: " << args.c << std::endl;
    os << "Tipo de instancia: "; if(args.is_deportes) os << "Deportes"; else os << "Páginas Web"; os << std::endl;
    os << "Archivo de Entrada: " << args.input_file_path << std::endl;
    os << "Modo metodo de la Potencia: " << ((args.power_method_mode == 0) ? "Cantidad de iteraciones fijas" : "Iterar hasta diferencia menor a tolerancia") << std::endl;
    os << "Tolerancia/Cant.iters fijas Metodo de la Potencia: " << args.epsilon << std::endl;
    os << "Vector inicial random: " << (args.random_initial_vector ? "Si" : "No") << std::endl;
    os << "Archivo de convergencia: " << args.pm_reporte_path << std::endl;
    os << "Archivo de timing: " << args.timing_path;

    return os;
};
