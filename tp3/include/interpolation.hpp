#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <io.hpp>

#define SPLINE_BLOCK_SIZE 4

class Interpolation {
	public:
		static double linear_interpolation(double x0, double y0, double x1, double y1, double x);
		static void saturate_8bit(double &x);
	
	private:

};

#endif
