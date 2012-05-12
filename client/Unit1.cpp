//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit.h"
#pragma package(smart_init)

unsigned char DS_Serials[200][8];
int DS_Serials_index = 0;

//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall Recv_Thread_C::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall Recv_Thread_C::Recv_Thread_C(bool CreateSuspended)
    : TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void Recv_Thread_C::SetName()
{
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = "Recv_Thread";
    info.dwThreadID = -1;
    info.dwFlags = 0;

    __try
    {
         RaiseException( 0x406D1388, 0, sizeof(info)/sizeof(DWORD),(DWORD*)&info );
    }
    __except (EXCEPTION_CONTINUE_EXECUTION)
    {
    }
}
//---------------------------------------------------------------------------
void __fastcall Recv_Thread_C::Execute()
{
    unsigned char buf[65535];
    int ret;
    unsigned char type = 0;
    unsigned short len = 0;

    SetName();
    //---- Place thread code here ----
    while(1)
    {
    if (Form1->ClientSocket1->Socket->Connected == true)
    {
    // receive Header
    if((ret = Form1->ClientSocket1->Socket->ReceiveBuf(buf, 3)) != 3)
    {
        return;
    };

    type = buf[0];
    len = (buf[1] << 8) + buf[2];

    if (len > 0)
    {
        if((ret = Form1->ClientSocket1->Socket->ReceiveBuf(buf, len)) != len)
        {
            return;
        };
    };

    switch(type)
    {
    case 1 : // Search result
        {
            char ID_str[256];


            /* Print the serial number */
            sprintf(ID_str, "%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
                        buf[0],
                        buf[1],
                        buf[2],
                        buf[3],
                        buf[4],
                        buf[5],
                        buf[6],
                        buf[7]);
            memcpy(&DS_Serials[DS_Serials_index++][0], buf, 8);
            Form1->Memo1->Lines->Add(ID_str);
        };
        break;
    case 2 : // Resultr read ScratchPad
        {
            char ID_str[256];

            /* Print the serial number */
            sprintf(ID_str, "%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX "
                            "%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
                        buf[0],
                        buf[1],
                        buf[2],
                        buf[3],
                        buf[4],
                        buf[5],
                        buf[6],
                        buf[7],

                        buf[8],
                        buf[9],
                        buf[10],
                        buf[11],
                        buf[12],
                        buf[13],
                        buf[14],
                        buf[15],
                        buf[16]
                        );

            Form1->Memo1->Lines->Add(ID_str);
            /* Calculate the CRC 8 checksum on the received data */
            /*setcrc8(0, 0);
            for( j = 1; j < 10; j++ )
                lastcrc8 = docrc8( 0, scratchpad[j] );
            */
            short int temp2 = (buf[9] << 8) | buf[8];
            float temp_c = temp2 / 16.0;
            Form1->Memo1->Lines->Add(temp_c);

        };
        break;
    };
    };
    }
}
//---------------------------------------------------------------------------
