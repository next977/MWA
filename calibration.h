#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <QWidget>
#include "global.h"
#include "sysparm.h"
#include "gpiotools.h"
#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include <QSocketNotifier>
#include <QLabel>
#include <QLCDNumber>
#include <QFontDatabase>
#include <QDebug>
#include <QFont>

class SysParm;
namespace Ui {
class calibration;
}

class calibration : public QWidget
{
    Q_OBJECT


public:
    SysParm *sysParm;
    uint16 data_in_index;
    void setIDstarmed(int i) {id_starmed = i; }
    bool eventFilter(QObject *target, QEvent *event);
    explicit calibration(QWidget *parent = nullptr, SysParm *sysParm = nullptr);
    ~calibration();

public slots:
    void slot_ScreenIn_Calibration();

private:
    Ui::calibration *ui;

    QTimer *DisplayTimer;
    QTimer *Button_Timer;
    QPushButton *PressedButton;
    QSocketNotifier *notRsRead;
    QLCDNumber *LCDNumber;
    QLabel *Label;
    int id_starmed;
    int Button_Counting;
    bool IsPressed;
    bool Output_state;

    void SetFont();
    void SetImage();
    void UDPressed(UPDOWN_Btns_State state);
    void Calibration_connect();
    void Choose_Click_state(Click_State state);
    void installFilter();
    void UI_Hide();
    void UI_Show();
    QPalette Ui_SetColor(QPalette::ColorRole , Qt::GlobalColor , QPalette::ColorRole , Qt::GlobalColor );
private slots:
    void slot_display();
    void slot_Button_Loop();


signals:
    void sig_ScreenIn_Setup();
    void sig_STARwave_Start();
    void sig_STARwave_Stop();


};

#endif // CALIBRATION_H
