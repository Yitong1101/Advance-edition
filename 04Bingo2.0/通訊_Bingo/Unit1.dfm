object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Bingo'
  ClientHeight = 471
  ClientWidth = 573
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -21
  Font.Name = 'Segoe UI'
  Font.Style = []
  Menu = MainMenu1
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 30
  object Label1: TLabel
    Left = 8
    Top = 24
    Width = 56
    Height = 30
    Caption = #29609#23478
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = #28304#27849#22291#39636' M'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 374
    Top = 24
    Width = 56
    Height = 30
    Caption = #29609#23478
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = #28304#27849#22291#39636' M'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label3: TLabel
    Left = 70
    Top = 24
    Width = 121
    Height = 30
    Caption = 'Score'#65306'0'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = #28304#27849#22291#39636' M'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label4: TLabel
    Left = 436
    Top = 24
    Width = 121
    Height = 30
    Caption = 'Score'#65306'0'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = #28304#27849#22291#39636' M'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label5: TLabel
    Left = 270
    Top = 24
    Width = 16
    Height = 30
    Caption = '0'
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlack
    Font.Height = -27
    Font.Name = #28304#27849#22291#39636' M'
    Font.Style = [fsBold]
    ParentFont = False
    Visible = False
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 432
    Width = 573
    Height = 39
    Panels = <>
    SimplePanel = True
  end
  object Memo2: TMemo
    Left = 16
    Top = 328
    Width = 549
    Height = 98
    Color = clMoneyGreen
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -23
    Font.Name = #24494#36575#27491#40657#39636
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 1
  end
  object MainMenu1: TMainMenu
    Left = 288
    Top = 376
    object Tool: TMenuItem
      Caption = 'Tool'
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
    Left = 64
    Top = 377
  end
  object ClientSocket1: TClientSocket
    Active = False
    ClientType = ctNonBlocking
    Port = 13579
    OnConnect = ClientSocket1Connect
    OnDisconnect = ClientSocket1Disconnect
    OnRead = ClientSocket1Read
    OnError = ClientSocket1Error
    Left = 8
    Top = 377
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 120
    Top = 376
  end
  object Timer2: TTimer
    Enabled = False
    OnTimer = Timer2Timer
    Left = 176
    Top = 376
  end
  object Timer3: TTimer
    Enabled = False
    OnTimer = Timer3Timer
    Left = 232
    Top = 376
  end
  object Timer4: TTimer
    Enabled = False
    Interval = 1
    OnTimer = Timer4Timer
    Left = 648
    Top = 288
  end
end
