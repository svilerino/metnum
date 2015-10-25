#include <io.hpp>

void read_video_from_args(std::istream& is, Video& video)
{
    // Read video metadata
    is >> video.frame_count;
    is >> video.frame_alto >> video.frame_ancho;
    is >> video.frame_rate;

    // Reserve memory for video dataframes
    video.frames.reserve(video.frame_count);

    // Read video dataframes
    for (uint cur_frame = 0; cur_frame < video.frame_count; cur_frame++)
    {
        // Reserve memory for <video.frame_alto> pixel rows
        frame_t frame;
        frame.reserve(video.frame_alto);

        for (uint i = 0; i < video.frame_alto; i++)
        {
            // Reserve memory for <video.frame_ancho> pixels per row
            frame[i].reserve(video.frame_ancho);

            for (uint j = 0; j < video.frame_ancho; j++)
            {
                is >> frame[i][j];
            }
        }        

        video.frames[cur_frame] = frame;
    }
}

void write_video_to_stream(std::ostream& os, Video& video)
{
    // Write video metadata
    os << video.frame_count << endl;
    os << video.frame_alto << " " << video.frame_ancho << endl;
    os << video.frame_rate << endl;

    // Write video data
    for (uint cur_frame = 0; cur_frame < video.frame_count; cur_frame++)
    {
        for (uint i = 0; i < video.frame_alto; i++)
        {
            for (uint j = 0; j < video.frame_ancho; j++)
            {
                os << video.frames[cur_frame][i][j] << " ";
            }
            os << endl;
        }        
    }
}