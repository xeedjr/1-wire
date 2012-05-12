#ifndef SERIALPORT@_H
#define SERIALPORT@_H

#include <windows.h>
#include <stdio.h>

#include "ownet.h"

class MSerial2 {
public:
    // Win32 globals needed
    HANDLE ComID[MAX_PORTNUM];
    OVERLAPPED osRead[MAX_PORTNUM],osWrite[MAX_PORTNUM];
    SMALLINT ComID_init;

    int OpenCOMEx(char *port_zstr);
    SMALLINT OpenCOM(int portnum, char *port_zstr);
    void CloseCOM(int portnum);
    void FlushCOM(int portnum);
    SMALLINT WriteCOM(int portnum, int outlen, uchar *outbuf);
    int ReadCOM(int portnum, int inlen, uchar *inbuf);
    void BreakCOM(int portnum);
    void SetBaudCOM(int portnum, uchar new_baud);
    //void msDelay(int len);
    //long msGettick(void);
};

#endif