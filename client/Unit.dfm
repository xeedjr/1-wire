object Form1: TForm1
  Left = 421
  Top = 387
  Width = 741
  Height = 362
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 8
    Top = 48
    Width = 75
    Height = 25
    Caption = 'Reset'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 160
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Connect'
    TabOrder = 1
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 96
    Top = 48
    Width = 75
    Height = 25
    Caption = 'Search'
    TabOrder = 2
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 184
    Top = 48
    Width = 75
    Height = 25
    Caption = 'Conv T'
    TabOrder = 3
    OnClick = Button4Click
  end
  object Memo1: TMemo
    Left = 8
    Top = 88
    Width = 537
    Height = 161
    Lines.Strings = (
      'Memo1')
    ScrollBars = ssBoth
    TabOrder = 4
  end
  object Edit1: TEdit
    Left = 8
    Top = 256
    Width = 145
    Height = 21
    TabOrder = 5
  end
  object Button5: TButton
    Left = 160
    Top = 256
    Width = 75
    Height = 25
    Caption = 'Read Scr'
    TabOrder = 6
    OnClick = Button5Click
  end
  object Button6: TButton
    Left = 24
    Top = 280
    Width = 75
    Height = 25
    Caption = 'Auto get T oll'
    TabOrder = 7
    OnClick = Button6Click
  end
  object Button7: TButton
    Left = 680
    Top = 8
    Width = 75
    Height = 25
    Caption = 'DB connect'
    TabOrder = 8
    OnClick = Button7Click
  end
  object BitBtn1: TBitBtn
    Left = 680
    Top = 48
    Width = 75
    Height = 25
    Caption = 'BitBtn1'
    TabOrder = 9
  end
  object Button8: TButton
    Left = 240
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Disconnect'
    TabOrder = 10
    OnClick = Button8Click
  end
  object Edit2: TEdit
    Left = 8
    Top = 8
    Width = 73
    Height = 21
    TabOrder = 11
    Text = '127.0.0.1'
  end
  object Edit3: TEdit
    Left = 88
    Top = 8
    Width = 57
    Height = 21
    TabOrder = 12
    Text = '15000'
  end
  object Edit4: TEdit
    Left = 352
    Top = 8
    Width = 113
    Height = 21
    TabOrder = 13
    Text = '127.0.0.1'
  end
  object Edit5: TEdit
    Left = 352
    Top = 32
    Width = 113
    Height = 21
    TabOrder = 14
    Text = 'Temperature'
  end
  object Edit6: TEdit
    Left = 352
    Top = 56
    Width = 113
    Height = 21
    TabOrder = 15
    Text = 'TEMPERATURES'
  end
  object Edit7: TEdit
    Left = 472
    Top = 8
    Width = 105
    Height = 21
    TabOrder = 16
    Text = 'postgres'
  end
  object Edit8: TEdit
    Left = 472
    Top = 32
    Width = 105
    Height = 21
    TabOrder = 17
    Text = '122121'
  end
  object Button9: TButton
    Left = 576
    Top = 8
    Width = 75
    Height = 25
    Caption = 'connect DB'
    TabOrder = 18
    OnClick = Button9Click
  end
  object Button10: TButton
    Left = 576
    Top = 40
    Width = 75
    Height = 25
    Caption = 'disconect DB'
    TabOrder = 19
    OnClick = Button10Click
  end
  object Button11: TButton
    Left = 576
    Top = 72
    Width = 75
    Height = 25
    Caption = 'cheack conn'
    TabOrder = 20
    OnClick = Button11Click
  end
  object ClientSocket1: TClientSocket
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnecting = ClientSocket1Connecting
    OnConnect = ClientSocket1Connect
    OnDisconnect = ClientSocket1Disconnect
    OnRead = ClientSocket1Read
    Left = 16
    Top = 104
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 56
    Top = 104
  end
  object ADOQuery1: TADOQuery
    Connection = ADOConnection1
    ParamCheck = False
    Parameters = <>
    SQL.Strings = (
      '')
    Left = 496
    Top = 96
  end
  object ADOConnection1: TADOConnection
    ConnectionString = 
      'Provider=MSDASQL.1;Persist Security Info=False;User ID=postgres;' +
      'Extended Properties="DSN=PostgreSQL30;DATABASE=Temperature;SERVE' +
      'R=localhost;PORT=5432;UID=postgres;SSLmode=disable;ReadOnly=0;Pr' +
      'otocol=7.4;FakeOidIndex=0;ShowOidColumn=0;RowVersioning=0;ShowSy' +
      'stemTables=0;ConnSettings=;Fetch=100;Socket=4096;UnknownSizes=0;' +
      'MaxVarcharSize=255;MaxLongVarcharSize=8190;Debug=0;CommLog=0;Opt' +
      'imizer=0;Ksqo=1;UseDeclareFetch=0;TextAsLongVarchar=1;UnknownsAs' +
      'LongVarchar=0;BoolsAsChar=1;Parse=0;CancelAsFreeStmt=0;ExtraSysT' +
      'ablePrefixes=dd_;;LFConversion=1;UpdatableCursors=1;DisallowPrem' +
      'ature=0;TrueIsMinus1=0;BI=0;ByteaAsLongVarBinary=0;UseServerSide' +
      'Prepare=0;LowerCaseIdentifier=0;GssAuthUseGSS=0;XaOpt=1"'
    Provider = 'MSDASQL.1'
    OnConnectComplete = ADOConnection1ConnectComplete
    Left = 464
    Top = 96
  end
end
