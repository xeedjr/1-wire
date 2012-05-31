//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "Unit.h"
#include "Unit1.h"
#include "CRC.h"
#include <libpq-fe.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
int Receive_stat = 1;
unsigned char type = 0;
unsigned short len = 0;
PGconn *conn;
int Timer_state = 1;

void TForm1::Write_to_DB(unsigned char *Serial, float temp)
{
    AnsiString SQLstmt;

    SQLstmt.printf("INSERT INTO public.\"%s\" (\"Serial_ID\", \"T\") VALUES ('%hhX%hhX%hhX%hhX%hhX%hhX%hhX%hhX', %f)",
                        Edit6->Text,
                        Serial[0],
                        Serial[1],
                        Serial[2],
                        Serial[3],
                        Serial[4],
                        Serial[5],
                        Serial[6],
                        Serial[7],
                        temp);
//ADOConnection1->ex
ADOQuery1->SQL->Clear();
ADOQuery1->SQL->Add(SQLstmt);
ADOQuery1->ExecSQL();

}

void TForm1::Write_to_PQDB(unsigned char *Serial, float temp)
{
    AnsiString SQLstmt;
    PGresult *result = NULL;

    SQLstmt.printf("INSERT INTO public.\"%s\" (\"Serial_ID\", \"T\") VALUES ('%hhX%hhX%hhX%hhX%hhX%hhX%hhX%hhX', %f)",
                        Edit6->Text,
                        Serial[0],
                        Serial[1],
                        Serial[2],
                        Serial[3],
                        Serial[4],
                        Serial[5],
                        Serial[6],
                        Serial[7],
                        temp);

    if (PQstatus(conn) == 0)
    {
        result = PQexec(conn, SQLstmt.c_str());
        if (PQresultStatus(result) != PGRES_COMMAND_OK)
        {
            Memo1->Lines->Add("9Error");
            Memo1->Lines->Add(PQresultErrorMessage(result));
        }
    }
    else
    {
        Memo1->Lines->Add("disconnected");
    };


}

void __fastcall TForm1::Send_message(unsigned char type,
                                     unsigned short len_ofdata,
                                     char *Data)
{
    unsigned char buf[10];

    buf[0] = type;
    buf[1] = len_ofdata >> 8;
    buf[2] = len_ofdata;
    if (ClientSocket1->Socket->Connected == true)
    {
        ClientSocket1->Socket->SendBuf(buf, 3);
        if (Data != NULL)
        {
            ClientSocket1->Socket->SendBuf(Data, len_ofdata);
        };
    };
}

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
    ClientSocket1->Host = Edit2->Text;
    ClientSocket1->Port = Edit3->Text.ToInt();
    ClientSocket1->Open();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
    Send_message(1, 0, NULL);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)
{
    Send_message(2, 0, NULL);
    DS_Serials_index = 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
    Send_message(3, 0, NULL);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button5Click(TObject *Sender)
{
    char Serial[8];

    sscanf(Edit1->Text.c_str(), "%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
                        &Serial[0],
                        &Serial[1],
                        &Serial[2],
                        &Serial[3],
                        &Serial[4],
                        &Serial[5],
                        &Serial[6],
                        &Serial[7]);
    Send_message(4, 8, Serial);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
    Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
    switch(Timer_state)
    {
    case 1:
        {
            // Start Convert T
            Send_message(3, 0, NULL);
            Timer_state = 2;
        }
        break;
    case 2:
        {
            // read T
            int i;

            for (i = 0; i < DS_Serials_index; i++)
            {
                // Get
                Send_message(4, 8, &DS_Serials[i][0]);
            }
            Timer_state = 1;
        }
        break;
    };


}
//---------------------------------------------------------------------------


void __fastcall TForm1::ClientSocket1Connecting(TObject *Sender,
      TCustomWinSocket *Socket)
{
    Memo1->Lines->Add("Conecting");    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ClientSocket1Disconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    Memo1->Lines->Add("Disconnect");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ClientSocket1Connect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    HANDLE hThr;
    unsigned long uThrID;

    Memo1->Lines->Add("Connect");
    //thread = new Recv_Thread_C(false);

    //hThr=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread, NULL, 0, &uThrID);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ClientSocket1Read(TObject *Sender,
      TCustomWinSocket *Socket)
{
    unsigned char buf[65535];
    int ret;
    CRC_C crc;

    switch(Receive_stat)
    {
    case 1: //headr
        // receive Header
        if (ClientSocket1->Socket->ReceiveLength() >= 3)
        {
            if((ret = Form1->ClientSocket1->Socket->ReceiveBuf(buf, 3)) != 3)
            {
                return;
            };
            Receive_stat = 2;
            type = buf[0];
            len = (buf[1] << 8) + buf[2];
        };
        break;
    case 2:
        if (len == 0)
        {
            Receive_stat = 1;
        }
        if (ClientSocket1->Socket->ReceiveLength() >= len)
        {
            if((ret = Form1->ClientSocket1->Socket->ReceiveBuf(buf, len)) != len)
            {
                return;
            };
            Receive_stat = 1;
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
                         int lastcrc8, j;
                         unsigned char scratchpad[9];

                         memcpy(scratchpad, &buf[8], 9);
                          // Print the serial number
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

                            scratchpad[0],
                            scratchpad[1],
                            scratchpad[2],
                            scratchpad[3],
                            scratchpad[4],
                            scratchpad[5],
                            scratchpad[6],
                            scratchpad[7],
                            scratchpad[8]
                            );

                        Form1->Memo1->Lines->Add(ID_str);
                        //Calculate the CRC 8 checksum on the received data
                        crc.setcrc8(0, 0);
                        for( j = 0; j < 9; j++ )
                            lastcrc8 = crc.docrc8( 0, scratchpad[j] );
                        if(lastcrc8 == 0)
                        {
                            // scratchpad is Corrrect CRC GOOOOD
                            short int temp2 = (scratchpad[1] << 8) | scratchpad[0];
                            float temp_c = temp2 / 16.0;
                            Form1->Memo1->Lines->Add(temp_c);

                            if (ADOConnection1->Connected == true)
                            {
                                Write_to_DB(buf, temp_c);
                            };

                            Write_to_PQDB(buf, temp_c);
                        }
                        else
                        {
                            Form1->Memo1->Lines->Add("CRC error");
                        };

                     };
                    break;
                };

        }
        break;
    };


}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
//ADOQuery1->ConnectionString = 'Provider=PostgreSQL.1;Data Source=сервер;Location=база;User ID=имя_пользователя;Password=пароль';
//ADOQuery1.SQL.Text := 'select * from table';
    AnsiString connectstr;

    connectstr.printf("Provider=MSDASQL.1;Persist Security Info=False;User ID=%s;Extended Properties=\"DSN=PostgreSQL30;DATABASE=%s;SERVER=%s;PORT=5432;UID=%s;SSLmode=disable;ReadOnly=0;Protocol=7.4;FakeOidIndex=0;ShowOidColumn=0;RowVersioning=0;ShowSystemTables=0;ConnSettings=;Fetch=100;Socket=4096;UnknownSizes=0;MaxVarcharSize=255;MaxLongVarcharSize=8190;Debug=0;CommLog=0;Optimizer=0;Ksqo=1;UseDeclareFetch=0;TextAsLongVarchar=1;UnknownsAsLongVarchar=0;BoolsAsChar=1;Parse=0;CancelAsFreeStmt=0;ExtraSysTablePrefixes=dd_;;LFConversion=1;UpdatableCursors=1;DisallowPremature=0;TrueIsMinus1=0;BI=0;ByteaAsLongVarBinary=0;UseServerSidePrepare=0;LowerCaseIdentifier=0;GssAuthUseGSS=0;XaOpt=1\"", Edit7->Text, Edit5->Text, Edit4->Text, Edit7->Text);
    if (ADOConnection1->Connected == false)
    {
        ADOConnection1->ConnectionString = connectstr;
        ADOConnection1->Connected = true;
    }
    else
    {
        Memo1->Lines->Add("Olready connected to DB");
    };


}
//---------------------------------------------------------------------------

void __fastcall TForm1::ADOConnection1ConnectComplete(
      TADOConnection *Connection, const Error *Error,
      TEventStatus &EventStatus)
{
    Memo1->Lines->Add("DB connect Compleate");    
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button8Click(TObject *Sender)
{
    ClientSocket1->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
    ClientSocket1->Close();
    ADOConnection1->Close();
    if (PQstatus(conn) == 0)
    {
        PQfinish(conn);
    }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button9Click(TObject *Sender)
{
    AnsiString connectstr;

    connectstr.printf("hostaddr = %s port = %s dbname = %s user = %s password = %s",
                        Edit4->Text,
                        "5432",
                        Edit5->Text,
                        Edit7->Text,
                        Edit8->Text);

    conn = PQconnectdb(connectstr.c_str());

    if (PQstatus( conn ) != CONNECTION_OK)
    {
        // error
        Memo1->Lines->Add(connectstr);
        Memo1->Lines->Add(PQerrorMessage(conn));

        PQfinish(conn);
    }
    else
    {
        // OK
        Memo1->Lines->Add("conection established");
        Memo1->Lines->Add(PQparameterStatus(conn, "server_version"));
        Memo1->Lines->Add(PQuser(conn));
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button10Click(TObject *Sender)
{
    if (PQstatus(conn) == 0)
    {
        PQfinish(conn);
        Memo1->Lines->Add("disconected OK");
    }
    else
    {
        Memo1->Lines->Add("Already disconnected");
    };


}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button11Click(TObject *Sender)
{
      Memo1->Lines->Add(PQstatus(conn));
}
//---------------------------------------------------------------------------


