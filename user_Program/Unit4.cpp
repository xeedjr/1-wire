//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit4.h"
#include "memory.h"
#include "adapters.h"
#include "DS9790U.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;



//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------






void __fastcall TForm4::FormShow(TObject *Sender)
{
    // Get config information from memory
    int i;

    for (i = 0; i < memory.mem.adapters_num; i++)
    {
        TreeView1->Items->Add(NULL, memory.mem.adapters[i].name);
    };
}
//---------------------------------------------------------------------------
void __fastcall TForm4::gfds1Click(TObject *Sender)
{
   uchar i,sendlen=0;
   uchar sendpacket[50];

   /* DS9790U.Connect(6);
    if (DS9790U.DS2480Detect(6) == true)
    {
        Memo1->Lines->Add("Detect OK");
    }
    else
    {
        Memo1->Lines->Add("Detect Error");
        DS9790U.Disconnect();
    };
    */
    if (DS9790U.owAcquire(6, "com6") == true)
    {
        Memo1->Lines->Add("owAcquire OK");
    }
    else
    {
        Memo1->Lines->Add("owAcquire Error");
    };
    
    if (DS9790U.DS2480Detect(6) == true)
    {
        Memo1->Lines->Add("Detect OK");
    }
    else
    {
        Memo1->Lines->Add("Detect Error");
        DS9790U.owRelease(6);
    };

    sendpacket[sendlen++] = 0xCC; //Skip ROM
    sendpacket[sendlen++] = 0x44; //Convert temperature

    if (DS9790U.owBlock(6, 1, sendpacket, sendlen) == true)
    {
        // OK
        Memo1->Lines->Add("owBlock OK");
    }
    else
    {
        //error
        Memo1->Lines->Add("owBlock Error");
    };

    Memo1->Lines->Add(TreeView1->Selected->Text);

}
//---------------------------------------------------------------------------


