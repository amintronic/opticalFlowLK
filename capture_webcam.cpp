#include "capture_webcam.h"


#define _DEV_VIDEO              1
#define _VIDEO_FPS              30
#define _CAP_GRAY               0

#define _FRAME_WIDTH            320
#define _FRAME_HEIGTH           240

#define _SCALED_WIDTH           320
#define _SCALED_HEIGTH          240

capture_webcam::capture_webcam(QObject *parent) : QObject(parent)
{
    cap.open(_DEV_VIDEO);
    if( cap.isOpened() ) {
        cap.set(CV_CAP_PROP_FPS, _VIDEO_FPS);
        cap.set(CV_CAP_PROP_FRAME_WIDTH, _FRAME_WIDTH);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT, _FRAME_HEIGTH);
        cap >> frame;
        cv::resize(frame, frame_scaled, Size(_SCALED_WIDTH, _SCALED_HEIGTH));
#if _CAP_GRAY
        cvtColor(frame_scaled, grayframe1, CV_BGR2GRAY);
#else
        frame_scaled.copyTo(grayframe1);
#endif
        grayframe1.copyTo(grayframe2);

        first_image_ready = false;
        second_image_ready = false;
        first_image_permission = true;

        scale = 640 / _SCALED_WIDTH;

        main_timer = new QTimer(this);
        connect(main_timer,SIGNAL(timeout()),this,SLOT(timerEvent()));

        main_timer->start(2);
    }
    else
    {
        qDebug() << "Couldn't open the camera, exit.";
        exit(0);
    }
}

void capture_webcam::timerEvent()
{
    cap >> frame;
    cv::resize(frame, frame_scaled, Size(_SCALED_WIDTH, _SCALED_HEIGTH));
    if(first_image_permission)
    {
#if _CAP_GRAY
        cvtColor(frame_scaled, grayframe1, CV_BGR2GRAY);
#else
        frame_scaled.copyTo(grayframe1);
#endif
        first_image_ready = true;
        second_image_ready = false;
//        qDebug() << "first ready";
    }
    else
    {
#if _CAP_GRAY
        cvtColor(frame_scaled, grayframe2, CV_BGR2GRAY);
#else
        frame_scaled.copyTo(grayframe2);
#endif
        second_image_ready = true;
        first_image_ready = false;
//        qDebug() << "second ready";
    }
}

capture_webcam::~capture_webcam()
{
    delete main_timer;
}

