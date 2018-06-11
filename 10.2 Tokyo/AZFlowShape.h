//---------------------------------------------------------------------------

#ifndef AZFlowShapeH
#define AZFlowShapeH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Vcl.Controls.hpp>
//============================================================================
// FlowShape의 각 Cell에 필요한 값들을 임시 저장하는 Property이다.
// 이 값은 dfm 파일에 저장되지 않고 실행 시간에 설정해야한다.

class PACKAGE TFlowShapeCell : public TPersistent
{
private:
    int     FTag;
    String  FCaption;
    TColor  FBGColor;
    TColor  FLineColor;
    TColor  FFontColor;
    bool    FSpecialFontColor;  // false일 경우 기본 Font의 Font Color를 사용한다. true이면 FFontColor를 사용한다. 

    void __fastcall SetTag(int n);
    void __fastcall SetCaption(String s);
    void __fastcall SetBGColor(TColor c);
    void __fastcall SetLineColor(TColor c);
    void __fastcall SetFontColor(TColor c);
    void __fastcall SetSpecialFontColor(bool b);

    TNotifyEvent     FOnChange;              ///< 나를 가져다 쓸 class에서 여기에 Event를 등록할 것이다.
    void     __fastcall DoOnChange(void);      ///< 내 속성이 변경될 때 마다 이 함수를 호출하여 FOnChange에 등록된 Evnet가 있을 경우 호출 시켜 준다.

public:
    // 생성자. /////////////////////////
    //__fastcall TFlowShapeCell(int r, int c);
    __fastcall TFlowShapeCell();

    //---------------
    int     Row;
    int     Col;
    bool    bChanged;

public:
    __property int     Tag              = { read =  FTag,               write = SetTag };
    __property String  Caption          = { read =  FCaption,           write = SetCaption };
    __property TColor  BGColor          = { read =  FBGColor,           write = SetBGColor };
    __property TColor  LineColor        = { read =  FLineColor,         write = SetLineColor };
    __property TColor  FontColor        = { read =  FFontColor,         write = SetFontColor };
    __property bool    SpecialFontColor = { read =  FSpecialFontColor,  write = SetSpecialFontColor };

    __property TNotifyEvent OnChange = {read = FOnChange, write = FOnChange};

};

//============================================================================

//============================================================================

//Title property를 위한 class

class PACKAGE TAZTitleProperty : public TPersistent
{
private:
    bool                        FVisible;
    bool                        FTransparent;
    String                      FCaption;
    TAlignment                  FAlignment;
    Vcl::Graphics::TFont    *   FFont;
    int                         FHeight;
    int                         FSpace;         ///< Title 과 Cell 사이의 높이 간격.
    TColor                      FColor;
    TColor                      FLineColor;

    void    __fastcall SetVisible(bool b);
    void    __fastcall SetCaption(String s);
    void    __fastcall SetAlignment(TAlignment e);
    void    __fastcall SetFont(Vcl::Graphics::TFont * p);
    void    __fastcall SetHeight(int n);
    void    __fastcall SetSpace(int n);
    void    __fastcall SetColor(int Index, TColor c);
    void    __fastcall SetTransparent(bool b);

    TNotifyEvent     FOnChange;              ///< 나를 가져다 쓸 class에서 여기에 Event를 등록할 것이다.
    void     __fastcall DoOnChange(void);      ///< 내 속성이 변경될 때 마다 이 함수를 호출하여 FOnChange에 등록된 Evnet가 있을 경우 호출 시켜 준다.
    void     __fastcall FontChanged(System::TObject* Sender);  ///< Font의 속성이 변경된 것을 Call Back 받기 위한 Event 함수.

public:
            __fastcall  TAZTitleProperty();
  virtual   __fastcall ~TAZTitleProperty();

  __property TNotifyEvent OnChange = {read = FOnChange, write = FOnChange};
  void      __fastcall Assign(TPersistent* Source);

__published:

    // 내가 새로 만든 속성 -----------------------
    __property bool                     Visible        = { read = FVisible,        write = SetVisible,     default = true         };
    __property String                   Caption      = { read = FCaption,        write = SetCaption                          };
    __property TAlignment               Alignment   = { read = FAlignment,      write = SetAlignment,   default = taCenter  };
    __property Vcl::Graphics::TFont *   Font         = { read = FFont,            write = SetFont,                             };
    __property int                      Height      = { read = FHeight,         write = SetHeight,      default = 20        };
    __property int                      Space       = { read = FSpace,          write = SetSpace,       default = 1         };
    __property TColor                   Color       = { read = FColor,          write = SetColor,       index = 1, default = clWhite };
    __property TColor                   LineColor   = { read = FLineColor,      write = SetColor,       index = 2, default = clBlack };
    __property bool                     Transparent = { read = FTransparent,    write = SetTransparent, default = false     };
};
//------------------------------------------------


//============================================================================

//============================================================================
typedef void __fastcall (__closure *TFlowShapeClickEvent)(System::TObject* Sender, int Row, int Col);
typedef void __fastcall (__closure *TFlowShapeMouseUpEvent)(System::TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int Row, int Col);
typedef void __fastcall (__closure *TFlowShapeMouseDownEvent)(System::TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int Row, int Col);

class PACKAGE TAZFlowShape : public TGraphicControl
{
private:
    int     FSpace; // 각 Cell 들의 간격.
    int     FCol;
    int     FRow;
    int     FRound;
    bool    FCaptionVisible;
    bool    FTransparent;
    bool    FCellLineColorUse;
    TColor  FCellLineColor;
    TColor  FColorOutLine;
    int     FOutLineSpace;
    bool    FInvalidateUse;
    TAZTitleProperty * FTitle;

    void    __fastcall SetCol(int n);
    void    __fastcall SetRow(int n);
    void    __fastcall SetSpace(int n);
    void    __fastcall SetRound(int n);
    void    __fastcall SetCaptionVisible(bool b);
    void    __fastcall SetTransparent(bool b);
    void    __fastcall SetCellLineColorUse(bool b);
    void    __fastcall SetCellLineColor(TColor c);
    void    __fastcall SetColorOutLine(TColor c);
    void    __fastcall SetOutLineSpace(int n);
    void    __fastcall SetInvalidateUse(bool b);
    void    __fastcall SetTitle(TAZTitleProperty * p);
    TAZTitleProperty * __fastcall GetTitle();

    TFlowShapeCell * __fastcall GetCells(int nRow, int nCol);
    //void             __fastcall SetCells(int nRow, int nCol);

    //-----------------------
    bool    bLoaded;
    bool    bBeginUpdate;       // Update 중에 Paint 하지 않기 위한 변수.
    bool    bChangedCellOnly;   // Cell 만 변경된 경우 Cell 만 다시 그린다.

    TFlowShapeCell ** FppCells;
    int     nCreateCellCount;
    void    __fastcall DeleteCells();
    void    __fastcall CreateCells();

    void    __fastcall CellChanged(System::TObject* Sender);
    void    __fastcall TitleChanged(System::TObject* Sender);

    int     nClickRow;
    int     nClickCol;
    bool    __fastcall FindCell(const int x, const int y, int &Row, int &Col);

    TFlowShapeClickEvent        FOnClick;
    TFlowShapeMouseDownEvent    FOnMouseDown;
    TFlowShapeMouseUpEvent      FOnMouseUp;
    
    void    __fastcall MyClick(TObject *Sender);
    void    __fastcall MyMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void    __fastcall MyMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

protected:
    virtual void __fastcall WndProc(TMessage& Message);     ///< WndProc 함수 오버라이드.
    virtual void    __fastcall SetEnabled(bool Value);      ///< SetEnabled 함수 오버라이드.
    void    __fastcall Loaded(void);                        ///< Loaded 함수 오버라이드.
    void __fastcall DrawPaint(bool bOnlyCellUpdate);

public:
            __fastcall  TAZFlowShape(TComponent* Owner);
            __fastcall ~TAZFlowShape();
    void    __fastcall  Paint();                            ///< Paint 함수 오버라이드.
    void    __fastcall  BeginUpdate();
    void    __fastcall  EndUpdate();

    __property TFlowShapeCell * Cells[int nRow][int nCol] = { read = GetCells };


__published:
    __property int      AZ_Col          = { read = FCol,            write = SetCol,     default = 3 };
    __property int      AZ_Row          = { read = FRow,            write = SetRow,     default = 3 };
    __property int      AZ_Space        = { read = FSpace,          write = SetSpace,   default = 2 };
    __property int      AZ_Round        = { read = FRound,          write = SetRound,   default = 2 };

    __property  bool    AZ_CellLineColorUse = { read = FCellLineColorUse, write = SetCellLineColorUse, default = true };
    __property  TColor  AZ_CellLineColor    = { read = FCellLineColor,    write = SetCellLineColor,    default = clGray };

    __property bool     AZ_CaptionVisible   = { read = FCaptionVisible, write = SetCaptionVisible, default = true};
    __property TColor   AZ_OutLineColor     = { read = FColorOutLine,   write = SetColorOutLine, default = clGray };
    __property int      AZ_OutLineSpace     = { read = FOutLineSpace,   write = SetOutLineSpace, default = 1 };
    __property bool     Transparent         = { read = FTransparent,    write = SetTransparent, default = true };
    __property bool     InvalidateUse       = { read = FInvalidateUse,  write = SetInvalidateUse, default = false };
    __property TAZTitleProperty * Title     = { read = GetTitle,        write = SetTitle };

    __property Color;
    __property Font;
    __property Align;
    __property Visible;

    // 재 지정한 Event ---------------------------
    __property TFlowShapeClickEvent     OnClick     = {read = FOnClick,     write = FOnClick        };
    __property TFlowShapeMouseDownEvent OnMouseDown = {read = FOnMouseDown, write = FOnMouseDown    };
    __property TFlowShapeMouseUpEvent   OnMouseUp   = {read = FOnMouseUp,   write = FOnMouseUp      };
    __property OnMouseLeave;
    __property OnMouseMove ;
};
//---------------------------------------------------------------------------
#endif
