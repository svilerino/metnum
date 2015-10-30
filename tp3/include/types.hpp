#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

typedef enum interpolation_method_t {NEAREST_NEIGHBOUR, LINEAL, SPLINE} interpolation_method_t;

// Asumo escala de grises entre 0 255
//Nota:Si uso uint8, el parser de txt piensa que son char y se va todo de mambo.
typedef uint8_t pixel_t;

typedef vector< vector<pixel_t> > frame_t;

using namespace std;

typedef struct Video
{
	uint frame_count;
	uint frame_height;
	uint frame_width;
	double frame_rate;

	vector< frame_t > frames;
	
	Video() : frame_count(0), frame_height(0), frame_width(0), frame_rate(0) { frames.resize(0); }
} Video;

typedef struct spline_polynomials_t
{
	vector<double> m_x; 
	vector<double> m_y; 
	vector<double> m_a; 
	vector<double> m_b; 
	vector<double> m_c;
} spline_polynomials_t;

#endif
