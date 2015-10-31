#include <getopt.h>
#include <highgui.h>
#include <cv.h>
#include <types.hpp>
#include <opencv2/imgproc/imgproc.hpp>

typedef struct CvVideoWriter CvVideoWriter;

void FileToVideo(const char *archivo_entrada, Video& video, const string carpeta_frames);
void VideoToFile (const char *archivo_salida, Video& video_output, const string carpeta_frames);