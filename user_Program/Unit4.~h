//---------------------------------------------------------------------------

#ifndef Unit4H
#define Unit4H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include "DS9790U.h"
//---------------------------------------------------------------------------
class TForm4 : public TForm
{
__published:	// IDE-managed Components
    TTreeView *TreeView1;
    TMemo *Memo1;
    TPopupMenu *PopupMenu1;
    TMenuItem *gfds1;
    TMenuItem *N1;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall gfds1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    DS9790U_C DS9790U;
    __fastcall TForm4(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm4 *Form4;
//---------------------------------------------------------------------------
#endif
