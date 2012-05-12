#ifndef CRC_H
#define CRC_H

// Include files
#include "ownet.h"

class CRC_C {
public:
    ushort utilcrc16[MAX_PORTNUM];
    uchar utilcrc8[MAX_PORTNUM];

    // Local subroutines
    void setcrc16(int,ushort);
    ushort docrc16(int,ushort);
    void setcrc8(int,uchar);
    uchar docrc8(int,uchar);
};

#endif