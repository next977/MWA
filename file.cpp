#include <QDateTime>
#include <QDir>
#include "file.h"

#define UTC_TIME_ASIA_SEOUL    (9 * 60 * 60)   // 9h
File_RW::File_RW(QObject *parent, SysParm *sysParm) :
    QObject(parent)
{
    this->sysParm = sysParm;
    filenumber = 0;

}

File_RW::~File_RW()
{
}

QString File_RW::FileDate()
{
    QString file_date,file_time,file_datetime;
    file_date.sprintf("%02d-%02d-%02d", sysParm->mRfDeviceFactors->Date_year ,sysParm->mRfDeviceFactors->Date_month
                      ,sysParm->mRfDeviceFactors->Date_day);
    file_time.sprintf("%02d.%02d.%02d", sysParm->mRfDeviceFactors->Time_hour , sysParm->mRfDeviceFactors->Time_min
                      ,sysParm->mRfDeviceFactors->Time_sec); //"%02d:%02d:%02d"
    file_datetime = file_date +'_'+ file_time;
    return file_datetime;
}

void File_RW::FileOpen(File_Type file_type)
{
#ifndef __FEATURE_MICROWAVE__
    QString datetime_format = "yyyy-MM-dd_HH:mm:ss";
    QDateTime file_date = QDateTime::currentDateTime();
    QString filedate = file_date.toString(datetime_format);
#endif
    if(file_type == TYPE_LOGDATA)
    {
        QString dirPath = "LogData";
#ifndef _ONPC_
        QDir dir("/app/app"); //STRIKER Release path
#else
        QDir dir; //STRIKER DEBUG path
#endif
        filenumber ++;
        filename.sprintf("file_%d_",filenumber);
#ifndef __FEATURE_MICROWAVE__
        filename = filename+filedate;
#endif
        filename = filename + FileDate();
        //dir
        dir.mkpath(dirPath);
        filename = dir.absolutePath() + '/' + dirPath +'/' + filename;
    }
    else if(file_type == TYPE_MRFA_TABLE)
    {
#ifndef __FEATURE_MICROWAVE__
        QString dirPath = "MRFA_TABLE_DIR";
        //QDir dir("/app/app"); //STRIKER Release path
        QDir dir; //STRIKER DEBUG path
#endif
#ifndef _ONPC_
        filename = "/app/app/mrfa_table/test_mrfa_rf_table.c";
#else
        filename = "test_mrfa_rf_table.c";
#endif
#ifndef __FEATURE_MICROWAVE__
        //dir
        dir.mkpath(dirPath);
        filename = dir.absolutePath() + '/' + dirPath +'/' + filename;
#endif
    }
    else if(file_type == TYPE_TEMPERATURE)
    {
        filename = "/sys/class/thermal/thermal_zone0/temp";
    }

}

void File_RW::FileClose()
{
    //filenumber = 0;
}

void File_RW::FileWrite(uint16 Mode)
{
#ifndef __FEATURE_MICROWAVE__
    QString str, str2, str3, str4, str5, str6, str7;
    QStringList fields, data;
    //QFile file(QCoreApplication::applicationDirPath());
#endif
    QFile file(filename);
    QTextStream Write(&file);
    if(!file.open(QFile::WriteOnly|QFile::Append|QFile::Text)) //real file open
    {
        qDebug("File_Open_Fail_WriteOnly");
        return ;
    }
    else
    {
        qDebug("File_Open_Success_WriteOnly");
    }
    Write<< "\n<------------------------ Setting --------------------->\n\n" ;
    if(Mode == 0)
    {
        Write<< "Continue Mode \n" ;
        Write<< "Setting Time : " << sysParm->tmRFCogluationTime_SetTimer/60 << ':'
                                    << sysParm->tmRFCogluationTime_SetTimer%60 <<" s \n";
        Write<< "Watt : " << sysParm->mRfDeviceFactors->set_power <<" w \n";
        Write<< "Freq : " << sysParm->mRfDeviceFactors->set_frequency << " MHz \n";

    }

    Write<< "\n<------------------MICROWAVE continue Start-------------->\n\n" ;
    Write<< "FW, RW, Freq, Tm, Ts  \n";
    for(int i  = 0;  i < sysParm->mRfDeviceFactors->rf_F_Watt.size() ; i++)
    {
#ifndef __MICROWAVE_TEST__
        Write << sysParm->mRfDeviceFactors->rf_Count.at(i) << " , ";
#else
#endif
        Write << sysParm->mRfDeviceFactors->rf_F_Watt.at(i) << " , ";
        Write << sysParm->mRfDeviceFactors->rf_R_Watt.at(i) << " , ";
        Write << sysParm->mRfDeviceFactors->rf_Freq.at(i) << " , ";
        Write << sysParm->mRfDeviceFactors->rf_TimeMin.at(i) << " , ";
        Write << sysParm->mRfDeviceFactors->rf_TimeSec.at(i) << " \n";

    }

    Write<< "\n<--------------------------End-------------------------->\n" ;
    if(Mode == 0)
    {
        Write<< "End Time : " << sysParm->tmElapsedTime/60 << ':' << sysParm->tmElapsedTime%60 << " s \n";
    }

    Write<< "\n<------------------------------------------------------->\n" ;

#ifdef __FEATURE_MICROWAVE__
#else
    switch(sysParm->mRfDeviceFactors->state)
    {
        case 1:
                Write<< "\n<------------------------ Setting --------------------->\n\n" ;
                if(Mode == 0)
                {
                    Write<< "Puncture Mode \n" ;
                    Write<< "Puncture Time : " << sysParm->tmRFCogluationTime_Puncture/10 << '.'
                                                << sysParm->tmRFCogluationTime_Puncture%10 <<" s \n";
                    Write<< "Continue Time : " << sysParm->tmRFCogluationTime_Continue/10 << '.'
                                                << sysParm->tmRFCogluationTime_Continue%10 <<" s \n";
                }
                else if(Mode == 1)
                {
                    Write<< "Pulse Mode \n" ;
                    Write<< "     Pulse Time : " << sysParm->tmRFCogluationTime_Pulse <<" ms \n";
                    Write<< "     Idle  Time : " << sysParm->tmRFCogluationTime_Idle <<" ms \n";
                    Write<< "Pulse Mode Time : " << sysParm->tmRFCogluationTime_PulseMode <<" ms \n";
                    Write<< "          Count : " << sysParm->mRfDeviceFactors->rfPulseCount <<" \n";
                }
                Write<< "         Watt : " << sysParm->mRfDeviceFactors->rfPowerLevel << "\n" ;
                Write<< "\n<------------------Impedance Check Start--------------->\n\n" ;
                Write<< "count, sate, tW, tVM, VM, IM, CM, PM, DAC  \n";
                sysParm->mRfDeviceFactors->state = 0;
                break;
        case 2:
                Write<< "\n<------------------------Mode Start-------------------->\n\n" ;
                Write<< "count, sate, tW, tVM, VM, IM, CM, PM, DAC  \n";
                break;
        case 3:
                Write<< "\n<------------------------Ready------------------------->\n\n" ;
                Write<< "count, sate, tW, tVM, VM, IM, CM, PM, DAC  \n";
                break;
        case 4:
                Write<< "\n<------------------Puncture continue Start-------------->\n\n" ;
                Write<< "count, sate, tW, tVM, VM, IM, CM, PM, DAC  \n";
                break;
        case 5:
                Write<< "\n<--------------------------End-------------------------->\n" ;
                if(Mode == 0)
                {
                    Write<< "End Time : " << sysParm->tmElapsedTime/10 << '.' << sysParm->tmElapsedTime%10 << " s \n";
                }
                else if(Mode == 1)
                {
                    Write<< "End Time : " << sysParm->tmElapsedTime*10 << " ms \n";
                }
                Write<< "\n<------------------------------------------------------->\n" ;
                break;
    }

    if(sysParm->mRfDeviceFactors->state!=5)
    {
        Write<< sysParm->mRfDeviceFactors->counting << " , ";
        Write<< sysParm->mRfDeviceFactors->state << " , ";
        Write<< sysParm->mRfDeviceFactors->targetWatt << " , ";
        Write<< sysParm->mRfDeviceFactors->targetVMS << " , ";
        Write<< sysParm->mRfDeviceFactors->rfVoltageMeasured << " , ";
        Write<< sysParm->mRfDeviceFactors->rfImpedanceMeasured << " , ";
        Write<< sysParm->mRfDeviceFactors->rfCurrentMeasured << " , ";
        Write<< sysParm->mRfDeviceFactors->rfPowerMeasured/100 << " , ";
        Write<< sysParm->mRfDeviceFactors->tmDACValueToSetLast << "\n";
   }
    sysParm->mRfDeviceFactors->state = 0;
#endif
    file.close();
}

bool File_RW::FileRead(File_Type file_type)
{
     QString Line_str, Head, Body, Tail, Fileds_Value, Read_Temp_Str;
     QStringList fields;

     QFile file(filename);
     QTextStream Read(&file);

     bool readLine_State = false;
     qDebug()<<"filename = "<<filename;

     if(file_type == TYPE_MRFA_TABLE)
     {
         if(!file.open(QFile::ReadOnly|QFile::Text)) //real file open
         {
            qDebug() << "Mrfa_table/Test mrfa open fail";
            return false;
         }
         else
         {
             qDebug() << "Test Mrfa Open_ReadOnly_Success = " <<  filename;
         }

         while(!Read.atEnd())
         {
             Line_str = Read.readLine();
             if(readLine_State == false)
             {
                 if(Line_str.contains("TABLE_DEVICE_ID",Qt::CaseInsensitive))
                 {
                     fields = Line_str.split("=");
                     Head = fields.value(0);
                     Tail = fields.value(1);
                     sysParm->File_TABLE_DEVICE_ID = Tail.toInt();
                     qDebug()<< "sysParm->File_TABLE_DEVICE_ID = " << sysParm->File_TABLE_DEVICE_ID;


                 }

                 else if(Line_str.contains("TABLE_STEP_VOLTAGE",Qt::CaseInsensitive))
                 {
                     fields = Line_str.split("=");
                     Head = fields.value(0);
                     Tail = fields.value(1);
                     sysParm->File_TABLE_STEP_VOLTAGE = Tail.toInt();
                     qDebug()<< "sysParm->File_TABLE_STEP_VOLTAGE = " << sysParm->File_TABLE_STEP_VOLTAGE;

                 }
                 else if(Line_str.contains("TABLE_STEP_CURRENT",Qt::CaseInsensitive))
                 {
                     fields = Line_str.split("=");
                     Head = fields.value(0);
                     Tail = fields.value(1);
                     sysParm->File_TABLE_STEP_CURRENT = Tail.toInt();
                     qDebug()<< "File_TABLE_STEP_CURRENT = " << sysParm->File_TABLE_STEP_CURRENT;

                 }
                 else if(Line_str.contains("TABLE_STEP_RFPOWER_REF",Qt::CaseInsensitive))
                 {
                     fields = Line_str.split("=");
                     Head = fields.value(0);
                     Tail = fields.value(1);
                     sysParm->File_TABLE_STEP_RFPOWER_REF = Tail.toInt();
                     qDebug()<< "sysParm->File_TABLE_STEP_RFPOWER_REF = " << sysParm->File_TABLE_STEP_RFPOWER_REF;
                     readLine_State = true;
                 }
             }
             else
             {
                 if(Line_str.contains("TransTableRFPowerToDAC",Qt::CaseInsensitive))
                 {
                    Line_str = Read.readLine();
                    for(int i =0 ; i <42 ; i ++)
                    {
                        Line_str = Read.readLine();
                        //qDebug() << "MRFA_Read_readLine = " << Line_str;
                        fields = Line_str.split("},{");
                        Head = fields.value(0);
                        Body = fields.value(1);
                        Tail = fields.value(2);
                        fields = Head.split(',');
                        Fileds_Value = fields.value(1);
                        //sysParm->FileTransTableRFPowerToDAC[i][1].tmRealValue=Fileds_Value.toInt();
                        sysParm->FileTransTableRFPowerToDAC[i][0].tmDigitalValue=Fileds_Value.toInt();
                        Head = fields.value(0);
                        fields = Head.split("{{");
                        Fileds_Value = fields.value(1);
                        sysParm->FileTransTableRFPowerToDAC[i][0].tmRealValue=Fileds_Value.toInt();
                        //qDebug() << "TransTableRFPowerToDAC[i][0] = " << sysParm->FileTransTableRFPowerToDAC[i][0].tmDigitalValue;
                        //qDebug() << "TransTableRFPowerToDAC[i][0] = " << sysParm->FileTransTableRFPowerToDAC[i][0].tmRealValue;
                    }
                 }
                 else if(Line_str.contains("TransTableRFVoltage",Qt::CaseInsensitive))
                 {
                    Line_str = Read.readLine();
                    for(int i =0 ; i <42 ; i ++)
                    {
                        Line_str = Read.readLine();
                        //qDebug() << "MRFA_Read_readLine = " << Line_str;
                        fields = Line_str.split("},{");
                        Head = fields.value(0);
                        Body = fields.value(1);
                        Tail = fields.value(2);
                        fields = Head.split(',');
                        Fileds_Value = fields.value(1);
                        //sysParm->FileTransTableRFVoltage[i][1].tmRealValue=Fileds_Value.toInt();
                        sysParm->FileTransTableRFVoltage[i][0].tmDigitalValue=Fileds_Value.toInt();
                        Head = fields.value(0);
                        fields = Head.split("{{");
                        Fileds_Value = fields.value(1);
                        sysParm->FileTransTableRFVoltage[i][0].tmRealValue=Fileds_Value.toInt();
                        //qDebug() << "TransTableRFVoltage[i][0] = " << sysParm->FileTransTableRFVoltage[i][0].tmDigitalValue;
                       // qDebug() << "TransTableRFVoltage[i][0] = " << sysParm->FileTransTableRFVoltage[i][0].tmRealValue;
                    }
                 }
                 else if(Line_str.contains("TransTableRFCurrent",Qt::CaseInsensitive))
                 {
                    Line_str = Read.readLine();
                    for(int i =0 ; i <42 ; i ++)
                    {
                        Line_str = Read.readLine();
                        //qDebug() << "MRFA_Read_readLine = " << Line_str;
                        fields = Line_str.split("},{");
                        Head = fields.value(0);
                        Body = fields.value(1);
                        Tail = fields.value(2);
                        fields = Head.split(',');
                        Fileds_Value = fields.value(1);
                       // sysParm->FileTransTableRFCurrent[i][1].tmRealValue=Fileds_Value.toInt();
                        sysParm->FileTransTableRFCurrent[i][0].tmDigitalValue=Fileds_Value.toInt();
                        Head = fields.value(0);
                        fields = Head.split("{{");
                        Fileds_Value = fields.value(1);
                        sysParm->FileTransTableRFCurrent[i][0].tmRealValue=Fileds_Value.toInt();
                       // qDebug() << "TransTableRFCurrent[i][0] = " << sysParm->FileTransTableRFCurrent[i][0].tmDigitalValue;
                       // qDebug() << "TransTableRFCurrent[i][0] = " << sysParm->FileTransTableRFCurrent[i][0].tmRealValue;
                    }
                 }
             }


         }
     }


    file.close();
    return true;
}
