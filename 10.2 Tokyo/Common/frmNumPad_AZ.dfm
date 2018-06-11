object fmNumPad_AZ: TfmNumPad_AZ
  Left = 705
  Top = 275
  BorderIcons = []
  Caption = 'Number Pad'
  ClientHeight = 507
  ClientWidth = 379
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object SpeedButton1: TSpeedButton
    Tag = 1
    Left = 2
    Top = 150
    Width = 123
    Height = 67
    Caption = '1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 16452374
    Font.Height = -32
    Font.Name = 'Arial Black'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton1Click
  end
  object SpeedButton2: TSpeedButton
    Tag = 2
    Left = 128
    Top = 150
    Width = 123
    Height = 67
    Caption = '2'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 16452374
    Font.Height = -32
    Font.Name = 'Arial Black'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton1Click
  end
  object SpeedButton3: TSpeedButton
    Tag = 3
    Left = 254
    Top = 150
    Width = 123
    Height = 67
    Caption = '3'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 16452374
    Font.Height = -32
    Font.Name = 'Arial Black'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton1Click
  end
  object SpeedButton4: TSpeedButton
    Tag = 4
    Left = 2
    Top = 218
    Width = 123
    Height = 67
    Caption = '4'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 16452374
    Font.Height = -32
    Font.Name = 'Arial Black'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton1Click
  end
  object SpeedButton5: TSpeedButton
    Tag = 5
    Left = 128
    Top = 218
    Width = 123
    Height = 67
    Caption = '5'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 16452374
    Font.Height = -32
    Font.Name = 'Arial Black'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton1Click
  end
  object SpeedButton6: TSpeedButton
    Tag = 6
    Left = 254
    Top = 218
    Width = 123
    Height = 67
    Caption = '6'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 16452374
    Font.Height = -32
    Font.Name = 'Arial Black'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton1Click
  end
  object SpeedButton7: TSpeedButton
    Tag = 7
    Left = 2
    Top = 286
    Width = 123
    Height = 67
    Caption = '7'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 16452374
    Font.Height = -32
    Font.Name = 'Arial Black'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton1Click
  end
  object SpeedButton8: TSpeedButton
    Tag = 8
    Left = 128
    Top = 286
    Width = 123
    Height = 67
    Caption = '8'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 16452374
    Font.Height = -32
    Font.Name = 'Arial Black'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton1Click
  end
  object SpeedButton9: TSpeedButton
    Tag = 9
    Left = 254
    Top = 286
    Width = 123
    Height = 67
    Caption = '9'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 16452374
    Font.Height = -32
    Font.Name = 'Arial Black'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton1Click
  end
  object SpeedButton10: TSpeedButton
    Left = 128
    Top = 354
    Width = 123
    Height = 67
    Caption = '0'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 16452374
    Font.Height = -32
    Font.Name = 'Arial Black'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton1Click
  end
  object SpeedButton_Dot: TSpeedButton
    Left = 2
    Top = 354
    Width = 123
    Height = 67
    Caption = '.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -47
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton_DotClick
  end
  object SpeedButton_Minus: TSpeedButton
    Left = 254
    Top = 354
    Width = 123
    Height = 67
    Caption = '-'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -47
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton_MinusClick
  end
  object SpeedButton_Cancel: TSpeedButton
    Left = -1
    Top = 424
    Width = 186
    Height = 82
    Caption = 'Cancel'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 5592575
    Font.Height = -27
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    OnClick = SpeedButton_CancelClick
  end
  object SpeedButton_OK: TSpeedButton
    Left = 191
    Top = 424
    Width = 186
    Height = 82
    Caption = 'OK'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 2333213
    Font.Height = -27
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    OnClick = SpeedButton_OKClick
  end
  object SpeedButton_Clear: TSpeedButton
    Tag = 1
    Left = 2
    Top = 95
    Width = 186
    Height = 54
    Caption = 'Clear'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -27
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    OnClick = SpeedButton_ClearClick
  end
  object SpeedButton_Back: TSpeedButton
    Tag = 1
    Left = 191
    Top = 95
    Width = 186
    Height = 54
    Caption = 'Backspace'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -27
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    OnClick = SpeedButton_BackClick
  end
  object SpeedButton_Up: TSpeedButton
    Left = 314
    Top = 48
    Width = 63
    Height = 46
    Caption = '+'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clFuchsia
    Font.Height = -60
    Font.Name = 'Let'#39's go Digital'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton_UpClick
  end
  object SpeedButton_Down: TSpeedButton
    Left = 2
    Top = 48
    Width = 63
    Height = 46
    Caption = '-'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clFuchsia
    Font.Height = -60
    Font.Name = 'Let'#39's go Digital'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton_DownClick
  end
  object StaticText1: TStaticText
    Left = 2
    Top = 2
    Width = 123
    Height = 20
    Alignment = taCenter
    AutoSize = False
    BorderStyle = sbsSingle
    Caption = 'Min Limit'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuHighlight
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
    TabOrder = 0
  end
  object StaticText2: TStaticText
    Left = 128
    Top = 2
    Width = 123
    Height = 20
    Alignment = taCenter
    AutoSize = False
    BorderStyle = sbsSingle
    Caption = 'Currnet Value'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuHighlight
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
    TabOrder = 1
  end
  object StaticText3: TStaticText
    Left = 254
    Top = 2
    Width = 123
    Height = 20
    Alignment = taCenter
    AutoSize = False
    BorderStyle = sbsSingle
    Caption = 'Max Limit'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuHighlight
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
    TabOrder = 2
  end
  object Panel_InputNum: TPanel
    Left = 66
    Top = 48
    Width = 248
    Height = 46
    Alignment = taRightJustify
    BevelKind = bkFlat
    BevelOuter = bvNone
    Caption = '0'
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clSilver
    Font.Height = -40
    Font.Name = 'Let'#39's go Digital'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentFont = False
    TabOrder = 3
  end
  object Panel_Min: TPanel
    Left = 2
    Top = 24
    Width = 123
    Height = 24
    BevelOuter = bvNone
    Caption = '0'
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clYellow
    Font.Height = -24
    Font.Name = 'Let'#39's go Digital'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 4
  end
  object Panel_Max: TPanel
    Left = 254
    Top = 24
    Width = 123
    Height = 24
    BevelOuter = bvNone
    Caption = '0'
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clYellow
    Font.Height = -24
    Font.Name = 'Let'#39's go Digital'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 5
  end
  object Panel_OrgValue: TPanel
    Left = 128
    Top = 24
    Width = 123
    Height = 24
    BevelKind = bkFlat
    BevelOuter = bvNone
    Caption = '0'
    Color = clCream
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -24
    Font.Name = 'Let'#39's go Digital'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 6
  end
end
