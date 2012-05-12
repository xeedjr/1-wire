#include "DS9790U.h"
#include <stdio.h>
#include "ownet.h"

DS9790U_C::DS9790U_C()
{
    connect_port = -1;
}

int DS9790U_C::Connect(int port)
{
    char str[10];

    if (connect_port != -1)
    {
        // port olready open
        return false;
    }

    sprintf(str, "com%d", port);

    if (COMport.OpenCOM(port, str) == false)
    {
        return false;
    };

    connect_port = port;

    return true;
}

int DS9790U_C::Disconnect()
{
    if (connect_port == -1)
    {
        // port olready closed
        return false;
    }
    COMport.CloseCOM(connect_port);

    return true;
}

SMALLINT DS9790U_C::DS2480Detect(int portnum)
{
  int	retry = 0;

  while( retry++ < 10 )
  {
    if( DS2480Try( portnum ) == TRUE )
      return TRUE;
  }

  return FALSE;
}

SMALLINT DS9790U_C::DS2480Try( int portnum )
{
   uchar sendpacket[10],readbuffer[10];
   uchar sendlen=0;

   // reset modes
   UMode[portnum] = MODSEL_COMMAND;
   UBaud[portnum] = PARMSET_9600;
   USpeed[portnum] = SPEEDSEL_FLEX;

   // set the baud rate to 9600
   COMport.SetBaudCOM(portnum,(uchar)UBaud[portnum]);

   // send a break to reset the DS2480
   COMport.BreakCOM(portnum);

   // delay to let line settle
   msDelay(2);

   // flush the buffers
   COMport.FlushCOM(portnum);

   // send the timing byte
   sendpacket[0] = 0xC1;
   if (COMport.WriteCOM(portnum,1,sendpacket) != 1)
   {
      //OWERROR(OWERROR_WRITECOM_FAILED);
      return FALSE;
   }

   // delay to let line settle
   msDelay(4);

   // set the FLEX configuration parameters
   // default PDSRC = 1.37Vus
   sendpacket[sendlen++] = CMD_CONFIG | PARMSEL_SLEW | PARMSET_Slew1p37Vus;
   // default W1LT = 10us
   sendpacket[sendlen++] = CMD_CONFIG | PARMSEL_WRITE1LOW | PARMSET_Write10us;
   // default DSO/WORT = 8us
   sendpacket[sendlen++] = CMD_CONFIG | PARMSEL_SAMPLEOFFSET | PARMSET_SampOff8us;

   // construct the command to read the baud rate (to test command block)
   sendpacket[sendlen++] = CMD_CONFIG | PARMSEL_PARMREAD | (PARMSEL_BAUDRATE >> 3);

   // also do 1 bit operation (to test 1-Wire block)
   sendpacket[sendlen++] = CMD_COMM | FUNCTSEL_BIT | UBaud[portnum] | BITPOL_ONE;

#ifdef DEBUG_USERIAL
   {
     int i;
           
     printf("sendpacket: ");
     for(i=0;i<5;i++)
       printf("%02X ", sendpacket[i] );
     printf("\n");
   }
#endif /* DEBUG_USERIAL */

   // flush the buffers
   COMport.FlushCOM(portnum);

   // send the packet
   if (COMport.WriteCOM(portnum,sendlen,sendpacket))
   {
      // read back the response
      if (COMport.ReadCOM(portnum,5,readbuffer) == 5)
      {
         // look at the baud rate and bit operation
         // to see if the response makes sense
         if (((readbuffer[3] & 0xF1) == 0x00) &&
             ((readbuffer[3] & 0x0E) == UBaud[portnum]) &&
             ((readbuffer[4] & 0xF0) == 0x90) &&
             ((readbuffer[4] & 0x0C) == UBaud[portnum]))
         {
#ifdef DEBUG_USERIAL
           int i;

           printf("readbuffer: ");
           for(i=0;i<5;i++)
             printf("%02X ", readbuffer[i] );
           printf("\n");
#endif /* DEBUG_USERIAL */

            return TRUE;
         } else {
#ifdef DEBUG_USERIAL
           int i;
           printf("readbuffer: ");
           for(i=0;i<5;i++)
             printf("%02X ", readbuffer[i] );
           printf("\n");
#endif /* DEBUG_USERIAL */

           //OWERROR(OWERROR_DS2480_BAD_RESPONSE);
         }
      }
      else
      {
         //OWERROR(OWERROR_READCOM_FAILED);
      }
   }
   else
   {
      //OWERROR(OWERROR_WRITECOM_FAILED);
   }

   return FALSE;
}

//---------------------------------------------------------------------------
// Change the DS2480B from the current baud rate to the new baud rate.
//
// 'portnum' - number 0 to MAX_PORTNUM-1.  This number was provided to
//             OpenCOM to indicate the port number.
// 'newbaud' - the new baud rate to change to, defined as:
//               PARMSET_9600     0x00
//               PARMSET_19200    0x02
//               PARMSET_57600    0x04
//               PARMSET_115200   0x06
//
// Returns:  current DS2480B baud rate.
//
SMALLINT DS9790U_C::DS2480ChangeBaud(int portnum, uchar newbaud)
{
   uchar rt=FALSE;
   uchar readbuffer[5],sendpacket[5],sendpacket2[5];
   uchar sendlen=0,sendlen2=0;

   // see if diffenent then current baud rate
   if (UBaud[portnum] == newbaud)
      return UBaud[portnum];
   else
   {
      // build the command packet
      // check if correct mode
      if (UMode[portnum] != MODSEL_COMMAND)
      {
         UMode[portnum] = MODSEL_COMMAND;
         sendpacket[sendlen++] = MODE_COMMAND;
      }
      // build the command
      sendpacket[sendlen++] = CMD_CONFIG | PARMSEL_BAUDRATE | newbaud;

      // flush the buffers
      COMport.FlushCOM(portnum);

      // send the packet
      if (!COMport.WriteCOM(portnum,sendlen,sendpacket))
      {
         //OWERROR(OWERROR_WRITECOM_FAILED);
         rt = FALSE;
      }
      else
      {
         // make sure buffer is flushed
         msDelay(5);

         // change our baud rate
         COMport.SetBaudCOM(portnum,newbaud);
         UBaud[portnum] = newbaud;

         // wait for things to settle
         msDelay(5);

         // build a command packet to read back baud rate
         sendpacket2[sendlen2++] = CMD_CONFIG | PARMSEL_PARMREAD | (PARMSEL_BAUDRATE >> 3);

         // flush the buffers
         COMport.FlushCOM(portnum);

         // send the packet
         if (COMport.WriteCOM(portnum,sendlen2,sendpacket2))
         {
            // read back the 1 byte response
            if (COMport.ReadCOM(portnum,1,readbuffer) == 1)
            {
               // verify correct baud
               if (((readbuffer[0] & 0x0E) == (sendpacket[sendlen-1] & 0x0E)))
                  rt = TRUE;
               else
               {
                  //OWERROR(OWERROR_DS2480_WRONG_BAUD);
               }
            }
            else
            {
               //OWERROR(OWERROR_READCOM_FAILED);
            }
         }
         else
         {
            //OWERROR(OWERROR_WRITECOM_FAILED);
         };
      }
   }

   // if lost communication with DS2480 then reset
   if (rt != TRUE)
      DS2480Detect(portnum);

   return UBaud[portnum];
}

//--------------------------------------------------------------------------
// Reset all of the devices on the 1-Wire Net and return the result.
//
// 'portnum'    - number 0 to MAX_PORTNUM-1.  This number was provided to
//                OpenCOM to indicate the port number.
//
// Returns: TRUE(1):  presense pulse(s) detected, device(s) reset
//          FALSE(0): no presense pulses detected
//
// WARNING: This routine will not function correctly on some
//          Alarm reset types of the DS1994/DS1427/DS2404 with
//          Rev 1,2, and 3 of the DS2480/DS2480B.
//
SMALLINT DS9790U_C::owTouchReset(int portnum)
{
   uchar readbuffer[10],sendpacket[10];
   uchar sendlen=0;

   // make sure normal level
   owLevel(portnum,MODE_NORMAL);

   // check if correct mode
   if (UMode[portnum] != MODSEL_COMMAND)
   {
      UMode[portnum] = MODSEL_COMMAND;
      sendpacket[sendlen++] = MODE_COMMAND;
   }

   // construct the command
   sendpacket[sendlen++] = (uchar)(CMD_COMM | FUNCTSEL_RESET | USpeed[portnum]);

   // flush the buffers
   COMport.FlushCOM(portnum);

   // send the packet
   if (COMport.WriteCOM(portnum,sendlen,sendpacket))
   {
      // read back the 1 byte response
      if (COMport.ReadCOM(portnum,1,readbuffer) == 1)
      {
         // make sure this byte looks like a reset byte
         if (((readbuffer[0] & RB_RESET_MASK) == RB_PRESENCE) ||
             ((readbuffer[0] & RB_RESET_MASK) == RB_ALARMPRESENCE))
         {
            // check if programming voltage available
            ProgramAvailable[portnum] = ((readbuffer[0] & 0x20) == 0x20);
            return TRUE;
         }
         else
         {
            //OWERROR(OWERROR_RESET_FAILED);
         }

      }
      else
      {
         //OWERROR(OWERROR_READCOM_FAILED);
      }
   }
   else
   {
      //OWERROR(OWERROR_WRITECOM_FAILED);
   }

   // an error occured so re-sync with DS2480
   DS2480Detect(portnum);

   return FALSE;
}

//--------------------------------------------------------------------------
// Send 1 bit of communication to the 1-Wire Net and return the
// result 1 bit read from the 1-Wire Net.  The parameter 'sendbit'
// least significant bit is used and the least significant bit
// of the result is the return bit.
//
// 'portnum' - number 0 to MAX_PORTNUM-1.  This number was provided to
//             OpenCOM to indicate the port number.
// 'sendbit' - the least significant bit is the bit to send
//
// Returns: 0:   0 bit read from sendbit
//          1:   1 bit read from sendbit
//
SMALLINT DS9790U_C::owTouchBit(int portnum, SMALLINT sendbit)
{
   uchar readbuffer[10],sendpacket[10];
   uchar sendlen=0;

   // make sure normal level
   owLevel(portnum,MODE_NORMAL);

   // check if correct mode
   if (UMode[portnum] != MODSEL_COMMAND)
   {
      UMode[portnum] = MODSEL_COMMAND;
      sendpacket[sendlen++] = MODE_COMMAND;
   }

   // construct the command
   sendpacket[sendlen] = (sendbit != 0) ? BITPOL_ONE : BITPOL_ZERO;
   sendpacket[sendlen++] |= CMD_COMM | FUNCTSEL_BIT | USpeed[portnum];

   // flush the buffers
   COMport.FlushCOM(portnum);

   // send the packet
   if (COMport.WriteCOM(portnum,sendlen,sendpacket))
   {
      // read back the response
      if (COMport.ReadCOM(portnum,1,readbuffer) == 1)
      {
         // interpret the response
         if (((readbuffer[0] & 0xE0) == 0x80) &&
             ((readbuffer[0] & RB_BIT_MASK) == RB_BIT_ONE))
            return 1;
         else
            return 0;
      }
      else
      {
         //OWERROR(OWERROR_READCOM_FAILED);
      }
   }
   else
   {
      //OWERROR(OWERROR_WRITECOM_FAILED);
   }

   // an error occured so re-sync with DS2480
   DS2480Detect(portnum);

   return 0;
}

//--------------------------------------------------------------------------
// Send 8 bits of communication to the 1-Wire Net and verify that the
// 8 bits read from the 1-Wire Net is the same (write operation).
// The parameter 'sendbyte' least significant 8 bits are used.
//
// 'portnum'  - number 0 to MAX_PORTNUM-1.  This number was provided to
//              OpenCOM to indicate the port number.
// 'sendbyte' - 8 bits to send (least significant byte)
//
// Returns:  TRUE: bytes written and echo was the same
//           FALSE: echo was not the same
//
SMALLINT DS9790U_C::owWriteByte(int portnum, SMALLINT sendbyte)
{
   return (owTouchByte(portnum,sendbyte) == (0xff & sendbyte)) ? TRUE : FALSE;
}


//--------------------------------------------------------------------------
// Send 8 bits of read communication to the 1-Wire Net and and return the
// result 8 bits read from the 1-Wire Net.
//
// 'portnum'  - number 0 to MAX_PORTNUM-1.  This number was provided to
//              OpenCOM to indicate the port number.
//
// Returns:  8 bits read from 1-Wire Net
//
SMALLINT DS9790U_C::owReadByte(int portnum)
{
   return owTouchByte(portnum,(SMALLINT)0xFF);
}

//--------------------------------------------------------------------------
// Send 8 bits of communication to the 1-Wire Net and return the
// result 8 bits read from the 1-Wire Net.  The parameter 'sendbyte'
// least significant 8 bits are used and the least significant 8 bits
// of the result is the return byte.
//
// 'portnum'  - number 0 to MAX_PORTNUM-1.  This number was provided to
//              OpenCOM to indicate the port number.
// 'sendbyte' - 8 bits to send (least significant byte)
//
// Returns:  8 bits read from sendbyte
//
SMALLINT DS9790U_C::owTouchByte(int portnum, SMALLINT sendbyte)
{
   uchar readbuffer[10],sendpacket[10];
   uchar sendlen=0;

   // make sure normal level
   owLevel(portnum,MODE_NORMAL);

   // check if correct mode
   if (UMode[portnum] != MODSEL_DATA)
   {
      UMode[portnum] = MODSEL_DATA;
      sendpacket[sendlen++] = MODE_DATA;
   }

   // add the byte to send
   sendpacket[sendlen++] = (uchar)sendbyte;

   // check for duplication of data that looks like COMMAND mode
   if (sendbyte ==(SMALLINT)MODE_COMMAND)
      sendpacket[sendlen++] = (uchar)sendbyte;

   // flush the buffers
   COMport.FlushCOM(portnum);

   // send the packet
   if (COMport.WriteCOM(portnum,sendlen,sendpacket))
   {
      // read back the 1 byte response
      if (COMport.ReadCOM(portnum,1,readbuffer) == 1)
      {
          // return the response
          return (int)readbuffer[0];
      }
      else
      {
         //OWERROR(OWERROR_READCOM_FAILED);
      }
   }
   else
   {
      //OWERROR(OWERROR_WRITECOM_FAILED);
   }

   // an error occured so re-sync with DS2480
   DS2480Detect(portnum);

   return 0;
}

//--------------------------------------------------------------------------
// Set the 1-Wire Net communucation speed.
//
// 'portnum'   - number 0 to MAX_PORTNUM-1.  This number was provided to
//               OpenCOM to indicate the port number.
// 'new_speed' - new speed defined as
//                MODE_NORMAL     0x00
//                MODE_OVERDRIVE  0x01
//
// Returns:  current 1-Wire Net speed
//
SMALLINT DS9790U_C::owSpeed(int portnum, SMALLINT new_speed)
{
   uchar sendpacket[5];
   uchar sendlen=0;
   uchar rt = FALSE;

   // check if change from current mode
   if (((new_speed == MODE_OVERDRIVE) &&
        (USpeed[portnum] != SPEEDSEL_OD)) ||
       ((new_speed == MODE_NORMAL) &&
        (USpeed[portnum] != SPEEDSEL_FLEX)))
   {
      if (new_speed == MODE_OVERDRIVE)
      {
         // if overdrive then switch to 115200 baud
         if (DS2480ChangeBaud(portnum,PARMSET_115200) == PARMSET_115200)
         {
            USpeed[portnum] = SPEEDSEL_OD;
            rt = TRUE;
         }

      }
      else if (new_speed == MODE_NORMAL)
      {
         // else normal so set to 9600 baud
         if (DS2480ChangeBaud(portnum,PARMSET_9600) == PARMSET_9600)
         {
            USpeed[portnum] = SPEEDSEL_FLEX;
            rt = TRUE;
         }

      }

      // if baud rate is set correctly then change DS2480 speed
      if (rt)
      {
         // check if correct mode
         if (UMode[portnum] != MODSEL_COMMAND)
         {
            UMode[portnum] = MODSEL_COMMAND;
            sendpacket[sendlen++] = MODE_COMMAND;
         }

         // proceed to set the DS2480 communication speed
         sendpacket[sendlen++] = CMD_COMM | FUNCTSEL_SEARCHOFF | USpeed[portnum];

         // send the packet
         if (!COMport.WriteCOM(portnum,sendlen,sendpacket))
         {
            //OWERROR(OWERROR_WRITECOM_FAILED);
            rt = FALSE;
            // lost communication with DS2480 then reset
            DS2480Detect(portnum);
         }
      }

   }

   // return the current speed
   return (USpeed[portnum] == SPEEDSEL_OD) ? MODE_OVERDRIVE : MODE_NORMAL;
}

//--------------------------------------------------------------------------
// Set the 1-Wire Net line level.  The values for new_level are
// as follows:
//
// 'portnum'   - number 0 to MAX_PORTNUM-1.  This number was provided to
//               OpenCOM to indicate the port number.
// 'new_level' - new level defined as
//                MODE_NORMAL     0x00
//                MODE_STRONG5    0x02
//                MODE_PROGRAM    0x04
//                MODE_BREAK      0x08 (not supported)
//
// Returns:  current 1-Wire Net level
//
SMALLINT DS9790U_C::owLevel(int portnum, SMALLINT new_level)
{
   uchar sendpacket[10],readbuffer[10];
   uchar sendlen=0;
   uchar rt=FALSE;

   // check if need to change level
   if (new_level != ULevel[portnum])
   {
      // check if just putting back to normal
      if (new_level == MODE_NORMAL)
      {
         // check if correct mode
         if (UMode[portnum] != MODSEL_COMMAND)
         {
            UMode[portnum] = MODSEL_COMMAND;
            sendpacket[sendlen++] = MODE_COMMAND;
         }

         // stop pulse command
         sendpacket[sendlen++] = MODE_STOP_PULSE;

         // flush the buffers
         COMport.FlushCOM(portnum);

         // send the packet
         if (COMport.WriteCOM(portnum,sendlen,sendpacket))
         {
            msDelay(4);
            // read back the 1 byte response
            if (COMport.ReadCOM(portnum,1,readbuffer) == 1)
            {
               // check response byte
               if ((readbuffer[0] & 0xE0) == 0xE0)
               {
                  rt = TRUE;
                  ULevel[portnum] = MODE_NORMAL;
               }
            }
            else
            {
               //OWERROR(OWERROR_READCOM_FAILED);
            }
         }
         else
         {
            //OWERROR(OWERROR_WRITECOM_FAILED);
         };
      }
      // set new level
      else
      {
         // check if correct mode
         if (UMode[portnum] != MODSEL_COMMAND)
         {
            UMode[portnum] = MODSEL_COMMAND;
            sendpacket[sendlen++] = MODE_COMMAND;
         }

         // strong 5 volts
         if (new_level == MODE_STRONG5)
         {
            // set the SPUD time value
            sendpacket[sendlen++] = CMD_CONFIG | PARMSEL_5VPULSE | PARMSET_infinite;
            // add the command to begin the pulse
            sendpacket[sendlen++] = CMD_COMM | FUNCTSEL_CHMOD | SPEEDSEL_PULSE | BITPOL_5V;
         }
         // 12 volts
         else if (new_level == MODE_PROGRAM)
         {
            // check if programming voltage available
            if (!ProgramAvailable[portnum])
               return MODE_NORMAL;

            // set the PPD time value
            sendpacket[sendlen++] = CMD_CONFIG | PARMSEL_12VPULSE | PARMSET_infinite;
            // add the command to begin the pulse
            sendpacket[sendlen++] = CMD_COMM | FUNCTSEL_CHMOD | SPEEDSEL_PULSE | BITPOL_12V;
         }

         // flush the buffers
         COMport.FlushCOM(portnum);

         // send the packet
         if (COMport.WriteCOM(portnum,sendlen,sendpacket))
         {
            // read back the 1 byte response from setting time limit
            if (COMport.ReadCOM(portnum,1,readbuffer) == 1)
            {
               // check response byte
               if ((readbuffer[0] & 0x81) == 0)
               {
                  ULevel[portnum] = new_level;
                  rt = TRUE;
               }
            }
            else
            {
               //OWERROR(OWERROR_READCOM_FAILED);
            }
         }
         else
         {
            //OWERROR(OWERROR_WRITECOM_FAILED);
         }
      }

      // if lost communication with DS2480 then reset
      if (rt != TRUE)
         DS2480Detect(portnum);
   }

   // return the current level
   return ULevel[portnum];
}

//--------------------------------------------------------------------------
// This procedure creates a fixed 480 microseconds 12 volt pulse
// on the 1-Wire Net for programming EPROM iButtons.
//
// 'portnum'  - number 0 to MAX_PORTNUM-1.  This number was provided to
//              OpenCOM to indicate the port number.
//
// Returns:  TRUE  successful
//           FALSE program voltage not available
//
SMALLINT DS9790U_C::owProgramPulse(int portnum)
{
   uchar sendpacket[10],readbuffer[10];
   uchar sendlen=0;

   // check if programming voltage available
   if (!ProgramAvailable[portnum])
      return FALSE;

   // make sure normal level
   owLevel(portnum,MODE_NORMAL);

   // check if correct mode
   if (UMode[portnum] != MODSEL_COMMAND)
   {
      UMode[portnum] = MODSEL_COMMAND;
      sendpacket[sendlen++] = MODE_COMMAND;
   }

   // set the SPUD time value
   sendpacket[sendlen++] = CMD_CONFIG | PARMSEL_12VPULSE | PARMSET_512us;

   // pulse command
   sendpacket[sendlen++] = CMD_COMM | FUNCTSEL_CHMOD | BITPOL_12V | SPEEDSEL_PULSE;

   // flush the buffers
   COMport.FlushCOM(portnum);

   // send the packet
   if (COMport.WriteCOM(portnum,sendlen,sendpacket))
   {
      // read back the 2 byte response
      if (COMport.ReadCOM(portnum,2,readbuffer) == 2)
      {
         // check response byte
         if (((readbuffer[0] | CMD_CONFIG) ==
                (CMD_CONFIG | PARMSEL_12VPULSE | PARMSET_512us)) &&
             ((readbuffer[1] & 0xFC) ==
                (0xFC & (CMD_COMM | FUNCTSEL_CHMOD | BITPOL_12V | SPEEDSEL_PULSE))))
            return TRUE;
      }
      else
      {
         //OWERROR(OWERROR_READCOM_FAILED);
      }
   }
   else
   {
      //OWERROR(OWERROR_WRITECOM_FAILED);
   }

   // an error occured so re-sync with DS2480
   DS2480Detect(portnum);

   return FALSE;
}

//--------------------------------------------------------------------------
// Send 8 bits of communication to the 1-Wire Net and verify that the
// 8 bits read from the 1-Wire Net is the same (write operation).  
// The parameter 'sendbyte' least significant 8 bits are used.  After the
// 8 bits are sent change the level of the 1-Wire net.
//
// 'portnum'  - number 0 to MAX_PORTNUM-1.  This number was provided to
//              OpenCOM to indicate the port number.
// 'sendbyte' - 8 bits to send (least significant bit)
//
// Returns:  TRUE: bytes written and echo was the same, strong pullup now on
//           FALSE: echo was not the same 
//
SMALLINT DS9790U_C::owWriteBytePower(int portnum, SMALLINT sendbyte)
{
   uchar sendpacket[10],readbuffer[10];
   uchar sendlen=0;
   uchar rt=FALSE;
   uchar i, temp_byte;

   // check if correct mode
   if (UMode[portnum] != MODSEL_COMMAND)
   {
      UMode[portnum] = MODSEL_COMMAND;
      sendpacket[sendlen++] = MODE_COMMAND;
   }

   // set the SPUD time value
   sendpacket[sendlen++] = CMD_CONFIG | PARMSEL_5VPULSE | PARMSET_infinite;

   // construct the stream to include 8 bit commands with the last one
   // enabling the strong-pullup
   temp_byte = sendbyte;
   for (i = 0; i < 8; i++)
   {
      sendpacket[sendlen++] = ((temp_byte & 0x01) ? BITPOL_ONE : BITPOL_ZERO)
                              | CMD_COMM | FUNCTSEL_BIT | USpeed[portnum] |
                              ((i == 7) ? PRIME5V_TRUE : PRIME5V_FALSE);
      temp_byte >>= 1;
   }

   // flush the buffers
   COMport.FlushCOM(portnum);

   // send the packet
   if (COMport.WriteCOM(portnum,sendlen,sendpacket))
   {
      // read back the 9 byte response from setting time limit
      if (COMport.ReadCOM(portnum,9,readbuffer) == 9)
      {
         // check response 
         if ((readbuffer[0] & 0x81) == 0)
         {
            // indicate the port is now at power delivery
            ULevel[portnum] = MODE_STRONG5;

            // reconstruct the echo byte
            temp_byte = 0; 
            for (i = 0; i < 8; i++)
            {
               temp_byte >>= 1;
               temp_byte |= (readbuffer[i + 1] & 0x01) ? 0x80 : 0;
            }
            
            if (temp_byte == sendbyte)
               rt = TRUE;
         }
      }
      else
      {
         //OWERROR(OWERROR_READCOM_FAILED);
      }
   }
   else
   {
      //OWERROR(OWERROR_WRITECOM_FAILED);
   }

   // if lost communication with DS2480 then reset
   if (rt != TRUE)
      DS2480Detect(portnum);
   
   return rt;
}

//--------------------------------------------------------------------------
// Send 1 bit of communication to the 1-Wire Net and verify that the
// response matches the 'applyPowerResponse' bit and apply power delivery
// to the 1-Wire net.  Note that some implementations may apply the power
// first and then turn it off if the response is incorrect.
//
// 'portnum'  - number 0 to MAX_PORTNUM-1.  This number was provided to
//              OpenCOM to indicate the port number.
// 'applyPowerResponse' - 1 bit response to check, if correct then start
//                        power delivery 
//
// Returns:  TRUE: bit written and response correct, strong pullup now on
//           FALSE: response incorrect
//
SMALLINT DS9790U_C::owReadBitPower(int portnum, SMALLINT applyPowerResponse)
{
   uchar sendpacket[3],readbuffer[3];
   uchar sendlen=0;
   uchar rt=FALSE;

   // check if correct mode
   if (UMode[portnum] != MODSEL_COMMAND)
   {
      UMode[portnum] = MODSEL_COMMAND;
      sendpacket[sendlen++] = MODE_COMMAND;
   }

   // set the SPUD time value
   sendpacket[sendlen++] = CMD_CONFIG | PARMSEL_5VPULSE | PARMSET_infinite;

   // enabling the strong-pullup after bit
   sendpacket[sendlen++] = BITPOL_ONE 
                           | CMD_COMM | FUNCTSEL_BIT | USpeed[portnum] |
                           PRIME5V_TRUE;
   // flush the buffers
   COMport.FlushCOM(portnum);

   // send the packet
   if (COMport.WriteCOM(portnum,sendlen,sendpacket))
   {
      // read back the 2 byte response from setting time limit
      if (COMport.ReadCOM(portnum,2,readbuffer) == 2)
      {
         // check response to duration set
         if ((readbuffer[0] & 0x81) == 0)
         {
            // indicate the port is now at power delivery
            ULevel[portnum] = MODE_STRONG5;

            // check the response bit
            if ((readbuffer[1] & 0x01) == applyPowerResponse)
               rt = TRUE;
            else
               owLevel(portnum,MODE_NORMAL);

            return rt;
         }
      }
      else
      {
         //OWERROR(OWERROR_READCOM_FAILED);
      };
   }
   else
   {
      //OWERROR(OWERROR_WRITECOM_FAILED);
   }

   // if lost communication with DS2480 then reset
   if (rt != TRUE)
      DS2480Detect(portnum);
   
   return rt;
}

//--------------------------------------------------------------------------
// This procedure indicates whether the adapter can deliver power.
//
// 'portnum'  - number 0 to MAX_PORTNUM-1.  This number was provided to
//              OpenCOM to indicate the port number.
//
// Returns:  TRUE  because all userial adapters have over drive. 
//
SMALLINT DS9790U_C::owHasPowerDelivery(int portnum)
{
   return TRUE;
}

//--------------------------------------------------------------------------
// This procedure indicates wether the adapter can deliver power.
//
// 'portnum'  - number 0 to MAX_PORTNUM-1.  This number was provided to
//              OpenCOM to indicate the port number.
//
// Returns:  TRUE  because all userial adapters have over drive. 
//
SMALLINT DS9790U_C::owHasOverDrive(int portnum)
{
   return TRUE;
}
//--------------------------------------------------------------------------
// This procedure creates a fixed 480 microseconds 12 volt pulse 
// on the 1-Wire Net for programming EPROM iButtons.
//
// 'portnum'  - number 0 to MAX_PORTNUM-1.  This number was provided to
//              OpenCOM to indicate the port number.
//
// Returns:  TRUE  program volatage available
//           FALSE program voltage not available  
SMALLINT DS9790U_C::owHasProgramPulse(int portnum)
{
   return ProgramAvailable[portnum];
}

//--------------------------------------------------------------------------
// The 'owFirst' finds the first device on the 1-Wire Net  This function
// contains one parameter 'alarm_only'.  When
// 'alarm_only' is TRUE (1) the find alarm command 0xEC is
// sent instead of the normal search command 0xF0.
// Using the find alarm command 0xEC will limit the search to only
// 1-Wire devices that are in an 'alarm' state.
//
// 'portnum'    - number 0 to MAX_PORTNUM-1.  This number is provided to
//                indicate the symbolic port number.
// 'do_reset'   - TRUE (1) perform reset before search, FALSE (0) do not
//                perform reset before search.
// 'alarm_only' - TRUE (1) the find alarm command 0xEC is
//                sent instead of the normal search command 0xF0
//
// Returns:   TRUE (1) : when a 1-Wire device was found and it's
//                       Serial Number placed in the global SerialNum
//            FALSE (0): There are no devices on the 1-Wire Net.
//
SMALLINT DS9790U_C::owFirst(int portnum, SMALLINT do_reset, SMALLINT alarm_only)
{
   // reset the search state
   LastDiscrepancy[portnum] = 0;
   LastDevice[portnum] = FALSE;
   LastFamilyDiscrepancy[portnum] = 0;

   return owNext(portnum, do_reset, alarm_only);
}

//--------------------------------------------------------------------------
// The 'owNext' function does a general search.  This function
// continues from the previos search state. The search state
// can be reset by using the 'owFirst' function.
// This function contains one parameter 'alarm_only'.
// When 'alarm_only' is TRUE (1) the find alarm command
// 0xEC is sent instead of the normal search command 0xF0.
// Using the find alarm command 0xEC will limit the search to only
// 1-Wire devices that are in an 'alarm' state.
//
// 'portnum'    - number 0 to MAX_PORTNUM-1.  This number was provided to
//                OpenCOM to indicate the port number.
// 'do_reset'   - TRUE (1) perform reset before search, FALSE (0) do not
//                perform reset before search.
// 'alarm_only' - TRUE (1) the find alarm command 0xEC is
//                sent instead of the normal search command 0xF0
//
// Returns:   TRUE (1) : when a 1-Wire device was found and it's
//                       Serial Number placed in the global SerialNum
//            FALSE (0): when no new device was found.  Either the
//                       last search was the last device or there
//                       are no devices on the 1-Wire Net.
//
SMALLINT DS9790U_C::owNext(int portnum, SMALLINT do_reset, SMALLINT alarm_only)
{
   uchar tmp_last_desc,pos;
   uchar tmp_serial_num[8];
   uchar readbuffer[20],sendpacket[40];
   uchar i,sendlen=0;
   uchar lastcrc8;

   // if the last call was the last one
   if (LastDevice[portnum])
   {
      // reset the search
      LastDiscrepancy[portnum] = 0;
      LastDevice[portnum] = FALSE;
      LastFamilyDiscrepancy[portnum] = 0;
      return FALSE;
   }

   // check if reset first is requested
   if (do_reset)
   {
      // reset the 1-wire
      // if there are no parts on 1-wire, return FALSE
      if (!owTouchReset(portnum))
      {
         // reset the search
         LastDiscrepancy[portnum] = 0;
         LastFamilyDiscrepancy[portnum] = 0;
         //OWERROR(OWERROR_NO_DEVICES_ON_NET);
         return FALSE;
      }
   }

   // build the command stream
   // call a function that may add the change mode command to the buff
   // check if correct mode
   if (UMode[portnum] != MODSEL_DATA)
   {
      UMode[portnum] = MODSEL_DATA;
      sendpacket[sendlen++] = MODE_DATA;
   }

   // search command
   if (alarm_only)
      sendpacket[sendlen++] = 0xEC; // issue the alarming search command
   else
      sendpacket[sendlen++] = 0xF0; // issue the search command

   // change back to command mode
   UMode[portnum] = MODSEL_COMMAND;
   sendpacket[sendlen++] = MODE_COMMAND;

   // search mode on
   sendpacket[sendlen++] = (uchar)(CMD_COMM | FUNCTSEL_SEARCHON | USpeed[portnum]);

   // change back to data mode
   UMode[portnum] = MODSEL_DATA;
   sendpacket[sendlen++] = MODE_DATA;

   // set the temp Last Descrep to none
   tmp_last_desc = 0xFF;

   // add the 16 bytes of the search
   pos = sendlen;
   for (i = 0; i < 16; i++)
      sendpacket[sendlen++] = 0;

   // only modify bits if not the first search
   if (LastDiscrepancy[portnum] != 0xFF)
   {
      // set the bits in the added buffer
      for (i = 0; i < 64; i++)
      {
         // before last discrepancy
         if (i < (LastDiscrepancy[portnum] - 1))
               bitacc(WRITE_FUNCTION,
                   bitacc(READ_FUNCTION,0,i,&SerialNum[portnum][0]),
                   (short)(i * 2 + 1),
                   &sendpacket[pos]);
         // at last discrepancy
         else if (i == (LastDiscrepancy[portnum] - 1))
                bitacc(WRITE_FUNCTION,1,
                   (short)(i * 2 + 1),
                   &sendpacket[pos]);
         // after last discrepancy so leave zeros
      }
   }

   // change back to command mode
   UMode[portnum] = MODSEL_COMMAND;
   sendpacket[sendlen++] = MODE_COMMAND;

   // search OFF
   sendpacket[sendlen++] = (uchar)(CMD_COMM | FUNCTSEL_SEARCHOFF | USpeed[portnum]);

   // flush the buffers
   COMport.FlushCOM(portnum);

   // send the packet
   if (COMport.WriteCOM(portnum,sendlen,sendpacket))
   {
      // read back the 1 byte response
      if (COMport.ReadCOM(portnum,17,readbuffer) == 17)
      {
         // interpret the bit stream
         for (i = 0; i < 64; i++)
         {
            // get the SerialNum bit
            bitacc(WRITE_FUNCTION,
                   bitacc(READ_FUNCTION,0,(short)(i * 2 + 1),&readbuffer[1]),
                   i,
                   &tmp_serial_num[0]);
            // check LastDiscrepancy
            if ((bitacc(READ_FUNCTION,0,(short)(i * 2),&readbuffer[1]) == 1) &&
                (bitacc(READ_FUNCTION,0,(short)(i * 2 + 1),&readbuffer[1]) == 0))
            {
               tmp_last_desc = i + 1;
               // check LastFamilyDiscrepancy
               if (i < 8)
                  LastFamilyDiscrepancy[portnum] = i + 1;
            }
         }

         // do dowcrc
         crc.setcrc8(portnum,0);
         for (i = 0; i < 8; i++)
            lastcrc8 = crc.docrc8(portnum,tmp_serial_num[i]);

         // check results
         if ((lastcrc8 != 0) || (LastDiscrepancy[portnum] == 63) || (tmp_serial_num[0] == 0))
         {
            // error during search
            // reset the search
            LastDiscrepancy[portnum] = 0;
            LastDevice[portnum] = FALSE;
            LastFamilyDiscrepancy[portnum] = 0;
            //OWERROR(OWERROR_SEARCH_ERROR);
            return FALSE;
         }
         // successful search
         else
         {
            // check for lastone
            if ((tmp_last_desc == LastDiscrepancy[portnum]) || (tmp_last_desc == 0xFF))
               LastDevice[portnum] = TRUE;

            // copy the SerialNum to the buffer
            for (i = 0; i < 8; i++)
               SerialNum[portnum][i] = tmp_serial_num[i];

            // set the count
            LastDiscrepancy[portnum] = tmp_last_desc;
            return TRUE;
         }
      }
      else
      {
         //OWERROR(OWERROR_READCOM_FAILED);
      }
   }
   else
   {
      //OWERROR(OWERROR_WRITECOM_FAILED);
   };

   // an error occured so re-sync with DS2480
   DS2480Detect(portnum);

   // reset the search
   LastDiscrepancy[portnum] = 0;
   LastDevice[portnum] = FALSE;
   LastFamilyDiscrepancy[portnum] = 0;

   return FALSE;
}

//--------------------------------------------------------------------------
// The 'owSerialNum' function either reads or sets the SerialNum buffer
// that is used in the search functions 'owFirst' and 'owNext'.
// This function contains two parameters, 'serialnum_buf' is a pointer
// to a buffer provided by the caller.  'serialnum_buf' should point to
// an array of 8 unsigned chars.  The second parameter is a flag called
// 'do_read' that is TRUE (1) if the operation is to read and FALSE
// (0) if the operation is to set the internal SerialNum buffer from
// the data in the provided buffer.
//
// 'portnum'       - number 0 to MAX_PORTNUM-1.  This number was provided to
//                   OpenCOM to indicate the port number.
// 'serialnum_buf' - buffer to that contains the serial number to set
//                   when do_read = FALSE (0) and buffer to get the serial
//                   number when do_read = TRUE (1).
// 'do_read'       - flag to indicate reading (1) or setting (0) the current
//                   serial number.
//
void DS9790U_C::owSerialNum(int portnum, uchar *serialnum_buf, SMALLINT do_read)
{
   uchar i;

   // read the internal buffer and place in 'serialnum_buf'
   if (do_read)
   {
      for (i = 0; i < 8; i++)
         serialnum_buf[i] = SerialNum[portnum][i];
   }
   // set the internal buffer from the data in 'serialnum_buf'
   else
   {
      for (i = 0; i < 8; i++)
         SerialNum[portnum][i] = serialnum_buf[i];
   }
}

//--------------------------------------------------------------------------
// Setup the search algorithm to find a certain family of devices
// the next time a search function is called 'owNext'.
//
// 'portnum'       - number 0 to MAX_PORTNUM-1.  This number was provided to
//                   OpenCOM to indicate the port number.
// 'search_family' - family code type to set the search algorithm to find
//                   next.
//
void DS9790U_C::owFamilySearchSetup(int portnum, SMALLINT search_family)
{
   uchar i;

   // set the search state to find search_family type devices
   SerialNum[portnum][0] = search_family;
   for (i = 1; i < 8; i++)
      SerialNum[portnum][i] = 0;
   LastDiscrepancy[portnum] = 64;
   LastDevice[portnum] = FALSE;
}

//--------------------------------------------------------------------------
// Set the current search state to skip the current family code.
//
// 'portnum'  - number 0 to MAX_PORTNUM-1.  This number was provided to
//               OpenCOM to indicate the port number.
//
void DS9790U_C::owSkipFamily(int portnum)
{
   // set the Last discrepancy to last family discrepancy
   LastDiscrepancy[portnum] = LastFamilyDiscrepancy[portnum];

   // check for end of list
   if (LastDiscrepancy[portnum] == 0)
      LastDevice[portnum] = TRUE;
}

//--------------------------------------------------------------------------
// The 'owAccess' function resets the 1-Wire and sends a MATCH Serial
// Number command followed by the current SerialNum code. After this
// function is complete the 1-Wire device is ready to accept device-specific
// commands.
//
// 'portnum'  - number 0 to MAX_PORTNUM-1.  This number was provided to
//              OpenCOM to indicate the port number.
//
// Returns:   TRUE (1) : reset indicates present and device is ready
//                       for commands.
//            FALSE (0): reset does not indicate presence or echos 'writes'
//                       are not correct.
//
SMALLINT DS9790U_C::owAccess(int portnum)
{
   uchar sendpacket[9];
   uchar i;

   // reset the 1-wire
   if (owTouchReset(portnum))
   {
      // create a buffer to use with block function
      // match Serial Number command 0x55
      sendpacket[0] = 0x55;
      // Serial Number
      for (i = 1; i < 9; i++)
         sendpacket[i] = SerialNum[portnum][i-1];

      // send/recieve the transfer buffer
      if (owBlock(portnum,FALSE,sendpacket,9))
      {
         // verify that the echo of the writes was correct
         for (i = 1; i < 9; i++)
            if (sendpacket[i] != SerialNum[portnum][i-1])
               return FALSE;
         if (sendpacket[0] != 0x55)
         {
            //OWERROR(OWERROR_WRITE_VERIFY_FAILED);
            return FALSE;
         }
         else
            return TRUE;
      }
      else
      {
         //OWERROR(OWERROR_BLOCK_FAILED);
      }
   }
   else
   {
      //OWERROR(OWERROR_NO_DEVICES_ON_NET);
   }

   // reset or match echo failed
   return FALSE;
}

//----------------------------------------------------------------------
// The function 'owVerify' verifies that the current device
// is in contact with the 1-Wire Net.
// Using the find alarm command 0xEC will verify that the device
// is in contact with the 1-Wire Net and is in an 'alarm' state.
//
// 'portnum'    - number 0 to MAX_PORTNUM-1.  This number was provided to
//                OpenCOM to indicate the port number.
// 'alarm_only' - TRUE (1) the find alarm command 0xEC
//                         is sent instead of the normal search
//                         command 0xF0.
//
// Returns:   TRUE (1) : when the 1-Wire device was verified
//                       to be on the 1-Wire Net
//                       with alarm_only == FALSE
//                       or verified to be on the 1-Wire Net
//                       AND in an alarm state when
//                       alarm_only == TRUE.
//            FALSE (0): the 1-Wire device was not on the
//                       1-Wire Net or if alarm_only
//                       == TRUE, the device may be on the
//                       1-Wire Net but in a non-alarm state.
//
SMALLINT DS9790U_C::owVerify(int portnum, SMALLINT alarm_only)
{
   uchar i,sendlen=0,goodbits=0,cnt=0,s,tst;
   uchar sendpacket[50];

   // construct the search rom
   if (alarm_only)
      sendpacket[sendlen++] = 0xEC; // issue the alarming search command
   else
      sendpacket[sendlen++] = 0xF0; // issue the search command
   // set all bits at first
   for (i = 1; i <= 24; i++)
      sendpacket[sendlen++] = 0xFF;
   // now set or clear apropriate bits for search
   for (i = 0; i < 64; i++)
      bitacc(WRITE_FUNCTION,bitacc(READ_FUNCTION,0,i,&SerialNum[portnum][0]),(int)((i+1)*3-1),&sendpacket[1]);

   // send/recieve the transfer buffer
   if (owBlock(portnum,TRUE,sendpacket,sendlen))
   {
      // check results to see if it was a success
      for (i = 0; i < 192; i += 3)
      {
         tst = (bitacc(READ_FUNCTION,0,i,&sendpacket[1]) << 1) |
                bitacc(READ_FUNCTION,0,(int)(i+1),&sendpacket[1]);

         s = bitacc(READ_FUNCTION,0,cnt++,&SerialNum[portnum][0]);

         if (tst == 0x03)  // no device on line
         {
              goodbits = 0;    // number of good bits set to zero
              break;     // quit
         }

         if (((s == 0x01) && (tst == 0x02)) ||
             ((s == 0x00) && (tst == 0x01))    )  // correct bit
            goodbits++;  // count as a good bit
      }

      // check too see if there were enough good bits to be successful
      if (goodbits >= 8)
         return TRUE;
   }
   else
   {
      //OWERROR(OWERROR_BLOCK_FAILED);
   };

   // block fail or device not present
   return FALSE;
}

//----------------------------------------------------------------------
// Perform a overdrive MATCH command to select the 1-Wire device with
// the address in the ID data register.
//
// 'portnum'  - number 0 to MAX_PORTNUM-1.  This number was provided to
//               OpenCOM to indicate the port number.
//
// Returns:  TRUE: If the device is present on the 1-Wire Net and
//                 can do overdrive then the device is selected.
//           FALSE: Device is not present or not capable of overdrive.
//
//  *Note: This function could be converted to send DS2480
//         commands in one packet.
//
SMALLINT DS9790U_C::owOverdriveAccess(int portnum)
{
   uchar sendpacket[8];
   uchar i, bad_echo = FALSE;

   // make sure normal level
   owLevel(portnum,MODE_NORMAL);

   // force to normal communication speed
   owSpeed(portnum,MODE_NORMAL);

   // call the 1-Wire Net reset function
   if (owTouchReset(portnum))
   {
      // send the match command 0x69
      if (owWriteByte(portnum,0x69))
      {
         // switch to overdrive communication speed
         owSpeed(portnum,MODE_OVERDRIVE);

         // create a buffer to use with block function
         // Serial Number
         for (i = 0; i < 8; i++)
            sendpacket[i] = SerialNum[portnum][i];

         // send/recieve the transfer buffer
         if (owBlock(portnum,FALSE,sendpacket,8))
         {
            // verify that the echo of the writes was correct
            for (i = 0; i < 8; i++)
               if (sendpacket[i] != SerialNum[portnum][i])
                  bad_echo = TRUE;
            // if echo ok then success
            if (!bad_echo)
               return TRUE;
            else
            {
               //OWERROR(OWERROR_WRITE_VERIFY_FAILED);
            }
         }
         else
         {
            //OWERROR(OWERROR_BLOCK_FAILED);
         }
      }
      else
      {
         //OWERROR(OWERROR_WRITE_BYTE_FAILED);
      };
   }
   else
   {
      //OWERROR(OWERROR_NO_DEVICES_ON_NET);
   };

   // failure, force back to normal communication speed
   owSpeed(portnum,MODE_NORMAL);

   return FALSE;
}

//--------------------------------------------------------------------------
// Bit utility to read and write a bit in the buffer 'buf'.
//
// 'op'    - operation (1) to set and (0) to read
// 'state' - set (1) or clear (0) if operation is write (1)
// 'loc'   - bit number location to read or write
// 'buf'   - pointer to array of bytes that contains the bit
//           to read or write
//
// Returns: 1   if operation is set (1)
//          0/1 state of bit number 'loc' if operation is reading
//
SMALLINT DS9790U_C::bitacc(SMALLINT op, SMALLINT state, SMALLINT loc, uchar *buf)
{
   SMALLINT nbyt,nbit;

   nbyt = (loc / 8);
   nbit = loc - (nbyt * 8);

   if (op == WRITE_FUNCTION)
   {
      if (state)
         buf[nbyt] |= (0x01 << nbit);
      else
         buf[nbyt] &= ~(0x01 << nbit);

      return 1;
   }
   else
      return ((buf[nbyt] >> nbit) & 0x01);
}

//---------------------------------------------------------------------------
// Attempt to acquire a 1-Wire net using a com port and a DS2480 based
// adapter.
//
// 'portnum'    - number 0 to MAX_PORTNUM-1.  This number was provided to
//                OpenCOM to indicate the port number.
// 'port_zstr'  - zero terminated port name.  For this platform
//                use format COMX where X is the port number.
//
// Returns: TRUE - success, COM port opened
//
// exportable functions defined in ownetu.c
SMALLINT DS9790U_C::owAcquire(int portnum, char *port_zstr)
{
   // attempt to open the communications port
   if (!COMport.OpenCOM(portnum,port_zstr))
   {
      //OWERROR(OWERROR_OPENCOM_FAILED);
      return FALSE;
   }

   // detect DS2480
   if (!DS2480Detect(portnum))
   {
      COMport.CloseCOM(portnum);
      //OWERROR(OWERROR_DS2480_NOT_DETECTED);
      return FALSE;
   }

   return TRUE;
}

//---------------------------------------------------------------------------
// Release the previously acquired a 1-Wire net.
//
// 'portnum'    - number 0 to MAX_PORTNUM-1.  This number was provided to
//                OpenCOM to indicate the port number.
//
void DS9790U_C::owRelease(int portnum)
{
   COMport.CloseCOM(portnum);
}


//--------------------------------------------------------------------------
// The 'owBlock' transfers a block of data to and from the
// 1-Wire Net with an optional reset at the begining of communication.
// The result is returned in the same buffer.
//
// 'portnum'  - number 0 to MAX_PORTNUM-1.  This number is provided to
//              indicate the symbolic port number.
// 'do_reset' - cause a owTouchReset to occure at the begining of
//              communication TRUE(1) or not FALSE(0)
// 'tran_buf' - pointer to a block of unsigned
//              chars of length 'tran_len' that will be sent
//              to the 1-Wire Net
// 'tran_len' - length in bytes to transfer

// Supported devices: all
//
// Returns:   TRUE (1) : The optional reset returned a valid
//                       presence (do_reset == TRUE) or there
//                       was no reset required.
//            FALSE (0): The reset did not return a valid prsence
//                       (do_reset == TRUE).
//
//  The maximum tran_length is 64
//
SMALLINT DS9790U_C::owBlock(int portnum, SMALLINT do_reset, uchar *tran_buf, SMALLINT tran_len)
{
   uchar sendpacket[150];
   uchar sendlen=0,pos,i;

   // check for a block too big
   if (tran_len > 64)
   {
      //OWERROR(OWERROR_BLOCK_TOO_BIG);
      return FALSE;
   }

   // check if need to do a owTouchReset first
   if (do_reset)
   {
      if (!owTouchReset(portnum))
      {
         //OWERROR(OWERROR_NO_DEVICES_ON_NET);
         return FALSE;
      }
   }

   // construct the packet to send to the DS2480
   // check if correct mode
   if (UMode[portnum] != MODSEL_DATA)
   {
      UMode[portnum] = MODSEL_DATA;
      sendpacket[sendlen++] = MODE_DATA;
   }

   // add the bytes to send
   pos = sendlen;
   for (i = 0; i < tran_len; i++)
   {
      sendpacket[sendlen++] = tran_buf[i];

      // check for duplication of data that looks like COMMAND mode
      if (tran_buf[i] == MODE_COMMAND)
         sendpacket[sendlen++] = tran_buf[i];
   }

   // flush the buffers
   COMport.FlushCOM(portnum);

   // send the packet
   if (COMport.WriteCOM(portnum,sendlen,sendpacket))
   {
      // read back the response
      if (COMport.ReadCOM(portnum,tran_len,tran_buf) == tran_len)
         return TRUE;
      else
      {
         //OWERROR(OWERROR_READCOM_FAILED);
      };
   }
   else
   {
      //OWERROR(OWERROR_WRITECOM_FAILED);
   }

   // an error occured so re-sync with DS2480
   DS2480Detect(portnum);

   return FALSE;
}

//--------------------------------------------------------------------------
// Read a Universal Data Packet from a standard NVRAM iButton
// and return it in the provided buffer. The page that the
// packet resides on is 'start_page'.  Note that this function is limited
// to single page packets. The buffer 'read_buf' must be at least
// 29 bytes long.
//
// The Universal Data Packet always start on page boundaries but
// can end anywhere.  The length is the number of data bytes not
// including the length byte and the CRC16 bytes.  There is one
// length byte. The CRC16 is first initialized to the starting
// page number.  This provides a check to verify the page that
// was intended is being read.  The CRC16 is then calculated over
// the length and data bytes.  The CRC16 is then inverted and stored
// low byte first followed by the high byte.
//
// Supported devices: DS1992, DS1993, DS1994, DS1995, DS1996, DS1982,
//                    DS1985, DS1986, DS2407, and DS1971.
//
// 'portnum'    - number 0 to MAX_PORTNUM-1.  This number is provided to
//                indicate the symbolic port number.
// 'do_access'  - flag to indicate if an 'owAccess' should be
//                peformed at the begining of the read.  This may
//                be FALSE (0) if the previous call was to read the
//                previous page (start_page-1).
// 'start_page' - page number to start the read from
// 'read_buf'   - pointer to a location to store the data read
//
// Returns:  >=0 success, number of data bytes in the buffer
//           -1  failed to read a valid UDP
//
//
SMALLINT DS9790U_C::owReadPacketStd(int portnum, SMALLINT do_access, int start_page, uchar *read_buf)
{
   uchar i,length,sendlen=0,head_len=0;
   uchar sendpacket[50];
   ushort lastcrc16;

   // check if access header is done
   // (only use if in sequention read with one access at begining)
   if (do_access)
   {
      // match command
      sendpacket[sendlen++] = 0x55;
      for (i = 0; i < 8; i++)
         sendpacket[sendlen++] = SerialNum[portnum][i];
      // read memory command
      sendpacket[sendlen++] = 0xF0;
      // write the target address
      sendpacket[sendlen++] = ((start_page << 5) & 0xFF);
      sendpacket[sendlen++] = (start_page >> 3);
      // check for DS1982 exception (redirection byte)
      if (SerialNum[portnum][0] == 0x09)
         sendpacket[sendlen++] = 0xFF;
      // record the header length
      head_len = sendlen;
   }
   // read the entire page length byte
   for (i = 0; i < 32; i++)
      sendpacket[sendlen++] = 0xFF;

   // send/recieve the transfer buffer
   if (owBlock(portnum,do_access,sendpacket,sendlen))
   {
      // seed crc with page number
      crc.setcrc16(portnum,(ushort)start_page);

      // attempt to read UDP from sendpacket
      length = sendpacket[head_len];
      crc.docrc16(portnum,(ushort)length);

      // verify length is not too large
      if (length <= 29)
      {
         // loop to read packet including CRC
         for (i = 0; i < length; i++)
         {
             read_buf[i] = sendpacket[i+1+head_len];
             crc.docrc16(portnum,read_buf[i]);
         }

         // read and compute the CRC16
         crc.docrc16(portnum,sendpacket[i+1+head_len]);
         lastcrc16 = crc.docrc16(portnum,sendpacket[i+2+head_len]);

         // verify the CRC16 is correct
         if (lastcrc16 == 0xB001)
           return length;        // return number of byte in record
         else
         {
            //OWERROR(OWERROR_CRC_FAILED);
         }
      }
      else
      {
        // OWERROR(OWERROR_INCORRECT_CRC_LENGTH);
      }
   }
   else
   {
     // OWERROR(OWERROR_BLOCK_FAILED);
   };

   // failed block or incorrect CRC
   return -1;
}

//--------------------------------------------------------------------------
// Write a Universal Data Packet onto a standard NVRAM 1-Wire device
// on page 'start_page'.  This function is limited to UDPs that
// fit on one page.  The data to write is provided as a buffer
// 'write_buf' with a length 'write_len'.
//
// The Universal Data Packet always start on page boundaries but
// can end anywhere.  The length is the number of data bytes not
// including the length byte and the CRC16 bytes.  There is one
// length byte. The CRC16 is first initialized to the starting
// page number.  This provides a check to verify the page that
// was intended is being read.  The CRC16 is then calculated over
// the length and data bytes.  The CRC16 is then inverted and stored
// low byte first followed by the high byte.
//
// Supported devices: is_eprom=0
//                        DS1992, DS1993, DS1994, DS1995, DS1996
//                    is_eprom=1, crc_type=0(CRC8)
//                        DS1982
//                    is_eprom=1, crc_type=1(CRC16)
//                        DS1985, DS1986, DS2407
//
// 'portnum'    - number 0 to MAX_PORTNUM-1.  This number is provided to
//                indicate the symbolic port number.
// 'start_page' - page number to write packet to
// 'write_buf'  - pointer to buffer containing data to write
// 'write_len'  - number of data byte in write_buf
// 'is_eprom'   - flag set if device is an EPROM (1 EPROM, 0 NVRAM)
// 'crc_type'   - if is_eprom=1 then indicates CRC type
//                (0 CRC8, 1 CRC16)
//
// Returns: TRUE(1)  success, packet written
//          FALSE(0) failure to write, contact lost or device locked
//
SMALLINT DS9790U_C::owWritePacketStd(int portnum, int start_page, uchar *write_buf,
                        SMALLINT write_len, SMALLINT is_eprom, SMALLINT crc_type)
{
   uchar construct_buffer[32];
   uchar i,buffer_cnt=0,start_address,do_access;
   ushort lastcrc16=0;

   // check to see if data too long to fit on device
   if (write_len > 29)
     return FALSE;

   // seed crc with page number
   crc.setcrc16(portnum,(ushort)start_page);

   // set length byte
   construct_buffer[buffer_cnt++] = (uchar)(write_len);
   crc.docrc16(portnum,(ushort)write_len);

   // fill in the data to write
   for (i = 0; i < write_len; i++)
   {
     lastcrc16 = crc.docrc16(portnum,write_buf[i]);
     construct_buffer[buffer_cnt++] = write_buf[i];
   }

   // add the crc
   construct_buffer[buffer_cnt++] = (uchar)(~(lastcrc16 & 0xFF));
   construct_buffer[buffer_cnt++] = (uchar)(~((lastcrc16 & 0xFF00) >> 8));

   // check if not EPROM
   if (!is_eprom)
   {
      // write the page
      if (!Write_Scratchpad(portnum,construct_buffer,start_page,buffer_cnt))
      {
         //OWERROR(OWERROR_WRITE_SCRATCHPAD_FAILED);
         return FALSE;
      }

      // copy the scratchpad
      if (!Copy_Scratchpad(portnum,start_page,buffer_cnt))
      {
         //OWERROR(OWERROR_COPY_SCRATCHPAD_FAILED);
         return FALSE;
      }

      // copy scratch pad was good then success
      return TRUE;
   }
   // is EPROM
   else
   {
      // calculate the start address
      start_address = ((start_page >> 3) << 8) | ((start_page << 5) & 0xFF);
      do_access = TRUE;
      // loop to program each byte
      for (i = 0; i < buffer_cnt; i++)
      {
         if (owProgramByte(portnum,construct_buffer[i], start_address + i,
             0x0F, crc_type, do_access) != construct_buffer[i])
         {
            //OWERROR(OWERROR_PROGRAM_BYTE_FAILED);
            return FALSE;
         }
         do_access = FALSE;
      }
      return TRUE;
   }
}

//--------------------------------------------------------------------------
// Write a byte to an EPROM 1-Wire device.
//
// Supported devices: crc_type=0(CRC8)
//                        DS1982
//                    crc_type=1(CRC16)
//                        DS1985, DS1986, DS2407
//
// 'portnum'    - number 0 to MAX_PORTNUM-1.  This number is provided to
//                indicate the symbolic port number.
// 'write_byte' - byte to program
// 'addr'       - address of byte to program
// 'write_cmd'  - command used to write (0x0F reg mem, 0x55 status)
// 'crc_type'   - CRC used (0 CRC8, 1 CRC16)
// 'do_access'  - Flag to access device for each byte
//                (0 skip access, 1 do the access)
//                WARNING, only use do_access=0 if programing the NEXT
//                byte immediatly after the previous byte.
//
// Returns: >=0   success, this is the resulting byte from the program
//                effort
//          -1    error, device not connected or program pulse voltage
//                not available
//
SMALLINT DS9790U_C::owProgramByte(int portnum, SMALLINT write_byte, int addr, SMALLINT write_cmd,
                    SMALLINT crc_type, SMALLINT do_access)
{
   ushort lastcrc16;
   uchar lastcrc8;

   // optionally access the device
   if (do_access)
   {
      if (!owAccess(portnum))
      {
         //OWERROR(OWERROR_ACCESS_FAILED);
         return -1;
      }

      // send the write command
      if (!owWriteByte(portnum,write_cmd))
      {
         //OWERROR(OWERROR_WRITE_BYTE_FAILED);
         return -1;
      }

      // send the address
      if (!owWriteByte(portnum,addr & 0xFF) || !owWriteByte(portnum,addr >> 8))
      {
         //OWERROR(OWERROR_WRITE_BYTE_FAILED);
         return -1;
      }
   }

   // send the data to write
   if (!owWriteByte(portnum,write_byte))
   {
      //OWERROR(OWERROR_WRITE_BYTE_FAILED);
      return -1;
   }

   // read the CRC
   if (crc_type == 0)
   {
      // calculate CRC8
      if (do_access)
      {
         crc.setcrc8(portnum,0);
         crc.docrc8(portnum,(uchar)write_cmd);
         crc.docrc8(portnum,(uchar)(addr & 0xFF));
         crc.docrc8(portnum,(uchar)(addr >> 8));
      }
      else
         crc.setcrc8(portnum,(uchar)(addr & 0xFF));

      crc.docrc8(portnum,(uchar)write_byte);
      // read and calculate the read crc
      lastcrc8 = crc.docrc8(portnum,(uchar)owReadByte(portnum));
      // crc should now be 0x00
      if (lastcrc8 != 0)
      {
         //OWERROR(OWERROR_CRC_FAILED);
         return -1;
      }
   }
   else
   {
      // CRC16
      if (do_access)
      {
         crc.setcrc16(portnum,0);
         crc.docrc16(portnum,(ushort)write_cmd);
         crc.docrc16(portnum,(ushort)(addr & 0xFF));
         crc.docrc16(portnum,(ushort)(addr >> 8));
      }
      else
         crc.setcrc16(portnum,(ushort)addr);
      crc.docrc16(portnum,(ushort)write_byte);
      // read and calculate the read crc
      crc.docrc16(portnum,(ushort)owReadByte(portnum));
      lastcrc16 = crc.docrc16(portnum,(ushort)owReadByte(portnum));
      // crc should now be 0xB001
      if (lastcrc16 != 0xB001)
      {
         //OWERROR(OWERROR_CRC_FAILED);
         return -1;
      }
   }

   // send the program pulse
   if (!owProgramPulse(portnum))
   {
      //OWERROR(OWERROR_PROGRAM_PULSE_FAILED);
      return -1;
   }

   // read back and return the resulting byte
   return owReadByte(portnum);
}

//--------------------------------------------------------------------------
// Write the scratchpad of a standard NVRam device such as the DS1992,3,4
// and verify its contents.
//
// 'portnum'    - number 0 to MAX_PORTNUM-1.  This number is provided to
//                indicate the symbolic port number.
// 'write_buf'  - pointer to buffer containing data to write
// 'start_page'    - page number to write packet to
// 'write_len'  - number of data byte in write_buf
//
// Returns: TRUE(1)  success, the data was written and verified
//          FALSE(0) failure, the data could not be written
//
//
SMALLINT DS9790U_C::Write_Scratchpad(int portnum, uchar *write_buf, int start_page, SMALLINT write_len)
{
   uchar i,sendlen=0;
   uchar sendpacket[50];

   // match command
   sendpacket[sendlen++] = 0x55;
   for (i = 0; i < 8; i++)
      sendpacket[sendlen++] = SerialNum[portnum][i];
   // write scratchpad command
   sendpacket[sendlen++] = 0x0F;
   // write the target address
   sendpacket[sendlen++] = ((start_page << 5) & 0xFF);
   sendpacket[sendlen++] = (start_page >> 3);

   // write packet bytes
   for (i = 0; i < write_len; i++)
      sendpacket[sendlen++] = write_buf[i];

   // send/recieve the transfer buffer
   if (owBlock(portnum,TRUE,sendpacket,sendlen))
   {
      // now attempt to read back to check
      sendlen = 0;
      // match command
      sendpacket[sendlen++] = 0x55;
      for (i = 0; i < 8; i++)
         sendpacket[sendlen++] = SerialNum[portnum][i];
      // read scratchpad command
      sendpacket[sendlen++] = 0xAA;
      // read the target address, offset and data
      for (i = 0; i < (write_len + 3); i++)
         sendpacket[sendlen++] = 0xFF;

      // send/recieve the transfer buffer
      if (owBlock(portnum,TRUE,sendpacket,sendlen))
      {
         // check address and offset of scratchpad read
         if ((sendpacket[10] != ((start_page << 5) & 0xFF)) ||
             (sendpacket[11] != (start_page >> 3)) ||
             (sendpacket[12] != (write_len - 1)))
         {
            //OWERROR(OWERROR_READ_VERIFY_FAILED);
            return FALSE;
         }

         // verify each data byte
         for (i = 0; i < write_len; i++)
            if (sendpacket[i+13] != write_buf[i])
            {
               //OWERROR(OWERROR_WRITE_VERIFY_FAILED);
               return FALSE;
            }

         // must have verified
         return TRUE;
      }
      else
      {
       //  OWERROR(OWERROR_BLOCK_FAILED);
      };
   }
   else
   {
     // OWERROR(OWERROR_BLOCK_FAILED);
   }

   // failed a block tranfer
   return FALSE;
}

//--------------------------------------------------------------------------
// Copy the contents of the scratchpad to its intended nv ram page.  The
// page and length of the data is needed to build the authorization bytes
// to copy.
//
// 'portnum'    - number 0 to MAX_PORTNUM-1.  This number is provided to
//                indicate the symbolic port number.
// 'start_page' - page number to write packet to
// 'write_len'  - number of data bytes that are being copied
//
// Returns: TRUE(1)  success
//          FALSE(0) failure
//
SMALLINT DS9790U_C::Copy_Scratchpad(int portnum, int start_page, SMALLINT write_len)
{
   uchar i,sendlen=0;
   uchar sendpacket[50];

   // match command
   sendpacket[sendlen++] = 0x55;
   for (i = 0; i < 8; i++)
      sendpacket[sendlen++] = SerialNum[portnum][i];
   // copy scratchpad command
   sendpacket[sendlen++] = 0x55;
   // write the target address
   sendpacket[sendlen++] = ((start_page << 5) & 0xFF);
   sendpacket[sendlen++] = (start_page >> 3);
   sendpacket[sendlen++] = write_len - 1;
   // read copy result
   sendpacket[sendlen++] = 0xFF;

   // send/recieve the transfer buffer
   if (owBlock(portnum,TRUE,sendpacket,sendlen))
   {
      // check address and offset of scratchpad read
      if ((sendpacket[10] != ((start_page << 5) & 0xFF)) ||
          (sendpacket[11] != (start_page >> 3)) ||
          (sendpacket[12] != (write_len - 1)) ||
          (sendpacket[13] & 0xF0))
      {
         //OWERROR(OWERROR_READ_VERIFY_FAILED);
         return FALSE;
      }
      else
         return TRUE;
   }
   else
   {
      //OWERROR(OWERROR_BLOCK_FAILED);
   }

   // failed a block tranfer
   return FALSE;
}


SMALLINT DS9790U_C::Read_ScratchPad(int portnum, char *scretchpad)
{
   uchar i, sendlen = 0;
   uchar sendpacket[50];

   // match command
   sendpacket[sendlen++] = 0x55;
   for (i = 0; i < 8; i++)
      sendpacket[sendlen++] = SerialNum[portnum][i];
   // read scratchpad command
   sendpacket[sendlen++] = 0xBE;
   // result
   for (i = 0; i < 9; i++)
      sendpacket[sendlen++] = 0xFF;

   // send/recieve the transfer buffer
   if (owBlock(portnum, TRUE, sendpacket, sendlen))
   {
       for (i = 0; i < 9; i++)
          scretchpad[i] = sendpacket[sendlen-9+i];

         return TRUE;
   }
   else
   {
      //OWERROR(OWERROR_BLOCK_FAILED);
   }

   // failed a block tranfer
   return FALSE;
}

SMALLINT DS9790U_C::Start_Convert_T(int portnum)
{
   uchar i, sendlen = 0;
   uchar sendpacket[50];

   // skip ROM
   sendpacket[sendlen++] = 0xCC;
   // Start convert
   sendpacket[sendlen++] = 0x44;

   // send/recieve the transfer buffer
   if (owBlock(portnum, TRUE, sendpacket, sendlen))
   {
         return TRUE;
   }
   else
   {
      //OWERROR(OWERROR_BLOCK_FAILED);
   }

   // failed a block tranfer
   return FALSE;
}


