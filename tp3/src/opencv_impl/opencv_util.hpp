#include <getopt.h>
#include <highgui.h>
#include <cv.h>
#include <io.hpp>

typedef struct CvVideoWriter CvVideoWriter;

void FileToVideo(const char *archivo_entrada, Video& video);
void VideoToFile (const char *archivo_salida, Video& video_output);