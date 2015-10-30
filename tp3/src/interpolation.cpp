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