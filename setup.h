#ifndef SETUP_H
#define SETUP_H

#include <QWidget>
#include <QFontDatabase>
#include <QFont>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include "sysparm.h"
#include "gpiotools.h"
#include "calibration.h"

namespace Ui {
class setup;
}

class setup : public QWidget
{
    Q_OBJECT

public:
    SysParm *sysParm;
    void setIDstarmed(int i) {id_starmed = i; }
    bool eventFilter(QObject *target, QEvent *event);

    explicit setup(QWidget *parent = nullptr , SysParm *sysParm=nullptr);
    ~setup();

public slots:
    void slot_Setup_ScreenIN();
    void slot_Setup_Screenin();
    void slot_STARwave_Start();
    void slot_STARwave_Stop();


private:
    Ui::setup *ui;
    QTimer *ButtonTimer;
    QTimer *DisplayTimer;
    calibration *Calibration_Screen;

    int Button_Counting;
    int id_starmed;
    bool IsPressed;


    QPushButton *PressedButton;
    QLabel *Label;


    void LCD_Display();
    void Control_RGBLED();
    void Choose_Click_state(Click_State state);

    void SetDefault_function();
    void SetDefault_value();
    void SetFont();
    void SetEventFilter();
    void SetConnect();
    void SetUI_Color();
    void SetImage();


    void UI_Show();
    void UI_Hide();


private slots:
    void slot_Button_Loop();
    void slot_Display();

signals:
    void sig_Main_Screen();
    void sig_ScreenIn_Calibration();
    void sig_STARwave_Start();
    void sig_STARwave_Stop();

};

#endif // SETUP_H
