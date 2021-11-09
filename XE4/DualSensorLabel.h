//---------------------------------------------------------------------------

#ifndef DualSensorLabelH
#define DualSensorLabelH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include "Define.h"

//---------------------------------------------------------------------------
class PACKAGE TDualSensorLabel : public TCustomLabel
{
private:
    bool            bLoaded;

    //--------------------------------
    // Sensor Led 
    TColor              FSenLineColor;
    TColor              FSenOnColor;
    TColor              FSenOffColor;
    TSensorLedType      FSenType;
    int                 FSenWidth;
    int                 FSenHeight;
    int                 FSenRectRound;
    int                 FSenSpace;
    bool                FSenLEDImage;   ///< true일 경우 기본 Default LED Image사용 
    TSensorLedColorType FSenColorType;

    Graphics::TBitmap* pBmpSenOn;
    Graphics::TBitmap* pBmpSenOff;
    Graphics::TBitmap* pBmpSenDisable;
    
    //--------------------------------
    bool            FSenOutLine;
    TColor          FSenOutLineColor;
    int             FSenOutLineRound;
    int             FSenOutSpace;
    int             FSenBetweenSpace;   // 센서 1번과 2번 사이의 공간 
    //--------------------------------
    bool            FSenOnL;        ///< true일 경우 On Color, false일 경우 Off Color를 적용 
    bool            FSenOnR;        ///< true일 경우 On Color, false일 경우 Off Color를 적용 
    bool            FSenVisibleL;   ///< 왼쪽 센서 보이기     
    bool            FSenVisibleR;   ///< 왼쪽 센서 보이기 

    bool            FSenOnL2;       ///< true일 경우 On Color, false일 경우 Off Color를 적용 
    bool            FSenOnR2;       ///< true일 경우 On Color, false일 경우 Off Color를 적용 
    bool            FSenVisibleL2;  ///< 왼쪽 센서 보이기     
    bool            FSenVisibleR2;  ///< 왼쪽 센서 보이기 
    
    //--------------------------------
    // Title



    
    //--------------------------------

    Graphics::TBitmap* pBitmapBase;

    //--------------------------------

    void            __fastcall SetSenLineColor(TColor v);
    void            __fastcall SetSenOnColor(TColor v);
    void            __fastcall SetSenOffColor(TColor v);
    void            __fastcall SetSenType(TSensorLedType  v);
    void            __fastcall SetSenWidth(int    v);
    void            __fastcall SetSenHeight(int    v);
    void            __fastcall SetSenRectRound(int v);
    void            __fastcall SetSenOnL(int Index, bool   v);
    void            __fastcall SetSenOnR(int Index, bool   v);
    void            __fastcall SetSenSpace(int    v);
    void            __fastcall SetSenLEDImage(bool v);
    void            __fastcall SetSenVisibleL(int Index, bool v);
    void            __fastcall SetSenVisibleR(int Index, bool v);
    void            __fastcall SetSenColorType(TSensorLedColorType  v);

    void            __fastcall SetSenOutLine(bool  v);
    void            __fastcall SetSenOutLineColor(TColor    v);
    void            __fastcall SetSenOutLineRound(int       v);
    void            __fastcall SetSenOutSpace(int       v);
    void            __fastcall SetSenBetweenSpace(int v);

protected:
    virtual void     __fastcall Loaded(void);                    ///< Loaded 함수 오버라이드.
    virtual void     __fastcall SetEnabled(bool Value);          ///< SetEnabled 함수 오버라이드.

    void            __fastcall DrawSensorL();
    void            __fastcall DrawSensorR();
    void            __fastcall DrawSensorL2();
    void            __fastcall DrawSensorR2();    
    void            __fastcall DrawTitle();
    void            __fastcall DrawOutLine();
    void            __fastcall LoadLEDImage(void);

public:
                    __fastcall TDualSensorLabel(TComponent* Owner);
    virtual         __fastcall ~TDualSensorLabel();
    virtual void    __fastcall Paint();                         ///< Paint 함수 오버라이드.

    void            __fastcall SetLoaded(bool b); // SSY_0171218 ( 다른 컨퍼넌트에서 동적으로 생성시 Loaded가 호출되지 않는다. 긍제로 Loaded를 된것처럼 설정하다 ) 

__published:

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

    __property  bool                SenOutLine      = { read = FSenOutLine,     write = SetSenOutLine,      default = true };
    __property  TColor              SenOutLineColor = { read = FSenOutLineColor,write = SetSenOutLineColor, default = clBlack };;
    __property  int                 SenOutLineRound = { read = FSenOutLineRound,write = SetSenOutLineRound, default = 0 };
    __property  int                 SenOutSpace     = { read = FSenOutSpace,        write = SetSenOutSpace,     default = 5 };
    __property  int                 SenBetweenSpace = { read = FSenBetweenSpace,    write = SetSenBetweenSpace, default = 3 };

    //---------------------------------------
    __property Width            = {default=200};
    __property Height           = {default=24};
    __property Align            = {default=0};
    __property Alignment        = {default=taCenter};
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
    __property Color            = {default=clBtnFace};
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
