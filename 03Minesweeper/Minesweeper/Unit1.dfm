object Form1: TForm1
  Left = 0
  Top = 0
  BorderStyle = bsSingle
  Caption = 'Minesweeper'
  ClientHeight = 400
  ClientWidth = 400
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clBlack
  Font.Height = -27
  Font.Name = 'Segoe UI'
  Font.Style = []
  Menu = MainMenu1
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 37
  object Label1: TLabel
    Left = 40
    Top = 64
    Width = 168
    Height = 37
    Caption = #35531#36664#20837#38263#24230#65306
  end
  object Label2: TLabel
    Left = 40
    Top = 144
    Width = 168
    Height = 37
    Caption = #35531#36664#20837#23532#24230#65306
  end
  object Label3: TLabel
    Left = 40
    Top = 224
    Width = 196
    Height = 37
    Caption = #35531#36664#20837#28856#24392#25976#65306
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 400
    Height = 400
    Align = alClient
    TabOrder = 4
    Visible = False
  end
  object Button1: TButton
    Left = 40
    Top = 288
    Width = 323
    Height = 49
    Caption = #36938#25138#38283#22987
    TabOrder = 0
    OnClick = Button1Click
  end
  object Edit1: TEdit
    Left = 242
    Top = 61
    Width = 121
    Height = 45
    TabOrder = 1
    Text = '10'
  end
  object Edit2: TEdit
    Left = 242
    Top = 141
    Width = 121
    Height = 45
    TabOrder = 2
    Text = '10'
  end
  object Edit3: TEdit
    Left = 242
    Top = 221
    Width = 121
    Height = 45
    TabOrder = 3
    Text = '5'
  end
  object MainMenu1: TMainMenu
    object N1: TMenuItem
      Caption = 'Game'
      object N2: TMenuItem
        Caption = #36879#35222#28856#24392
        OnClick = N2Click
      end
      object N3: TMenuItem
        Caption = #37325#26032#38283#22987
        OnClick = N3Click
      end
      object N6: TMenuItem
        Caption = #35373#23450#36938#25138
        OnClick = N6Click
      end
    end
    object N4: TMenuItem
      Caption = 'Time'#65306'0'
    end
    object N5: TMenuItem
      Caption = 'Boom'
    end
    object N7: TMenuItem
      Caption = 'Flag'
    end
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 56
  end
end
