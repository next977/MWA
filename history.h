#ifndef HISTORY_H
#define HISTORY_H

#include <QWidget>
#include <QFontDatabase>
#include <QFont>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include "sysparm.h"
#include "gpiotools.h"

namespace Ui {
class history;
}

class history : public QWidget
{
    Q_OBJECT

public:
    SysParm *sysParm;
    void setIDstarmed(int i) {id_starmed = i; }
    bool eventFilter(QObject *target, QEvent *event);
    explicit history(QWidget *parent = nullptr , SysParm *sysParm=nullptr);
    ~history();
public slots:
    void slot_History_ScreenIN();

private:
    Ui::history *ui;
    QTimer *ButtonTimer;
    QTimer *DisplayTimer;
    QPushButton *PressedButton;
    QLabel *Label;
    int Button_Counting;
    int id_starmed;
    bool IsPressed;

    void SetEventFilter();
    void SetConnect();
    void SetFont();
    void SetImage();
    void SetDefault_value();
    void SetDefault_function();

private slots:
    void slot_Button_Loop();
    void slot_Display();
signals:
    void sig_Main_Screen();
};

#endif // HISTORY_H
