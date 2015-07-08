#include "plot_opencv.h"

Plot_opencv::Plot_opencv(QWidget *parent) :
    QWidget(parent)
{
    /* plot_init */
    plot_width = 1000, plot_height = 300; plot_step = 3;

    plot_counter = 0; plot_now = 0;
    plot_mat.create(plot_height, plot_width, CV_8UC3);

    plot_prev.push_back(0);
    plot_prev.push_back(0);
    plot_prev.push_back(0);
    plot_prev.push_back(0);
    plot_prev.push_back(0);

    plot_colors[0] = Scalar(0,0,255);        //red
    plot_colors[1] = Scalar(255,0,0);        //blue
    plot_colors[2] = Scalar(0,255,0);        //green
    plot_colors[3] = Scalar(255,255,0);      //indigo
    plot_colors[4] = Scalar(255,0,255);      //violet

    clean_plot(0, plot_width, 0, plot_height, 0);

//    plot_names = Plot_Name;
//    imshow(plot_names.toLatin1().data(), plot_mat);
}

void Plot_opencv::clean_plot(int StartX, int EndX, int StartY, int EndY, int Zero)
{
    for(int x = StartX; x < EndX; x++)
        for(int y = StartY; y < EndY; y++)
            for(int z = 0; z < 3; z++)
                plot_mat.at<Vec3b>(y, x).val[z] = 255;
    for(int x = StartX; x < EndX; x++)
    {
        if(x % 50 == 0)
            for(int y = StartY; y < EndY; y++)
                for(int z = 0; z < 3; z++)
                    plot_mat.at<Vec3b>(y, x).val[z] = 180;
        else if(x % 10 == 0)
            for(int y = StartY; y < EndY; y++)
                for(int z = 0; z < 3; z++)
                    plot_mat.at<Vec3b>(y, x).val[z] = 230;
    }
    for(int x = StartX; x < EndX; x++)
        for(int y = StartY; y < EndY; y+=10)
        {
            if(y % 50 == 0)
                for(int z = 0; z < 3; z++)
                    plot_mat.at<Vec3b>(y, x).val[z] = 180;
            else
                for(int z = 0; z < 3; z++)
                    plot_mat.at<Vec3b>(y, x).val[z] = 230;
        }
    for(int x = StartX; x < EndX; x++)
        for(int z = 0; z < 3; z++)
        {
            plot_mat.at<Vec3b>(plot_height - Zero - 1, x).val[z] = 0;
            plot_mat.at<Vec3b>(plot_height - Zero - 2, x).val[z] = 0;
        }
}

void Plot_opencv::Plot(const String Plot_Name, const double Plot_Min, const double Plot_Max, std::vector<double> Plot_Data)
{
    bool plot_flag;
    for(size_t i = 0; i < Plot_Data.size(); i++)
        if(Plot_Data[i] > Plot_Max)
            plot_flag = false;
        else
            plot_flag = true;
    if(plot_flag == true)
    {
        if(plot_counter >= plot_step)   /* Clean the Plot */
            clean_plot(plot_counter - plot_step + 1, plot_counter + 1, 0, plot_height, (plot_height / (Plot_Max - Plot_Min)) * (-Plot_Min));
        else
            clean_plot(plot_counter - plot_step, plot_counter + 1, 0, plot_height, (plot_height / (Plot_Max - Plot_Min)) * (-Plot_Min));

        for(size_t i = 0; i < Plot_Data.size(); i++)
        {
            plot_now = ((plot_height / (Plot_Max - Plot_Min)) * (Plot_Data[i] - Plot_Min));
            line(plot_mat, Point(plot_counter - plot_step, plot_height - plot_prev[i]),
                 Point(plot_counter, plot_height - plot_now), plot_colors[i]);

            plot_prev[i] = plot_now;
        }

        plot_names = QString::fromStdString(Plot_Name);
        imshow(plot_names.toLatin1().data(), plot_mat);
        plot_counter += plot_step;
        if(plot_counter >= plot_width)
            plot_counter = 0;
    }
}
