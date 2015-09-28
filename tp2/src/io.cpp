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
    std::string extracted_test_name = input_file_path.substr(0, input_file_path.find_last_of("\\."));

    // Vector inicial aleatorio para power method / false => equiprobable en R**n
    random_initial_vector = (argc > 3) ? atoi(argv[3]) : false;

    // Path archivo de reporte
    pm_reporte_path = (argc > 4) ? argv[4] : (extracted_test_name + ".convergence");

    // Path archivo de timing
    timing_path = (argc > 5) ? argv[5] : (extracted_test_name + ".timing");
};

CSR<double>* read_args_from_stream_pagerank(std::istream& is,const problem_arguments& args)
{
    uint nodes,edges;
    DoK<double> dok_transposed;
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

        std::vector<double> degs(nodes,0);
        while(edges > 0)
        {
            uint row,col;
            is >> row >> col;
            ++degs[row-1];
            dok_transposed[col-1][row-1] = 1; //de momento, guardo el link
            --edges;
        };
        for(auto it=degs.begin();it!=degs.end();++it) *it = (double)1/(*it);
        for(auto it=dok_transposed.begin();it!=dok_transposed.end();++it) *it->val = degs[it->col];

    } else {
        is >> nodes >> edges;
        std::vector<uint> diff_goles_partidos_perdidos(nodes,0);
        while(edges > 0)
        {
            uint fecha,team_win,goals_win,team_loser,goals_loser;
            is >> fecha >> team_win >> goals_win >> team_loser >> goals_loser;
            --team_win;--team_loser;
            double goals_diff = goals_win-goals_loser;
            diff_goles_partidos_perdidos[team_loser]+=goals_diff;
            dok_transposed[team_loser][team_win]+=goals_diff;
            --edges;
        };
        for(auto it=dok_transposed.begin();it!=dok_transposed.end();++it) *it->val/=diff_goles_partidos_perdidos[it->row];
    };

    return new CSR<double>(dok_transposed);
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
    os << "Probabilidad de Teletransportación: " << args.c << std::endl;
    os << "Tipo de instancia: "; if(args.is_deportes) os << "Deportes"; else os << "Páginas Web"; os << std::endl;
    os << "Archivo de Entrada: " << args.input_file_path << std::endl;
    os << "Tolerancia Metodo de la Potencia: " << args.epsilon << std::endl;
    os << "Vector inicial random: " << (args.random_initial_vector ? "Si" : "No") << std::endl;
    os << "Archivo de convergencia: " << args.pm_reporte_path << std::endl;
    os << "Archivo de timing: " << args.timing_path;

    return os;
};
