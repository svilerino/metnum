#include <io.hpp>
#include <limits>
#include <cassert>

void read_video_from_args(std::istream& is, Video& video)
{
    // Read video metadata
    is >> video.frame_count;
    assert(video.frame_count > 0);
    
    is >> video.frame_alto;
    is.ignore(std::numeric_limits<std::streamsize>::max(),',');
    is >> video.frame_ancho;
    
    is >> video.frame_rate;

    // Reserve memory for video dataframes
    video.frames.resize(video.frame_count);

    // Read video dataframes
    for (uint cur_frame = 0; cur_frame < video.frame_count; cur_frame++)
    {
        // Reserve memory for <video.frame_alto> pixel rows
        frame_t frame;
        frame.resize(video.frame_alto);

        for (uint i = 0; i < video.frame_alto; i++)
        {
            // Reserve memory for <video.frame_ancho> pixels per row
            frame[i].resize(video.frame_ancho);

            for (uint j = 0; j < video.frame_ancho; j++)
            {
                is >> frame[i][j];
                if(is.peek() == ',') is.ignore();
            }
        }

        video.frames[cur_frame] = frame;
    }
}

void write_video_to_stream(std::ostream& os, Video& video)
{
    // Write video metadata
    os << video.frame_count << endl;
    os << video.frame_alto << "," << video.frame_ancho << endl;
    os << video.frame_rate << endl;

    // Write video data
    for (uint cur_frame = 0; cur_frame < video.frame_count; cur_frame++)
    {
        for (uint i = 0; i < video.frame_alto; i++)
        {
            for (uint j = 0; j < video.frame_ancho; j++)
            {
                os << video.frames[cur_frame][i][j];
                if(j < video.frame_ancho - 1) os << ",";
            }
            os << endl;
        }        
    }
}