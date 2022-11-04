#include "setup.h"
#include "ui_setup.h"

setup::setup(QWidget *parent, SysParm *sysParm) :
    QWidget(parent),
    ui(new Ui::setup)
{
    ui->setupUi(this);
    this->sysParm = sysParm;
    this->setIDstarmed(QFontDatabase::addApplicationFont(":/Fonts/starmedfont.ttf"));
    ButtonTimer = new QTimer(this);
    DisplayTimer = new QTimer(this);
    Calibration_Screen = new calibration(this, sysParm);
    Calibration_Screen->close();

    SetDefault_value();
    SetDefault_function();

}

setup::~setup()
{
    delete ui;
}
void setup::SetConnect()
{
    connect(ButtonTimer, SIGNAL(timeout()), this, SLOT(slot_Button_Loop()));
    connect(DisplayTimer, SIGNAL(timeout()), this, SLOT(slot_Display()));
    connect(this, SIGNAL(sig_ScreenIn_Calibration()), Calibration_Screen, SLOT(slot_ScreenIn_Calibration()));
    connect(Calibration_Screen, SIGNAL(sig_ScreenIn_Setup()), this, SLOT(slot_Setup_Screenin()));
    connect(Calibration_Screen, SIGNAL(sig_STARwave_Start()), this, SLOT(slot_STARwave_Start()));
    connect(Calibration_Screen, SIGNAL(sig_STARwave_Stop()), this, SLOT(slot_STARwave_Stop()));

}
void setup::slot_Setup_ScreenIN()
{
    qDebug("Setup Screen IN");
#ifndef _ONPC_
        gpioTools::RFSwitchLED_OFF();
#endif

    Choose_Click_state(CLICK_NONE);
    DisplayTimer->start(50);
    IsPressed = false;
    this->show();
}
void setup::slot_Setup_Screenin()
{
    qDebug("Setup Screen in");
    SetDefault_value();
    Choose_Click_state(CLICK_NONE);
    DisplayTimer->start(50);
    IsPressed = false;
    UI_Show();
}
void setup::slot_STARwave_Start()
{
    emit sig_STARwave_Start();
}
void setup::slot_STARwave_Stop()
{
    emit sig_STARwave_Stop();
}

void setup::SetDefault_value()
{
    IsPressed = false;

}

void setup::SetDefault_function()
{
    SetImage();
    SetUI_Color();
    SetFont();
    SetConnect();
    SetEventFilter();

}


void setup::SetFont()
{
    QString family = QFontDatabase::applicationFontFamilies(id_starmed).at(0);
    QFont title(family,25, QFont::Bold);
    QFont big_title(family,50, QFont::Bold);

    QFont button(family,20, QFont::Bold);
    QFont uinit(family,50, QFont::Bold);
    QFont data(family,35, QFont::Bold);
    QFont sw_data(family,30, QFont::Bold);

    QFont smalldata(family,35, QFont::Bold);

    QFont symbol(family,15, QFont::Bold);
    QFont mode(family,20, QFont::Bold);
    QFont language(family,20, QFont::Bold);

    //label size

    ui->startup_title_label->setFont(big_title);

    ui->date_day_title_label->setFont(title);
    ui->date_month_title_label->setFont(title);
    ui->date_year_title_label->setFont(title);
    ui->time_hour_title_label->setFont(title);
    ui->time_minute_title_label->setFont(title);
    ui->time_ampm_title_label->setFont(title);
    ui->serial_title_label->setFont(title);

    //data label size
    ui->volume_data_label->setFont(data);
    ui->date_day_label->setFont(smalldata);
    ui->date_month_label->setFont(smalldata);
    ui->date_year_label->setFont(smalldata);
    ui->time_hour_label->setFont(smalldata);
    ui->time_minute_label->setFont(smalldata);
    ui->time_ampm_label->setFont(smalldata);
    ui->interval_data_label->setFont(data);
    ui->interval_symbol_label->setFont(data);
    ui->serial_data_label->setFont(sw_data);

    //title color

    ui->startup_title_label->setStyleSheet("QLabel { color : white; }"); //text color

    ui->date_day_title_label->setStyleSheet("QLabel { color : white; }"); //text color
    ui->date_month_title_label->setStyleSheet("QLabel { color : white; }"); //text color
    ui->date_year_title_label->setStyleSheet("QLabel { color : white; }"); //text color
    ui->time_hour_title_label->setStyleSheet("QLabel { color : white; }"); //text color
    ui->time_minute_title_label->setStyleSheet("QLabel { color : white; }"); //text color
    ui->time_ampm_title_label->setStyleSheet("QLabel { color : white; }"); //text color

    ui->interval_symbol_label->setStyleSheet("QLabel { color : white; }"); //text color

    //data color

    ui->interval_data_label->setStyleSheet("QLabel { color : white; }"); //text color
    ui->volume_data_label->setStyleSheet("QLabel { color : white; }"); //text color
    ui->date_year_label->setStyleSheet("QLabel { color : white; }"); //text color
    ui->date_month_label->setStyleSheet("QLabel { color : white; }"); //text color
    ui->date_day_label->setStyleSheet("QLabel { color : white; }"); //text color
    ui->time_hour_label->setStyleSheet("QLabel { color : white; }"); //text color
    ui->time_minute_label->setStyleSheet("QLabel { color : white; }"); //text color
    ui->time_ampm_label->setStyleSheet("QLabel { color : white; }"); //text color

    ui->serial_data_label->setStyleSheet("QLabel { color : orange; }"); //text color

    //title text

    ui->startup_title_label->setText("Setup");




    ui->date_day_title_label->setAlignment(Qt::AlignCenter);
    ui->date_month_title_label->setAlignment(Qt::AlignCenter);
    ui->date_year_title_label->setAlignment(Qt::AlignCenter);
    ui->time_hour_title_label->setAlignment(Qt::AlignCenter);
    ui->time_minute_title_label->setAlignment(Qt::AlignCenter);
    ui->time_ampm_title_label->setAlignment(Qt::AlignCenter);
    ui->serial_title_label->setAlignment(Qt::AlignCenter);

    ui->interval_data_label->setAlignment(Qt::AlignCenter);
    ui->volume_data_label->setAlignment(Qt::AlignCenter);
    ui->date_day_label->setAlignment(Qt::AlignCenter);
    ui->date_month_label->setAlignment(Qt::AlignCenter);
    ui->date_year_label->setAlignment(Qt::AlignCenter);

    ui->time_hour_label->setAlignment(Qt::AlignCenter);
    ui->time_minute_label->setAlignment(Qt::AlignCenter);
    ui->time_ampm_label->setAlignment(Qt::AlignCenter);
    ui->serial_data_label->setAlignment(Qt::AlignCenter);

}
void setup::SetUI_Color()
{

    //ui->mode_button->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,   stop:0 rgba(50, 50, 255, 100), stop:1 rgba(20, 50, 150, 150))");
    //ui->language_button->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,   stop:0 rgba(200, 200, 50, 100), stop:1 rgba(150, 150, 50, 150))");

    //ui->setup_button->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,   stop:0 rgba(20, 20, 20, 100), stop:1 rgba(20, 20, 20, 150))");
    //ui->up_button->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,   stop:0 rgba(60, 255, 50, 100), stop:1 rgba(98, 255, 150, 150))");
    //ui->down_button->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,   stop:0 rgba(60, 255, 100, 100), stop:1 rgba(98, 255, 150, 150))");

    //ui->Result_frame->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,   stop:0 rgba(60, 255, 50, 50), stop:1 rgba(98, 255, 150, 30))");
}

void setup::SetImage()
{
    ui->image_screen->setPixmap(QPixmap(":/Images/state_setup.png"));
    ui->back_label->setPixmap(QPixmap(":/Images/button_back_enable.png"));
    ui->up_button_label->setPixmap(QPixmap(":/Images/button_up_enable_1.png"));
    ui->down_button_label->setPixmap(QPixmap(":/Images/button_down_enable_1.png"));
    ui->interval_highlight_label->setPixmap(QPixmap(":/Images/highlight.png"));
    ui->volume_highlight_label->setPixmap(QPixmap(":/Images/highlight.png"));
    ui->date_day_highlight_label->setPixmap(QPixmap(":/Images/highlight.png"));
    ui->date_month_highlight_label->setPixmap(QPixmap(":/Images/highlight.png"));
    ui->date_year_highlight_label->setPixmap(QPixmap(":/Images/highlight.png"));
    ui->time_hour_highlight_label->setPixmap(QPixmap(":/Images/highlight.png"));
    ui->time_miute_highlight_label->setPixmap(QPixmap(":/Images/highlight.png"));
    ui->time_ampm_highlight_label->setPixmap(QPixmap(":/Images/highlight.png"));
    ui->interval_title_label->setPixmap(QPixmap(":/Images/switching_symbol.png"));
    ui->volume_title_label->setPixmap(QPixmap(":/Images/volume_symbol.png"));

    ui->image_screen->setScaledContents(true);
    ui->back_label->setScaledContents(true);
    ui->up_button_label->setScaledContents(true);
    ui->down_button_label->setScaledContents(true);

    ui->interval_highlight_label->setScaledContents(true);
    ui->volume_highlight_label->setScaledContents(true);
    ui->date_day_highlight_label->setScaledContents(true);
    ui->date_month_highlight_label->setScaledContents(true);
    ui->date_year_highlight_label->setScaledContents(true);
    ui->time_hour_highlight_label->setScaledContents(true);
    ui->time_miute_highlight_label->setScaledContents(true);
    ui->time_ampm_highlight_label->setScaledContents(true);
    ui->interval_title_label->setScaledContents(true);
    ui->volume_title_label->setScaledContents(true);
}

void setup::SetEventFilter()
{

    ui->back_label->installEventFilter(this);

    ui->date_day_label->installEventFilter(this);
    ui->date_month_label->installEventFilter(this);
    ui->date_year_label->installEventFilter(this);
    ui->time_hour_label->installEventFilter(this);
    ui->time_ampm_label->installEventFilter(this);
    ui->time_minute_label->installEventFilter(this);
    ui->volume_data_label->installEventFilter(this);
    ui->interval_data_label->installEventFilter(this);
    ui->up_button_label->installEventFilter(this);
    ui->down_button_label->installEventFilter(this);

    ui->startup_title_label->installEventFilter(this);

}


void setup::Choose_Click_state(Click_State state)
{
    sysParm->mRfDeviceFactors->click_state = state;
    switch(state)
    {
        case CLICK_NONE:
            ui->interval_data_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->volume_data_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_year_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_month_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_day_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_hour_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_minute_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_ampm_label->setStyleSheet("QLabel { color : white; }");
            ui->interval_highlight_label->hide();
            ui->volume_highlight_label->hide();
            ui->date_day_highlight_label->hide();
            ui->date_month_highlight_label->hide();
            ui->date_year_highlight_label->hide();
            ui->time_hour_highlight_label->hide();
            ui->time_miute_highlight_label->hide();
            ui->time_ampm_highlight_label->hide();
            break;

        case CLICK_TIMER_DATA:
            ui->interval_data_label->setStyleSheet("QLabel { color : orange; }");
            ui->volume_data_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_year_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_month_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_day_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_hour_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_minute_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_ampm_label->setStyleSheet("QLabel { color : white; }");
            ui->interval_highlight_label->show();
            ui->volume_highlight_label->hide();
            ui->date_day_highlight_label->hide();
            ui->date_month_highlight_label->hide();
            ui->date_year_highlight_label->hide();
            ui->time_hour_highlight_label->hide();
            ui->time_miute_highlight_label->hide();
            ui->time_ampm_highlight_label->hide();
            break;

        case CLICK_VOLUME:
            ui->volume_data_label->setStyleSheet("QLabel { color : orange; }");
            ui->interval_data_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_year_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_month_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_day_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_hour_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_minute_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_ampm_label->setStyleSheet("QLabel { color : white; }");
            ui->interval_highlight_label->hide();
            ui->volume_highlight_label->show();
            ui->date_day_highlight_label->hide();
            ui->date_month_highlight_label->hide();
            ui->date_year_highlight_label->hide();
            ui->time_hour_highlight_label->hide();
            ui->time_miute_highlight_label->hide();
            ui->time_ampm_highlight_label->hide();
            break;
        case CLICK_DATE_DAY:
            ui->date_day_label->setStyleSheet("QLabel { color : orange; }");
            ui->interval_data_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->volume_data_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_year_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_month_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_hour_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_minute_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_ampm_label->setStyleSheet("QLabel { color : white; }");
            ui->interval_highlight_label->hide();
            ui->volume_highlight_label->hide();
            ui->date_day_highlight_label->show();
            ui->date_month_highlight_label->hide();
            ui->date_year_highlight_label->hide();
            ui->time_hour_highlight_label->hide();
            ui->time_miute_highlight_label->hide();
            ui->time_ampm_highlight_label->hide();
            break;
        case CLICK_DATE_MONTH:
            ui->date_month_label->setStyleSheet("QLabel { color : orange; }");
            ui->interval_data_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->volume_data_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_year_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_day_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_hour_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_minute_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_ampm_label->setStyleSheet("QLabel { color : white; }");
            ui->interval_highlight_label->hide();
            ui->volume_highlight_label->hide();
            ui->date_day_highlight_label->hide();
            ui->date_month_highlight_label->show();
            ui->date_year_highlight_label->hide();
            ui->time_hour_highlight_label->hide();
            ui->time_miute_highlight_label->hide();
            ui->time_ampm_highlight_label->hide();
            break;
        case CLICK_DATE_YEAR:
            ui->date_year_label->setStyleSheet("QLabel { color : orange; }");
            ui->interval_data_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->volume_data_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_month_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_day_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_hour_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_minute_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_ampm_label->setStyleSheet("QLabel { color : white; }");
            ui->interval_highlight_label->hide();
            ui->volume_highlight_label->hide();
            ui->date_day_highlight_label->hide();
            ui->date_month_highlight_label->hide();
            ui->date_year_highlight_label->show();
            ui->time_hour_highlight_label->hide();
            ui->time_miute_highlight_label->hide();
            ui->time_ampm_highlight_label->hide();

            break;
        case CLICK_TIME_HOUR:
            ui->time_hour_label->setStyleSheet("QLabel { color : orange; }");
            ui->interval_data_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->volume_data_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_year_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_month_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_day_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_minute_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_ampm_label->setStyleSheet("QLabel { color : white; }");
            ui->interval_highlight_label->hide();
            ui->volume_highlight_label->hide();
            ui->date_day_highlight_label->hide();
            ui->date_month_highlight_label->hide();
            ui->date_year_highlight_label->hide();
            ui->time_hour_highlight_label->show();
            ui->time_miute_highlight_label->hide();
            ui->time_ampm_highlight_label->hide();
            break;
        case CLICK_TIME_MINUTE:
            ui->time_minute_label->setStyleSheet("QLabel { color : orange; }");
            ui->interval_data_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->volume_data_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_year_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_month_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_day_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_hour_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_ampm_label->setStyleSheet("QLabel { color : white; }");
            ui->interval_highlight_label->hide();
            ui->volume_highlight_label->hide();
            ui->date_day_highlight_label->hide();
            ui->date_month_highlight_label->hide();
            ui->date_year_highlight_label->hide();
            ui->time_hour_highlight_label->hide();
            ui->time_miute_highlight_label->show();
            ui->time_ampm_highlight_label->hide();
            break;
        case CLICK_TIME_AMPM:
            ui->time_ampm_label->setStyleSheet("QLabel { color : orange; }");
            ui->interval_data_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->volume_data_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_year_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_month_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_day_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_hour_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_minute_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->interval_highlight_label->hide();
            ui->volume_highlight_label->hide();
            ui->date_day_highlight_label->hide();
            ui->date_month_highlight_label->hide();
            ui->date_year_highlight_label->hide();
            ui->time_hour_highlight_label->hide();
            ui->time_miute_highlight_label->hide();
            ui->time_ampm_highlight_label->show();

            break;
        case CLICK_BRIGHTNESS:
            ui->interval_data_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->volume_data_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_year_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_month_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_day_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_hour_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_minute_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_ampm_label->setStyleSheet("QLabel { color : white; }");
            ui->interval_highlight_label->hide();
            ui->volume_highlight_label->hide();
            ui->date_day_highlight_label->hide();
            ui->date_month_highlight_label->hide();
            ui->date_year_highlight_label->hide();
            ui->time_hour_highlight_label->hide();
            ui->time_miute_highlight_label->hide();
            ui->time_ampm_highlight_label->hide();
            break;

        default:
            ui->interval_data_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->volume_data_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_year_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_month_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->date_day_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_hour_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_minute_label->setStyleSheet("QLabel { color : white; }"); //text color
            ui->time_ampm_label->setStyleSheet("QLabel { color : white; }");
            ui->interval_highlight_label->hide();
            ui->volume_highlight_label->hide();
            ui->date_day_highlight_label->hide();
            ui->date_month_highlight_label->hide();
            ui->date_year_highlight_label->hide();
            ui->time_hour_highlight_label->hide();
            ui->time_miute_highlight_label->hide();
            ui->time_ampm_highlight_label->hide();
            break;
   }
}




void setup::LCD_Display()
{
    QString strText;

    strText.sprintf("%d", sysParm->mRfDeviceFactors->Volume);
    ui->volume_data_label->setText(strText);
    strText.sprintf("%d", sysParm->mRfDeviceFactors->Date_year);
    ui->date_year_label->setText(strText);
    strText.sprintf("%d", sysParm->mRfDeviceFactors->Date_month);
    ui->date_month_label->setText(strText);
    strText.sprintf("%d", sysParm->mRfDeviceFactors->Date_day);
    ui->date_day_label->setText(strText);
    strText.sprintf("%d", sysParm->mRfDeviceFactors->Time_hour);
    ui->time_hour_label->setText(strText);
    strText.sprintf("%d", sysParm->mRfDeviceFactors->Time_min);
    ui->time_minute_label->setText(strText);

    ui->time_ampm_label->setText(sysParm->mRfDeviceFactors->Time_ampm);

}


void setup::UI_Show()
{
    ui->back_label->show();
    ui->date_day_label->show();

    ui->date_month_label->show();
    ui->date_year_label->show();
    ui->down_button_label->show();

    ui->image_screen->show();
    ui->startup_title_label->show();

    ui->interval_data_label->show();

    ui->interval_symbol_label->show();
    ui->interval_title_label->show();
    ui->time_ampm_label->show();
    ui->time_hour_label->show();
    ui->time_minute_label->show();
    ui->up_button_label->show();
    ui->volume_data_label->show();
    ui->volume_title_label->show();

    ui->date_day_title_label->show();
    ui->date_month_title_label->show();
    ui->date_year_title_label->show();
    ui->time_hour_title_label->show();
    ui->time_minute_title_label->show();
    ui->time_ampm_title_label->show();
    ui->serial_title_label->show();
    ui->serial_data_label->show();


    ui->line->show();
    ui->line_2->show();
    ui->line_3->show();
    ui->line_4->show();

}
void setup::UI_Hide()
{
    ui->back_label->hide();
    ui->date_day_label->hide();

    ui->date_month_label->hide();
    ui->date_year_label->hide();
    ui->down_button_label->hide();
    ui->image_screen->hide();
    ui->startup_title_label->hide();

    ui->interval_data_label->hide();

    ui->interval_symbol_label->hide();
    ui->interval_title_label->hide();
    ui->time_ampm_label->hide();

    ui->time_hour_label->hide();

    ui->time_minute_label->hide();


    ui->date_day_title_label->hide();
    ui->date_month_title_label->hide();
    ui->date_year_title_label->hide();
    ui->time_hour_title_label->hide();
    ui->time_minute_title_label->hide();
    ui->time_ampm_title_label->hide();
    ui->serial_title_label->hide();
    ui->serial_data_label->hide();

    ui->up_button_label->hide();
    ui->volume_data_label->hide();

    ui->volume_title_label->hide();

    ui->line->hide();
    ui->line_2->hide();
    ui->line_3->hide();
    ui->line_4->hide();


}
void setup::slot_Display()
{

    LCD_Display();
}


void setup::slot_Button_Loop()
{
        if((Label == ui->up_button_label)&&(ui->up_button_label->isEnabled()))
        {
            if(Button_Counting > 3) //30ms
            {

                Button_Counting = 0;
            }
        }
        else if((Label == ui->down_button_label)&&(ui->down_button_label->isEnabled()))
        {
            if(Button_Counting > 3) //30ms
            {

                Button_Counting = 0;
            }
        }
        else if(Label == ui->back_label)
        {
            if(Button_Counting > 1) //10ms
            {
                qDebug("back_button_label_click");
                emit sig_Main_Screen();
                //display_Timer->stop();
                //Hide();
                DisplayTimer->stop();
                ButtonTimer->stop();
                this->close();
            }
        }
        else if(Label == ui->startup_title_label)
        {
            if(Button_Counting > 1) //10ms
            {
                qDebug("Calibration Click");
                UI_Hide();
                Choose_Click_state(CLICK_NONE);
                DisplayTimer->stop();
                ButtonTimer->stop();
                emit sig_ScreenIn_Calibration();
            }
        }

        else if(Label == ui->interval_data_label)
        {
            if(Button_Counting > 1) //10ms
            {
                Choose_Click_state(CLICK_TIMER_DATA);
                ButtonTimer->stop();
            }
        }
        else if(Label == ui->volume_data_label)
        {
            if(Button_Counting > 1) //10ms
            {
                Choose_Click_state(CLICK_VOLUME);
                ButtonTimer->stop();
            }
        }
        else if(Label == ui->date_day_label)
        {
            if(Button_Counting > 1) //10ms
            {
                Choose_Click_state(CLICK_DATE_DAY);
                ButtonTimer->stop();
            }
        }
        else if(Label == ui->date_month_label)
        {
            if(Button_Counting > 1) //10ms
            {
                Choose_Click_state(CLICK_DATE_MONTH);
                ButtonTimer->stop();
            }
        }
        else if(Label == ui->date_year_label)
        {
            if(Button_Counting > 1) //10ms
            {
                Choose_Click_state(CLICK_DATE_YEAR);
                ButtonTimer->stop();
            }
        }
        else if(Label == ui->time_hour_label)
        {
            if(Button_Counting > 1) //10ms
            {
                Choose_Click_state(CLICK_TIME_HOUR);
                ButtonTimer->stop();
            }
        }
        else if(Label == ui->time_minute_label)
        {
            if(Button_Counting > 1) //10ms
            {
                Choose_Click_state(CLICK_TIME_MINUTE);
                ButtonTimer->stop();
            }
        }
        else if(Label == ui->time_ampm_label)
        {
            if(Button_Counting > 1) //10ms
            {
                Choose_Click_state(CLICK_TIME_AMPM);
                ButtonTimer->stop();
            }
        }

        Button_Counting++; //10ms
}

bool setup::eventFilter(QObject *target, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        if(IsPressed == false)
        {
            PressedButton = (QPushButton *)target;
            Label = (QLabel *)target;
            IsPressed = true;
            Button_Counting = 0;
            ButtonTimer->setInterval(10);
            ButtonTimer->start();
        }
        if(Button_Counting > 5)
        {
            PressedButton = nullptr;
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        if(IsPressed == true)
        {
            PressedButton = nullptr;
            Label = nullptr;
            IsPressed = false;
            ButtonTimer->stop();
        }
    }

    return QWidget::eventFilter(target, event);
}
