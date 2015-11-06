#ifndef SLOWMOTION_H
#define SLOWMOTION_H

#include <types.hpp>
#include <vector>
#include <interpolation.hpp>

class SlowMotionEffect {
	public:
		static void slowmotion(interpolation_method_t interp_method, uint interpol_frame_count, const Video& video_input, Video& video_output, uint spline_block_size);

	private:
		static void nearest_neighbour_interpolation(interpolation_method_t interp_method, uint interpol_frame_count, const Video& video_input, Video& video_output);

		static void progressive_linear_interpolation(interpolation_method_t interp_method, uint interpol_frame_count, const Video& video_input, Video& video_output);
		static void create_linear_frame_mix(const frame_t& left_frame, const frame_t& right_frame, frame_t& mixed_frame, double linear_step);

		static void spline_method_interpolation(interpolation_method_t interp_method, uint interpol_frame_count, uint spline_window_size, const Video& video_input, Video& video_output);
		//ending frame is not inclusive [starting_frame..ending_frame)
		static void process_spline_block(const Video& video_input, Video& video_output, uint starting_frame, const uint ending_frame, uint interpol_frame_count,bool enough_trailing_frames = true);
		static void create_spline_frame_mix(uint frame_height, uint frame_width, frame_t& mixed_frame, double spline_step, int position_frame, vector< vector<spline_polynomials_t> >& splines_list);

};

#endif
