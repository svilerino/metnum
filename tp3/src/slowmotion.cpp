#include <slowmotion.hpp>
#include <cmath>

// ---
// Los videos de entrada y salida son distintos asi puedo llamar iterativamente para medir tiempo promedio sin perder el input.
// ---

void SlowMotionEffect::interpolate(interpolation_method_t interp_method, uint interpol_frame_count, Video& video_input, Video& video_output)
{
	
	//Initialize common video output struct	
	video_output.frame_height = video_input.frame_height;
	video_output.frame_width = video_input.frame_width;
	video_output.frame_rate = video_input.frame_rate;

	if (interp_method == NEAREST_NEIGHBOUR){
		
		//Calculate particular output video metadata
		uint new_frame_count = video_input.frame_count + (video_input.frame_count-1)*interpol_frame_count;
		video_output.frame_count = new_frame_count;
		
		// Avoid dynamic reallocation in between frames insertion.
		video_output.frames.resize(new_frame_count);
		
		nearest_neighbour_interpolation(interp_method, interpol_frame_count, video_input, video_output);

	} else if (interp_method == LINEAL){



		interpol_frame_count = 1;


		//Calculate particular output video metadata
		uint new_frame_count = video_input.frame_count + (video_input.frame_count-1)*interpol_frame_count;
		video_output.frame_count = new_frame_count;
		
		// Avoid dynamic reallocation in between frames insertion.
		video_output.frames.resize(new_frame_count);
		
		linear_interpolation(interp_method, interpol_frame_count, video_input, video_output);

	} else if (interp_method == SPLINE){

	}else{
		cerr << "Invalid interpolation mode: " << interp_method << endl;
		exit(-1);
	}

}

void SlowMotionEffect::linear_interpolation(interpolation_method_t interp_method, uint interpol_frame_count, Video& video_input, Video& video_output)
{
	uint16_t output_frame_idx = 0;
	for (uint cur_frame = 0; cur_frame < video_input.frame_count - 1; cur_frame++)
	{
		// Add current input frame
		video_output.frames[output_frame_idx++] = video_input.frames[cur_frame];

		//Interpolate new frame from current and next
		frame_t mixed_frame;
		create_linear_frame_mix(video_input.frames[cur_frame], video_input.frames[cur_frame + 1], mixed_frame);

		//Add the mixed frame
		video_output.frames[output_frame_idx++] = mixed_frame;
	}

	// Last frame
	video_output.frames[output_frame_idx++] = video_input.frames[video_input.frame_count - 1];
}

void SlowMotionEffect::create_linear_frame_mix(frame_t& left_frame, frame_t& right_frame, frame_t& mixed_frame)
{

	uint frame_height = left_frame.size();
	uint frame_width = left_frame[1].size();

	mixed_frame.resize(frame_height);

	for (uint i = 0; i < frame_height; i++)
	{
		mixed_frame[i].resize(frame_width);

		for (uint j = 0; j < frame_width; j++)
		{
			mixed_frame[i][j] = ( left_frame[i][j] + right_frame[i][j] ) / 2;
		}	
	}
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