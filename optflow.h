#ifndef OPTFLOW_H
#define OPTFLOW_H

#include <QMainWindow>
#include "QSerialPort"
#include "QSerialPortInfo"
#include "plot_opencv.h"
#include "capture_webcam.h"

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
    bool done;

    Mat prevGray, image;
    vector<Point2f> points[2];
    TermCriteria termcrit;
    Size subPixWinSize, winSize;
    int MAX_COUNT, THRESH_REFRESH;
    bool needToInit;
    bool nightMode;
    Point2f point;
    bool addRemovePt;

    QSerialPort *serial_port;
//    QByteArray coordinate_packet;

    Plot_opencv plot1;
    std::vector<double> plot_datas;
    double delta_x, delta_y;

    union _ch2int
    {
        int16_t real;
        char byte[2];
    };
    typedef union _ch2int  ch2int;
    typedef struct {

        int data_recieved[32];
        char decode_stage;
        char sum;
        char received_packet_length;
        char ready;
        ch2int conv;
        int integer_received_counter;
        int data_num;
        QByteArray data_send;
        uchar Num;
        uchar check_Sum;
    }_MPC;
    _MPC MPC;

    QByteArray serialPacketData;

    void MPC_decode(uchar data);
    void MPC_Empty_Data();
    void MPC_Fill_Data(uchar num , ... );
    void MPC_Send_Data();

    void OptFlow_LK();

private:
    Ui::OptFlow *ui;

public slots:
    void timerEvent();
};

#endif // OPTFLOW_H
