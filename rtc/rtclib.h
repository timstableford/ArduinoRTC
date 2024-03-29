#ifndef RTCLIB
#define RTCLIB
#include "Arduino.h"

//Structure to make maxipulating the trickle control byte simpler
typedef struct trickle_struct
{
  uint8_t enabled:4;
  uint8_t num_diodes:2;
  uint8_t resistor_val:2;
};

//Structure to hold the 8 received bytes
typedef struct ds1302_struct
{
  uint8_t Seconds:4;      // low decimal digit 0-9
  uint8_t Seconds10:3;    // high decimal digit 0-5
  uint8_t CH:1;           // CH = Clock Halt
  uint8_t Minutes:4;
  uint8_t Minutes10:3;
  uint8_t reserved1:1;
  uint8_t Hour:4;
  uint8_t Hour10:2;
  uint8_t reserved2:1;
  uint8_t hour_12_24:1; // 0 for 24 hour format
  uint8_t Date:4;           // Day of month, 1 = first day
  uint8_t Date10:2;
  uint8_t reserved3:2;
  uint8_t Month:4;          // Month, 1 = January
  uint8_t Month10:1;
  uint8_t reserved4:3;
  uint8_t Day:3;            // Day of week, 1 = first day (any day)
  uint8_t reserved5:5;
  uint8_t Year:4;           // Year, 0 = year 2000
  uint8_t Year10:4;
  uint8_t reserved6:7;
  uint8_t WP:1;             // WP = Write Protect
};

// Macros to convert the bcd values of the registers to normal
// integer variables.
// The code uses seperate variables for the high byte and the low byte
// of the bcd, so these macros handle both bytes seperately.
#define bcd2bin(h,l)    (((h)*10) + (l))
#define bin2bcd_h(x)   ((x)/10)
#define bin2bcd_l(x)    ((x)%10)
// Register names.
// Since the highest bit is always '1',
// the registers start at 0x80
// If the register is read, the lowest bit should be '1'.
#define DS1302_SECONDS           0x80
#define DS1302_MINUTES           0x82
#define DS1302_HOURS             0x84
#define DS1302_DATE              0x86
#define DS1302_MONTH             0x88
#define DS1302_DAY               0x8A
#define DS1302_YEAR              0x8C
#define DS1302_ENABLE            0x8E
#define DS1302_TRICKLE           0x90
#define DS1302_CLOCK_BURST       0xBE
#define DS1302_CLOCK_BURST_WRITE 0xBE
#define DS1302_CLOCK_BURST_READ  0xBF
#define DS1302_RAMSTART          0xC0
#define DS1302_RAMEND            0xFC
#define DS1302_RAM_BURST         0xFE
#define DS1302_RAM_BURST_WRITE   0xFE
#define DS1302_RAM_BURST_READ    0xFF
// Defines for the bits, to be able to change
// between bit number and binary definition.
// By using the bit number, using the DS1302
// is like programming an AVR microcontroller.
// But instead of using "(1<<X)", or "_BV(X)",
// the Arduino "bit(X)" is used.
#define DS1302_D0 0
#define DS1302_D1 1
#define DS1302_D2 2
#define DS1302_D3 3
#define DS1302_D4 4
#define DS1302_D5 5
#define DS1302_D6 6
#define DS1302_D7 7

// Bit for reading (bit in address)
#define DS1302_READBIT DS1302_D0 // READBIT=1: read instruction

// Bit for clock (0) or ram (1) area,
// called R/C-bit (bit in address)
#define DS1302_RC DS1302_D6

// Seconds Register
#define DS1302_CH DS1302_D7   // 1 = Clock Halt, 0 = start

// Hour Register
#define DS1302_AM_PM DS1302_D5 // 0 = AM, 1 = PM
#define DS1302_12_24 DS1302 D7 // 0 = 24 hour, 1 = 12 hour

// Enable Register
#define DS1302_WP DS1302_D7   // 1 = Write Protect, 0 = enabled

// Trickle Register
#define DS1302_ROUT0 DS1302_D0
#define DS1302_ROUT1 DS1302_D1
#define DS1302_DS0   DS1302_D2
#define DS1302_DS1   DS1302_D2
#define DS1302_TCS0  DS1302_D4
#define DS1302_TCS1  DS1302_D5
#define DS1302_TCS2  DS1302_D6
#define DS1302_TCS3  DS1302_D7

class Time{
  private:
    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int date;
    unsigned int seconds;
    unsigned int minutes;
    unsigned int hours;
  public:
    Time(unsigned int year, unsigned int month,
      unsigned int date, unsigned int day,
      unsigned int hours, unsigned int minutes,
      unsigned int seconds);
    unsigned int getHours();
    unsigned int getMinutes();
    unsigned int getSeconds();
    unsigned int getDate();
    unsigned int getMonth();
    unsigned int getDay();
    unsigned int getYear();
    void setYear(unsigned int year);
    void setMonth(unsigned int month);
    void setDate(unsigned int date);
    void setDay(unsigned int day);
    void setHours(unsigned int hours);
    void setMinutes(unsigned int minutes);
    void setSeconds(unsigned int seconds);
    int compare(Time other);
    void clone(Time *t);
    void loadFromEEPROM(int address);
    void saveToEEPROM(int address);
    Time();
};
//Define the DS1302 RTC Class
class RTC {
  private:
    //Pins
    uint8_t ds_sclk;
    uint8_t ds_io;
    uint8_t ds_ce;
    //Time buffer
    struct ds1302_struct ds_time;
    Time time;
    //Private methods
    void RTCBurstWrite();
    uint8_t RTCRead(int address);
    void RTCWrite(int address, uint8_t data);
    void RTCStart(void);
    void RTCStop(void);
    uint8_t RTCToggleRead(void);
    void RTCToggleWrite( uint8_t data, uint8_t release);
  public: 
    RTC(){};
    RTC(uint8_t sclk, uint8_t io, uint8_t ce);
    void read();
    Time *getDateTime();
    void setTrickleState(int num_diodes, int resistor_val);
    void setDateTime(Time t);
    ~RTC();
};
#endif
