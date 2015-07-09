#ifndef QUAD_BOARD_H
#define QUAD_BOARD_H


#ifndef QUAD_BOARD
#define QUAD_BOARD
#include <QObject>
#include "QSerialPort"
#include "QSerialPortInfo"


class Quad_Board : public QObject
{
    Q_OBJECT
public:
    explicit Quad_Board(QObject *parent = 0);
    ~Quad_Board();

    void decode(uchar data);
    void Empty_Data();
    void Fill_Data(uchar num , ... );
    void Send_Data();


    union _ch2int
        {
            int16_t real;
            char byte[2];
        };


    typedef union _ch2int  ch2int;

private:

    bool ready;
    char decode_stage;
    char sum;
    char received_packet_length;
    int integer_received_counter;
    int data_num;
    int data_recieved[32];
    QByteArray data_send;
    uchar Num;
    uchar check_Sum;
    QSerialPort *serial_port;
    ch2int conv;



};









#endif // QUAD_BOARD



#endif // QUAD_BOARD_H
