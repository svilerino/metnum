#ifndef PROBLEM_H
#define PROBLEM_H
#define CANT_ITERS_MEDICION 1

#include <math.h>
#include "io.h"
#include "timing.h"
#include <cassert>

class Problem {
	// Abstrae funciones y datos útiles de un problema
public:
    Problem(ProblemArguments &in_args);

    void resolver_instancias(Results &output, ostream &timing_result_os);

private:

};
#endif
