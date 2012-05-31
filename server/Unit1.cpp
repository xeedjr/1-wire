//---------------------------------------------------------------------------

#include <vcl.h>
#include <string.h>
#pragma hdrstop

#include "Unit1.h"
#include "DS9790U.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
DS9790U_C   DS9790U;

void __fastcall TForm1::Send_message(TCustomWinSocket *Socket,
                                    unsigned char type,
                                     unsigned short len_ofdata,
                                     char *Data)
{
    unsigned char buf[10];

    buf[0] = type;
    buf[1] = len_ofdata >> 8;
    buf[2] = len_ofdata;
    Socket->SendBuf(buf, 3);
    if (Data != NULL && len_ofdata > 0)
    {
        Socket->SendBuf(Data, len_ofdata);
    };
}
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ServerSocket1ClientConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    Memo1->Lines->Add("Connect");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ServerSocket1ClientDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    Memo1->Lines->Add("Disconnect");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ServerSocket1ClientError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    Memo1->Lines->Add("Client Error");    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ServerSocket1ClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    Memo1->Lines->Add("Client read");    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ServerSocket1ClientWrite(TObject *Sender,
      TCustomWinSocket *Socket)
{
    Memo1->Lines->Add("Client Write");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ServerSocket1Listen(TObject *Sender,
      TCustomWinSocket *Socket)
{
        Memo1->Lines->Add("On Listen");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
    ServerSocket1->Port = Edit1->Text.ToInt();
    ServerSocket1->Active = true;
    int res = false;

    if ((res = DS9790U.owAcquire(6, "com6")) == true)
    {
        Memo1->Lines->Add("Open adapter OK");
    }
    else
    {
        Memo1->Lines->Add("Open adapter Error");
    };
}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
    ServerSocket1->Active = false;
    DS9790U.owRelease(6);
}
//---------------------------------------------------------------------------

//--------------------------------------------------------------------------- 
// Instead of using a client socket component that you place in your application
// from the Component palette, the server client thread must use the TServerClientWinSocket 
// object that is created when the listening server socket accepts a client connection. 
// This is available as the public ClientSocket property. In addition, you can use the 
// protected HandleException method rather than writing your own thread-safe exception
// handling.
void __fastcall TMyServerThread::ClientExecute(void)
{
   unsigned char buf[65535];

   // Format incoming commands
   // 1byte - type of message
   // 2bytes - len of next data
   // n bytes - Data

   Form1->Memo1->Lines->Add("Thread Created");

   // make sure connection is active
   while (!Terminated && ClientSocket->Connected)
   {
        int ret;
        unsigned char type = 0;
        unsigned short len = 0;

        // receive Header
        if((ret = ClientSocket->ReceiveBuf(buf, 3)) != 3)
        {
            return;
        };

        type = buf[0];
        len = (buf[1] << 8) + buf[2];

        // Receive data
        if (len > 0)
        {
            if((ret = ClientSocket->ReceiveBuf(buf, len)) != len)
            {
                return;
            };
        };

        switch(type)
        {
        case 1 : // Reset
            // received comand owreset
            Form1->Memo1->Lines->Add("Received command reset 1-Wire");
            if (DS9790U.owTouchReset(6) == true)
            {
                Form1->Memo1->Lines->Add("owTouchReset OK");
            }
            else
            {
                Form1->Memo1->Lines->Add("owTouchReset Error");
            };
            break;
        case 2 : // search
            {
                unsigned char TempSN[8], InfoByte[3];
                short result;
                int i;
                char ID_str[256];

                // received comand search
                Form1->Memo1->Lines->Add("Received command search 1-Wire devices");
                result = DS9790U.owFirst( 6, true, false );
                while(result)
                {
                    DS9790U.owSerialNum( 6, TempSN, TRUE );
                    // Print the serial number
                    sprintf(ID_str, "%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
                        TempSN[0],
                        TempSN[1],
                        TempSN[2],
                        TempSN[3],
                        TempSN[4],
                        TempSN[5],
                        TempSN[6],
                        TempSN[7]);
                    Form1->Memo1->Lines->Add(ID_str);
                    Form1->Send_message(ClientSocket, 1, 8, TempSN);
                    result = DS9790U.owNext( 6, true, false );
                };
            }
            break;
        case 3 : // Start Convert T
            {
                unsigned char TempSN[8], InfoByte[3];
                short result;
                int i;
                char ID_str[256];

                // received comand start convert T
                DS9790U.Start_Convert_T(6);
                Form1->Memo1->Lines->Add("Received command start convert T");
            };
            break;
        case 4 : // Read ScretChpad
            {
                unsigned char ScratchPad[9];
                short result;
                int i;
                char ID_str[256];
                unsigned char message[65535];

                // received comand read T
                DS9790U.owSerialNum( 6, buf, FALSE );
                DS9790U.Read_ScratchPad(6, ScratchPad);

                memcpy(message, buf, 8);
                memcpy(&message[8], ScratchPad, 9);
                Form1->Send_message(ClientSocket, 2, 9 + 8, message);
                Form1->Memo1->Lines->Add("Received command read T");
            };
            break;
        }
   } 
} 
//---------------------------------------------------------------------------

void __fastcall TForm1::ServerSocket1GetThread(TObject *Sender,
      TServerClientWinSocket *ClientSocket,
      TServerClientThread *&SocketThread)
{
      SocketThread = new TMyServerThread(false, ClientSocket);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ServerSocket1Accept(TObject *Sender,
      TCustomWinSocket *Socket)
{
    Memo1->Lines->Add("Accept");
}
//---------------------------------------------------------------------------

