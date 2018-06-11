object fmAZNumShapeEditor: TfmAZNumShapeEditor
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'AZ Num Shape Property Editor'
  ClientHeight = 450
  ClientWidth = 487
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel25: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 415
    Width = 481
    Height = 32
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    object Button2: TButton
      Left = 321
      Top = 0
      Width = 80
      Height = 32
      Align = alRight
      Caption = 'Cancel'
      TabOrder = 0
      OnClick = Button2Click
    end
    object Button1: TButton
      Left = 401
      Top = 0
      Width = 80
      Height = 32
      Align = alRight
      Caption = 'OK'
      TabOrder = 1
      OnClick = Button1Click
    end
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 487
    Height = 412
    ActivePage = TabSheet1
    Align = alClient
    TabHeight = 25
    TabOrder = 1
    TabWidth = 100
    object TabSheet1: TTabSheet
      Caption = 'Edit Box'
      object Panel1: TPanel
        AlignWithMargins = True
        Left = 3
        Top = 1
        Width = 473
        Height = 213
        Margins.Top = 1
        Align = alTop
        BevelKind = bkFlat
        BevelOuter = bvNone
        TabOrder = 0
        object ColorListBox1: TColorListBox
          AlignWithMargins = True
          Left = 330
          Top = 26
          Width = 136
          Height = 180
          Align = alRight
          TabOrder = 0
          OnClick = ColorListBox1Click
          OnDblClick = ColorListBox1DblClick
        end
        object Panel9: TPanel
          AlignWithMargins = True
          Left = 3
          Top = 26
          Width = 321
          Height = 180
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 1
          object Panel2: TPanel
            Left = 0
            Top = 0
            Width = 321
            Height = 30
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 0
            object Panel7: TPanel
              Left = 0
              Top = 0
              Width = 80
              Height = 30
              Align = alLeft
              BevelKind = bkFlat
              BevelOuter = bvNone
              Caption = 'Part'
              Color = clGray
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -13
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentBackground = False
              ParentFont = False
              TabOrder = 0
            end
            object Panel8: TPanel
              Left = 80
              Top = 0
              Width = 80
              Height = 30
              Align = alLeft
              BevelKind = bkFlat
              BevelOuter = bvNone
              Caption = 'Normal'
              Color = clHotLight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -13
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentBackground = False
              ParentFont = False
              TabOrder = 1
            end
            object Panel10: TPanel
              Left = 160
              Top = 0
              Width = 80
              Height = 30
              Align = alLeft
              BevelKind = bkFlat
              BevelOuter = bvNone
              Caption = 'Selected'
              Color = clHotLight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -13
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentBackground = False
              ParentFont = False
              TabOrder = 2
            end
            object Panel15: TPanel
              Left = 240
              Top = 0
              Width = 80
              Height = 30
              Align = alLeft
              BevelKind = bkFlat
              BevelOuter = bvNone
              Caption = 'Changed'
              Color = clHotLight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -13
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentBackground = False
              ParentFont = False
              TabOrder = 3
            end
          end
          object Panel3: TPanel
            Left = 0
            Top = 30
            Width = 321
            Height = 30
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 1
            object Panel6: TPanel
              Left = 0
              Top = 0
              Width = 80
              Height = 30
              Align = alLeft
              BevelKind = bkFlat
              BevelOuter = bvNone
              Caption = 'Value'
              Color = clMedGray
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -13
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentBackground = False
              ParentFont = False
              TabOrder = 0
            end
            object Panel_ColorValueNormal: TPanel
              Left = 80
              Top = 0
              Width = 80
              Height = 30
              Align = alLeft
              BevelWidth = 2
              BiDiMode = bdLeftToRight
              Caption = 'Normal'
              ParentBiDiMode = False
              ParentBackground = False
              TabOrder = 1
              OnClick = Panel_ColorValueNormalClick
            end
            object Panel_ColorValueSelect: TPanel
              Tag = 2
              Left = 160
              Top = 0
              Width = 80
              Height = 30
              Align = alLeft
              BevelWidth = 2
              BiDiMode = bdLeftToRight
              Caption = 'Selected'
              ParentBiDiMode = False
              ParentBackground = False
              TabOrder = 2
              OnClick = Panel_ColorValueNormalClick
            end
            object Panel_ColorValueChanged: TPanel
              Tag = 2
              Left = 240
              Top = 0
              Width = 80
              Height = 30
              Align = alLeft
              BevelWidth = 2
              BiDiMode = bdLeftToRight
              Caption = 'Changed'
              ParentBiDiMode = False
              ParentBackground = False
              TabOrder = 3
              OnClick = Panel_ColorValueNormalClick
            end
          end
          object Panel16: TPanel
            Left = 0
            Top = 60
            Width = 321
            Height = 30
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 2
            object Panel17: TPanel
              Left = 0
              Top = 0
              Width = 80
              Height = 30
              Align = alLeft
              BevelKind = bkFlat
              BevelOuter = bvNone
              Caption = 'Back G.'
              Color = clMedGray
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -13
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentBackground = False
              ParentFont = False
              TabOrder = 0
            end
            object Panel_ColorBGSelect: TPanel
              Tag = 22
              Left = 160
              Top = 0
              Width = 80
              Height = 30
              Align = alLeft
              BevelWidth = 2
              BiDiMode = bdLeftToRight
              Caption = 'Selected'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentBiDiMode = False
              ParentBackground = False
              ParentFont = False
              TabOrder = 1
              OnClick = Panel_ColorValueNormalClick
            end
            object Panel_ColorBGNormal: TPanel
              Tag = 20
              Left = 80
              Top = 0
              Width = 80
              Height = 30
              Align = alLeft
              BevelWidth = 2
              BiDiMode = bdLeftToRight
              Caption = 'Normal'
              ParentBiDiMode = False
              ParentBackground = False
              TabOrder = 2
              OnClick = Panel_ColorValueNormalClick
            end
          end
          object Panel4: TPanel
            Left = 0
            Top = 90
            Width = 321
            Height = 30
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 3
            object Panel11: TPanel
              Left = 0
              Top = 0
              Width = 80
              Height = 30
              Align = alLeft
              BevelKind = bkFlat
              BevelOuter = bvNone
              Caption = 'Old Value'
              Color = clMedGray
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -13
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentBackground = False
              ParentFont = False
              TabOrder = 0
            end
            object Panel_ColorOldValueSelect: TPanel
              Tag = 12
              Left = 160
              Top = 0
              Width = 80
              Height = 30
              Align = alLeft
              BevelWidth = 2
              BiDiMode = bdLeftToRight
              Caption = 'Selected'
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clGray
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentBiDiMode = False
              ParentBackground = False
              ParentFont = False
              TabOrder = 1
              OnClick = Panel_ColorValueNormalClick
            end
            object Panel_ColorOldValueNormal: TPanel
              Tag = 10
              Left = 80
              Top = 0
              Width = 80
              Height = 30
              Align = alLeft
              BevelWidth = 2
              BiDiMode = bdLeftToRight
              Caption = 'Normal'
              ParentBiDiMode = False
              ParentBackground = False
              TabOrder = 2
              OnClick = Panel_ColorValueNormalClick
            end
          end
          object Panel5: TPanel
            Left = 0
            Top = 120
            Width = 321
            Height = 30
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 4
            object Panel12: TPanel
              Left = 0
              Top = 0
              Width = 80
              Height = 30
              Align = alLeft
              BevelKind = bkFlat
              BevelOuter = bvNone
              Caption = 'Unit'
              Color = clMedGray
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -13
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentBackground = False
              ParentFont = False
              TabOrder = 0
            end
            object Panel_ColorUnitSelect: TPanel
              Tag = 22
              Left = 160
              Top = 0
              Width = 80
              Height = 30
              Align = alLeft
              BevelWidth = 2
              BiDiMode = bdLeftToRight
              Caption = 'Selected'
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clGray
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentBiDiMode = False
              ParentBackground = False
              ParentFont = False
              TabOrder = 1
              OnClick = Panel_ColorValueNormalClick
            end
            object Panel_ColorUnitNormal: TPanel
              Tag = 20
              Left = 80
              Top = 0
              Width = 80
              Height = 30
              Align = alLeft
              BevelWidth = 2
              BiDiMode = bdLeftToRight
              Caption = 'Normal'
              ParentBiDiMode = False
              ParentBackground = False
              TabOrder = 2
              OnClick = Panel_ColorValueNormalClick
            end
          end
          object Panel13: TPanel
            Left = 0
            Top = 150
            Width = 321
            Height = 30
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 5
            object Panel14: TPanel
              Left = 0
              Top = 0
              Width = 80
              Height = 30
              Align = alLeft
              BevelKind = bkFlat
              BevelOuter = bvNone
              Caption = 'Out Line'
              Color = clMedGray
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -13
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentBackground = False
              ParentFont = False
              TabOrder = 0
            end
            object Panel_ColorLineSelect: TPanel
              Tag = 32
              Left = 160
              Top = 0
              Width = 80
              Height = 30
              Align = alLeft
              BevelWidth = 2
              BiDiMode = bdLeftToRight
              Caption = 'Selected'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlack
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentBiDiMode = False
              ParentBackground = False
              ParentFont = False
              TabOrder = 1
              OnClick = Panel_ColorValueNormalClick
            end
            object Panel_ColorLineNormal: TPanel
              Tag = 30
              Left = 80
              Top = 0
              Width = 80
              Height = 30
              Align = alLeft
              BevelWidth = 2
              BiDiMode = bdLeftToRight
              Caption = 'Normal'
              ParentBiDiMode = False
              ParentBackground = False
              TabOrder = 2
              OnClick = Panel_ColorValueNormalClick
            end
          end
        end
        object Panel18: TPanel
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 463
          Height = 20
          Margins.Bottom = 0
          Align = alTop
          Alignment = taLeftJustify
          BevelOuter = bvNone
          Caption = '  Color Setting'
          Color = clGray
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentBackground = False
          ParentFont = False
          TabOrder = 2
        end
      end
      object Panel19: TPanel
        AlignWithMargins = True
        Left = 307
        Top = 220
        Width = 169
        Height = 154
        Align = alClient
        BevelKind = bkFlat
        BevelOuter = bvNone
        TabOrder = 1
        object Label1: TLabel
          Left = 16
          Top = 31
          Width = 56
          Height = 13
          Caption = 'Max. Value '
        end
        object Label2: TLabel
          Left = 16
          Top = 58
          Width = 52
          Height = 13
          Caption = 'Min. Value '
        end
        object Label3: TLabel
          Left = 16
          Top = 85
          Width = 73
          Height = 13
          Caption = 'Up/Down Value'
        end
        object Panel20: TPanel
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 159
          Height = 20
          Margins.Bottom = 0
          Align = alTop
          Alignment = taLeftJustify
          BevelOuter = bvNone
          Caption = '  Key Pad Setting'
          Color = clGray
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentBackground = False
          ParentFont = False
          TabOrder = 0
        end
        object Edit_MaxValue: TEdit
          Left = 103
          Top = 31
          Width = 60
          Height = 21
          ImeName = 'Microsoft IME 2010'
          NumbersOnly = True
          TabOrder = 1
          Text = '1000'
        end
        object Edit_MinValue: TEdit
          Left = 103
          Top = 58
          Width = 60
          Height = 21
          ImeName = 'Microsoft IME 2010'
          NumbersOnly = True
          TabOrder = 2
          Text = '-1000'
        end
        object Edit_UpDownValue: TEdit
          Left = 103
          Top = 85
          Width = 60
          Height = 21
          ImeName = 'Microsoft IME 2010'
          NumbersOnly = True
          TabOrder = 3
          Text = '1'
        end
        object CheckBox_ReadOnly: TCheckBox
          Left = 16
          Top = 112
          Width = 97
          Height = 17
          Caption = 'Read Only'
          TabOrder = 4
        end
      end
      object Panel21: TPanel
        AlignWithMargins = True
        Left = 3
        Top = 220
        Width = 144
        Height = 154
        Align = alLeft
        BevelKind = bkFlat
        BevelOuter = bvNone
        TabOrder = 2
        object Label4: TLabel
          Left = 8
          Top = 81
          Width = 69
          Height = 13
          Caption = 'Decimal Places'
        end
        object Label5: TLabel
          Left = 8
          Top = 129
          Width = 77
          Height = 13
          Caption = 'Out Line Round '
        end
        object Label6: TLabel
          Left = 8
          Top = 105
          Width = 29
          Height = 13
          Caption = 'Space'
        end
        object Panel22: TPanel
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 134
          Height = 20
          Margins.Bottom = 0
          Align = alTop
          Alignment = taLeftJustify
          BevelOuter = bvNone
          Caption = '  Type Setting'
          Color = clGray
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentBackground = False
          ParentFont = False
          TabOrder = 0
        end
        object RadioGroup_Valuetype: TRadioGroup
          AlignWithMargins = True
          Left = 5
          Top = 26
          Width = 130
          Height = 49
          Margins.Left = 5
          Margins.Right = 5
          Align = alTop
          Caption = 'Value Type'
          Columns = 2
          ItemIndex = 1
          Items.Strings = (
            'Int'
            'Float')
          TabOrder = 1
        end
        object Edit_DecimalPlaces: TEdit
          Left = 95
          Top = 81
          Width = 26
          Height = 21
          ImeName = 'Microsoft IME 2010'
          MaxLength = 1
          NumbersOnly = True
          TabOrder = 2
          Text = '3'
        end
        object Edit_OutlineRound: TEdit
          Left = 95
          Top = 129
          Width = 38
          Height = 21
          ImeName = 'Microsoft IME 2010'
          MaxLength = 2
          NumbersOnly = True
          TabOrder = 3
          Text = '5'
        end
        object Edit_Space: TEdit
          Left = 95
          Top = 105
          Width = 38
          Height = 21
          ImeName = 'Microsoft IME 2010'
          MaxLength = 3
          NumbersOnly = True
          TabOrder = 4
          Text = '3'
        end
      end
      object Panel23: TPanel
        AlignWithMargins = True
        Left = 153
        Top = 220
        Width = 148
        Height = 154
        Align = alLeft
        BevelKind = bkFlat
        BevelOuter = bvNone
        TabOrder = 3
        object Label_UnitWidth: TLabel
          Left = 8
          Top = 60
          Width = 50
          Height = 13
          Caption = 'Unit Width'
        end
        object Label7: TLabel
          Left = 8
          Top = 79
          Width = 49
          Height = 13
          Caption = 'Unit Name'
        end
        object Panel24: TPanel
          AlignWithMargins = True
          Left = 3
          Top = 106
          Width = 138
          Height = 20
          Margins.Bottom = 0
          Alignment = taLeftJustify
          BevelOuter = bvNone
          Caption = '  Old Value Setting'
          Color = clGray
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentBackground = False
          ParentFont = False
          TabOrder = 0
        end
        object Edit_UnitWidth: TEdit
          Left = 64
          Top = 55
          Width = 57
          Height = 21
          ImeName = 'Microsoft IME 2010'
          MaxLength = 3
          NumbersOnly = True
          TabOrder = 1
          Text = '3'
        end
        object CheckBox_UnitVisible: TCheckBox
          Left = 8
          Top = 32
          Width = 97
          Height = 17
          Caption = 'Unit Visible'
          TabOrder = 2
        end
        object Edit_Unit: TEdit
          Left = 63
          Top = 79
          Width = 58
          Height = 21
          ImeName = 'Microsoft IME 2010'
          TabOrder = 3
          Text = 'unit'
        end
        object Panel26: TPanel
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 138
          Height = 20
          Margins.Bottom = 0
          Align = alTop
          Alignment = taLeftJustify
          BevelOuter = bvNone
          Caption = '  Unit Setting'
          Color = clGray
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentBackground = False
          ParentFont = False
          TabOrder = 4
        end
        object CheckBox_OldValueVisible: TCheckBox
          Left = 8
          Top = 131
          Width = 97
          Height = 17
          Caption = 'Old Value Visible'
          TabOrder = 5
        end
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Title Box'
      ImageIndex = 1
      object Panel27: TPanel
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 97
        Height = 272
        Margins.Right = 0
        Align = alLeft
        BevelOuter = bvNone
        Color = 7368816
        ParentBackground = False
        TabOrder = 0
        ExplicitHeight = 271
        object Label8: TLabel
          AlignWithMargins = True
          Left = 1
          Top = 1
          Width = 95
          Height = 26
          Margins.Left = 1
          Margins.Top = 1
          Margins.Right = 1
          Margins.Bottom = 0
          Align = alTop
          Alignment = taCenter
          AutoSize = False
          Caption = 'Visible'
          Color = clSilver
          ParentColor = False
          Transparent = False
          Layout = tlCenter
        end
        object Label9: TLabel
          AlignWithMargins = True
          Left = 1
          Top = 28
          Width = 95
          Height = 26
          Margins.Left = 1
          Margins.Top = 1
          Margins.Right = 1
          Margins.Bottom = 0
          Align = alTop
          Alignment = taCenter
          AutoSize = False
          Caption = 'Caption'
          Color = clSilver
          ParentColor = False
          Transparent = False
          Layout = tlCenter
          ExplicitTop = 27
        end
        object Label10: TLabel
          AlignWithMargins = True
          Left = 1
          Top = 55
          Width = 95
          Height = 26
          Margins.Left = 1
          Margins.Top = 1
          Margins.Right = 1
          Margins.Bottom = 0
          Align = alTop
          Alignment = taCenter
          AutoSize = False
          Caption = 'Round'
          Color = clSilver
          ParentColor = False
          Transparent = False
          Layout = tlCenter
          ExplicitTop = 53
        end
        object Label11: TLabel
          AlignWithMargins = True
          Left = 1
          Top = 82
          Width = 95
          Height = 26
          Margins.Left = 1
          Margins.Top = 1
          Margins.Right = 1
          Margins.Bottom = 0
          Align = alTop
          Alignment = taCenter
          AutoSize = False
          Caption = 'Space'
          Color = clSilver
          ParentColor = False
          Transparent = False
          Layout = tlCenter
          ExplicitTop = 79
        end
        object Label12: TLabel
          AlignWithMargins = True
          Left = 1
          Top = 109
          Width = 95
          Height = 26
          Margins.Left = 1
          Margins.Top = 1
          Margins.Right = 1
          Margins.Bottom = 0
          Align = alTop
          Alignment = taCenter
          AutoSize = False
          Caption = 'Transparent'
          Color = clSilver
          ParentColor = False
          Transparent = False
          Layout = tlCenter
          ExplicitTop = 105
        end
        object Label13: TLabel
          AlignWithMargins = True
          Left = 1
          Top = 136
          Width = 95
          Height = 26
          Margins.Left = 1
          Margins.Top = 1
          Margins.Right = 1
          Margins.Bottom = 0
          Align = alTop
          Alignment = taCenter
          AutoSize = False
          Caption = 'Show Out Line'
          Color = clSilver
          ParentColor = False
          Transparent = False
          Layout = tlCenter
          ExplicitTop = 131
        end
        object Label14: TLabel
          AlignWithMargins = True
          Left = 1
          Top = 163
          Width = 95
          Height = 26
          Margins.Left = 1
          Margins.Top = 1
          Margins.Right = 1
          Margins.Bottom = 0
          Align = alTop
          Alignment = taCenter
          AutoSize = False
          Caption = 'Box Align'
          Color = clSilver
          ParentColor = False
          Transparent = False
          Layout = tlCenter
          ExplicitTop = 157
        end
        object Label15: TLabel
          AlignWithMargins = True
          Left = 1
          Top = 190
          Width = 95
          Height = 26
          Margins.Left = 1
          Margins.Top = 1
          Margins.Right = 1
          Margins.Bottom = 0
          Align = alTop
          Alignment = taCenter
          AutoSize = False
          Caption = 'Caption Alignment'
          Color = clSilver
          ParentColor = False
          Transparent = False
          Layout = tlCenter
          ExplicitTop = 183
        end
        object Label16: TLabel
          AlignWithMargins = True
          Left = 1
          Top = 217
          Width = 95
          Height = 26
          Margins.Left = 1
          Margins.Top = 1
          Margins.Right = 1
          Margins.Bottom = 0
          Align = alTop
          Alignment = taCenter
          AutoSize = False
          Caption = 'Color'
          Color = clSilver
          ParentColor = False
          Transparent = False
          Layout = tlCenter
          ExplicitTop = 209
        end
        object Label17: TLabel
          AlignWithMargins = True
          Left = 1
          Top = 244
          Width = 95
          Height = 26
          Margins.Left = 1
          Margins.Top = 1
          Margins.Right = 1
          Margins.Bottom = 0
          Align = alTop
          Alignment = taCenter
          AutoSize = False
          Caption = 'Out Line Color'
          Color = clSilver
          ParentColor = False
          Transparent = False
          Layout = tlCenter
          ExplicitTop = 235
        end
      end
      object Panel28: TPanel
        AlignWithMargins = True
        Left = 100
        Top = 3
        Width = 137
        Height = 272
        Margins.Left = 0
        Align = alLeft
        BevelKind = bkFlat
        BevelOuter = bvNone
        ParentBackground = False
        TabOrder = 1
        object Bevel2: TBevel
          AlignWithMargins = True
          Left = 0
          Top = 273
          Width = 133
          Height = 6
          Margins.Left = 0
          Margins.Right = 0
          Align = alTop
          Shape = bsTopLine
          ExplicitTop = 278
          ExplicitWidth = 137
        end
        object CheckBox_Title_Visible: TCheckBox
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 127
          Height = 21
          Align = alTop
          TabOrder = 0
          ExplicitWidth = 131
        end
        object Edit_Title_Caption: TEdit
          AlignWithMargins = True
          Left = 3
          Top = 30
          Width = 127
          Height = 21
          Align = alTop
          ImeName = 'Microsoft IME 2010'
          TabOrder = 1
          ExplicitWidth = 131
        end
        object Edit_Title_Round: TEdit
          AlignWithMargins = True
          Left = 3
          Top = 57
          Width = 127
          Height = 21
          Align = alTop
          ImeName = 'Microsoft IME 2010'
          NumbersOnly = True
          TabOrder = 2
          ExplicitWidth = 131
        end
        object Edit_Title_Space: TEdit
          AlignWithMargins = True
          Left = 3
          Top = 84
          Width = 127
          Height = 21
          Align = alTop
          ImeName = 'Microsoft IME 2010'
          NumbersOnly = True
          TabOrder = 3
          ExplicitWidth = 131
        end
        object CheckBox_Title_Transparent: TCheckBox
          AlignWithMargins = True
          Left = 3
          Top = 111
          Width = 127
          Height = 21
          Align = alTop
          TabOrder = 4
          ExplicitWidth = 131
        end
        object CheckBox_Title_OutLineVisible: TCheckBox
          AlignWithMargins = True
          Left = 3
          Top = 138
          Width = 127
          Height = 21
          Align = alTop
          TabOrder = 5
          ExplicitWidth = 131
        end
        object ComboBox_Title_Align: TComboBox
          AlignWithMargins = True
          Left = 3
          Top = 165
          Width = 127
          Height = 21
          Align = alTop
          ImeName = 'Microsoft IME 2010'
          TabOrder = 6
          Text = 'Left'
          Items.Strings = (
            'Left'
            'Right'
            'Top'
            'Bottom')
          ExplicitWidth = 131
        end
        object ComboBox_Title_Alignment: TComboBox
          AlignWithMargins = True
          Left = 3
          Top = 192
          Width = 127
          Height = 21
          Align = alTop
          ImeName = 'Microsoft IME 2010'
          TabOrder = 7
          Text = 'Left'
          Items.Strings = (
            'Left'
            'Right'
            'Center')
          ExplicitWidth = 131
        end
        object Panel_Title_Color: TPanel
          AlignWithMargins = True
          Left = 3
          Top = 218
          Width = 127
          Height = 23
          Margins.Top = 2
          Margins.Bottom = 2
          Align = alTop
          BevelWidth = 2
          BiDiMode = bdLeftToRight
          ParentBiDiMode = False
          ParentBackground = False
          TabOrder = 8
          OnClick = Panel_Title_ColorClick
          ExplicitWidth = 131
        end
        object Panel_Title_OutLineColor: TPanel
          AlignWithMargins = True
          Left = 3
          Top = 245
          Width = 127
          Height = 23
          Margins.Top = 2
          Margins.Bottom = 2
          Align = alTop
          BevelWidth = 2
          BiDiMode = bdLeftToRight
          ParentBiDiMode = False
          ParentBackground = False
          TabOrder = 9
          OnClick = Panel_Title_ColorClick
          ExplicitWidth = 131
        end
      end
      object ColorListBox2: TColorListBox
        AlignWithMargins = True
        Left = 243
        Top = 3
        Width = 233
        Height = 270
        Margins.Bottom = 5
        Align = alClient
        TabOrder = 2
        OnClick = ColorListBox2Click
        OnDblClick = ColorListBox2DblClick
        ExplicitHeight = 269
      end
      object Panel29: TPanel
        AlignWithMargins = True
        Left = 3
        Top = 278
        Width = 473
        Height = 99
        Margins.Top = 0
        Margins.Bottom = 0
        Align = alBottom
        BevelOuter = bvNone
        BiDiMode = bdLeftToRight
        ParentBiDiMode = False
        ParentBackground = False
        TabOrder = 3
        object Panel30: TPanel
          AlignWithMargins = True
          Left = 0
          Top = 3
          Width = 97
          Height = 93
          Margins.Left = 0
          Margins.Right = 0
          Align = alLeft
          BevelOuter = bvNone
          Color = 7368816
          ParentBackground = False
          TabOrder = 0
          ExplicitLeft = 4
          ExplicitTop = 4
          ExplicitHeight = 92
          object Label18: TLabel
            AlignWithMargins = True
            Left = 1
            Top = 1
            Width = 95
            Height = 26
            Margins.Left = 1
            Margins.Top = 1
            Margins.Right = 1
            Margins.Bottom = 0
            Align = alTop
            Alignment = taCenter
            AutoSize = False
            Caption = 'Edit Box Width'
            Color = clSilver
            ParentColor = False
            Transparent = False
            Layout = tlCenter
          end
          object Label19: TLabel
            AlignWithMargins = True
            Left = 1
            Top = 28
            Width = 95
            Height = 26
            Margins.Left = 1
            Margins.Top = 1
            Margins.Right = 1
            Margins.Bottom = 0
            Align = alTop
            Alignment = taCenter
            AutoSize = False
            Caption = 'Edit Box Height'
            Color = clSilver
            ParentColor = False
            Transparent = False
            Layout = tlCenter
          end
          object Label20: TLabel
            AlignWithMargins = True
            Left = 1
            Top = 55
            Width = 95
            Height = 37
            Margins.Left = 1
            Margins.Top = 1
            Margins.Right = 1
            Margins.Bottom = 1
            Align = alClient
            Alignment = taCenter
            AutoSize = False
            Color = clSilver
            ParentColor = False
            Transparent = False
            Layout = tlCenter
            ExplicitTop = 75
            ExplicitHeight = 26
          end
        end
        object Panel31: TPanel
          AlignWithMargins = True
          Left = 97
          Top = 3
          Width = 137
          Height = 93
          Margins.Left = 0
          Align = alLeft
          BevelKind = bkFlat
          BevelOuter = bvNone
          ParentBackground = False
          TabOrder = 1
          object Edit_EditWidth: TEdit
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 127
            Height = 21
            Align = alTop
            ImeName = 'Microsoft IME 2010'
            NumbersOnly = True
            TabOrder = 0
            ExplicitTop = 1
            ExplicitWidth = 131
          end
          object Edit_EditHeight: TEdit
            AlignWithMargins = True
            Left = 3
            Top = 30
            Width = 127
            Height = 21
            Align = alTop
            ImeName = 'Microsoft IME 2010'
            NumbersOnly = True
            TabOrder = 1
            ExplicitTop = 28
            ExplicitWidth = 131
          end
        end
      end
    end
  end
  object ColorDialog1: TColorDialog
    Left = 384
    Top = 72
  end
end
