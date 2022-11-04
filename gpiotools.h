#ifndef GPIOTOOLS_H
#define GPIOTOOLS_H

#include "global.h"
#include <QThread>

#define mdelay(n) QThread::msleep(n)
#define udelay(n) QThread::usleep(n)

#define I2C_ADDR_MAX9768 0x4b //MAX9768 Volume Slave Address (Chip Address)
#define I2C_ADDR_MAX31343 0x68 //MAX31343 RTC Slave Address (Chip Address)
#define I2C_ADDR_SECUREDIC 0x18 //Secured IC Slave Address (Chip Address)

// defines the data direction (reading from I2C device) in I2C_start(),I2C_rep_start()
#define I2C_READ    1

// defines the data direction (writing to I2C device) in I2C_start(),I2C_rep_start()
#define I2C_WRITE   0

// Flags on I2C_read
#define ACK    1
#define NACK   0

#define EXPECT_ACK    1
#define EXPECT_NACK   0

#define POLL_LIMIT  200

// block modes
#define MODE_WRITEBLOCK     1
#define MODE_READBLOCK      2
#define MODE_WRITEREADBLOCK 3
#define MODE_WRITEPOLL      4
// misc constants

/*
 * MAX31856
 *
 */
#define MAX31856_CR0_REG 0x00         ///< Config 0 register
#define MAX31856_CR0_AUTOCONVERT 0x80 ///< Config 0 Auto convert flag
#define MAX31856_CR0_1SHOT 0x40       ///< Config 0 one shot convert flag
#define MAX31856_CR0_OCFAULT1 0x20    ///< Config 0 open circuit fault 1 flag
#define MAX31856_CR0_OCFAULT0 0x10    ///< Config 0 open circuit fault 0 flag
//#define MAX31856_CR0_CJ 0x08          ///< Config 0 cold junction disable flag
#define MAX31856_CR0_CJ 0x00          ///< Config 0 cold junction enable flag
#define MAX31856_CR0_FAULT 0x04       ///< Config 0 fault mode flag
#define MAX31856_CR0_FAULTCLR 0x02    ///< Config 0 fault clear flag

#define MAX31856_CR1_REG 0x01  ///< Config 1 register
#define MAX31856_MASK_REG 0x02 ///< Fault Mask register
#define MAX31856_CJHF_REG 0x03 ///< Cold junction High temp fault register
#define MAX31856_CJLF_REG 0x04 ///< Cold junction Low temp fault register
#define MAX31856_LTHFTH_REG                                                    \
  0x05 ///< Linearized Temperature High Fault Threshold Register, MSB
#define MAX31856_LTHFTL_REG                                                    \
  0x06 ///< Linearized Temperature High Fault Threshold Register, LSB
#define MAX31856_LTLFTH_REG                                                    \
  0x07 ///< Linearized Temperature Low Fault Threshold Register, MSB
#define MAX31856_LTLFTL_REG                                                    \
  0x08 ///< Linearized Temperature Low Fault Threshold Register, LSB
#define MAX31856_CJTO_REG 0x09  ///< Cold-Junction Temperature Offset Register
#define MAX31856_CJTH_REG 0x0A  ///< Cold-Junction Temperature Register, MSB
#define MAX31856_CJTL_REG 0x0B  ///< Cold-Junction Temperature Register, LSB
#define MAX31856_LTCBH_REG 0x0C ///< Linearized TC Temperature, Byte 2
#define MAX31856_LTCBM_REG 0x0D ///< Linearized TC Temperature, Byte 1
#define MAX31856_LTCBL_REG 0x0E ///< Linearized TC Temperature, Byte 0
#define MAX31856_SR_REG 0x0F    ///< Fault Status Register

#define MAX31856_FAULT_CJRANGE                                                 \
  0x80 ///< Fault status Cold Junction Out-of-Range flag
#define MAX31856_FAULT_TCRANGE                                                 \
  0x40 ///< Fault status Thermocouple Out-of-Range flag
#define MAX31856_FAULT_CJHIGH                                                  \
  0x20 ///< Fault status Cold-Junction High Fault flag
#define MAX31856_FAULT_CJLOW 0x10 ///< Fault status Cold-Junction Low Fault flag
#define MAX31856_FAULT_TCHIGH                                                  \
  0x08 ///< Fault status Thermocouple Temperature High Fault flag
#define MAX31856_FAULT_TCLOW                                                   \
  0x04 ///< Fault status Thermocouple Temperature Low Fault flag
#define MAX31856_FAULT_OVUV                                                    \
  0x02 ///< Fault status Overvoltage or Undervoltage Input Fault flag
#define MAX31856_FAULT_OPEN                                                    \
  0x01 ///< Fault status Thermocouple Open-Circuit Fault flag

/** Noise filtering options enum. Use with setNoiseFilter() */
typedef enum {
  MAX31856_NOISE_FILTER_50HZ,
  MAX31856_NOISE_FILTER_60HZ
} max31856_noise_filter_t;

/** Multiple types of thermocouples supported */
typedef enum {
  MAX31856_TCTYPE_B = 0b0000,
  MAX31856_TCTYPE_E = 0b0001,
  MAX31856_TCTYPE_J = 0b0010,
  MAX31856_TCTYPE_K = 0b0011,
  MAX31856_TCTYPE_N = 0b0100,
  MAX31856_TCTYPE_R = 0b0101,
  MAX31856_TCTYPE_S = 0b0110,
  MAX31856_TCTYPE_T = 0b0111,
  MAX31856_VMODE_G8 = 0b1000,
  MAX31856_VMODE_G32 = 0b1100,
} max31856_thermocoupletype_t;

/** Temperature conversion mode */
typedef enum {
  MAX31856_ONESHOT,
  MAX31856_ONESHOT_NOWAIT,
  MAX31856_CONTINUOUS
} max31856_conversion_mode_t;




namespace gpioTools {
    enum SPI_DEVICES {SPI_MAX31856, ALL_SPI_DEVICES};
    enum SPI_MAX31856_CH { CH1, CH2, CH3, CH4, Default };

    enum en_GPIOPort
    {
        GPIO_1,
        GPIO_2,
        GPIO_3,
        GPIO_4,
        GPIO_5,
        GPIO_6,
        GHIO_LAST
    };

    enum ELECTRODE_Type_en
    {
        ELECTRODE_NONE = 0,
        ELECTRODE_MONOPOLAR,
        ELECTRODE_BI_3CM,
        ELECTRODE_BI_7CM
    };

//<------------------------------------------->
    enum I2C_State
    {
        I2C_STATE_RTC,
        I2C_STATE_VOLUME,
        I2C_STATE_TEMP,
        I2C_STATE_SIC
    };
//<--------------------RTC & Volume----------------------->

    enum OM_State
    {
        OM_CLASSICPWM = 0xdf,
        OM_FLITERLESS = 0xd6
    };

    enum RTC_REGISTERS_State
    {
        RTC_REGISTERS_SECONDS = 0x06,
        RTC_REGISTERS_MINUTES = 0x07,
        RTC_REGISTERS_HOURS = 0x08,
        //REGISTERS_DFTW = 0x03,
        RTC_REGISTERS_DAY = 0x09,
        RTC_REGISTERS_DATES = 0x0A,
        RTC_REGISTERS_MONTHS = 0x0B,
        RTC_REGISTERS_YEARS = 0x0C,
        RTC_REGISTERS_ALARM1_SECONDS = 0x0D,
        RTC_REGISTERS_ALARM1_MINUTES = 0x0E,
        RTC_REGISTERS_ALARM1_HOURS = 0x0F,
        RTC_REGISTERS_ALARM1_DAYDATE = 0x10,
        RTC_REGISTERS_ALARM2_MINUTES = 0x13,
        RTC_REGISTERS_ALARM2_HOURS = 0x14,
        RTC_REGISTERS_ALARM2_DAYDATE = 0x15,
        RTC_REGISTERS_PWR_MGMT = 0x18,
        RTC_REGISTERS_TRICKLE_REG = 0x19
        //REGISTERS_CONTROL = 0x0E,
        //REGISTERS_STATUS = 0x0F,
        //REGISTERS_TC = 0x10
    };






    #define DACPortOutput(a) DAC_Send(a)

    static const uint32 GPIO_HIGH = 1;
    static const uint32 GPIO_LOW = 0;

#ifndef _ONPC_
    int32 uart_init();
    void uart_printf(char *pcFmt,...);
    void uart_binwrite(unsigned char *data, int length);
    char* uart_binread(int *length);
    void uart_close();
    int32 USB_uart_init();
    void USB_uart_printf(char *pcFmt,...);
    void USB_uart_write(char *pbString);
    void USB_uart_binwrite(unsigned char *data, int length);
    char* USB_uart_binread(int *length);
    void USB_uart_close();
#endif
    void gpio_init();
    volatile unsigned *GetEncorderAddress();
    void WriteGPIOPort(uint32 gpio, uint32 pin, bool flag);
    void WriteGPIOPortHigh(uint32 port_id, uint32 port_value);
    void WriteGPIOPortLow(uint32 port_id, uint32 port_value);
    uint32 ReadGPIOPort(uint32 gpio, uint32 pin);

    void AudioAMPControl_Enable();
    void AudioAMPControl_Disable();
    void RFSwitchLED_ON();
    void RFSwitchLED_OFF();

    void SetElectrodeType(ELECTRODE_Type_en eType);


    void spi_init();
    void spiAllclose();
    int spiOpen(SPI_DEVICES dn);
    int spiOpen(const int8 *devspi, uint8 *_mode, uint8 *_bits_per_word, uint32 *_speed);
    void spiWrite(int32 fdId, volatile uint8 *tbuf, uint32 length);
    void spiWriteRead(int32 fdId, volatile uint8 *tbuf, volatile uint8 *rbuf, uint32 length);
    int spiClose(int32 _spifd);


//<---------------------------------------------------->
    /*
     * MAX31856
     *
     */


    bool max31856_begin(void);

    void setConversionMode(max31856_conversion_mode_t mode);
    max31856_conversion_mode_t getConversionMode(void);

    void setThermocoupleType(max31856_thermocoupletype_t type);
    max31856_thermocoupletype_t getThermocoupleType(void);

    uint8_t readFault(void);

    void triggerOneShot(void);
    bool conversionComplete(void);

    float readCJTemperature(void);
    float readThermocoupleTemperature(void);

    void setTempFaultThreshholds(float flow, float fhigh);
    void setColdJunctionFaultThreshholds(int8_t low, int8_t high);
    void setNoiseFilter(max31856_noise_filter_t noiseFilter);





    void readRegisterN(uint8_t addr, uint8_t buffer[], uint8_t n);

    uint8_t readRegister8(uint8_t addr);

    uint32_t readRegister24(uint8_t addr);

    void writeRegister8(uint8_t addr, uint8_t reg);

////////////////////////////////////////////////////////////////////////
    volatile float SPI_Temp_Read(SPI_DEVICES dev, SPI_MAX31856_CH channel);


    void msDelay(int delay);
//<--------------------RTC & Volume----------------------->
    int RTC_Open(void);
    int Volume_Open(void);
    int RTC_Close(void);
    int Volume_Close(void);

    int MAX9768_Write(uint32 OM, uint32 DataValue);
    int MAX31343_Write(uint32 Registers , uint32 DataValue);
    unsigned char MAX31343_Read(uint32 REG_ADDR);

    int I2C_write(uchar data, int expect_ack);
    bool I2C_Write(I2C_State state, unsigned char *data, int length);
    unsigned char I2C_Read(I2C_State state, int ack, int *result);
    uchar I2C_read(int ack, int *result);
    //unsigned I2C_read(int ack, int *result);
    // I2C high level functions

    int I2C_start();
    void I2C_stop(void);
    int I2C_rep_start();
    int I2C_set_speed(int speed);
    // I2C high level functions
    int I2C_writeBlock(int i2c_addr, int addr, int block_len, uchar *block);
    int I2C_readBlock(int i2c_addr, int addr, int block_len, uchar *block, int skip_set_pointer);
    int I2C_writeReadBlock(int i2c_addr, int addr, int write_len, uchar *write_block,
                           int read_len, uchar *read_block);
    int I2C_readBlockPoll(int i2c_addr, int write_len, uchar *write_block,
                           uchar ones_mask, uchar zeros_mask, uchar *rslt);
    // I2C Utility functions


    int SecuredIC_Open(void);
    int SecuredIC_Close(void);


//<------------------------------------------------------>
}
#endif // GPIOTOOLS_H
