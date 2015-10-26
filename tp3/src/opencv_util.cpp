#include <opencv_util.hpp>

static struct CvVideoWriter *abrir_writer(const char *archivo_salida, int fps, CvSize size) {
    struct CvVideoWriter *dstVid = NULL;
    dstVid = cvCreateVideoWriter(archivo_salida, CV_FOURCC('M','J','P','G'), fps, size, 1);
    if(dstVid == NULL) {
        fprintf(stderr, "Invalid dstVid\n");
        exit(EXIT_FAILURE);
    }

    return dstVid;
}

static CvCapture *abrir_video(const char *archivo_entrada) {
    CvCapture *srcVid = NULL;
    srcVid = cvCaptureFromFile(archivo_entrada);
    if( srcVid == NULL) {
        /* Esto no está documentado. No debería pasar nunca. */
        fprintf(stderr, "Invalid srcVid\n%s \n", archivo_entrada);
        exit(EXIT_FAILURE);
    }

    return srcVid;
}


void FileToVideo(const char *archivo_entrada, Video& video)
{
    CvCapture *srcVid = abrir_video(archivo_entrada);

    CvSize dst_size;
    dst_size.width = cvGetCaptureProperty(srcVid,CV_CAP_PROP_FRAME_WIDTH);
    dst_size.height = cvGetCaptureProperty(srcVid,CV_CAP_PROP_FRAME_HEIGHT);
    int nchannels = 3;
    double fps = cvGetCaptureProperty(srcVid,CV_CAP_PROP_FPS);

    video.frame_height = dst_size.height;
    video.frame_width = nchannels * dst_size.width;
    video.frame_rate = fps;
    uint frame_count = 0;
    while( true ) {
        
        IplImage *cvFrame = cvQueryFrame(srcVid);

        if (cvFrame == NULL) {
            break;
        }

        unsigned char *src = (unsigned char *) cvFrame->imageData;
        uint idx_src=0;

        // Reserve memory for <video.frame_height> pixel rows
        frame_t frame;
        frame.resize(video.frame_height);

        for (uint i = 0; i < video.frame_height; i++)
        {
            // Reserve memory for <video.frame_width> pixels per row
            frame[i].resize(video.frame_width);

            for (uint j = 0; j < video.frame_width; j++)
            {
                frame[i][j] = src[idx_src++];
            }
        }

        video.frames.push_back(frame);

        frame_count++;
    }

    video.frame_count = frame_count;

    cvReleaseCapture(&srcVid);
}


void VideoToFile (const char *archivo_salida, Video& video_output) 
{
// ------
// -- Write back to video
// ------

    uint nchannels = 3;
    CvSize dst_size;
    dst_size.width = video_output.frame_width/nchannels;
    dst_size.height = video_output.frame_height;

    typedef struct CvVideoWriter CvVideoWriter;
    CvVideoWriter* dstVid = NULL;
    dstVid = abrir_writer(archivo_salida, video_output.frame_rate, dst_size);

    /* Armo la imagen destino. */
    IplImage *dst = cvCreateImage (dst_size, IPL_DEPTH_8U, nchannels);
    if (dst == NULL) {
        fprintf(stderr, "Error armando la imagen destino\n");
        exit(EXIT_FAILURE);
    }

    unsigned char *dst_ptr = (unsigned char *) dst->imageData;

    for (uint cur_frame = 0; cur_frame < video_output.frame_count; cur_frame++)
    {
        uint idx_dst=0;
        for (uint i = 0; i < video_output.frame_height; i++)
        {
            for (uint j = 0; j < video_output.frame_width; j++)
            {                
                dst_ptr[idx_dst] = video_output.frames[cur_frame][i][j];
                idx_dst++;
            }
        }    

        cvWriteFrame(dstVid, dst);
    }

    cvReleaseImage(&dst);
    cvReleaseVideoWriter(&dstVid);

}