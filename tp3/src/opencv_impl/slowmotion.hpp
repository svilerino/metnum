#ifndef SLOWMOTION_H
#define SLOWMOTION_H

#include <io.hpp>

typedef enum interpolation_method_t {NEAREST_NEIGHBOUR, LINEAR, SPLINE} interpolation_method_t;

class SlowMotionEffect {
	public:
		static void interpolate(interpolation_method_t interp_method, uint interpol_frame_count, Video& video_input, Video& video_output);

	private:
		static void nearest_neighbour_interpolation(interpolation_method_t interp_method, uint interpol_frame_count, Video& video_input, Video& video_output);
		static void progressive_linear_interpolation(interpolation_method_t interp_method, uint interpol_frame_count, Video& video_input, Video& video_output);

		static void create_linear_frame_mix(frame_t& left_frame, frame_t& right_frame, frame_t& mixed_frame, double linear_step);
		static double linear_interpolation(double x0, double y0, double x1, double y1, double x);
		//static std::vector<PolinomioSpline> spline_algorithm(vector<pixel_t> x0, vector<pixel_t> y0);
		//static void add_spline_frame_value(frame_t& current_frame, int indice_x0, frame_t& frame_x0, frame_t& frame_y0);
};

#endif
