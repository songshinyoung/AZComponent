//---------------------------------------------------------------------------

#ifndef IndexButtonH
#define IndexButtonH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "Common\\Define.h"


//------------------------------------------------------------------------------
class PACKAGE TMatrixButtonProperty : public TPersistent
{
    typedef TMatrixButtonProperty inherited;

private:
   
    bool                FEnabled;
    int                 FColCount;
    int                 FRowCount;

    TNotifyEvent        FOnChange;                                  ///< ���� ������ �� class���� ���⿡ Event�� ����� ���̴�.
    void    __fastcall  DoOnChange(void);                           ///< �� �Ӽ��� ����� �� ���� �� �Լ��� ȣ���Ͽ� FOnChange�� ��ϵ� Evnet�� ���� ��� ȣ�� ���� �ش�.

protected:
    void    __fastcall  SetEnabled(bool b);
    void    __fastcall  SetColCount(int n);
    void    __fastcall  SetRowCount(int n);

public:
            __fastcall  TMatrixButtonProperty();
  virtual   __fastcall ~TMatrixButtonProperty();

  __property TNotifyEvent OnChange = {read = FOnChange, write = FOnChange};
  void      __fastcall  Assign(TPersistent* Source);

__published:

    // ���� ���� ���� �Ӽ� -----------------------
    __property bool Enabled              = { read = FEnabled,     write = SetEnabled,    default = false      };
    __property int  ColCount             = { read = FColCount,    write = SetColCount,   default = 0          };
    __property int  RowCount             = { read = FRowCount,    write = SetRowCount,   default = 0          };
};
//===========================================================================

class PACKAGE TIndexBtnNumberingProperty : public TPersistent
{
    typedef TIndexBtnNumberingProperty inherited;

private:
    bool                        FEnabled;
    int                         FStartNumber;
    Vcl::Graphics::TFont    *   FFont;          ///< ��Ʈ Ư��

    TNotifyEvent                FOnChange;                                  ///< ���� ������ �� class���� ���⿡ Event�� ����� ���̴�.
    void    __fastcall          DoOnChange(void);                           ///< �� �Ӽ��� ����� �� ���� �� �Լ��� ȣ���Ͽ� FOnChange�� ��ϵ� Evnet�� ���� ��� ȣ�� ���� �ش�.
    void    __fastcall          FontChanged(System::TObject* Sender);       ///< Font�� �Ӽ��� ����� ���� Call Back �ޱ� ���� Event �Լ�.

protected:
    void    __fastcall  SetEnabled(bool b);
    void    __fastcall  SetStartNumber(int n);
    void    __fastcall  SetFont(Vcl::Graphics::TFont * p);

public:
            __fastcall  TIndexBtnNumberingProperty();
  virtual   __fastcall ~TIndexBtnNumberingProperty();

  __property TNotifyEvent OnChange = {read = FOnChange, write = FOnChange};
  void      __fastcall  Assign(TPersistent* Source);

__published:

    // ���� ���� ���� �Ӽ� -----------------------
    __property bool Enabled                 = { read = FEnabled,        write = SetEnabled,     default = false      };
    __property int  StartNumber             = { read = FStartNumber,    write = SetStartNumber, default = 0          };
    __property Vcl::Graphics::TFont * Font  = { read = FFont,           write = SetFont  };
};

//===========================================================================

typedef void __fastcall (__closure *TIndexBtnClickEvent)(System::TObject* Sender, int Index);
typedef void __fastcall (__closure *TMatrixBtnClickEvent)(System::TObject* Sender, int Index, int Row, int Col);

typedef void __fastcall (__closure *TIndexBtnMouseDownEvent)(System::TObject *Sender, TMouseButton Button,
                                                             TShiftState Shift, int Index, int Row, int Col);
typedef void __fastcall (__closure *TIndexBtnMouseLeaveEvent)(TObject *Sender, int Index, int Row, int Col);
typedef void __fastcall (__closure *TIndexBtnMouseUpEvent)(  TObject *Sender, TMouseButton Button,
                                                             TShiftState Shift, int Index, int Row, int Col);


//---------------------------------------------------------------------------
class PACKAGE TIndexButton : public TWinControl
{
private:
    bool                bLoaded;

    System::Classes::TStrings * FList;
    int                 FBtnHeight;             ///< �� SpeedButton�� ����. (FLayout�� ibVertical�� ��� ��ȿ��)
    int                 FBtnWidth;              ///< �� SpeedButton�� ����. (FLayout�� ibHorizontal�� ��� ��ȿ��)
    int                 FMargin;                ///< �̹����� �ܰ��� �Ÿ�
    int                 FSpacing;               ///< �̹����� Caption�� �Ÿ�.
    int                 FGlyphRound;            ///< Glyph�� �𼭸� Round ����.
    int                 FGlyphWidth;            ///< ��� �̹����� ���� (-1�� ��� �ڵ� ����)
    int                 FGlyphHeight;           ///< ��� �̹����� ���� (-1�� ��� �ڵ� ����)
    int                 FIndex;                 ///< ���� ���õ� ��ư Index (0 ���� ����) (-1�� ��� ���õ� ��ư ����)
    
    TColor              FColorUp;               ///< ���õ��� ���� ���� ����. ( Enable)
    TColor              FColorDisable;          ///< Disable ���� ����.
    TColor              FColorDisableSelect;    ///< Select Disable ���� ����.
    TColor              FColorDown;             ///< ����ڰ� Mouse Down ���� ���� ����.
    TColor              FColorSelect;           ///< ���õ� ���� ����.
    TColor              FColorLine;             ///< �ε������ ���� ����.

    bool                FGlyphDefault;          ///< ���ҽ��� ���Ե� default image�� indicator�� ����Ѵ�. 
    Vcl::Graphics::TBitmap*     FGlyph;         ///< Indicator Image

    TIndexButtonLayout  FLayout;                ///< ��ư�� ���̴� ���� (��-��, ��-�Ʒ�)

    bool                FAllowAllUp;            ///< true : ��ư�� �ϳ��� ���õ��� �ʴ� ���� ����. 

    TButtonLayout       FGlyphLayout;

    bool                m_bDarkMode;
    
    //-------------------------------
    // Matrix Button ���� �Ӽ�
    TMatrixButtonProperty * FMatrixButton;      ///< Matrix Button Property
    
    //-------------------------------
    // Indicator Numbering ���� �Ӽ�. 
    TIndexBtnNumberingProperty * FNumbering;

    //-------------------------------

    void __fastcall SetList(System::Classes::TStrings * s);
    void __fastcall SetColor(int Index, TColor c);
    void __fastcall SetBtnHeight(int n);
    void __fastcall SetBtnWidth(int n);
    void __fastcall SetMargin(int n);
    void __fastcall SetSpacing(int n);
    void __fastcall SetGlyphRound(int n);
    void __fastcall SetGlyphWidth(int n);
    void __fastcall SetGlyphHeight(int n);
    void __fastcall SetGlyph(Vcl::Graphics::TBitmap* Value);
    Vcl::Graphics::TBitmap* __fastcall GetGlyph(void);
    void __fastcall SetGlyphDefault(bool b);
    void __fastcall SetGlyphLayout(TButtonLayout e);
    void __fastcall SetLayout(TIndexButtonLayout e) ;
    void __fastcall SetIndex(int n);
    int  __fastcall GetIndex();

    void __fastcall SetButtonEnabled(int Index, bool b);
    bool __fastcall GetButtonEnabled(int Index);

    void __fastcall SetButtonEnabledMatrix(int nRow, int nCol, bool b);
    bool __fastcall GetButtonEnabledMatrix(int nRow, int nCol);

    void __fastcall SetDown(int nIndex, bool b);
    bool __fastcall GetDown(int nIndex);

    void __fastcall SetDownMatrix(int nRow, int nCol, bool b);
    bool __fastcall GetDownMatrix(int nRow, int nCol);

    int  __fastcall GetCount(void);


    void    __fastcall SetCaption(int n, String s);
    String  __fastcall GetCaption(int n);

    void    __fastcall SetCaptionMatrix(int nRow, int nCol, String s);
    String  __fastcall GetCaptionMatrix(int nRow, int nCol);

    void    __fastcall SetAllowAllUp(bool b);
    
    //-------------------------------
    Graphics::TBitmap* BMP_Indicator;
    Graphics::TBitmap* BMP_IndicatorDisable;
    Graphics::TBitmap* BMP_IndicatorNumbering;

    TList * BtnList;

    void    __fastcall DisplayUpdate();
    void    __fastcall CreateIndicatorImage();
    void    __fastcall CreateButton();

    void    __fastcall MyResize(TObject *Sender);
    System::Classes::TNotifyEvent FOnResize;

    TIndexBtnMouseDownEvent     FOnMouseDown;
    TIndexBtnMouseLeaveEvent    FOnMouseLeave;
    TIndexBtnMouseUpEvent       FOnMouseUp;
    TIndexBtnClickEvent         FOnClick;
    TMatrixBtnClickEvent        FOnMatrixClick;
    void     __fastcall MyClick(TObject *Sender);
    void     __fastcall MyMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void     __fastcall MyMouseLeave(TObject *Sender);
    void     __fastcall MyMouseUp(  TObject *Sender, TMouseButton Button,
                                        TShiftState Shift, int X, int Y);
    TSpeedButton *    __fastcall GetSpeedButton(int Index);
    TSpeedButton *    __fastcall GetSpeedButtonMatrix(int nRow, int nCol);

    void __fastcall ListChanged(System::TObject* Sender);

    void __fastcall GlyphChanged(System::TObject* Sender);

    TColor __fastcall MakeBrightColor(TColor c, int Add);

    void __fastcall DrawIndicatorPeice( TCanvas * pCnavas,
                                        int Index,
                                        int nWidth,
                                        int nHeight,
                                        TColor MyColor,
                                        bool bDualColor);

    void __fastcall TIndexButton::DrawIndicatorNumber( Vcl::Graphics::TBitmap *   pBMP_Target,
                                                       Vcl::Graphics::TBitmap *   pBMP_BackGr,
                                                       int         Index);
        
    int     __fastcall FindDownIndex();

    //-------------------------------

    void __fastcall MatrixPropertyChanged(System::TObject* Sender);
    void __fastcall SetMatrixButtonAlign(int nMaxRow, int nMaxCol);
    void __fastcall SetMatrixButton(TMatrixButtonProperty * p);

    //-------------------------------
    void __fastcall NumberingPropertyChanged(System::TObject* Sender);
    void __fastcall SetNumbering(TIndexBtnNumberingProperty * p);

protected:
    virtual void     __fastcall Loaded(void);                ///< Loaded �Լ� �������̵�.
    virtual void     __fastcall SetEnabled(bool Value);      ///< SetEnabled �Լ� �������̵�.

public:
    __fastcall TIndexButton(TComponent* Owner);
    virtual __fastcall ~TIndexButton();

    // Index ���� �ָ� Matrix button �迭�� �ش��ϴ� Row, Col ���� ã���ش�. (Matrix Button Mode������ ��� ����)
    void    __fastcall FindMatrix(__in int nIndex, __out int &nRow, __out int &nCol);

    // Row, Col ���� �Է��ϸ� Index ���� �����Ѵ�. (Matrix Button Mode������ ��� ����) 
    int     __fastcall FindIndex(__in int nRow, __in int nCol);

    // ���� Down�� Button�� Row, Col ���� ã���ش�. (Matrix Button Mode������ ��� ����)
    // ã�� ��� 0�̻��� ���� �����ϰ� ��ã�� ��� -1�� �����Ѵ�. 
    int     __fastcall GetDownButtonMatrix(__out int &nRow, __out int &nCol);

    __property TSpeedButton * SpeedButtons[int Index]   = { read = GetSpeedButton };
    __property bool           ButtonEnabled[int n]      = { read = GetButtonEnabled,    write = SetButtonEnabled }; 
    __property String         Caption[int n]            = { read = GetCaption,          write = SetCaption };
    __property bool           Down[int n]               = { read = GetDown,             write = SetDown };
    
    __property TSpeedButton * SpeedButtonsMatrix[int nRow][int nCol]  = { read = GetSpeedButtonMatrix };
    __property bool           ButtonEnabledMatrix[int nRow][int nCol] = { read = GetButtonEnabledMatrix,    write = SetButtonEnabledMatrix}; 
    __property String         CaptionMatrix[int nRow][int nCol]       = { read = GetCaptionMatrix,          write = SetCaptionMatrix};
    __property bool           DownMatrix[int nRow][int nCol]          = { read = GetDownMatrix,             write = SetDownMatrix };
    
__published:
    //__property TStringList * List = { read = FList, write = FList};
    __property System::Classes::TStrings* Captions = {read=FList, write=SetList};

    //----------------------------
    // EVENT
    __property System::Classes::TNotifyEvent OnResize       = { read = FOnResize,       write = FOnResize};
    __property TIndexBtnClickEvent           OnClick        = { read = FOnClick,        write = FOnClick};
    __property TMatrixBtnClickEvent          OnMatrixClick  = { read = FOnMatrixClick,  write = FOnMatrixClick };
    __property TIndexBtnMouseDownEvent       OnMouseDown    = { read=FOnMouseDown,      write = FOnMouseDown};
    __property TIndexBtnMouseLeaveEvent      OnMouseLeave   = { read=FOnMouseLeave,     write = FOnMouseLeave};
    __property TIndexBtnMouseUpEvent         OnMouseUp      = { read=FOnMouseUp,        write = FOnMouseUp};
    
    //------------------------------
    __property int              BtnHeight       = { read = FBtnHeight,     write = SetBtnHeight,   default = 70 };
    __property int              BtnWidth        = { read = FBtnWidth,      write = SetBtnWidth,    default = 100 };
    __property int              Margin          = { read = FMargin,        write = SetMargin,      default = 0 };
    __property int              Spacing         = { read = FSpacing,       write = SetSpacing,     default = 5 };
    __property int              GlyphRound      = { read = FGlyphRound,    write = SetGlyphRound,  default = 5 };
    __property int              GlyphWidth      = { read = FGlyphWidth,    write = SetGlyphWidth,  default = -1 };
    __property int              GlyphHeight     = { read = FGlyphHeight,   write = SetGlyphHeight, default = -1 };
    __property TButtonLayout    GlyphLayout     = { read = FGlyphLayout,   write = SetGlyphLayout, default = blGlyphLeft};

    __property TColor           ColorUp             = { read = FColorUp,            write = SetColor,       index = 1, default = (TColor)0x09B9B9B };
    __property TColor           ColorDisable        = { read = FColorDisable,       write = SetColor,       index = 2, default = (TColor)0x0CDCDCD };
    __property TColor           ColorDown           = { read = FColorDown,          write = SetColor,       index = 3, default = (TColor)0x04080FF };
    __property TColor           ColorSelect         = { read = FColorSelect,        write = SetColor,       index = 4, default = (TColor)0x0DBDB00 };
    __property TColor           ColorLine           = { read = FColorLine,          write = SetColor,       index = 5, default = clGray };
    __property TColor           ColorDisableSelect  = { read = FColorDisableSelect, write = SetColor,       index = 6, default = (TColor)0x0787878 };


    __property Vcl::Graphics::TBitmap*  Glyph   = { read = GetGlyph,       write = SetGlyph                            };
    __property bool             GlyphDefault    = { read = FGlyphDefault,  write = SetGlyphDefault, default = false };
    __property TIndexButtonLayout Layout        = { read = FLayout,        write = SetLayout,       default = ibVertical };
    __property int              Count           = { read = GetCount   };
    __property int              Index           = { read = GetIndex,       write = SetIndex,        default = -1 };
    //__property bool             Down[int n]           = { read = GetDown,           write = SetDown };  // Down �ʿ� ���� Index�� Down�� �����. 

    __property bool             AllowAllUp      = { read = FAllowAllUp,    write = SetAllowAllUp, default = false };

    __property TMatrixButtonProperty * MatrixButton  = { read = FMatrixButton,  write = SetMatrixButton };

    __property TIndexBtnNumberingProperty * Numbering  = { read = FNumbering,write = SetNumbering };
        
    __property Align;
    __property Font;
    __property Enabled;
    __property Visible;
};
//---------------------------------------------------------------------------
#endif
