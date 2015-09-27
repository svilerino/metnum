#include <io.hpp>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include <csr_matrix.tpp>
#include <dok_matrix.tpp>

CSR<double>* read_args_from_stream(std::istream& is,const problem_arguments& args)
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
    os << "Tolerancia Metodo de la Potencia: " << args.epsilon;
    return os;
};
