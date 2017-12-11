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

    TColor              FColorUp;               ///< ���õ��� ���� ���� ����. ( Enable)
    TColor              FColorDisable;          ///< Disable ���� ����.
    TColor              FColorDown;             ///< ����ڰ� Mouse Down ���� ���� ����.
    TColor              FColorSelect;           ///< ���õ� ���� ����.
    TColor              FColorLine;             ///< �ε������ ���� ����.

    TCaption            FCaptionLeft;
    TCaption            FCaptionRight;
    TDualButtonState    FState;                 ///< ���� ���õ� ��ư ����. 0:�ƹ��͵� ���� �ȵ�. 1:����, 2:������
    bool                FAllowAllUp;            ///< �� ��ư�� ���ÿ� Up ���·� ���� �� �ִ� ���� �����.
    int                 FMargin;                ///< �̹����� �ܰ��� �Ÿ�
    int                 FSpacing;               ///< �̹����� Caption�� �Ÿ�.
    int                 FGlyphRound;            ///< Glyph�� �𼭸� Round ����.

    TDualButtonLayout   FLayout;                ///< ��ư�� ���̴� ���� (��-��, ��-�Ʒ�)
    TButtonLayout       FGlyphLayout;           ///< ��ư�� Glyph�� ���̴� ����.

    //---------------------------------
    // Sensor Led 
    TColor              FSenBGColor;
    TColor              FSenLineColor;
    TColor              FSenOnColor;
    TColor              FSenOffColor;
    TSensorLedType      FSenType;
    int                 FSenWidth;
    int                 FSenHeight;
    int                 FSenRectRound;
    int                 FSenSpace;
    bool                FSenLEDImage;   ///< true�� ��� �⺻ Default LED Image��� 
    bool                FSenVisibleL;   ///< ���� ���� ���̱�     
    bool                FSenVisibleR;   ///< ���� ���� ���̱� 

    bool                FSenOutLine;
    TColor              FSenOutLineColor;
    int                 FSenOutLineRound;
    int                 FSenOutSpace;

    int                 FHeightTitle;

    Vcl::Graphics::TFont    *   FSenFont;          ///< ��Ʈ Ư��
    
    //--------------------------------
    bool                FSenOnL;        ///< true�� ��� On Color, false�� ��� Off Color�� ���� 
    bool                FSenOnR;        ///< true�� ��� On Color, false�� ��� Off Color�� ���� 
    

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
    void                __fastcall SetSenOnL(bool   v);
    void                __fastcall SetSenOnR(bool   v);
    void                __fastcall SetSenSpace(int    v);
    void                __fastcall SetSenLEDImage(bool v);
    void                __fastcall SetSenVisibleL(bool v);
    void                __fastcall SetSenVisibleR(bool v);

    void                __fastcall SetSenOutLine(bool  v);
    void                __fastcall SetSenOutLineColor(TColor    v);
    void                __fastcall SetSenOutLineRound(int       v);
    void                __fastcall SetSenOutSpace(int       v);

    void                __fastcall SetHeightTitle(int v);
    void                __fastcall SetMyCaption(TCaption v);
    void                __fastcall SetSenFont(Vcl::Graphics::TFont * p);

    //-------------------------------
    Graphics::TBitmap* BMP_Indicator;

    void __fastcall DisplayUpdate();
    void __fastcall CreateIndicatorImage();

    void __fastcall MyResize(TObject *Sender);
    System::Classes::TNotifyEvent FOnResize;

    TActuatorBtnClickEvent FOnClick;
    void __fastcall MyClick(TObject *Sender);

    void            __fastcall  SenFontChanged(System::TObject* Sender);   ///< Font�� �Ӽ��� ����� ���� Call Back �ޱ� ���� Event �Լ�.

protected:
    virtual void     __fastcall Loaded(void);                ///< Loaded �Լ� �������̵�.
    virtual void     __fastcall SetEnabled(bool Value);      ///< SetEnabled �Լ� �������̵�.
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
    __property  TColor          SenBGColor      = { read = FSenBGColor,     write = SetSenBGColor,      default = clBtnFace  };
    __property  TColor          SenLineColor    = { read = FSenLineColor,   write = SetSenLineColor,    default = clBlack };
    __property  TColor          SenOnColor      = { read = FSenOnColor,     write = SetSenOnColor,      default = clLime };
    __property  TColor          SenOffColor     = { read = FSenOffColor,    write = SetSenOffColor,     default = clSilver };
    __property  TSensorLedType  SenType         = { read = FSenType,        write = SetSenType,         default = slCircle };
    __property  int             SenWidth        = { read = FSenWidth,       write = SetSenWidth,        default = 15 };
    __property  int             SenHeight       = { read = FSenHeight,      write = SetSenHeight,       default = 15 };
    __property  int             SenRectRound    = { read = FSenRectRound,   write = SetSenRectRound,    default = 3  };
    __property  bool            SenOnL          = { read = FSenOnL,         write = SetSenOnL,          default = false };     ///< true�� ��� On Color, false�� ��� Off Color�� ���� 
    __property  bool            SenOnR          = { read = FSenOnR,         write = SetSenOnR,          default = false };     ///< true�� ��� On Color, false�� ��� Off Color�� ���� 
    __property  int             SenSpace        = { read = FSenSpace,       write = SetSenSpace,        default = 5 };
    __property  bool            SenLEDImage     = { read = FSenLEDImage,    write = SetSenLEDImage,     default = true };
    __property  bool            SenVisibleL     = { read = FSenVisibleL,    write = SetSenVisibleL,     default = true };
    __property  bool            SenVisibleR     = { read = FSenVisibleR,    write = SetSenVisibleR,     default = true };

    __property  bool            SenOutLine      = { read = FSenOutLine,     write = SetSenOutLine,      default = true };
    __property  TColor          SenOutLineColor = { read = FSenOutLineColor,write = SetSenOutLineColor, default = clBlack };;
    __property  int             SenOutLineRound = { read = FSenOutLineRound,write = SetSenOutLineRound, default = 0 };
    __property  int             SenOutSpace     = { read = FSenOutSpace,    write = SetSenOutSpace,     default = 5 };

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
