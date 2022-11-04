#ifndef SYSPARM_H
#define SYSPARM_H



#include <vector>
#include <QList>
#include "global.h"

#define MAX_RF_DEVICE_NUM 1

#define RF_COGLUATIONTIME_PUNCTURE_MAX 10
#define RF_COGLUATIONTIME_CONTINUE_MAX 20
#define RF_COGLUATIONTIME_PULSE_MAX 990
#define RF_COGLUATIONTIME_IDLE_MAX 990
#define RF_POWERLEVEL_MAX 50
#define RF_PULSECOUNT_MAX 100



//----------------------Main Check Impedance DAC ------------------//
#define CHECKIMPEDANCE_DAC_MIN 400 //min 100 ohm
#define CHECKIMPEDANCE_DAC_MAX 1000 //max 6000 ohm
//-----------------------Main Button time ms ----------------------//
#define RFSTARTBUTTON_TIME_MS 1000 //1000ms min



using namespace std;
enum State_EN
{
    STATE_SYSTEM_STANDBY,
    STATE_SYSTEM_READY,
    STATE_SYSTEM_ACTIVE,
    STATE_SYSTEM_OFF,
    STATE_SYSTEM_ALARM,
    STATE_SYSTEM_ERROR,
    STATE_SCREEN_MAIN,
    STATE_SCREEN_SETUP
};
#if 0
enum State_en
{
    STATE_SYSTEM_FAIL,
    STATE_SYSTEM_START,
    STATE_SYSTEM_INITIALIZING,
    STATE_SYSTEM_CALIBRATION,
    STATE_SYSTEM_MODE_SETTING,
    STATE_POWER_READYTOACTIVE,
    STATE_POWER_ACTIVE,
    STATE_POWER_STOP,
    STATE_ERROR_OVER_IMPEDANCE,
    STATE_ERROR_LOW_IMPEDANCE,
    STATE_ERROR_OVER_TEMPERATURE,
    STATE_ERROR_FAIL_PARSING,
    STATE_ERROR_FAIL_DATASETTING,
    STATE_LAST

};
#endif
enum Alert_State
{
    ALARM_ACTIVE_ANTENNA_NOT_CONNECT,
    ALARM_ACTIVE_ANTENNA_OVER_TEMPERATURE

};
enum Error_State
{
    ERROR_READY_ACTIVE_AMP_OVER_TEMPERATURE,
    ERROR_STANDBY_ANTENNA_NOT_IDENTIFIED,
    ERROR_STANDBY_READY_ANTENNA_OVERLAP_CHECK

};

enum System_Status
{
    PA_ENABLE,
    HIGH_TEMP,
    OVER_POWER,
    VSWR,
    PLL_UNLOCK,
    SYSTEM_STATUS_SIZE
};


enum UPDOWN_Btns_State
{
    UPDOWN_BTNS_CH1_WATT_UP,
    UPDOWN_BTNS_CH1_WATT_DOWN,
    UPDOWN_BTNS_CH2_WATT_UP,
    UPDOWN_BTNS_CH2_WATT_DOWN,

    UPDOWN_BTNS_CH1_TIME_UP,
    UPDOWN_BTNS_CH1_TIME_DOWN,
    UPDOWN_BTNS_CH2_TIME_UP,
    UPDOWN_BTNS_CH2_TIME_DOWN,

    UPDOWN_BTNS_CAL_POWER_UP,
    UPDOWN_BTNS_CAL_POWER_DOWN,
    UPDOWN_BTNS_CAL_FREQUENCY_UP,
    UPDOWN_BTNS_CAL_FREQUENCY_DOWN

};

enum Channel_State
{
    STATE_CHANNEL_CH1,
    STATE_CHANNEL_CH2,
    STATE_CHANNEL_SINGLE,
    STATE_CHANNEL_MULTI,
    STATE_CHANNEL_NONE
};
enum Mode_RF
{
    MODE_RF_CONTINUANCE,
    MODE_RF_TRACK


};

enum Click_State
{

    CLICK_NONE,
    CLICK_1CH_WATT,
    CLICK_2CH_WATT,
    CLICK_1CH_TIME,
    CLICK_2CH_TIME,
    CLICK_TIMER_DATA,
    CLICK_VOLUME,
    CLICK_DATE_DAY,
    CLICK_DATE_MONTH,
    CLICK_DATE_YEAR,
    CLICK_TIME_HOUR,
    CLICK_TIME_MINUTE,
    CLICK_TIME_AMPM,
    CLICK_BRIGHTNESS,

    CLICK_CALIBRATION_POWER_SETTING,
    CLICK_CALIBRATION_FREQUENCY_SETTING


};

typedef struct tagRfFactors
{
    uint8 RfDeivceID; //RF Device ID

    uint8 tmMRFAState;
    uint8 tmAmplifierState;
    uint8 tmLastError;

    uint16 tmReadADCVoltage;
    uint16 tmReadADCCurrent;
    uint16 tmReadTemp_ch1;
    uint16 tmReadTemp_ch2;
    uint16 tmReadTemp_ch3;
    uint16 tmReadTemp_ch4;
    bool tmReadTemp_ch1_state;
    bool tmReadTemp_ch2_state;
    bool tmReadTemp_ch3_state;
    bool tmReadTemp_ch4_state;
    bool tmReadTemp_multi_state;


    uint16 tmDACValueToSetLast;
    uint16 tmDACValueToSetSave;
    uint16 tmDACValueToSetforCal;


    uint16	tmRFonTime;

    uint16 rfPowerLevel_Init;
    uint16 rfPowerLevel;
    uint16 rfStepPowerLevel;
    uint16 rfTempLevel;
    uint16 rfPulseCount;


    uint16 rfLastCurrentMeasured;

    uint16 rfPowerMeasured;
    uint16 rfVoltageMeasured;
    uint16 rfCurrentMeasured;
    uint16 rfImpedanceMeasured;
    uint16 rfTemperatureMeasuredA;
    uint16 rfTemperatureMeasuredB;
    uint16 rfTemperaturePreviousA;
    uint16 rfTemperaturePreviousB;

    uint16 tmTooHighImpedanceDelayTime;
    uint16 tmTooHighTemperatureDelayTime;

    uint32 tmMinImpedance;
    //U32 tmImpedanceFactor;
    uint16 CheckImpedace_DAC;


    uint16 targetVMS;
    uint16 targetWatt;

    uint16 RMS_ADC_Volt;
    uint16 RMS_ADC_Current;
    
    
    uint32 mode_click_state;

    uint32 tmTotalJ;
    uint32 tmAccumulation;

    uint32 amp_temperature;
    uint32 get_power;
    uint32 set_power;
    uint32 get_frequency;
    uint32 set_frequency;
    uint32 set_pulse_width_value;
    uint32 set_pulse_period_value;

    uint32 mode_state;



    uint8 SIC_page0[32];
    uint8 SIC_page1[32];
    bool SIC_result;
    bool SIC_Parsing;
    bool SIC_Connect_State;

    //bool  tmAmplifierActive;      //RF amplifer enable

//<------------------Date file----------------->
    bool AMP_Connect_State;
    uint32 Volume;
    uint32 Date_year;
    uint32 Date_month;
    uint32 Date_day;
    uint32 Time_hour;
    uint32 Time_min;
    uint32 Time_sec;

    QString Time_ampm;



    Click_State click_state;
    Mode_RF mode_rf;

    State_EN state_system;
    State_EN current_state_system;

//<-------------------------------------------->
//<------------------Data Graph----------------->

    QList<uint32> rf_Count;
    QList<uint32> rf_State;
    QList<uint32> rf_IM;
    QList<uint32> rf_VM;
    QList<uint32> rf_CM;
    QList<uint32> rf_PM;
    QList<uint32> rf_DAC;
//<-------------------------------------------->
//<------------------MICROWAVE LogData----------------->
    QList<uint32> rf_F_Watt;
    QList<uint32> rf_R_Watt;
    QList<uint32> rf_Freq;
    QList<uint32> rf_TimeMin;
    QList<uint32> rf_TimeSec;

//<-------------------------------------------->




    uint32 counting;
    uint32 state;
    //uint32 Puncture_state;
    uint32 TEST_DACValueToSetLast;
    uint16 last_Impedance;
    uint16 V_div_factor;
    uint16 I_div_factor;

    bool calibration_state=false;




    bool System_result;
    bool tmRollOff;
    bool isStepwise;
    bool tmTooHighTempeatureFlag;
    bool tmTooHighImpedanceFlag;

//->->->->->->->->->->->->->->->->->->->->->->-> VIVA BD ->->->->->->->->->->->->->->->->->->->->->->->//

    uint16 combo_CheckSafetyCount;
//<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<- VIVA BD <-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-//

}RF_DEVICE_FACTORS;


class SysParm
{
public:
    const uint8 *SERIAL_NO;
    const uint8 *SW_VER;

    //en_RFDeviceOPMode 		tmOperationMode;

    typedef struct tagRFCTable
    {
        uint16 tmRealValue;
        uint16 tmDigitalValue;
    } RFC_TABLE;
    RFC_TABLE FileTransTableRFPowerToDAC[42][3];
    RFC_TABLE FileTransTableRFVoltage[42][3];
    RFC_TABLE FileTransTableRFCurrent[42][3];

    uint8      tmImpedanceControlMethod;

    uint16 tmRFPowerLevelPerModes[3] = {50, 50, 0};     //puncture, pulse, continuance // Watt
    uint16 tmRFPowerLevelExtPerModes[3] = {0, 0, 120};  //puncture, pulse, continuance // Temp

    uint32 tmRFCogluationTime_PerModes_C[3] = {10, 0, 25}; //puncture, pulse, continuance // Time Continue add
    uint32 tmRFCogluationTime_PerModes_P[3] = {0, 0, 0}; //puncture, pulse, continuance// Time Puncture add
    uint32 tmRFCgluationTimesPerModes[3] = {0, 0 ,0}; //puncture, pulse, continuance// Main Time sec save add

//    U32 tmRFSteptimePerModes[RF_DEVICE_OP_MODE_SIZE];

    uint32 tmElapsedTime;
    uint32 tmSystemElapsedTime;
#if 0
    uint32 Temperature_tmElapsedTime;

    uint32 Continuance_tmElapsedTime;
    uint32 Puncture_tmElapsedTime;

    uint32 Cutoff_tmElapsedTime;
#endif

#if 0
    uint32 tmCutoff = 0;
#endif
    uint32 tmRFCogluationTime_Puncture;
    uint32 tmRFCogluationTime_Continue;
    uint32 tmRFCogluationTime_Pulse;
    uint32 tmRFCogluationTime_Idle;
    uint32 tmRFCogluationTime_PulseMode;

    uint32 tmRFCogluationTime_SetTimer;


    uint32 tmRFCogluationTimeM;
    int32 modeSettingParam;


    uint32 File_TABLE_DEVICE_ID;
    uint32 File_TABLE_STEP_VOLTAGE;
    uint32 File_TABLE_STEP_CURRENT;
    uint32 File_TABLE_STEP_RFPOWER_REF;



    bool isDoTestforCal;

    RF_DEVICE_FACTORS *mRfDeviceFactors;
    vector <uint16> vRFPowerToDAC;
    vector <uint16> vRFVoltage;
    vector <uint16> vRFCurrent;
    vector <uint16> vTemperatureA;
    vector <uint16> vTemperatureB;

public:
    SysParm();
    bool Inspect_table();
    bool MRFA_Table_open();
    ~SysParm();

    uint16 GetLastADVoltage(void) const {return lastADVoltage;}
    uint16 GetLastVMS(void) const {return lastVMS;}
    uint16 GetLastADCurrent(void) const {return lastADCurrent;}
    uint16 GetLastADTemperatureA(void) const {return lastADTemperatureA;}
    uint16 GetLastADTemperatureB(void) const {return lastADTemperatureB;}
    uint16 GetLastDACPower(void) const {return lastDACPower;}

    uint16 getLimitPower(void) const;
    uint16 getMinPower(void) const;
    uint16 GetWattToDACValuefor1Watt(void) const;

    bool doTestforCal(void) const {return isDoTestforCal;}
    void vector_at();
private:
    uint16 lastADVoltage;
    uint16 lastVMS;
    uint16 lastADCurrent;
    uint16 lastADTemperatureA;
    uint16 lastADTemperatureB;
    uint16 lastADTemperatureBoard;
    uint16 lastDACPower;
    uint16 limitPower;
    uint16 minPower;
    uint16 dacValuefor1Watt;




};

#endif // SYSPARM_H
