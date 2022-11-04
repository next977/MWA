#include "uart.h"

uart::uart(QWidget *parent) : QWidget(parent)
{

    Frame_Class = new frame(this);
    Uart_Timer = new QTimer(this);
    m_mutex = new QMutex();
    Setting();

}
uart::~uart()
{
#ifndef _ONPC_
    gpioTools::uart_close();
#endif
}

void uart::Setting()
{
    Event_connect();
    Send_Data_size = SEND_DATA_SIZE_PA;
    Uart_Timer->setInterval(500); //100
    uart_Connect();


    DownLoad_Flag = false;
    combine_Index = 0;

}

void uart::Event_connect()
{
#ifndef __WINDOWS_MICROWAVE__
   // connect(SerialPort, SIGNAL(readyRead()), this, SLOT(slot_Read_Event()));
#endif
    connect(Uart_Timer, SIGNAL(timeout()), this, SLOT(slot_Frame_write()));

}

void uart::uart_Disconnect()
{
    Uart_Timer->stop();
    qDebug()<<"Serial_Port_Close";
}

void uart::uart_Connect()
{
#ifndef _ONPC_
    int32 uart_fd = gpioTools::uart_init();
    gpioTools::USB_uart_init();
    if(uart_fd == -1)
    {
        qDebug()<<"uart connect error!";
    }
    else
    {
        qDebug()<<"uart connect success!";
        notRsRead = new QSocketNotifier(uart_fd , QSocketNotifier::Read , this);
        connect(notRsRead , SIGNAL(activated(int)) , this, SLOT(slot_Read_Event()));

    }
    Uart_Timer->start();
#endif
}

void uart::slot_Read_Event()
{
    char *pdata;
    int length;
#ifndef _ONPC_
    pdata = gpioTools::uart_binread(&length);
#endif
    for(int i  = 0 ; i< length; i ++)
    {
        //qDebug("uart_Read_Data[%d]=%02X", i, 0xFF & pdata[i]);
        if(pdata[i] == 0xF5)
        {
            memcpy(read_Data, combine, combine_Index-1);
            //memcpy(read_Data, pdata, length);
            //read_Data_length = length;
            read_Data_length = combine_Index+1;
            combine_Index = 0;
#ifdef __MICROWAVE_DEBUG__
#else
            qDebug()<<"read_Data_length = "<<read_Data_length;
            qDebug()<<"---------------------------------";
#endif
            emit sig_data_frameparsing();
        }
        else
        {
           *(combine+combine_Index) = *(pdata+i);
            combine_Index++;
        }
    }
}



void uart::slot_Frame_write()
{
    m_mutex->lock();

#ifdef __MICROWAVE_TEST__
#else
    uint8 Uart_test[] = {0x16,0x16,0x16,0x16,0xA0,0x21,0x11,0x00,0x00,0x1F,0xCD,0xF5};
    for(int i  = 0 ; i< Send_Data_size; i ++)
    {
        qDebug()<<"Uart_test = " <<QString::number(Uart_test[i], 16);
    }

#endif

#ifndef _ONPC_
    gpioTools::uart_binwrite(Frame_Class->Frame_Combine(),Frame_Class->Frame_Total_Size);
#endif
    //qDebug("3. free Frame_combine = %x", Frame_Class->Frame_combine);
#ifdef __MICROWAVE_DEBUG__
#else
    //qDebug() << "Send_Data_size = " <<Send_Data_size ;
    qDebug() << "Frame_Class->Frame_Total_Size = " <<Frame_Class->Frame_Total_Size ;

#endif
    free(Frame_Class->Frame_combine_array);
    Frame_Class->Frame_combine_array = nullptr;

#ifdef __MICROWAVE_DEBUG__
#else
    QByteArray Send_Data;
    uint8 *FramePointer;
    Send_Data.resize(Send_Data_size); //12
    FramePointer = Frame_Class->Frame_Combine();
    for(uint32 i = 0; i < Send_Data_size; i ++) //12
    {
        Send_Data[i] = FramePointer[i];
        //qDebug("send Data hex = %x" , Frame_Class->Frame_Combine()[i]);
    }
    qDebug()<<"send Data = "<<Send_Data;
#endif
#ifdef __MICROWAVE_TEST__
    if(Frame_Class->command_status == frame::COMMAND_PA_ENABLE)
    {
        Frame_Command(frame::COMMAND_PA_CONTROL);
    }
    else
    {
        Frame_Command(frame::COMMAND_PA_STATUS);
    }

#else

   // Uart_Timer->stop();
#endif

    m_mutex->unlock();
}

void uart::Frame_Number(uint32 frame_number)
{
    qDebug() <<" In Uart Frame_Number ";
    //Frame_Class->freq_mode_status = mode;
#if 1
    if(Frame_Class->Sub_DL_Data_Index-1 > frame_number)
    {
        Frame_Class->Sub_DL_Data_Index = frame_number+1;
        qDebug()<< "Frame_Class->Sub_DL_Data_Index = " << Frame_Class->Sub_DL_Data_Index;
    }
    Frame_Class->Sub_DL_Data_Index = frame_number+1;
    qDebug()<< "Frame_Class->Sub_DL_Data_Index = " << Frame_Class->Sub_DL_Data_Index;
#endif
}

void uart::Frame_Command(frame::Command_Status command)
{
    Frame_Class->command_status = command;
#ifdef __MICROWAVE_TEST__
#else
    qDebug() << "Frame ::command = " << command ;
#endif
}
void uart::Frame_Freq_Mode(frame::Frequency_Mode_Status mode)
{
    Frame_Class->freq_mode_status = mode;
}

void uart::Frame_Reg_Address(frame::Address_Status address)
{
    Frame_Class->address_status = address;
}

void uart::Frame_Sysctrl_Set(uint32 *freq, double *pulse_width, double *pulse_period, uint32 *power_value)
{
    Frame_Class->set_freq = *freq;
    Frame_Class->set_pulse_width = *pulse_width;
    Frame_Class->set_pulse_period = *pulse_period;
    Frame_Class->set_power_value = *power_value;
#ifdef __MICROWAVE_TEST__
#else
    qDebug()<<"Frame_Class->set_freq = "<<Frame_Class->set_freq;
    qDebug()<<"Frame_Class->set_pulse_width = "<<Frame_Class->set_pulse_width;
    qDebug()<<"Frame_Class->set_pulse_period = "                                        <<Frame_Class->set_pulse_period;
    qDebug()<<"Frame_Class->set_power_value = "<<Frame_Class->set_power_value;
#endif
}

void uart::Frame_FreqFind_Set(bool *freq_stable_mode,uint32 *start_freq, uint32 *end_freq, uint32 *freq_step,
                              uint32 *freq_stable_range, uint32 *freq_stable_interval , uint32 *freq_stable_step)
{
    Frame_Class->set_freq_stable_mode = *freq_stable_mode;
    Frame_Class->set_start_freq = *start_freq;
    Frame_Class->set_end_freq = *end_freq;
    Frame_Class->set_freq_step = *freq_step;
    Frame_Class->set_freq_stable_range = *freq_stable_range;
    Frame_Class->set_freq_stable_interval = *freq_stable_interval;
    Frame_Class->set_freq_stable_step = *freq_stable_step;

#ifdef __MICROWAVE_TEST__
#else
    qDebug()<<"Frame_Class->set_start_freq = "<<Frame_Class->set_start_freq;
    qDebug()<<"Frame_Class->set_end_freq = "<<Frame_Class->set_end_freq;
    qDebug()<<"Frame_Class->set_freq_step = "<<Frame_Class->set_freq_step;
    qDebug()<<"Frame_Class->set_freq_stable_range = "<<Frame_Class->set_freq_stable_range;
    qDebug()<<"Frame_Class->set_freq_stable_interval = "<<Frame_Class->set_freq_stable_interval;
    qDebug()<<"Frame_Class->set_freq_stable_step = "<<Frame_Class->set_freq_stable_step;
#endif
}
void uart::Frame_Reg_Set(uint32 *freq, uint32 *pulse_width, uint32 *pulse_period, uint32 *alarm_data)
{
    if(*freq != 0)
        Frame_Class->set_address_data = *freq;
    else if(*pulse_width != 0)
        Frame_Class->set_address_data = *pulse_width;
    else if(*pulse_period != 0)
        Frame_Class->set_address_data = *pulse_period;
    else
        Frame_Class->set_address_data = *alarm_data;
#ifndef __MICROWAVE_TEST__
#else
    qDebug()<<"Frame_Class->set_address_data = "<<Frame_Class->set_address_data;
#endif

}

