//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "DualSensorLabel.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TDualSensorLabel *)
{
    new TDualSensorLabel(NULL);
}
//---------------------------------------------------------------------------
__fastcall TDualSensorLabel::TDualSensorLabel(TComponent* Owner)
    : TCustomLabel(Owner)
{
    bLoaded         = false;
    
    pBitmapBase     = NULL;

    AutoSize        = false;
    Transparent     = true;
    ParentColor     = false;
    Width           = 200;
    Height          = 24;
    Alignment       = taCenter;
    Color           = clBtnFace;

    FSenLineColor   = clBlack ;
    FSenOnColor     = clLime ;
    FSenOffColor    = clSilver ;
    FSenType        = slCircle;
    FSenColorType   = slColorGreen;
    FSenWidth       = 15 ;
    FSenHeight      = 15 ;
    FSenRectRound   = 3;
    FSenOnL         = false ;
    FSenOnR         = false ;
    FSenSpace       = 5 ;    
    FSenLEDImage    = true;
    FSenVisibleL    = true;
    FSenVisibleR    = true;
    
    FSenOutLine         = true;
    FSenOutLineColor    = clBlack;
    FSenOutLineRound    = 0;
    FSenOutSpace        = 5;

    pBmpSenOn       = new Graphics::TBitmap;
    pBmpSenOff      = new Graphics::TBitmap;
    pBmpSenDisable  = new Graphics::TBitmap;

    LoadLEDImage();

    //--------------------------
    if(ComponentState.Contains(csDesigning)) {
        bLoaded = true;
    }      
}

__fastcall TDualSensorLabel::~TDualSensorLabel()
{
    if(pBitmapBase)     delete pBitmapBase;
    if(pBmpSenOn)       delete pBmpSenOn;
    if(pBmpSenOff)      delete pBmpSenOff;
    if(pBmpSenDisable)  delete pBmpSenDisable;
    
}

//---------------------------------------------------------------------------
//namespace Titlelabel
//{
//    void __fastcall PACKAGE Register()
//    {
//         TComponentClass classes[1] = {__classid(TDualSensorLabel)};
//         RegisterComponents(L"AZ", classes, 0);
//    }
//}

//---------------------------------------------------------------------------

void __fastcall TDualSensorLabel::Loaded(void)
{
    TGraphicControl::Loaded();

    bLoaded = true;

    LoadLEDImage();

    Invalidate();
}

void            __fastcall TDualSensorLabel::SetLoaded(bool b)
{ 
    bLoaded = b;
}

void     __fastcall TDualSensorLabel::SetEnabled(bool Value)
{
    if(Enabled != Value) {
        TCustomLabel::SetEnabled(Value);
        Invalidate();
    }

}
void __fastcall TDualSensorLabel::Paint()
{
    DrawOutLine();
    
    DrawSensorL();
    
    DrawSensorR();

    DrawTitle();
}

//---------------------------------------------------------------------------
void __fastcall TDualSensorLabel::LoadLEDImage(void)
{
    if(FSenType == slCircle) {
        switch(FSenColorType) {
            case slColorGreen:
            default:
                pBmpSenOn       ->LoadFromResourceName((int)HInstance, "SensorLED_On");
                pBmpSenOff      ->LoadFromResourceName((int)HInstance, "SensorLED_Off");
                pBmpSenDisable  ->LoadFromResourceName((int)HInstance, "SensorLED_Disable");
                break;
                
            case slColorRed:
                pBmpSenOn       ->LoadFromResourceName((int)HInstance, "SensorLED_Red_On");
                pBmpSenOff      ->LoadFromResourceName((int)HInstance, "SensorLED_Red_Off");
                pBmpSenDisable  ->LoadFromResourceName((int)HInstance, "SensorLED_Disable");
                break;
                
            case slColorBlue:
                pBmpSenOn       ->LoadFromResourceName((int)HInstance, "SensorLED_Blue_On");
                pBmpSenOff      ->LoadFromResourceName((int)HInstance, "SensorLED_Blue_Off");
                pBmpSenDisable  ->LoadFromResourceName((int)HInstance, "SensorLED_Disable");
                break;
        }
    }
    else {
        switch(FSenColorType) {
            case slColorGreen:
            default:
                pBmpSenOn       ->LoadFromResourceName((int)HInstance, "SquareLED_On");
                pBmpSenOff      ->LoadFromResourceName((int)HInstance, "SquareLED_Off");
                pBmpSenDisable  ->LoadFromResourceName((int)HInstance, "SquareLED_Disable");
                break;
                
            case slColorRed:
                pBmpSenOn       ->LoadFromResourceName((int)HInstance, "SquareLED_Red_On");
                pBmpSenOff      ->LoadFromResourceName((int)HInstance, "SquareLED_Red_Off");
                pBmpSenDisable  ->LoadFromResourceName((int)HInstance, "SquareLED_Disable");
                break;
                
            case slColorBlue:
                pBmpSenOn       ->LoadFromResourceName((int)HInstance, "SquareLED_Blue_On");
                pBmpSenOff      ->LoadFromResourceName((int)HInstance, "SquareLED_Blue_Off");
                pBmpSenDisable  ->LoadFromResourceName((int)HInstance, "SquareLED_Disable");
                break;
        }
    }

}

//---------------------------------------------------------------------------

void __fastcall TDualSensorLabel::DrawOutLine()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(!bLoaded) return;
    
    if(this->Width <= 0 || this->Height <=0) return;
    
    Canvas->Lock();
    
    SetBkMode(Canvas->Handle, TRANSPARENT);

    //-------------------------
    // Out-Line
    if(FSenOutLine) {
        if(Transparent) {
            Canvas->Brush->Style = bsClear;
        }
        else {
            Canvas->Brush->Style = bsSolid;
        }
        
        Canvas->Brush->Color = Enabled ? Color : clBtnFace;
        Canvas->Pen->Color   = Enabled ? FSenOutLineColor : clGray;
        Canvas->RoundRect(1, 0, this->Width-1, this->Height-1, FSenOutLineRound, FSenOutLineRound); 
    }
    else {
        if(!Transparent) {
            Canvas->Brush->Style = bsSolid;
            Canvas->Brush->Color = Enabled ? Color : clBtnFace;
            Canvas->Pen->Color   = Enabled ? Color : clBtnFace;
            Canvas->RoundRect(1, 0, this->Width-1, this->Height-1, FSenOutLineRound, FSenOutLineRound); 
        }
    }

    Canvas->Unlock();
    
}

//---------------------------------------------------------------------------

void __fastcall TDualSensorLabel::DrawSensorL()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(!bLoaded) return;

    if(!FSenVisibleL) return;
    if(Width <= 0 || Height <= 0 || FSenWidth <= 0 || FSenHeight <= 0) return;
    
    //Vcl::Graphics::TCanvas* BuffCanvas = NULL;

    int nX1 = FSenOutLine ? FSenOutSpace + 2 : FSenOutSpace;
    int nY1 = 0;
    int nX2 = nX1 + FSenWidth;
    int nY2 = Height;

    Canvas->Lock();

    if(FSenLEDImage) {
        int nSenWidth  = 20;
        int nSenHeight = 20;
        TColor clTransP = clWhite;
        Graphics::TBitmap* pLEDBitmap = NULL;

        if(!Enabled) {
            pLEDBitmap  = pBmpSenDisable;
        }
        else if(FSenOnL) {
            pLEDBitmap  = pBmpSenOn;
        }
        else {
            pLEDBitmap  = pBmpSenOff;
        }

        nSenWidth  = pLEDBitmap->Width;
        nSenHeight = pLEDBitmap->Height;
        clTransP   = pLEDBitmap->Canvas->Pixels[0][0];
    
        nY1 = nSenWidth >= Height ? 0            : ((Height - nSenWidth) / 2);
        //nY2 = nSenWidth >= Height ? Height       : (nY1 + nSenHeight);
        //nX2 = nX1 + nSenWidth;

        TransparentBlt(
            Canvas->Handle,
            nX1,
            nY1,
            nSenWidth,
            nSenHeight,
            pLEDBitmap->Canvas->Handle,
            0,
            0,
            nSenWidth,
            nSenHeight,
            clTransP);


    }
    else {
        SetBkMode(Canvas->Handle, TRANSPARENT);

        if(Enabled) {
            Canvas->Brush->Color = FSenOnL ? FSenOnColor : FSenOffColor;
            Canvas->Pen->Color   = FSenLineColor;
        }
        else {
            Canvas->Brush->Color = clSilver;
            Canvas->Pen->Color   = clGray;
        }
        
        Canvas->Brush->Style = bsSolid;

        switch(FSenType) {
            case slCircle: 
                nY1 = FSenWidth >= Height ? 0            : ((Height - FSenWidth) / 2);
                nY2 = FSenWidth >= Height ? Height       : (nY1 + FSenWidth);
                nX2 = FSenWidth >= Height ? nX1 + Height : nX1 + FSenWidth;
                
                Canvas->Ellipse(nX1, nY1, nX2, nY2); 
                break;
                
            case slRect:   
                nY1 = FSenHeight >= Height ? 0            : ((Height - FSenHeight) / 2);
                nY2 = FSenHeight >= Height ? Height       : (nY1 + FSenHeight);
                
                Canvas->RoundRect(nX1, nY1, nX2, nY2, FSenRectRound, FSenRectRound); 
                break;
        }
    }
    
    Canvas->Unlock();
}
//---------------------------------------------------------------------------

void __fastcall TDualSensorLabel::DrawSensorR()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(!bLoaded) return;

    if(!FSenVisibleR) return;
    if(Width <= 0 || Height <= 0 || FSenWidth <= 0 || FSenHeight <= 0) return;
    
    //Vcl::Graphics::TCanvas* BuffCanvas = NULL;

    int nX1 = FSenOutLine ? (this->Width - FSenWidth - FSenOutSpace - 3) : (this->Width - FSenWidth - FSenOutSpace - 1);
    int nY1 = 0;
    int nX2 = nX1 + FSenWidth;
    int nY2 = Height;

    Canvas->Lock();

    if(FSenLEDImage) {
        int nSenWidth  = 20;
        int nSenHeight = 20;
        TColor clTransP = clWhite;
        Graphics::TBitmap* pLEDBitmap = NULL;

        if(!Enabled) {
            pLEDBitmap  = pBmpSenDisable;
        }
        else if(FSenOnR) {
            pLEDBitmap  = pBmpSenOn;
        }
        else {
            pLEDBitmap  = pBmpSenOff;
        }

        nSenWidth  = pLEDBitmap->Width;
        nSenHeight = pLEDBitmap->Height;
        clTransP   = pLEDBitmap->Canvas->Pixels[0][0];

        nX1 = FSenOutLine ? (this->Width - nSenWidth - FSenOutSpace - 3) : (this->Width - nSenWidth - FSenOutSpace - 1);
        nX2 = nX1 + nSenWidth;
        nY1 = nSenWidth >= Height ? 0            : ((Height - nSenWidth) / 2);

        TransparentBlt(
            Canvas->Handle,
            nX1,
            nY1,
            nSenWidth,
            nSenHeight,
            pLEDBitmap->Canvas->Handle,
            0,
            0,
            nSenWidth,
            nSenHeight,
            clTransP);


    }
    else {
        SetBkMode(Canvas->Handle, TRANSPARENT);

        if(Enabled) {
            Canvas->Brush->Color = FSenOnR ? FSenOnColor : FSenOffColor;
            Canvas->Pen->Color   = FSenLineColor;
        }
        else {
            Canvas->Brush->Color = clSilver;
            Canvas->Pen->Color   = clGray;
        }
        
        Canvas->Brush->Style = bsSolid;

        switch(FSenType) {
            case slCircle: 
                nX1 = FSenWidth >= Height ? (this->Width - Height - 1) : (this->Width - FSenWidth - 1);
                nX1 = FSenOutLine ? (nX1 - FSenOutSpace - 2) : (nX1 - FSenOutSpace);
                nY1 = FSenWidth >= Height ? 0            : ((Height - FSenWidth) / 2);
                nY2 = FSenWidth >= Height ? Height       : (nY1 + FSenWidth);
                nX2 = FSenWidth >= Height ? nX1 + Height : nX1 + FSenWidth;
                
                Canvas->Ellipse(nX1, nY1, nX2, nY2); 
                break;
                
            case slRect:   
                nY1 = FSenHeight >= Height ? 0            : ((Height - FSenHeight) / 2);
                nY2 = FSenHeight >= Height ? Height       : (nY1 + FSenHeight);
                
                Canvas->RoundRect(nX1, nY1, nX2, nY2, FSenRectRound, FSenRectRound); 
                break;
        }
    }
    
    Canvas->Unlock();
}
//---------------------------------------------------------------------------

void __fastcall TDualSensorLabel::DrawTitle()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(!bLoaded) return;

    if(Width <= 0 || Height <= 0 || (Width < FSenWidth + FSenSpace)) return;

    Canvas->Lock();
    
    SetBkMode(Canvas->Handle, TRANSPARENT);

    Canvas->Font->Assign(Font);

    if(!Enabled) {
        Canvas->Font->Color = clGray;
    }

    int nTxtW = Canvas->TextWidth(Caption);
    int nTxtH = Canvas->TextHeight(Caption);

    int nSenWidth = 0;

    if(FSenLEDImage) {
        if(!Enabled)     nSenWidth = pBmpSenDisable->Width;
        else if(FSenOnL) nSenWidth = pBmpSenOn->Width;
        else             nSenWidth = pBmpSenOff->Width;
    }
    else {
        nSenWidth = FSenWidth;
    }
    
    int nStartX = nSenWidth + FSenSpace + FSenOutSpace;
    nStartX = FSenOutLine ? nStartX + 1 : nStartX;
    int nStartY = (Height - nTxtH) / 2;

    switch(Alignment ) {
        case taLeftJustify:     nStartX = nStartX;
            break;
        case taRightJustify:    nStartX = (this->Width - nTxtW - nStartX);
            break;
        case taCenter:          nStartX = ((this->Width - nTxtW) / 2);
            break;
    }

    Canvas->Brush->Style = bsClear;
    Canvas->TextOutA(nStartX, nStartY, Caption);

    Canvas->Unlock();
    
}
//---------------------------------------------------------------------------


void __fastcall TDualSensorLabel::SetSenLineColor(TColor v)
{
    if(FSenLineColor != v) {
        FSenLineColor = v;
        Invalidate();
    }
}

void __fastcall TDualSensorLabel::SetSenOnColor(TColor v)
{
    if(FSenOnColor != v) {
        FSenOnColor = v;
        DrawSensorL();
        DrawSensorR();
    }
}

void __fastcall TDualSensorLabel::SetSenOffColor(TColor v)
{
    if(FSenOffColor != v) {
        FSenOffColor = v;
        DrawSensorL();
        DrawSensorR();
    } 
}

void __fastcall TDualSensorLabel::SetSenType(TSensorLedType  v)
{
    if(FSenType != v) {
        FSenType = v;

        LoadLEDImage(); 
    
        Invalidate();
    }
}

void __fastcall TDualSensorLabel::SetSenWidth(int    v)
{
    if(FSenWidth != v) {
        FSenWidth = v;
        Invalidate();
    }
}

void __fastcall TDualSensorLabel::SetSenHeight(int    v)
{
    if(FSenHeight != v) {
        FSenHeight = v;
        Invalidate();
    }
}

void __fastcall TDualSensorLabel::SetSenRectRound(int v)
{
    if(FSenRectRound != v) {
        FSenRectRound = v;
        Invalidate();
    }
}

void __fastcall TDualSensorLabel::SetSenOnL(bool   v)
{
    if(FSenOnL != v) {
        FSenOnL = v;
        DrawSensorL();
    }
}

void __fastcall TDualSensorLabel::SetSenOnR(bool   v)
{
    if(FSenOnR != v) {
        FSenOnR = v;
        DrawSensorR();
    }
}

void __fastcall TDualSensorLabel::SetSenSpace(int    v)
{
    if(FSenSpace != v) {
        FSenSpace = v;
        Invalidate();
    }
}


void  __fastcall TDualSensorLabel::SetSenLEDImage(bool v)
{
    if(FSenLEDImage != v) {
        FSenLEDImage = v;
        Invalidate();
    }
}

void  __fastcall TDualSensorLabel::SetSenVisibleL(bool v)
{
    if(FSenVisibleL != v) {
        FSenVisibleL = v;
        Invalidate();
    }
}

void  __fastcall TDualSensorLabel::SetSenVisibleR(bool v)
{
    if(FSenVisibleR != v) {
        FSenVisibleR = v;
        Invalidate();
    }
}

void __fastcall TDualSensorLabel::SetSenOutLine(bool  v)
{
    if(FSenOutLine != v) {
        FSenOutLine = v;
        Invalidate();
    }
}

void __fastcall TDualSensorLabel::SetSenOutLineColor(TColor    v)
{
    if(FSenOutLineColor != v) {
        FSenOutLineColor = v;
        Invalidate();
    }
}

void __fastcall TDualSensorLabel::SetSenOutLineRound(int       v)
{
    if(FSenOutLineRound != v) {
        FSenOutLineRound = v;
        Invalidate();
    }
}

void __fastcall TDualSensorLabel::SetSenOutSpace(int       v)
{
    if(FSenOutSpace != v) {
        FSenOutSpace = v;
        Invalidate();
    }
}

void __fastcall TDualSensorLabel::SetSenColorType(TSensorLedColorType  v)
{
    if(FSenColorType != v) {
        FSenColorType = v;

        LoadLEDImage(); 
    
        Invalidate();
    }
}

