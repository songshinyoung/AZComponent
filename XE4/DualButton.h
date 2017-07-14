//---------------------------------------------------------------------------

#ifndef DualButtonH
#define DualButtonH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "Common\\Define.h"
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TDualBtnClickEvent)(System::TObject* Sender, TDualButtonState eState);

//---------------------------------------------------------------------------
class PACKAGE TDualButton : public TWinControl
{
private:
    TSpeedButton *      FSBLeft;
    TSpeedButton *      FSBRight;

    TColor              FColorUp;               ///< 선택되지 않은 상태 색상. ( Enable)
    TColor              FColorDisable;          ///< Disable 상태 색상.
    TColor              FColorDown;             ///< 사용자가 Mouse Down 중인 상태 색상.
    TColor              FColorSelect;           ///< 선택된 상태 색상.
    TColor              FColorLine;             ///< 인디게이터 라인 색상.

    TCaption            FCaptionLeft;
    TCaption            FCaptionRight;
    TDualButtonState    FState;                 ///< 현재 선택된 버튼 상태. 0:아무것도 선택 안됨. 1:왼쪽, 2:오른쪽
    bool                FAllowAllUp;            ///< 두 버튼이 동시에 Up 상태로 있을 수 있는 것을 허용함.
    int                 FMargin;                ///< 이미지와 외곽의 거리
    int                 FSpacing;               ///< 이미지와 Caption의 거리.
    int                 FGlyphRound;            ///< Glyph의 모서리 Round 정도.

    TDualButtonLayout   FLayout;                ///< 버튼이 놓이는 방향 (좌-우, 위-아래)
    TButtonLayout       FGlyphLayout;           ///< 버튼에 Glyph이 놓이는 방향.

    void __fastcall SetCaptionLeft(TCaption s);
    void __fastcall SetCaptionRight(TCaption s);
    void __fastcall SetState(TDualButtonState e);
    void __fastcall SetAllowAllUp(bool b);
    void __fastcall SetMargin(int n);
    void __fastcall SetSpacing(int n);
    void __fastcall SetColor(int Index, TColor c);
    void __fastcall SetLayout(TDualButtonLayout e);
    void __fastcall SetGlyphLayout(TButtonLayout e);
    void __fastcall SetGlyphRound(int n);

    //-------------------------------
    Graphics::TBitmap* BMP_Indicator;

    void __fastcall DisplayUpdate();
    void __fastcall CreateIndicatorImage();

    void __fastcall MyResize(TObject *Sender);
    System::Classes::TNotifyEvent FOnResize;

    TDualBtnClickEvent FOnClick;
    void __fastcall MyClick(TObject *Sender);

protected:
    virtual void     __fastcall Loaded(void);                ///< Loaded 함수 오버라이드.
    virtual void     __fastcall SetEnabled(bool Value);      ///< SetEnabled 함수 오버라이드.
    //virtual void    __fastcall SetParent(Vcl::Controls::TWinControl* AParent);

    //void     __fastcall FontChanged(System::TObject* Sender);

public:
    __fastcall TDualButton(TComponent* Owner);
    virtual __fastcall ~TDualButton();

__published:
    __property TSpeedButton * SBLeft  = { read = FSBLeft};
    __property TSpeedButton * SBRight = { read = FSBRight};

    //----------------------------
    // PROPERTY
    __property TCaption         CaptionLeft     = { read = FCaptionLeft,   write = SetCaptionLeft };
    __property TCaption         CaptionRight    = { read = FCaptionRight,  write = SetCaptionRight };
    __property TDualButtonState ButtonState     = { read = FState,         write = SetState,       default = dbNone };
    __property bool             AllowAllUp      = { read = FAllowAllUp,    write = SetAllowAllUp,  default = false };
    __property int              Margin          = { read = FMargin,        write = SetMargin,      default = -1 };
    __property int              Spacing         = { read = FSpacing,       write = SetSpacing,     default = 4 };
    __property int              GlyphRound      = { read = FGlyphRound,    write = SetGlyphRound,  default = 2 };

    __property TColor           ColorUp         = { read = FColorUp,       write = SetColor,       index = 1, default = clGray };
    __property TColor           ColorDisable    = { read = FColorDisable,  write = SetColor,       index = 2, default = clBtnFace };
    __property TColor           ColorDown       = { read = FColorDown,     write = SetColor,       index = 3, default = clRed };
    __property TColor           ColorSelect     = { read = FColorSelect,   write = SetColor,       index = 4, default = clLime };
    __property TColor           ColorLine       = { read = FColorLine,     write = SetColor,       index = 5, default = clBlack };

    __property TDualButtonLayout Layout         = { read = FLayout,        write = SetLayout,       default = dbHorizontal };
    __property TButtonLayout     GlyphLayout    = { read = FGlyphLayout,   write = SetGlyphLayout,  default = blGlyphTop };

    __property Font;
    __property Enabled  = {default = 1};

    __property Tag;
    __property Align;
    __property Left;
    __property Top;
    __property Width;
    __property Height;
    __property Visible;

    //----------------------------
    // EVENT
    __property System::Classes::TNotifyEvent OnResize = {read=FOnResize, write=FOnResize};

    __property TDualBtnClickEvent OnClick = {read=FOnClick, write=FOnClick};
};
//---------------------------------------------------------------------------
#endif
