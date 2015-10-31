#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <types.hpp>

#define SPLINE_BLOCK_SIZE 16

class Interpolation {
	public:
		static double linear_interpolation(double x0, double y0, double x1, double y1, double x);
		static void saturate_8bit(double &x);

		static void calculate_spline_polynomials(
			const vector<double>& m_x, 
			const vector<double>& m_y, 
			spline_polynomials_t& polinomios);
			
		static double eval_spline(
			const double x, 
			spline_polynomials_t& polinomios);

	private:

};

#endif
