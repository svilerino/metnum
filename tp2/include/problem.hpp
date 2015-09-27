#ifndef PROBLEM_H
#define PROBLEM_H

#include <cassert>
#include <cmath>
#include <vector>

#include <io.hpp>
#include <timing.hpp>

typedef unsigned int uint;

class Problem {
// Abstrae funciones y datos útiles de un problema
    public:
        Problem(const problem_arguments&);
        ~Problem();

        void resolver_instancias(Results& output, std::ostream& timing_result_os);

    private:
        CSR<double>* csr_ptr;
        std::vector<std::pair<uint,uint> >* in_deg_ptr;
        bool is_pagerank;
};

template<class A, class B>
std::ostream& operator<<(std::ostream& os, const std::pair<A,B>& p)
{
    os << "(" << p.first << ", " << p.second << ")";

    return os;
};
#endif
