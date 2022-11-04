#ifndef UART_H
#define UART_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #include <windows.h>
#else

#endif

#define SEND_DATA_SIZE_PA 12
#define SEND_DATA_SIZE_REG_READ 16
#define SEND_DATA_SIZE_REG_WRITE 20

#define SEND_DATA_SIZE_CONTFIND 30

#define SEND_DATA_SIZE_REMOTE_DATA 514//44 //300


//#include <QSerialPort>

#include <include.h>
#include <QWidget>
#include <QComboBox>

#include <QDebug>
#include <QTimer>
#include <QMutex>
#include <QSocketNotifier>
#include "frame.h"
#include "gpiotools.h"

class mainsystem;
class uart : public QWidget
{
    Q_OBJECT
public:
    explicit uart(QWidget *parent = 0);
    ~uart();
#ifdef __WINDOWS_MICROWAVE__
    void Scan_Serial_Port(QComboBox *combobox);
#endif
    bool serial_connect(QString Comport, QString Baudrate);
    void Serial_Disconnect();
    void Frame_Sysctrl_Set(uint32 *freq, double *pulse_width, double *pulse_period, uint32 *power_value);
    void Frame_FreqFind_Set(bool *freq_stable_mode,uint32 *start_freq, uint32 *end_freq, uint32 *freq_step,
                            uint32 *freq_stable_range, uint32 *freq_stable_interval , uint32 *freq_stable_step);
    void Frame_Reg_Set(uint32 *freq, uint32 *pulse_width, uint32 *pulse_period, uint32 *alarm_data);
    void Frame_Command(frame::Command_Status);
    void Frame_Freq_Mode(frame::Frequency_Mode_Status);
    void Frame_Reg_Address(frame::Address_Status);
    void Frame_Number(uint32 frame_number);
    char read_Data[256];
    int read_Data_length;
    bool DownLoad_Flag;
    QTimer *Uart_Timer;

private:
#ifndef __WINDOWS_MICROWAVE__
    //QSerialPort *SerialPort;
#endif


    QMutex *m_mutex;
    frame *Frame_Class;
    QSocketNotifier *notRsRead;
    void Event_connect();
    void Setting();
    void uart_Connect();
    void uart_Disconnect();

#ifndef __WINDOWS_MICROWAVE__
    QString TCHARToString(const TCHAR* ptsz);
#endif
    uint32 Send_Data_size;
    char combine[256];
    uint32 combine_Index;

signals:
    void sig_data_frameparsing();
public slots:
    void slot_Frame_write();
private slots:
    void slot_Read_Event();

};

#endif // UART_H
