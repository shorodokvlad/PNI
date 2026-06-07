object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Model Subiect'
  ClientHeight = 670
  ClientWidth = 1155
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  TextHeight = 15
  object Image1: TImage
    Left = 188
    Top = 34
    Width = 341
    Height = 296
    OnMouseDown = Image1MouseDown
  end
  object Image2: TImage
    Left = 535
    Top = 34
    Width = 341
    Height = 296
  end
  object Label4: TLabel
    Left = 912
    Top = 30
    Width = 21
    Height = 15
    Caption = 'Info'
  end
  object Image3: TImage
    Left = 535
    Top = 336
    Width = 341
    Height = 296
  end
  object Chart1: TChart
    Left = 8
    Top = 349
    Width = 521
    Height = 283
    Legend.Visible = False
    Title.Text.Strings = (
      'Histograma')
    View3D = False
    TabOrder = 0
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
    object Series1: TLineSeries
      HoverElement = [heCurrent]
      Title = 'Red'
      Brush.BackColor = clDefault
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series2: TLineSeries
      HoverElement = [heCurrent]
      Title = 'Green'
      Brush.BackColor = clDefault
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series3: TLineSeries
      HoverElement = [heCurrent]
      Title = 'Blue'
      Brush.BackColor = clDefault
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
  end
  object Button1: TButton
    Left = 8
    Top = 71
    Width = 169
    Height = 25
    Caption = 'Load'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 8
    Top = 218
    Width = 169
    Height = 25
    Caption = 'Mean Filtering'
    TabOrder = 2
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 8
    Top = 249
    Width = 169
    Height = 25
    Caption = 'Edge Detection'
    TabOrder = 3
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 13
    Top = 125
    Width = 169
    Height = 25
    Caption = 'Selected Area'
    TabOrder = 4
    OnClick = Button4Click
  end
  object CheckBox1: TCheckBox
    Left = 25
    Top = 102
    Width = 150
    Height = 17
    Caption = 'Muchii zona selectata'
    TabOrder = 5
  end
  object Button6: TButton
    Left = 8
    Top = 40
    Width = 167
    Height = 25
    Caption = 'Save'
    TabOrder = 6
    OnClick = Button6Click
  end
  object Button5: TButton
    Left = 8
    Top = 156
    Width = 167
    Height = 25
    Caption = 'Print Info'
    TabOrder = 7
    OnClick = Button5Click
  end
  object Button7: TButton
    Left = 8
    Top = 187
    Width = 167
    Height = 25
    Caption = 'Cumulative histogram'
    TabOrder = 8
    OnClick = Button7Click
  end
  object Button8: TButton
    Left = 8
    Top = 280
    Width = 167
    Height = 25
    Caption = 'Posterization'
    TabOrder = 9
    OnClick = Button8Click
  end
  object CheckBox2: TCheckBox
    Left = 13
    Top = 311
    Width = 97
    Height = 17
    Caption = 'Negativare'
    TabOrder = 10
  end
  object Edit1: TEdit
    Left = 134
    Top = 308
    Width = 41
    Height = 23
    TabOrder = 11
    Text = '32'
  end
  object OpenDialog1: TOpenDialog
  end
  object SaveDialog1: TSaveDialog
    Left = 32
  end
end
