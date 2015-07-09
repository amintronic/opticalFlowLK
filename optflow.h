#ifndef OPTFLOW_H
#define OPTFLOW_H

#include <QMainWindow>
#include "QSerialPort"
#include "QSerialPortInfo"
#include "plot_opencv.h"
#include "capture_webcam.h"

//#define _GRAPH

#define _DATA_SCALE             100

#define MAX_COUNT               100
#define THRESH_REFRESH          50

#define winSize                 Size(31,31)
#define subPixWinSize           Size(10,10)

using namespace cv;
using namespace std;

namespace Ui {
class OptFlow;
}

class OptFlow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OptFlow(QWidget *parent = 0);
    ~OptFlow();

    Mat colorframe, grayframe;
    QTimer *main_timer;
    capture_webcam *cap;


    Mat prevGray, image;
    vector<Point2f> points[2];
    TermCriteria termcrit;
    Plot_opencv plot1;
    std::vector<double> plot_datas;
    Point2f point;


    bool needToInit;
    bool done;

    size_t Features_counter;//count Features state true

    double delta_x, delta_y;

    void OptFlow_LK();

private:
    Ui::OptFlow *ui;

public slots:
    void timerEvent();
};

#endif // OPTFLOW_H
