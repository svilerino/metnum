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

/*void SlowMotionEffect::spline_method_interpolation(interpolation_method_t interp_method, uint interpol_frame_count, Video& video_input, Video& video_output)
{
	std::vector< frame_t > xFrames(interpol_frame_count);
	std::vector< frame_t > yFrames(interpol_frame_count);
	for (uint cur_frame = 0; cur_frame < video_input.frame_count ; cur_frame++)
	{
		add_spline_frame_value(video_input.frames[cur_frame], cur_frame, xFrames[cur_frame], yFrames[cur_frame]);
		// Formo mi tabla de valores de los pixeles por cada frame
	}
	polinomio_conjunto funciones_spline;
	funciones_spline.resize(yFrames.size());
	for(int i = 0; i < splines_frames_list.size() ; i++)
	{
		funciones_spline[i].resize(yFrames.size());
	}
	for(uint splineIndex = 0; splineIndex < yFrames.size() ; splineIndex++)
	{
		funciones_spline[splineIndex] = create_spline_method(yFrame[splineIndex]); 
		// Se construye la funcion S por cada pixel que sera guardado en el conjunto polinomio.
	}
	// TODO: Una vez que tengo los S_{j} ( funcion spline ) de cada pixel, realizar la interpolacion
	// evaluado en un valor cualquiera ( el enunciado creo que pide el intermedio ).
	// Igualmente, me quedan bastantes dudas al respecto. 
}

void add_spline_frame_value(frame_t& current_frame, int indice_x0, frame_t& frame_x0, frame_t& frame_y0)
{
	uint frame_height = current_frame.size();
	uint frame_width = current_frame[1].size();
	for (uint i = 0; i < frame_height; i++)
	{
		for (uint j = 0; j < frame_width; j++)
		{
			frame_x0[i][j] = indice_x0;
			frame_y0[i][j] = current_frame[i][j];
		}	
	}
}

vector<polinomio_spline_t> SlowMotionEffect::spline_algorithm(vector<pixel_t> x0, vector<pixel_t> y0)
{
    int n = x0.size()-1;
    vector<pixel_t> a;
    a.insert(a.begin(), y0.begin(), y0.end());
    vector<pixel_t> b(n);
    vector<pixel_t> d(n);
    vector<pixel_t> h;
    for(int i = 0; i < n; ++i)
        h.push_back(x[i+1]-x[i]);
    vector<pixel_t> alpha;
    for(int i = 0; i < n; ++i)
        alpha.push_back( 3*(a[i+1]-a[i])/h[i] - 3*(a[i]-a[i-1])/h[i-1]  );
    vector<pixel_t> c(n+1);
    vector<pixel_t> l(n+1);
    vector<pixel_t> mu(n+1);
    vector<pixel_t> z(n+1);
    l[0] = 1;
    mu[0] = 0;
    z[0] = 0;
    for(int i = 1; i < n; ++i)
    {
        l[i] = 2 *(x0[i+1]-x0[i-1])-h[i-1]*mu[i-1];
        mu[i] = h[i]/l[i];
        z[i] = (alpha[i]-h[i-1]*z[i-1])/l[i];
    }
    l[n] = 1;
    z[n] = 0;
    c[n] = 0;
    for(int j = n-1; j >= 0; --j)
    {
        c[j] = z [j] - mu[j] * c[j+1];
        b[j] = (a[j+1]-a[j])/h[j]-h[j]*(c[j+1]+2*c[j])/3;
        d[j] = (c[j+1]-c[j])/3/h[j];
    }
    vector<TuplaSpline> splineRes(n);
    for(int j = 0;j < n; ++j)
    {
        splineRes[j].a = a[j];
        splineRes[j].b = b[j];
        splineRes[j].c = c[j];
        splineRes[j].d = d[j];
        splineRes[j].xj = xj[j];
        // Sj(x) = a_{j} + b_{j} [(x - x_{j})] + c_{j} [(x - x_{j})^2] + d_{j} [(x - x_{j})^3]
    }
    return splineRes;
}*/