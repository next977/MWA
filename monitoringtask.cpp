#include "monitoringtask.h"
#include <vector>
#include <QTimer>
#include "mrfa_rf_table.h"
#include "gpiotools.h"
#include <math.h>


#define __INTERVAL_MONITORING__ 0.01   //30ms, 33 cycle
#define __INTERVAL_TEST__ false
#define DATA_AVERAGE_COUNT 7

#if defined(__GNUC__) || defined(__HP_aCC) || defined(__clang__)
    #define INLINE inline __attribute__((always_inline))
#else
    #define INLINE __forceinline
#endif


using namespace std;

INLINE unsigned int fast_upper_bound2(const vector<uint16>& vec, uint16 value)
{
    unsigned int m_len = vec.size()/2;
    unsigned int low = 0;
    unsigned int high = m_len;

    while (high - low > 1) {
        unsigned int probe = (low + high) / 2;
        unsigned int v = vec[probe];
        if (v > value)
            high = probe;
        else
            low = probe;
    }

    if (high == m_len)
        return m_len;
    else
        return high;
}


MonitoringTask::MonitoringTask(QObject *_parent, SysParm *_sysParm) : QObject(_parent)
{
#if __INTERVAL_TEST__
    QTimer *checkpersec = new QTimer(this);
    counter = 0;
    connect(checkpersec, SIGNAL(timeout()), this, SLOT(slot_dispCounter()));
    checkpersec->start(1000);
#endif
    this->sysParm = _sysParm;
}

MonitoringTask::~MonitoringTask()
{
    if(timer->isActive())
    {
        timer->stop();
    }
    delete timer;
}

void MonitoringTask::RunTimer()
{
 
    timer = new QTimer();
    timer->setInterval(30);
    timer->start();

#if __INTERVAL_TEST__
    connect(timer, SIGNAL(timeout()), this, SLOT(run()));
#else
    connect(timer, SIGNAL(timeout()), this, SLOT(GetAmplifierParameters()));
#endif
 
}

void MonitoringTask::GetAmplifierParameters()
{

    int32 i,j;
#if 0
    uint32 data_sum;
    uint16 tmp;
#endif
    float tmp, data_sum;

    RF_DEVICE_FACTORS *mRfDeviceFactors = sysParm->mRfDeviceFactors;

    mRfDeviceFactors->tmReadTemp_ch1 = gpioTools::SPI_Temp_Read(gpioTools::SPI_MAX31856, gpioTools::CH1);

    //qDebug("tmReadTemp_ch1 = %d",mRfDeviceFactors->tmReadTemp_ch1 );
    //qDebug("tmReadTemp_ch2 = %d",mRfDeviceFactors->tmReadTemp_ch2 );
    //gpioTools::msDelay(100);
#if 1

    mRfDeviceFactors->tmReadTemp_ch2 = gpioTools::SPI_Temp_Read(gpioTools::SPI_MAX31856, gpioTools::CH2);;
#if 0
    if(mRfDeviceFactors->tmReadTemp_ch1 <= 50 &&
       mRfDeviceFactors->tmReadTemp_ch1_state == false)
    {
        mRfDeviceFactors->tmReadTemp_ch1_state = true;
    }
    else if( mRfDeviceFactors->tmReadTemp_ch1 > 50 &&
             mRfDeviceFactors->tmReadTemp_ch1_state == true )
    {
        mRfDeviceFactors->tmReadTemp_ch1_state = false;
    }
    if(mRfDeviceFactors->tmReadTemp_ch2 <= 50 &&
       mRfDeviceFactors->tmReadTemp_ch2_state == false)
    {
        mRfDeviceFactors->tmReadTemp_ch2_state = true;
    }
    else if( mRfDeviceFactors->tmReadTemp_ch2 > 50 &&
             mRfDeviceFactors->tmReadTemp_ch2_state == true )
    {
        mRfDeviceFactors->tmReadTemp_ch2_state = false;
    }
#endif


#if 0
    qDebug("tmReadTemp_ch1 = %d",mRfDeviceFactors->tmReadTemp_ch1 );
    qDebug("tmReadTemp_ch2 = %d",mRfDeviceFactors->tmReadTemp_ch2 );
    gpioTools::msDelay(500);
#endif
#endif
#if 0
    //////////SIC Start///////////////////////////
    if( 0 <= mRfDeviceFactors->tmReadTemp_ch1 && mRfDeviceFactors->tmReadTemp_ch1 <=100 &&
             mRfDeviceFactors->SIC_Connect_State == false)
    {
        mRfDeviceFactors->SIC_Connect_State = true;
        emit sig_SecuredIC_Start();
    }
    else if( 0 <= mRfDeviceFactors->tmReadTemp_ch2 && mRfDeviceFactors->tmReadTemp_ch2 <=100 &&
             mRfDeviceFactors->SIC_Connect_State == false)
    {
        mRfDeviceFactors->SIC_Connect_State = true;
        emit sig_SecuredIC_Start();
    }
    else
    {
        mRfDeviceFactors->SIC_Connect_State = false;
        mRfDeviceFactors->SIC_result = false;
        mRfDeviceFactors->SIC_Parsing = true;
    }
#endif
    //////////////SIC Parsing//////////////////////////

    //qDebug() << "TempA ADC Data = " <<mRfDeviceFactors->tmReadADCTemperatureA;

    //gpioTools::msDelay(100);

    //MRFA_TranslateRFTable();

}

void MonitoringTask::slot_SIC_Display()
{
    uint32 CheckID_Length = 4;
    uint32 Lot_Length = 15;
    uint32 Type1_Length = 2;
    uint32 Type2_Length = 2;
    uint32 Reserved_Length = 2;

    uint8 CheckID[CheckID_Length];
    uint8 Lot[Lot_Length];
    uint8 Type1[Type1_Length];
    uint8 Type2[Type2_Length];
    uint8 Reserved[Reserved_Length];

    if(sysParm->mRfDeviceFactors->SIC_result == true && sysParm->mRfDeviceFactors->SIC_Parsing == true)
    {
        for(int i = 0; i < CheckID_Length; i++)
        {
            CheckID[i] = sysParm->mRfDeviceFactors->SIC_page0[i];
        }
        if(CheckID[0] == 'S' &&
           CheckID[1] == 'T' &&
           CheckID[2] == 'N' &&
           CheckID[3] == 'D' )
        {
            for(int i = 0; i < Lot_Length; i++)
            {
                Lot[i] = sysParm->mRfDeviceFactors->SIC_page0[i+CheckID_Length];
            }
            for(int i = 0; i < Type1_Length; i++)
            {
                Type1[i] = sysParm->mRfDeviceFactors->SIC_page0[i+CheckID_Length+Lot_Length];
                qDebug("Type1[%d] = %c", i , Type1[i]);
            }
            for(int i = 0; i < Type2_Length; i++)
            {
                Type2[i] = sysParm->mRfDeviceFactors->SIC_page0[i+CheckID_Length+Lot_Length+Type1_Length];
                qDebug("Type2[%d] = %c", i , Type2[i]);
            }
            for(int i = 0; i < Reserved_Length; i++)
            {
                Reserved[i] = sysParm->mRfDeviceFactors->SIC_page0[i+CheckID_Length+Lot_Length+Type1_Length+Type2_Length];
            }


            sysParm->mRfDeviceFactors->SIC_Parsing = false;
            qDebug("needle connect");

        }

    }

}

#if 0
void MonitoringTask::MRFA_TranslateRFTable()
{
    uint32 j;

    uint16 rfa,rfb;
    uint16 div_factor;

    uint16 rfCurrentMeasured, rfVoltageMeasured, rfPowerMeasured, rfTemperatureMeasuredA,rfTemperatureMeasuredB;
    RF_DEVICE_FACTORS *mRfDeviceFactors = sysParm->mRfDeviceFactors;

    rfa = mRfDeviceFactors->tmReadADCTemperatureA;
    j = fast_upper_bound2(sysParm->vTemperatureA, rfa);

    if(j == TABLE_STEP_TEMPERATURE)
    {
        rfb = sysParm->vTemperatureA.at(TABLE_STEP_TEMPERATURE + j - 1);
    }
    else if(rfa <  sysParm->vTemperatureA.at(j-1)) //TransTableRFVoltage[j-1][i].tmDigitalValue )
    {
        div_factor = (  rfa * 100 ) / (sysParm->vTemperatureA.at(j-1));
        rfb = ( sysParm->vTemperatureA.at(TABLE_STEP_TEMPERATURE + j-1) * div_factor ) /100;
    }
    else
    {
        div_factor = ( ( rfa - sysParm->vTemperatureA.at(j-1) ) * 100 ) /
                     (	sysParm->vTemperatureA.at(j) - sysParm->vTemperatureA.at(j-1) );

        rfb = sysParm->vTemperatureA.at(TABLE_STEP_TEMPERATURE + j-1) +
                                 ( ( sysParm->vTemperatureA.at(TABLE_STEP_TEMPERATURE + j) - sysParm->vTemperatureA.at(TABLE_STEP_TEMPERATURE + j-1))* div_factor ) /100;
    }
    rfTemperatureMeasuredA = rfb;




    mRfDeviceFactors->rfTemperatureMeasuredA = rfTemperatureMeasuredA/10;
    //qDebug() << "TempA Real Data = " <<mRfDeviceFactors->rfTemperatureMeasuredA;

}

#endif

uint16 MonitoringTask::IOADCSPIPort(int32 n_cs, int32 channel)
{
    uint16 adc_value = 0;
    Q_UNUSED(n_cs);
    switch(channel)
    {
        case 2: //voltage
            //adc_value = mRfDeviceFactors->tmReadADCVoltage++;
            if(adc_voltage++ > 60000)
            {
                adc_voltage = 0;
            }
            adc_value = adc_voltage;

            break;

        case 1: //current
            //adc_value = mRfDeviceFactors->tmReadADCCurrent++;
            if(adc_current++ > 40000)
            {
                adc_current = 0;
            }
            adc_value = adc_current;
            break;

        case 0: //temperature
            //adc_value = mRfDeviceFactors->tmReadADCTemperature++;
            if(adc_temp++> 2000)
            {
                adc_temp = 0;
            }
            adc_value = adc_temp;
            break;

    }

#if 0
    //if(channel == 0)
    {
        mRfDeviceFactors->tmReadADCTemperature++;
        if(sysParm->mRfDeviceFactors->tmReadADCTemperature > 60000)
            sysParm->mRfDeviceFactors->tmReadADCTemperature = 0;
        //else if(sysParm->mRfDeviceFactors->tmReadADCVoltage >  55279)
//           sysParm->mRfDeviceFactors->tmReadADCVoltage = 10000;
    }
    return mRfDeviceFactors->tmReadADCTemperature;
#endif
    return adc_value;
}



uint16 MonitoringTask::MedianFilter(int32 deviceid, int32 index, uint16 data)
{
    uint16 i,j,tmp;
    uint16 *p_sort, *p_data;


    switch(index)
    {
        case 0:
            p_data = static_cast<uint16*>(tm_median_voltage[deviceid]);
            p_sort = static_cast<uint16*>(tm_median_voltage_sort[deviceid]);
            break;

        case 1:
            p_data = static_cast<uint16*>(tm_median_current[deviceid]);
            p_sort = static_cast<uint16*>(tm_median_current_sort[deviceid]);
            break;

        case 2:
            p_data = static_cast<uint16*>(tm_median_tempA[deviceid]);
            p_sort = static_cast<uint16*>(tm_median_temp_sortA[deviceid]);
            break;

        case 3:
            p_data = static_cast<uint16*>(tm_median_tempB[deviceid]);
            p_sort = static_cast<uint16*>(tm_median_temp_sortB[deviceid]);
            break;

    }

    //Filtering
    p_sort[0] = p_data[0]=   p_data[1];
    p_sort[1] = p_data[1]=   p_data[2];
    p_sort[2] = p_data[2]=   data;
    //sort
    for( i =0; i < MEDIAN_ORDER - 1; i++)
    {
        for( j =i + 1; j < MEDIAN_ORDER; j++)
        {
            if( p_sort[i] < p_sort[j] )
            {
                tmp = p_sort[j];
                p_sort[j] = p_sort[i];
                p_sort[i] =tmp ;
            }
        }
    }

    //Get Median
    return p_sort[1];

}



void MonitoringTask::slot_dispCounter()
{
    qDebug("MonitoringTask run %d per 1seconds", counter);
    counter = 0;
}


void MonitoringTask::run()
{

    /*
     *  Start call function.
     *  Synchronous call function 일반적인 처리는 동기로
     *  Sensing adc
     */

    GetAmplifierParameters();

    /*
     *  End call function.
     */


#if __INTERVAL_TEST__
    counter++;
#endif

}
