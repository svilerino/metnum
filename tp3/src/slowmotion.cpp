#include <vector_operations.tpp>
#include <slowmotion.hpp>
#include <cmath>
#include <vector>
#include <iostream>

using namespace std;
// ---
// Los videos de entrada y salida son distintos asi puedo llamar iterativamente para medir tiempo promedio sin perder el input.
// ---

void SlowMotionEffect::interpolate(interpolation_method_t interp_method, uint interpol_frame_count, Video& video_input, Video& video_output)
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
		
		spline_method_interpolation(interp_method, interpol_frame_count, SPLINE_BLOCK_SIZE, video_input, video_output);

		uint new_frame_count = video_output.frames.size();
		video_output.frame_count = new_frame_count;
		//cout << video_output.frame_count << endl;

	}else{
		cerr << "Invalid interpolation mode: " << interp_method << endl;
		exit(-1);
	}

}

void SlowMotionEffect::progressive_linear_interpolation(interpolation_method_t interp_method, uint interpol_frame_count, Video& video_input, Video& video_output)
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
void SlowMotionEffect::spline_method_interpolation(interpolation_method_t interp_method, uint interpol_frame_count, uint spline_block_size, Video& video_input, Video& video_output)
{	

	assert(spline_block_size > 2);

	uint blocks_count = (uint) (video_input.frame_count / spline_block_size);
	uint remaining_trailing_frames = (uint) (video_input.frame_count % spline_block_size);

	cout << video_input.frame_count << " frames in video" << endl;
	cout << blocks_count << " blocks of " << spline_block_size << " frames" << endl;

	cout << "Trailing frames: " << remaining_trailing_frames << endl;
	if(remaining_trailing_frames > 0)
	{
		blocks_count++; // extra trailing frames block
		cout << "Blocks including trailing frames: " << blocks_count << endl;
	}

	// Process blocks
	uint starting_frame = 0;
	for (uint block_idx = 0; block_idx < blocks_count-1; block_idx++)
	{
		cout << "[Block #" << block_idx << "]Processing frames in range [" << starting_frame << ".."<< (starting_frame + spline_block_size) << ")" << endl;
		//Real frame index: block_idx*spline_block_size + frame_idx
		process_spline_block(
									video_input,
									video_output,
									starting_frame,
									starting_frame + spline_block_size,
									interpol_frame_count
									);
		starting_frame += spline_block_size;
	}

	// Process last incomplete block( % trailing frames )
	if(remaining_trailing_frames > 0)
	{
		cout << "[Block #" << (blocks_count-1) << "]Processing frames in range [" << starting_frame << ".."<< video_input.frame_count << ")" << endl;
	 	process_spline_block(
			video_input, 
			video_output,
			starting_frame, 
			video_input.frame_count,
			interpol_frame_count
		);
	}
}

void SlowMotionEffect::process_spline_block(Video& video_input, Video& video_output, const uint starting_frame, const uint ending_frame, uint interpol_frame_count)
{
	// Create (x, y) table for pixel (i, j)
	// Let interpolation_window_frames_count be (ending_frame - starting_frame)
	// Assume x in range [0..interpolation_window_frames_count)
	// For kth frame in [starting_frame, ending_frame):
	//     For each (i,j) pixel 
	//         y_k = pixel(i, j, k)

	vector<uint> x0;
	vector<pixel_t> y0;
	block_spline_polynomials_t pixel_polynomials;
	pixel_polynomials.resize(video_input.frame_height);
	for(uint i = 0; i < video_input.frame_height ; i++) 
 	{	
 		pixel_polynomials[i].resize(video_input.frame_width);
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
			create_spline_polynomial(x0, y0, pixel_polynomials[i][j]);
		}
 	}

 	// En pixel_polynomials[i][j][k] tengo los coeficientes de la ecuacion Sk del pixel i,j

 	// Iterate over all frames in current block
	for (uint cur_frame = starting_frame; cur_frame < ending_frame-1; cur_frame++)
	{
		// Add current input frame
		video_output.frames.push_back(video_input.frames[cur_frame]);

		// Add new interpolated frames
		double spline_step = 1 / (double) (interpol_frame_count + 1);
		for (uint j = 0; j < interpol_frame_count; j++)
		{
		//	cout << "Aca!" << endl;
			frame_t new_interpolated_frame;

			cout << "Interpolating frame " << ((j+1)*spline_step + cur_frame) << endl;
			create_spline_frame_mix(video_output.frame_height, video_output.frame_width, new_interpolated_frame, pixel_polynomials, (j+1)*spline_step, cur_frame);
			
			//Add the mixed frame
			video_output.frames.push_back(new_interpolated_frame);
		}
	}

	// Last frame
	video_output.frames.push_back(video_input.frames[ending_frame - 1]);
}

void SlowMotionEffect::create_spline_frame_mix(uint frame_height, uint frame_width, frame_t& mixed_frame, block_spline_polynomials_t& pixel_polynomials, double spline_step, int position_frame)
{
	mixed_frame.resize(frame_height);

	for (uint i = 0; i < frame_height; i++)
	{
		mixed_frame[i].resize(frame_width);

		for (uint j = 0; j < frame_width; j++)
		{
			mixed_frame[i][j] = evaluate_cubic_spline(pixel_polynomials[i][j][position_frame], position_frame + spline_step);
		}	
	}
}

double SlowMotionEffect::evaluate_cubic_spline(polinomio_spline_t& pixel_single_polynomial, double x)
{
	double coef_a = pixel_single_polynomial.a;
	double coef_b = pixel_single_polynomial.b;
	double coef_c = pixel_single_polynomial.c;
	double coef_d = pixel_single_polynomial.d;
	double xj = pixel_single_polynomial.xj;

	//cout << coef_a << " " << coef_b << " " << coef_c << " " << coef_d << endl;
	//cout << xj << endl;

	double result = (coef_a + coef_b * (x - xj) + coef_c * pow(x - xj, 2) + coef_d * pow(x - xj, 3));
	
	//cout << result << endl;
	
	assert(result >=0);

	return result;
    // Sj(x) = a_{j} + b_{j} [(x - x_{j})] + c_{j} [(x - x_{j})^2] + d_{j} [(x - x_{j})^3]
}

void SlowMotionEffect::create_spline_polynomial(const vector<uint>& x0, const vector<pixel_t>& y0, pixel_polynomial_t& pol_interpolate)
{
   int n = x0.size()-1;

   vector<double> a;
   a.insert(a.begin(), y0.begin(), y0.end()); // Step 1 : a_i = y_i

   // Step 2 : create b and d ( size n )
   vector<double> b(n);
   vector<double> d(n);
   
   // Step 3 : create h ( size n ) where h_i = x_{i+1} - x_{i}
   vector<double> h;
   for(int i = 0; i < n; ++i)
       h.push_back(x0[i+1]-x0[i]);

   // Step 4 : create alpha (size n) where alpha_i = (3/h_i) * (a_i+1 - a_i) - (3/h_i-1) * (a_i - a_i-1)
   vector<double> alpha;
   for(int i = 0; i < n; ++i)
       alpha.push_back( ( 3*(a[i+1]-a[i])/h[i] ) - ( 3*(a[i]-a[i-1])/h[i-1] ) );


   // Step 5 : create array c, l, mu and z ( size n+1 )
   vector<double> c(n+1);
   vector<double> l(n+1);
   vector<double> mu(n+1);
   vector<double> z(n+1);

   // Step 6 : Set l[0] = 1 , mu[0] = 0 and z[0] = 0
   l[0] = 1;
   mu[0] = 0;
   z[0] = 0;

   // Step 7 : (i = 1 .. n) 
   // 	Set l_i = 2 (x_i+1 - x_i-1) - (h[i-1]* mu[i-1])
   //	Set u_i = h_i / l_i
   // 	Set z_i = (alpha_i - (h_i-1 * z_i-1)) / l_i
   for(int i = 1; i < n; ++i)
   {
       l[i] = ( 2 *(x0[i+1]-x0[i-1]) ) - (h[i-1]*mu[i-1]);
       mu[i] = h[i]/l[i];
       z[i] = (alpha[i]-(h[i-1]*z[i-1]))/l[i];
   }

   // Step 8 : Set l[n] = 1, z[n] = 0 and c[n] = 0
   l[n] = 1;
   z[n] = 0;
   c[n] = 0;

   // Step 9 : (j = n-1 .. 0) 
   //	Set c_j = z_j - mu_j * c_j+1
   //	Set b_j = ((a[j+1] - a_j)/h_j) - ( h_j * (c_j+1 + 2 c_j) / 3)
   //	Set d_j = (c_j+1 - c_j) / ( 3 * h_j)
   for(int j = n-1; j >= 0; --j)
   {
       c[j] = z [j] - (mu[j] * c[j+1]);
       b[j] = ( (a[j+1]-a[j])/h[j] ) - (h[j]* (c[j+1] + 2*c[j])/ 3);
       d[j] = (c[j+1]-c[j])/(3*h[j]);
   }

   pol_interpolate.resize(n);
   for(int j = 0;j < n; ++j)
   {
       pol_interpolate[j].a =  a[j];
       pol_interpolate[j].b =  b[j];
       pol_interpolate[j].c =  c[j];
       pol_interpolate[j].d =  d[j];
       pol_interpolate[j].xj = x0[j];

       // Sj(x) = a_{j} + b_{j} [(x - x_{j})] + c_{j} [(x - x_{j})^2] + d_{j} [(x - x_{j})^3]
   } 
}

