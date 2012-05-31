//---------------------------------------------------------------------------

#ifndef UnitH
#define UnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ScktComp.hpp>
#include <ExtCtrls.hpp>
#include "Unit1.h"
#include <ADODB.hpp>
#include <DB.hpp>
#include <Buttons.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TClientSocket *ClientSocket1;
    TButton *Button1;
    TButton *Button2;
    TButton *Button3;
    TButton *Button4;
    TMemo *Memo1;
    TEdit *Edit1;
    TButton *Button5;
    TTimer *Timer1;
    TButton *Button6;
    TButton *Button7;
    TADOQuery *ADOQuery1;
    TADOConnection *ADOConnection1;
    TBitBtn *BitBtn1;
    TButton *Button8;
    TEdit *Edit2;
    TEdit *Edit3;
    TEdit *Edit4;
    TEdit *Edit5;
    TEdit *Edit6;
    TEdit *Edit7;
    TEdit *Edit8;
    TButton *Button9;
    TButton *Button10;
    TButton *Button11;
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall Button5Click(TObject *Sender);
    void __fastcall Button6Click(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall ClientSocket1Connecting(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket1Disconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket1Connect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket1Read(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall Button7Click(TObject *Sender);
    void __fastcall ADOConnection1ConnectComplete(
          TADOConnection *Connection, const Error *Error,
          TEventStatus &EventStatus);
    void __fastcall Button8Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Button9Click(TObject *Sender);
    void __fastcall Button10Click(TObject *Sender);
    void __fastcall Button11Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    void Write_to_DB(unsigned char *Serial, float temp);
    void Write_to_PQDB(unsigned char *Serial, float temp);
    Recv_Thread_C *thread;
    void __fastcall Thread( void* pParams );
    void __fastcall Send_message(unsigned char type,
                                     unsigned short len_ofdata,
                                     char *Data);
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
