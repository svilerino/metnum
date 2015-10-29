#ifndef SLOWMOTION_H
#define SLOWMOTION_H

#include <io.hpp>

typedef enum interpolation_method_t {NEAREST_NEIGHBOUR, LINEAL, SPLINE} interpolation_method_t;

#define SPLINE_BLOCK_SIZE 4

class SlowMotionEffect {
	public:
		static void interpolate(interpolation_method_t interp_method, uint interpol_frame_count, Video& video_input, Video& video_output);

	private:
		static void nearest_neighbour_interpolation(interpolation_method_t interp_method, uint interpol_frame_count, Video& video_input, Video& video_output);
		static void progressive_linear_interpolation(interpolation_method_t interp_method, uint interpol_frame_count, Video& video_input, Video& video_output);
		static void create_linear_frame_mix(frame_t& left_frame, frame_t& right_frame, frame_t& mixed_frame, double linear_step);
		static double linear_interpolation(double x0, double y0, double x1, double y1, double x);
		static pixel_t evaluate_cubic_spline(polinomio_spline_t& pixel_polynomial, double x);

		static void spline_method_interpolation(interpolation_method_t interp_method, uint interpol_frame_count, uint spline_window_size, Video& video_input, Video& video_output);
		//ending frame is not inclusive [starting_frame..ending_frame)
		static void process_spline_block(Video& video_input, Video& video_output, const uint starting_frame, const uint ending_frame, uint interpol_frame_count);
		static void create_spline_polynomial(const vector<uint>& x0, const vector<pixel_t>& y0, pixel_polynomial_t& pol_interpolate);
		static void create_spline_frame_mix(uint frame_height, uint frame_width, frame_t& mixed_frame, block_spline_polynomials_t& pixel_polynomials, double spline_step, int position_frame);

};

#endif
