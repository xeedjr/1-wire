object Form1: TForm1
  Left = 338
  Top = 148
  Width = 688
  Height = 478
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object StatusBar1: TStatusBar
    Left = 0
    Top = 385
    Width = 681
    Height = 25
    Panels = <
      item
        Width = 50
      end
      item
        Width = 50
      end
      item
        Width = 50
      end
      item
        Width = 50
      end
      item
        Width = 50
      end
      item
        Width = 50
      end>
    SimplePanel = False
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 681
    Height = 41
    TabOrder = 1
    object Button1: TButton
      Left = 8
      Top = 8
      Width = 49
      Height = 25
      Caption = #1057#1090#1072#1088#1090
      TabOrder = 0
      OnClick = Button1Click
    end
    object Button2: TButton
      Left = 64
      Top = 8
      Width = 41
      Height = 25
      Caption = #1057#1090#1086#1087
      TabOrder = 1
      OnClick = Button2Click
    end
    object Button3: TButton
      Left = 112
      Top = 8
      Width = 105
      Height = 25
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1075#1088#1072#1092#1110#1082#1091
      TabOrder = 2
    end
    object Button4: TButton
      Left = 232
      Top = 8
      Width = 129
      Height = 25
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1087#1088#1086#1075#1088#1072#1084#1084#1080
      TabOrder = 3
      OnClick = Button4Click
    end
    object Button5: TButton
      Left = 376
      Top = 8
      Width = 153
      Height = 25
      Caption = #1042#1110#1076#1082#1088#1080#1090#1080' '#1084#1077#1085#1077#1076#1078#1077#1088' '#1086#1073#1108#1082#1090#1110#1074
      TabOrder = 4
      OnClick = Button5Click
    end
  end
  object Chart1: TChart
    Left = 8
    Top = 40
    Width = 673
    Height = 257
    BackWall.Brush.Color = clWhite
    BackWall.Brush.Style = bsClear
    Title.Text.Strings = (
      'TChart')
    BottomAxis.Title.Caption = #1063#1072#1089' '#1110' '#1044#1072#1090#1072
    LeftAxis.Title.Caption = #1058#1077#1084#1087#1077#1088#1072#1090#1091#1088#1072
    View3D = False
    TabOrder = 2
  end
  object Memo1: TMemo
    Left = 0
    Top = 344
    Width = 681
    Height = 41
    Lines.Strings = (
      'Memo1')
    TabOrder = 3
  end
  object MainMenu1: TMainMenu
    Top = 32
    object F1: TMenuItem
      Caption = #1060#1072#1081#1083
      object N4: TMenuItem
        Caption = #1042#1080#1093#1110#1076
        OnClick = N4Click
      end
    end
    object N1: TMenuItem
      Caption = #1059#1087#1088#1072#1074#1083#1110#1085#1085#1103
      object N5: TMenuItem
        Caption = #1057#1090#1072#1088#1090
      end
      object N6: TMenuItem
        Caption = #1057#1090#1086#1087
      end
      object N8: TMenuItem
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072
        OnClick = N8Click
      end
      object N7: TMenuItem
        Caption = #1052#1077#1085#1077#1076#1078#1077#1088' '#1086#1073#1108#1082#1090#1110#1074
        OnClick = N7Click
      end
    end
    object N2: TMenuItem
      Caption = #1042#1080#1076
      object N9: TMenuItem
        Caption = #1058#1072#1073#1083#1080#1094#1103
      end
      object N10: TMenuItem
        Caption = #1051#1086#1075
      end
      object N11: TMenuItem
        Caption = #1043#1088#1072#1092#1110#1082
      end
      object N12: TMenuItem
        Caption = #1055#1072#1085#1077#1083#1100' '#1110#1085#1089#1090#1088#1091#1084#1077#1085#1090#1110#1074
      end
      object N13: TMenuItem
        Caption = #1056#1103#1076#1086#1082' '#1089#1090#1072#1090#1091#1089#1091
      end
    end
    object N3: TMenuItem
      Caption = #1057#1087#1088#1072#1074#1082#1072
    end
  end
end
