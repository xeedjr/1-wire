#ifndef DS9790U_H
#define DS9790U_H

#include "ownet.h"
#include "SerialPort2.h"
#include "CRC.h"

#ifndef __MC68K__
#include <stdlib.h>
#ifndef _WIN32_WCE
#include <stdio.h>
#endif
#endif

// Typedefs
//--------------------------------------------------------------//
// Typedefs
//--------------------------------------------------------------//
#ifndef SMALLINT
   //
   // purpose of smallint is for compile-time changing of formal
   // parameters and return values of functions.  For each target
   // machine, an integer is alleged to represent the most "simple"
   // number representable by that architecture.  This should, in
   // most cases, produce optimal code for that particular arch.
   // BUT...  The majority of compilers designed for embedded
   // processors actually keep an int at 16 bits, although the
   // architecture might only be comfortable with 8 bits.
   // The default size of smallint will be the same as that of
   // an integer, but this allows for easy overriding of that size.
   //
   // NOTE:
   // In all cases where a smallint is used, it is assumed that
   // decreasing the size of this integer to something as low as
   // a single byte _will_not_ change the functionality of the
   // application.  e.g. a loop counter that will iterate through
   // several kilobytes of data should not be SMALLINT.  The most
   // common place you'll see smallint is for boolean return types.
   //
   #define SMALLINT int
#endif

#ifndef OW_UCHAR
   #define OW_UCHAR
   typedef unsigned char  uchar;
   #ifdef WIN32
      #ifndef __CYGWIN__ //&& GCC
      //already defined in sys/types.h for cygwin
      typedef unsigned short ushort;
      #endif
   typedef unsigned long  ulong;
   #endif
#endif

// Mode Commands
#define MODE_DATA                      0xE1
#define MODE_COMMAND                   0xE3
#define MODE_STOP_PULSE                0xF1

// Return byte value
#define RB_CHIPID_MASK                 0x1C
#define RB_RESET_MASK                  0x03
#define RB_1WIRESHORT                  0x00
#define RB_PRESENCE                    0x01
#define RB_ALARMPRESENCE               0x02
#define RB_NOPRESENCE                  0x03

#define RB_BIT_MASK                    0x03
#define RB_BIT_ONE                     0x03
#define RB_BIT_ZERO                    0x00

// Masks for all bit ranges
#define CMD_MASK                       0x80
#define FUNCTSEL_MASK                  0x60
#define BITPOL_MASK                    0x10
#define SPEEDSEL_MASK                  0x0C
#define MODSEL_MASK                    0x02
#define PARMSEL_MASK                   0x70
#define PARMSET_MASK                   0x0E

// Command or config bit
#define CMD_COMM                       0x81
#define CMD_CONFIG                     0x01

// Function select bits
#define FUNCTSEL_BIT                   0x00
#define FUNCTSEL_SEARCHON              0x30
#define FUNCTSEL_SEARCHOFF             0x20
#define FUNCTSEL_RESET                 0x40
#define FUNCTSEL_CHMOD                 0x60

// Bit polarity/Pulse voltage bits
#define BITPOL_ONE                     0x10
#define BITPOL_ZERO                    0x00
#define BITPOL_5V                      0x00
#define BITPOL_12V                     0x10

// One Wire speed bits
#define SPEEDSEL_STD                   0x00
#define SPEEDSEL_FLEX                  0x04
#define SPEEDSEL_OD                    0x08
#define SPEEDSEL_PULSE                 0x0C

// Data/Command mode select bits
#define MODSEL_DATA                    0x00
#define MODSEL_COMMAND                 0x02

// 5V Follow Pulse select bits (If 5V pulse
// will be following the next byte or bit.)
#define PRIME5V_TRUE                   0x02
#define PRIME5V_FALSE                  0x00

// Parameter select bits
#define PARMSEL_PARMREAD               0x00
#define PARMSEL_SLEW                   0x10
#define PARMSEL_12VPULSE               0x20
#define PARMSEL_5VPULSE                0x30
#define PARMSEL_WRITE1LOW              0x40
#define PARMSEL_SAMPLEOFFSET           0x50
#define PARMSEL_ACTIVEPULLUPTIME       0x60
#define PARMSEL_BAUDRATE               0x70

// Pull down slew rate.
#define PARMSET_Slew15Vus              0x00
#define PARMSET_Slew2p2Vus             0x02
#define PARMSET_Slew1p65Vus            0x04
#define PARMSET_Slew1p37Vus            0x06
#define PARMSET_Slew1p1Vus             0x08
#define PARMSET_Slew0p83Vus            0x0A
#define PARMSET_Slew0p7Vus             0x0C
#define PARMSET_Slew0p55Vus            0x0E

// 12V programming pulse time table
#define PARMSET_32us                   0x00
#define PARMSET_64us                   0x02
#define PARMSET_128us                  0x04
#define PARMSET_256us                  0x06
#define PARMSET_512us                  0x08
#define PARMSET_1024us                 0x0A
#define PARMSET_2048us                 0x0C
#define PARMSET_infinite               0x0E

// 5V strong pull up pulse time table
#define PARMSET_16p4ms                 0x00
#define PARMSET_65p5ms                 0x02
#define PARMSET_131ms                  0x04
#define PARMSET_262ms                  0x06
#define PARMSET_524ms                  0x08
#define PARMSET_1p05s                  0x0A
#define PARMSET_2p10s                  0x0C
#define PARMSET_infinite               0x0E

// Write 1 low time
#define PARMSET_Write8us               0x00
#define PARMSET_Write9us               0x02
#define PARMSET_Write10us              0x04
#define PARMSET_Write11us              0x06
#define PARMSET_Write12us              0x08
#define PARMSET_Write13us              0x0A
#define PARMSET_Write14us              0x0C
#define PARMSET_Write15us              0x0E

// Data sample offset and Write 0 recovery time
#define PARMSET_SampOff3us             0x00
#define PARMSET_SampOff4us             0x02
#define PARMSET_SampOff5us             0x04
#define PARMSET_SampOff6us             0x06
#define PARMSET_SampOff7us             0x08
#define PARMSET_SampOff8us             0x0A
#define PARMSET_SampOff9us             0x0C
#define PARMSET_SampOff10us            0x0E

// Active pull up on time
#define PARMSET_PullUp0p0us            0x00
#define PARMSET_PullUp0p5us            0x02
#define PARMSET_PullUp1p0us            0x04
#define PARMSET_PullUp1p5us            0x06
#define PARMSET_PullUp2p0us            0x08
#define PARMSET_PullUp2p5us            0x0A
#define PARMSET_PullUp3p0us            0x0C
#define PARMSET_PullUp3p5us            0x0E

// Baud rate bits
#define PARMSET_9600                   0x00
#define PARMSET_19200                  0x02
#define PARMSET_57600                  0x04
#define PARMSET_115200                 0x06

// DS2480B program voltage available
#define DS2480PROG_MASK                0x20

// mode bit flags
#define MODE_NORMAL                    0x00
#define MODE_OVERDRIVE                 0x01
#define MODE_STRONG5                   0x02
#define MODE_PROGRAM                   0x04
#define MODE_BREAK                     0x08

class DS9790U_C {

public:
    MSerial2    COMport;
    CRC_C       crc;
    int connect_port;

    // global DS2480B state
    SMALLINT ULevel[MAX_PORTNUM]; // current DS2480B 1-Wire Net level
    SMALLINT UBaud[MAX_PORTNUM];  // current DS2480B baud rate
    SMALLINT UMode[MAX_PORTNUM];  // current DS2480B command or data mode state
    SMALLINT USpeed[MAX_PORTNUM]; // current DS2480B 1-Wire Net communication speed

    // exportable functions defined in ds2480ut.c
    DS9790U_C();
    int Connect(int port);
    int Disconnect();
    SMALLINT DS2480Detect(int);
    SMALLINT DS2480ChangeBaud(int,uchar);
    SMALLINT DS2480Try( int portnum );

//  owLLU.C - Link Layer 1-Wire Net functions using the DS2480/DS2480B (U)
//            serial interface chip.
    // exportable functions defined in owllu.c
    SMALLINT owTouchReset(int);
    SMALLINT owTouchBit(int,SMALLINT);
    SMALLINT owTouchByte(int,SMALLINT);
    SMALLINT owWriteByte(int,SMALLINT);
    SMALLINT owReadByte(int);
    SMALLINT owSpeed(int,SMALLINT);
    SMALLINT owLevel(int,SMALLINT);
    SMALLINT owProgramPulse(int);
    SMALLINT owWriteBytePower(int,SMALLINT);
    SMALLINT owHasPowerDelivery(int);
    SMALLINT owHasProgramPulse(int);
    SMALLINT owHasOverDrive(int);
    SMALLINT owReadBitPower(int, SMALLINT);

    // local varable flag, true if program voltage available
    SMALLINT ProgramAvailable[MAX_PORTNUM];

//  owNetU.C - Network functions for 1-Wire Net devices
//             using the DS2480/DS2480B (U) serial interface chip.
    // exportable functions defined in ownetu.c
    SMALLINT  owFirst(int,SMALLINT,SMALLINT);
    SMALLINT  owNext(int,SMALLINT,SMALLINT);
    void      owSerialNum(int,uchar *,SMALLINT);
    void      owFamilySearchSetup(int,SMALLINT);
    void      owSkipFamily(int);
    SMALLINT  owAccess(int);
    SMALLINT  owVerify(int,SMALLINT);
    SMALLINT  owOverdriveAccess(int);

    // local functions defined in ownetu.c
    SMALLINT bitacc(SMALLINT,SMALLINT,SMALLINT,uchar *);

    // global variables for this module to hold search state information
    int LastDiscrepancy[MAX_PORTNUM];
    int LastFamilyDiscrepancy[MAX_PORTNUM];
    uchar LastDevice[MAX_PORTNUM];
    uchar SerialNum[MAX_PORTNUM][8];

//  owSesU.C - Acquire and release a Session on the 1-Wire Net.
    // exportable functions defined in owsesu.c
    SMALLINT owAcquire(int,char *);
    void     owRelease(int);

//  owTranU.C - Transport functions for 1-Wire Net
//              using the DS2480B (U) serial interface chip.
    // exportable functions defined in owtrnu.c
    SMALLINT owBlock(int,SMALLINT,uchar *,SMALLINT);
    SMALLINT owReadPacketStd(int,SMALLINT,int,uchar *);
    SMALLINT owWritePacketStd(int,int,uchar *,SMALLINT,SMALLINT,SMALLINT);
    SMALLINT owProgramByte(int,SMALLINT,int,SMALLINT,SMALLINT,SMALLINT);

    // local static functions
    SMALLINT Write_Scratchpad(int,uchar *,int,SMALLINT);
    SMALLINT Copy_Scratchpad(int,int,SMALLINT);
    SMALLINT Start_Convert_T(int);
    SMALLINT Read_ScratchPad(int portnum, char *scretchpad);
};

#endif
