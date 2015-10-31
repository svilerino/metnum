#ifndef FRAME_ANALYSIS_H
#define FRAME_ANALYSIS_H

#include <types.hpp>

class Frame_Analysis{
	static double ecm(frame_t expected, frame_t observed);
	static double psnr(frame_t expected, frame_t observed);
};

#endif