//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ScktComp.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TServerSocket *ServerSocket1;
    TMemo *Memo1;
    TEdit *Edit1;
    void __fastcall ServerSocket1ClientConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ServerSocket1ClientDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ServerSocket1ClientError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall ServerSocket1ClientRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ServerSocket1ClientWrite(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ServerSocket1Listen(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall ServerSocket1GetThread(TObject *Sender,
          TServerClientWinSocket *ClientSocket,
          TServerClientThread *&SocketThread);
    void __fastcall ServerSocket1Accept(TObject *Sender,
          TCustomWinSocket *Socket);
private:	// User declarations
public:		// User declarations
    void __fastcall Send_message(TCustomWinSocket *Socket,
                                    unsigned char type,
                                     unsigned short len_ofdata,
                                     char *Data);
    __fastcall TForm1(TComponent* Owner);
};
//--------------------------------------------------------------------------- 
// Threads for server connections are descendants of TServerClientThread. 
// Thus, you may not use the New Thread object dialog. 
// Instead, declare your thread manually as follows: 
class PACKAGE TMyServerThread : public Scktcomp::TServerClientThread 
{ 
   public: 
      // if true, FreeOnTerminate is set to false before the thread terminates, 
      // and the thread is left in the thread cache. When KeepInCache is false, 
      // the thread is freed when execution terminates.
      __fastcall TMyServerThread(bool CreateSuspended, TServerClientWinSocket* ASocket) 
         : Scktcomp::TServerClientThread(CreateSuspended, ASocket) 
         { CreateSuspended = false; KeepInCache=true; FreeOnTerminate=false; };

      // To implement this thread, you override the ClientExecute method instead of the Execute method. 
      void __fastcall ClientExecute(void); 
}; 
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
