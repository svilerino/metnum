#include <slowmotion.hpp>
#include <cmath>

// ---
// Los videos de entrada y salida son distintos asi puedo llamar iterativamente para medir tiempo promedio sin perder el input.
// ---

void SlowMotionEffect::interpolate(interpolation_method_t interp_method, uint interpol_frame_count, Video& video_input, Video& video_output){
	
	//Calculate output video metadata
	uint new_frame_count = video_input.frame_count + (video_input.frame_count-1)*interpol_frame_count;

	//Initialize video output struct
	video_output.frame_count = new_frame_count;
	video_output.frame_alto = video_input.frame_alto;
	video_output.frame_ancho = video_input.frame_ancho;
	video_output.frame_rate = video_input.frame_rate;
	// NOTAR QUE NO COPIO LOS FRAMES ORIGINALES AL VIDEO DE SALIDA ACA. 
	// Como voy a  hacer inserts intermedios en un vector de la stl, primero resizeo al tamaño que va a tener a la salida y me ahorro mover todo en cada insercion de frame interpolado
	video_output.frames.reserve(new_frame_count);

	if (interp_method == NEAREST_NEIGHBOUR){
		
		nearest_neighbour_interpolation(interp_method, interpol_frame_count, video_input, video_output);

	} else if (interp_method == LINEAL){

	} else if (interp_method == SPLINE){

	}else{
		cerr << "Invalid interpolation mode: " << interp_method << endl;
	}

}

void SlowMotionEffect::nearest_neighbour_interpolation(interpolation_method_t interp_method, uint interpol_frame_count, Video& video_input, Video& video_output){
	for (uint cur_frame = 0; cur_frame < video_input.frame_count - 1; cur_frame++)
	{
		// Add current input frame
		video_output.frames.push_back(video_input.frames[cur_frame]);

		// Insert new <interpol_frame_count> interpolated frames

		// First half
		for (uint j = 0; j < (interpol_frame_count >> 1); j++)
		{
			// Copy current frame
			video_output.frames.push_back(video_input.frames[cur_frame]);
		}

		// Second half
		for (uint j = (interpol_frame_count >> 1) + 1; j < interpol_frame_count; j++)
		{
			// Copy next frame
			video_output.frames.push_back(video_input.frames[cur_frame + 1]);
		}
	}
}