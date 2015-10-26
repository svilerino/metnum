#ifndef IO_H
#define IO_H

#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

// Asumo escala de grises entre 0 255
//Si uso uint8, el parser piensa que son char y se va todo de mambo.
typedef uint16_t pixel_t;

typedef vector< vector<pixel_t> > frame_t;

typedef struct PolinomioSpline
{
	pixel_t a;
	pixel_t b;
	pixel_t c;
	pixel_t d;
	pixel_t xj;
};

typedef vector< vector< PolinomioSpline > > polinomio_conjunto;

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

#endif
