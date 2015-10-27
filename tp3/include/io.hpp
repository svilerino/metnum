#ifndef IO_H
#define IO_H

#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

// Asumo escala de grises entre 0 255
//Nota:Si uso uint8, el parser de txt piensa que son char y se va todo de mambo.
typedef uint16_t pixel_t;

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

typedef struct polinomio_spline_t
{
	pixel_t a;
	pixel_t b;
	pixel_t c;
	pixel_t d;
	pixel_t xj;
} polinomio_spline_t;

typedef vector< polinomio_spline_t > pixel_polynomial_t;
typedef vector< vector<pixel_polynomial_t> > block_spline_polynomials_t;

// Txt impl
//void read_video_from_args(std::istream& is, Video& video);
//void write_video_to_stream(std::ostream& os, Video& video);

#endif
