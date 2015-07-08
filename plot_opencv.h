#ifndef PLOT_OPENCV_H
#define PLOT_OPENCV_H

#include <QWidget>
#include <QDebug>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <QApplication>

using namespace cv;

class Plot_opencv : public QWidget
{
    Q_OBJECT
public:
    explicit Plot_opencv(QWidget *parent = 0);

    QString plot_names;
    Mat plot_mat;
    int plot_counter, plot_width, plot_height, plot_step;
    double plot_now;
    std::vector<double> plot_prev;
    Scalar plot_colors[5];
    
signals:
    
public slots:
    void Plot(const String Plot_Name, const double Plot_Min, const double Plot_Max, std::vector<double> Plot_Data);
    void clean_plot(int StartX, int EndX, int StartY, int EndY, int Zero);
    
};

#endif // PLOT_OPENCV_H
