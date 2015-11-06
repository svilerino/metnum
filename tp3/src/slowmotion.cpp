#include <vector_operations.tpp>
#include <slowmotion.hpp>
#include <vector>
#include <iostream>
#include <unistd.h>

using namespace std;
// ---
// Los videos de entrada y salida son distintos asi puedo llamar iterativamente para medir tiempo promedio sin perder el input.
// ---

void SlowMotionEffect::slowmotion(interpolation_method_t interp_method, uint interpol_frame_count, const Video& video_input, Video& video_output, uint spline_block_size)
{

	//Initialize video output struct
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
		//Calculate particular output video metadata
		uint new_frame_count = video_input.frame_count + (video_input.frame_count-1)*interpol_frame_count;
		video_output.frame_count = new_frame_count;

		// Avoid dynamic reallocation in between frames insertion.
		video_output.frames.resize(new_frame_count);
		progressive_linear_interpolation(interp_method, interpol_frame_count, video_input, video_output);

	} else if (interp_method == SPLINE){
		// Use dynamic reallocation for output frames.
		video_output.frames.clear();

		spline_method_interpolation(interp_method, interpol_frame_count, spline_block_size, video_input, video_output);

		video_output.frame_count = video_output.frames.size();

	}else{
		cerr << "Invalid interpolation mode: " << interp_method << endl;
		exit(-1);
	}

}

void SlowMotionEffect::progressive_linear_interpolation(interpolation_method_t interp_method, uint interpol_frame_count, const Video& video_input, Video& video_output)
{
	uint16_t output_frame_idx = 0;
	// cout << video_input.frame_count << " frames in video" << endl;
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
			// cout << output_frame_idx << endl;
		}
	}

	// Last frame
	video_output.frames[output_frame_idx++] = video_input.frames[video_input.frame_count - 1];
}

void SlowMotionEffect::create_linear_frame_mix(const frame_t& left_frame, const frame_t& right_frame, frame_t& mixed_frame, double linear_step)
{

	uint frame_height = left_frame.size();
	uint frame_width = left_frame[1].size();

	mixed_frame.resize(frame_height);

	for (uint i = 0; i < frame_height; i++)
	{
		mixed_frame[i].resize(frame_width);

		for (uint j = 0; j < frame_width; j++)
		{
			double interpol = Interpolation::linear_interpolation(0,
			 										left_frame[i][j],
			 										1,
			 										right_frame[i][j],
			 										linear_step
			 										);
			Interpolation::saturate_8bit(interpol);

			mixed_frame[i][j] = (pixel_t) interpol;
		}
	}
}

void SlowMotionEffect::nearest_neighbour_interpolation(interpolation_method_t interp_method, uint interpol_frame_count, const Video& video_input, Video& video_output)
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
void SlowMotionEffect::spline_method_interpolation(interpolation_method_t interp_method, uint interpol_frame_count, uint spline_block_size, const Video& video_input, Video& video_output)
{

	assert(spline_block_size > 2);

	uint blocks_count = (uint) (video_input.frame_count / spline_block_size);
	uint remaining_trailing_frames = (uint) (video_input.frame_count % spline_block_size);

	// cout << video_input.frame_count << " frames in video" << endl;
	// cout << blocks_count << " blocks of " << spline_block_size << " frames" << endl;

	// cout << "Trailing frames: " << remaining_trailing_frames << endl;
	//if(remaining_trailing_frames > 1)
	//{
	//	blocks_count++; // extra trailing frames block
	//	// cout << "Blocks including trailing frames: " << blocks_count << endl;
	//}

	// Necesito al menos 3 para aplicar splines en el ultimo bloque.
	//assert((remaining_trailing_frames == 0) || (remaining_trailing_frames >=3));
	assert(remaining_trailing_frames >= 0);

	// Process blocks
	uint starting_frame = 0;
	//for (uint block_idx = 0; block_idx < blocks_count-1; block_idx++)
	for (uint block_idx = 0; block_idx < blocks_count-1; block_idx++)
	{
		// cout << "[Block #" << block_idx << "]Processing frames in range [" << starting_frame << ".."<< (starting_frame + spline_block_size) << "]" << endl;
		//Real frame index: block_idx*spline_block_size + frame_idx
		process_spline_block(
                                video_input,
                                video_output,
                                starting_frame,
                                starting_frame + spline_block_size + 1, // Que tambien tenga en cuenta el primer frame del proximo bloque para que no haya saltos
                                interpol_frame_count
                            );
        starting_frame += spline_block_size;
    }

    //Process last block
    if(remaining_trailing_frames == 0)
    {
        process_spline_block(
                                video_input,
                                video_output,
                                starting_frame,
                                starting_frame + spline_block_size,
                                interpol_frame_count
                            );
    } else {
        process_spline_block(
                                video_input,
                                video_output,
                                starting_frame,
                                starting_frame + spline_block_size + 1, // Que tambien tenga en cuenta el primer frame del proximo bloque para que no haya saltos
                                interpol_frame_count
                            );
        starting_frame += spline_block_size;

        // Process last incomplete block( % trailing frames )
        if(remaining_trailing_frames >= 3)
        {
            // cout << "[Block #" << (blocks_count-1) << "]Processing frames in range [" << starting_frame << ".."<< video_input.frame_count << ")" << endl;
            process_spline_block(
                video_input,
                video_output,
                starting_frame,
                video_input.frame_count,
                interpol_frame_count
            );
        } else if(remaining_trailing_frames == 2){
            --starting_frame;
            process_spline_block(
                video_input,
                video_output,
                starting_frame,
                video_input.frame_count,
                interpol_frame_count,
                remaining_trailing_frames
            );
        } //Si remaining_trailing_frames == 1 no hace falta interpolar nada mas
    }
}

void SlowMotionEffect::process_spline_block(const Video& video_input, Video& video_output, uint starting_frame, const uint ending_frame, uint interpol_frame_count, uint remaining_trailing_frames)
{
	vector<double> x0;
	vector<double> y0;

	vector< vector<spline_polynomials_t> > splines_list;
	splines_list.resize(video_input.frame_height);

	for(uint i = 0; i < video_input.frame_height ; i++)
 	{

 		splines_list[i].resize(video_input.frame_width);

        for(uint j = 0 ; j < video_input.frame_width ; j++)
        {
            x0.clear();
            y0.clear();
            for(uint cur_frame = starting_frame; cur_frame < ending_frame ; cur_frame++) // Formo el polinomio, posicion x posicion
            {
                x0.push_back(cur_frame);
                y0.push_back(video_input.frames[cur_frame][i][j]);
            }
            // De esta forma, recorre y construye el polinomio en columna
            Interpolation::calculate_spline_polynomials(x0, y0, splines_list[i][j]);
        }

 	}

    // Para el caso de 2 o menos trailing frames en el ultimo bloque
    if(remaining_trailing_frames == 2) ++starting_frame;

 	// Iterate over all frames in current block
	for (uint cur_frame = starting_frame; cur_frame < ending_frame-1; cur_frame++)
	{


		// Add current input frame

		// Hack sucio para que no repita el primer frame al final y al princiṕio de cada bloque
		// aka: fixear esto
		// Interpolating frame 4.5
		// Adding original frame 5 <---------- se agrega aca primero !
		// [Block #1]Processing frames in range [5..10]
		// Adding original frame 5  <---------- se agrega de nuevo !
		// Interpolating frame 5.5
		// Adding original frame 6

		if((starting_frame == 0) || (cur_frame > starting_frame)){
			// cout << "Adding original frame " << cur_frame << endl;
			video_output.frames.push_back(video_input.frames[cur_frame]);
		}


		// Add new interpolated frames
		double spline_step = 1 / (double) (interpol_frame_count + 1);
		for (uint j = 0; j < interpol_frame_count; j++)
		{
			frame_t new_interpolated_frame;

			// cout << "Interpolating frame " << ((j+1)*spline_step + cur_frame) << endl;
			create_spline_frame_mix(video_output.frame_height,
			 video_output.frame_width,
			  new_interpolated_frame,
			  (j+1)*spline_step,
			  cur_frame,
			  splines_list);

			//Add the mixed frame
			video_output.frames.push_back(new_interpolated_frame);
		}
	}

	// Last frame
	// cout << "Adding original frame " << ending_frame-1 << endl;
	video_output.frames.push_back(video_input.frames[ending_frame - 1]);
}

void SlowMotionEffect::create_spline_frame_mix(uint frame_height, uint frame_width, frame_t& mixed_frame, double spline_step, int position_frame, vector< vector<spline_polynomials_t> >& splines_list)
{
	mixed_frame.resize(frame_height);

	for (uint i = 0; i < frame_height; i++)
	{
		mixed_frame[i].resize(frame_width);

		for (uint j = 0; j < frame_width; j++)
		{
			double interpol = Interpolation::eval_spline(
														position_frame + spline_step,
														 splines_list[i][j]);

			Interpolation::saturate_8bit(interpol);

			mixed_frame[i][j] = (pixel_t) interpol;
		}
	}
}
