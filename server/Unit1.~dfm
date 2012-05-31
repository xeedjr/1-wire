object Form1: TForm1
  Left = 192
  Top = 103
  Width = 313
  Height = 194
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Memo1: TMemo
    Left = 0
    Top = 24
    Width = 305
    Height = 137
    Lines.Strings = (
      'Memo1')
    ScrollBars = ssBoth
    TabOrder = 0
  end
  object Edit1: TEdit
    Left = 0
    Top = 0
    Width = 41
    Height = 21
    TabOrder = 1
    Text = '15000'
  end
  object ServerSocket1: TServerSocket
    Active = False
    Port = 15000
    ServerType = stThreadBlocking
    OnListen = ServerSocket1Listen
    OnAccept = ServerSocket1Accept
    OnGetThread = ServerSocket1GetThread
    OnClientConnect = ServerSocket1ClientConnect
    OnClientDisconnect = ServerSocket1ClientDisconnect
    OnClientRead = ServerSocket1ClientRead
    OnClientWrite = ServerSocket1ClientWrite
    OnClientError = ServerSocket1ClientError
  end
end
