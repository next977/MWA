#include "calibration.h"
#include "ui_calibration.h"

calibration::calibration(QWidget *parent, SysParm *sysParm) :
    QWidget(parent),
    ui(new Ui::calibration)
{
    ui->setupUi(this);
    this->sysParm = sysParm;

    Button_Timer = new QTimer(this);
    DisplayTimer = new QTimer(this);
    DisplayTimer->setInterval(50);
    this->setIDstarmed(QFontDatabase::addApplicationFont(":/Fonts/starmedfont.ttf"));
    IsPressed = false;
    Output_state = false;
    SetFont();
    SetImage();
    Calibration_connect();
    installFilter();

}

calibration::~calibration()
{
    delete ui;
}

void calibration::SetFont()
{
    QString family = QFontDatabase::applicationFontFamilies(id_starmed).at(0);
    QFont label_font(family,25);
    QFont symbol_font(family,20);
    QFont serial_label_font(family,20);
    QFont uart_read_label_font(family,20);

    QFont button_font(family,25);
    QFont main_font(family,30);
    QFont title_font(family,35);

    ui->Modelabel->setFont(main_font);
    ui->Display_Title_label->setFont(title_font);
    ui->Setting_Title_label->setFont(title_font);
    ui->Power_Display_Title_label->setFont(label_font);
    ui->Power_Display_data_label->setFont(label_font);
    ui->Frequency_Display_Title_label->setFont(label_font);
    ui->Frequency_Display_data_label->setFont(label_font);
    ui->InternalTemp_Display_title_label->setFont(label_font);
    ui->InternalTemp_Display_data_label->setFont(label_font);
    ui->Power_Setting_Title_label->setFont(label_font);
    ui->Frequency_Setting_Title_label->setFont(label_font);
    ui->AMP_Connect_State_label->setFont(label_font);


    ui->Start_button->setFont(button_font);
    ui->Start_button->setPalette(Ui_SetColor(QPalette::ButtonText, Qt::white, QPalette::Button, Qt::blue));
    ui->Start_button->setText("Start");
}

void calibration::Choose_Click_state(Click_State state)
{
    sysParm->mRfDeviceFactors->mode_click_state = state;
    switch(state)
    {
        case CLICK_NONE:
            ui->Power_line->hide();
            ui->Frequency_line->hide();
            break;
        case CLICK_CALIBRATION_POWER_SETTING :
            ui->Power_line->show();
            ui->Frequency_line->hide();
            break;
        case CLICK_CALIBRATION_FREQUENCY_SETTING :
            ui->Power_line->hide();
            ui->Frequency_line->show();
            break;

        default:
            break;
   }
}

void calibration::Calibration_connect()
{
      connect(DisplayTimer,SIGNAL(timeout()), this, SLOT(slot_display()));
      connect(Button_Timer, SIGNAL(timeout()), this, SLOT(slot_Button_Loop()));


}

void calibration::slot_ScreenIn_Calibration()
{
    qDebug("Calibration Screen In");
    IsPressed = false;
    Output_state = false;
    //UI_Show();
    this->show();
    DisplayTimer->start();
    Choose_Click_state(CLICK_NONE);
}

void calibration::slot_display()
{

    QString LCD_Data, Label_Data;
    LCD_Data.sprintf("%d", sysParm->mRfDeviceFactors->set_power);
    ui->Power_Setting_lcd->display(LCD_Data);
    LCD_Data.sprintf("%d", sysParm->mRfDeviceFactors->set_frequency);
    ui->Frequency_Setting_lcd->display(LCD_Data);
    Label_Data.sprintf("%d", sysParm->mRfDeviceFactors->get_power);
    ui->Power_Display_data_label->setText(Label_Data);
    Label_Data.sprintf("%d", sysParm->mRfDeviceFactors->get_frequency);
    ui->Frequency_Display_data_label->setText(Label_Data);
    Label_Data.sprintf("%d", sysParm->mRfDeviceFactors->amp_temperature);
    ui->InternalTemp_Display_data_label->setText(Label_Data);

    if(sysParm->mRfDeviceFactors->AMP_Connect_State == true)
    {
        ui->AMP_Connect_State_Color_label->setPalette(Ui_SetColor(QPalette::WindowText, Qt::white, QPalette::Background, Qt::green));
    }
    else
    {
        ui->AMP_Connect_State_Color_label->setPalette(Ui_SetColor(QPalette::WindowText, Qt::white, QPalette::Background, Qt::red));
    }
}

QPalette calibration::Ui_SetColor(QPalette::ColorRole Ui_Text, Qt::GlobalColor Text_color,
                              QPalette::ColorRole Ui, Qt::GlobalColor color)
{
    QPalette Ui_Color = palette();
    Ui_Color.setBrush(Ui_Text, Text_color);
    Ui_Color.setBrush(Ui, color);
    return Ui_Color;
}




void calibration::UI_Hide()
{
    ui->back_label->hide();
    ui->up_button_label->hide();
    ui->down_button_label->hide();
    ui->Modelabel->hide();

    ui->Display_Title_label->hide();
    ui->Setting_Title_label->hide();
    ui->Power_Display_Title_label->hide();
    ui->Power_Display_data_label->hide();
    ui->Frequency_Display_Title_label->hide();
    ui->Frequency_Display_data_label->hide();
    ui->InternalTemp_Display_title_label->hide();
    ui->InternalTemp_Display_data_label->hide();
    ui->Power_Setting_Title_label->hide();
    ui->Frequency_Setting_Title_label->hide();
    ui->Power_line->hide();
    ui->Frequency_line->hide();
    ui->Start_button->hide();

}

void calibration::UI_Show()
{
    ui->back_label->show();
    ui->up_button_label->show();
    ui->down_button_label->show();
    ui->Modelabel->show();
    ui->Display_Title_label->show();
    ui->Setting_Title_label->show();
    ui->Power_Display_Title_label->show();
    ui->Power_Display_data_label->show();
    ui->Frequency_Display_Title_label->show();
    ui->Frequency_Display_data_label->show();
    ui->InternalTemp_Display_title_label->show();
    ui->InternalTemp_Display_data_label->show();
    ui->Power_Setting_Title_label->show();
    ui->Frequency_Setting_Title_label->show();
    ui->Start_button->show();
}

void calibration::SetImage()
{
    ui->back_label->setPixmap(QPixmap(":/Images/button_back_enable.png"));
    ui->up_button_label->setPixmap(QPixmap(":/Images/button_up_enable_2.png"));
    ui->down_button_label->setPixmap(QPixmap(":/Images/button_down_enable_2.png"));

    ui->back_label->setScaledContents(true);
    ui->up_button_label->setScaledContents(true);
    ui->down_button_label->setScaledContents(true);

}
void calibration::UDPressed(UPDOWN_Btns_State state)
{
    switch(state)
    {
        case UPDOWN_BTNS_CAL_POWER_UP:
            //qDebug("Watt Up");
            if(sysParm->mRfDeviceFactors->set_power < 210)
            {
               sysParm->mRfDeviceFactors->set_power += 5;
            }
            break;
        case UPDOWN_BTNS_CAL_POWER_DOWN:
            //qDebug("Watt Down");
            if(sysParm->mRfDeviceFactors->set_power > 0)
            {
                sysParm->mRfDeviceFactors->set_power -= 5;
            }
            break;
        case UPDOWN_BTNS_CAL_FREQUENCY_UP:
            //qDebug("Watt Up");
            if(sysParm->mRfDeviceFactors->set_frequency < 2500)
            {
               sysParm->mRfDeviceFactors->set_frequency += 10;
            }
            break;
        case UPDOWN_BTNS_CAL_FREQUENCY_DOWN:
            //qDebug("Watt Down");
            if(sysParm->mRfDeviceFactors->set_frequency > 2400)
            {
                sysParm->mRfDeviceFactors->set_frequency -= 10;
            }
            break;


        default :
            break;
    }
}


void calibration::installFilter()
{

    ui->back_label->installEventFilter(this);
    ui->up_button_label->installEventFilter(this);
    ui->down_button_label->installEventFilter(this);
    ui->Modelabel->installEventFilter(this);
    ui->Frequency_Setting_lcd->installEventFilter(this);
    ui->Power_Setting_lcd->installEventFilter(this);
    ui->Start_button->installEventFilter(this);

}

void calibration::slot_Button_Loop()
{
        if(Label == ui->back_label )
        {
            if(Button_Counting > 1) //10ms
            {

                DisplayTimer->stop();
                //emit sig_setting();
                emit sig_ScreenIn_Setup();
                this->close();
                Button_Timer->stop();
            }
        }
        else if((Label == ui->up_button_label)&&(ui->up_button_label->isEnabled()))
        {
            if(Button_Counting > 3) //30ms
            {
                if(sysParm->mRfDeviceFactors->mode_click_state ==
                   CLICK_CALIBRATION_POWER_SETTING )
                {
                    UDPressed(UPDOWN_BTNS_CAL_POWER_UP);
                }
                else if(sysParm->mRfDeviceFactors->mode_click_state ==
                        CLICK_CALIBRATION_FREQUENCY_SETTING)
                {
                    UDPressed(UPDOWN_BTNS_CAL_FREQUENCY_UP);
                }
                Button_Counting = 0;
            }
        }
        else if((Label == ui->down_button_label)&&(ui->down_button_label->isEnabled()))
        {
            if(Button_Counting > 3) //30ms
            {
                if(sysParm->mRfDeviceFactors->mode_click_state ==
                   CLICK_CALIBRATION_POWER_SETTING )
                {
                    UDPressed(UPDOWN_BTNS_CAL_POWER_DOWN);
                }
                else if(sysParm->mRfDeviceFactors->mode_click_state ==
                        CLICK_CALIBRATION_FREQUENCY_SETTING)
                {
                    UDPressed(UPDOWN_BTNS_CAL_FREQUENCY_DOWN);
                }
                Button_Counting = 0;
            }
        }
        else if(LCDNumber == ui->Power_Setting_lcd)
        {
            if(Button_Counting > 1) //10ms
            {
                Choose_Click_state(CLICK_CALIBRATION_POWER_SETTING);
                Button_Timer->stop();
            }
        }
        else if(LCDNumber == ui->Frequency_Setting_lcd)
        {
            if(Button_Counting > 1) //10ms
            {
                Choose_Click_state(CLICK_CALIBRATION_FREQUENCY_SETTING);
                Button_Timer->stop();
            }
        }
        else if(PressedButton == ui->Start_button)
        {
            if(Button_Counting >= 1) //10ms
            {
                Button_Timer->stop();

                Output_state ^= true;

                if(Output_state == true)
                {
                    qDebug() << "Start State";

                    //slot_MWA_Start();
                    emit sig_STARwave_Start();
                    //Uart_class->Frame_Command(frame::COMMAND_PA_CONTROL);
                    ui->Start_button->setPalette(Ui_SetColor(QPalette::ButtonText, Qt::white, QPalette::Button, Qt::red));
                    ui->Start_button->setText("Stop");
#if 0
                    sysParm->mRfDeviceFactors->rf_F_Watt.clear();
                    sysParm->mRfDeviceFactors->rf_R_Watt.clear();
                    sysParm->mRfDeviceFactors->rf_Freq.clear();
                    sysParm->mRfDeviceFactors->rf_TimeMin.clear();
                    sysParm->mRfDeviceFactors->rf_TimeSec.clear();
                    sysParm->mRfDeviceFactors->rf_Count.clear();
#endif

                }
                else
                {
                    qDebug() << "Stop State";
                    //slot_MWA_Stop();
                    emit sig_STARwave_Stop();
                    ui->Start_button->setPalette(Ui_SetColor(QPalette::ButtonText, Qt::white, QPalette::Button, Qt::blue));
                    ui->Start_button->setText("Start");
                }
            }

        }


        Button_Counting++; //10ms
}

bool calibration::eventFilter(QObject *target, QEvent *event)
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
        }
        if(Button_Counting > 3)
        {
            PressedButton = nullptr;
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        if(IsPressed == true)
        {
            PressedButton = nullptr;
            LCDNumber = nullptr;
            Label = nullptr;
            IsPressed = false;
            Button_Timer->stop();
        }
    }
    return QWidget::eventFilter(target, event);
}
