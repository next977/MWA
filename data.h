#ifndef DATA_H
#define DATA_H


#include <QObject>
#include <QString>
#include <QDebug>
#include "global.h"

#define READ_COMMAND_STATUS 0x11
#define READ_COMMAND_REGREAD 0x78
#define READ_COMMAND_REMOTE_START 0xA1
#define READ_COMMAND_REMOTE_DATA 0xA2
#define READ_COMMAND_REMOTE_CONFIRM 0xA3



#define REG_FREQ 0x0051
#define REG_PULSE_PERIOD 0x0054
#define REG_PULSE_WIDTH 0x0056
#define REG_TEMP_ALARM_VALUE 0x0011
#define REG_OVERPOWER_MINUS_ALARM_VALUE 0x001A
#define REG_OVERPOWER_PLUS_ALARM_VALUE 0x001B
#define REG_VSWR_ALARM_VALUE 0x001E
#define REG_TEMP_ALARM_TIME 0x0020
#define REG_OVERPOWER_ALARM_TIME 0x0021
#define REG_VSWR_ALARM_TIME 0x0022
#define REG_PLLUNLOCK_ALARM_TIME 0x0023
#define REG_LINKFAIL_ALARM_TIME 0x0024



class Data : public QObject
{
    Q_OBJECT
public:
    explicit Data(QObject *parent = nullptr);
            ~Data();
    enum Read_Data_SysStatus
    {
        DATA_READ_COMMAND = 7,
        DATA_HIGH_LENGTH = 8,
        DATA_LOW_LENGTH = 9,

        DATA_ALARM_STATUS = 10,
        DATA_OPERATION_MODE = 11,

        DATA_HIGH_TEMP = 12,
        DATA_LOW_TEMP = 13,

        DATA_HIGH_F_POWER_DBM= 14,
        DATA_LOW_F_POWER_DBM = 15,
        DATA_HIGH_R_POWER_DBM= 16,
        DATA_LOW_R_POWER_DBM = 17,

        DATA_HIGH_F_POWER = 18,
        DATA_LOW_F_POWER = 19,
        DATA_HIGH_R_POWER = 20,
        DATA_LOW_R_POWER = 21,

        DATA_F_HIGH_PLL_FREQ= 24,
        DATA_F_LOW_PLL_FREQ = 25,
        DATA_T_HIGH_PLL_FREQ= 26,
        DATA_T_LOW_PLL_FREQ = 27,

        DATA_F_HIGH_PLL_FREQ_MAX= 28,
        DATA_F_LOW_PLL_FREQ_MAX = 29,
        DATA_T_HIGH_PLL_FREQ_MAX= 30,
        DATA_T_LOW_PLL_FREQ_MAX = 31,

        DATA_HIGH_STABLE_POWER= 32,
        DATA_LOW_STABLE_POWER = 33,

        DATA_F_HIGH_PULSE_PERIOD = 34,
        DATA_F_LOW_PULSE_PERIOD = 35,
        DATA_T_HIGH_PULSE_PERIOD = 36,
        DATA_T_LOW_PULSE_PERIOD = 37,

        DATA_F_HIGH_PULSE_WIDTH = 38,
        DATA_F_LOW_PULSE_WIDTH = 39,
        DATA_T_HIGH_PULSE_WIDTH = 40,
        DATA_T_LOW_PULSE_WIDTH = 41,

        DATA_FIRMWARE_VERSION = 50

    };

    enum Read_Data_Register
    {
        REG_HIGH_SUBDATA_LENGTH = 10,
        REG_LOW_SUBDATA_LENGTH = 11,

        REG_HIGH_START_ADDRESS = 12,
        REG_LOW_START_ADDRESS = 13,

        REG_F_HIGH_FREQPULSE = 14,
        REG_F_LOW_FREQPULSE  = 15,
        REG_T_HIGH_FREQPULSE = 16,
        REG_T_LOW_FREQPULSE  = 18,

        REG_HIGH_ALARM       = 14,
        REG_LOW_ALARM        = 15

        /*
        REG_F_HIGH_FREQ= 14,
        REG_F_LOW_FREQ= 15,
        REG_T_HIGH_FREQ= 14,
        REG_T_LOW_FREQ= 15,
        REG_F_HIGH_P_PERIOD = 8,
        REG_F_LOW_P_PERIOD = 9,
        REG_T_HIGH_P_PERIOD = 8,
        REG_T_LOW_P_PERIOD = 9,
        REG_F_HIGH_P_WIDTH = 8,
        REG_F_LOW_P_WIDTH = 9,
        REG_T_HIGH_P_WIDTH = 8,
        REG_T_LOW_P_WIDTH = 9,

        REG_HIGH_TEMP_ALRAM_VALUE = 8,
        REG_LOW_TEMP_ALRAM_VALUE = 9,
        REG_HIGH_OVERPOWER_MINUS_ALRAM_VALUE = 8,
        REG_LOW_OVERPOWER_MINUS_ALRAM_VALUE = 9,
        REG_HIGH_OVERPOWER_PLUS_ALRAM_VALUE = 8,
        REG_LOW_OVERPOWER_PLUS_ALRAM_VALUE = 9,
        REG_HIGH_VSWR_ALRAM_VALUE = 8,
        REG_LOW_VSWR_ALRAM_VALUE = 9,

        REG_HIGH_TEMP_ALARM_TIME = 8,
        REG_LOW_TEMP_ALARM_TIME = 9,
        REG_HIGH_OVERPOWER_ALARM_TIME = 8,
        REG_LOW_OVERPOWER_ALARM_TIME = 9,
        REG_HIGH_VSWR_ALARM_TIME = 8,
        REG_LOW_VSWR_ALARM_TIME = 9,
        REG_HIGH_PLLUNLOCK_ALARM_TIME = 8,
        REG_LOW_PLLUNLOCK_ALARM_TIME = 9,
        REG_HIGH_LINKFAIL_ALARM_TIME = 8,
        REG_LOW_LINKFAIL_ALARM_TIME = 9
        */
    };

    struct head
    {
       QByteArray StartCode;
    };
    struct body
    {
       struct address
       {
            QByteArray Source;
            QByteArray Destination;

       };
       QByteArray Command;
       QByteArray SubDataLength;
       QByteArray Subdata;
       struct subdata
       {

           struct system_control
           {
                QByteArray Control_Flag;
                QByteArray Gain_Control_Value;
                QByteArray Power_Level;
                QByteArray Stable_Mode;
                QByteArray PLL_Frequency;
                QByteArray Pulse_Period_Value;
                QByteArray Pulse_Width_Value;
                QByteArray Reserved1;
                QByteArray Reserved2;
           };
           struct frequency_find
           {
                QByteArray Control_Flag;
                QByteArray Frequency_Stable_Mode;
                QByteArray Start_Frequency;
                QByteArray End_Frequency;
                QByteArray Search_Frequency_Step;
                QByteArray All_Zero;
                QByteArray Frequency_Stable_Operation_Interval;
                QByteArray Frequency_Stable_Range;
                QByteArray Frequency_Stable_Step;
           };

           system_control *System_Control = new system_control;
           frequency_find *Frequency_Find = new frequency_find;
       };
       //QByteArray SubData;
       QByteArray CRC;

       address *Address = new address;
       subdata *SubData = new subdata;

    };
    struct tail
    {
        QByteArray EndCode;
    };
    head *Head = new head;
    body *Body = new body;
    tail *Tail = new tail;

    bool PA_Enable;
    bool High_Temp;
    bool Over_Power;
    bool VSWR;
    bool PLL_Unlock;
    //double Temperature;
    //double Frequency;
    uint32 Temperature;
    uint32 Frequency;
    double Pulse_Width;
    double Pulse_Period;
    //double F_Power_W;
    uint32 F_Power_W;
    uint32 Power_Level;
    uint32 Firmware_Version;
    //double R_Power_W;
    uint32 R_Power_W;
    uint32 PLL_Freq_Max;
    QString Value;

    //double get_Temperature();
    //double get_Frequency();
    uint32 get_Temperature();
    uint32 get_Frequency();
    double get_Pulse_Width();
    double get_Pulse_Period();
    //double get_F_Power();
    uint32 get_F_Power();
    uint32 get_Power_Level();
    uint32 get_R_Power();
    //double get_R_Power();
    uint32 get_PLL_Freq_Max();
    bool get_PA_Enable();
    bool get_High_Temp();
    bool get_Over_Power();
    bool get_VSWR();
    bool get_PLL_Unlock();

    bool Frame_Parsing(char *Read_Data, int length );

signals:
    void sig_FD_Ready_Status_OK();
    void sig_FD_Ready_Status_NO();
    void sig_FD_Status_Confirm();
    void sig_Frame_Number(int);

public slots:

private:
    uint32 Value_Size;
    void Data_Setting();
    void Alarm_Status_Funtion(uint8 data);
    void Operation_Mode_Funtion(uint8 data);
};

#endif // DATA_H
