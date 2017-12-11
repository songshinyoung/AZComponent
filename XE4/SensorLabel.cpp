//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "SensorLabel.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TSensorLabel *)
{
    new TSensorLabel(NULL);
}
//---------------------------------------------------------------------------
__fastcall TSensorLabel::TSensorLabel(TComponent* Owner)
    : TCustomLabel(Owner)
{
    bLoaded         = false;
    
    pBitmapBase     = NULL;

    Color           = clBtnFace;
    AutoSize        = false;
    Transparent     = true;
    ParentColor     = false;
    Width           = 200;
    Height          = 20;

    FNumFont           = new Vcl::Graphics::TFont;
    FNumFont->OnChange = NumFontChanged; // Font가 변경될 경우 이를 감지할 Call Back 함수를 등록한다.
    //FNumFont->Name     = "Arial";
    //FNumFont->Size     = 10;

    FNumVisible     = true ;
    FNumber         = 0 ;
    FNumBGColor     = clTeal ;
    FNumLineColor   = clBlack ;
    FNumRectRound   = 0 ;
    FNumWidth       = 25 ;
    FNumSpace       = 5 ;
    FSenLineColor   = clBlack ;
    FSenOnColor     = clLime ;
    FSenOffColor    = clSilver ;
    FSenType        = slCircle ;
    FSenWidth       = 20 ;
    FSenHeight      = 20 ;
    FSenRectRound   = 3;
    FSenOn          = false ;
    FSenSpace       = 5 ;    
    FSenLEDImage    = true;

    pBmpSenOn       = new Graphics::TBitmap;
    pBmpSenOff      = new Graphics::TBitmap;
    pBmpSenDisable  = new Graphics::TBitmap;

    pBmpSenOn       ->LoadFromResourceName((int)HInstance, "SensorLED_On");
    pBmpSenOff      ->LoadFromResourceName((int)HInstance, "SensorLED_Off");
    pBmpSenDisable  ->LoadFromResourceName((int)HInstance, "SensorLED_Disable");

    //--------------------------
    if(ComponentState.Contains(csDesigning)) {
        bLoaded = true;
    }      
}

__fastcall TSensorLabel::~TSensorLabel()
{
    if(pBitmapBase)     delete pBitmapBase;
    if(FNumFont)        delete FNumFont;

    if(pBmpSenOn)       delete pBmpSenOn;
    if(pBmpSenOff)      delete pBmpSenOff;
    if(pBmpSenDisable)  delete pBmpSenDisable;
    
}

//---------------------------------------------------------------------------
//namespace Titlelabel
//{
//    void __fastcall PACKAGE Register()
//    {
//         TComponentClass classes[1] = {__classid(TSensorLabel)};
//         RegisterComponents(L"AZ", classes, 0);
//    }
//}

//---------------------------------------------------------------------------

void __fastcall TSensorLabel::Loaded(void)
{
    TGraphicControl::Loaded();

    bLoaded = true;
    
    Invalidate();
}


void     __fastcall TSensorLabel::SetEnabled(bool Value)
{
    if(Enabled != Value) {
        TCustomLabel::SetEnabled(Value);
        Invalidate();
    }

}

void __fastcall TSensorLabel::Paint()
{

    DrawNumber();

    DrawSensor();

    DrawTitle();

}

//---------------------------------------------------------------------------

void __fastcall TSensorLabel::DrawNumber()
{
    if(!bLoaded) return;
    if(!FNumVisible || Width <= 0 || Height <= 0 || FNumWidth <= 0) return;
    
    Vcl::Graphics::TCanvas* BuffCanvas = NULL;
    int nTxtW = 0, nTxtH = 0;

    int nRoundWidth  = FNumWidth;       // Unit과의 간격을 유지하는 크기인 Space 공간을 제외한 크기.
    int nRoundHeight = Height;

    Canvas->Lock();
    
    SetBkMode(Canvas->Handle, TRANSPARENT);

    Canvas->Brush->Color = FNumBGColor;
    Canvas->Brush->Style = bsSolid;
    Canvas->Pen->Color   = FNumLineColor;
    Canvas->RoundRect(0, 0, nRoundWidth, nRoundHeight, FNumRectRound, FNumRectRound); 
    
    AnsiString sNum = AnsiString(FNumber);

    Canvas->Font->Assign(FNumFont);

    nTxtW = Canvas->TextWidth(sNum);
    nTxtH = Canvas->TextHeight(sNum);

    Canvas->TextOutA( (nRoundWidth - nTxtW) / 2, (nRoundHeight - nTxtH)/2, sNum);

    if(FNumBGColor != FNumLineColor) {
        Canvas->Brush->Style = bsClear;
        Canvas->Pen->Color   = FNumLineColor;
        Canvas->RoundRect(0, 0, nRoundWidth, nRoundHeight, FNumRectRound, FNumRectRound); 
    }

//    switch(Title->Align) {
//        case caLeft:
//            Canvas->Draw(0, 0, tmpBitmap1);
//            break;
//            
//        case caRight:
//            Canvas->Draw(FEditWidth,0,tmpBitmap1);
//            break;
//            
//        case caTop:
//            Canvas->Draw(0,0,tmpBitmap1);
//            break;
//            
//        case caBottom:
//            Canvas->Draw(0,FEditHeight,tmpBitmap1);                
//            break;
//    }

    Canvas->Unlock();

}
//---------------------------------------------------------------------------

void __fastcall TSensorLabel::DrawSensor()
{
    if(!bLoaded) return;

    if(Width <= 0 || Height <= 0 || FSenWidth <= 0 || FSenHeight <= 0) return;
    
    Vcl::Graphics::TCanvas* BuffCanvas = NULL;

    int nRoundWidth  = FNumWidth;       // Unit과의 간격을 유지하는 크기인 Space 공간을 제외한 크기.
    int nRoundHeight = Height;

    int nX1 = FNumWidth + FNumSpace;
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
        else if(FSenOn) {
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

        Canvas->Brush->Color = FSenOn ? FSenOnColor : FSenOffColor;
        Canvas->Brush->Style = bsSolid;
        Canvas->Pen->Color   = FSenLineColor;

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

void __fastcall TSensorLabel::DrawTitle()
{
    if(!bLoaded) return;

    if(Width <= 0 || Height <= 0 || (Width < FNumWidth + FNumSpace + FSenWidth + FSenSpace)) return;

    Canvas->Lock();
    
    SetBkMode(Canvas->Handle, TRANSPARENT);

    Canvas->Font->Assign(Font);

    int nTxtW = Canvas->TextWidth(Caption);
    int nTxtH = Canvas->TextHeight(Caption);

    int nSenWidth = 0;

    if(FSenLEDImage) {
        if(!Enabled)    nSenWidth = pBmpSenDisable->Width;
        else if(FSenOn) nSenWidth = pBmpSenOn->Width;
        else            nSenWidth = pBmpSenOff->Width;
    }
    else {
        nSenWidth = FSenWidth;
    }
    
    int nStartX = FNumWidth + FNumSpace + nSenWidth + FSenSpace;
    int nStartY = (Height - nTxtH) / 2;

    switch(Alignment ) {
        case taLeftJustify:     nStartX = nStartX;
            break;
        case taRightJustify:    nStartX = (this->Width - nTxtW - 2);
            break;
        case taCenter:          nStartX = nStartX + ((this->Width - nStartX - nTxtW) / 2);
            break;
    }

    Canvas->Brush->Style = bsClear;
    Canvas->TextOutA(nStartX, nStartY, Caption);

    Canvas->Unlock();
    
}
//---------------------------------------------------------------------------
void     __fastcall TSensorLabel::NumFontChanged(System::TObject* Sender)
{
    Invalidate();
}


void    __fastcall  TSensorLabel::SetNumFont(Vcl::Graphics::TFont * p)
{
    FNumFont->Assign(p);
    Invalidate();
}

void __fastcall TSensorLabel::SetNumVisible(bool b)
{
    if(FNumVisible != b) {
        FNumVisible = b;
        Invalidate();
    }
}

void __fastcall TSensorLabel::SetNumber(int    v)
{
    if(FNumber != v) {
        FNumber = v;
        Invalidate();
    }
}

void __fastcall TSensorLabel::SetNumBGColor(TColor v)
{
    if(FNumBGColor != v) {
        FNumBGColor = v;
        Invalidate();
    }
}

void __fastcall TSensorLabel::SetNumLineColor(TColor v)
{
    if(FNumLineColor != v) {
        FNumLineColor = v;
        Invalidate();
    }
}

void __fastcall TSensorLabel::SetNumRectRound(int    v)
{
    if(FNumRectRound != v) {
        FNumRectRound = v;
        Invalidate();
    }
}

void __fastcall TSensorLabel::SetNumWidth(int    v)
{
    if(FNumWidth != v) {
        FNumWidth = v;
        Invalidate();
    }
}

void __fastcall TSensorLabel::SetNumSpace(int    v)
{
    if(FNumSpace != v) {
        FNumSpace = v;
        Invalidate();
    }
}

void __fastcall TSensorLabel::SetSenLineColor(TColor v)
{
    if(FSenLineColor != v) {
        FSenLineColor = v;
        Invalidate();
    }
}

void __fastcall TSensorLabel::SetSenOnColor(TColor v)
{
    if(FSenOnColor != v) {
        FSenOnColor = v;
        DrawSensor();
    }
}

void __fastcall TSensorLabel::SetSenOffColor(TColor v)
{
    if(FSenOffColor != v) {
        FSenOffColor = v;
        DrawSensor();
    } 
}

void __fastcall TSensorLabel::SetSenType(TSensorLedType  v)
{
    if(FSenType != v) {
        FSenType = v;
        Invalidate();
    }
}

void __fastcall TSensorLabel::SetSenWidth(int    v)
{
    if(FSenWidth != v) {
        FSenWidth = v;
        Invalidate();
    }
}

void __fastcall TSensorLabel::SetSenHeight(int    v)
{
    if(FSenHeight != v) {
        FSenHeight = v;
        Invalidate();
    }
}

void __fastcall TSensorLabel::SetSenRectRound(int v)
{
    if(FSenRectRound != v) {
        FSenRectRound = v;
        Invalidate();
    }
}

void __fastcall TSensorLabel::SetSenOn(bool   v)
{
    if(FSenOn != v) {
        FSenOn = v;
        DrawSensor();
    }
}

void __fastcall TSensorLabel::SetSenSpace(int    v)
{
    if(FSenSpace != v) {
        FSenSpace = v;
        Invalidate();
    }
}


void  __fastcall TSensorLabel::SetSenLEDImage(bool v)
{
    if(FSenLEDImage != v) {
        FSenLEDImage = v;
        Invalidate();
    }
}

