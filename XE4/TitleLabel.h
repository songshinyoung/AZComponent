//---------------------------------------------------------------------------

#ifndef TitleLabelH
#define TitleLabelH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TTitleLabel : public TCustomLabel
{
private:
    int     FSpace;             ///< 외곽과 Caption 사이의 공간
    int     FTopDownGap;        ///< 상단과 하단의 색상 밝기 차이
    int     FLineInterval;      ///< 라인별로 그라데이션 효과 정도.

    bool    FOutLine;           ///< 외곽 라인 표시.
    TColor  FOutLineColor;      ///< 외곽 라인 색상.
    TColor  FShadowTextColor;
    bool    FShadowText;
    int     FShadowTextRange;

    Graphics::TBitmap* pBitmapBase;
    void __fastcall CreateBaseImage(TColor color, bool bEnabled);

    void __fastcall SetSpace(int n);
    void __fastcall SetTopDownGap(int n);
    void __fastcall SetLineInterval(int n);
    void __fastcall SetOutLine(bool b);
    void __fastcall SetOutLineColor(TColor c);
    void __fastcall SetShadowTextColor(TColor c);
    void __fastcall SetShadowText(bool b);
    void __fastcall SetShadowTextRange(int n);

protected:
    virtual void     __fastcall Loaded(void);                    ///< Loaded 함수 오버라이드.
    virtual void     __fastcall Paint();                         ///< Paint 함수 오버라이드.
    virtual void     __fastcall SetEnabled(bool Value);        ///< SetEnabled 함수 오버라이드.

public:
    __fastcall TTitleLabel(TComponent* Owner);
    virtual __fastcall ~TTitleLabel();

__published:

    __property int Space            = { read = FSpace,          write = SetSpace,           default = 10 };
    __property int TopDownGap       = { read = FTopDownGap,     write = SetTopDownGap,      default = 30 };
    __property int LineInterval     = { read = FLineInterval,   write = SetLineInterval,    default = 3 };

    __property bool OutLine         = { read = FOutLine,        write = SetOutLine,         default = true };
    __property TColor OutLineColor  = { read = FOutLineColor,   write = SetOutLineColor,    default = (TColor)0xA53E1E };

    __property TColor ShadowTextColor   = { read = FShadowTextColor,    write = SetShadowTextColor,     default = clBlack };
    __property bool ShadowText          = { read = FShadowText,         write = SetShadowText,          default = false };
    __property int  ShadowTextRange     = { read = FShadowTextRange,    write = SetShadowTextRange,     default = 1 };

    //---------------------------------------
    __property Align = {default=0};
    __property Alignment = {default=0};
    __property Anchors = {default=3};
    __property AutoSize = {default=0};
    __property BiDiMode;
    __property Caption = {default=0};
    __property Color = {default = (TColor)0xA53E1E};
    __property Constraints;
    __property DragCursor = {default=-12};
    __property DragKind = {default=0};
    __property DragMode = {default=0};
    __property EllipsisPosition = {default=0};
    __property Enabled = {default=1};
    __property FocusControl;
    __property Font;
    __property GlowSize = {default=0};
    __property ParentBiDiMode = {default=1};
    __property ParentColor = {default=0};
    __property ParentFont = {default=0};
    __property ParentShowHint = {default=1};
    __property PopupMenu;
    __property ShowAccelChar = {default=1};
    __property ShowHint;
    __property Touch;
    __property Transparent= {default = 0};
    __property Layout = {default=0};
    __property Visible = {default=1};
    __property WordWrap = {default=0};
    __property StyleElements = {default=7};
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
