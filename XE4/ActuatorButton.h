//---------------------------------------------------------------------------

#ifndef ActuatorButtonH
#define ActuatorButtonH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "Common\\Define.h"
#include "DualSensorLabel.h"
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TActuatorBtnClickEvent)(System::TObject* Sender, TDualButtonState eState);

//---------------------------------------------------------------------------
class PACKAGE TActuatorButton : public TWinControl
{
private:
    bool                bLoaded;
    
    TDualSensorLabel *  FDualSenLabel;
    TSpeedButton *      FSBLeft;
    TSpeedButton *      FSBRight;

    Vcl::Graphics::TBitmap*     FGlyph_Left;
    Vcl::Graphics::TBitmap*     FGlyph_Right;
    Vcl::Buttons::TNumGlyphs    FNumGlyphs;

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

    bool                m_bDarkMode;

    //---------------------------------
    // Sensor Led 
    TColor              FSenBGColor;
    TColor              FSenLineColor;
    TColor              FSenOnColor;
    TColor              FSenOffColor;
    TSensorLedType      FSenType;
    TSensorLedColorType FSenColorType;
    int                 FSenWidth;
    int                 FSenHeight;
    int                 FSenRectRound;
    int                 FSenSpace;
    bool                FSenLEDImage;   ///< true일 경우 기본 Default LED Image사용 
    bool                FSenVisibleL;   ///< 왼쪽 센서 보이기     
    bool                FSenVisibleR;   ///< 왼쪽 센서 보이기 

    bool                FSenOutLine;
    TColor              FSenOutLineColor;
    int                 FSenOutLineRound;
    int                 FSenOutSpace;
    int                 FSenBetweenSpace;
    
    int                 FHeightTitle;

    Vcl::Graphics::TFont    *   FSenFont;          ///< 폰트 특성
    
    //--------------------------------
    bool                FSenOnL;        ///< true일 경우 On Color, false일 경우 Off Color를 적용 
    bool                FSenOnR;        ///< true일 경우 On Color, false일 경우 Off Color를 적용 
    
    bool                FSenOnL2; 
    bool                FSenOnR2;
    bool                FSenVisibleL2;
    bool                FSenVisibleR2;

    bool                FSenOnL3; 
    bool                FSenOnR3;
    bool                FSenVisibleL3;
    bool                FSenVisibleR3;

    bool                FSenOnL4; 
    bool                FSenOnR4;
    bool                FSenVisibleL4;
    bool                FSenVisibleR4;

    void                __fastcall SetCaptionLeft(TCaption s);
    void                __fastcall SetCaptionRight(TCaption s);
    void                __fastcall SetState(TDualButtonState e);
    void                __fastcall SetAllowAllUp(bool b);
    void                __fastcall SetMargin(int n);
    void                __fastcall SetSpacing(int n);
    void                __fastcall SetColor(int Index, TColor c);
    void                __fastcall SetLayout(TDualButtonLayout e);
    void                __fastcall SetGlyphLayout(TButtonLayout e);
    void                __fastcall SetGlyphRound(int n);

    //-------------------------------
    void                __fastcall SetSenBGColor(TColor v);
    void                __fastcall SetSenLineColor(TColor v);
    void                __fastcall SetSenOnColor(TColor v);
    void                __fastcall SetSenOffColor(TColor v);
    void                __fastcall SetSenType(TSensorLedType  v);
    void                __fastcall SetSenWidth(int    v);
    void                __fastcall SetSenHeight(int    v);
    void                __fastcall SetSenRectRound(int v);
    void                __fastcall SetSenOnL(int Index, bool   v);
    void                __fastcall SetSenOnR(int Index, bool   v);
    void                __fastcall SetSenSpace(int    v);
    void                __fastcall SetSenLEDImage(bool v);
    void                __fastcall SetSenVisibleL(int Index, bool v);
    void                __fastcall SetSenVisibleR(int Index, bool v);
    void                __fastcall SetSenColorType(TSensorLedColorType  v);

    void                __fastcall SetSenOutLine(bool  v);
    void                __fastcall SetSenOutLineColor(TColor    v);
    void                __fastcall SetSenOutLineRound(int       v);
    void                __fastcall SetSenOutSpace(int       v);
    void                __fastcall SetSenBetweenSpace(int       v);
    

    void                __fastcall SetHeightTitle(int v);
    void                __fastcall SetMyCaption(TCaption v);
    void                __fastcall SetSenFont(Vcl::Graphics::TFont * p);

    void                        __fastcall SetGlyph(int Index, Vcl::Graphics::TBitmap* Value);
    Vcl::Graphics::TBitmap*     __fastcall GetGlyph(int Index);
    void                        __fastcall SetNumGlyphs(Vcl::Buttons::TNumGlyphs Value);
    Vcl::Buttons::TNumGlyphs    __fastcall GetNumGlyphs(void);

    void                        __fastcall GlyphChanged(System::TObject* Sender);

    //-------------------------------
    Graphics::TBitmap* BMP_Indicator;

    void __fastcall DisplayUpdate();
    void __fastcall CreateIndicatorImage();

    void __fastcall MyResize(TObject *Sender);
    System::Classes::TNotifyEvent FOnResize;

    TActuatorBtnClickEvent FOnClick;
    void __fastcall MyClick(TObject *Sender);

    void            __fastcall  SenFontChanged(System::TObject* Sender);   ///< Font의 속성이 변경된 것을 Call Back 받기 위한 Event 함수.

protected:
    virtual void     __fastcall Loaded(void);                ///< Loaded 함수 오버라이드.
    virtual void     __fastcall SetEnabled(bool Value);      ///< SetEnabled 함수 오버라이드.
    //virtual void    __fastcall SetParent(Vcl::Controls::TWinControl* AParent);

    //void     __fastcall FontChanged(System::TObject* Sender);

public:
    __fastcall TActuatorButton(TComponent* Owner);
    virtual __fastcall ~TActuatorButton();

        
__published:
    __property TDualSensorLabel * DualSenLabel  = { read = FDualSenLabel, stored = true};
    __property TSpeedButton     * SBLeft        = { read = FSBLeft};
    __property TSpeedButton     * SBRight       = { read = FSBRight};

    //----------------------------
    __property  TColor              SenBGColor      = { read = FSenBGColor,     write = SetSenBGColor,      default = clBtnFace  };
    __property  TColor              SenLineColor    = { read = FSenLineColor,   write = SetSenLineColor,    default = clBlack };
    __property  TColor              SenOnColor      = { read = FSenOnColor,     write = SetSenOnColor,      default = clLime };
    __property  TColor              SenOffColor     = { read = FSenOffColor,    write = SetSenOffColor,     default = clSilver };
    __property  TSensorLedType      SenType         = { read = FSenType,        write = SetSenType,         default = slRect };
    __property  int                 SenWidth        = { read = FSenWidth,       write = SetSenWidth,        default = 15 };
    __property  int                 SenHeight       = { read = FSenHeight,      write = SetSenHeight,       default = 15 };
    __property  int                 SenRectRound    = { read = FSenRectRound,   write = SetSenRectRound,    default = 3  };
    __property  bool                SenOnL          = { read = FSenOnL,         write = SetSenOnL,          index=0, default = false };     ///< true일 경우 On Color, false일 경우 Off Color를 적용 
    __property  bool                SenOnR          = { read = FSenOnR,         write = SetSenOnR,          index=0, default = false };     ///< true일 경우 On Color, false일 경우 Off Color를 적용 
    __property  int                 SenSpace        = { read = FSenSpace,       write = SetSenSpace,        default = 5 };
    __property  bool                SenLEDImage     = { read = FSenLEDImage,    write = SetSenLEDImage,     default = true };
    __property  bool                SenVisibleL     = { read = FSenVisibleL,    write = SetSenVisibleL,     index=0, default = true };
    __property  bool                SenVisibleR     = { read = FSenVisibleR,    write = SetSenVisibleR,     index=0, default = true };
    __property  TSensorLedColorType SenColorType    = { read = FSenColorType,   write = SetSenColorType,    default = slColorGreen  };

    __property  bool                SenOnL2         = { read = FSenOnL2,        write = SetSenOnL,          index=1, default = false };     ///< true일 경우 On Color, false일 경우 Off Color를 적용 
    __property  bool                SenOnR2         = { read = FSenOnR2,        write = SetSenOnR,          index=1, default = false };     ///< true일 경우 On Color, false일 경우 Off Color를 적용 
    __property  bool                SenVisibleL2    = { read = FSenVisibleL2,   write = SetSenVisibleL,     index=1, default = false };
    __property  bool                SenVisibleR2    = { read = FSenVisibleR2,   write = SetSenVisibleR,     index=1, default = false };

    __property  bool                SenOnL3         = { read = FSenOnL3,        write = SetSenOnL,          index=2, default = false };     ///< true일 경우 On Color, false일 경우 Off Color를 적용 
    __property  bool                SenOnR3         = { read = FSenOnR3,        write = SetSenOnR,          index=2, default = false };     ///< true일 경우 On Color, false일 경우 Off Color를 적용 
    __property  bool                SenVisibleL3    = { read = FSenVisibleL3,   write = SetSenVisibleL,     index=2, default = false };
    __property  bool                SenVisibleR3    = { read = FSenVisibleR3,   write = SetSenVisibleR,     index=2, default = false };

    __property  bool                SenOnL4         = { read = FSenOnL4,        write = SetSenOnL,          index=3, default = false };     ///< true일 경우 On Color, false일 경우 Off Color를 적용 
    __property  bool                SenOnR4         = { read = FSenOnR4,        write = SetSenOnR,          index=3, default = false };     ///< true일 경우 On Color, false일 경우 Off Color를 적용 
    __property  bool                SenVisibleL4    = { read = FSenVisibleL4,   write = SetSenVisibleL,     index=3, default = false };
    __property  bool                SenVisibleR4    = { read = FSenVisibleR4,   write = SetSenVisibleR,     index=3, default = false };


    __property  bool            SenOutLine      = { read = FSenOutLine,     write = SetSenOutLine,      default = true };
    __property  TColor          SenOutLineColor = { read = FSenOutLineColor,write = SetSenOutLineColor, default = clBlack };;
    __property  int             SenOutLineRound = { read = FSenOutLineRound,write = SetSenOutLineRound, default = 0 };
    __property  int             SenOutSpace     = { read = FSenOutSpace,        write = SetSenOutSpace,     default = 5 };
    __property  int             SenBetweenSpace = { read = FSenBetweenSpace,    write = SetSenBetweenSpace, default = 3 };

    

    __property  int             HeightTitle     = { read = FHeightTitle,    write = SetHeightTitle,     default = 30 };
    __property Vcl::Graphics::TFont * SenFont   = { read = FSenFont,        write = SetSenFont,         stored = true     };
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

    __property Vcl::Graphics::TBitmap *     Glyph_Left  = { read = GetGlyph,        write = SetGlyph,   index = 1       };
    __property Vcl::Graphics::TBitmap *     Glyph_Right = { read = GetGlyph,        write = SetGlyph,   index = 2       };
    __property Vcl::Buttons::TNumGlyphs     NumGlyphs   = { read = GetNumGlyphs,    write = SetNumGlyphs,   default=4   };

    __property Font;
    __property Enabled  = {default = 1};

    __property Tag;
    __property Align;
    __property Left;
    __property Top;
    __property Width;
    __property Height   = {default = 81};
    __property Visible;
    __property Caption = { write = SetMyCaption };

    //----------------------------
    // EVENT
    __property System::Classes::TNotifyEvent OnResize = {read=FOnResize, write=FOnResize};

    __property TActuatorBtnClickEvent OnClick = {read=FOnClick, write=FOnClick};
};
//---------------------------------------------------------------------------
#endif
