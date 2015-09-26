#ifndef PROBLEM_H
#define PROBLEM_H

#include <math.h>
#include <io.hpp>
#include <timing.hpp>
#include <cassert>

class Problem {
	// Abstrae funciones y datos útiles de un problema
public:
    Problem(ProblemArguments &in_args);

    void resolver_instancias(Results &output, std::ostream &timing_result_os);

private:

};
#endif
