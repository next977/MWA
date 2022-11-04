#ifndef CRCCHECK_H
#define CRCCHECK_H

#define CRC_16_POLYNOMIAL 0x1021
#define CRC_16_SEED 0x0000
#define CRC_16_STEP_SEED (~((uint16) CRC_16_SEED))
#define CRC_TAB_SIZE 256

#include <QObject>
#include <QDebug>
#include "global.h"
class CRCCheck : public QObject
{
    Q_OBJECT
public:
    explicit CRCCheck(QObject *parent = nullptr);
    ~CRCCheck();
    uint16 crc_16;
    uint8 data;
    uint16 Generate_CRC (uint8 *buf_ptr, int len) ;
signals:

public slots:
};

#endif // CRCCHECK_H
