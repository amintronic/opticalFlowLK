
#include "optflow.h"
#include <QApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OptFlow w;

    QThread mtr;
    w.cap->moveToThread(&mtr);
    mtr.start(QThread::HighestPriority);


    return a.exec();
}
