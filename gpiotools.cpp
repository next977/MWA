
#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <QDebug>
#include "gpiotools.h"
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <stdarg.h>
#include <termio.h>
#include <sys/timeb.h>
#include <signal.h>

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

//<--------------------RTC & Volume----------------------->


// Flags on I2C_read
#define ACK    1
#define NACK   0


#define mSleep(n) usleep(n*1000)
//<------------------------------------------------------>

using namespace std;

namespace gpioTools {

#define DECODE_MODE		0x09
#define INTENSITY_CONTROL	0x0A
#define SCAN_LIMIT			0x0B
#define SHUTDOWN			0X0C
#define FEATURE				0X0E
#define DISPLAY_TEST		0X0F

static const uint32 IOMUXC = 0x020E0000;
static const uint32 IOMUXC_SW_MUX_CTL_PAD_SD4_DATA0 = 0x020E031C;
static const uint32 GPIO_BASE = 0x0209C000;

//GPIO Memory Map
static const uint32 GPIO1_DR = 0x0209C000;
static const uint32 GPIO2_DR = 0x020A0000;
static const uint32 GPIO3_DR = 0x020A4000;
static const uint32 GPIO4_DR = 0x020A8000;
static const uint32 GPIO5_DR = 0x020AC000;
static const uint32 GPIO6_DR = 0x020B0000;


static volatile uint32 *gpio1_address;
static volatile uint32 *gpio2_address;
static volatile uint32 *gpio3_address;
static volatile uint32 *gpio4_address;
static volatile uint32 *gpio5_address;
static volatile uint32 *gpio6_address;

static const uint32 GPIO_OUTPUT = 1;
static const uint32 GPIO_INPUT = 0;

static int32 _spifd[ALL_SPI_DEVICES];
static uint8 _mode[ALL_SPI_DEVICES];
static uint8 _bits_per_word[ALL_SPI_DEVICES];
static uint32 _speed[ALL_SPI_DEVICES];

static volatile uint8 _tx_buf[ALL_SPI_DEVICES][10];
static volatile uint8 _rx_buf[ALL_SPI_DEVICES][10];

static struct spi_ioc_transfer _io_trans[2];

static int32 uart_fd;
static int32 usb_uart_fd;
struct termios oldtio , newtio;
const char *TAG = "MAX31856";
const int8 *USB0_uart = "/dev/ttyUSB0";

const int8 *usb_uart = "/dev/ttymxc2";
const int8 *pump_uart = "/dev/ttymxc3";
#define BUFF_SIZE 256
#define BAUDRATE B115200
char readBuf[BUFF_SIZE];
unsigned char *readbuffer;


#ifdef _ONPC_
#define SETBIT(ADDRESS, BIT)
#define CLEARBIT(ADDRESS, BIT)
#define TESTBIT(ADDRESS, BIT)

#else
#define SETBIT(ADDRESS, BIT) (ADDRESS |= (1<<BIT))
#define CLEARBIT(ADDRESS, BIT) (ADDRESS &= ~(1<<BIT))
#define TESTBIT(ADDRESS, BIT) (ADDRESS & (1<<BIT))
#endif
#define IOMUXC_BASE_ADDR		0x020E0000




int fd_SIC = -1;

int fd_volume;
int fd_RTC;
int fd_Temp_I2C;

bool check_write = false;
bool i2cdebug = true;
bool packetdebug = false;
unsigned char I2C_address;

bool initialized = false;
max31856_conversion_mode_t conversionMode;

void msDelay(int delay)
{
   int i;


   // special case while setting up adapter, long power up delay
   if (delay > 100)
   {
      for (i = 0; i < delay/100; i++)
      {
         mSleep(100);
         printf(".");
      }
   }
   else
      mSleep(delay);
}
//---------------------------------------------------------------------------
//-------- Secured IC I2C Low-level functions
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// I2C Stop - Terminates the data transfer and releases the I2C bus
//



void I2C_stop(void)
{
   unsigned char sendpacket[2];

   // debug
   if (i2cdebug)
      ;//printf("P\n");
}

//---------------------------------------------------------------------------
// I2C Start - Issues a start condition and sends address and transfer
// direction
//
// Returns:  TRUE (1): start successful
//           FALSE (0): start failed
//
int I2C_start()
{
   // debug
   if (i2cdebug)
      ;//printf("S ");

   check_write = true;

   return true;
}

//---------------------------------------------------------------------------
// I2C Repeated start - Issues a repeated start condition and sends address
// and transfer direction
//
// Returns:  TRUE (1): repeated start successful
//           FALSE (0): repeated start failed
//
int I2C_rep_start()
{
   // debug
   if (i2cdebug)
      ;//printf("Sr ");

   check_write = true;

   return true;
}



//---------------------------------------------------------------------------
// I2C Set Speed
//
// 'speed':   1 fast mode
//            0 standard mode
//
// Returns:  TRUE (1): repeated start successful
//           FALSE (0): repeated start failed
//
int I2C_set_speed(int speed)
{

   // debug
   if (i2cdebug)
      ;//printf("<SP ");


   return true;
}

//---------------------------------------------------------------------------
//-------- I2C High-Level functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// I2C writeBlock
//
// 'i2c_addr'  : I2C addres
// 'addr'      : Memory address
// 'block_len' : write block length
// 'block'     : block buffer
//
// Returns:  TRUE (1): block written
//           FALSE (0): problem writing block
//
int I2C_writeBlock(int i2c_addr, int addr, int block_len, uchar *block)
{
   return 0;
}

//---------------------------------------------------------------------------
// I2C readBlock
//
// 'i2c_addr'  : I2C addres
// 'addr'      : Memory address
// 'block_len' : read block length
// 'block'     : block buffer
// 'skip_set_pointer : flag to indicate the device address pointer does not need to
//               be set before starting the read
//
// Returns:  TRUE (1): block read
//           FALSE (0): problem reading block
//
int I2C_readBlock(int i2c_addr, int addr, int block_len, uchar *block, int skip_set_pointer)
{
    return 0;
}

//---------------------------------------------------------------------------
// I2C writeReadBlock - Write and then read a block after a repeated start.
//
// 'i2c_addr'    : I2C addres
// 'addr'        : Memory address
// 'write_len'   : write block length
// 'write_block' : block buffer
// 'read_len'    : read block length
// 'read_block'  : block buffer
//
// Returns:  TRUE (1): block written and read
//           FALSE (0): problem reading block
//
int I2C_writeReadBlock(int i2c_addr, int addr, int write_len, uchar *write_block,
                       int read_len, uchar *read_block)
{
    return 0;
}

//---------------------------------------------------------------------------
// I2C readBlockPoll, write a block of data, do a repeated start, and then
// read waiting for a bit mask to match the 1's.
//
// 'i2c_addr'    : I2C addres
// 'write_len'   : write block length
// 'write_block' : block buffer
// 'ones_mask'   : bit mask indicating which bits must be 1's to stop polling
// 'zeros_mask'  : bit mask indicating which bits must be 0's to stop polling
// 'rslt'        : pointer to byte that contains the final value read when polling
//
// Returns:  TRUE (1): poll completed successfully
//           FALSE (0): poll on completed
//
int I2C_readBlockPoll(int i2c_addr, int write_len, uchar *write_block, uchar ones_mask, uchar zeros_mask, uchar *rslt)
{
   return 0;
}

//---------------------------------------------------------------------------
//-------- I2C Utility functions
//---------------------------------------------------------------------------

//<------------------------------------------------------>
int SecuredIC_Open(void)
{
    fd_SIC = open("/dev/i2c-1", O_RDWR);

    if (fd_SIC < 0) {
        perror("Open error ");
        return false;
    }
    if (ioctl(fd_SIC, I2C_SLAVE, I2C_ADDR_SECUREDIC) < 0) {
        perror("ioctl error ln 24 ");
        return false;
    }

   return true;
}

int SecuredIC_Close(void)
{
    close(fd_SIC);
}

int RTC_Open(void)
{
    fd_RTC = open("/dev/i2c-2", O_RDWR); //i2c-2 : i2c bus number
                                     //i2c Checking : i2cdetect -y 0~2
    if (fd_RTC < 0) {
        printf("Open error: %s\n", strerror(errno));
        return 1;
    }


    if (ioctl(fd_RTC, I2C_SLAVE, I2C_ADDR_MAX31343) < 0) {
        printf("ioctl error ln 24 : %s\n", strerror(errno));
        return 1;
    }
}

int Volume_Open(void)
{
    fd_volume = open("/dev/i2c-1", O_RDWR); //i2c-0 : i2c bus number
                                     //i2c Checking : i2cdetect -y 0~2
    if (fd_volume < 0) {
        printf("Open error: %s\n", strerror(errno));
        return 1;
    }

    if (ioctl(fd_volume, I2C_SLAVE, I2C_ADDR_MAX9768) < 0) {
        printf("ioctl error ln 24 : %s\n", strerror(errno));
        return 1;
    }

}

int RTC_Close(void)
{
    close(fd_RTC);
}

int Volume_Close(void)
{
    close(fd_volume);
}


int I2C_write(uchar data, int expect_ack)
{

    return true;
}



bool I2C_Write(I2C_State state, unsigned char *data, int length)
{
    /*
    if(i2cdebug)
    {
        for(int i=0; i<length; i++)
        {
            printf(">%02X ", *(data+i));
        }
        printf("\n");
    }
    */
    //printf("state = %d , data = %02X, length = %d",state, *data,length);
    switch(state)
    {
        case I2C_STATE_RTC:
                qDebug("I2C_STATE_RTC Write");

                for(int i=0; i<length; i++)
                {
                    qDebug(">%02X ", *(data+i));
                }
                //printf("I2C_STATE_RTC");
                if (write(fd_RTC, data, length) != length)
                {
                    perror("RTC_write error");
                    return false;
                }
                break;
        case I2C_STATE_VOLUME:
                if (write(fd_volume, data, length) != length)
                {
                    perror("volume_write error");
                    return false;
                }
                break;
        case I2C_STATE_SIC:
                //printf("I2C_STATE_SIC");
                if (write(fd_SIC, data, length) != length)
                {
                    perror("SIC_write error");
                    return false;
                }
                break;


        default :
                break;

    }
    //printf("I2C_Write_Success");
    return true;

}


unsigned char I2C_Read(I2C_State state, int ack, int *result)
{
    unsigned char readbuffer[10];

    //readbuffer = static_cast<unsigned char*>(malloc(sizeof(unsigned char) * 10)) ;
    switch(state)
    {
        case I2C_STATE_RTC:
                if(read(fd_RTC, readbuffer, 1) != 1)
                {
                    perror("RTC_read error");
                    return 0;
                }
                //printf("I2C_Read readbuffer[0]= %02X \n", readbuffer[0]);
                //qDebug("I2C_Read readbuffer[0]= %02X", readbuffer[0]);
                for(int i=0; i<10; i++)
                {
                    qDebug("<Readbuffer %d = %02X ", i, *(readbuffer+i));
                }
                break;

        default :
                break;

    }

    *result = true;
    return readbuffer[0];

    return false;

}

//---------------------------------------------------------------------------
// I2C Read - read one byte from the I2C device
//
// Parameters:  ack - (1) send ACK, request more data from device
//                    (0) send NAK, read is followed by a stop condition
//              result - pointer to integer containing the operation result
//                       (0) to indicate a failure (1) to indication success
//
// Returns:  byte read from I2C device
//
uchar I2C_read(int ack, int *result) //SIC I2C_read
{
    unsigned char readbuffer[10];

    if(read(fd_SIC, readbuffer, 1) != 1){
        perror("read error");
        return 0;
    }
    else
    {
        *result = true;
        return readbuffer[0];
    }

    *result = false;

    return false;
}



int MAX9768_Write(uint32 OM, uint32 DataValue) //OM : OutputModulation
{
    int result;
    unsigned char sendbuffer[10], readbuffer[10];
    int cnt = 0, poll_count=0;
    unsigned char status;
    i2c_smbus_data smbusData;
    smbusData.byte= (uint8)OM;


    sendbuffer[cnt++] = OM; // OutputModulation
    if(!I2C_Write(I2C_STATE_VOLUME, sendbuffer, cnt))
    {
        printf("send error\n");
        return 0;
    }
    I2C_Read(I2C_STATE_VOLUME, ACK, &result);

    cnt=0;
    sendbuffer[cnt++] = DataValue; //Volume Data Value (0x00~0x3f)
    if(!I2C_Write(I2C_STATE_VOLUME, sendbuffer, cnt))
    {
        printf("send error\n");
        return 0;
    }
    I2C_Read(I2C_STATE_VOLUME, ACK, &result);

}

int MAX31343_Write(uint32 Registers , uint32 DataValue) //RTC
{
    unsigned char sendbuffer[10];
    int cnt = 0;
    fd_RTC = open("/dev/i2c-2", O_RDWR); //i2c-2 : i2c bus number
                                     //i2c Checking : i2cdetect -y 0~2
    if (fd_RTC < 0) {
        printf("Open error: %s\n", strerror(errno));
        return 1;
    }
    if (ioctl(fd_RTC, I2C_SLAVE, I2C_ADDR_MAX31343) < 0) {
        printf("ioctl error ln 24 : %s\n", strerror(errno));
        return 1;
    }
    sendbuffer[cnt++] = Registers; //Data Register 0x05
    sendbuffer[cnt++] = DataValue; //RTC Data Value 0x10 (0x00~0xff)
    if(!I2C_Write(I2C_STATE_RTC, sendbuffer, cnt))
    {
        printf("send error\n");
        return 0;
    }
    close(fd_RTC);
}

unsigned char MAX31343_Read(uint32 REG_ADDR)
{
    struct i2c_rdwr_ioctl_data data;
    data.msgs = (struct i2c_msg *)malloc(2 * sizeof(struct i2c_msg));
    unsigned char sendbuf[sizeof(unsigned char) + 1] = {0};
    unsigned char recvbuf[sizeof(unsigned char) + 1] = {0};
    uint32 I2C_ADDR = I2C_ADDR_MAX31343;
    fd_RTC = open("/dev/i2c-2", O_RDWR); //i2c-2 : i2c bus number
                                    //i2c Checking : i2cdetect -y 0~2
    if (fd_RTC < 0) {
       printf("Open error: %s\n", strerror(errno));
       return 1;
    }
    //qDebug("Start MAX31343_Read");

    //2.read reg value
    data.nmsgs = 2;
    data.msgs[0].len = 1;//  reg
    data.msgs[0].addr = I2C_ADDR; //i2c
    data.msgs[0].flags = 0; //write flag
    data.msgs[0].buf = sendbuf;//     2 Byte
    data.msgs[0].buf[0] = REG_ADDR;//  reg

    data.msgs[1].len = 1;//  reg
    data.msgs[1].addr = I2C_ADDR; //i2c
    data.msgs[1].flags = 1; //read flag
    data.msgs[1].buf = recvbuf;//

    ioctl(fd_RTC,I2C_RDWR,(unsigned long)&data);
    //qDebug("buf[0] = %x",data.msgs[1].buf[0]);

    close(fd_RTC);
    return data.msgs[1].buf[0];


}

volatile unsigned *mapRegAddr(unsigned long baseAddr)
{
    void *regAddrMap = MAP_FAILED;
    int mem_fd = 0;

    if(!mem_fd)
    {
        if((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0)
        {
            cout << "Cant open /dev/mem" << endl;
            exit(1);
        }
    }

    regAddrMap = mmap(NULL, 32*8,
                      PROT_READ|PROT_WRITE|PROT_EXEC,
                      MAP_SHARED, mem_fd, baseAddr);

    if(regAddrMap == MAP_FAILED)
    {
        cout << "mmap is failed" << endl;
        exit(1);
    }


    if(close(mem_fd) < 0)
    {
        cout << "cant close /dev/mem" << endl;
        exit(1);

    }
    return (volatile unsigned *)regAddrMap;

}
#ifndef _ONPC_
int32 uart_init()
{

    //unsigned long baudrate = 115200;
    struct termios options;
    char *tx = "STARmed";
#ifdef _ONPC_
    uart_fd = open(USB0_uart, O_RDWR | O_NOCTTY);
#else
    uart_fd = open(pump_uart, O_RDWR | O_NOCTTY);
#endif

    fcntl(uart_fd, F_SETFL, 0);

    tcgetattr(uart_fd,&oldtio); /* ĮöĀį žģÁĪĀŧ oldtioŋĄ ĀúĀå */

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR | IXOFF;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VMIN]     = 0;   /* ÃÖžŌ 5 đŪĀÚ đÞĀŧ ķ§ąîÁø blocking*/
    newtio.c_cc[VTIME]    = 1;   /* đŪĀÚ ŧįĀĖĀĮ timerļĶ disable */


    tcflush(uart_fd, TCIFLUSH);
    tcsetattr(uart_fd,TCSANOW,&newtio);

    return uart_fd;
}

void uart_close()
{
    tcsetattr(uart_fd,TCSANOW,&oldtio);
    close(uart_fd);
}
void uart_binwrite(unsigned char *data, int length)
{

    write(uart_fd, data, length);

#ifndef __MICROWAVE_DEBUG__
        for(int i  = 0 ; i< length; i ++) //len
        {
            qDebug("uart_Write_data[%d]=%02X", i, 0xFF & data[i]);
        }
#endif
}

char* uart_binread(int *length)
{
    *length = read(uart_fd, readBuf, BUFF_SIZE);
    readBuf[*length] = '\0';
    return readBuf;
}


void uart_printf(char *pcFmt,...)
{
    va_list ap;
    char pbString[256];
    va_start(ap,pcFmt);
    vsprintf(pbString,pcFmt,ap);
    write(uart_fd, pbString, strlen(pbString));
    va_end(ap);
}
#endif

int32 USB_uart_init()
{

    //unsigned long baudrate = 115200;
    struct termios options;
    char *tx = "STARmed";


#ifdef _ONPC_
    uart_fd = open(USB0_uart, O_RDWR | O_NOCTTY);
#else
    usb_uart_fd = open(usb_uart, O_RDWR | O_NOCTTY);
#endif

    fcntl(usb_uart_fd , F_SETFL, 0);

    tcgetattr(usb_uart_fd, &options);
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~PARODD;
    options.c_cflag |= CS8;
    options.c_cflag &= ~CRTSCTS;

    options.c_lflag &= ~(ICANON | IEXTEN | ISIG | ECHO);
    options.c_oflag &= ~OPOST;
    options.c_iflag &= ~(ICRNL | INPCK | ISTRIP | IXON | BRKINT );

    options.c_cc[VMIN] = 1;
    options.c_cc[VTIME] = 0;

    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);

    tcsetattr(usb_uart_fd, TCSANOW, &options);
    write(usb_uart_fd, tx, strlen(tx));

    return usb_uart_fd ;

}

void USB_uart_close()
{
    tcsetattr(usb_uart_fd ,TCSANOW,&oldtio);
    close(usb_uart_fd );
}
void USB_uart_binwrite(unsigned char *data, int length)
{

    write(usb_uart_fd , data, length);

#ifndef __MICROWAVE_DEBUG__
        for(int i  = 0 ; i< length; i ++) //len
        {
            qDebug("uart_Write_data[%d]=%02X", i, 0xFF & data[i]);
        }
#endif
}

char* USB_uart_binread(int *length)
{
    *length = read(usb_uart_fd , readBuf, BUFF_SIZE);
    readBuf[*length] = '\0';
    return readBuf;
}


void USB_uart_printf(char *pcFmt,...)
{
    va_list ap;
    char pbString[256];
    va_start(ap,pcFmt);
    vsprintf(pbString,pcFmt,ap);
    write(usb_uart_fd , pbString, strlen(pbString));
    va_end(ap);
}

void gpio_init()
{
#ifndef _ONPC_
    gpio1_address = mapRegAddr(GPIO1_DR);
    gpio2_address = mapRegAddr(GPIO2_DR);
    gpio3_address = mapRegAddr(GPIO3_DR);
    gpio4_address = mapRegAddr(GPIO4_DR);
    gpio5_address = mapRegAddr(GPIO5_DR);
    gpio6_address = mapRegAddr(GPIO6_DR);
#endif

}

volatile unsigned *GetEncorderAddress()
{
    return gpio2_address;
}

uint32 ReadGPIOPort(uint32 gpio, uint32 pin)
{

Q_UNUSED(gpio);
Q_UNUSED(pin);
#ifdef _ONPC_
    uint32 result = 1;
    return result;
#else

    switch(gpio)
    {
        case GPIO_1:
            if(TESTBIT(*(gpio1_address), pin)) return GPIO_HIGH;
            break;

        case GPIO_2:
            if(TESTBIT(*(gpio2_address), pin)) return GPIO_HIGH;
            break;

        case GPIO_3:
            if(TESTBIT(*(gpio3_address), pin)) return GPIO_HIGH;
            break;

        case GPIO_4:
            if(TESTBIT(*(gpio4_address), pin)) return GPIO_HIGH;
            break;

        case GPIO_5:
            if(TESTBIT(*(gpio5_address), pin)) return GPIO_HIGH;
            break;

        case GPIO_6:
            if(TESTBIT(*(gpio6_address), pin)) return GPIO_HIGH;
            break;
    }

    return GPIO_LOW;
#endif
}


void WriteGPIOPort(uint32 gpio, uint32 pin, bool flag)
{
    /*
     * 	flag : true is HI, false is LOW
     *
     */
Q_UNUSED(gpio);
Q_UNUSED(pin);
Q_UNUSED(flag);
#ifdef _ONPC_
    //qDebug("gpio=%d, pin=%d, flag=%d\n", gpio, pin, flag);
#else
    switch(gpio)
    {
        case GPIO_1:
            if(flag)
                SETBIT(*(gpio1_address), pin);    //HI
            else
                CLEARBIT(*(gpio1_address), pin);    //Low
            break;

        case GPIO_2:
            if(flag)
                SETBIT(*(gpio2_address), pin);    //HI
            else
                CLEARBIT(*(gpio2_address), pin);    //Low
            break;

        case GPIO_3:
            if(flag)
                SETBIT(*(gpio3_address), pin);    //HI
            else
                CLEARBIT(*(gpio3_address), pin);    //Low
            break;
        case GPIO_4:
            if(flag)
                SETBIT(*(gpio4_address), pin);    //HI
            else
                CLEARBIT(*(gpio4_address), pin);    //Low
            break;

        case GPIO_5:
            if(flag)
                SETBIT(*(gpio5_address), pin);    //HI
            else
                CLEARBIT(*(gpio5_address), pin);    //Low
            break;

        case GPIO_6:
            if(flag)
                SETBIT(*(gpio6_address), pin);    //HI
            else
                CLEARBIT(*(gpio6_address), pin);    //Low
            break;

        default:
            break;
    }
#endif
}

void WriteGPIOPortHigh(uint32_t port_id, uint32_t port_value)
{
    WriteGPIOPort(port_id, port_value, GPIO_HIGH);
}

void WriteGPIOPortLow(uint32_t port_id, uint32_t port_value)
{
    WriteGPIOPort(port_id, port_value, GPIO_LOW);
}

void RFSwitchLED_ON()
{
    //qDebug() << "RFSwitchLED ON";
    WriteGPIOPortHigh(GPIO_1, 17);
}
void RFSwitchLED_OFF()
{
    //qDebug() << "RFSwitchLED OFF";
    WriteGPIOPortLow(GPIO_1, 17);
}

void AudioAMPControl_Enable()
{
    qDebug() << "AudioAMPControl_Enable" ;
    WriteGPIOPortHigh(GPIO_2, 1);

}
void AudioAMPControl_Disable()
{
    qDebug() << "AudioAMPControl_Disable" ;
    WriteGPIOPortLow(GPIO_2, 1);
}

void SetElectrodeType(ELECTRODE_Type_en eType)
{
    switch(eType)
    {
        case ELECTRODE_MONOPOLAR:
            WriteGPIOPort(GPIO_3, 31, GPIO_HIGH);
            WriteGPIOPort(GPIO_3, 30, GPIO_LOW);
            WriteGPIOPort(GPIO_3, 29, GPIO_LOW);
            break;

        case ELECTRODE_BI_3CM:
            WriteGPIOPort(GPIO_3, 31, GPIO_LOW);
            WriteGPIOPort(GPIO_3, 30, GPIO_HIGH);
            WriteGPIOPort(GPIO_3, 29, GPIO_LOW);
            break;

        case ELECTRODE_BI_7CM:
            WriteGPIOPort(GPIO_3, 31, GPIO_LOW);
            WriteGPIOPort(GPIO_3, 30, GPIO_HIGH);
            WriteGPIOPort(GPIO_3, 29, GPIO_HIGH);
            break;

        default:
            WriteGPIOPort(GPIO_3, 31, GPIO_LOW);
            WriteGPIOPort(GPIO_3, 30, GPIO_LOW);
            WriteGPIOPort(GPIO_3, 29, GPIO_LOW);
            break;

    }
}


void spi_init()
{
    qDebug() << "spi_init";
    for(int32 i=0; i<ALL_SPI_DEVICES; i++)
    {
        _spifd[i] = -1;
    }
    spiOpen(SPI_MAX31856);

}

void spiAllclose()
{
    for(int32 i = 0; i<ALL_SPI_DEVICES; i++)
    {
        if(_spifd[i] != -1)
        {
            spiClose(_spifd[i]);
        }
        _spifd[i] = -1;
    }
}



int32 spiOpen(SPI_DEVICES dn)
{
    std::string devspi;

    /*
     *  SPI_CPOL = 0 : low clock ,
     * 							1 : High clock
     *
     * SPI_CPHA = 0 : rising edge
     *                   			1 : falling edge
     *
     *  SPI_MODE_0		(0|0)
     *	 SPI_MODE_1		(0|SPI_CPHA)
     *	 SPI_MODE_2		(SPI_CPOL|0)
     *	 SPI_MODE_3		(SPI_CPOL|SPI_CPHA)
     *
     */

    switch(dn)
    {
        case SPI_MAX31856:
            devspi = "/dev/spidev1.0"; //1.0
            _mode[SPI_MAX31856] = SPI_MODE_1;
            _bits_per_word[SPI_MAX31856] = 8;
            _speed[SPI_MAX31856] = 1000000; //2000000
            break;

        default:
            printf(">>can not find spidev\n");
            return -1;
    }

    _spifd[dn] =  spiOpen(devspi.c_str(), &_mode[dn], &_bits_per_word[dn], &_speed[dn]);

    return _spifd[dn];
}

int32 spiOpen(const char *devspi, uint8 *_mode, uint8 *_bits_per_word, uint32 *_speed)
{
    int32 res = 0;
    int32 fd = -1;

    Q_UNUSED(res);
    Q_UNUSED(devspi);
    Q_UNUSED(_mode);
    Q_UNUSED(_bits_per_word);
    Q_UNUSED(_speed);

#ifdef _ONPC_
    fd = 1;
#else
    fd = open(devspi, O_RDWR);
    if (fd < 0)
    {
        qDebug("cannot open spi device:%s\n",devspi);
        return -1;
    }

    res = ioctl(fd, SPI_IOC_WR_MODE, _mode);
    if (res == -1)
    {
        qDebug("can't set spi SPI_IOC_WR_MODE:0x%x\n", *_mode);
        return -1;
    }

    res = ioctl(fd, SPI_IOC_RD_MODE, _mode);
    if (res == -1)
    {
        qDebug("can't set spi SPI_IOC_RD_MODE:0x%x\n", *_mode);
        return -1;
    }
    /*
    * bits per word
    */
    res = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, _bits_per_word);
    if (res == -1)
    {
        qDebug("can't set spi SPI_IOC_WR_BITS_PER_WORD:0x%x\n", *_bits_per_word);
        return -1;
    }

    res = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, _bits_per_word);
    if (res == -1)
    {
        qDebug("can't set spi SPI_IOC_WR_BITS_PER_WORD:0x%x\n", *_bits_per_word);
        return -1;
    }

    /*
    * max speed hz
    */
    res = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, _speed);
    if (res == -1)
    {
        qDebug("can't set spi SPI_IOC_WR_BITS_PER_WORD:%d\n", *_speed);
        return -1;
    }

    res = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, _speed);
    if (res == -1)
    {
        qDebug("can't set spi SPI_IOC_WR_BITS_PER_WORD:%d\n", *_speed);
        return -1;
    }
#endif
    return fd;
}

int32 spiClose(int32 _spifd)
{
    int32 res = -1;
    Q_UNUSED(_spifd);
#ifndef _ONPC_
    res = close(_spifd);
    if(res < 0)
    {
        qDebug("error Spi close spifd=0x%x\n", _spifd);
        return -1;
    }
#endif
    return res;
}

volatile float SPI_Temp_Read(SPI_DEVICES dev, SPI_MAX31856_CH channel)
{
    volatile float readValue = 0;
    uint8_t fault;
    if(dev == SPI_MAX31856)
    {
        switch(channel)
        {
            case CH1:
                WriteGPIOPort(GPIO_3, 31, GPIO_HIGH); //MUX_EN
                msDelay(1);
                WriteGPIOPort(GPIO_3, 29, GPIO_LOW); //MUX_A1
                WriteGPIOPort(GPIO_3, 30, GPIO_LOW); //MUX_A0

                break;
            case CH2:
                WriteGPIOPort(GPIO_3, 31, GPIO_HIGH); //MUX_EN
                msDelay(1);
                WriteGPIOPort(GPIO_3, 29, GPIO_LOW); //MUX_A1
                WriteGPIOPort(GPIO_3, 30, GPIO_HIGH); //MUX_A0
                break;
            case CH3:
                WriteGPIOPort(GPIO_3, 31, GPIO_HIGH); //MUX_EN
                msDelay(1);
                WriteGPIOPort(GPIO_3, 29, GPIO_HIGH); //MUX_A1
                WriteGPIOPort(GPIO_3, 30, GPIO_LOW); //MUX_A0
                break;
            case CH4:
                WriteGPIOPort(GPIO_3, 31, GPIO_HIGH); //MUX_EN
                msDelay(1);
                WriteGPIOPort(GPIO_3, 29, GPIO_HIGH); //MUX_A1
                WriteGPIOPort(GPIO_3, 30, GPIO_HIGH); //MUX_A0
                break;

            default :
                WriteGPIOPort(GPIO_3, 29, GPIO_LOW); //MUX_A1
                WriteGPIOPort(GPIO_3, 30, GPIO_LOW); //MUX_A0
                WriteGPIOPort(GPIO_3, 31, GPIO_LOW); //MUX_EN
                return 0;
                break;
        }
    }
    else
    {
        return -1;
    }
    msDelay(1);
    //qDebug("Cold Junction Temp: %.2f", readCJTemperature());
    readValue = readThermocoupleTemperature();
    WriteGPIOPort(GPIO_3, 31, GPIO_LOW); //MUX_Disable
    msDelay(1);
#if 0
    fault = readFault();
    if (fault)
    {
        if (fault & MAX31856_FAULT_CJRANGE) qDebug("Cold Junction Range Fault");
        if (fault & MAX31856_FAULT_TCRANGE) qDebug("Thermocouple Range Fault");
        if (fault & MAX31856_FAULT_CJHIGH)  qDebug("Cold Junction High Fault");
        if (fault & MAX31856_FAULT_CJLOW)   qDebug("Cold Junction Low Fault");
        if (fault & MAX31856_FAULT_TCHIGH)  qDebug("Thermocouple High Fault");
        if (fault & MAX31856_FAULT_OVUV)    qDebug("Over/Under Voltage Fault");
        if (fault & MAX31856_FAULT_OPEN)    qDebug("Thermocouple Open Fault");
    }
#endif
    return readValue;
}

bool max31856_begin(void)
{
  uint8_t fault;
  // assert on any fault
  writeRegister8(MAX31856_MASK_REG, 0x0);

  // enable open circuit fault detection
  writeRegister8(MAX31856_CR0_REG, MAX31856_CR0_OCFAULT0); //

  // set cold junction temperature offset to zero
  writeRegister8(MAX31856_CJTO_REG, 0x0);

  // set Noise filter 60hz
  setNoiseFilter(MAX31856_NOISE_FILTER_60HZ);

  // set Type T by default
  setThermocoupleType(MAX31856_TCTYPE_T);

  // set One-Shot conversion mode
  setConversionMode(MAX31856_ONESHOT);



  if(getThermocoupleType() != MAX31856_TCTYPE_T )
  {
      qDebug("None Type");
      return false;
  }
  else
  {
      qDebug("T Type");
      qDebug("Cold Junction Temp: %.2f", readCJTemperature());
  }
  fault = readFault();
  if (fault)
  {

      if (fault & MAX31856_FAULT_CJRANGE) qDebug("Cold Junction Range Fault");
      if (fault & MAX31856_FAULT_TCRANGE) qDebug("Thermocouple Range Fault");
      if (fault & MAX31856_FAULT_CJHIGH)  qDebug("Cold Junction High Fault");
      if (fault & MAX31856_FAULT_CJLOW)   qDebug("Cold Junction Low Fault");
      if (fault & MAX31856_FAULT_TCHIGH)  qDebug("Thermocouple High Fault");
      if (fault & MAX31856_FAULT_OVUV)    qDebug("Over/Under Voltage Fault");
      if (fault & MAX31856_FAULT_OPEN)    qDebug("Thermocouple Open Fault");
  }
  return true;

}

/**************************************************************************/
/*!
    @brief  Set temperature conversion mode
    @param mode The conversion mode
*/
/**************************************************************************/
void setConversionMode(max31856_conversion_mode_t mode)
{
  conversionMode = mode;
  uint8_t t = readRegister8(MAX31856_CR0_REG); // get current register value
  if (conversionMode == MAX31856_CONTINUOUS) {
    t |= MAX31856_CR0_AUTOCONVERT; // turn on automatic
    t &= ~MAX31856_CR0_1SHOT;      // turn off one-shot
  } else {
    t &= ~MAX31856_CR0_AUTOCONVERT; // turn off automatic
    t |= MAX31856_CR0_1SHOT;        // turn on one-shot
  }
  writeRegister8(MAX31856_CR0_REG, t); // write value back to register
}

/**************************************************************************/
/*!
    @brief  Get temperature conversion mode
    @returns The conversion mode
*/
/**************************************************************************/
max31856_conversion_mode_t getConversionMode(void)
{
  return conversionMode;
}

/**************************************************************************/
/*!
    @brief  Set which kind of Thermocouple (K, J, T, etc) to detect & decode
    @param type The enumeration type of the thermocouple
*/
/**************************************************************************/
void setThermocoupleType(max31856_thermocoupletype_t type)
{
  uint8_t t = readRegister8(MAX31856_CR1_REG);
  t &= 0xF0; // mask off bottom 4 bits
  t |= (uint8_t)type & 0x0F;
  writeRegister8(MAX31856_CR1_REG, t);
}

/**************************************************************************/
/*!
    @brief  Get which kind of Thermocouple (K, J, T, etc) we are using
    @returns The enumeration type of the thermocouple
*/
/**************************************************************************/
max31856_thermocoupletype_t getThermocoupleType(void)
{
  uint8_t t = readRegister8(MAX31856_CR1_REG);
  t &= 0x0F;

  return (max31856_thermocoupletype_t)(t);
}

/**************************************************************************/
/*!
    @brief  Read the fault register (8 bits)
    @returns 8 bits of fault register data
*/
/**************************************************************************/
uint8_t readFault(void)
{
  return readRegister8(MAX31856_SR_REG);
}

/**************************************************************************/
/*!
    @brief  Sets the threshhold for internal chip temperature range
    for fault detection. NOT the thermocouple temperature range!
    @param  low Low (min) temperature, signed 8 bit so -128 to 127 degrees C
    @param  high High (max) temperature, signed 8 bit so -128 to 127 degrees C
*/
/**************************************************************************/
void setColdJunctionFaultThreshholds(int8_t low, int8_t high)
{
  writeRegister8(MAX31856_CJLF_REG, low);
  writeRegister8(MAX31856_CJHF_REG, high);
}

/**************************************************************************/
/*!
    @brief  Sets the mains noise filter. Can be set to 50 or 60hz.
    Defaults to 60hz. You need to call this if you live in a 50hz country.
    @param  noiseFilter One of MAX31856_NOISE_FILTER_50HZ or
   MAX31856_NOISE_FILTER_60HZ
*/
/**************************************************************************/
void setNoiseFilter(max31856_noise_filter_t noiseFilter)
{
  uint8_t t = readRegister8(MAX31856_CR0_REG);
  if (noiseFilter == MAX31856_NOISE_FILTER_50HZ) {
    t |= 0x01;
  } else {
    t &= 0xfe;
  }
  writeRegister8(MAX31856_CR0_REG, t);
}

/**************************************************************************/
/*!
    @brief  Sets the threshhold for thermocouple temperature range
    for fault detection. NOT the internal chip temperature range!
    @param  flow Low (min) temperature, floating point
    @param  fhigh High (max) temperature, floating point
*/
/**************************************************************************/
void setTempFaultThreshholds(float flow, float fhigh)
{
  int16_t low, high;

  flow *= 16;
  low = flow;

  fhigh *= 16;
  high = fhigh;

  writeRegister8(MAX31856_LTHFTH_REG, high >> 8);
  writeRegister8(MAX31856_LTHFTL_REG, high);

  writeRegister8(MAX31856_LTLFTH_REG, low >> 8);
  writeRegister8(MAX31856_LTLFTL_REG, low);
}

/**************************************************************************/
/*!
    @brief  Begin a one-shot (read temperature only upon request) measurement.
    Value must be read later, not returned here!
*/
/**************************************************************************/
void triggerOneShot(void)
{

  if (conversionMode == MAX31856_CONTINUOUS)
    return;

  uint8_t t = readRegister8(MAX31856_CR0_REG); // get current register value
  t &= ~MAX31856_CR0_AUTOCONVERT;              // turn off autoconvert
  t |= MAX31856_CR0_1SHOT;                     // turn on one-shot
  writeRegister8(MAX31856_CR0_REG, t);         // write value back to register
                                       // conversion starts when CS goes high
}

/**************************************************************************/
/*!
    @brief  Return status of temperature conversion.
    @returns true if conversion complete, otherwise false
*/
/**************************************************************************/
bool conversionComplete(void)
{

  if (conversionMode == MAX31856_CONTINUOUS)
    return true;
  return !(readRegister8(MAX31856_CR0_REG) & MAX31856_CR0_1SHOT);
}

/**************************************************************************/
/*!
    @brief  Return cold-junction (internal chip) temperature
    @returns Floating point temperature in Celsius
*/
/**************************************************************************/
float readCJTemperature(void)
{
#if 0
  return readRegister16(MAX31856_CJTH_REG) / 256.0;
#endif
  int32_t temp;
  uint8_t buf_read[3], buf_write[2]={MAX31856_CJTH_REG, MAX31856_CJTL_REG};

  buf_read[0] = readRegister8(buf_write[0]); //1
  buf_read[1] = readRegister8(buf_write[1]); //1

  //Convert the registers contents into the correct value
  temp =((int32_t)(buf_read[0] << 6));        //Shift the MSB into place
  temp|=((int32_t)(buf_read[1] >> 2));        //Shift the LSB into place
  float val=((float)(temp/64.0));             //Divide the binary string by 2 to the 6th power

  return val;

}

/**************************************************************************/
/*!
    @brief  Return hot-junction (thermocouple) temperature
    @returns Floating point temperature in Celsius
*/
/**************************************************************************/
float readThermocoupleTemperature(void)
{
  float val;
  int temp;
  uint8_t temp_reg[3];
  // for one-shot, make it happen
  if (conversionMode == MAX31856_ONESHOT) {
    triggerOneShot();
    while (!conversionComplete()) {
      gpioTools::msDelay(10);
    }
  }
  // read the thermocouple temperature registers (3 bytes)
  int32_t temp24 = readRegister24(MAX31856_LTCBH_REG);
  // and compute temperature
  if (temp24 & 0x800000) {
    temp24 |= 0xFF000000; // fix sign
  }

  temp24 >>= 5; // bottom 5 bits are unused

  return temp24 * 0.0078125;

}

/**********************************************/

uint8_t readRegister8(uint8_t addr)
{

  addr &= 0x7F; // MSB=0 for read, make sure top bit is not set
  uint8_t RxBuffer[2] = {0, 0};
  uint8_t TxBuffer[2];

  WriteGPIOPort(GPIO_2, 27, GPIO_LOW); //cs
  TxBuffer[0] = addr;
  TxBuffer[1] = 0;

  spiWriteRead(SPI_MAX31856, TxBuffer, RxBuffer, 2);
  WriteGPIOPort(GPIO_2, 27, GPIO_HIGH); //cs
  uint8_t ret = RxBuffer[1];
  return ret;
}

uint32_t readRegister24(uint8_t addr)
{
  addr &= 0x7F; // MSB=0 for read, make sure top bit is not set
  uint8_t RxBuffer[4] = {0, 0, 0, 0};
  uint8_t TxBuffer[2];
  WriteGPIOPort(GPIO_2, 27, GPIO_LOW); //cs
  TxBuffer[0] = addr;
  TxBuffer[1] = 0;

  spiWriteRead(SPI_MAX31856, TxBuffer, RxBuffer, 4);
  WriteGPIOPort(GPIO_2, 27, GPIO_HIGH); //cs

  uint32_t ret = RxBuffer[1];
  ret <<= 8;
  ret |= RxBuffer[2];
  ret <<= 8;
  ret |= RxBuffer[3];
  return ret;
}

void readRegisterN(uint8_t addr, uint8_t buffer[],
                                      uint8_t n)
{
  addr &= 0x7F; // MSB=0 for read, make sure top bit is not set
  uint8_t TxBuffer[2];
  WriteGPIOPort(GPIO_2, 27, GPIO_LOW); //cs
  TxBuffer[0] = addr;
  TxBuffer[1] = 0;

  spiWriteRead(SPI_MAX31856, TxBuffer, buffer, n);
  WriteGPIOPort(GPIO_2, 27, GPIO_HIGH); //cs
}

void writeRegister8(uint8_t addr, uint8_t data)
{
  addr |= 0x80; // MSB=1 for write, make sure top bit is set
  uint8_t TxBuffer[2];

  WriteGPIOPort(GPIO_2, 27, GPIO_LOW); //cs

  TxBuffer[0] = addr;
  TxBuffer[1] = data;
  spiWrite(SPI_MAX31856, TxBuffer, 2); //1
  WriteGPIOPort(GPIO_2, 27, GPIO_HIGH); //cs
}

void spiWrite(int32 fdId, volatile uint8 *tbuf, uint32 length)
{
    int32 ret = 0;
    Q_UNUSED(ret);
    memset(_io_trans, 0, sizeof(_io_trans));

    _io_trans[0].tx_buf = (unsigned long)tbuf;
    _io_trans[0].bits_per_word = _bits_per_word[fdId];
    _io_trans[0].speed_hz = _speed[fdId];
    _io_trans[0].len = length;

#ifndef _ONPC_
    ret = ioctl(_spifd[fdId], SPI_IOC_MESSAGE(1), &_io_trans);
    if(ret == 1)
    {
        printf("Cant send spi message=0x%x\n", _spifd[fdId]);
    }
    else if(ret == -1)
    {
        printf("Failed transferring data=%d\n", errno);
    }
#endif
}

void spiWriteRead(int32 fdId, volatile uint8 *tbuf, volatile uint8 *rbuf, uint32 length)
{
    int32 ret = 0;
    Q_UNUSED(ret);
    memset(_io_trans, 0, sizeof(_io_trans));

    _io_trans[0].tx_buf = (unsigned long)tbuf;
    _io_trans[0].rx_buf = (unsigned long)rbuf;
    _io_trans[0].bits_per_word = _bits_per_word[fdId];
    _io_trans[0].speed_hz = _speed[fdId];
    _io_trans[0].len = length;


#ifndef _ONPC_
    ret = ioctl(_spifd[fdId], SPI_IOC_MESSAGE(1), &_io_trans);

    if(ret == 1)
    {
        printf("Cant send spi message=0x%x\n", _spifd[fdId]);
    }
    else if(ret == -1)
    {
        printf("Failed transferring data=%d\n", errno);
    }

#else
#if test
    printf("fdId=%d, spi=%d, bps=%d, speed=%d, len=%d\n",fdId, _spifd[fdId], _bits_per_word[fdId], _speed[fdId], length );
#endif
#endif
    }

}
