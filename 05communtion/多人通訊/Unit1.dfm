object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 560
  ClientWidth = 731
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Menu = MainMenu1
  Position = poScreenCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 15
  object StatusBar1: TStatusBar
    Left = 0
    Top = 516
    Width = 731
    Height = 44
    Panels = <>
    SimplePanel = True
  end
  object Panel1: TPanel
    Left = 191
    Top = 347
    Width = 354
    Height = 169
    Align = alCustom
    Caption = 'Player1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object Label1: TLabel
      Left = 0
      Top = 125
      Width = 99
      Height = 30
      Caption = #31820#30908#65306'100'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 120
      Top = 125
      Width = 66
      Height = 30
      Caption = #36077#27880#65306
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
    end
    object Label3: TLabel
      Left = 240
      Top = 125
      Width = 66
      Height = 30
      Caption = #40670#25976#65306
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 185
    Height = 516
    Align = alLeft
    Caption = 'Player2'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    object Label4: TLabel
      Left = 32
      Top = 279
      Width = 99
      Height = 30
      Caption = #31820#30908#65306'100'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
    end
    object Label5: TLabel
      Left = 32
      Top = 321
      Width = 66
      Height = 30
      Caption = #36077#27880#65306
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
    end
    object Label6: TLabel
      Left = 32
      Top = 357
      Width = 66
      Height = 30
      Caption = #40670#25976#65306
    end
  end
  object Panel3: TPanel
    Left = 191
    Top = 0
    Width = 354
    Height = 169
    Align = alCustom
    Caption = 'Player3'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    object Label7: TLabel
      Left = 0
      Top = 131
      Width = 99
      Height = 30
      Caption = #31820#30908#65306'100'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
    end
    object Label8: TLabel
      Left = 120
      Top = 131
      Width = 66
      Height = 30
      Caption = #36077#27880#65306
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
    end
    object Label9: TLabel
      Left = 240
      Top = 131
      Width = 66
      Height = 30
      Caption = #40670#25976#65306
    end
  end
  object Panel4: TPanel
    Left = 546
    Top = 0
    Width = 185
    Height = 516
    Align = alRight
    Caption = 'Player4'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    object Label10: TLabel
      Left = 48
      Top = 279
      Width = 99
      Height = 30
      Caption = #31820#30908#65306'100'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
    end
    object Label11: TLabel
      Left = 48
      Top = 321
      Width = 66
      Height = 30
      Caption = #36077#27880#65306
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
    end
    object Label12: TLabel
      Left = 48
      Top = 357
      Width = 66
      Height = 30
      Caption = #40670#25976#65306
    end
  end
  object ComboBox1: TComboBox
    Left = 191
    Top = 175
    Width = 218
    Height = 23
    TabOrder = 5
    Text = 'ALL'
  end
  object Memo1: TMemo
    Left = 191
    Top = 204
    Width = 218
    Height = 137
    ScrollBars = ssVertical
    TabOrder = 6
  end
  object Button1: TButton
    Left = 447
    Top = 175
    Width = 98
    Height = 25
    Caption = 'Button1'
    TabOrder = 7
    Visible = False
    OnClick = Button1Click
  end
  object Button5: TButton
    Left = 481
    Top = 278
    Width = 60
    Height = 55
    Caption = '100'
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 8
    OnClick = Button5Click
  end
  object Button4: TButton
    Left = 415
    Top = 278
    Width = 60
    Height = 55
    Caption = '50'
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 9
    OnClick = Button4Click
  end
  object Button3: TButton
    Left = 481
    Top = 206
    Width = 60
    Height = 55
    Caption = '30'
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 10
    OnClick = Button3Click
  end
  object Button2: TButton
    Left = 415
    Top = 206
    Width = 60
    Height = 55
    Caption = '10'
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    TabOrder = 11
    OnClick = Button2Click
  end
  object MainMenu1: TMainMenu
    Left = 112
    Top = 496
    object Tools: TMenuItem
      Caption = 'Tools'
      object Listen: TMenuItem
        Caption = 'Listen'
        OnClick = ListenClick
      end
      object Connect: TMenuItem
        Caption = 'Connect'
        OnClick = ConnectClick
      end
      object Disconnect: TMenuItem
        Caption = 'Disconnect'
        OnClick = DisconnectClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Start_Game: TMenuItem
        Caption = 'Start Game'
        Enabled = False
        OnClick = Start_GameClick
      end
    end
    object Debug: TMenuItem
      Caption = 'Debug'
      object Opendebugwindow: TMenuItem
        Caption = 'Open debug window'
        OnClick = OpendebugwindowClick
      end
    end
  end
  object ServerSocket1: TServerSocket
    Active = False
    Port = 13579
    ServerType = stNonBlocking
    OnAccept = ServerSocket1Accept
    OnClientConnect = ServerSocket1ClientConnect
    OnClientDisconnect = ServerSocket1ClientDisconnect
    OnClientRead = ServerSocket1ClientRead
    OnClientError = ServerSocket1ClientError
    Left = 56
    Top = 496
  end
  object ClientSocket1: TClientSocket
    Active = False
    ClientType = ctNonBlocking
    Port = 13579
    OnConnect = ClientSocket1Connect
    OnDisconnect = ClientSocket1Disconnect
    OnRead = ClientSocket1Read
    OnError = ClientSocket1Error
    Top = 496
  end
  object IdIPWatch1: TIdIPWatch
    Active = False
    HistoryFilename = 'iphist.dat'
    Left = 168
    Top = 496
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 5000
    OnTimer = Timer1Timer
    Left = 223
    Top = 496
  end
  object Timer2: TTimer
    Enabled = False
    Interval = 3000
    OnTimer = Timer2Timer
    Left = 279
    Top = 496
  end
  object Timer3: TTimer
    Enabled = False
    OnTimer = Timer3Timer
    Left = 335
    Top = 496
  end
  object Timer4: TTimer
    Enabled = False
    Interval = 5000
    OnTimer = Timer4Timer
    Left = 391
    Top = 499
  end
  object Timer5: TTimer
    Interval = 5000
    OnTimer = Timer5Timer
    Left = 447
    Top = 499
  end
end
