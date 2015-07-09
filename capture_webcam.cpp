#include "capture_webcam.h"



capture_webcam::capture_webcam(QObject *parent) : QObject(parent)
{



    cap.open(_DEV_VIDEO);
    if( cap.isOpened() )
    {

        cap.set(CV_CAP_PROP_FPS, _VIDEO_FPS);
        cap.set(CV_CAP_PROP_FRAME_WIDTH, _FRAME_WIDTH);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT, _FRAME_HEIGTH);
        cap >> frame;

        frame.copyTo(first_frame);

        frame.copyTo(second_frame);

        first_image_ready = false;
        second_image_ready = false;
        first_image_permission = true;

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

}

capture_webcam::~capture_webcam()
{
    delete main_timer;
}

