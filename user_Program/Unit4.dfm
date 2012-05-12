object Form4: TForm4
  Left = 282
  Top = 209
  Width = 870
  Height = 500
  Caption = 'Form4'
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
  object TreeView1: TTreeView
    Left = 0
    Top = 0
    Width = 833
    Height = 289
    HideSelection = False
    Indent = 19
    MultiSelect = True
    PopupMenu = PopupMenu1
    RightClickSelect = True
    TabOrder = 0
    Items.Data = {
      01000000200000000000000000000000FFFFFFFFFFFFFFFF0000000000000000
      07C2E8E1F0E0EDB3}
  end
  object Memo1: TMemo
    Left = 0
    Top = 288
    Width = 337
    Height = 89
    Lines.Strings = (
      'Memo1')
    TabOrder = 1
  end
  object PopupMenu1: TPopupMenu
    Top = 392
    object gfds1: TMenuItem
      Caption = #1054#1087#1080#1090#1072#1090#1080' '#1072#1076#1072#1087#1090#1077#1088
      OnClick = gfds1Click
    end
    object N1: TMenuItem
      Caption = #1044#1086#1073#1072#1074#1080#1090#1080' '#1076#1086' '#1074#1080#1073#1088#1072#1085#1080#1093
    end
    object TMenuItem
    end
  end
end
