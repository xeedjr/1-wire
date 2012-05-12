//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
#include "memory.h"
#include "adapters.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------




void __fastcall TForm3::RadioButton1Click(TObject *Sender)
{
    RadioButton2->Checked = false;
    GroupBox3->Visible = false;

    RadioButton1->Checked = true;
    GroupBox2->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::RadioButton2Click(TObject *Sender)
{
    RadioButton1->Checked = false;
    GroupBox2->Visible = false;

    RadioButton2->Checked = true;
    GroupBox3->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button1Click(TObject *Sender)
{
    if (RadioButton1->Checked == true)
    {
        // com port
        memory.add_adapter(ADAPTER_DS9097U, Edit1->Text.c_str(), Edit2->Text.ToInt(), 0, NULL);
    }

}
//---------------------------------------------------------------------------





void __fastcall TForm3::FormShow(TObject *Sender)
{
    int i;

    /// Get from memory configurations
    for (i = 0; i < memory.mem.adapters_num; i++)
    {
        ListBox1->Items->Add(memory.mem.adapters[i].name);
    };
}
//---------------------------------------------------------------------------

