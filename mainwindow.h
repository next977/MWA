#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QFontDatabase>
#include <QFont>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QElapsedTimer>
#include <QLCDNumber>
#include <QDebug>
#include <QLabel>
#include "gpiobutton.h"
#include "gpiotools.h"
#include "setup.h"
#include "sysparm.h"
#include "monitoringtask.h"
#include "outputthread.h"
#include "file.h"
#include "data.h"
#include "uart.h"
#include "audiooutput.h"
#include "starmedlogo.h"
#include "sysmessage.h"
#include "usbotg.h"
#include "securedic.h"
#include "history.h"

#define DISPLAY_INTERVAL_VALUE 100

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    SysParm *sysParm;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setIDstarmed(int i) {id_starmed = i; }
    bool eventFilter(QObject *target, QEvent *event);
#if 0
    enum System_State
    {
        SYSTEM_STANDBY,
        SYSTEM_READY,
        SYSTEM_IGNITION

    };
#endif


private:
    Ui::MainWindow *ui;
    MonitoringTask *Monitoringtask;
    OutputThread *Outputthread;
    Data *Data_Class;
    uart *Uart_Class;
    AudioOutput *Audio_Output;
    File_RW *File_Rw;
    starmedlogo *Starmedlogo_Screen;
    securedic *SecuredIC;
    usbotg *USBOTG;
    setup *Setup_Screen;
    sysmessage *Sysmessage_Screen;
    history *History_Screen;

    QPushButton *PressedButton;
    QLCDNumber *LCDNumber;
    QLabel *Label;
    GPIOButton *pGButton;
    QTimer *Button_Timer;
    QTimer *Display_Timer;
    QElapsedTimer ElapsedTime;

    int id_starmed;
    int Button_Counting;
    bool IsPressed;

    bool PA_Enable_Button_Flag;
    bool System_Status_Button_Flag[SYSTEM_STATUS_SIZE];
    bool Track_Mode_State;
    uint32 Channel_Select_State;

    Click_State Set_Click_State;
#if 0
    System_State Set_System_State;
#endif
    Channel_State channel_last_state;

    uint32 Ch1_Watt_value;
    uint32 Ch2_Watt_value;

    uint32 Ch1_Time_value;
    uint32 Ch2_Time_value;

    void Open_Device();
    void Setting();
    void SetValue();
    void SetFunction();
    void Setting_RTCVolume();
    void Set_Get_Data();

    void UI_Hide();
    void UI_Show();
    void SetFont();
    void SetImage();
    void Set_LCDDisplay();
    void Set_TempDisplay(Channel_State state);
    void Set_System_State(State_EN state);
    void Set_Track_Mode(bool state);
    void SetEventFilter();

    void Event_connect();
    void Port_Connect();
    void Parsing();
    void UDPressed(UPDOWN_Btns_State state);
    void Choose_Click_state(Click_State state);
    void Set_UI_Position(Channel_State channel_state);
    void Set_UI(Channel_State state);
    void Search_Start();
    void Data_Setting();
    void frame_dataset();
    void frame_sysctrl_set();
    void frame_sysctrl_set(uint32 power_value);
    void frame_sysctrl_set(uint32 freq, double pulse_width, double pulse_period);
    void frame_freqfind_set(frame::Frequency_Mode_Status state);
    void frame_reg_write_set(frame::Address_Status address);
    void display(Data *data);

private slots:
    void slot_Button_Loop();
    void slot_UiDisplay();
    void slot_Main_Screen();
    void slot_data_frameparsing();
    void slot_STARwave_Start();
    void slot_STARwave_Stop();


#ifdef __FEATURE_MICROWAVE__
#elif
    void slot_File_Download();
#endif
    void slot_FD_Ready_Status_OK();
    void slot_FD_Ready_Status_NO();
    void slot_FD_Status_Confirm();
    void slot_Frame_Number(int frame_number);

signals:
    void sig_Setup_Screen();
    void sig_History_Screen();
    void sig_alert_state(Alert_State state);
    void sig_error_state(Error_State state);

};

#endif // MAINWINDOW_H
