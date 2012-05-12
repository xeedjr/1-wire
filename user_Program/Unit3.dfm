object Form3: TForm3
  Left = 478
  Top = 117
  Width = 433
  Height = 639
  Caption = 'Form3'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object TabControl1: TTabControl
    Left = 0
    Top = 0
    Width = 425
    Height = 601
    TabOrder = 0
    Tabs.Strings = (
      #1054#1089#1085#1086#1074#1085#1110
      #1030#1085#1090#1077#1088#1092#1077#1081#1089
      #1051#1086#1075
      #1047#1074#1091#1082#1080)
    TabIndex = 0
    object GroupBox1: TGroupBox
      Left = 16
      Top = 32
      Width = 393
      Height = 281
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1072#1076#1072#1087#1090#1077#1088#1110#1074
      TabOrder = 0
      object Label1: TLabel
        Left = 152
        Top = 40
        Width = 32
        Height = 13
        Caption = #1053#1072#1079#1074#1072
      end
      object GroupBox3: TGroupBox
        Left = 160
        Top = 104
        Width = 185
        Height = 105
        Caption = #1087#1086' '#1084#1077#1088#1077#1078#1110
        TabOrder = 7
        Visible = False
        object Label3: TLabel
          Left = 8
          Top = 16
          Width = 10
          Height = 13
          Caption = 'IP'
        end
        object Label4: TLabel
          Left = 8
          Top = 48
          Width = 18
          Height = 13
          Caption = 'port'
        end
        object Edit3: TEdit
          Left = 24
          Top = 16
          Width = 121
          Height = 21
          TabOrder = 0
          Text = '127.0.0.1'
        end
        object Edit4: TEdit
          Left = 32
          Top = 48
          Width = 121
          Height = 21
          TabOrder = 1
          Text = '8900'
        end
      end
      object GroupBox2: TGroupBox
        Left = 160
        Top = 104
        Width = 185
        Height = 105
        Caption = 'COM '#1087#1086#1088#1090
        TabOrder = 6
        object Label2: TLabel
          Left = 8
          Top = 24
          Width = 25
          Height = 13
          Caption = #1055#1086#1088#1090
        end
        object Edit2: TEdit
          Left = 40
          Top = 24
          Width = 121
          Height = 21
          TabOrder = 0
          Text = '6'
        end
      end
      object ListBox1: TListBox
        Left = 16
        Top = 40
        Width = 129
        Height = 209
        ItemHeight = 13
        TabOrder = 0
      end
      object Button1: TButton
        Left = 160
        Top = 224
        Width = 121
        Height = 25
        Caption = #1044#1086#1076#1072#1090#1080' '#1085#1086#1074#1080#1081' '#1072#1076#1072#1087#1090#1077#1088
        TabOrder = 1
        OnClick = Button1Click
      end
      object Button2: TButton
        Left = 288
        Top = 224
        Width = 75
        Height = 25
        Caption = #1042#1080#1076#1072#1083#1080#1090#1080
        TabOrder = 2
      end
      object Edit1: TEdit
        Left = 192
        Top = 40
        Width = 121
        Height = 21
        TabOrder = 3
        Text = #1052#1110#1081' '#1087#1077#1088#1077#1093#1110#1076#1085#1080#1082
      end
      object RadioButton1: TRadioButton
        Left = 152
        Top = 64
        Width = 113
        Height = 17
        Caption = #1085#1072' COM '#1087#1086#1088#1090#1110
        Checked = True
        TabOrder = 4
        TabStop = True
        OnClick = RadioButton1Click
      end
      object RadioButton2: TRadioButton
        Left = 152
        Top = 88
        Width = 113
        Height = 17
        Caption = #1087#1086' '#1084#1077#1088#1077#1078#1110
        TabOrder = 5
        OnClick = RadioButton2Click
      end
    end
  end
end
