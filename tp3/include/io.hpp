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

using namespace std;

typedef struct Video
{
	uint frame_count;
	uint frame_alto;
	uint frame_ancho;
	uint frame_rate;

	vector< frame_t > frames;
	
	Video() : frame_count(0), frame_alto(0), frame_ancho(0), frame_rate(0) { frames.resize(0); }
} Video;

void read_video_from_args(std::istream& is, Video& video);
void write_video_to_stream(std::ostream& os, Video& video);

#endif
