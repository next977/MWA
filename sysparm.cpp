#include "sysparm.h"
#include "mrfa_rf_table.h"
#include <QDebug>

extern const uint8 SERIAL_NO[];
extern const uint8 SW_VER[];

SysParm::SysParm()
{
    mRfDeviceFactors = new RF_DEVICE_FACTORS;

    mRfDeviceFactors->tmReadADCCurrent = 0;
    mRfDeviceFactors->tmReadADCVoltage = 0;

    mRfDeviceFactors->tmReadTemp_ch1 = 0;
    mRfDeviceFactors->tmReadTemp_ch2 = 0;
    mRfDeviceFactors->tmReadTemp_ch3 = 0;
    mRfDeviceFactors->tmReadTemp_ch4 = 0;
    mRfDeviceFactors->tmReadTemp_ch1_state = false;
    mRfDeviceFactors->tmReadTemp_ch2_state = false;
    mRfDeviceFactors->tmReadTemp_ch3_state = false;
    mRfDeviceFactors->tmReadTemp_ch4_state = false;
    mRfDeviceFactors->tmReadTemp_multi_state = false;


    mRfDeviceFactors->rfPowerLevel = 0;
    mRfDeviceFactors->rfPulseCount=0;
    mRfDeviceFactors->AMP_Connect_State = false;


    mRfDeviceFactors->targetVMS=0;
    mRfDeviceFactors->targetWatt=0;
    mRfDeviceFactors->counting = 0;

    mRfDeviceFactors->tmDACValueToSetLast = 0;
    mRfDeviceFactors->tmDACValueToSetforCal = 0;


    mRfDeviceFactors->amp_temperature = 0;
    mRfDeviceFactors->get_power = 0;
    mRfDeviceFactors->set_power = 0;
    mRfDeviceFactors->get_frequency = 0;
    mRfDeviceFactors->set_frequency = 0;


    tmElapsedTime= 0;
    tmSystemElapsedTime= 0;

    tmRFCogluationTime_Puncture = 0;
    tmRFCogluationTime_Continue = 0;
    tmRFCogluationTime_Pulse = 0;
    tmRFCogluationTime_Idle = 0;
    tmRFCogluationTime_PulseMode = 0;
    tmRFCogluationTime_SetTimer = 0;

    isDoTestforCal = false;

    mRfDeviceFactors->rf_Count.clear();
    mRfDeviceFactors->rf_F_Watt.clear();
    mRfDeviceFactors->rf_R_Watt.clear();
    mRfDeviceFactors->rf_Freq.clear();
    mRfDeviceFactors->rf_TimeMin.clear();
    mRfDeviceFactors->rf_TimeSec.clear();


    mRfDeviceFactors->SIC_result = false;
    mRfDeviceFactors->SIC_Parsing = true;
    mRfDeviceFactors->SIC_Connect_State = false;
    for(int i = 0; i < 32; i ++)
    {
        mRfDeviceFactors->SIC_page0[i] = 0;
        mRfDeviceFactors->SIC_page1[i] = 0;

    }
//    this->SERIAL_NO = (const U8*)&SERIAL_NO[0];
//    this->SW_VER = (const U8*)&SW_VER[0];

    mRfDeviceFactors->Volume = 4; //volume
    mRfDeviceFactors->Date_day = 24;
    mRfDeviceFactors->Date_month = 05;
    mRfDeviceFactors->Date_year = 22;
    mRfDeviceFactors->Time_hour = 1;
    mRfDeviceFactors->Time_min = 30;
    mRfDeviceFactors->Time_ampm = "pm";







    //MRFA_Table_open();

    Inspect_table();



}

SysParm::~SysParm()
{
    if(mRfDeviceFactors)
        delete mRfDeviceFactors;
}

uint16 SysParm::getMinPower() const
{
    return minPower;
}

uint16 SysParm::GetWattToDACValuefor1Watt() const
{
    return dacValuefor1Watt;
}

uint16 SysParm::getLimitPower() const
{
    return limitPower;
}
#if 0
void SysParm::vector_at()
{
    limitPower = vRFPowerToDAC.at(TABLE_STEP_RFPOWER_REF-11);
    minPower = vRFPowerToDAC.at(14);

    for(uint32 i=0; i<TABLE_STEP_RFPOWER_REF; i++)
    {
        if(vRFPowerToDAC.at(TABLE_STEP_RFPOWER_REF+i) >= 10 )
        {
            dacValuefor1Watt = vRFPowerToDAC.at(i);
            break;
        }
    }
}
bool SysParm::MRFA_Table_open()
{
    /*
    if()
    {
        qCritical("error MRFA_Table_Open_Fail");
        return false;
    }
    */

    uint16 v1, v2, v3, v4, v5, i;

    v1 = v2 = v3 = v4 = v5 = 0;

    for (i = 0; i < TABLE_STEP_RFPOWER_REF; i++)
    {
        if(v1 >  FileTransTableRFPowerToDAC[i][0].tmDigitalValue)
        {
            qCritical("error FileTransTableRFPowerToDAC_DigitalValue");
            return false;
        }
        v1 = FileTransTableRFPowerToDAC[i][0].tmDigitalValue;
        vRFPowerToDAC.push_back(v1);
    }
    lastDACPower = static_cast<uint16>(v1);
    //lastDACPower = v1;

    v1 = 0;
    for (i = 0; i < TABLE_STEP_RFPOWER_REF; i++)
    {
        if(v1 >  FileTransTableRFPowerToDAC[i][0].tmRealValue)
        {
            qCritical("error FileTransTableRFPowerToDAC_RealValue");
            return false;
        }
        v1 = FileTransTableRFPowerToDAC[i][0].tmRealValue;
        vRFPowerToDAC.push_back(v1);
    }



    for (i = 0; i < TABLE_STEP_VOLTAGE; i++)
    {
        if(v2 >  FileTransTableRFVoltage[i][0].tmDigitalValue )
        {
            qCritical("error FileTransTableRFVoltage_DigitalValue");
            return false;
        }
        v2 = FileTransTableRFVoltage[i][0].tmDigitalValue;
        vRFVoltage.push_back(v2);
    }
    lastADVoltage = static_cast<uint16>(v2);
    v2 = 0;
    for (i = 0; i < TABLE_STEP_VOLTAGE; i++)
    {
        if(v2 >  FileTransTableRFVoltage[i][0].tmRealValue )
        {
            qCritical("error FileTransTableRFVoltage_RealValue");
            return false;
        }
        v2 = FileTransTableRFVoltage[i][0].tmRealValue;
        vRFVoltage.push_back(v2);
    }
    lastVMS = v2;

    for (i = 0; i < TABLE_STEP_CURRENT; i++)
    {
        if(v3 >  FileTransTableRFCurrent[i][0].tmDigitalValue )
        {
            qCritical("error FileTransTableRFCurrent_DigitalValue");
            return false;
        }
        v3 = FileTransTableRFCurrent[i][0].tmDigitalValue;
        vRFCurrent.push_back(v3);
    }
    lastADCurrent = static_cast<uint16>(v3);
    v3=0;
    for (i = 0; i < TABLE_STEP_CURRENT; i++)
    {
        if(v3 >  FileTransTableRFCurrent[i][0].tmRealValue)
        {
            qCritical("error FileTransTableRFCurrent_RealValue");
            return false;
        }
        v3 = FileTransTableRFCurrent[i][0].tmRealValue;
        vRFCurrent.push_back(v3);
    }
    mRfDeviceFactors->rfLastCurrentMeasured = static_cast<uint16>(v3);
    v3=0;


    vector_at();
    return true;

}
#endif

bool SysParm::Inspect_table()
{
    uint16 v1, i;

    v1 = 0;


    for (i = 0; i < TABLE_STEP_TEMPERATURE; i++)
    {
        if(v1 >  TransTableTemperature[i][0].tmDigitalValue)
        {
            qCritical("error TransTableTemperature");
            return false;
        }
        v1 = TransTableTemperature[i][0].tmDigitalValue;
        vTemperatureA.push_back(v1);
    }
    lastADTemperatureA = static_cast<uint16>(v1);
    v1=0;
    for (i = 0; i < TABLE_STEP_TEMPERATURE; i++)
    {
        if(v1 >  TransTableTemperature[i][0].tmRealValue)
        {
            qCritical("error TransTableTemperature");
            return false;
        }
        v1 = TransTableTemperature[i][0].tmRealValue;
        vTemperatureA.push_back(v1);
    }


    return true;
}

