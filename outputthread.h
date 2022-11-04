#ifndef OUTPUTTHREAD_H
#define OUTPUTTHREAD_H

#include <QObject>
#include "workerthread.h"

#include <QTime>
#include <QTimer>
#include <QBasicTimer>
#include "monitoringtask.h"

using namespace std;
class SysParm;


class OutputThread : public QObject, public WorkerThread
{
    Q_OBJECT
public:
    explicit OutputThread(QObject *parent = nullptr, SysParm *sysParm = nullptr);

    ~OutputThread();

signals:

public slots:

private:

    MonitoringTask *monitoringTask;


    bool tmRFPowerActiveState;
    void loop();
    clock_t begin;

public:
#ifndef __FEATURE_STRIKER__
    enum RF_Output_CM_Mode_en
    {
        RF_DEVICE_OP_MODE_3CM = 0,
        RF_DEVICE_OP_MODE_7CM
    };
#endif
    SysParm *sysParm;
    RF_DEVICE_FACTORS *mRfDeviceFactors;

    QBasicTimer btimer;




    void RFStart();
    void RFStop();
    bool GetRFPowerActiveState() const;
    void SetRFPowerActiveState(bool state);
    void MRFA_InitRFADeviceState();
    void MRFA_DeInitRFADeviceState();
    //void DACPortOutputInc(U16, bool pass=true);
    //void DACPortOutputDec(U16, bool pass=true);
    //void DACPortOutputSet(U16);

    void CheckImpedanceState();
    void CheckOverTemperatureState();
    void CheckSafetyState();
    void InitThread(SysParm *sysParm);

    //void timerEvent(QTimerEvent *event);


//for test
private:
    int counter;
    //int PWM_count;
    //bool PWM_Count_State;
    bool div_level_state;
    double waitTime;
    QTimer *checkpersec;
   // bool Check_state = false;
#ifdef __STRIKER_TEST__
    bool RF_Tick = false;
#endif
};
#endif // OUTPUTTHREAD_H
