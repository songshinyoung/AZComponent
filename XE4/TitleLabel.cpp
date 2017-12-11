//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "TitleLabel.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TTitleLabel *)
{
    new TTitleLabel(NULL);
}
//---------------------------------------------------------------------------
__fastcall TTitleLabel::TTitleLabel(TComponent* Owner)
    : TCustomLabel(Owner)
{
    bLoaded         = false;
    AutoSize        = false;

    pBitmapBase     = NULL;

    Color = (TColor)0xA53E1E;

    Transparent     = false;
    ParentColor     = false;

    FSpace          = 10;
    FTopDownGap     = 30;
    FLineInterval   = 3;
    FOutLine        = true;
    FOutLineColor   = (TColor)0xA53E1E;

    Font->Size      = 12;
    Font->Color     = clWhite;
    Font->Style     = TFontStyles() << fsBold;

    Width           = 200;
    Height          = 40;

    FShadowTextColor    = clBlack;
    FShadowText         = false;
    FShadowTextRange    = 1;

    //--------------------------
    if(ComponentState.Contains(csDesigning)) {
        bLoaded = true;
    }      
}

__fastcall TTitleLabel::~TTitleLabel()
{
    if(pBitmapBase) delete pBitmapBase;
}

//---------------------------------------------------------------------------
//namespace Titlelabel
//{
//    void __fastcall PACKAGE Register()
//    {
//         TComponentClass classes[1] = {__classid(TTitleLabel)};
//         RegisterComponents(L"AZ", classes, 0);
//    }
//}
//---------------------------------------------------------------------------

void __fastcall TTitleLabel::Loaded(void)
{
    TGraphicControl::Loaded();

    bLoaded = true;

    Invalidate();
}


void     __fastcall TTitleLabel::SetEnabled(bool Value)
{
    if(Enabled != Value) {
        TCustomLabel::SetEnabled(Value);
        Invalidate();
    }

}

void __fastcall TTitleLabel::Paint()
{
    if(!bLoaded) return;
    
    if(Enabled) CreateBaseImage(Color,      Enabled);
    else        CreateBaseImage(clSilver,   Enabled);

    Canvas->StretchDraw(Rect(0, 0, Width, Height), pBitmapBase);

    if(FOutLine) {
        Canvas->Pen->Color = Enabled ? FOutLineColor : clSilver;
        Canvas->MoveTo(0,0);
        Canvas->LineTo(Width-1, 0);
        Canvas->LineTo(Width-1, Height-1);
        Canvas->LineTo(0, Height-1);
        Canvas->LineTo(0, 0);

    }

    // ---------------------------------------
    // Caption 출력
    Canvas->Font->Assign(Font);

    if(!Enabled) {
        Canvas->Font->Color    = clBtnFace;
    }

    int nTxtW = Canvas->TextWidth(Caption);
    int nTxtH = Canvas->TextHeight(Caption);

    int nStartX = 0;
    int nStartY = (Height - nTxtH) / 2;

    switch(Alignment ) {
        case taLeftJustify:
            nStartX = 2 + FSpace;
            break;
        case taRightJustify:
              nStartX = (this->Width - nTxtW - 2) - FSpace;
            break;
        case taCenter:
            nStartX = (this->Width - nTxtW) / 2;
            break;
    }

    Canvas->Brush->Style = bsClear;

    if(FShadowText) {
        Canvas->Font->Color = Enabled ? FShadowTextColor : clGray;
        Canvas->TextOutA(nStartX+FShadowTextRange, nStartY+FShadowTextRange, Caption);
    }

    Canvas->Font->Color = Enabled ? Font->Color : clBtnFace;
    //SetBkMode(Canvas->Handle, TRANSPARENT); // 투명 출력.
    Canvas->TextOutA(nStartX, nStartY, Caption);
    Canvas->Brush->Style = bsSolid;


}


void __fastcall TTitleLabel::CreateBaseImage(TColor cColor, bool bEnabled)
{
    if(pBitmapBase) delete pBitmapBase;

    pBitmapBase = new Graphics::TBitmap;

    pBitmapBase->Width      = 1;
    pBitmapBase->Height     = 40;

    int nRGB = ColorToRGB(cColor);
    int nB = (nRGB & 0xFF0000) >> 16;
    int nG = (nRGB & 0x00FF00) >> 8;
    int nR = (nRGB & 0x0000FF);

    pBitmapBase->Canvas->Pixels[0][20] = cColor;

    int nTopDownGap     = bEnabled ? FTopDownGap    : 10;
    int nLineInterval   = bEnabled ? FLineInterval  : 1;

    int nTempR = nR + nTopDownGap;
    int nTempG = nG + nTopDownGap;
    int nTempB = nB + nTopDownGap;

    for(int i=19; i>=0; i--) {

        nTempR += nLineInterval;
        nTempG += nLineInterval;
        nTempB += nLineInterval;

        nTempR = nTempR > 255 ? 255 : nTempR;
        nTempG = nTempG > 255 ? 255 : nTempG;
        nTempB = nTempB > 255 ? 255 : nTempB;

        int nTempRGB = (nTempB << 16) | (nTempG << 8) | nTempR;
        pBitmapBase->Canvas->Pixels[0][i] = (TColor)nTempRGB;
    }

    nTempR = nR;
    nTempG = nG;
    nTempB = nB;

    for(int i=21; i<pBitmapBase->Height; i++) {

        nTempR += nLineInterval;
        nTempG += nLineInterval;
        nTempB += nLineInterval;

        nTempR = nTempR > 255 ? 255 : nTempR;
        nTempG = nTempG > 255 ? 255 : nTempG;
        nTempB = nTempB > 255 ? 255 : nTempB;

        int nTempRGB = (nTempB << 16) | (nTempG << 8) | nTempR;
        pBitmapBase->Canvas->Pixels[0][i] = (TColor)nTempRGB;
    }
}

void __fastcall TTitleLabel::SetSpace(int n)
{
    if(FSpace != n) {
        FSpace = n;
        Invalidate();
    }
}

void __fastcall TTitleLabel::SetTopDownGap(int n)
{
    if(FTopDownGap != n) {
        FTopDownGap = n;
        Invalidate();
    }
}

void __fastcall TTitleLabel::SetLineInterval(int n)
{
    if(FLineInterval != n) {
        FLineInterval = n;
        Invalidate();
    }
}

void __fastcall TTitleLabel::SetOutLine(bool b)
{
    if(FOutLine != b) {
        FOutLine = b;
        Invalidate();
    }
}

void __fastcall TTitleLabel::SetOutLineColor(TColor c)
{
    if(FOutLineColor != c) {
        FOutLineColor = c;
        Invalidate();
    }
}

void __fastcall TTitleLabel::SetShadowTextColor(TColor c)
{
    if(FShadowTextColor != c) {
        FShadowTextColor = c;
        Invalidate();
    }
}

void __fastcall TTitleLabel::SetShadowText(bool b)
{
    if(FShadowText != b) {
        FShadowText = b;
        Invalidate();
    }
}

void __fastcall TTitleLabel::SetShadowTextRange(int n)
{
    if(FShadowTextRange != n) {
        FShadowTextRange = n;
        Invalidate();
    }
}


