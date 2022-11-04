#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setIDstarmed(QFontDatabase::addApplicationFont(":/Fonts/starmedfont.ttf"));
    sysParm = new SysParm();
    Outputthread = new OutputThread(this, sysParm);
    Monitoringtask = new MonitoringTask(this, sysParm);
    Setup_Screen = new setup(this, sysParm);
    History_Screen = new history(this, sysParm);

    Data_Class = new Data(this);
    Uart_Class = new uart(this);
    Audio_Output = new AudioOutput(this);
    File_Rw = new File_RW(this, sysParm);
    Starmedlogo_Screen = new starmedlogo(this);
    Sysmessage_Screen = new sysmessage(this);

    Button_Timer = new QTimer(this);
    Display_Timer = new QTimer(this);
    Setup_Screen->close();
    History_Screen->close();
    Sysmessage_Screen->close();

#ifndef _ONPC_
    Outputthread->start();
    Open_Device();
    SecuredIC = new securedic(this, sysParm);
    USBOTG = new usbotg(this);
#endif

    Display_Timer->setInterval(50);
    Display_Timer->start();

    Setting();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Open_Device()
{
#ifndef _ONPC_

#ifdef __FEATURE_MICROWAVE__
#else
    file_RW->FileOpen(File_RW::TYPE_LOGDATA);
#endif
    gpioTools::gpio_init();
    gpioTools::spi_init();
    gpioTools::SecuredIC_Open();
    gpioTools::Volume_Open();

    if(gpioTools::max31856_begin() == true)
        qDebug("Temp max31856 setting Success");
    else
        qDebug("max31856 setting Fail");
#endif
}

void MainWindow::SetValue()
{
    IsPressed = false;
    Track_Mode_State = false;
    Channel_Select_State = 1; //channel ch1
    //Set_System_State = SYSTEM_STANDBY;
    channel_last_state = STATE_CHANNEL_NONE;
    Ch1_Watt_value = 100;
    Ch2_Watt_value = 150;
    Ch1_Time_value = 600;
    Ch2_Time_value = 600;
}
void MainWindow::SetFunction()
{
    Choose_Click_state(CLICK_NONE);
    //Set_UI_Position(STATE_CHANNEL_NONE);
    Set_UI_Position(STATE_CHANNEL_CH1);
}

void MainWindow::Setting()
{

#ifndef _ONPC_
    Setting_RTCVolume();
#endif
#ifdef __FEATURE_MICROWAVE__
#else
    Screen_Hide();
#endif
    SetFont();
    SetImage();
    SetEventFilter();
    Event_connect();
    SetValue();
    SetFunction();
    Set_Track_Mode(false);
    //Ui_Enable();
    Choose_Click_state(CLICK_NONE);

#ifndef _ONPC_
    gpioTools::WriteGPIOPortLow(gpioTools::GPIO_2, 17);
    qDebug() << "Channel1 Start ";//"SPTD High Button Push";
#endif
#if 0
    ui->Mode_button->setText("Pulse Mode");
    ui->Channel_button->setText("Channel 1");
    ui->widget_frame->setStyleSheet("border: 5px solid cyan");
    ui->Mode_button->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,   stop:0 rgba(60, 186, 162, 255), stop:1 rgba(98, 211, 162, 255))");
    ui->Channel_button->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,   stop:0 rgba(60, 255, 50, 50), stop:1 rgba(98, 255, 150, 30))");

    ui->Start_button->setPalette(Ui_SetColor(QPalette::ButtonText, Qt::white, QPalette::Button, Qt::blue));
#endif
    //Mode_state = true;
    //Channel_state = true;
    //Output_state = false;

    //Output_Counting = 0;
    //LogData_Write_Interval = 0;
    sysParm->mRfDeviceFactors->set_power = 0;
    sysParm->mRfDeviceFactors->set_frequency = 2450;
    sysParm->mRfDeviceFactors->set_pulse_width_value = 1;
    sysParm->mRfDeviceFactors->set_pulse_period_value= 1;

    //Set_pulse_width_value = 1;
    //Set_pulse_period_value = 1;
    Audio_Output->playSound(AudioOutput::SND_BOOTING);
    IsPressed = false;
    //ui->PA_ON_button->setFocus();
    Display_Timer->setInterval(DISPLAY_INTERVAL_VALUE); //50
    Display_Timer->start();
}
void MainWindow::SetImage()
{

    ui->image_screen->setPixmap(QPixmap(":/Images/state_default.png"));
    ui->Up_button_label->setPixmap(QPixmap(":/Images/button_up_enable_1.png"));
    ui->Down_button_label->setPixmap(QPixmap(":/Images/button_down_enable_1.png"));
    ui->Setup_button_label->setPixmap(QPixmap(":/Images/button_setup_enable_1.png"));
    ui->Track_button_label->setPixmap(QPixmap(":/Images/button_track_enable.png"));

    ui->Time_tag_label->setPixmap(QPixmap(":/Images/time_tag.png"));
    ui->Watt_tag_label->setPixmap(QPixmap(":/Images/watt_tag.png"));
    ui->History_button_label->setPixmap(QPixmap(":/Images/button_history_enable.png"));
    ui->Multi_Time_Switching_tag_label->setPixmap(QPixmap(":/Images/multi_time_switching.png"));

    ui->Channel_Single_State_label->setPixmap(QPixmap(":/Images/single_1ch_antenna_ready.png"));
    ui->Channel_Multi_State_label->setPixmap(QPixmap(":/Images/multi_2ch_antenna_ready.png"));
    ui->Channel_Single_bar_label->setPixmap(QPixmap(":/Images/single_1ch_bar.png"));
    ui->Channel_Multi_bar_label->setPixmap(QPixmap(":/Images/single_1ch_bar.png"));

    ui->Watt_Single_Symbol_label->setPixmap(QPixmap(":/Images/watt_symbol.png"));
    ui->Time_Single_Symbol_label->setPixmap(QPixmap(":/Images/single_time.png"));
    ui->Watt_Multi_Symbol_label->setPixmap(QPixmap(":/Images/watt_symbol.png"));
    ui->Temp_Single_Symbol_label->setPixmap(QPixmap(":/Images/temp_symbol.png"));
    ui->Temp_Multi_Symbol_label->setPixmap(QPixmap(":/Images/temp_symbol.png"));
    ui->Time_Multi_Symbol_label->setPixmap(QPixmap(":/Images/multi_time.png"));
    ui->Switching_Multi_Symbol_label->setPixmap(QPixmap(":/Images/multi_interval.png"));
    ui->ExTemp_label->setPixmap(QPixmap(":/Images/temp_symbol.png"));

    ui->image_screen->setScaledContents(true);
    ui->Up_button_label->setScaledContents(true);
    ui->Down_button_label->setScaledContents(true);
    ui->Setup_button_label->setScaledContents(true);
    ui->Track_button_label->setScaledContents(true);
    ui->Time_tag_label->setScaledContents(true);
    ui->Watt_tag_label->setScaledContents(true);
    ui->History_button_label->setScaledContents(true);
    ui->Multi_Time_Switching_tag_label->setScaledContents(true);
    ui->Watt_Single_Symbol_label->setScaledContents(true);
    ui->Watt_Multi_Symbol_label->setScaledContents(true);
    ui->Temp_Single_Symbol_label->setScaledContents(true);
    ui->Temp_Multi_Symbol_label->setScaledContents(true);
    ui->ExTemp_label->setScaledContents(true);
    ui->Channel_Single_State_label->setScaledContents(true);
    ui->Channel_Multi_State_label->setScaledContents(true);
    ui->Channel_Single_bar_label->setScaledContents(true);
    ui->Channel_Multi_bar_label->setScaledContents(true);
    ui->Time_Multi_Symbol_label->setScaledContents(true);
    ui->Switching_Multi_Symbol_label->setScaledContents(true);
}

void MainWindow::SetFont()
{
    QString family = QFontDatabase::applicationFontFamilies(id_starmed).at(0);
    QFont label_font(family,35);
    QFont big_title_font(family,50, QFont::Bold);
    QFont button_font(family,20);
    QFont track_button_font(family,15);

    QFont default_label_font(family,20);
    QFont data_label_font(family,45);
    QFont symbol_label_font(family,20);
    QFont unit_label_font(family,50);
    QFont unit_big_label_font(family,40);


    QFont channel_label_font(family,50);

    ui->State_label->setFont(big_title_font);
    ui->Watt_Single_Unit_label->setFont(unit_label_font);
    ui->Watt_Multi_Unit_label->setFont(unit_label_font);
    ui->Time_Single_Unit_label->setFont(unit_label_font);

    ui->Time_Multi_Unit_label->setFont(unit_big_label_font);

    ui->Switching_Interval_Unit_label->setFont(unit_big_label_font);
    ui->Switching_Interval_Data_label->setFont(data_label_font);
    ui->Temp_Single_Data_label->setFont(label_font);
    ui->Temp_Single_Unit_label->setFont(symbol_label_font);
    ui->Temp_Multi_Data_label->setFont(label_font);
    ui->Temp_Multi_Unit_label->setFont(symbol_label_font);
    ui->ExTemp_Data_label->setFont(label_font);
    ui->ExTemp_Unit_label->setFont(symbol_label_font);

    ui->Channel_Select_Button->setFont(button_font);
    ui->Channel_Single_Symbol_label->setFont(label_font);
    ui->Channel_Multi_Symbol_label->setFont(label_font);

    ui->Alert_Button->setFont(button_font);
    ui->Error_Button->setFont(button_font);
    ui->RF_StartStop_Button->setFont(button_font);
    ui->Track_data_label->setFont(track_button_font);
    ui->Track_title_label->setFont(label_font);

    ui->State_label->setAlignment(Qt::AlignCenter);
    ui->Watt_Single_Unit_label->setAlignment(Qt::AlignCenter);
    ui->Time_Single_Unit_label->setAlignment(Qt::AlignCenter);
    ui->Temp_Single_Data_label->setAlignment(Qt::AlignCenter);
    ui->Temp_Single_Unit_label->setAlignment(Qt::AlignCenter);
    ui->Watt_Multi_Unit_label->setAlignment(Qt::AlignCenter);
    ui->Time_Multi_Unit_label->setAlignment(Qt::AlignCenter);
    ui->Temp_Multi_Data_label->setAlignment(Qt::AlignCenter);
    ui->Temp_Multi_Unit_label->setAlignment(Qt::AlignCenter);
    ui->ExTemp_Data_label->setAlignment(Qt::AlignCenter);
    ui->ExTemp_Unit_label->setAlignment(Qt::AlignCenter);
    ui->Track_button_label->setAlignment(Qt::AlignCenter);
    ui->Track_title_label->setAlignment(Qt::AlignCenter);
    ui->Track_data_label->setAlignment(Qt::AlignCenter);
    ui->Channel_Single_Symbol_label->setAlignment(Qt::AlignCenter);
    ui->Channel_Multi_Symbol_label->setAlignment(Qt::AlignCenter);
    ui->Switching_Interval_Unit_label->setAlignment(Qt::AlignCenter);
    ui->Switching_Interval_Data_label->setAlignment(Qt::AlignCenter);


    ui->State_label->setStyleSheet("QLabel { color : white; }"); //text color
    ui->Track_title_label->setStyleSheet("QLabel { color : orange; }"); //text color
    ui->Track_data_label->setStyleSheet("QLabel { color : orange; }"); //text color
}

void MainWindow::Setting_RTCVolume()
{
    QString Date_year, Date_month, Date_day, Time_hour, Time_min, Time_sec;
#ifndef _ONPC_
    gpioTools::MAX9768_Write(gpioTools::OM_FLITERLESS, sysParm->mRfDeviceFactors->Volume*7); //0~63
#endif
    qDebug() << "sysParm->mRfDeviceFactors->Volume = " <<sysParm->mRfDeviceFactors->Volume;
#ifndef _ONPC_
    qDebug("RTC Setting Start");
#if 1
    gpioTools::MAX31343_Write(gpioTools::RTC_REGISTERS_PWR_MGMT, 0x10); //0x10 //0x1c
    gpioTools::MAX31343_Write(gpioTools::RTC_REGISTERS_TRICKLE_REG, 0x50); //0x50 //0x00

    Date_year.sprintf("%x", gpioTools::MAX31343_Read(gpioTools::RTC_REGISTERS_YEARS));
    Date_month.sprintf("%x", gpioTools::MAX31343_Read(gpioTools::RTC_REGISTERS_MONTHS));
    Date_day.sprintf("%x", gpioTools::MAX31343_Read(gpioTools::RTC_REGISTERS_DATES));
    Time_hour.sprintf("%x", gpioTools::MAX31343_Read(gpioTools::RTC_REGISTERS_HOURS));
    Time_min.sprintf("%x", gpioTools::MAX31343_Read(gpioTools::RTC_REGISTERS_MINUTES));
    Time_sec.sprintf("%x", gpioTools::MAX31343_Read(gpioTools::RTC_REGISTERS_SECONDS));

    sysParm->mRfDeviceFactors->Date_year = Date_year.toUInt();
    sysParm->mRfDeviceFactors->Date_month = Date_month.toUInt();
    sysParm->mRfDeviceFactors->Date_day  = Date_day.toUInt();
    sysParm->mRfDeviceFactors->Time_hour = Time_hour.toUInt();
    sysParm->mRfDeviceFactors->Time_min = Time_min.toUInt();
    sysParm->mRfDeviceFactors->Time_sec = Time_sec.toUInt();

    qDebug()<<"Date_year x=" << Date_year ;
    qDebug()<<"Date_month x=" << Date_month ;
    qDebug()<<"Date_day x=" << Date_day ;
    qDebug()<<"Time_hour x=" << Time_hour;
    qDebug()<<"Time_min x=" << Time_min ;
    qDebug()<<"Time_sec x=" << Time_sec ;

    qDebug()<<"Date_year" << sysParm->mRfDeviceFactors->Date_year ;
    qDebug()<<"Date_month" << sysParm->mRfDeviceFactors->Date_month ;
    qDebug()<<"Date_day" << sysParm->mRfDeviceFactors->Date_day ;
    qDebug()<<"Time_hour" << sysParm->mRfDeviceFactors->Time_hour;
    qDebug()<<"Time_min" << sysParm->mRfDeviceFactors->Time_min ;
    qDebug()<<"Time_sec" << sysParm->mRfDeviceFactors->Time_sec ;

    QString time_date = "20"+Date_year+'-'+Date_month+'-'+Date_day+' '+Time_hour+':'+Time_min+':'+Time_sec;
    qDebug() << "timer_date="<<time_date;

    //QDateTime datetimer = QDateTime::fromString("M1d1y9800:01:02", "'M'M'd'd'y'yyhh:mm:ss");
    QString time_format = "yyyy-MM-dd HH:mm:ss";
    //QDateTime a = QDateTime::currentDateTime();

    //QDateTime datetimer = time_date;
    QDateTime datetimer = QDateTime::fromString(time_date, time_format);
    qDebug() << "datetimer = "<<datetimer;
    qDebug() << "datetimer toString()= "<<datetimer.toString();
    qDebug() << "datetimer toUTC() = "<<datetimer.toUTC().toString();
#endif
#endif
}

void MainWindow::SetEventFilter()
{

#ifdef _ONPC_
    ui->Channel_Select_Button->installEventFilter(this);
#endif

    ui->Watt_Single_lcd->installEventFilter(this);
    ui->Watt_Multi_lcd->installEventFilter(this);
    ui->Time_Single_lcd->installEventFilter(this);
    ui->Time_Multi_lcd->installEventFilter(this);
    ui->Setup_button_label->installEventFilter(this);
    ui->History_button_label->installEventFilter(this);
    ui->Error_Button->installEventFilter(this);
    ui->Alert_Button->installEventFilter(this);
    ui->Up_button_label->installEventFilter(this);
    ui->Down_button_label->installEventFilter(this);
    ui->Track_data_label->installEventFilter(this);
    ui->RF_StartStop_Button->installEventFilter(this);
}
void MainWindow::Event_connect()
{
    connect(Button_Timer, SIGNAL(timeout()), this, SLOT(slot_Button_Loop()));
    connect(Display_Timer, SIGNAL(timeout()), this, SLOT(slot_UiDisplay()));
    connect(Data_Class, SIGNAL(sig_FD_Ready_Status_OK()), this, SLOT(slot_FD_Ready_Status_OK()));
    connect(Data_Class, SIGNAL(sig_FD_Ready_Status_NO()), this, SLOT(slot_FD_Ready_Status_NO()));
    connect(Data_Class, SIGNAL(sig_Frame_Number(int)), this, SLOT(slot_Frame_Number(int)));
    connect(Data_Class, SIGNAL(sig_FD_Status_Confirm()), this, SLOT(slot_FD_Status_Confirm()));
    connect(Uart_Class, SIGNAL(sig_data_frameparsing()), this, SLOT(slot_data_frameparsing()));
    connect(Setup_Screen, SIGNAL(sig_Main_Screen()), this, SLOT(slot_Main_Screen()));
    connect(Setup_Screen, SIGNAL(sig_STARwave_Start()), this, SLOT(slot_STARwave_Start()));
    connect(Setup_Screen, SIGNAL(sig_STARwave_Stop()), this, SLOT(slot_STARwave_Stop()));
    connect(History_Screen, SIGNAL(sig_Main_Screen()), this, SLOT(slot_Main_Screen()));
    connect(Sysmessage_Screen, SIGNAL(sig_Main_Screen()), this, SLOT(slot_Main_Screen()));
    connect(this, SIGNAL(sig_Setup_Screen()), Setup_Screen, SLOT(slot_Setup_ScreenIN()));
    connect(this, SIGNAL(sig_History_Screen()), History_Screen, SLOT(slot_History_ScreenIN()));
    connect(this, SIGNAL(sig_alert_state(Alert_State)), Sysmessage_Screen, SLOT(slot_alert_state(Alert_State)));
    connect(this, SIGNAL(sig_error_state(Error_State)), Sysmessage_Screen, SLOT(slot_error_state(Error_State)));

#ifndef _ONPC_
    connect(Monitoringtask, SIGNAL(sig_SecuredIC_Start()), SecuredIC, SLOT(slot_SecuredIC_Start()));
#endif
#if 0
    //<----------------------------------- Foot Switch ------------------------------------>
    connect(pGButton, SIGNAL(sig_btnShortPressed(int)), this, SLOT(slot_MWA_Stop(int)));
    connect(pGButton, SIGNAL(sig_btnLongPressed(int)), this, SLOT(slot_MWA_Start(int)));
#endif
}
void MainWindow::Set_UI(Channel_State state)
{
    switch(state)
    {
        case STATE_CHANNEL_NONE:
            Set_UI_Position(STATE_CHANNEL_NONE);
            Set_System_State(STATE_SYSTEM_STANDBY);
            break;
        case STATE_CHANNEL_CH1:
            Set_UI_Position(STATE_CHANNEL_CH1);
            Set_System_State(STATE_SYSTEM_READY);
            break;
        case STATE_CHANNEL_CH2:
            Set_UI_Position(STATE_CHANNEL_CH2);
            Set_System_State(STATE_SYSTEM_READY);
            break;
        case STATE_CHANNEL_MULTI:
            Set_UI_Position(STATE_CHANNEL_MULTI);
            Set_System_State(STATE_SYSTEM_READY);
            break;
        default :
            break;
    }
}
void MainWindow::display(Data *data)
{
   // qDebug("display");
    uint32 Settime;
    sysParm->tmElapsedTime = static_cast<uint32>(ElapsedTime.elapsed()/1000);
    QString LCD_Data,Label_Data,P_Set_Data;
    QString tempString = "--";
    static bool tmReadTemp_state = false;
#if 0
    qDebug() << "PLL_Freq_Max = " << data->get_PLL_Freq_Max();
#endif
#ifndef _ONPC_

    if(sysParm->mRfDeviceFactors->tmReadTemp_ch1 <= 50 &&
       sysParm->mRfDeviceFactors->tmReadTemp_ch1_state == false)
    {
        sysParm->mRfDeviceFactors->tmReadTemp_ch1_state = true;
        if(sysParm->mRfDeviceFactors->tmReadTemp_ch1_state == true &&
                sysParm->mRfDeviceFactors->tmReadTemp_ch2_state == true  )
        {
            Set_UI(STATE_CHANNEL_MULTI);
        }
        else if(sysParm->mRfDeviceFactors->tmReadTemp_ch1_state == true &&
           sysParm->mRfDeviceFactors->tmReadTemp_ch2_state == false  )
        {
            Set_UI(STATE_CHANNEL_CH1);
        }
    }
    else if( sysParm->mRfDeviceFactors->tmReadTemp_ch1 > 50 &&
             sysParm->mRfDeviceFactors->tmReadTemp_ch1_state == true )
    {
        sysParm->mRfDeviceFactors->tmReadTemp_ch1_state = false;
        if(sysParm->mRfDeviceFactors->tmReadTemp_ch1_state == false &&
           sysParm->mRfDeviceFactors->tmReadTemp_ch2_state == true  )
        {
            Set_UI(STATE_CHANNEL_CH2);
        }
        else if(sysParm->mRfDeviceFactors->tmReadTemp_ch1_state == false &&
                sysParm->mRfDeviceFactors->tmReadTemp_ch2_state == false  )
        {
            Set_UI(STATE_CHANNEL_NONE);
        }
    }

    if(sysParm->mRfDeviceFactors->tmReadTemp_ch2 <= 50 &&
       sysParm->mRfDeviceFactors->tmReadTemp_ch2_state == false)
    {
        sysParm->mRfDeviceFactors->tmReadTemp_ch2_state = true;
        if(sysParm->mRfDeviceFactors->tmReadTemp_ch1_state == true &&
           sysParm->mRfDeviceFactors->tmReadTemp_ch2_state == true  )
        {
            Set_UI(STATE_CHANNEL_MULTI);
        }
        else if(sysParm->mRfDeviceFactors->tmReadTemp_ch1_state == false &&
           sysParm->mRfDeviceFactors->tmReadTemp_ch2_state == true  )
        {
            Set_UI(STATE_CHANNEL_CH2);
        }
    }
    else if( sysParm->mRfDeviceFactors->tmReadTemp_ch2 > 50 &&
             sysParm->mRfDeviceFactors->tmReadTemp_ch2_state == true )
    {
        sysParm->mRfDeviceFactors->tmReadTemp_ch2_state = false;
        if(sysParm->mRfDeviceFactors->tmReadTemp_ch1_state == true &&
           sysParm->mRfDeviceFactors->tmReadTemp_ch2_state == false  )
        {
            Set_UI(STATE_CHANNEL_CH1);
        }
        else if(sysParm->mRfDeviceFactors->tmReadTemp_ch1_state == false &&
                sysParm->mRfDeviceFactors->tmReadTemp_ch2_state == false  )
        {
            Set_UI(STATE_CHANNEL_NONE);
        }
    }

#endif
#if 1
    if(sysParm->mRfDeviceFactors->tmReadTemp_ch1 > 50)
    {
        ui->Temp_Single_Data_label->setText(tempString);
    }
    else
    {
        LCD_Data.sprintf("%d", sysParm->mRfDeviceFactors->tmReadTemp_ch1);
        ui->Temp_Single_Data_label->setText(LCD_Data);
    }
    if(sysParm->mRfDeviceFactors->tmReadTemp_ch2 > 50)
    {
        ui->Temp_Multi_Data_label->setText(tempString);
    }
    else
    {
        LCD_Data.sprintf("%d", sysParm->mRfDeviceFactors->tmReadTemp_ch2);
        ui->Temp_Multi_Data_label->setText(LCD_Data);
    }
#endif
#if 0
    LCD_Data.sprintf("%d", sysParm->mRfDeviceFactors->set_power_value);
    ui->Set_F_Power_lcd->display(LCD_Data);

    Settime = sysParm->tmRFCogluationTime_SetTimer;

    if(Output_state == true)
    {
        Output_Counting ++;
        LogData_Write_Interval++;
        ui->Freq_Search_button->setStyleSheet("background-color: yellow");
        //qDebug() << "Main_Timer = " << Main_Timer;
#ifndef _ONPC_
        gpioTools::USB_uart_printf("count :%d, FW:%d, RW:%d, Freq:%d, T:%d  \r\n",
                                   Output_Counting,
                                   data->get_F_Power(),
                                   int(data->get_R_Power()),
                                   data->get_Frequency(),
                                   sysParm->tmElapsedTime%60);
#endif
        if(LogData_Write_Interval > (DISPLAY_INTERVAL_VALUE/10)) //LogData_Write_Interval : 1000ms
        {
            sysParm->mRfDeviceFactors->rf_F_Watt.append(data->get_F_Power());
            sysParm->mRfDeviceFactors->rf_R_Watt.append(data->get_R_Power());
            sysParm->mRfDeviceFactors->rf_Freq.append(data->get_Frequency());
            sysParm->mRfDeviceFactors->rf_TimeMin.append(sysParm->tmElapsedTime/60);
            sysParm->mRfDeviceFactors->rf_TimeSec.append(sysParm->tmElapsedTime%60);
            sysParm->mRfDeviceFactors->rf_Count.append(Output_Counting);
            LogData_Write_Interval = 0;
        }
        LCD_Data.sprintf(" %02d:%02d ", sysParm->tmElapsedTime/60 , sysParm->tmElapsedTime%60);
        ui->Time_lcd->display(LCD_Data);
        if(sysParm->tmElapsedTime >= Settime)
        {
            Output_state = false;
            //Output_Counting = 0;
#ifdef __FEATURE_MICROWAVE__
#else
            file_RW->FileWrite(0);
#endif
            Ui_Enable();
            Uart_class->Frame_Command(frame::COMMAND_PA_DISABLE);
            ui->Start_button->setText("Start");

            ui->Start_button->setPalette(Ui_SetColor(QPalette::ButtonText, Qt::white, QPalette::Button, Qt::blue));

            //gpioTools::USB_uart_close();
        }
    }
    else
    {
        ui->Freq_Search_button->setStyleSheet("background-color: gray");
    }
    LCD_Data.sprintf(" %02d:%02d ", Settime/60 , Settime%60);
    ui->Set_Time_lcd->display(LCD_Data);
#endif
}

void MainWindow::UDPressed(UPDOWN_Btns_State state)
{
    switch(state)
    {
        case UPDOWN_BTNS_CH1_WATT_UP:
            //qDebug("Watt Up");
            if(Ch1_Watt_value < 210)
            {
               Ch1_Watt_value += 5;
            }
            break;
        case UPDOWN_BTNS_CH1_WATT_DOWN:
            //qDebug("Watt Down");
            if(Ch1_Watt_value > 0)
            {
                Ch1_Watt_value -= 5;
            }
            break;
        case UPDOWN_BTNS_CH2_WATT_UP:
            //qDebug("Watt Up");
            if(Ch2_Watt_value < 210)
            {
               Ch2_Watt_value += 5;
            }
            break;
        case UPDOWN_BTNS_CH2_WATT_DOWN:
            //qDebug("Watt Down");
            if(Ch2_Watt_value > 0)
            {
                Ch2_Watt_value -= 5;
            }
            break;
        case UPDOWN_BTNS_CH1_TIME_UP:
            //qDebug("Time Up");
            if(Ch1_Time_value < 1800)
            {
                Ch1_Time_value += 10;
            }
            break;
        case UPDOWN_BTNS_CH1_TIME_DOWN:
            //qDebug("Time Down");
            if(Ch1_Time_value > 0)
            {
                Ch1_Time_value -= 10;
            }
            break;
        case UPDOWN_BTNS_CH2_TIME_UP:
            //qDebug("Time Up");
            if(Ch2_Time_value < 1800)
            {
                Ch2_Time_value += 10;
            }
            break;
        case UPDOWN_BTNS_CH2_TIME_DOWN:
            //qDebug("Time Down");
            if(Ch2_Time_value > 0)
            {
                Ch2_Time_value -= 10;
            }
            break;

        default :
            break;
    }
}

void MainWindow::Choose_Click_state(Click_State state)
{
    Set_Click_State = state;
    switch(state)
    {
        case CLICK_NONE:
            ui->Watt_Single_line->hide();
            ui->Watt_Multi_line->hide();
            ui->Time_Single_line->hide();
            ui->Time_Multi_line->hide();
            break;
        case CLICK_1CH_WATT:
            ui->Watt_Single_line->show();
            ui->Watt_Multi_line->hide();
            ui->Time_Single_line->hide();
            ui->Time_Multi_line->hide();
            break;
        case CLICK_1CH_TIME:
            ui->Watt_Single_line->hide();
            ui->Watt_Multi_line->hide();
            ui->Time_Single_line->show();
            ui->Time_Multi_line->hide();
            break;
        case CLICK_2CH_WATT:
            ui->Watt_Single_line->hide();
            ui->Watt_Multi_line->show();
            ui->Time_Single_line->hide();
            ui->Time_Multi_line->hide();
            break;
        case CLICK_2CH_TIME:
            ui->Watt_Single_line->hide();
            ui->Watt_Multi_line->hide();
            ui->Time_Single_line->hide();
            ui->Time_Multi_line->show();
            break;
        default:
            break;
   }
}
void MainWindow::frame_sysctrl_set(uint32 freq, double pulse_width, double pulse_period)
{
    uint32 power_value = 0;
    qDebug()<< "frame_set_1" ;
    qDebug()<< "freq = " << freq;
    qDebug()<< "pulse_width = " << pulse_width;
    qDebug()<< "pulse_period = " << pulse_period;
    qDebug()<< "power_value = " << power_value;
    Uart_Class->Frame_Sysctrl_Set(&freq, &pulse_width, &pulse_period, &power_value);
}
void MainWindow::frame_sysctrl_set(uint32 power_value)
{

    uint32 freq = sysParm->mRfDeviceFactors->set_frequency;
    double pulse_width = 1;
    double pulse_period = 1;
    qDebug()<< "frame_set_2" ;
    qDebug()<< "freq = " << freq;
    qDebug()<< "pulse_width = " << pulse_width;
    qDebug()<< "pulse_period = " << pulse_period;

    Uart_Class->Frame_Sysctrl_Set(&freq, &pulse_width, &pulse_period, &power_value);

}
void MainWindow::frame_sysctrl_set()
{
    uint32 power_value;
    uint32 freq = sysParm->mRfDeviceFactors->set_frequency;
    double pulse_width = 1;
    double pulse_period = 1;
    power_value = sysParm->mRfDeviceFactors->set_power;
    qDebug()<< "frame_set_3" ;
    qDebug()<< "freq = " << freq;
    qDebug()<< "pulse_width = " << pulse_width;
    qDebug()<< "pulse_period = " << pulse_period;
    qDebug()<< "power_value = " << power_value;
    Uart_Class->Frame_Sysctrl_Set(&freq, &pulse_width, &pulse_period, &power_value);
}

void MainWindow::frame_freqfind_set(frame::Frequency_Mode_Status state)
{
    bool freq_stable_mode;
#if 0
    uint32 start_freq;
    uint32 end_freq;
    double freq_step;
#endif
    uint32 start_freq;
    uint32 end_freq;
    uint32 freq_step;
    uint32 freq_stable_range;
    uint32 freq_stable_interval;
    uint32 freq_stable_step;
    switch(state)
    {
        case frame::MODE_NONE:
            freq_stable_mode = false;
            start_freq = 0;
            end_freq = 0;
            freq_step = 0;
            freq_stable_range = 0;
            freq_stable_interval= 0;
            freq_stable_step = 0;
            break;
        case frame::MODE_FIND:
            freq_stable_mode = false;
            start_freq = 2400; //Mhz
            end_freq = 2500; //Mhz
            freq_step = 1; //1Mhz
            freq_stable_range = 0;
            freq_stable_interval= 0;
            freq_stable_step = 0;
            break;
        case frame::MODE_STABLE:
            freq_stable_mode = true;
            start_freq = 0;
            end_freq = 0;
            freq_step = 0;
            break;

        default :
            break;
    }
    Uart_Class->Frame_FreqFind_Set(&freq_stable_mode ,&start_freq, &end_freq, &freq_step, &freq_stable_range,
                                   &freq_stable_interval, &freq_stable_step);
}

void MainWindow::frame_reg_write_set(frame::Address_Status address)
{
#ifdef __FEATURE_MICROWAVE__
#elif
    uint32 freq = ui->Sys_Ctrl_Freq_lineEdit->text().toUInt()*1000;
    uint32 pulse_width = (uint32)(ui->Sys_Ctrl_Pulse_Width_lineEdit->text().toDouble()*10000);
    uint32 pulse_period =(uint32)(ui->Sys_Ctrl_Pulse_Period_lineEdit->text().toDouble()*1000);
    uint32 alarm_data;
    switch(address)
    {
        case frame::ADRESS_FREQUENCY:
            pulse_width = 0;
            pulse_period = 0;
            break;
        case frame::ADRESS_PULSE_PERIOD:
            freq = 0;
            pulse_width = 0;
            break;
        case frame::ADRESS_PULSE_WIDTH:
            freq = 0;
            pulse_period = 0;
            break;
        case frame::ADRESS_ALARMVALUE_OVER_POWER_HIGH:
        case frame::ADRESS_ALARMVALUE_OVER_POWER_LOW:
        case frame::ADRESS_ALARMVALUE_OVER_TEMP:
        case frame::ADRESS_ALARMVALUE_VSWR:
        case frame::ADRESS_ALARMTIME_OVER_POWER:
        case frame::ADRESS_ALARMTIME_OVER_TEMP:
        case frame::ADRESS_ALARMTIME_PLL_UNLOCK:
        case frame::ADRESS_ALARMTIME_VSWR:
        case frame::ADRESS_ALARMTIME_LINK_FAIL:
            freq = 0;
            pulse_period = 0;
            pulse_width = 0;
            alarm_data = 10; //overPower_Time test 5s
            break;

        default :
            break;
    }
    Uart_class->Frame_Reg_Set(&freq , &pulse_width , &pulse_period , &alarm_data);
#endif
}
void MainWindow::Search_Start()
{
    qDebug()<<"Search_Start!";
    frame_freqfind_set(frame::MODE_FIND);
    Uart_Class->Frame_Command(frame::COMMAND_FREQ_SEARCH);
    Uart_Class->Frame_Freq_Mode(frame::MODE_FIND);
}
void MainWindow::slot_STARwave_Start()
{
    //Ui_Disable();
    qDebug("slot_STARwave_Start, Real Start");
    frame_sysctrl_set();
    Uart_Class->Frame_Command(frame::COMMAND_PA_ENABLE);
    ElapsedTime.restart();
}

void MainWindow::slot_STARwave_Stop()
{
    //Ui_Enable();
    qDebug("slot_STARwave_Stop, Real Stop");
    Uart_Class->Frame_Command(frame::COMMAND_PA_DISABLE);
}
void MainWindow::slot_data_frameparsing()
{
#ifdef __MICROWAVE_DEBUG__
#else
    qDebug()<< "Data_Frame_Parsing Start" ;
#endif
    if(Data_Class->Frame_Parsing(Uart_Class->read_Data, Uart_Class->read_Data_length) == true)
    {
        sysParm->mRfDeviceFactors->AMP_Connect_State = true;
    }
    else
    {
        sysParm->mRfDeviceFactors->AMP_Connect_State = false;
    }
}

#ifdef __FEATURE_MICROWAVE__
#else
void mainsystem::slot_File_Download()
{
    qDebug()<<"Main File_Download In";
    //Uart_class->DownLoad_Flag = true;
    Uart_class->Frame_Command(frame::COMMAND_REMOTE_START);
    QTimer::singleShot(5*100,Uart_class,SLOT(slot_Frame_write()));

    //frame_reg_write_set(frame::ADRESS_FREQUENCY);
    //Uart_class->Frame_Reg_Address(frame::ADRESS_FREQUENCY);

}
#endif
void MainWindow::UI_Hide()
{
    ui->centralWidget->hide();
#if 0
    ui->bottom_line->hide();
    ui->Channel_Multi_State_label->hide();
    ui->Channel_Select_Button->hide();
    ui->Channel_Single_label->hide();
    ui->Channel_Single_State_label->hide();
    ui->Down_Button->hide();
    ui->ExTemp_Data_label->hide();
    ui->ExTemp_Symbol_label->hide();
    ui->ExTemp_Unit_label->hide();
    ui->midle_line->hide();
    ui->Setup_label->hide();
    ui->side_line->hide();
    ui->State_label->hide();
#endif
}
void MainWindow::UI_Show()
{
    ui->centralWidget->show();
}
void MainWindow::slot_FD_Ready_Status_OK()
{
#ifdef __MICROWAVE_DEBUG__
    qDebug() <<"File DownLoad Ready Status Success";
#endif
    Uart_Class->Frame_Command(frame::COMMAND_REMOTE_DATA);
    Uart_Class->Uart_Timer->setInterval(1000);
    Uart_Class->Uart_Timer->start();
    //QTimer::singleShot(10*100, Uart_class,SLOT(slot_Frame_write()));
}
void MainWindow::slot_Main_Screen()
{
    UI_Show();
    Display_Timer->setInterval(50);
    Display_Timer->start();
}
void MainWindow::slot_FD_Ready_Status_NO()
{
    qDebug() <<"File DownLoad Ready Status Fail";
    Uart_Class->Frame_Command(frame::COMMAND_REMOTE_START);
    QTimer::singleShot(5*100,Uart_Class,SLOT(slot_Frame_write()));
    //frame_reg_write_set(frame::ADRESS_FREQUENCY);
    //Uart_class->Frame_Reg_Address(frame::ADRESS_FREQUENCY);
}
void MainWindow::slot_FD_Status_Confirm()
{
    qDebug() << "In MainSystem slot_FD_Status_Confirm";
    Uart_Class->Uart_Timer->stop();
    Uart_Class->Frame_Command(frame::COMMAND_REMOTE_CONFIRM);
    QTimer::singleShot(5*100,Uart_Class,SLOT(slot_Frame_write()));
}

void MainWindow::slot_Frame_Number(int frame_number)
{
    qDebug() <<" In MainSystem slot_Frame_Number ";
    Uart_Class->Frame_Number((uint32)frame_number);
}
void MainWindow::Set_UI_Position(Channel_State channel_state)
{
    //Choose_Click_state(CLICK_NONE);
    channel_last_state = channel_state;
    switch(channel_state)
    {

#if 1
        case STATE_CHANNEL_CH1:

            ui->Watt_Single_lcd->setGeometry(130,190,201,141);
            ui->Watt_Single_Unit_label->setGeometry(340,240, 81,81);
            ui->Watt_Single_line->setGeometry(190,310, 131,20);
            ui->Watt_Single_Symbol_label->setGeometry(40,230, 81,81);
            ui->Time_Single_lcd->setGeometry(120,430,281,101);
            ui->Time_Single_Unit_label->setGeometry(350,450,81,81);
            ui->Time_Single_line->setGeometry(170,520, 171,20);
            ui->Time_Single_Symbol_label->setGeometry(40,440, 81,81);
            ui->Channel_Single_State_label->setGeometry(700,170,221,321);
            ui->Channel_Single_bar_label->setGeometry(610,485,401,121);
            ui->Channel_Multi_Symbol_label->setGeometry(50,500,121,81);
            ui->Channel_Single_Symbol_label->setGeometry(660,500,121,81);
            ui->Channel_Multi_bar_label->setGeometry(10,480,401,121);
            ui->Temp_Single_Symbol_label->setGeometry(790,505,81,81);
            ui->Temp_Single_Data_label->setGeometry(850,500,81,81);
            ui->Temp_Single_Unit_label->setGeometry(900,510,81,81);
            ui->Up_button_label->setGeometry(460,160,131,141);
            ui->Down_button_label->setGeometry(460,320,131,141);
            ui->Setup_button_label->setGeometry(450,480,151,111);
            ui->Setup_button_label->setPixmap(QPixmap(":/Images/button_setup_enable_1.png"));
            ui->Channel_Single_State_label->setPixmap(QPixmap(":/Images/single_1ch_antenna_ready.png"));
            ui->Channel_Single_bar_label->setPixmap(QPixmap(":/Images/single_1ch_bar.png"));
            ui->Channel_Multi_bar_label->setPixmap(QPixmap(":/Images/single_2ch_bar.png"));
            ui->Watt_Single_Symbol_label->setPixmap(QPixmap(":/Images/single_watt.png"));
            //ui->Time_Single_Symbol_label->setPixmap(QPixmap(":/Images/single_time.png"));
            //ui->Watt_Multi_Symbol_label->setPixmap(QPixmap(":/Images/watt_symbol.png"));

            ui->Watt_Single_Symbol_label->show();
            ui->Watt_Single_lcd->show();
            ui->Watt_Single_Unit_label->show();
            ui->Watt_Multi_Symbol_label->hide();
            ui->Watt_Multi_lcd->hide();
            ui->Watt_Multi_Unit_label->hide();
            //ui->Watt_Multi_line->hide();
            ui->Time_Single_lcd->show();
            ui->Time_Single_Unit_label->show();
            ui->Time_Single_Symbol_label->show();
            //ui->Time_Single_line->hide();

            ui->Time_Multi_lcd->hide();
            ui->Time_Multi_Unit_label->hide();
            ui->Time_Multi_Symbol_label->hide();
            //ui->Time_Multi_line->hide();
            ui->Channel_Multi_State_label->hide();
            ui->Channel_Single_State_label->show();
            ui->Channel_Single_bar_label->show();
            ui->Channel_Multi_bar_label->hide();
            ui->Channel_Single_Symbol_label->show();
            ui->Channel_Multi_Symbol_label->hide();
            ui->Temp_Single_Data_label->show();
            ui->Temp_Single_Symbol_label->show();
            ui->Temp_Single_Unit_label->show();
            ui->Temp_Multi_Data_label->hide();
            ui->Temp_Multi_Symbol_label->hide();
            ui->Temp_Multi_Unit_label->hide();
            ui->Switching_Multi_Symbol_label->hide();
            ui->Switching_Interval_Data_label->hide();
            ui->Switching_Interval_Unit_label->hide();
            ui->Multi_Time_Switching_tag_label->hide();
            ui->Watt_tag_label->show();
            ui->Time_tag_label->show();
            break;
        case STATE_CHANNEL_CH2:
            ui->Watt_Multi_lcd->setGeometry(130,190,201,141);
            ui->Watt_Single_Unit_label->setGeometry(340,240, 81,81);
            ui->Watt_Single_Symbol_label->setGeometry(40,230, 81,81);
            ui->Time_Single_Unit_label->setGeometry(350,450,81,81);
            ui->Time_Multi_lcd->setGeometry(120,430,281,101);
            ui->Watt_Multi_line->setGeometry(190,310, 131,20);
            ui->Watt_Single_line->setGeometry(190,310, 131,20);
            ui->Time_Single_Symbol_label->setGeometry(40,440, 81,81);
            ui->Time_Single_line->setGeometry(220,520, 131,20);
            ui->Time_Multi_line->setGeometry(170,520, 171,20);
            ui->Channel_Single_State_label->setGeometry(700,170,221,321);
            ui->Channel_Single_bar_label->setGeometry(10,485,401,121);
            ui->Channel_Multi_bar_label->setGeometry(610,485,401,121);
            ui->Channel_Multi_Symbol_label->setGeometry(660,500,121,81);
            ui->Channel_Single_Symbol_label->setGeometry(50,500,121,81);
            ui->Temp_Single_Symbol_label->setGeometry(790,505,81,81);
            ui->Temp_Multi_Data_label->setGeometry(850,500,81,81);
            ui->Temp_Single_Unit_label->setGeometry(900,510,81,81);
            ui->Up_button_label->setGeometry(460,160,131,141);
            ui->Down_button_label->setGeometry(460,320,131,141);
            ui->Setup_button_label->setGeometry(450,480,151,111);
            ui->Setup_button_label->setPixmap(QPixmap(":/Images/button_setup_enable_1.png"));
            ui->Channel_Single_State_label->setPixmap(QPixmap(":/Images/single_2ch_antenna_ready.png"));
            ui->Channel_Single_bar_label->setPixmap(QPixmap(":/Images/single_1ch_bar.png"));
            ui->Channel_Multi_bar_label->setPixmap(QPixmap(":/Images/single_2ch_bar.png"));
            ui->Watt_Single_Symbol_label->setPixmap(QPixmap(":/Images/single_watt.png"));
            //ui->Time_Single_Symbol_label->setPixmap(QPixmap(":/Images/single_time.png"));
            ui->Watt_Single_Symbol_label->show();
            ui->Watt_Multi_Symbol_label->hide();
            ui->Watt_Multi_lcd->show();
            ui->Watt_Multi_Unit_label->hide();
            //ui->Watt_Multi_line->hide();
            ui->Watt_Single_lcd->hide();
            ui->Time_Single_lcd->hide();
            ui->Time_Single_Unit_label->show();
            ui->Time_Single_Symbol_label->show();
            //ui->Time_Single_line->hide();
            ui->Time_Multi_lcd->show();
            ui->Time_Multi_Unit_label->hide();
            ui->Time_Multi_Symbol_label->hide();
            //ui->Time_Multi_line->hide();
            ui->Channel_Multi_State_label->hide();
            ui->Channel_Single_State_label->show();
            ui->Channel_Single_bar_label->hide();
            ui->Channel_Multi_bar_label->show();
            ui->Channel_Single_Symbol_label->hide();
            ui->Channel_Multi_Symbol_label->show();
            ui->Temp_Single_Data_label->hide();
            ui->Temp_Single_Symbol_label->show();
            ui->Temp_Single_Unit_label->show();
            ui->Temp_Multi_Data_label->show();
            ui->Temp_Multi_Symbol_label->hide();
            ui->Temp_Multi_Unit_label->hide();
            ui->Switching_Interval_Data_label->hide();
            ui->Switching_Interval_Unit_label->hide();
            ui->Switching_Multi_Symbol_label->hide();
            ui->Multi_Time_Switching_tag_label->hide();
            ui->Watt_tag_label->show();
            ui->Time_tag_label->show();
            break;
#endif
        case STATE_CHANNEL_MULTI:
            ui->Watt_Single_Symbol_label->setGeometry(40,270,81,81);
            ui->Watt_Single_lcd->setGeometry(130,250,201,111);
            ui->Watt_Single_Unit_label->setGeometry(350,280, 81,81);
            ui->Watt_Single_line->setGeometry(190,350, 131,20);
            ui->Time_Single_lcd->setGeometry(160,400,201,61);
            ui->Time_Multi_lcd->setGeometry(310,170,201,61);
            ui->Time_Multi_line->setGeometry(340,220, 131,20);
            ui->Time_Single_Unit_label->setGeometry(350,390,81,81);
            ui->Time_Single_line->setGeometry(220,460, 131,20);
            ui->Watt_Multi_Symbol_label->setGeometry(620,270, 81,81);
            ui->Watt_Multi_lcd->setGeometry(720,250,201,111);
            ui->Watt_Multi_Unit_label->setGeometry(930,280, 81,81);
            ui->Watt_Multi_line->setGeometry(770,350, 131,20);
            ui->Channel_Single_State_label->setGeometry(50,370,311,131);
            ui->Channel_Multi_State_label->setGeometry(660,370,311,131);
            ui->Channel_Single_bar_label->setGeometry(10,485,401,121);
            ui->Channel_Multi_bar_label->setGeometry(620,480,391,121);
            ui->Channel_Multi_Symbol_label->setGeometry(660,500,121,81);
            ui->Channel_Single_Symbol_label->setGeometry(50,500,121,81);
            ui->Temp_Multi_Symbol_label->setGeometry(790,500,81,81);
            ui->Temp_Multi_Data_label->setGeometry(850,500,81,81);
            ui->Temp_Multi_Unit_label->setGeometry(900,510,81,81);
            ui->Temp_Single_Symbol_label->setGeometry(200,500,81,81);
            ui->Temp_Single_Data_label->setGeometry(260,500,81,81);
            ui->Temp_Single_Unit_label->setGeometry(310,510,81,81);
#if 0
            ui->Temp_Multi_Symbol_label->setGeometry(610,370,81,81);
            ui->Temp_Multi_Data_label->setGeometry(860,370,81,81);
            ui->Temp_Multi_Unit_label->setGeometry(930,380,81,81);
#endif
            ui->Up_button_label->setGeometry(460,250,131,111);
            ui->Down_button_label->setGeometry(460,380,131,111);
            ui->Setup_button_label->setGeometry(420,500,201,111);
            ui->Channel_Single_State_label->setPixmap(QPixmap(":/Images/multi_1ch_antenna_ready.png"));
            ui->Channel_Multi_State_label->setPixmap(QPixmap(":/Images/multi_2ch_antenna_ready.png"));
            ui->Watt_Single_Symbol_label->setPixmap(QPixmap(":/Images/watt_symbol.png"));
            ui->Setup_button_label->setPixmap(QPixmap(":/Images/button_setup_enable_2.png"));
            ui->Channel_Single_bar_label->setPixmap(QPixmap(":/Images/multi_1ch_bar.png"));
            ui->Channel_Multi_bar_label->setPixmap(QPixmap(":/Images/multi_2ch_bar.png"));
            ui->Watt_Single_Symbol_label->show();
            ui->Watt_Single_lcd->show();
            ui->Watt_Single_Unit_label->show();
            ui->Watt_Multi_Symbol_label->show();
            ui->Watt_Multi_lcd->show();
            ui->Watt_Multi_Unit_label->show();
            //ui->Watt_Multi_line->hide();
            ui->Time_Single_lcd->hide();
            ui->Time_Single_Unit_label->hide();
            ui->Time_Single_Symbol_label->hide();
            //ui->Time_Single_line->hide();
            ui->Time_Multi_lcd->show();
            ui->Time_Multi_Unit_label->show();
            ui->Time_Multi_Symbol_label->show();
            //ui->Time_Multi_line->hide();
            ui->Channel_Multi_State_label->show();
            ui->Channel_Single_bar_label->show();
            ui->Channel_Multi_bar_label->show();
            ui->Channel_Single_Symbol_label->show();
            ui->Channel_Multi_Symbol_label->show();
            ui->Temp_Single_Data_label->show();
            ui->Temp_Single_Symbol_label->show();
            ui->Temp_Single_Unit_label->show();
            ui->Temp_Multi_Data_label->show();
            ui->Temp_Multi_Symbol_label->show();
            ui->Temp_Multi_Unit_label->show();
            ui->Switching_Multi_Symbol_label->show();
            ui->Switching_Interval_Data_label->show();
            ui->Switching_Interval_Unit_label->show();
            ui->Multi_Time_Switching_tag_label->show();
            ui->Watt_tag_label->hide();
            ui->Time_tag_label->hide();
            break;
#if 0
            case STATE_CHANNEL_SINGLE:
                if(channel_last_state == STATE_CHANNEL_CH1)
                {
                    ui->Watt_Single_lcd->setGeometry(160,240,201,61);
                    ui->Watt_Single_Unit_label->setGeometry(350,250, 81,81);
                    ui->Watt_Single_line->setGeometry(220,300, 131,20);
                    ui->Time_Single_lcd->setGeometry(160,460,201,61);
                    ui->Time_Single_Unit_label->setGeometry(350,470,81,81);
                    ui->Time_Single_line->setGeometry(220,520, 131,20);
                    ui->Channel_Single_label->setGeometry(630,500,111,71);
                    ui->Channel_Single_State_label->setGeometry(700,170,221,321);
                    ui->Temp_Single_Symbol_label->setGeometry(760,500,81,81);
                    ui->Temp_Single_Data_label->setGeometry(850,500,81,81);
                    ui->Temp_Single_Unit_label->setGeometry(910,510,81,81);
                    ui->Up_button_label->setGeometry(460,160,131,141);
                    ui->Down_button_label->setGeometry(460,320,131,141);
                    ui->Setup_button_label->setGeometry(450,480,151,111);
                    ui->Setup_button_label->setPixmap(QPixmap(":/Images/button_setup_enable_1.png"));
                    ui->Channel_Single_State_label->setPixmap(QPixmap(":/Images/single_antenna.png"));
                    ui->Channel_Single_label->setPixmap(QPixmap(":/Images/1ch_symbol.png"));

                    ui->Watt_Single_Symbol_label->hide();
                    ui->Watt_Single_lcd->show();
                    ui->Watt_Single_Unit_label->show();
                    ui->Watt_Multi_Symbol_label->hide();
                    ui->Watt_Multi_lcd->hide();
                    ui->Watt_Multi_Unit_label->hide();
                    ui->Watt_Multi_line->hide();
                    ui->Time_Single_lcd->show();
                    ui->Time_Single_Unit_label->show();
                    ui->Time_Single_line->hide();

                    ui->Time_Multi_lcd->hide();
                    ui->Time_Multi_Unit_label->hide();
                    ui->Time_Multi_line->hide();
                    ui->Channel_Multi_label->hide();
                    ui->Channel_Multi_State_label->hide();
                    ui->Channel_Single_label->show();
                    ui->Channel_Single_State_label->show();
                    ui->Temp_Single_Data_label->show();
                    ui->Temp_Single_Symbol_label->show();
                    ui->Temp_Single_Unit_label->show();
                    ui->Temp_Multi_Data_label->hide();
                    ui->Temp_Multi_Symbol_label->hide();
                    ui->Temp_Multi_Unit_label->hide();

                    ui->Switching_Interval_Data_label->hide();
                    ui->Switching_Interval_Unit_label->hide();
                    ui->Multi_Time_Switching_tag_label->hide();
                    ui->Watt_tag_label->show();
                    ui->Time_tag_label->show();
                }
                else if(channel_last_state == STATE_CHANNEL_CH2)
                {
                    ui->Watt_Multi_lcd->setGeometry(160,240,201,61);
                    ui->Time_Multi_lcd->setGeometry(160,460,201,61);
                    ui->Watt_Multi_line->setGeometry(220,300, 131,20);
                    ui->Watt_Single_Unit_label->setGeometry(350,250, 81,81);
                    ui->Watt_Single_line->setGeometry(220,300, 131,20);

                    ui->Time_Single_Unit_label->setGeometry(350,470,81,81);
                    ui->Time_Single_line->setGeometry(220,520, 131,20);
                    ui->Time_Multi_line->setGeometry(220,520, 131,20);
                    ui->Channel_Single_label->setGeometry(630,500,111,71);
                    ui->Channel_Single_State_label->setGeometry(700,170,221,321);
                    ui->Temp_Single_Symbol_label->setGeometry(760,500,81,81);
                    ui->Temp_Multi_Data_label->setGeometry(850,500,81,81);
                    ui->Temp_Single_Unit_label->setGeometry(910,510,81,81);
                    ui->Up_button_label->setGeometry(460,160,131,141);
                    ui->Down_button_label->setGeometry(460,320,131,141);
                    ui->Setup_button_label->setGeometry(450,480,151,111);
                    ui->Setup_button_label->setPixmap(QPixmap(":/Images/button_setup_enable_1.png"));
                    ui->Channel_Single_State_label->setPixmap(QPixmap(":/Images/single_antenna.png"));
                    ui->Channel_Single_label->setPixmap(QPixmap(":/Images/2ch_symbol.png"));


                    ui->Watt_Single_Symbol_label->hide();
                    ui->Watt_Multi_Symbol_label->hide();
                    ui->Watt_Multi_lcd->show();
                    ui->Watt_Multi_Unit_label->hide();
                    ui->Watt_Multi_line->hide();
                    ui->Watt_Single_lcd->hide();
                    ui->Time_Single_lcd->hide();
                    ui->Time_Single_Unit_label->show();
                    ui->Time_Single_line->hide();

                    ui->Time_Multi_lcd->show();
                    ui->Time_Multi_Unit_label->hide();
                    ui->Time_Multi_line->hide();

                    ui->Channel_Multi_label->hide();
                    ui->Channel_Multi_State_label->hide();
                    ui->Channel_Single_label->show();
                    ui->Channel_Single_State_label->show();
                    ui->Temp_Single_Data_label->hide();
                    ui->Temp_Multi_Data_label->show();
                    ui->Temp_Multi_Symbol_label->hide();
                    ui->Temp_Multi_Unit_label->hide();

                    ui->Switching_Interval_Data_label->hide();
                    ui->Switching_Interval_Unit_label->hide();
                    ui->Multi_Time_Switching_tag_label->hide();
                    ui->Watt_tag_label->show();
                    ui->Time_tag_label->show();
                }


                break;
#endif
            case STATE_CHANNEL_NONE :
                break;

#if 0
        case STATE_CHANNEL_CH1:
            ui->Watt_Single_Symbol_label->setGeometry(80,190, 81,81);
            ui->Watt_Single_lcd->setGeometry(330,200,201,61);
            ui->Watt_Single_Unit_label->setGeometry(570,200, 81,81);
            ui->Watt_Single_line->setGeometry(380,260, 131,20);

            ui->Time_Single_Symbol_label->setGeometry(80,350,81,81);
            ui->Time_Single_lcd->setGeometry(330,360,201,61);
            ui->Time_Single_Unit_label->setGeometry(570,340,81,81);
            ui->Time_Single_line->setGeometry(370,420, 131,20);

            ui->Temp_Single_Symbol_label->setGeometry(900,340,81,81);
            ui->Temp_Single_Data_label->setGeometry(900,420,81,81);
            ui->Temp_Single_Unit_label->setGeometry(950,420,81,81);

            ui->ExTemp_Symbol_label->setGeometry(20,520,81,71);
            ui->ExTemp_Data_label->setGeometry(100,510,91,91);
            ui->ExTemp_Unit_label->setGeometry(170,520,81,81);

            ui->Channel_Single_label->setGeometry(870,110,141,81);
            ui->Channel_Single_State_label->setGeometry(870,190,141,131);
            ui->Up_Button->setGeometry(700,180,121,121);
            ui->Down_Button->setGeometry(700,340,121,121);

            ui->bottom_line->show();
            ui->midle_line->hide();
            ui->Watt_Multi_Symbol_label->hide();
            ui->Watt_Multi_lcd->hide();
            ui->Watt_Multi_Unit_label->hide();
            ui->Watt_Multi_line->hide();
            ui->Time_Multi_Symbol_label->hide();
            ui->Time_Multi_lcd->hide();
            ui->Time_Multi_Unit_label->hide();
            ui->Time_Multi_line->hide();
            ui->Temp_Multi_Symbol_label->hide();
            ui->Temp_Multi_Data_label->hide();
            ui->Temp_Multi_Unit_label->hide();

            ui->Channel_Multi_State_label->hide();
            break;
        case STATE_CHANNEL_CH2:
            ui->Watt_Multi_Symbol_label->setGeometry(80,190, 81,81);
            ui->Watt_Multi_lcd->setGeometry(330,200,201,61);
            ui->Watt_Multi_Unit_label->setGeometry(570,200, 81,81);
            ui->Watt_Multi_line->setGeometry(380,260, 131,20);

            ui->Time_Multi_Symbol_label->setGeometry(80,350,81,81);
            ui->Time_Multi_lcd->setGeometry(330,360,201,61);
            ui->Time_Multi_Unit_label->setGeometry(570,340,81,81);
            ui->Time_Multi_line->setGeometry(370,420, 131,20);

            ui->Temp_Multi_Symbol_label->setGeometry(900,340,81,81);
            ui->Temp_Multi_Data_label->setGeometry(900,420,81,81);
            ui->Temp_Multi_Unit_label->setGeometry(950,420,81,81);

            ui->ExTemp_Symbol_label->setGeometry(20,520,81,71);
            ui->ExTemp_Data_label->setGeometry(100,510,91,91);
            ui->ExTemp_Unit_label->setGeometry(170,520,81,81);

            ui->Channel_Single_label->setGeometry(870,110,141,81);
            ui->Channel_Multi_State_label->setGeometry(870,190,141,131);
            ui->Up_Button->setGeometry(700,180,121,121);
            ui->Down_Button->setGeometry(700,340,121,121);

            ui->bottom_line->show();
            ui->midle_line->hide();
            ui->Watt_Single_Symbol_label->hide();
            ui->Watt_Single_lcd->hide();
            ui->Watt_Single_Unit_label->hide();
            ui->Watt_Single_line->hide();
            ui->Time_Single_Symbol_label->hide();
            ui->Time_Single_lcd->hide();
            ui->Time_Single_Unit_label->hide();
            ui->Time_Single_line->hide();
            ui->Temp_Single_Symbol_label->hide();
            ui->Temp_Single_Data_label->hide();
            ui->Temp_Single_Unit_label->hide();

            ui->Channel_Single_State_label->hide();
            break;

        case STATE_CHANNEL_MULTI:
            ui->ExTemp_Symbol_label->setGeometry(780,10,81,71);
            ui->ExTemp_Data_label->setGeometry(860,0,91,91);
            ui->ExTemp_Unit_label->setGeometry(930,10,81,81);

            ui->Watt_Single_Symbol_label->setGeometry(40,190, 81,81);
            ui->Watt_Single_lcd->setGeometry(130,200,201,61);
            ui->Watt_Single_Unit_label->setGeometry(330,200, 81,81);
            ui->Watt_Single_line->setGeometry(180,260, 131,20);

            ui->Time_Single_Symbol_label->setGeometry(40,350,81,81);
            ui->Time_Single_lcd->setGeometry(130,360,201,61);
            ui->Time_Single_Unit_label->setGeometry(340,350,81,81);
            ui->Time_Single_line->setGeometry(170,420, 131,20);


            ui->Watt_Multi_Symbol_label->setGeometry(480,190,81,81);
            ui->Watt_Multi_lcd->setGeometry(570,200,201,61);
            ui->Watt_Multi_Unit_label->setGeometry(780,200,81,81);
            ui->Watt_Multi_line->setGeometry(620,260,131,20);

            ui->Time_Multi_Symbol_label->setGeometry(480,350,81,81);
            ui->Time_Multi_lcd->setGeometry(570,360,201,61);
            ui->Time_Multi_Unit_label->setGeometry(780,360,81,81);
            ui->Time_Multi_line->setGeometry(610,420,131,20);

            ui->Channel_Single_State_label->setGeometry(120,100,201,91);
            ui->Channel_Multi_State_label->setGeometry(550,100,201,91);

            ui->Temp_Single_Symbol_label->setGeometry(80,440,81,81);
            ui->Temp_Single_Data_label->setGeometry(240,440,81,81);
            ui->Temp_Single_Unit_label->setGeometry(290,440,81,81);
            ui->Temp_Multi_Symbol_label->setGeometry(480,440,81,81);
            ui->Temp_Multi_Data_label->setGeometry(640,440,81,81);
            ui->Temp_Multi_Unit_label->setGeometry(690,440,81,81);


            ui->Up_Button->setGeometry(880,180,121,121);
            ui->Down_Button->setGeometry(880,340,121,121);

            ui->bottom_line->hide();
            ui->midle_line->show();
            ui->Watt_Single_Symbol_label->show();
            ui->Watt_Single_lcd->show();
            ui->Watt_Single_Unit_label->show();
            ui->Watt_Single_line->show();
            ui->Time_Single_Symbol_label->show();
            ui->Time_Single_lcd->show();
            ui->Time_Single_Unit_label->show();
            ui->Time_Single_line->show();
            ui->Temp_Single_Symbol_label->show();
            ui->Temp_Single_Data_label->show();
            ui->Temp_Single_Unit_label->show();



            ui->Watt_Multi_Symbol_label->show();
            ui->Watt_Multi_lcd->show();
            ui->Watt_Multi_Unit_label->show();
            //ui->Watt_Multi_line->hide();
            ui->Time_Multi_Symbol_label->show();
            ui->Time_Multi_lcd->show();
            ui->Time_Multi_Unit_label->show();
            //ui->Time_Multi_line->hide();

            ui->Temp_Multi_Symbol_label->show();
            ui->Temp_Multi_Data_label->show();
            ui->Temp_Multi_Unit_label->show();

            ui->Channel_Multi_State_label->show();
            ui->Channel_Single_State_label->show();
            break;
        case STATE_CHANNEL_NONE:
            if(channel_last_state == STATE_CHANNEL_CH1)
            {
                ui->Watt_Single_Symbol_label->setGeometry(80,190, 81,81);
                ui->Watt_Single_lcd->setGeometry(330,200,201,61);
                ui->Watt_Single_Unit_label->setGeometry(570,200, 81,81);
                ui->Watt_Single_line->setGeometry(380,260, 131,20);

                ui->Time_Single_Symbol_label->setGeometry(80,350,81,81);
                ui->Time_Single_lcd->setGeometry(330,360,201,61);
                ui->Time_Single_Unit_label->setGeometry(570,340,81,81);
                ui->Time_Single_line->setGeometry(370,420, 131,20);

                ui->Temp_Single_Symbol_label->setGeometry(900,340,81,81);
                ui->Temp_Single_Data_label->setGeometry(900,420,81,81);
                ui->Temp_Single_Unit_label->setGeometry(950,420,81,81);

                ui->ExTemp_Symbol_label->setGeometry(20,520,81,71);
                ui->ExTemp_Data_label->setGeometry(100,510,91,91);
                ui->ExTemp_Unit_label->setGeometry(170,520,81,81);

                ui->Channel_Single_label->setGeometry(870,110,141,81);
                ui->Channel_Single_State_label->setGeometry(870,190,141,131);
                ui->Up_Button->setGeometry(700,180,121,121);
                ui->Down_Button->setGeometry(700,340,121,121);

                ui->bottom_line->show();
                ui->midle_line->hide();
                ui->Watt_Multi_Symbol_label->hide();
                ui->Watt_Multi_lcd->hide();
                ui->Watt_Multi_Unit_label->hide();
                ui->Watt_Multi_line->hide();
                ui->Time_Multi_Symbol_label->hide();
                ui->Time_Multi_lcd->hide();
                ui->Time_Multi_Unit_label->hide();
                ui->Time_Multi_line->hide();
                ui->Temp_Multi_Symbol_label->hide();
                ui->Temp_Multi_Data_label->hide();
                ui->Temp_Multi_Unit_label->hide();

                ui->Channel_Multi_State_label->hide();
            }
            else if(channel_last_state == STATE_CHANNEL_CH2)
            {
                ui->Watt_Multi_Symbol_label->setGeometry(80,190, 81,81);
                ui->Watt_Multi_lcd->setGeometry(330,200,201,61);
                ui->Watt_Multi_Unit_label->setGeometry(570,200, 81,81);
                ui->Watt_Multi_line->setGeometry(380,260, 131,20);

                ui->Time_Multi_Symbol_label->setGeometry(80,350,81,81);
                ui->Time_Multi_lcd->setGeometry(330,360,201,61);
                ui->Time_Multi_Unit_label->setGeometry(570,340,81,81);
                ui->Time_Multi_line->setGeometry(370,420, 131,20);

                ui->Temp_Multi_Symbol_label->setGeometry(900,340,81,81);
                ui->Temp_Multi_Data_label->setGeometry(900,420,81,81);
                ui->Temp_Multi_Unit_label->setGeometry(950,420,81,81);

                ui->ExTemp_Symbol_label->setGeometry(20,520,81,71);
                ui->ExTemp_Data_label->setGeometry(100,510,91,91);
                ui->ExTemp_Unit_label->setGeometry(170,520,81,81);

                ui->Channel_Single_label->setGeometry(870,110,141,81);
                ui->Channel_Multi_State_label->setGeometry(870,190,141,131);
                ui->Up_Button->setGeometry(700,180,121,121);
                ui->Down_Button->setGeometry(700,340,121,121);

                ui->bottom_line->show();
                ui->midle_line->hide();
                ui->Watt_Single_Symbol_label->hide();
                ui->Watt_Single_lcd->hide();
                ui->Watt_Single_Unit_label->hide();
                ui->Watt_Single_line->hide();
                ui->Time_Single_Symbol_label->hide();
                ui->Time_Single_lcd->hide();
                ui->Time_Single_Unit_label->hide();
                ui->Time_Single_line->hide();
                ui->Temp_Single_Symbol_label->hide();
                ui->Temp_Single_Data_label->hide();
                ui->Temp_Single_Unit_label->hide();

                ui->Channel_Single_State_label->hide();
            }

            break;
#endif

        default :

            break;
    }
}
void MainWindow::Set_System_State(State_EN state)
{
    sysParm->mRfDeviceFactors->state_system = state;
    switch(state)
    {
        case STATE_SYSTEM_STANDBY:
            ui->State_label->setText("Standby");
            break;
        case STATE_SYSTEM_READY:
            ui->State_label->setText("Ready");
            break;
        case STATE_SYSTEM_ACTIVE:
            ui->State_label->setText("Active");
            break;

        default :
            break;
    }
}
void MainWindow::Set_Track_Mode(bool state)
{
    if(state == true)
    {
        ui->Track_title_label->show();
    }
    else
    {
        ui->Track_title_label->hide();
    }
}

void MainWindow::Set_Get_Data()
{
    sysParm->mRfDeviceFactors->amp_temperature = Data_Class->get_Temperature();
    sysParm->mRfDeviceFactors->get_power = Data_Class->get_F_Power();
    sysParm->mRfDeviceFactors->get_frequency = Data_Class->get_Frequency();
    display(Data_Class);
}
void MainWindow::Set_LCDDisplay()
{
    QString LCD_Data;
    LCD_Data.sprintf("%d", Data_Class->get_Temperature());
    if(sysParm->mRfDeviceFactors->state_system == STATE_SYSTEM_STANDBY)
    {
        if(channel_last_state == STATE_CHANNEL_CH1)
        {
            LCD_Data.sprintf("%d", Ch1_Watt_value);
            ui->Watt_Single_lcd->display(LCD_Data);
            LCD_Data.sprintf(" %02d:%02d ", Ch1_Time_value/60 , Ch1_Time_value%60);
            ui->Time_Single_lcd->display(LCD_Data);
        }
        else if(channel_last_state == STATE_CHANNEL_CH2)
        {
            LCD_Data.sprintf("%d", Ch2_Watt_value);
            ui->Watt_Multi_lcd->display(LCD_Data);
            LCD_Data.sprintf(" %02d:%02d ", Ch2_Time_value/60 , Ch2_Time_value%60);
            ui->Time_Multi_lcd->display(LCD_Data);
        }
    }
    else
    {
        LCD_Data.sprintf("%d", Ch1_Watt_value);
        ui->Watt_Single_lcd->display(LCD_Data);
        LCD_Data.sprintf("%d", Ch2_Watt_value);
        ui->Watt_Multi_lcd->display(LCD_Data);
        LCD_Data.sprintf(" %02d:%02d ", Ch1_Time_value/60 , Ch1_Time_value%60);
        ui->Time_Single_lcd->display(LCD_Data);
        LCD_Data.sprintf(" %02d:%02d ", Ch2_Time_value/60 , Ch2_Time_value%60);
        ui->Time_Multi_lcd->display(LCD_Data);
    }
}

void MainWindow::Set_TempDisplay(Channel_State state)
{
    QString tempString = "--";
    QString LCD_Data;
    switch(state)
    {
        case STATE_CHANNEL_CH1:
            LCD_Data.sprintf("%d", sysParm->mRfDeviceFactors->tmReadTemp_ch1);
            ui->Temp_Single_Data_label->setText(LCD_Data);
            break;
        case STATE_CHANNEL_CH2:
            LCD_Data.sprintf("%d", sysParm->mRfDeviceFactors->tmReadTemp_ch2);
            ui->Temp_Multi_Data_label->setText(LCD_Data);
            break;
        case STATE_CHANNEL_MULTI:
            LCD_Data.sprintf("%d", sysParm->mRfDeviceFactors->tmReadTemp_ch1);
            ui->Temp_Single_Data_label->setText(LCD_Data);
            LCD_Data.sprintf("%d", sysParm->mRfDeviceFactors->tmReadTemp_ch2);
            ui->Temp_Multi_Data_label->setText(LCD_Data);
            break;
        case STATE_CHANNEL_NONE:
            ui->Temp_Single_Data_label->setText(tempString);
            ui->Temp_Multi_Data_label->setText(tempString);
            break;
        default :
            break;
    }
}
void MainWindow::slot_UiDisplay()
{

    Set_Get_Data();
    Set_LCDDisplay();
}

void MainWindow::slot_Button_Loop()
{
    if((Label == ui->Up_button_label)&&(ui->Up_button_label->isEnabled()))
    {
        if(Button_Counting >= 5) //10ms
        {

            if(Set_Click_State == CLICK_1CH_WATT)
            {
                UDPressed(UPDOWN_BTNS_CH1_WATT_UP);
            }
            else if(Set_Click_State == CLICK_2CH_WATT)
            {
                UDPressed(UPDOWN_BTNS_CH2_WATT_UP);
            }
            else if(Set_Click_State == CLICK_1CH_TIME)
            {
                UDPressed(UPDOWN_BTNS_CH1_TIME_UP);
            }
            else if(Set_Click_State == CLICK_2CH_TIME)
            {
                UDPressed(UPDOWN_BTNS_CH2_TIME_UP);
            }
            Button_Counting = 0;
        }
    }
    else if((Label == ui->Down_button_label)&&(ui->Down_button_label->isEnabled()))
    {
        if(Button_Counting >= 5) //10ms
        {

            if(Set_Click_State == CLICK_1CH_WATT)
            {
                UDPressed(UPDOWN_BTNS_CH1_WATT_DOWN);
            }
            else if(Set_Click_State == CLICK_2CH_WATT)
            {
                UDPressed(UPDOWN_BTNS_CH2_WATT_DOWN);
            }
            else if(Set_Click_State == CLICK_1CH_TIME)
            {
                UDPressed(UPDOWN_BTNS_CH1_TIME_DOWN);
            }
            else if(Set_Click_State == CLICK_2CH_TIME)
            {
                UDPressed(UPDOWN_BTNS_CH2_TIME_DOWN);
            }
            Button_Counting = 0;
        }
    }
    else if(Label == ui->History_button_label)
    {
        if(Button_Counting >= 1) //10ms
        {
            qDebug("history_button_label_click");

            Audio_Output->playSound(AudioOutput::SND_CLICK_BUTTON);
            UI_Hide();
            Display_Timer->stop();
            Button_Timer->stop();
            emit sig_History_Screen();

        }
    }
    else if(PressedButton == ui->Channel_Select_Button)
    {
        if(Button_Counting >= 1) //10ms
        {

            if(Channel_Select_State == 0)
            {
                qDebug("Channel Select CH1");
                Set_UI_Position(STATE_CHANNEL_CH1);
            }
            else if(Channel_Select_State == 1)
            {
                qDebug("Channel Select CH2");
                Set_UI_Position(STATE_CHANNEL_CH2);
            }
            else if(Channel_Select_State == 2)
            {
                qDebug("Channel Select Multi");
                Set_UI_Position(STATE_CHANNEL_MULTI);
            }
            Channel_Select_State += 1; //channel change
            if(Channel_Select_State >= 3)
            {
                Channel_Select_State = 0;
            }
            Button_Timer->stop();
        }
    }
    else if(PressedButton == ui->Error_Button)
    {
        if(Button_Counting >= 1) //10ms
        {
            qDebug("Error Event label_click");

            Audio_Output->playSound(AudioOutput::SND_ERROR);
            //UI_Hide();
            Display_Timer->stop();
            Button_Timer->stop();
            emit sig_error_state(ERROR_READY_ACTIVE_AMP_OVER_TEMPERATURE);

        }
    }
    else if(PressedButton == ui->Alert_Button)
    {
        if(Button_Counting >= 1) //10ms
        {
            qDebug("Alert Event label_click");

            Audio_Output->playSound(AudioOutput::SND_ALERT);
            //UI_Hide();
            Display_Timer->stop();
            Button_Timer->stop();
            emit sig_alert_state(ALARM_ACTIVE_ANTENNA_NOT_CONNECT);

        }
    }
    else if(PressedButton == ui->RF_StartStop_Button)
    {
        if(Button_Counting >= 1) //10ms
        {
            qDebug("RF_Start/Stop Button_click");

            //Audio_Output->playSound(AudioOutput::SND_ALERT);
            //UI_Hide();
            Display_Timer->stop();
            Button_Timer->stop();


        }
    }

#if 0
    else if(Label == ui->setup_label &&
           (sysParm->mRfDeviceFactors->state_system == STATE_SYSTEM_STANDBY ||
            sysParm->mRfDeviceFactors->state_system == STATE_SYSTEM_READY))
#endif
    else if(Label == ui->Setup_button_label)
    {
        if(Button_Counting > 1) //10ms
        {
            qDebug("setup_button_label_click");

            Audio_Output->playSound(AudioOutput::SND_CLICK_BUTTON);
            UI_Hide();
            Display_Timer->stop();
            Button_Timer->stop();
            emit sig_Setup_Screen();
        }
    }
    else if(Label == ui->Track_data_label)
    {
        if(Button_Counting > 1) //10ms
        {
            qDebug("track_button_label_click");

            Audio_Output->playSound(AudioOutput::SND_CLICK_BUTTON);
            Track_Mode_State ^= true;
            Set_Track_Mode(Track_Mode_State);
#if 0
            UI_Hide();
            Display_Timer->stop();
            Button_Timer->stop();
            emit sig_Setup_Screen();
#endif
            Button_Timer->stop();
        }
    }

    else if(LCDNumber == ui->Watt_Single_lcd)
    {
        if(Button_Counting > 1) //10ms
        {

            Choose_Click_state(CLICK_1CH_WATT);
            Button_Timer->stop();
        }
    }
    else if(LCDNumber == ui->Time_Single_lcd)
    {
        if(Button_Counting > 1) //10ms
        {

            Choose_Click_state(CLICK_1CH_TIME);
            Button_Timer->stop();
        }
    }
    else if(LCDNumber == ui->Watt_Multi_lcd)
    {
        if(Button_Counting > 1) //10ms
        {

            Choose_Click_state(CLICK_2CH_WATT);
            Button_Timer->stop();
        }
    }
    else if(LCDNumber == ui->Time_Multi_lcd)
    {
        if(Button_Counting > 1) //10ms
        {

            Choose_Click_state(CLICK_2CH_TIME);
            Button_Timer->stop();
        }
    }

#if 0
    else if(PressedButton == ui->Start_button)
    {
        if(Button_Counting >= 1) //10ms
        {
            Button_Timer->stop();


        }

    }
#endif
    Button_Counting++; //10ms
    //qDebug()<<"Button Counting = "<<Button_Counting;
}


bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        if(IsPressed == false)
        {
            PressedButton = (QPushButton *)target;
            LCDNumber =(QLCDNumber *)target;
            Label = (QLabel *)target;
            IsPressed = true;
            Button_Counting = 0;
            Button_Timer->setInterval(10);
            Button_Timer->start();
           // DisplayTimer->stop();
            qDebug("Push Button");
        }
        if(Button_Counting > 5)
        {
            PressedButton = nullptr;
        }
    }
    else if(event->type() == QEvent::MouseButtonRelease)
    {
        if(IsPressed == true)
        {
            PressedButton = nullptr;
            LCDNumber = nullptr;
            Label = nullptr;
            IsPressed = false;
            Button_Timer->stop();
            qDebug("Release Button");
        }
    }
   // qDebug()<<"event->type() = "<<event->type();
    return QWidget::eventFilter(target, event);
}

