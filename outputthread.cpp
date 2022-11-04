
#include "outputthread.h"
#include "gpiotools.h"
#include <QElapsedTimer>

using namespace gpioTools;


#define __INTERVAL_MONITORING__ 9//18//23//9   //30ms, 33 cycle
#define __INTERVAL_TEST__ false

OutputThread::OutputThread(QObject *parent, SysParm *sysParm) : QObject (parent), WorkerThread ()
{
    //slot signals
#if __INTERVAL_TEST__
    checkpersec = new QTimer;
    counter = 0;
    connect(checkpersec, SIGNAL(timeout()), this, SLOT(slot_dispCounter()));
    checkpersec->start(1000);
#endif
    this->sysParm = sysParm;
    this->mRfDeviceFactors = sysParm->mRfDeviceFactors;
    monitoringTask = new MonitoringTask(this, sysParm);



}

OutputThread::~OutputThread()
{
    if(monitoringTask) delete monitoringTask;
}


void OutputThread::loop()
{
    QElapsedTimer _mTimer;
    QElapsedTimer _countTimer;
    _mTimer.start();
    _countTimer.start();
    /*
     *  Start call function.
     *  Synchronous call function 일반적인 처리는 동기로
     *  Sensing adc
     */
    monitoringTask->GetAmplifierParameters();

    //Calibration mode

    //Impedance check

    //rf output for mode



    waitTime = __INTERVAL_MONITORING__ - _mTimer.elapsed();    // 30ms 32 - 34 per 1 second to the pc
    //waitTime = outputMode->Interbal_Monitoring - _mTimer.elapsed();
    //qDebug("%d",waitTime);
    if(waitTime >= 0)
    {
        std::this_thread::sleep_for(std::chrono::duration<double, std::milli> (waitTime));


    }
     //qDebug("%d",_countTimer.elapsed()); //outputMode->Interbal_Monitoring = 9  ----  9~10ms


}


