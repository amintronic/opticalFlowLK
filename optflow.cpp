#include "optflow.h"
#include "ui_optflow.h"

//#define _GRAPH
#define _SERIAL

#define _DEV_SERIAL             "/dev/ttyUSB0"
#define _DATA_SCALE             100

OptFlow::OptFlow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OptFlow)
{
//    ui->setupUi(this);

    MAX_COUNT = 100; THRESH_REFRESH = 50;
    needToInit = false;
    nightMode = false;
    addRemovePt = false;
    plot_datas.resize(2);
    termcrit = TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03);
    subPixWinSize = Size(10,10); winSize = Size(31,31);
    cap = new capture_webcam(this);
    main_timer = new QTimer(this);
    connect(main_timer,SIGNAL(timeout()),this,SLOT(timerEvent()));

    serial_port = new QSerialPort(this);
#ifdef _SERIAL
    serial_port->close();
    serial_port->setPortName(_DEV_SERIAL);
    serial_port->setBaudRate(QSerialPort::Baud115200);
    serial_port->setParity(QSerialPort::NoParity);
    serial_port->setFlowControl(QSerialPort::NoFlowControl);
    serial_port->setDataBits(QSerialPort::Data8);
    serial_port->setStopBits(QSerialPort::OneStop);
    serial_port->open(QIODevice::ReadWrite);
    serial_port->setDataTerminalReady(0);
    serial_port->setRequestToSend(0);
    MPC.ready = 0;
    MPC.decode_stage = 0;
    MPC.sum = 0;
    MPC.integer_received_counter = 0;
    MPC.Num = 0;
    if( serial_port->isOpen())
    {
        qDebug("Serial Port Opened /dev/ttySAC0 .");
//        connect(serial_port, SIGNAL(readyRead()), this, SLOT(readData()));
    }
    else {
        qDebug("Serial Port NOT Opened!!!!!!!");
        qApp->exit();
    }
#endif

    main_timer->start(2);
}

void OptFlow::timerEvent()
{
    done = true;
    if(cap->first_image_ready)
    {
        cap->first_image_permission = false;
        cap->grayframe1.copyTo(colorframe);
    }
    else if(cap->second_image_ready)
    {
        cap->first_image_permission = true;
        cap->grayframe2.copyTo(colorframe);
    }
    else
        done = false;

    if(done)
    {
        cvtColor(colorframe, grayframe, CV_BGR2GRAY);
        OptFlow_LK();
    }
}

void OptFlow::OptFlow_LK()
{

    if( needToInit )
    {
        // automatic initialization
        goodFeaturesToTrack(grayframe, points[1], MAX_COUNT, 0.01, 10, Mat(), 3, 0, 0.04);
        cornerSubPix(grayframe, points[1], subPixWinSize, Size(-1,-1), termcrit);
    }
    else if( !points[0].empty() )
    {
        delta_x = 0;
        delta_y = 0;
        vector<uchar> status;
        vector<float> err;
        if(prevGray.empty())
            grayframe.copyTo(prevGray);
        calcOpticalFlowPyrLK(prevGray, grayframe, points[0], points[1], status, err, winSize,
                             3, termcrit, 0, 0.001);

        size_t i, k;
        for( i = k = 0; i < points[1].size(); i++ )
        {
            if( !status[i] )
                continue;

            points[1][k++] = points[1][i];
//            circle( colorframe, points[1][i], 3, Scalar(0,255,0), -1, 8);
            line(colorframe, points[0][i], points[1][i], Scalar(0,255,0), 1, 8, 0);
            Point2f p1 = points[0][i], p2 = points[1][i];
            delta_x += (p1.x - p2.x);
            delta_y += (p1.y - p2.y);
        }
        delta_x /= points[1].size();
        delta_y /= points[1].size();
        points[1].resize(k);
    }
#ifdef _GRAPH
    plot_datas[0] = delta_x;
    plot_datas[1] = delta_y;

    plot1.Plot("X(t)-Y(t) Velocity", -100, 100, plot_datas);
#endif

    qDebug() << (int)(delta_x * _DATA_SCALE) << ", " << (int)(delta_y * _DATA_SCALE);
    MPC_Fill_Data(2, (int)(delta_x * _DATA_SCALE), (int)(delta_y * _DATA_SCALE));

//    qDebug() << points[1].size();
    if(points[1].size() < THRESH_REFRESH)
        needToInit = true;
    else
        needToInit = false;

#ifdef _GRAPH
    imshow("LK Demo", colorframe);
#endif
    char c = (char)waitKey(1);

    std::swap(points[1], points[0]);
    cv::swap(prevGray, grayframe);
}

void OptFlow::MPC_Empty_Data()
{
    MPC.Num = 0;
    MPC.check_Sum = 0;
    MPC.data_send.clear();
    MPC.data_send.append(QByteArray::fromHex("FF"));
    MPC.data_send.append(QByteArray::fromHex("FF"));
}

void OptFlow::MPC_Fill_Data(uchar num , ... )
{
    va_list arguments;
    ch2int conv;
    va_start( arguments, num );

    MPC_Empty_Data();
    MPC.data_send.append(((num * 2) + 3));
    MPC.data_send.append(MPC.Num);
    for(int i = 0; i < num; i++)
    {
        conv.real=(uint)va_arg( arguments, int );
        MPC.check_Sum = MPC.check_Sum + (uchar)conv.byte[0] + (uchar)conv.byte[1];
        MPC.data_send.append(conv.byte[0]);
        MPC.data_send.append(conv.byte[1]);
    }
    MPC_Send_Data();

    va_end ( arguments );
}

void OptFlow::MPC_Send_Data()
{
    MPC.data_send.append((~(MPC.check_Sum) + 1));
//    qDebug() << MPC.data_send.toHex();
    serial_port->write(MPC.data_send);
    serial_port->flush();
}


OptFlow::~OptFlow()
{
//    delete ui;
}
