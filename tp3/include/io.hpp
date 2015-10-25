#ifndef IO_H
#define IO_H

#include <iostream>
#include <vector>

using namespace std;

typedef unsigned int uint;

// Asumo escala de grises entre 0 255
// Si pinta hacerlo con colores, si cambias pixel_t de unsigned char por un struct unsigned char rojo, verde, azul, fiesta.
typedef unsigned char pixel_t;

typedef vector< vector<pixel_t> > frame_t;

using namespace std;

typedef struct Video
{
	uint frame_count;
	uint frame_alto;
	uint frame_ancho;
	uint frame_rate;

	vector< frame_t > frames;
} Video;

void read_video_from_args(std::istream& is, Video& video);
void write_video_to_stream(std::ostream& os, Video& video);

#endif
