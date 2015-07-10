#include "quad_board.h"

#define _DEV_SERIAL             "/dev/ttyUSB0"
#define _DATA_SCALE             100

Quad_Board::Quad_Board(QObject *parent) : QObject(parent)
{

    serial_port = new QSerialPort(this);
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
    ready = false;
    decode_stage = 0;
    sum = 0;
    integer_received_counter = 0;
    Num = 0;
    if( serial_port->isOpen())
    {
        qDebug("Serial Port Opened  .");
    }
    else
    {
        qDebug("Serial Port NOT Opened!!!!!!!");

    }

}

Quad_Board::~Quad_Board()
{

}

void Quad_Board::decode(uchar data)
{


}

void Quad_Board::Empty_Data()
{
    Num = 0;
    check_Sum = 0;
    data_send.clear();
    data_send.append(QByteArray::fromHex("FF"));
    data_send.append(QByteArray::fromHex("FF"));

}

void Quad_Board::Fill_Data(uchar num,...)
{
    va_list arguments;
    va_start( arguments, num );

    Empty_Data();
    data_send.append(((num * 2) + 3));
    data_send.append(Num);
    for(int i = 0; i < num; i++)
    {
        conv.real=(uint)va_arg( arguments, int );
        check_Sum = check_Sum + (uchar)conv.byte[0] + (uchar)conv.byte[1];
        data_send.append(conv.byte[0]);
        data_send.append(conv.byte[1]);
    }
    Send_Data();

    va_end ( arguments );

}

void Quad_Board::Send_Data()
{
    data_send.append((~(check_Sum) + 1));
    serial_port->write(data_send);
    serial_port->flush();

}



