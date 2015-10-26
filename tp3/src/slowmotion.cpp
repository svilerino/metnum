#include <slowmotion.hpp>
#include <cmath>

// ---
// Los videos de entrada y salida son distintos asi puedo llamar iterativamente para medir tiempo promedio sin perder el input.
// ---

void SlowMotionEffect::interpolate(interpolation_method_t interp_method, uint interpol_frame_count, Video& video_input, Video& video_output)
{
	
	//Initialize video output struct	
	video_output.frame_height = video_input.frame_height;
	video_output.frame_width = video_input.frame_width;
	video_output.frame_rate = video_input.frame_rate;
	//Calculate particular output video metadata
	uint new_frame_count = video_input.frame_count + (video_input.frame_count-1)*interpol_frame_count;
	video_output.frame_count = new_frame_count;
	
	// Avoid dynamic reallocation in between frames insertion.
	video_output.frames.resize(new_frame_count);

	if (interp_method == NEAREST_NEIGHBOUR){
		
		nearest_neighbour_interpolation(interp_method, interpol_frame_count, video_input, video_output);

	} else if (interp_method == LINEAL){

		progressive_linear_interpolation(interp_method, interpol_frame_count, video_input, video_output);

	} else if (interp_method == SPLINE){

	}else{
		cerr << "Invalid interpolation mode: " << interp_method << endl;
		exit(-1);
	}

}

void SlowMotionEffect::progressive_linear_interpolation(interpolation_method_t interp_method, uint interpol_frame_count, Video& video_input, Video& video_output)
{
	uint16_t output_frame_idx = 0;
	for (uint cur_frame = 0; cur_frame < video_input.frame_count - 1; cur_frame++)
	{
		// Add current input frame
		video_output.frames[output_frame_idx++] = video_input.frames[cur_frame];

		//Interpolate new frames from current and next
		double linear_step = 1 / (double)(interpol_frame_count+1);

		for (uint j = 0; j < interpol_frame_count; j++)
		{
			frame_t mixed_frame;
			create_linear_frame_mix(video_input.frames[cur_frame], video_input.frames[cur_frame + 1], mixed_frame, (j+1)*linear_step);

			//Add the mixed frame
			video_output.frames[output_frame_idx++] = mixed_frame;
		}
	}

	// Last frame
	video_output.frames[output_frame_idx++] = video_input.frames[video_input.frame_count - 1];
}

void SlowMotionEffect::create_linear_frame_mix(frame_t& left_frame, frame_t& right_frame, frame_t& mixed_frame, double linear_step)
{

	uint frame_height = left_frame.size();
	uint frame_width = left_frame[1].size();

	mixed_frame.resize(frame_height);

	for (uint i = 0; i < frame_height; i++)
	{
		mixed_frame[i].resize(frame_width);

		for (uint j = 0; j < frame_width; j++)
		{
			mixed_frame[i][j] = linear_interpolation(0,
			 										left_frame[i][j],
			 										1,
			 										right_frame[i][j],
			 										linear_step
			 										);
		}	
	}
}

double SlowMotionEffect::linear_interpolation(double x0, double y0, double x1, double y1, double x)
{
	double y = y0 + (y1-y0) * ( (x - x0) / (x1 - x0) );
	return y;
}


void SlowMotionEffect::nearest_neighbour_interpolation(interpolation_method_t interp_method, uint interpol_frame_count, Video& video_input, Video& video_output)
{
	uint16_t output_frame_idx = 0;
	for (uint cur_frame = 0; cur_frame < video_input.frame_count - 1; cur_frame++)
	{
		// Add current input frame
		video_output.frames[output_frame_idx++] = video_input.frames[cur_frame];

		// Insert new <interpol_frame_count> interpolated frames

		// First half
		//cout << "[0.." << (interpol_frame_count / 2) << ")" <<  endl;
		for (uint j = 0; j < (interpol_frame_count / 2); j++)
		{
			// Copy current frame
			//cout << "primer for con indice " << j << endl;
			video_output.frames[output_frame_idx++] = video_input.frames[cur_frame];
		}

		// Second half
		//cout << "[" << (interpol_frame_count / 2) << ".." << interpol_frame_count << ")" << endl;
		for (uint j = (interpol_frame_count / 2); j < interpol_frame_count; j++)
		{
			// Copy next frame
			//cout << "segundo for con indice " << j << endl;
			video_output.frames[output_frame_idx++] = video_input.frames[cur_frame + 1];
		}
	}

	// Last frame
	video_output.frames[output_frame_idx++] = video_input.frames[video_input.frame_count - 1];
}