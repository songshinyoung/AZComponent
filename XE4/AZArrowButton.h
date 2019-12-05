//---------------------------------------------------------------------------

#ifndef AZArrowButtonH
#define AZArrowButtonH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Buttons.hpp>
#include "Common\\Define.h"
#include "AZUnitProperty.h"
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TArrowBtnClickEvent)(System::TObject* Sender, TArrowButtonState eState);
typedef void __fastcall (__closure *TArrowBtnMouseDownEvent)(System::TObject *Sender, TMouseButton Button,
                                                             TShiftState Shift, TArrowButtonState eState);
typedef void __fastcall (__closure *TArrowBtnMouseLeaveEvent)(TObject *Sender, TArrowButtonState eState);
typedef void __fastcall (__closure *TArrowBtnMouseUpEvent)(  TObject *Sender, TMouseButton Button,
                                                             TShiftState Shift, TArrowButtonState eState);

//---------------------------------------------------------------------------
class PACKAGE TAZArrowButton : public TWinControl
{
private:
    int              m_nTestCount;

    bool             m_bLoaded; // Load가 완료되면 true 
    
    TArrowButtomView FButtonView;           // 보여질 버튼 설정. (수평, 수직, 둘 다)
    bool             FEditBoxVisible;
    TFont *          FFontTitle;
    TFont *          FFontValue;
    TFont *          FFontUnit;
    String           FCaptionTitle;
    String           FCaptionUnit;
    double           FCaptionValue;

    String           FBtnCaptionLeft;
    String           FBtnCaptionRight;
    String           FBtnCaptionTop;
    String           FBtnCaptionBottom;

    TColor           FColorBox;
    TColor           FColorEdit;

    int              FGlyphType;            // 화살표 모양

    Vcl::Graphics::TBitmap*     FGlyph;
    Vcl::Buttons::TNumGlyphs    FNumGlyphs;

    TAZEditBoxProperty * FInputEdit;

    void __fastcall SetButtonView(TArrowButtomView e);
    void __fastcall SetEditBoxVisible(bool b);
    void __fastcall SetMyFont(int Index, TFont * p);
    void __fastcall SetMyCaption(int Index, String s);
    void __fastcall SetColor(int Index, TColor c);
    void __fastcall SetGlyphType(int n);
    void __fastcall SetInputEdit(TAZEditBoxProperty * p);
    void __fastcall SetCaptionValue(double d);

    void                    __fastcall SetGlyph(Vcl::Graphics::TBitmap* Value);
    Vcl::Graphics::TBitmap* __fastcall GetGlyph(void);
    void                    __fastcall SetNumGlyphs(TNumGlyphs Value);
    TNumGlyphs              __fastcall GetNumGlyphs(void);

    System::Classes::TNotifyEvent FOnEditClick;

    //-------------------------------
    TBitBtn * FBitBtn[abMax];
    TPanel  * FBasePanel;
    TLabel  * FLabelTitle;
    TLabel  * FLabelValue;
    TLabel  * FLabelUnit;

    //Graphics::TBitmap* BMP_Indicator;

    void        __fastcall DisplayUpdate(bool bView);
    void        __fastcall DisplayEditBox(int nWidth, int nHeight);
    void        __fastcall CreateIndicatorImage(bool bView);

    void         __fastcall MyResize(TObject *Sender);
    System::Classes::TNotifyEvent FOnResize;

    TArrowBtnClickEvent FOnClick;
    void         __fastcall MyClick(TObject *Sender);
    void         __fastcall MyEditClick(TObject *Sender);

    TArrowBtnMouseDownEvent     FOnMouseDown;
    TArrowBtnMouseLeaveEvent    FOnMouseLeave;
    TArrowBtnMouseUpEvent       FOnMouseUp;
    void        __fastcall MyMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void        __fastcall MyMouseLeave(TObject *Sender);
    void        __fastcall MyMouseUp(  TObject *Sender, TMouseButton Button,
                                        TShiftState Shift, int X, int Y);

    TBitBtn *   __fastcall GetBitBtn(int n);

    void        __fastcall FontChanged(System::TObject* Sender);
    void        __fastcall InputEditChanged(System::TObject* Sender);

    bool        __fastcall InputNumber(TObject *Sender);

    void        __fastcall GlyphChanged(System::TObject* Sender);
    bool        __fastcall IsCustom(void);

protected:
    virtual void     __fastcall Loaded(void);                ///< Loaded 함수 오버라이드.
    virtual void     __fastcall SetEnabled(bool Value);      ///< SetEnabled 함수 오버라이드.

public:
                __fastcall  TAZArrowButton(TComponent* Owner);
    virtual     __fastcall ~TAZArrowButton();

    __property TBitBtn * BitBtn[int n] = { read = GetBitBtn };

__published:

    __property TArrowButtomView ButtonView = { read = FButtonView,  write = SetButtonView, default = avBoth };
    __property bool EditBoxVisible = { read = FEditBoxVisible, write = SetEditBoxVisible, default = true };

    __property String CaptionTitle = { read = FCaptionTitle, write = SetMyCaption, index = 1};
    __property String CaptionUnit  = { read = FCaptionUnit,  write = SetMyCaption, index = 3};
    __property String BtnCaptionLeft    = { read = FBtnCaptionLeft,     write = SetMyCaption, index = 4};
    __property String BtnCaptionRight   = { read = FBtnCaptionRight,    write = SetMyCaption, index = 5};
    __property String BtnCaptionTop     = { read = FBtnCaptionTop,      write = SetMyCaption, index = 6};
    __property String BtnCaptionBottom  = { read = FBtnCaptionBottom,   write = SetMyCaption, index = 7};

    __property double CaptionValue = { read = FCaptionValue, write = SetCaptionValue, default = 0};

    __property Font;
    __property TFont *  FontTitle  = { read = FFontTitle,  write = SetMyFont,    index = 1 };
    __property TFont *  FontValue  = { read = FFontValue,  write = SetMyFont,    index = 2 };
    __property TFont *  FontUnit   = { read = FFontUnit,   write = SetMyFont,    index = 3 };

    __property TColor   ColorBox   = { read = FColorBox,  write = SetColor,  index = 1, default = clBlack };
    __property TColor   ColorEdit  = { read = FColorEdit, write = SetColor,  index = 2, default = (TColor)0x373737 };

    __property int      GlyphType  = { read = FGlyphType, write = SetGlyphType, default = 1 };

    __property Vcl::Graphics::TBitmap* Glyph = { read = GetGlyph,     write = SetGlyph,     stored=IsCustom};
    __property TNumGlyphs  NumGlyphs         = { read = GetNumGlyphs, write = SetNumGlyphs, stored=IsCustom, default=4};

    __property TAZEditBoxProperty * InputEdit= { read = FInputEdit,   write = SetInputEdit };

    __property Enabled  = {default = 1};

    __property Tag;
    __property Align;
    __property Left;
    __property Top;
    __property Width    = { default = 200 };
    __property Height   = { default = 200 };

//   __property TBitBtn * BitBtnTop       = { read = FBitBtn[abTop] };
//   __property TBitBtn * BitBtnBottom    = { read = FBitBtn[abBottom] };
//   __property TBitBtn * BitBtnLeft      = { read = FBitBtn[abLeft] };
//   __property TBitBtn * BitBtnRight     = { read = FBitBtn[abRight] };

    //----------------------------
    // EVENT
    __property System::Classes::TNotifyEvent    OnResize     = {read=FOnResize, write=FOnResize};
    __property TArrowBtnClickEvent              OnClick      = {read=FOnClick, write=FOnClick};
    __property System::Classes::TNotifyEvent    OnEditClick  = {read=FOnEditClick, write=FOnEditClick};
    __property TArrowBtnMouseDownEvent          OnMouseDown  = {read=FOnMouseDown, write=FOnMouseDown};
    __property TArrowBtnMouseLeaveEvent         OnMouseLeave = {read=FOnMouseLeave, write=FOnMouseLeave};
    __property TArrowBtnMouseUpEvent            OnMouseUp    = {read=FOnMouseUp, write=FOnMouseUp};
    __property Visible;

};
//---------------------------------------------------------------------------
#endif
