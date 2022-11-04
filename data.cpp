#include "data.h"
Data::Data(QObject *parent) : QObject(parent)
{
    Data_Setting();
}
Data::~Data()
{

}
void Data::Data_Setting()
{
    PA_Enable = true;
    High_Temp = true;
    Over_Power = true;
    VSWR = true;
    PLL_Unlock = true;

    Temperature = 0;
    Frequency = 0;
    Pulse_Width = 0;
    Pulse_Period = 0;
    F_Power_W = 0;
    Power_Level = 0;
    R_Power_W = 0;
    Firmware_Version = 0;

}

bool Data::Frame_Parsing(char *Read_Data, int length )
{
    int32 Frame_Length = length;
    uint16 SubData_Length = Read_Data[DATA_LOW_LENGTH] | (Read_Data[DATA_HIGH_LENGTH] << 8);
    uint8 Read_Command = Read_Data[DATA_READ_COMMAND];
    uint32 SysStatus_Length = 13 + SubData_Length; //Frame_Length - SubData_Length = 13

    //uint32 Reg_Length = 21;
    //uint32 Reg_Alarm_Time_Length = 19;
    uint16 Reg_Start_Address;

#ifdef __MICROWAVE_TEST__
#else
    qDebug() <<"Data read_data = "<<Read_Data;
#endif
    if(Frame_Length == SysStatus_Length )
    {
        if(Read_Command == READ_COMMAND_STATUS)
        {
#ifdef __MICROWAVE_TEST__
#else
         qDebug() << "Frame_Parsing Read_Data Ok" ;
         qDebug() <<"Data read_data = "<<Read_Data;
#endif
         Temperature = (double)(Read_Data[DATA_LOW_TEMP] | (Read_Data[DATA_HIGH_TEMP] << 8))/256;
         F_Power_W = (double)(Read_Data[DATA_LOW_F_POWER] | (Read_Data[DATA_HIGH_F_POWER] << 8))/100;
         R_Power_W = (double)(Read_Data[DATA_LOW_R_POWER] | (Read_Data[DATA_HIGH_R_POWER] << 8))/100;

         Frequency = (double)((Read_Data[DATA_T_LOW_PLL_FREQ] | (Read_Data[DATA_T_HIGH_PLL_FREQ] << 8) )
                              | (Read_Data[DATA_F_LOW_PLL_FREQ] | (Read_Data[DATA_F_HIGH_PLL_FREQ] << 8))<<16)/1000;

         PLL_Freq_Max = (double)((Read_Data[DATA_T_LOW_PLL_FREQ_MAX] | (Read_Data[DATA_T_HIGH_PLL_FREQ_MAX] << 8) )
                              | (Read_Data[DATA_F_LOW_PLL_FREQ_MAX] | (Read_Data[DATA_F_HIGH_PLL_FREQ_MAX] << 8))<<16)/1000;

         Power_Level = Read_Data[DATA_LOW_STABLE_POWER] | (Read_Data[DATA_HIGH_STABLE_POWER] << 8);

         Pulse_Period = (double)((Read_Data[DATA_T_LOW_PULSE_PERIOD]| (Read_Data[DATA_T_HIGH_PULSE_PERIOD] << 8) )
                              | (Read_Data[DATA_F_LOW_PULSE_PERIOD] | (Read_Data[DATA_F_HIGH_PULSE_PERIOD] << 8))<<16)/1000;

         Pulse_Width = (double)((Read_Data[DATA_T_LOW_PULSE_WIDTH] | (Read_Data[DATA_T_HIGH_PULSE_WIDTH] << 8) )
                              | (Read_Data[DATA_F_LOW_PULSE_WIDTH] | (Read_Data[DATA_F_HIGH_PULSE_WIDTH] << 8))<<16)/10000;


         Firmware_Version = Read_Data[DATA_FIRMWARE_VERSION];
         Alarm_Status_Funtion(Read_Data[DATA_ALARM_STATUS]);
         Operation_Mode_Funtion(Read_Data[DATA_OPERATION_MODE]);
        // qDebug("Firmware Version = %d",Firmware_Version);
#ifdef __MICROWAVE_TEST__
#else
        qDebug()<<"Alarm_Status = " <<Read_Data[DATA_ALARM_STATUS];
        qDebug()<<"Temperature = " <<Temperature;
        qDebug()<<"Forward_Power_W = " <<F_Power_W;
        qDebug()<<"Reverse_Power_W = " <<R_Power_W;

        qDebug()<<"Frequency = " <<Frequency;
        qDebug()<<"Power_Control_W = " << Power_Level;
        qDebug()<<"Pulse_Period = " <<Pulse_Period;
        qDebug()<<"Pulse_Width = " <<Pulse_Width;

        qDebug() <<"PA_Enable = " <<PA_Enable;
        qDebug() <<"High_Temp = " <<High_Temp;
        qDebug() <<"Over_Power = " <<Over_Power;
        qDebug() <<"VSWR = " <<VSWR;
        qDebug() <<"PLL_Unlock" <<PLL_Unlock;
        qDebug() <<"Firmware Version" <<Firmware_Version;

#endif

        }
        else if (Read_Command == READ_COMMAND_REGREAD)
        {
            uint32 Reg_Freq, Reg_Pulse_Period , Reg_Pulse_Width;
            uint16 Reg_Temp_Alarm_Value, Reg_OP_Minus_Alarm_Value, Reg_OP_Plus_Alarm_Value, Reg_VSWR_Alarm_Value;
            uint16 Reg_Temp_Alarm_Time, Reg_OP_Alarm_Time, Reg_VSWR_Alarm_Time, Reg_PLLUnlock_Alarm_Time, Reg_LinkFail_Alarm_Time;

            Reg_Start_Address = (Read_Data[REG_LOW_START_ADDRESS] | (Read_Data[REG_HIGH_START_ADDRESS] << 8));
            switch(Reg_Start_Address)
            {
                case REG_FREQ:
                        qDebug() <<"Reg_Freq " ;
                        qDebug()<< ((Read_Data[REG_T_LOW_FREQPULSE]  | (Read_Data[REG_T_HIGH_FREQPULSE] << 8) )
                                 | (Read_Data[REG_F_LOW_FREQPULSE] | (Read_Data[REG_F_HIGH_FREQPULSE] << 8))<<16);
                    break;

                case REG_PULSE_PERIOD:
                        qDebug() <<"Reg_Pulse_Period " ;
                        qDebug()<< ((Read_Data[REG_T_LOW_FREQPULSE]  | (Read_Data[REG_T_HIGH_FREQPULSE] << 8) )
                                 | (Read_Data[REG_F_LOW_FREQPULSE] | (Read_Data[REG_F_HIGH_FREQPULSE] << 8))<<16);
                    break;

                case REG_PULSE_WIDTH:
                        qDebug() <<"Reg_Pulse_Width " ;
                        qDebug()<< ((Read_Data[REG_T_LOW_FREQPULSE]  | (Read_Data[REG_T_HIGH_FREQPULSE] << 8) )
                                 | (Read_Data[REG_F_LOW_FREQPULSE] | (Read_Data[REG_F_HIGH_FREQPULSE] << 8))<<16);
                    break;

                case REG_TEMP_ALARM_VALUE:
                    qDebug() <<"Reg_Temp_Alarm_Value " ;
                    qDebug() << (Read_Data[REG_LOW_ALARM ] | (Read_Data[REG_HIGH_ALARM] << 8));
                    break;

                case REG_OVERPOWER_MINUS_ALARM_VALUE:
                    qDebug() <<"Reg_OverPower_Minus_Alarm_Value " ;
                    qDebug() << (Read_Data[REG_LOW_ALARM ] | (Read_Data[REG_HIGH_ALARM] << 8));
                    break;

                case REG_OVERPOWER_PLUS_ALARM_VALUE:
                    qDebug() <<"Reg_OverPower_Plus_Alarm_Value " ;
                    qDebug() << (Read_Data[REG_LOW_ALARM ] | (Read_Data[REG_HIGH_ALARM] << 8));
                    break;
                case REG_VSWR_ALARM_VALUE:
                    qDebug() <<"Reg_VSWR_Alarm_Value " ;
                    qDebug() << (Read_Data[REG_LOW_ALARM ] | (Read_Data[REG_HIGH_ALARM] << 8));
                    break;
                case REG_TEMP_ALARM_TIME:
                    qDebug() <<"Reg_Temp_Alarm_Time " ;
                    qDebug() << (Read_Data[REG_LOW_ALARM ] | (Read_Data[REG_HIGH_ALARM] << 8));
                    break;
                case REG_OVERPOWER_ALARM_TIME:
                    qDebug() <<"Reg_OverPower_Alarm_Time " ;
                    qDebug() << (Read_Data[REG_LOW_ALARM ] | (Read_Data[REG_HIGH_ALARM] << 8));
                    break;
                case REG_VSWR_ALARM_TIME:
                    qDebug() <<"Reg_VSWR_Alarm_Time " ;
                    qDebug() << (Read_Data[REG_LOW_ALARM ] | (Read_Data[REG_HIGH_ALARM] << 8));
                    break;
                case REG_PLLUNLOCK_ALARM_TIME:
                    qDebug() <<"Reg_PLLUnlock_Alarm_Time " ;
                    qDebug() << (Read_Data[REG_LOW_ALARM ] | (Read_Data[REG_HIGH_ALARM] << 8));
                    break;
                case REG_LINKFAIL_ALARM_TIME:
                    qDebug() <<"Reg_LinkFail_Alarm_Time " ;
                    qDebug() << (Read_Data[REG_LOW_ALARM ] | (Read_Data[REG_HIGH_ALARM] << 8));
                    break;

                default :
                    break;
            }

        }
        else if(Read_Command == READ_COMMAND_REMOTE_START)
        {
             //Ready Status
            if(Read_Data[10] == 0x01) //Ready
            {
                emit sig_FD_Ready_Status_OK();
            }
            else if (Read_Data[10] == 0x0F) //Not Ready
            {
                emit sig_FD_Ready_Status_NO();
            }
        }
        else if(Read_Command == READ_COMMAND_REMOTE_DATA)
        {
            int Frame_Number;
            Frame_Number = (Read_Data[11] | (Read_Data[10] << 8));
            if(Frame_Number != 0x36)
            {
                emit sig_Frame_Number(Frame_Number);
            }
            else
            {
                emit sig_FD_Status_Confirm();
            }

        }
        else if(Read_Command == READ_COMMAND_REMOTE_CONFIRM)
        {

        }
        return true;
    }

    else
    {
#ifndef __MICROWAVE_TEST__
#else
        qDebug() << "Frame_Parsing Read_Data Fail" ;
#endif
        return false;
    }
}


void Data::Operation_Mode_Funtion(uint8 data)
{
    const unsigned char reserved1 = 1 << 0; // 0000 0001
    const unsigned char reserved2 = 1 << 1; // 0000 0010
    const unsigned char reserved3 = 1 << 2; // 0000 0100
    const unsigned char reserved4 = 1 << 3; // 0000 1000
    const unsigned char power_stable_mode = 1 << 4; // 0001 0000
    const unsigned char frequency_stable_mode = 1 << 5; // 0010 0000
    const unsigned char reserved5 = 1 << 6; // 0100 0000
    const unsigned char reserved6 = 1 << 7; // 1000 0000


    if(data & power_stable_mode) ; // PA_Enable = false;
    if(data & frequency_stable_mode) ; //High_Temp = false;

}

void Data::Alarm_Status_Funtion(uint8 data)
{
    //qDebug() << "Alarm Data = " << data;
    const unsigned char pa_enable = 1 << 0; // 0000 0001
    const unsigned char high_temperature = 1 << 1; // 0000 0010
    const unsigned char reserved1 = 1 << 2; // 0000 0100
    const unsigned char over_power = 1 << 3; // 0000 1000
    const unsigned char vswr = 1 << 4; // 0001 0000
    const unsigned char reserved2 = 1 << 5; // 0010 0000
    const unsigned char reserved3 = 1 << 6; // 0100 0000
    const unsigned char pll_unlock = 1 << 7; // 1000 0000

    if(data & pa_enable) PA_Enable = false; //disable
    else PA_Enable = true; //enable
    if(data & high_temperature) High_Temp = false; //disable
    else High_Temp = true; //enable
    if(data & over_power) Over_Power = false; //disable
    else Over_Power = true; //enable
    if(data & vswr) VSWR = false; //disable
    else VSWR = true; //enable
    if(data & pll_unlock) PLL_Unlock= false; //disable
    else PLL_Unlock= true; //enable
}

//double Data::get_Temperature()
uint32 Data::get_Temperature()
{
    return Temperature;
}
//double Data::get_Frequency()
uint32 Data::get_Frequency()
{
    return Frequency;
}
double Data::get_Pulse_Width()
{
    return Pulse_Width;
}
double Data::get_Pulse_Period()
{
    return Pulse_Period;
}
//double Data::get_F_Power()
uint32 Data::get_F_Power()
{
    return F_Power_W;
}
uint32 Data::get_Power_Level()
{
    return Power_Level;
}

//double Data::get_R_Power()
uint32 Data::get_R_Power()
{
    return R_Power_W;
}
uint32 Data::get_PLL_Freq_Max()
{
    return PLL_Freq_Max;
}
bool Data::get_PA_Enable()
{
    return PA_Enable;
}
bool Data::get_High_Temp()
{
    return High_Temp;
}

bool Data::get_Over_Power()
{
    return Over_Power;
}

bool Data::get_VSWR()
{
    return VSWR;
}
bool Data::get_PLL_Unlock()
{
    return PLL_Unlock;
}
