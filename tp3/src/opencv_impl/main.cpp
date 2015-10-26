#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <opencv_util.hpp>
#include <slowmotion.hpp>
#include <io.hpp>

int main( int argc, char** argv ) {
    assert(argc > 2);
    char* input_file = argv[1];
    char* output_file = argv[2];

    Video video_input;
    Video video_output;

    FileToVideo(input_file, video_input);

// ------
// -- Process video
// ------

    // typedef enum interpolation_method_t {NEAREST_NEIGHBOUR, LINEAR, SPLINE} interpolation_method_t;

    interpolation_method_t interp_method = NEAREST_NEIGHBOUR;
    uint interpol_frame_count = 5;

    SlowMotionEffect::interpolate(interp_method, interpol_frame_count, video_input, video_output);

    VideoToFile(output_file, video_output);

    return 0;
}
