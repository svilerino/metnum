#include <frame_analysis.hpp>
#include <cmath>
#include <cassert>

double Frame_Analysis::ecm(frame_t expected, frame_t observed)
{
	assert(expected.size() == observed.size());
	assert(expected.size() > 0);
	
	uint filas = expected.size();

	for (uint i = 0; i < filas; ++i)
	{
		assert(expected[i].size() == observed[i].size());
		assert(expected[i].size() > 0);		
	}

	uint columnas = expected[0].size();

	double acum = 0;
	for (uint i = 0; i < filas; i++)
	{
		for (uint j = 0; j < columnas; j++)
		{
			acum += pow((expected[i][j] - observed[i][j]), 2);
		}
	}
	acum /= (filas * columnas);
	return acum;
}

double Frame_Analysis::psnr(frame_t expected, frame_t observed)
{
	double aux = (255*255)/ecm(expected, observed);
	double psnr = 10*log10(aux); 
	return psnr;
}