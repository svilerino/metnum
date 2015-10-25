#ifndef SLOWMOTION_H
#define SLOWMOTION_H

#include <io.hpp>

typedef enum interpolation_method_t {NEAREST_NEIGHBOUR, LINEAL, SPLINE} interpolation_method_t;

class SlowMotionEffect {
	public:
		static void interpolate(interpolation_method_t interp_method, uint interpol_frame_count, Video& video_input, Video& video_output);

	private:
		static void nearest_neighbour_interpolation(interpolation_method_t interp_method, uint interpol_frame_count, Video& video_input, Video& video_output);
};

#endif
