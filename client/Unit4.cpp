//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit4.h"
#include "Unit1.h"

#include <libpq-fe.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;

/*void TForm4::DB_Get_T(unsigned char *Serial, float temp)
{
    AnsiString SQLstmt;
    PGresult *result = NULL;

    SQLstmt.printf("INSERT INTO public.\"%s\" (\"Serial_ID\", \"T\") VALUES ('%hhX%hhX%hhX%hhX%hhX%hhX%hhX%hhX', %f)",
                        "",
                        Serial[0],
                        Serial[1],
                        Serial[2],
                        Serial[3],
                        Serial[4],
                        Serial[5],
                        Serial[6],
                        Serial[7],
                        temp);

    if (PQstatus(Form1->conn) == 0)
    {
        result = PQexec(Form1->conn, SQLstmt.c_str());
        if (PQresultStatus(result) != PGRES_COMMAND_OK)
        {
            Form1->Memo1->Lines->Add("9Error");
            Form1->Memo1->Lines->Add(PQresultErrorMessage(result));
        }
    }
    else
    {
        Form1->Memo1->Lines->Add("disconnected");
    };


}
*/
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm4::Button1Click(TObject *Sender)
{
    AnsiString date_start, time_stop;
    int start_day, start_month, start_year;

    //sscanf(DateTimePicker1->Date.DateString().c_str(), "%d.%d.%d", &start_day, &start_month, &start_year);


}
//---------------------------------------------------------------------------



