#ifndef CAPTURE_WEBCAM_H
#define CAPTURE_WEBCAM_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <opencv2/opencv.hpp>

#define _DEV_VIDEO              1
#define _VIDEO_FPS              60

#define _FRAME_WIDTH            320
#define _FRAME_HEIGTH           240

using namespace cv;
using namespace std;

class capture_webcam : public QObject
{
    Q_OBJECT
public:
    explicit capture_webcam(QObject *parent = 0);
    ~capture_webcam();
    Mat frame, first_frame, second_frame;
    bool first_image_ready, second_image_ready, first_image_permission;
    VideoCapture cap;
    double scale;

    QTimer *main_timer;

signals:

public slots:
    void timerEvent();
};

#endif // CAPTURE_WEBCAM_H
