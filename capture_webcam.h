#ifndef CAPTURE_WEBCAM_H
#define CAPTURE_WEBCAM_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class capture_webcam : public QObject
{
    Q_OBJECT
public:
    explicit capture_webcam(QObject *parent = 0);
    ~capture_webcam();
    Mat frame, frame_scaled, grayframe1, grayframe2;
    bool first_image_ready, second_image_ready, first_image_permission;
    VideoCapture cap;
    double scale;

    QTimer *main_timer;

signals:

public slots:
    void timerEvent();
};

#endif // CAPTURE_WEBCAM_H
