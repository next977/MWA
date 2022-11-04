#include "history.h"
#include "ui_history.h"

history::history(QWidget *parent, SysParm *sysParm) :
    QWidget(parent),
    ui(new Ui::history)
{
    ui->setupUi(this);
    this->sysParm = sysParm;
    this->setIDstarmed(QFontDatabase::addApplicationFont(":/Fonts/starmedfont.ttf"));
    ButtonTimer = new QTimer(this);
    DisplayTimer = new QTimer(this);
    SetDefault_value();
    SetDefault_function();
}

history::~history()
{
    delete ui;
}

void history::SetDefault_value()
{
    IsPressed = false;
}

void history::SetDefault_function()
{
    SetImage();
    SetFont();
    SetConnect();
    SetEventFilter();

}
void history::slot_History_ScreenIN()
{

    DisplayTimer->start(50);
    IsPressed = false;

    this->show();
}

void history::SetConnect()
{
    connect(ButtonTimer, SIGNAL(timeout()), this, SLOT(slot_Button_Loop()));
    connect(DisplayTimer, SIGNAL(timeout()), this, SLOT(slot_Display()));
#if 0
    connect(this, SIGNAL(sig_ScreenIn_Calibration()), Calibration_Screen, SLOT(slot_ScreenIn_Calibration()));
    connect(Calibration_Screen, SIGNAL(sig_ScreenIn_Setup()), this, SLOT(slot_Setup_Screenin()));
#endif
}

void history::SetFont()
{

    QString family = QFontDatabase::applicationFontFamilies(id_starmed).at(0);
    QFont title(family,30, QFont::Bold);
    QFont big_title(family,50, QFont::Bold);

    QFont text(family,15, QFont::Bold);
    QFont uinit(family,50, QFont::Bold);
    QFont data(family,35, QFont::Bold);
    QFont sw_data(family,30, QFont::Bold);

    ui->history_title_label->setFont(big_title);
    ui->history_title_label->setStyleSheet("QLabel { color : white; }"); //text color
    ui->text_1_label->setFont(text);
    ui->text_2_label->setFont(text);
    ui->text_3_label->setFont(text);

    ui->history_title_label->setAlignment(Qt::AlignCenter);

}

void history::SetImage()
{
    ui->image_screen->setPixmap(QPixmap(":/Images/state_default.png"));
    ui->back_label->setPixmap(QPixmap(":/Images/button_back_enable.png"));
    ui->up_button_label->setPixmap(QPixmap(":/Images/button_up_enable_2.png"));
    ui->down_button_label->setPixmap(QPixmap(":/Images/button_down_enable_2.png"));
    ui->usb_button_label->setPixmap(QPixmap(":/Images/button_usb_enable.png"));

    ui->image_screen->setScaledContents(true);
    ui->back_label->setScaledContents(true);
    ui->up_button_label->setScaledContents(true);
    ui->down_button_label->setScaledContents(true);
    ui->usb_button_label->setScaledContents(true);
#if 0
    ui->image_screen->setPixmap(QPixmap(":/Images/state_setup.jpg"));
    ui->back_label->setPixmap(QPixmap(":/Images/button_back_enable.png"));
    ui->up_button_label->setPixmap(QPixmap(":/Images/button_up_enable.png"));
    ui->down_button_label->setPixmap(QPixmap(":/Images/button_down_enable.png"));
    ui->timer_highlight_label->setPixmap(QPixmap(":/Images/highlight.png"));
    ui->volume_highlight_label->setPixmap(QPixmap(":/Images/highlight.png"));
    ui->date_day_highlight_label->setPixmap(QPixmap(":/Images/highlight.png"));
    ui->date_month_highlight_label->setPixmap(QPixmap(":/Images/highlight.png"));
    ui->date_year_highlight_label->setPixmap(QPixmap(":/Images/highlight.png"));
    ui->time_hour_highlight_label->setPixmap(QPixmap(":/Images/highlight.png"));
    ui->time_miute_highlight_label->setPixmap(QPixmap(":/Images/highlight.png"));
    ui->time_ampm_highlight_label->setPixmap(QPixmap(":/Images/highlight.png"));
    ui->image_screen->setScaledContents(true);
    ui->back_label->setScaledContents(true);
    ui->up_button_label->setScaledContents(true);
    ui->down_button_label->setScaledContents(true);
    ui->timer_highlight_label->setScaledContents(true);
    ui->volume_highlight_label->setScaledContents(true);
    ui->date_day_highlight_label->setScaledContents(true);
    ui->date_month_highlight_label->setScaledContents(true);
    ui->date_year_highlight_label->setScaledContents(true);
    ui->time_hour_highlight_label->setScaledContents(true);
    ui->time_miute_highlight_label->setScaledContents(true);
    ui->time_ampm_highlight_label->setScaledContents(true);
#endif
}
void history::SetEventFilter()
{
#if 0
    ui->back_label->installEventFilter(this);
    ui->date_day_label->installEventFilter(this);
    ui->date_month_label->installEventFilter(this);
    ui->date_year_label->installEventFilter(this);
    ui->time_hour_label->installEventFilter(this);
    ui->time_ampm_label->installEventFilter(this);
    ui->time_minute_label->installEventFilter(this);
    ui->volume_data_label->installEventFilter(this);
    ui->timer_data_label->installEventFilter(this);
    ui->up_button_label->installEventFilter(this);
    ui->down_button_label->installEventFilter(this);
    ui->startup_title_label->installEventFilter(this);
#endif
    ui->back_label->installEventFilter(this);
    ui->up_button_label->installEventFilter(this);
    ui->down_button_label->installEventFilter(this);
    ui->usb_button_label->installEventFilter(this);

}
void history::slot_Display()
{
#if 0
    LCD_Display();
#endif
}


void history::slot_Button_Loop()
{

        if((Label == ui->up_button_label)&&(ui->up_button_label->isEnabled()))
        {
            if(Button_Counting > 3) //30ms
            {
                qDebug("up_button_label_click");
                Button_Counting = 0;
            }
        }
        else if((Label == ui->down_button_label)&&(ui->down_button_label->isEnabled()))
        {
            if(Button_Counting > 3) //30ms
            {
                qDebug("down_button_label_click");
                Button_Counting = 0;
            }
        }
        else if(Label == ui->back_label)
        {
            if(Button_Counting > 1) //10ms
            {
                qDebug("back_button_label_click");
                emit sig_Main_Screen();

                DisplayTimer->stop();
                ButtonTimer->stop();
                this->close();
            }
        }
        else if(Label == ui->usb_button_label)
        {
            if(Button_Counting > 1) //10ms
            {
                qDebug("usb_button_label_click");
                ButtonTimer->stop();
            }
        }
        Button_Counting++; //10ms
}

bool history::eventFilter(QObject *target, QEvent *event)
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


