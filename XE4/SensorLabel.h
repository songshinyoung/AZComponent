//---------------------------------------------------------------------------

#ifndef SensorLabelH
#define SensorLabelH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include "Define.h"

//---------------------------------------------------------------------------
class PACKAGE TSensorLabel : public TCustomLabel
{
private:
    bool            bLoaded;
    //--------------------------------
    // Number 
    bool            FNumVisible;
    int             FNumber;
    TColor          FNumBGColor;
    TColor          FNumLineColor;
    int             FNumRectRound;
    int             FNumWidth;
    int             FNumSpace;
    Vcl::Graphics::TFont    *   FNumFont;          ///< 폰트 특성

    //--------------------------------
    // Sensor Led 
    TColor          FSenLineColor;
    TColor          FSenOnColor;
    TColor          FSenOffColor;
    TSensorLedType  FSenType;
    int             FSenWidth;
    int             FSenHeight;
    int             FSenRectRound;
    bool            FSenOn;         ///< true일 경우 On Color, false일 경우 Off Color를 적용 
    int             FSenSpace;
    bool            FSenLEDImage;   ///< true일 경우 기본 Default LED Image사용 

    Graphics::TBitmap* pBmpSenOn;
    Graphics::TBitmap* pBmpSenOff;
    Graphics::TBitmap* pBmpSenDisable;

    //--------------------------------
    // Title



    
    //--------------------------------

    Graphics::TBitmap* pBitmapBase;

    //--------------------------------

    void            __fastcall SetNumVisible(bool b);
    void            __fastcall SetNumber(int    v);
    void            __fastcall SetNumFont(Vcl::Graphics::TFont * p);
    void            __fastcall SetNumBGColor(TColor v);
    void            __fastcall SetNumLineColor(TColor v);
    void            __fastcall SetNumRectRound(int    v);
    void            __fastcall SetNumWidth(int    v);
    void            __fastcall SetNumSpace(int    v);
    void            __fastcall SetSenLineColor(TColor v);
    void            __fastcall SetSenOnColor(TColor v);
    void            __fastcall SetSenOffColor(TColor v);
    void            __fastcall SetSenType(TSensorLedType  v);
    void            __fastcall SetSenWidth(int    v);
    void            __fastcall SetSenHeight(int    v);
    void            __fastcall SetSenRectRound(int v);
    void            __fastcall SetSenOn(bool   v);
    void            __fastcall SetSenSpace(int    v);
    void            __fastcall SetSenLEDImage(bool v);

    void            __fastcall  NumFontChanged(System::TObject* Sender);   ///< Font의 속성이 변경된 것을 Call Back 받기 위한 Event 함수.

protected:
    virtual void     __fastcall Loaded(void);                    ///< Loaded 함수 오버라이드.
    virtual void     __fastcall Paint();                         ///< Paint 함수 오버라이드.
    virtual void     __fastcall SetEnabled(bool Value);          ///< SetEnabled 함수 오버라이드.

    void            __fastcall DrawSensor();
    void            __fastcall DrawNumber();
    void            __fastcall DrawTitle();

public:
    __fastcall TSensorLabel(TComponent* Owner);
    virtual __fastcall ~TSensorLabel();

__published:

    __property Vcl::Graphics::TFont * NumFont   = { read = FNumFont,        write = SetNumFont,         stored = true     };
    __property  bool            NumVisible      = { read = FNumVisible  ,   write = SetNumVisible,      default = true };
    __property  int             Number          = { read = FNumber,         write = SetNumber,          default = 0 };
    __property  TColor          NumBGColor      = { read = FNumBGColor,     write = SetNumBGColor,      default = clTeal };
    __property  TColor          NumLineColor    = { read = FNumLineColor,   write = SetNumLineColor,    default = clBlack };
    __property  int             NumRectRound    = { read = FNumRectRound,   write = SetNumRectRound,    default = 0 };
    __property  int             NumWidth        = { read = FNumWidth,       write = SetNumWidth,        default = 25};
    __property  int             NumSpace        = { read = FNumSpace,       write = SetNumSpace,        default = 5 };

    __property  TColor          SenLineColor    = { read = FSenLineColor,   write = SetSenLineColor,    default = clBlack };
    __property  TColor          SenOnColor      = { read = FSenOnColor,     write = SetSenOnColor,      default = clLime };
    __property  TColor          SenOffColor     = { read = FSenOffColor,    write = SetSenOffColor,     default = clSilver };
    __property  TSensorLedType  SenType         = { read = FSenType,        write = SetSenType,         default = slCircle };
    __property  int             SenWidth        = { read = FSenWidth,       write = SetSenWidth,        default = 20 };
    __property  int             SenHeight       = { read = FSenHeight,      write = SetSenHeight,       default = 20 };
    __property  int             SenRectRound    = { read = FSenRectRound,   write = SetSenRectRound,    default = 3  };
    __property  bool            SenOn           = { read = FSenOn,          write = SetSenOn,           default = false };     ///< true일 경우 On Color, false일 경우 Off Color를 적용 
    __property  int             SenSpace        = { read = FSenSpace,       write = SetSenSpace,        default = 5 };
    __property  bool            SenLEDImage     = { read = FSenLEDImage,    write = SetSenLEDImage,     default = true };


    //---------------------------------------
    __property Width            = {default=200};
    __property Height           = {default=20};
    __property Align            = {default=0};
    __property Alignment        = {default=0};
    __property Anchors          = {default=3};
    //__property AutoSize         = {default=false};
    __property BiDiMode;
    __property Caption          = {default=0};
    //__property Color = {default = (TColor)0xA53E1E};
    __property Constraints;
    __property DragCursor       = {default=-12};
    __property DragKind         = {default=0};
    __property DragMode         = {default=0};
    __property EllipsisPosition = {default=0};
    __property Enabled          = {default=1};
    __property FocusControl;
    __property Font;
    __property GlowSize         = {default=0};
    __property ParentBiDiMode   = {default=1};
    __property ParentColor      = {default=0};
    __property ParentFont       = {default=0};
    __property ParentShowHint   = {default=1};
    __property PopupMenu;
    __property ShowAccelChar    = {default=1};
    __property ShowHint;
    __property Touch;
    __property Transparent      = {default = true};
    __property Layout           = {default=0};
    __property Visible          = {default=1};
    __property WordWrap         = {default=0};
    __property StyleElements    = {default=7};
    __property OnClick;
    __property OnContextPopup;
    __property OnDblClick;
    __property OnDragDrop;
    __property OnDragOver;
    __property OnEndDock;
    __property OnEndDrag;
    __property OnGesture;
    __property OnMouseActivate;
    __property OnMouseDown;
    __property OnMouseMove;
    __property OnMouseUp;
    __property OnMouseEnter;
    __property OnMouseLeave;
    __property OnStartDock;
    __property OnStartDrag;

};
//---------------------------------------------------------------------------
#endif
