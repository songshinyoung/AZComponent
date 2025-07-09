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

    TNotifyEvent        FOnChange;                                  ///< 나를 가져다 쓸 class에서 여기에 Event를 등록할 것이다.
    void    __fastcall  DoOnChange(void);                           ///< 내 속성이 변경될 때 마다 이 함수를 호출하여 FOnChange에 등록된 Evnet가 있을 경우 호출 시켜 준다.

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

    // 내가 새로 만든 속성 -----------------------
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
    Vcl::Graphics::TFont    *   FFont;          ///< 폰트 특성

    TNotifyEvent                FOnChange;                                  ///< 나를 가져다 쓸 class에서 여기에 Event를 등록할 것이다.
    void    __fastcall          DoOnChange(void);                           ///< 내 속성이 변경될 때 마다 이 함수를 호출하여 FOnChange에 등록된 Evnet가 있을 경우 호출 시켜 준다.
    void    __fastcall          FontChanged(System::TObject* Sender);       ///< Font의 속성이 변경된 것을 Call Back 받기 위한 Event 함수.

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

    // 내가 새로 만든 속성 -----------------------
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
    int                 FBtnHeight;             ///< 각 SpeedButton의 높이. (FLayout이 ibVertical일 경우 유효함)
    int                 FBtnWidth;              ///< 각 SpeedButton의 넓이. (FLayout이 ibHorizontal일 경우 유효함)
    int                 FMargin;                ///< 이미지와 외곽의 거리
    int                 FSpacing;               ///< 이미지와 Caption의 거리.
    int                 FGlyphRound;            ///< Glyph의 모서리 Round 정도.
    int                 FGlyphWidth;            ///< 모양 이미지의 넓이 (-1일 경우 자동 설정)
    int                 FGlyphHeight;           ///< 모양 이미지의 높이 (-1일 경우 자동 설정)
    int                 FIndex;                 ///< 현재 선택된 버튼 Index (0 부터 시작) (-1일 경우 선택된 버튼 없음)
    
    TColor              FColorUp;               ///< 선택되지 않은 상태 색상. ( Enable)
    TColor              FColorDisable;          ///< Disable 상태 색상.
    TColor              FColorDisableSelect;    ///< Select Disable 상태 색상.
    TColor              FColorDown;             ///< 사용자가 Mouse Down 중인 상태 색상.
    TColor              FColorSelect;           ///< 선택된 상태 색상.
    TColor              FColorLine;             ///< 인디게이터 라인 색상.

    bool                FGlyphDefault;          ///< 리소스에 포함된 default image를 indicator로 사용한다. 
    Vcl::Graphics::TBitmap*     FGlyph;         ///< Indicator Image

    TIndexButtonLayout  FLayout;                ///< 버튼이 놓이는 방향 (좌-우, 위-아래)

    bool                FAllowAllUp;            ///< true : 버튼이 하나도 선택되지 않는 상태 가능. 

    TButtonLayout       FGlyphLayout;

    bool                m_bDarkMode;
    
    //-------------------------------
    // Matrix Button 관련 속성
    TMatrixButtonProperty * FMatrixButton;      ///< Matrix Button Property
    
    //-------------------------------
    // Indicator Numbering 관련 속성. 
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
    virtual void     __fastcall Loaded(void);                ///< Loaded 함수 오버라이드.
    virtual void     __fastcall SetEnabled(bool Value);      ///< SetEnabled 함수 오버라이드.

public:
    __fastcall TIndexButton(TComponent* Owner);
    virtual __fastcall ~TIndexButton();

    // Index 값을 주면 Matrix button 배열에 해당하는 Row, Col 값을 찾아준다. (Matrix Button Mode에서만 사용 가능)
    void    __fastcall FindMatrix(__in int nIndex, __out int &nRow, __out int &nCol);

    // Row, Col 값을 입력하면 Index 값을 리턴한다. (Matrix Button Mode에서만 사용 가능) 
    int     __fastcall FindIndex(__in int nRow, __in int nCol);

    // 현재 Down된 Button의 Row, Col 값을 찾아준다. (Matrix Button Mode에서만 사용 가능)
    // 찾을 경우 0이상의 값을 리턴하고 못찾을 경우 -1을 리턴한다. 
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
    //__property bool             Down[int n]           = { read = GetDown,           write = SetDown };  // Down 필요 없음 Index가 Down을 대신함. 

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
