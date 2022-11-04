#ifndef FRAME_H
#define FRAME_H

#define FRAME_START 0x16
#define FRAME_SOURCEADDRESS_MASTER 0xA0
#define FRAME_DESTINATIONADDRESS_PA 0x21

#define FRAME_COMMAND_PA_STATUS 0x11
#define FRAME_COMMAND_PA_ENABLE 0x22
#define FRAME_COMMAND_PA_DISABLE 0x23
#define FRAME_COMMAND_PA_CONTROL 0x51
#define FRAME_COMMAND_FREQUENCY_SEARCH 0x90
#define FRAME_COMMAND_REG_READ 0x78
#define FRAME_COMMAND_REG_WRITE 0x79
#define FRAME_COMMAND_REMOTE_START 0xA1
#define FRAME_COMMAND_REMOTE_DATA 0xA2
#define FRAME_COMMAND_REMOTE_CONFIRM 0xA3



#define STARTADDRESS_DATALENGTH_ALARM 0x01
#define STARTADDRESS_DATALENGTH_VALUE 0x02

#define STARTADDRESS_FREQUENCY 0x51
#define STARTADDRESS_PULSE_PERIOD 0x54
#define STARTADDRESS_PULSE_WIDTH 0x56
#define STARTADDRESS_ALARMVALUE_OVER_TEMP 0x11
#define STARTADDRESS_ALARMVALUE_OVER_POWER_LOW 0x1A
#define STARTADDRESS_ALARMVALUE_OVER_POWER_HIGH 0x1B
#define STARTADDRESS_ALARMVALUE_VSWR 0x1E
#define STARTADDRESS_ALARMTIME_OVER_TEMP 0X20
#define STARTADDRESS_ALARMTIME_OVER_POWER 0X21
#define STARTADDRESS_ALARMTIME_VSWR 0X22
#define STARTADDRESS_ALARMTIME_PLL_UNLOCK 0X23
#define STARTADDRESS_ALARMTIME_LINK_FAIL 0X24

#define FRAME_END 0xF5


//#define FRAME_REMOTE_DOWNLOAD_DATA_SIZE 500
//#define FRAME_REMOTE_DOWNLOAD_SUBDATA_SIZE 502
//#define FRAME_REMOTE_DOWNLOAD_DATA_SIZE 514


#include <QByteArray>
#include <QObject>
#include <QDebug>
#include <QFileDialog>
#include <iostream>
#include <fstream>
#include "include.h"
#include "crccheck.h"

class frame : public QObject
{
    Q_OBJECT
public:
    enum Command_Status
    {
        COMMAND_PA_STATUS,
        COMMAND_PA_ENABLE,
        COMMAND_PA_DISABLE,
        COMMAND_PA_CONTROL,
        COMMAND_FREQ_SEARCH,
        COMMAND_REG_READ,
        COMMAND_REG_WRITE,
        COMMAND_REMOTE_START,
        COMMAND_REMOTE_DATA,
        COMMAND_REMOTE_CONFIRM
    };
    enum Address_Status
    {
        ADRESS_FREQUENCY,
        ADRESS_PULSE_PERIOD,
        ADRESS_PULSE_WIDTH,
        ADRESS_ALARMVALUE_OVER_TEMP,
        ADRESS_ALARMVALUE_OVER_POWER_LOW,
        ADRESS_ALARMVALUE_OVER_POWER_HIGH,
        ADRESS_ALARMVALUE_VSWR,
        ADRESS_ALARMTIME_OVER_TEMP,
        ADRESS_ALARMTIME_OVER_POWER,
        ADRESS_ALARMTIME_VSWR,
        ADRESS_ALARMTIME_PLL_UNLOCK,
        ADRESS_ALARMTIME_LINK_FAIL
    };
    enum Frequency_Mode_Status
    {
        MODE_NONE,
        MODE_FIND,
        MODE_STABLE
    };
    Command_Status command_status;
    Address_Status address_status;
    Frequency_Mode_Status freq_mode_status;
    explicit frame(QObject *parent = nullptr);
    ~frame();
    struct head
    {
       uint8 StartCode[4];
    };
    struct body
    {
       //<------Address-------->
       uint8 ADR_Source[1];
       uint8 ADR_Destination[1];
       //<--------------------->

       //<------Command-------->
       uint8 Command[1];
       //<--------------------->

       //<------Sub Data Length----->
       uint8 Sub_Length[2];
       //<-------------------------->

       //<---SubData System control--->
       uint8 Sub_SysControlFlag[1];
       uint8 Sub_GainControlValue[2];
       uint8 Sub_PowerLevel[2];
       uint8 Sub_StableMode[1];
       uint8 Sub_PLLFreq[4];
       uint8 Sub_PPeriodValue[4];
       uint8 Sub_PWidthValue[4];
       //<--------------------------->

       //<---SubData Frequency Find--->
       uint8 Sub_FindControlFlag[1];
       uint8 Sub_FreqStableMode[1];
       uint8 Sub_StartFreq[4];
       uint8 Sub_EndFreq[4];
       uint8 Sub_SearchFreqStep[4];
       uint8 Sub_All_Zero[5];
       uint8 Sub_FreqStableOperInterval[2];
       uint8 Sub_FreqStableRange[4];
       uint8 Sub_FreqStableStep[2];
       //<---------------------------->

       //<---SubData Register Read/Write--->
       uint8 Sub_Reg_Data_Length[2];
       uint8 Sub_Reg_Start_Address[2];
       uint8 Sub_Reg_Data[4];
       //<---------------------------->


       //<---SubData DownLoad --------->
       //uint32 Sub_DL_Offset;
       uint8 Sub_DL_Frame_Number[2];
       //uint8 Sub_DL_Data[500];   //30 // 250 ,
       uint8 *Sub_DL_Data;
       uint8 Sub_DL_Total_Length[4];
       uint8 Sub_DL_Total_CRC[2];
       //<---------------------------->



       uint8 Sub_Value[502]; //32//18 Sub_DL_Frame_Number+Sub_DL_Data
       //uint8 *Sub_Value;
       //<-----------CRC------------->
       uint8 CRC[2];
       //<--------------------------->
    };
    struct tail
    {
        uint8 EndCode[1];
    };
    head *Head = new head;
    body *Body = new body;
    tail *Tail = new tail;

    uint32 set_freq;
    double set_pulse_width;
    double set_pulse_period;
    uint32 set_power_value;
    bool set_freq_stable_mode;

    uint32 set_start_freq;
    uint32 set_end_freq;
    //double set_freq_step;
    uint32 set_freq_step;
    uint32 set_freq_stable_range;
    uint32 set_freq_stable_interval;
    uint32 set_freq_stable_step;
    uint32 set_address_data;
    uint32 Sub_DL_Data_Index;
    uint8 *Frame_combine_array;
    uint32 Frame_Total_Size;
    uint32 Frame_SubData_Size;
    uint32 Frame_RD_Size; //Frame_RemoteDownload_Size
    uint32 Frame_Basic_Size;
signals:

public slots:
    uint8 *Frame_Combine();
private:
    CRCCheck *crccheck;
    void Head_Function();
    void Body_Function();
    void Address_Function();
    void Command_Function();
    void SubDataLength_Function();
    void SubData_Function();
    void Reg_ReadWrite_Function();
    void CRC_Function();
    void Tail_Function();
    void SysControl_Function();
    void FreqFind_Function();
    void Download_Data_Function();
    void Download_File_Function();
    void Download_Confirm_Function();
    uint8 SysControlFlag_Funtion();
    uint8 FindControlFlag_Funtion(Frequency_Mode_Status state);

    void Setting();

    uint32 File_Length;

    uint8 crc_check[507]; //12+26 =38
    int crc_count;



    uint8 *Sub_DL_Data_Buf;

    uint32 Sub_DL_fseek_Index;
};

#endif // FRAME_H
