#-------------------------------------------------
#
# Project created by QtCreator 2015-07-08T13:25:02
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = optFlow_LK
TEMPLATE = app


SOURCES += main.cpp\
        optflow.cpp \
    capture_webcam.cpp \
    plot_opencv.cpp \
    quad_board.cpp

HEADERS  += optflow.h \
    capture_webcam.h \
    plot_opencv.h \
    quad_board.h

FORMS    += optflow.ui

LIBS += -lopencv_core -lopencv_ml -lopencv_highgui -lopencv_imgproc -lopencv_ts -lopencv_gpu -lopencv_video -lopencv_calib3d \
    -lopencv_legacy -lopencv_flann -lopencv_objdetect -lopencv_features2d -lopencv_contrib -lopencv_photo -lopencv_stitching \
    -lopencv_videostab -lopencv_superres -lopencv_ocl
