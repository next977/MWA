#include "frame.h"

frame::frame(QObject *parent) : QObject(parent)
{
    crccheck = new CRCCheck(this);
    Setting();
}

frame::~frame()
{

}

void frame::Setting()
{
    command_status = COMMAND_PA_STATUS;
    set_freq = 0;
    set_pulse_width = 0;
    set_pulse_period = 0;
    set_power_value = 0;

    Sub_DL_Data_Index = 0;
    Sub_DL_fseek_Index = 0;
    Frame_Basic_Size = 12;
    Frame_Total_Size = 0;
}

void frame::Head_Function()
{
    for(uint32 i = 0 ; i < sizeof(Head->StartCode); i++ )
    {
        Head->StartCode[i] = FRAME_START;
    }
}

void frame::Address_Function()
{
    Body->ADR_Source[0] = FRAME_SOURCEADDRESS_MASTER;
    Body->ADR_Destination[0] = FRAME_DESTINATIONADDRESS_PA;
}

void frame::Command_Function()
{
    switch(command_status)
    {
        case COMMAND_PA_STATUS:
            Body->Command[0] = FRAME_COMMAND_PA_STATUS;
            break;
        case COMMAND_PA_ENABLE:
            Body->Command[0] = FRAME_COMMAND_PA_ENABLE;
            break;
        case COMMAND_PA_DISABLE:
            Body->Command[0] = FRAME_COMMAND_PA_DISABLE;
            break;
        case COMMAND_PA_CONTROL:
            Body->Command[0] = FRAME_COMMAND_PA_CONTROL;
            break;
        case COMMAND_FREQ_SEARCH:
            Body->Command[0] = FRAME_COMMAND_FREQUENCY_SEARCH;
            break;
        case COMMAND_REG_READ:
            Body->Command[0] = FRAME_COMMAND_REG_READ;
            break;
        case COMMAND_REG_WRITE:
            Body->Command[0] = FRAME_COMMAND_REG_WRITE;
            break;
        case COMMAND_REMOTE_START:
            Body->Command[0] = FRAME_COMMAND_REMOTE_START;
            break;
        case COMMAND_REMOTE_DATA:
            Body->Command[0] = FRAME_COMMAND_REMOTE_DATA;
            break;
        case COMMAND_REMOTE_CONFIRM:
            Body->Command[0] = FRAME_COMMAND_REMOTE_CONFIRM;
            break;

        default:
            break;
    }
}
void frame::SubDataLength_Function()
{
    Body->Sub_Length[0] = 0x00;
    switch(command_status)
    {
        case COMMAND_PA_STATUS:
            Body->Sub_Length[1] = 0x00;
            Frame_SubData_Size = 0x00;
            break;
        case COMMAND_PA_ENABLE:
            Body->Sub_Length[1] = 0x00;
            Frame_SubData_Size = 0x00;
            break;
        case COMMAND_PA_DISABLE:
            Body->Sub_Length[1] = 0x00;
            Frame_SubData_Size = 0x00;
            break;
        case COMMAND_PA_CONTROL:
            Body->Sub_Length[1] = 0x12;
            Frame_SubData_Size = 0x12;
            break;
        case COMMAND_FREQ_SEARCH:
            Body->Sub_Length[1] = 0x12;
            Frame_SubData_Size = 0x12;
            break;
        case COMMAND_REG_READ:
            Body->Sub_Length[1] = 0x04;
            Frame_SubData_Size = 0x04;
            break;
        case COMMAND_REG_WRITE:
            if( address_status == ADRESS_FREQUENCY    ||
                address_status == ADRESS_PULSE_PERIOD ||
                address_status == ADRESS_PULSE_WIDTH )
            {
                Body->Sub_Length[1] = 0x08;
                Frame_SubData_Size = 0x08;
            }
            else
            {
                Body->Sub_Length[1] = 0x06;
                Frame_SubData_Size = 0x06;
            }
            break;
        case COMMAND_REMOTE_START:
            qDebug()<< "Command_Remote_Start In";
            Body->Sub_Length[1] = 0x00; //no SubData
            Frame_SubData_Size = 0x00;
            break;
        case COMMAND_REMOTE_DATA:
            {
                qDebug()<< "Command_Remote_Data In";
                uint16 Remote_subdata = sizeof(Body->Sub_DL_Frame_Number)+Frame_RD_Size;//2+30//2+250//0x6994; //2Byte+NByte (Bin Data Length)
                uint8 Remote_SubData[2];
                Frame_SubData_Size = Remote_subdata;
                memcpy(Remote_SubData, &Remote_subdata , 2);
                for(uint32 i = 0 ; i < sizeof(Body->Sub_Length) ; i++)
                {
                    Body->Sub_Length[i]= Remote_SubData[(sizeof(Body->Sub_Length)-1)-i];
                }

#ifdef __MICROWAVE_DEBUG__
                //qDebug()<< "Command_Remote_Data Out";
                for(int i  = 0 ; i< sizeof(Body->Sub_Length); i ++)
                {
                    qDebug("Body->Sub_Length[%d]=%02X", i, 0xFF & Body->Sub_Length[i]);
                }
                qDebug() << "Remote_subdata_Length = " <<Remote_subdata ;
#endif
                break;
            }
        case COMMAND_REMOTE_CONFIRM:
            qDebug()<< "Command_Remote_ConFirm In";

            Body->Sub_Length[1] = sizeof(Body->Sub_DL_Total_Length)+sizeof(Body->Sub_DL_Total_CRC); //4Byte+2Byte
            Frame_SubData_Size = sizeof(Body->Sub_DL_Total_Length)+sizeof(Body->Sub_DL_Total_CRC);
            break;

        default:
            break;
    }
}

void frame::SubData_Function()
{
    int Sizeof_Index = 0;

    switch(command_status)
    {
        case COMMAND_PA_STATUS:
            break;
        case COMMAND_PA_CONTROL:
            SysControl_Function();
            for(uint32 i = 0; i < sizeof(Body->Sub_SysControlFlag); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_SysControlFlag[i];
                Sizeof_Index ++;
            }
            for(uint32 i = 0; i < sizeof(Body->Sub_GainControlValue); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_GainControlValue[i];
                Sizeof_Index ++;
            }
            for(uint32 i = 0; i < sizeof(Body->Sub_PowerLevel); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_PowerLevel[i];
                Sizeof_Index ++;
            }
            for(uint32 i = 0; i < sizeof(Body->Sub_StableMode); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_StableMode[i];
                Sizeof_Index ++;
            }
            for(uint32 i = 0; i < sizeof(Body->Sub_PLLFreq); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_PLLFreq[i];
                Sizeof_Index ++;
            }
            for(uint32 i = 0; i < sizeof(Body->Sub_PPeriodValue); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_PPeriodValue[i];
                Sizeof_Index ++;
            }
            for(uint32 i = 0; i < sizeof(Body->Sub_PWidthValue); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_PWidthValue[i];
                Sizeof_Index ++;
            }
            break;
        case COMMAND_FREQ_SEARCH:
            FreqFind_Function();
            for(uint32 i = 0; i < sizeof(Body->Sub_FindControlFlag); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_FindControlFlag[i];
                Sizeof_Index ++;
            }
            for(uint32 i = 0; i < sizeof(Body->Sub_FreqStableMode); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_FreqStableMode[i];
                Sizeof_Index ++;
            }
            for(uint32 i = 0; i < sizeof(Body->Sub_StartFreq); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_StartFreq[i];
                Sizeof_Index ++;
            }
            for(uint32 i = 0; i < sizeof(Body->Sub_EndFreq); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_EndFreq[i];
                Sizeof_Index ++;
            }
            for(uint32 i = 0; i < sizeof(Body->Sub_SearchFreqStep); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_SearchFreqStep[i];
                Sizeof_Index ++;
            }
            for(uint32 i = 0; i < sizeof(Body->Sub_All_Zero); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_All_Zero[i];
                Sizeof_Index ++;
            }
            for(uint32 i = 0; i < sizeof(Body->Sub_FreqStableOperInterval); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_FreqStableOperInterval[i];
                Sizeof_Index ++;
            }
            for(uint32 i = 0; i < sizeof(Body->Sub_FreqStableRange); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_FreqStableRange[i];
                Sizeof_Index ++;
            }
            for(uint32 i = 0; i < sizeof(Body->Sub_FreqStableStep); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_FreqStableStep[i];
                Sizeof_Index ++;
            }
            break;
        case COMMAND_REG_READ:
            Reg_ReadWrite_Function();
            for(uint32 i = 0; i < sizeof(Body->Sub_Reg_Data_Length); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_Reg_Data_Length[i];
                Sizeof_Index ++;
            }
            for(uint32 i = 0; i < sizeof(Body->Sub_Reg_Start_Address); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_Reg_Start_Address[i];
                Sizeof_Index ++;
            }
            break;
        case COMMAND_REG_WRITE:
            uint32 Reg_DataValue_Length;
            if( address_status == ADRESS_FREQUENCY    ||
                address_status == ADRESS_PULSE_PERIOD ||
                address_status == ADRESS_PULSE_WIDTH )
            {
                Reg_DataValue_Length = 4;
            }
            else
            {
                Reg_DataValue_Length = 2;
            }
            Reg_ReadWrite_Function();
            for(uint32 i = 0; i < sizeof(Body->Sub_Reg_Data_Length); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_Reg_Data_Length[i];
                Sizeof_Index ++;
            }
            for(uint32 i = 0; i < sizeof(Body->Sub_Reg_Start_Address); i++)
            {

                Body->Sub_Value[Sizeof_Index] = Body->Sub_Reg_Start_Address[i];
                Sizeof_Index ++;
            }
            for(uint32 i = 0; i < Reg_DataValue_Length; i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_Reg_Data[i];
                Sizeof_Index ++;
            }
            break;
        case COMMAND_REMOTE_START:
            Sub_DL_fseek_Index = 0;
            break;
        case COMMAND_REMOTE_DATA:
            {
                Download_File_Function();
                Download_Data_Function();
                for(uint32 i = 0; i < sizeof(Body->Sub_DL_Frame_Number); i++)
                {
                    Body->Sub_Value[Sizeof_Index] = Body->Sub_DL_Frame_Number[i];
                    Sizeof_Index ++;
                }
                qDebug() << "Sub Frame Numver >> Sub_Value ";
                for(uint32 i = 0; i < sizeof(Body->Sub_DL_Data); i++)
                {
                    Body->Sub_Value[Sizeof_Index] = Body->Sub_DL_Data[i];
                    Sizeof_Index ++;
                }
                free(Body->Sub_DL_Data);
                Body->Sub_DL_Data = nullptr;
                //qDebug() << "Sub DL Data >> Sub_Value ";

            }
            break;

        case COMMAND_REMOTE_CONFIRM:
            qDebug() << "REMOTE_CONFIRM IN" ;
            Download_Confirm_Function();
            for(uint32 i = 0; i < sizeof(Body->Sub_DL_Total_Length); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_DL_Total_Length[i];
                Sizeof_Index ++;
            }
            qDebug() << "Sub Frame Numver >> Sub_Value ";
            for(uint32 i = 0; i < sizeof(Body->Sub_DL_Total_CRC); i++)
            {
                Body->Sub_Value[Sizeof_Index] = Body->Sub_DL_Total_CRC[i];
                Sizeof_Index ++;
            }

            break;
        default:
            break;
    }
}

void frame::SysControl_Function()
{
    //<------- Input ------->
    uint32 gain_control = 0;
    uint32 power_level = set_power_value;//50; //w
    uint32 stable_mode = 0;
    uint32 pll_freq = set_freq*1000;//2500000; // 2400MHz~2500MHz
    uint32 p_period = (uint32)(set_pulse_period*1000);//2000;
    uint32 p_width = (uint32)(set_pulse_width*10000);//5000;

    uint8 gain_control_value[2], power_level_value[2], pll_freq_value[4];
    uint8 p_period_value[4], p_width_value[4];

    memcpy(gain_control_value, &gain_control , 2);
    memcpy(power_level_value, &power_level , 2);
    memcpy(pll_freq_value, &pll_freq , 4);
    memcpy(p_period_value, &p_period , 4);
    memcpy(p_width_value, &p_width , 4);

    Body->Sub_SysControlFlag[0] = SysControlFlag_Funtion();
    for(uint32 i = 0 ; i < sizeof(Body->Sub_GainControlValue) ; i++)
    {
        Body->Sub_GainControlValue[i]= gain_control_value[(sizeof(Body->Sub_GainControlValue)-1)-i];
    }
    for(uint32 i = 0 ; i < sizeof(Body->Sub_PowerLevel) ; i++)
    {
        Body->Sub_PowerLevel[i]= power_level_value[(sizeof(Body->Sub_PowerLevel)-1)-i];
    }
    Body->Sub_StableMode[0] = stable_mode;
    for(uint32 i = 0 ; i < sizeof(Body->Sub_PLLFreq) ; i++)
    {
        Body->Sub_PLLFreq[i]= pll_freq_value[(sizeof(Body->Sub_PLLFreq)-1)-i];
    }
    for(uint32 i = 0 ; i < sizeof(Body->Sub_PPeriodValue) ; i++)
    {
        Body->Sub_PPeriodValue[i]= p_period_value[(sizeof(Body->Sub_PPeriodValue)-1)-i];
    }
    for(uint32 i = 0 ; i < sizeof(Body->Sub_PWidthValue) ; i++)
    {
        Body->Sub_PWidthValue[i]= p_width_value[(sizeof(Body->Sub_PWidthValue)-1)-i];
    }
}

void frame::FreqFind_Function()
{
    //<------- Input ------->
    uint32 start_freq = set_start_freq*1000; //2410000;
    uint32 end_freq = set_end_freq*1000;//2500000;
    uint32 search_freq_step = set_freq_step*1000; //500
    uint32 all_zero = 0x000000000; //always 0x000000000
    uint32 freq_stable_operinterval = set_freq_stable_interval; // 1 = 0.5s
    uint32 freq_stable_range = set_freq_stable_range*1000; //2400000
    uint32 freq_stable_step = set_freq_stable_step*1000; //1000

    uint8 start_freq_value[4], end_freq_value[4], search_freq_step_value[4];
    uint8 all_zero_value[5], freq_stable_operinterval_value[2], freq_stable_range_value[4];
    uint8 freq_stable_step_value[2];

    memcpy(start_freq_value, &start_freq , 4);
    memcpy(end_freq_value, &end_freq , 4);
    memcpy(search_freq_step_value, &search_freq_step , 4);
    memcpy(all_zero_value, &all_zero , 5);
    memcpy(freq_stable_operinterval_value, &freq_stable_operinterval , 2);
    memcpy(freq_stable_range_value, &freq_stable_range , 4);
    memcpy(freq_stable_step_value, &freq_stable_step , 2);

    Body->Sub_FindControlFlag[0] = FindControlFlag_Funtion(freq_mode_status);
    Body->Sub_FreqStableMode[0] = set_freq_stable_mode;

    for(uint32 i = 0 ; i < sizeof(Body->Sub_StartFreq) ; i++)
    {
        Body->Sub_StartFreq[i]= start_freq_value[(sizeof(Body->Sub_StartFreq)-1)-i];
    }
    for(uint32 i = 0 ; i < sizeof(Body->Sub_EndFreq) ; i++)
    {
        Body->Sub_EndFreq[i]= end_freq_value[(sizeof(Body->Sub_EndFreq)-1)-i];
    }
    for(uint32 i = 0 ; i < sizeof(Body->Sub_SearchFreqStep) ; i++)
    {
        Body->Sub_SearchFreqStep[i]= search_freq_step_value[(sizeof(Body->Sub_SearchFreqStep)-1)-i];
    }
    for(uint32 i = 0 ; i < sizeof(Body->Sub_All_Zero) ; i++)
    {
        Body->Sub_All_Zero[i]= all_zero_value[(sizeof(Body->Sub_All_Zero)-1)-i];
    }
    for(uint32 i = 0 ; i < sizeof(Body->Sub_FreqStableOperInterval) ; i++)
    {
        Body->Sub_FreqStableOperInterval[i]= freq_stable_operinterval_value[(sizeof(Body->Sub_FreqStableOperInterval)-1)-i];
    }
    for(uint32 i = 0 ; i < sizeof(Body->Sub_FreqStableRange) ; i++)
    {
        Body->Sub_FreqStableRange[i]= freq_stable_range_value[(sizeof(Body->Sub_FreqStableRange)-1)-i];
    }
    for(uint32 i = 0 ; i < sizeof(Body->Sub_FreqStableStep) ; i++)
    {
        Body->Sub_FreqStableStep[i]= freq_stable_step_value[(sizeof(Body->Sub_FreqStableStep)-1)-i];
    }
    qDebug() << "start_freq = " << start_freq;
    qDebug() << "end_freq= " << end_freq;
    qDebug() << "search_freq_step = " << search_freq_step;

    qDebug() << "freq_set_freq_stable_mode= " << set_freq_stable_mode;
    qDebug() << "freq_stable_range= " << freq_stable_range;
    qDebug() << "freq_stable_step = " << freq_stable_step;
    qDebug() << "freq_stable_operinterval = " << freq_stable_operinterval;
}

void frame::Reg_ReadWrite_Function()
{
    uint32 Data = set_address_data;
    uint32 DataValue_Length;
    if( address_status == ADRESS_FREQUENCY    ||
        address_status == ADRESS_PULSE_PERIOD ||
        address_status == ADRESS_PULSE_WIDTH )
    {
        DataValue_Length = 4;
    }
    else
    {
        DataValue_Length = 2;
    }
    //qDebug() << "Data = " << Data;
    uint8 Data_value[DataValue_Length];
    memcpy(Data_value, &Data , DataValue_Length);
    for(uint32 i = 0 ; i < DataValue_Length ; i++)
    {
        Body->Sub_Reg_Data[i]= Data_value[(DataValue_Length-1)-i];
    }
#ifdef __MICROWAVE_TEST__
#else
    for(int i  = 0 ; i< 2; i ++)
    {
        qDebug("[%d]=%02X", i, 0xFF & Body->Sub_Reg_Data[i]);
    }
#endif
    Body->Sub_Reg_Data_Length[0] =  0x00;
    Body->Sub_Reg_Start_Address[0] = 0x00;
    switch(address_status)
    {
        case ADRESS_FREQUENCY:
            Body->Sub_Reg_Data_Length[1] =  STARTADDRESS_DATALENGTH_VALUE;
            Body->Sub_Reg_Start_Address[1] =  STARTADDRESS_FREQUENCY;
            break;
        case ADRESS_PULSE_PERIOD:
            Body->Sub_Reg_Data_Length[1] =  STARTADDRESS_DATALENGTH_VALUE;
            Body->Sub_Reg_Start_Address[1] =  STARTADDRESS_PULSE_PERIOD;
            break;
        case ADRESS_PULSE_WIDTH:
            Body->Sub_Reg_Data_Length[1] =  STARTADDRESS_DATALENGTH_VALUE;
            Body->Sub_Reg_Start_Address[1] =  STARTADDRESS_PULSE_WIDTH;
            break;
        case ADRESS_ALARMVALUE_OVER_TEMP:
            Body->Sub_Reg_Data_Length[1] =  STARTADDRESS_DATALENGTH_ALARM;
            Body->Sub_Reg_Start_Address[1] =  STARTADDRESS_ALARMVALUE_OVER_TEMP;
            break;
        case ADRESS_ALARMVALUE_OVER_POWER_LOW:
            Body->Sub_Reg_Data_Length[1] =  STARTADDRESS_DATALENGTH_ALARM;
            Body->Sub_Reg_Start_Address[1] =  STARTADDRESS_ALARMVALUE_OVER_POWER_LOW;
            break;
        case ADRESS_ALARMVALUE_OVER_POWER_HIGH:
            Body->Sub_Reg_Data_Length[1] =  STARTADDRESS_DATALENGTH_ALARM;
            Body->Sub_Reg_Start_Address[1] =  STARTADDRESS_ALARMVALUE_OVER_POWER_HIGH;
            break;
        case ADRESS_ALARMVALUE_VSWR:
            Body->Sub_Reg_Data_Length[1] =  STARTADDRESS_DATALENGTH_ALARM;
            Body->Sub_Reg_Start_Address[1] =  STARTADDRESS_ALARMVALUE_VSWR;
            break;
        case ADRESS_ALARMTIME_OVER_TEMP:
            Body->Sub_Reg_Data_Length[1] =  STARTADDRESS_DATALENGTH_ALARM;
            Body->Sub_Reg_Start_Address[1] =  STARTADDRESS_ALARMTIME_OVER_TEMP;
            break;
        case ADRESS_ALARMTIME_OVER_POWER:
            Body->Sub_Reg_Data_Length[1] =  STARTADDRESS_DATALENGTH_ALARM;
            Body->Sub_Reg_Start_Address[1] =  STARTADDRESS_ALARMTIME_OVER_POWER;
            break;
        case ADRESS_ALARMTIME_VSWR:
            Body->Sub_Reg_Data_Length[1] =  STARTADDRESS_DATALENGTH_ALARM;
            Body->Sub_Reg_Start_Address[1] =  STARTADDRESS_ALARMTIME_VSWR;
            break;
        case ADRESS_ALARMTIME_PLL_UNLOCK:
            Body->Sub_Reg_Data_Length[1] =  STARTADDRESS_DATALENGTH_ALARM;
            Body->Sub_Reg_Start_Address[1] =  STARTADDRESS_ALARMTIME_PLL_UNLOCK;
            break;
        case ADRESS_ALARMTIME_LINK_FAIL:
            Body->Sub_Reg_Data_Length[1] =  STARTADDRESS_DATALENGTH_ALARM;
            Body->Sub_Reg_Start_Address[1] =  STARTADDRESS_ALARMTIME_LINK_FAIL;
            break;
        default:
            break;
    }
}

void frame::Download_Data_Function()
{

    uint16 download_frame_number = Sub_DL_Data_Index;//++;//Sub_DL_Data_Index+1
    uint8 Download_Frame_Number[2];

    memcpy(Download_Frame_Number, &download_frame_number , 2);
    for(uint32 i = 0 ; i < sizeof(Body->Sub_DL_Frame_Number) ; i++)
    {
        Body->Sub_DL_Frame_Number[i]= Download_Frame_Number[(sizeof(Body->Sub_DL_Frame_Number)-1)-i];
    }

    qDebug() << "Download_Data_Function Out";

}

void frame::Download_File_Function()
{
    //int8 Sub_DL_Size = 250;
    //uint8 *Sub_DL_Data_Buf = static_cast<uint8*>(malloc(sizeof(uint8) * Sub_DL_Size)) ;

    uint32 Data_Byte_Length;
    uint32 File_Data_Quotient;
    uint32 File_Data_Remainder;

#ifdef __MICROWAVE_TEST__
#else
    FILE *fp = fopen("/home/ubuntu/MICROWAVE/MICROWAVE_Linux_Boad_FWUpdate_v5_201110/build-MICROWAVE-Desktop_Qt_5_8_0_GCC_64bit-Debug/MedicalPA_FW_v99.bin","rb");
    for (size_to_send = fsiz=00
         uart_Write_data[8]=12e; size_to_send > 0; ){
      rc = sendfile(newsockd, fd, &Sub_DL_Offset, size_to_send);
      if (rc <= 0){
        perror("sendfile");
        onexit(newsockd, sockd, fd, 3);
      }
      Sub_DL_Offset += rc;
      size_to_send -= rc;
    }
    close(fp); /* la chiusura del file va qui altrimenti rischio loop infinito e scrittura all'interno del file */

    /*
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, "226 File Successfully transfered\n");
    if(send(newsockd, buffer, strlen(buffer), 0) < 0){
      perror("Errore durante l'invio 226");
      onexit(newsockd, sockd, 0, 2);
    }
    memset(buffer, 0, sizeof(buffer));
    */for(uint32 i = 0; i < 18; i++) //sizeof(Body->Sub_Value)
    {
       Frame_combine[Sizeof_Index] = Body->Sub_Value[i];
       crc_check[crc_count] = Body->Sub_Value[i];
       Sizeof_Index ++;
       crc_count++;
    }
#endif
#ifdef _ONPC_
    FILE *fp = fopen("/home/ubuntu/MICROWAVE/MICROWAVE_Linux_Boad_FWUpdate_v5_201110/build-MICROWAVE-Desktop_Qt_5_8_0_GCC_64bit-Debug/MedicalPA_FW_v99.bin","rb");
#else
    FILE *fp = fopen("/app/app/fw_download/MedicalPA_FW_v99.bin","rb");
#endif

#ifndef __MICROWAVE_TEST__
#else
    if(fp != NULL)
    {
        //qDebug() <<"Filename : " <<fp ;
        if(Sub_DL_fseek_Index == 0)
        {
            fseek(fp,0,SEEK_END); //go to end
            File_Length = ftell(fp); //get position at end (length)
            fseek(fp,0,SEEK_SET); //go to beg.

        }

        Data_Byte_Length = 500; //MAX 500Byte
        File_Data_Quotient = File_Length / Data_Byte_Length; //27
        File_Data_Remainder = File_Length % Data_Byte_Length; //400
        qDebug() << "fp_length = " << File_Length ;
        qDebug() << "File_Data_Quotient = " <<File_Data_Quotient;
        qDebug() << "File_Data_Remainder = " <<File_Data_Remainder;

        if(Sub_DL_Data_Index < File_Data_Quotient) //54
        {

            Sub_DL_fseek_Index = (Sub_DL_Data_Index)*Data_Byte_Length;
            Frame_RD_Size = Data_Byte_Length; //500


        }
        else if(Sub_DL_Data_Index == File_Data_Quotient)
        {
            //Sub_DL_fseek_Index = File_Data_Remainder; //28
            Sub_DL_fseek_Index = (Sub_DL_Data_Index)*Data_Byte_Length;
            Data_Byte_Length = File_Data_Remainder;
            Frame_RD_Size = File_Data_Remainder; //41
            //command_status = COMMAND_REMOTE_CONFIRM;
            qDebug() << "Last Data DownLoad " ;

        }

        qDebug() << "Sub_DL_Data_Index = " << Sub_DL_Data_Index;
        qDebug() << "Sub_DL_fseek_Index = " << Sub_DL_fseek_Index;


        Body->Sub_DL_Data = static_cast<uint8*>(malloc(sizeof(uint8) * Data_Byte_Length));
        fseek(fp,Sub_DL_fseek_Index,SEEK_SET); //12 , SEEK_SET
        fread(Body->Sub_DL_Data, Data_Byte_Length,1,fp); //read into buffer //250 //3


        fclose(fp);
        qDebug() << "File Close" ;
    }
    else
    {
        qDebug() << "DownLoad FileOpen Fail! ";
    }





#endif

#ifdef __MICROWAVE_TEST__
#else
    //ifstream File; //Input - Read (in file stream)
    ifstream File(filename.toStdString(),ios::in | ios::binary);
    //File.open(filename.toStdString(),ios::in | ios::binary);
    char FileRead_arr[256];
    if(File.is_open())
    {
        qDebug() << "File Open Success" ;
        while(!File.eof())    //file end
        {
            File.read(FileRead_arr, 256);

            //File.getline(FileRead_arr, 256);    //Line Read
            qDebug()<<"FileRead_arr = " << FileRead_arr;
        }
        //qDebug()<<"FileRead_arr = " << FileRead_arr;
    }
    else
    {
        qDebug() << "File Open Error" ;
        return;
    }
#endif



#ifdef __MICROWAVE_TEST__
#else
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "File Open Error" ;
        return;
    }
    qDebug() << "File Open Success" ;
    QByteArray File_Read = file.readAll();
    qDebug() << "File_Read = " <<File_Read;
#endif
   // File.close();

}

void frame::Download_Confirm_Function()
{


    uint32 total_data_length = 0x00006994;
    uint32 total_data_crc = 0x14D9;


    uint8 total_data_length_value[4];
    uint8 total_data_crc_value[2];

    memcpy(total_data_length_value, &total_data_length , 4);
    memcpy(total_data_crc_value, &total_data_crc , 2);


    for(uint32 i = 0 ; i < sizeof(Body->Sub_DL_Total_Length) ; i++)
    {
        Body->Sub_DL_Total_Length[i]= total_data_length_value[(sizeof(Body->Sub_DL_Total_Length)-1)-i];
    }
    for(uint32 i = 0 ; i < sizeof(Body->Sub_DL_Total_CRC) ; i++)
    {
        Body->Sub_DL_Total_CRC[i]= total_data_crc_value[(sizeof(Body->Sub_DL_Total_CRC)-1)-i];
    }

}

uint8 frame::SysControlFlag_Funtion()
{
    uint8 control_flag = 0x00;
    const unsigned char gain_control  = 1 << 0; // 0000 0001
    const unsigned char power_set     = 1 << 1; // 0000 0010
    const unsigned char stable_mode   = 1 << 2; // 0000 0100
    const unsigned char not_use1      = 1 << 3; // 0000 1000
    const unsigned char pll_frequency = 1 << 4; // 0001 0000
    const unsigned char not_use2      = 1 << 5; // 0010 0000
    const unsigned char pulse_period  = 1 << 6; // 0100 0000
    const unsigned char pulse_width   = 1 << 7; // 1000 0000

    control_flag &= ~(not_use1|not_use2); //not_use1,2는 항상 0
    //control_flag |= gain_control;
    control_flag |= power_set;
    control_flag |= pll_frequency;
    control_flag |= pulse_period;
    control_flag |= pulse_width;
    //qDebug()<<"control_flag = "<<control_flag;

    return control_flag;
}

uint8 frame::FindControlFlag_Funtion(Frequency_Mode_Status state)
{
    uint8 control_flag = 0x00;
    const unsigned char frequency_stable_mode  = 1 << 0; // 0000 0001
    const unsigned char reserved1              = 1 << 1; // 0000 0010
    const unsigned char reserved2              = 1 << 2; // 0000 0100
    const unsigned char reserved3              = 1 << 3; // 0000 1000
    const unsigned char frequency_find         = 1 << 4; // 0001 0000
    const unsigned char reserved4              = 1 << 5; // 0010 0000
    const unsigned char reserved5              = 1 << 6; // 0100 0000
    const unsigned char reserved6              = 1 << 7; // 1000 0000
    Q_UNUSED(reserved1);
    Q_UNUSED(reserved2);
    Q_UNUSED(reserved3);
    Q_UNUSED(reserved4);
    Q_UNUSED(reserved5);
    Q_UNUSED(reserved6);
    //control_flag |= frequency_stable_mode;
    //control_flag |= frequency_find;
    switch(state)
    {
        case MODE_NONE:
            break;
        case MODE_FIND:
            control_flag |= frequency_find;
            break;
        case MODE_STABLE:
            control_flag |= frequency_stable_mode;
            break;
        default :
            break;
    }
#ifdef __MICROWAVE_TEST__
#else
    qDebug()<<"control_flag = " <<control_flag;
#endif
    return control_flag;
}


void frame::CRC_Function()
{  
   uint16 crc = crccheck->Generate_CRC(crc_check , crc_count);
   uint8 crc_value[2];
   memcpy(crc_value, &crc , 2);
   Body->CRC[0] = crc_value[1];
   Body->CRC[1] = crc_value[0];
}
void frame::Body_Function()
{
    Address_Function();
    Command_Function();

    SubData_Function();
    SubDataLength_Function();
}

void frame::Tail_Function()
{

    CRC_Function();
    Tail->EndCode[0] = FRAME_END;
}

uint8 *frame::Frame_Combine()
{
    uint32 Sizeof_Index = 0;
    crc_count = 0;

    Head_Function();
    Body_Function();


    Frame_Total_Size = Frame_Basic_Size + Frame_SubData_Size; //500
    //uint32 size = 12+sizeof(Body->Sub_Value);//300
#ifdef __MICROWAVE_DEBUG__
#else
    qDebug()<< "Frame_Combine Total_Size = " <<Frame_Total_Size ;
#endif
    //uint8 Frame_combine[size];
    //uint8 *Frame_combine = static_cast<uint8*>(malloc(sizeof(uint8) * size)) ;
    Frame_combine_array = static_cast<uint8*>(malloc(sizeof(uint8) * Frame_Total_Size)) ;
    //qDebug("1. malloc Frame_combine = %x", Frame_combine);

    //uint8 Frame_combine[size];



    for(uint32 i = 0; i < sizeof(Head->StartCode); i++)
    {
       Frame_combine_array[Sizeof_Index] = Head->StartCode[i];
       Sizeof_Index ++;
    }
    for(uint32 i = 0; i < sizeof(Body->ADR_Source); i++)
    {
       Frame_combine_array[Sizeof_Index] = Body->ADR_Source[i];
       crc_check[crc_count] = Body->ADR_Source[i];
       Sizeof_Index ++;
       crc_count++;
    }
    for(uint32 i = 0; i < sizeof(Body->ADR_Destination); i++)
    {
       Frame_combine_array[Sizeof_Index] = Body->ADR_Destination[i];
       crc_check[crc_count] = Body->ADR_Destination[i];
       Sizeof_Index ++;
       crc_count++;
    }

    for(uint32 i = 0; i < sizeof(Body->Command); i++)
    {
       Frame_combine_array[Sizeof_Index] = Body->Command[i];
       crc_check[crc_count] = Body->Command[i];
       Sizeof_Index ++;
       crc_count++;
    }
    for(uint32 i = 0; i < sizeof(Body->Sub_Length); i++)
    {
       Frame_combine_array[Sizeof_Index] = Body->Sub_Length[i];
       crc_check[crc_count] = Body->Sub_Length[i];
       Sizeof_Index ++;
       crc_count++;
    }
    //<------------------SubData----------------->
    switch(command_status)
    {
        case COMMAND_PA_STATUS:
            break;
        case COMMAND_PA_CONTROL:
        case COMMAND_FREQ_SEARCH:
            for(uint32 i = 0; i < 18; i++) //sizeof(Body->Sub_Value)
            {
               Frame_combine_array[Sizeof_Index] = Body->Sub_Value[i];
               crc_check[crc_count] = Body->Sub_Value[i];
               Sizeof_Index ++;
               crc_count++;

            }
            break;
        case COMMAND_REG_READ:
            for(uint32 i = 0; i < 4; i++) //sizeof(Body->Sub_Value)-14
            {
               Frame_combine_array[Sizeof_Index] = Body->Sub_Value[i];
               crc_check[crc_count] = Body->Sub_Value[i];
               Sizeof_Index ++;
               crc_count++;
            }
            break;
        case COMMAND_REG_WRITE:
            uint32 Reg_SubDataTotal_Length;
            if( address_status == ADRESS_FREQUENCY    ||
                address_status == ADRESS_PULSE_PERIOD ||
                address_status == ADRESS_PULSE_WIDTH )
            {
                Reg_SubDataTotal_Length = 8;
            }
            else
            {
                Reg_SubDataTotal_Length = 6;
            }
            for(uint32 i = 0; i < Reg_SubDataTotal_Length; i++)
            {
               Frame_combine_array[Sizeof_Index] = Body->Sub_Value[i];
               crc_check[crc_count] = Body->Sub_Value[i];
               Sizeof_Index ++;
               crc_count++;
            }
            break;
        case COMMAND_REMOTE_START:
            break;
        case COMMAND_REMOTE_DATA:
            for(uint32 i = 0; i < Frame_SubData_Size; i++) //12//sizeof(Body->Sub_Value) 252
            {
               Frame_combine_array[Sizeof_Index] = Body->Sub_Value[i];
               crc_check[crc_count] = Body->Sub_Value[i];
               Sizeof_Index ++;
               crc_count++;
            }
            break;
        case COMMAND_REMOTE_CONFIRM:
            for(uint32 i = 0; i < Frame_SubData_Size; i++) //12//sizeof(Body->Sub_Value) 252
            {
               Frame_combine_array[Sizeof_Index] = Body->Sub_Value[i];
               crc_check[crc_count] = Body->Sub_Value[i];
               Sizeof_Index ++;
               crc_count++;
            }
            break;

        default:
            break;
    }
    //<----------------------------------------->
    Tail_Function();



    for(uint32 i = 0; i < sizeof(Body->CRC); i++)
    {
       Frame_combine_array[Sizeof_Index] = Body->CRC[i];
       Sizeof_Index ++;
    }
    for(uint32 i = 0; i < sizeof(Tail->EndCode); i++)
    {
       Frame_combine_array[Sizeof_Index] = Tail->EndCode[i];
       Sizeof_Index ++;
    }
    //qDebug()<<"Frame_combine = "<<Frame_combine;
#ifndef __MICROWAVE_DEBUG__
        for(int i  = 0 ; i< 44; i ++) //len
        {
            qDebug("Frame_combine[%d]=%02X", i, 0xFF & Frame_combine[i]);
        }
#endif

    //qDebug("2. sizeindex = %d", Sizeof_Index);
    return Frame_combine_array;
}
