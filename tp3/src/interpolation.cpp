#include <interpolation.hpp>
#include <cmath>
#include <cassert>
#include <matriz.h>

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

// spline implementation
// -----------------------

void Interpolation::calculate_spline_polynomials(const vector<double>& m_x, const vector<double>& m_y,
    spline_polynomials_t& polinomios)
{

    // Validar input

    assert(m_x.size() > 2);
    assert(m_x.size() == m_y.size());

    uint n = m_x.size();

    // Check sorted m_x input
    for(uint i=0; i<n-1; i++) {
        assert(m_x[i ]< m_x[i+1]);
    }

    // f(x) = a*(x-x_i)^3 + b*(x-x_i)^2 + c*(x-x_i) + y_i

    polinomios.m_x = m_x;

    // spline coefficients
    polinomios.m_a.resize(n, 0);
    polinomios.m_b.resize(n, 0);
    polinomios.m_c.resize(n, 0);

    // Sistema lineal para armar spline
    Matriz A(n, n);
    vector<double> vect_indep(n);

    // Armar matriz entre 1 y n-1
    for(uint i = 1; i < n-1; i++)
    {
        A[i][i-1] = (m_x[i] - m_x[i-1]);
        A[i][i] = 2.0*(m_x[i+1] - m_x[i-1]);
        A[i][i+1] = (m_x[i+1] - m_x[i]);
        vect_indep[i]=3.0*((m_y[i+1]-m_y[i]) / (m_x[i+1]-m_x[i]) - (m_y[i]-m_y[i-1])/(m_x[i]-m_x[i-1]));
    }

    // Agregar ecuaciones de condicion de frontera

    // b[0] = f''
    A[0][0] = 1.0;
    A[0][1] = 0.0;
    vect_indep[0] = 0.0;// 2nd derivative 0 in the left edge

    // b[n-1] = f''
    A[n-1][n-1] = 1.0;
    A[n-1][n-2] = 0.0;
    vect_indep[n-1] = 0.0; // 2nd derivative 0 in the left edge

    // Resolver sistema lineal
    SistemaEcuaciones sist_ec(A, vect_indep);

    sist_ec.eliminacion_gaussiana(false/*no usar pivoteo parcial*/, polinomios.m_b);

    // Despejar a[] y c[] en base a b[]

    for(uint i=0; i<n-1; i++) {
        polinomios.m_a[i]=1.0/3.0*(polinomios.m_b[i+1]-polinomios.m_b[i])/(m_x[i+1]-m_x[i]);
        polinomios.m_c[i]=(m_y[i+1]-m_y[i])/(m_x[i+1]-m_x[i])
               - 1.0/3.0*(2.0*polinomios.m_b[i]+polinomios.m_b[i+1])*(m_x[i+1]-m_x[i]);
    }

    polinomios.m_y = m_y;

}

double Interpolation::eval_spline(const double x, spline_polynomials_t& polinomios)
{
    size_t n=polinomios.m_x.size();
    // find the closest point polinomios.m_x[idx] < x, idx=0 even if x<polinomios.m_x[0]
    vector<double>::const_iterator it;
    it=lower_bound(polinomios.m_x.begin(),polinomios.m_x.end(),x);
    uint idx = max( int(it-polinomios.m_x.begin())-1, 0);

    double h = x - polinomios.m_x[idx];

    // Let's check the evaluation is inside the spline range.
    assert(x >= polinomios.m_x[0]);
    assert(x <= polinomios.m_x[n-1]);

    // interpolation
    double interpol = ((polinomios.m_a[idx]*h + polinomios.m_b[idx])*h + polinomios.m_c[idx])*h + polinomios.m_y[idx];

    return interpol;
}
