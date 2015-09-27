#include <io.hpp>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include <csr_matrix.tpp>
#include <dok_matrix.tpp>

CSR<double>* read_args_from_stream(std::istream& is,const problem_arguments& args)
{
    DoK<double*> dok_transposed;
    if(!args.is_deportes)
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

        std::vector<double> degs(nodes,0);
        while(edges > 0)
        {
            uint row,col;
            is >> row >> col;
            ++degs[row-1];
            dok_transposed[col-1][row-1] = new double; //de momento, guardo el link
            --edges;
        };
        for(auto it=degs.begin();it!=degs.end();++it) *it = (double)1/(*it);
        for(auto it=dok_transposed.begin();it!=dok_transposed.end();++it) **it->val = degs[it->col];
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
