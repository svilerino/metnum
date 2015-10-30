#include <interpolation.hpp>
#include <cmath>
#include <cassert>

using namespace std;

double Interpolation::linear_interpolation(double x0, double y0, double x1, double y1, double x)
{
	double y = y0 + (y1-y0) * ( (x - x0) / (x1 - x0) );
	return y;
}

void Interpolation::saturate_8bit(double &x)
{
    if ( x < 0 )
    {
        x = 0;
    }
    else if(x > 255)
    {
        x = 255;
    }
}

double Interpolation::evaluate_cubic_spline(polinomio_spline_t& pixel_single_polynomial, double x)
{
	double coef_a = pixel_single_polynomial.a;
	double coef_b = pixel_single_polynomial.b;
	double coef_c = pixel_single_polynomial.c;
	double coef_d = pixel_single_polynomial.d;
	double xj = pixel_single_polynomial.xj;

	//cout << coef_a << " " << coef_b << " " << coef_c << " " << coef_d << endl;
	//cout << xj << endl;

	double result = (coef_a + coef_b * (x - xj) + coef_c * pow(x - xj, 2) + coef_d * pow(x - xj, 3));
	
	//cout << result << endl;
	
	assert(result >=0);

	return result;
    // Sj(x) = a_{j} + b_{j} [(x - x_{j})] + c_{j} [(x - x_{j})^2] + d_{j} [(x - x_{j})^3]
}


void Interpolation::create_spline_polynomial(const vector<uint>& x0, const vector<pixel_t>& y0, pixel_polynomial_t& pol_interpolate)
{
   int n = x0.size()-1;

   vector<double> a;
   a.insert(a.begin(), y0.begin(), y0.end()); // Step 1 : a_i = y_i

   // Step 2 : create b and d ( size n )
   vector<double> b(n);
   vector<double> d(n);
   
   // Step 3 : create h ( size n ) where h_i = x_{i+1} - x_{i}
   vector<double> h;
   for(int i = 0; i < n; ++i)
       h.push_back(x0[i+1]-x0[i]);

   // Step 4 : create alpha (size n) where alpha_i = (3/h_i) * (a_i+1 - a_i) - (3/h_i-1) * (a_i - a_i-1)
   vector<double> alpha;
   for(int i = 0; i < n; ++i)
       alpha.push_back( ( 3*(a[i+1]-a[i])/h[i] ) - ( 3*(a[i]-a[i-1])/h[i-1] ) );


   // Step 5 : create array c, l, mu and z ( size n+1 )
   vector<double> c(n+1);
   vector<double> l(n+1);
   vector<double> mu(n+1);
   vector<double> z(n+1);

   // Step 6 : Set l[0] = 1 , mu[0] = 0 and z[0] = 0
   l[0] = 1;
   mu[0] = 0;
   z[0] = 0;

   // Step 7 : (i = 1 .. n) 
   // 	Set l_i = 2 (x_i+1 - x_i-1) - (h[i-1]* mu[i-1])
   //	Set u_i = h_i / l_i
   // 	Set z_i = (alpha_i - (h_i-1 * z_i-1)) / l_i
   for(int i = 1; i < n; ++i)
   {
       l[i] = ( 2 *(x0[i+1]-x0[i-1]) ) - (h[i-1]*mu[i-1]);
       mu[i] = h[i]/l[i];
       z[i] = (alpha[i]-(h[i-1]*z[i-1]))/l[i];
   }

   // Step 8 : Set l[n] = 1, z[n] = 0 and c[n] = 0
   l[n] = 1;
   z[n] = 0;
   c[n] = 0;

   // Step 9 : (j = n-1 .. 0) 
   //	Set c_j = z_j - mu_j * c_j+1
   //	Set b_j = ((a[j+1] - a_j)/h_j) - ( h_j * (c_j+1 + 2 c_j) / 3)
   //	Set d_j = (c_j+1 - c_j) / ( 3 * h_j)
   for(int j = n-1; j >= 0; --j)
   {
       c[j] = z [j] - (mu[j] * c[j+1]);
       b[j] = ( (a[j+1]-a[j])/h[j] ) - (h[j]* (c[j+1] + 2*c[j])/ 3);
       d[j] = (c[j+1]-c[j])/(3*h[j]);
   }

   pol_interpolate.resize(n);
   for(int j = 0;j < n; ++j)
   {
       pol_interpolate[j].a =  a[j];
       pol_interpolate[j].b =  b[j];
       pol_interpolate[j].c =  c[j];
       pol_interpolate[j].d =  d[j];
       pol_interpolate[j].xj = x0[j];

       // Sj(x) = a_{j} + b_{j} [(x - x_{j})] + c_{j} [(x - x_{j})^2] + d_{j} [(x - x_{j})^3]
   } 
}