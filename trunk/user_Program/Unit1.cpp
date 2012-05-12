//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit3.h"
#include "Unit4.h"
#include "adapters.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Unit2"
#pragma resource "*.dfm"
TForm1 *Form1;
TAdapters   Adapter;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N4Click(TObject *Sender)
{
    exit(1);
}
//---------------------------------------------------------------------------



void __fastcall TForm1::FormResize(TObject *Sender)
{
    Panel1->Width = Form1->Width;
    Chart1->Width = Form1->Width;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    Panel1->Width = Form1->Width;
    Chart1->Width = Form1->Width;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
    Memo1->Lines->Add("Старт");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
    Memo1->Lines->Add("Стоп");
}
//---------------------------------------------------------------------------




void __fastcall TForm1::Button4Click(TObject *Sender)
{
    Form3->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
    Form4->Visible = true;    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N8Click(TObject *Sender)
{
    Form3->Visible = true;    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N7Click(TObject *Sender)
{
    Form4->Visible = true;      
}
//---------------------------------------------------------------------------

