#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#define SPLINE_BLOCK_SIZE 4
#include <io.hpp>

class Interpolation {
	public:
		static double linear_interpolation(double x0, double y0, double x1, double y1, double x);
		static double evaluate_cubic_spline(polinomio_spline_t& pixel_polynomial, double x);
		static void create_spline_polynomial(const vector<uint>& x0, const vector<pixel_t>& y0, pixel_polynomial_t& pol_interpolate);
		static void saturate_8bit(double &x);
	
	private:

};

#endif
