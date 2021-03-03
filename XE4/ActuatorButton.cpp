//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "ActuatorButton.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TActuatorButton *)
{
    new TActuatorButton(NULL);
}
//---------------------------------------------------------------------------
__fastcall TActuatorButton::TActuatorButton(TComponent* Owner)
    : TWinControl(Owner)
{
    bLoaded                 = false;
    
    TWinControl::OnResize   = MyResize;

    //--------------------------------

    FAllowAllUp             = false;
    FMargin                 = -1;
    FSpacing                = 4;

    FDualSenLabel           = new TDualSensorLabel(this);
    FSBLeft                 = new TSpeedButton(this);
    FSBRight                = new TSpeedButton(this);

    FDualSenLabel->Parent   = this;
    FSBLeft->Parent         = this;
    FSBRight->Parent        = this;

    FDualSenLabel->ParentFont = false;
    FSBLeft->ParentFont     = true;
    FSBRight->ParentFont    = true;

    FSBLeft->AllowAllUp     = FAllowAllUp;
    FSBRight->AllowAllUp    = FAllowAllUp;

    FSBLeft->GroupIndex     = 1;
    FSBRight->GroupIndex    = 1;

    FSBLeft->OnClick        = MyClick;
    FSBRight->OnClick       = MyClick;

    FCaptionLeft            = "Use";
    FCaptionRight           = "Not Use";

    FColorUp                = clGray;           // 선택되지 않은 상태 색상. ( Enable)
    FColorDisable           = clBtnFace;        // Disable 상태 색상.
    FColorDown              = clRed;            // 사용자가 Mouse Down 중인 상태 색상.
    FColorSelect            = clLime;           // 선택된 상태 색상.

    FGlyphRound             = 2;

    FLayout                 = dbHorizontal;
    FGlyphLayout            = blGlyphTop;

    this->Width             = 200;
    this->Height            = 80;

    //--------------------------------
    BMP_Indicator = new Graphics::TBitmap;

    //--------------------------------
    FSenBGColor         = clBtnFace;
    FSenLineColor       = clBlack ;
    FSenOnColor         = clLime ;
    FSenOffColor        = clSilver ;
    FSenType            = slCircle ;
    FSenColorType       = slColorGreen;
    FSenWidth           = 15 ;
    FSenHeight          = 15 ;
    FSenRectRound       = 3;
    FSenOnL             = false ;
    FSenOnR             = false ;
    FSenSpace           = 5 ;
    FSenLEDImage        = true;
    FSenVisibleL        = true;
    FSenVisibleR        = true;

    FSenOutLine         = true;
    FSenOutLineColor    = clBlack;
    FSenOutLineRound    = 0;
    FSenOutSpace        = 5;
    FHeightTitle        = 30;

    FSenFont            = new Vcl::Graphics::TFont;
    FSenFont->OnChange  = SenFontChanged; // Font가 변경될 경우 이를 감지할 Call Back 함수를 등록한다.


    FGlyph_Left             = new Vcl::Graphics::TBitmap;
    FGlyph_Right            = new Vcl::Graphics::TBitmap;
    FGlyph_Left->OnChange   = GlyphChanged;
    FGlyph_Right->OnChange  = GlyphChanged;
    FGlyph_Left->Width      = 0;
    FGlyph_Left->Height     = 0;
    FGlyph_Right->Width     = 0;
    FGlyph_Right->Height    = 0;

    FNumGlyphs = 4;

    //--------------------------
    if(ComponentState.Contains(csDesigning)) {
        bLoaded = true;
        DisplayUpdate();
    }  

}

__fastcall TActuatorButton::~TActuatorButton()
{
    if(FDualSenLabel)           delete FDualSenLabel;
    if(FSBLeft)                 delete FSBLeft;
    if(FSBRight)                delete FSBRight;
    if(BMP_Indicator)           delete BMP_Indicator;
    if(FGlyph_Left)             delete FGlyph_Left;
    if(FGlyph_Right)            delete FGlyph_Right;
    
}
//---------------------------------------------------------------------------
//namespace ActuatorButton
//{
//    void __fastcall PACKAGE Register()
//    {
//         TComponentClass classes[1] = {__classid(TActuatorButton)};
//         RegisterComponents(L"AZ", classes, 0);
//    }
//}

//---------------------------------------------------------------------------
void __fastcall TActuatorButton::Loaded(void)
{
    TWinControl::Loaded();

    bLoaded = true;

    FDualSenLabel->SetLoaded(true);

    DisplayUpdate();
}
//---------------------------------------------------------------------------
void     __fastcall TActuatorButton::SetEnabled(bool Value)
{
    TWinControl::SetEnabled(Value);

    FDualSenLabel->Enabled  = Value;
    FSBLeft->Enabled        = Value;
    FSBRight->Enabled       = Value;

    //DisplayUpdate();
}
//---------------------------------------------------------------------------
//void     __fastcall TActuatorButton::FontChanged(System::TObject* Sender)
//{
//    DisplayUpdate();
//}

//---------------------------------------------------------------------------
//void    __fastcall TActuatorButton::SetParent(Vcl::Controls::TWinControl* AParent)
//{
//    if(AParent == NULL) return;
//
//
//}

//---------------------------------------------------------------------------
void     __fastcall TActuatorButton::MyResize(TObject *Sender)
{
    if(FOnResize) FOnResize(this);

    DisplayUpdate();
}

//---------------------------------------------------------------------------
void     __fastcall TActuatorButton::MyClick(TObject *Sender)
{
    TSpeedButton * pBtn  =dynamic_cast<TSpeedButton *>(Sender);

    if(pBtn == NULL) return;

    if(FSBLeft->Down)         FState = dbLeft;
    else if(FSBRight->Down)   FState = dbRight;
    else                      FState = dbNone;

    if(FOnClick) FOnClick(this, FState);
}
//---------------------------------------------------------------------------
void __fastcall TActuatorButton::CreateIndicatorImage()
{
    if(BMP_Indicator == NULL) return;

    int nImg1Width  = 0;
    int nImg1Height = 0;

    switch(FGlyphLayout) {
        case blGlyphLeft:
        case blGlyphRight:
            nImg1Width  = FSBLeft->Width  / 6;
            nImg1Height = FSBLeft->Height / 2;
            nImg1Width  = nImg1Width > 20 ? 20 : nImg1Width;
            break;

        case blGlyphTop:
        case blGlyphBottom:
        default:
            nImg1Width  = FSBLeft->Width  / 2;
            nImg1Height = FSBLeft->Height / 6;
            nImg1Height = nImg1Height > 20 ? 20 : nImg1Height;
            break;
    }

    nImg1Height = nImg1Height < 6 ? 6 : nImg1Height;

    BMP_Indicator->Width  = nImg1Width * 4;
    BMP_Indicator->Height = nImg1Height;

    BMP_Indicator->Canvas->Brush->Color = clBtnFace;
    BMP_Indicator->Canvas->FillRect(Rect(0,0,BMP_Indicator->Width, BMP_Indicator->Height));

    BMP_Indicator->Canvas->Pen->Color   = FColorLine;       // Line.
    BMP_Indicator->Canvas->Brush->Color = FColorUp;         // Normal
    BMP_Indicator->Canvas->RoundRect(1, 1, nImg1Width-2, nImg1Height-1, FGlyphRound, FGlyphRound);

    BMP_Indicator->Canvas->Brush->Color = FColorDisable;    // Disable
    BMP_Indicator->Canvas->RoundRect(nImg1Width   + 1,  1, nImg1Width*2 -2, nImg1Height-1, FGlyphRound, FGlyphRound);

    BMP_Indicator->Canvas->Brush->Color = FColorDown;       // Down Click
    BMP_Indicator->Canvas->RoundRect(nImg1Width*2 + 1,  1, nImg1Width*3 -2, nImg1Height-1, FGlyphRound, FGlyphRound);

    BMP_Indicator->Canvas->Brush->Color = FColorSelect;     // Selected
    BMP_Indicator->Canvas->RoundRect(nImg1Width*3 + 1,  1, nImg1Width*4 -2, nImg1Height-1, FGlyphRound, FGlyphRound);


    if(FGlyph_Left != NULL && ((FGlyph_Left->Width > 0) && (FGlyph_Left->Height > 0))) {
        FSBLeft->Glyph->Assign(FGlyph_Left);
        FSBLeft->NumGlyphs = FNumGlyphs;
    }
    else {
        FSBLeft->Glyph->Assign(BMP_Indicator);
        FSBLeft->NumGlyphs = 4;
    }

    if(FGlyph_Right != NULL && ((FGlyph_Right->Width > 0) && (FGlyph_Right->Height > 0))) {
        FSBRight->Glyph->Assign(FGlyph_Right);
    }
    else {
        FSBRight->Glyph->Assign(BMP_Indicator);
        FSBRight->NumGlyphs = 4;
    }    

}
//---------------------------------------------------------------------------
void __fastcall TActuatorButton::DisplayUpdate()
{   
    if(!ComponentState.Contains(csDesigning) && !Visible) return;
    if(!bLoaded) return;
    
    int nWidth = FLayout == dbHorizontal ? this->Width / 2 : this->Width;

    //FSBLeft->Font->Assign(this->Font);
    //FSBRight->Font->Assign(this->Font);
    
    FDualSenLabel->Align   = alTop;

    if(FLayout == dbHorizontal) {
        FSBLeft->Align   = alLeft;
        FSBRight->Align  = alClient;

        FSBLeft->Width   = nWidth;
        FSBRight->Width  = nWidth;
    }
    else {
        FSBLeft->Align   = alTop;
        FSBLeft->Top     = FDualSenLabel->Height;
        FSBRight->Align  = alClient;

        FSBLeft->Height  = (this->Height - FDualSenLabel->Height) / 2;
        FSBRight->Height = (this->Height - FDualSenLabel->Height) / 2;
    }

    FSBLeft->Layout  = FGlyphLayout;
    FSBRight->Layout = FGlyphLayout;

    FSBLeft->NumGlyphs  = 4;
    FSBRight->NumGlyphs = 4;

    FSBLeft->Caption  =  FCaptionLeft;
    FSBRight->Caption =  FCaptionRight;

    CreateIndicatorImage();

    // -------------------------------------
    FDualSenLabel->Font               = FSenFont;
    FDualSenLabel->Color              = FSenBGColor;
    FDualSenLabel->SenLineColor       = FSenLineColor       ;
    FDualSenLabel->SenOnColor         = FSenOnColor         ;
    FDualSenLabel->SenOffColor        = FSenOffColor        ;
    FDualSenLabel->SenType            = FSenType            ;
    FDualSenLabel->SenColorType       = FSenColorType       ;
    FDualSenLabel->SenWidth           = FSenWidth           ;
    FDualSenLabel->SenHeight          = FSenHeight          ;
    FDualSenLabel->SenRectRound       = FSenRectRound       ;
    FDualSenLabel->SenOnL             = FSenOnL             ;
    FDualSenLabel->SenOnR             = FSenOnR             ;
    FDualSenLabel->SenSpace           = FSenSpace           ;    
    FDualSenLabel->SenLEDImage        = FSenLEDImage        ;

    FDualSenLabel->SenOutLine         = FSenOutLine;
    FDualSenLabel->SenOutLineColor    = FSenOutLineColor;
    FDualSenLabel->SenOutLineRound    = FSenOutLineRound;
    FDualSenLabel->SenOutSpace        = FSenOutSpace;
    FDualSenLabel->Caption            = Caption;
    FDualSenLabel->Height             = FHeightTitle;


//    AnsiString sTemp = IntToStr(this->Font->Size);
//    MessageBox(NULL, sTemp.c_str(), "", MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TActuatorButton::SetCaptionLeft(TCaption s)
{
    if(FCaptionLeft != s) {
        FCaptionLeft = s;
        DisplayUpdate();
    }
}

void __fastcall TActuatorButton::SetCaptionRight(TCaption s)
{
    if(FCaptionRight != s) {
        FCaptionRight = s;
        DisplayUpdate();
    }
}

void __fastcall TActuatorButton::SetState(TDualButtonState e)
{
    if(FState != e) {
        FState = e;
        switch(FState) {
            case dbNone:
                FSBLeft->Down  = false;
                FSBRight->Down = false;
                break;
            case dbLeft:
                FSBLeft->Down  = true;
                FSBRight->Down = false;
                break;
            case dbRight:
                FSBLeft->Down  = false;
                FSBRight->Down = true;
                break;
        }
    }
}
void __fastcall TActuatorButton::SetAllowAllUp(bool b)
{
    if(FAllowAllUp != b) {
        FAllowAllUp = b;

        FSBLeft->AllowAllUp     = FAllowAllUp;
        FSBRight->AllowAllUp    = FAllowAllUp;

        DisplayUpdate();
    }
}

void __fastcall TActuatorButton::SetMargin(int n)
{
    if(FMargin != n) {
        FMargin = n;

        FSBLeft->Margin = FMargin;
        FSBRight->Margin = FMargin;
    }
}

void __fastcall TActuatorButton::SetSpacing(int n)
{
    if(FSpacing != n) {
        FSpacing = n;

        FSBLeft->Spacing = FSpacing;
        FSBRight->Spacing = FSpacing;
    }
}

void __fastcall TActuatorButton::SetColor(int Index, TColor c)
{
    switch(Index) {
        case 1: FColorUp = c;       break;
        case 2: FColorDisable = c;  break;
        case 3: FColorDown = c;     break;
        case 4: FColorSelect = c;   break;
        case 5: FColorLine = c;     break;
    }

    DisplayUpdate();
}

void __fastcall TActuatorButton::SetLayout(TDualButtonLayout e)
{
    if(FLayout != e) {
        FLayout = e;

        DisplayUpdate();
    }
}

void __fastcall TActuatorButton::SetGlyphLayout(TButtonLayout e)
{
    if(FGlyphLayout != e) {
        FGlyphLayout = e;

        DisplayUpdate();
    }
}

void __fastcall TActuatorButton::SetGlyphRound(int n)
{
    if(FGlyphRound != n) {
        FGlyphRound = n;
        FGlyphRound = FGlyphRound > 10 ? 10 : FGlyphRound;

        DisplayUpdate();
    }
}
//---------------------------------------------------------------------------
void __fastcall TActuatorButton::SetSenBGColor(TColor v)
{
    if(FSenBGColor != v) {
        FSenBGColor = v;
        FDualSenLabel->Color = v;
    }
}


void __fastcall TActuatorButton::SetSenLineColor(TColor v)
{
    if(FSenLineColor != v) {
        FSenLineColor = v;
        FDualSenLabel->SenLineColor = v;
    }
}

void __fastcall TActuatorButton::SetSenOnColor(TColor v)
{
    if(FSenOnColor != v) {
        FSenOnColor = v;
        FDualSenLabel->SenOnColor = v;
    }
}

void __fastcall TActuatorButton::SetSenOffColor(TColor v)
{
    if(FSenOffColor != v) {
        FSenOffColor = v;
        FDualSenLabel->SenOffColor = v;
    }
}

void __fastcall TActuatorButton::SetSenType(TSensorLedType  v)
{
    if(FSenType != v) {
        FSenType = v;
        FDualSenLabel->SenType = v;
    }
}

void __fastcall TActuatorButton::SetSenColorType(TSensorLedColorType  v)
{
    if(FSenColorType != v) {
        FSenColorType = v;
        FDualSenLabel->SenColorType = v;
    }
}

void __fastcall TActuatorButton::SetSenWidth(int    v)
{
    if(FSenWidth != v) {
        FSenWidth = v;
        FDualSenLabel->SenWidth = v;
    }
}

void __fastcall TActuatorButton::SetSenHeight(int    v)
{
    if(FSenHeight != v) {
        FSenHeight = v;
        FDualSenLabel->SenHeight = v;
    }
}

void __fastcall TActuatorButton::SetSenRectRound(int v)
{
    if(FSenRectRound != v) {
        FSenRectRound = v;
        FDualSenLabel->SenRectRound = v;
    }
}

void __fastcall TActuatorButton::SetSenOnL(bool   v)
{
    if(FSenOnL != v) {
        FSenOnL = v;
        FDualSenLabel->SenOnL = v;
    }
}

void __fastcall TActuatorButton::SetSenOnR(bool   v)
{
    if(FSenOnR != v) {
        FSenOnR = v;
        FDualSenLabel->SenOnR = v;
    }
}

void __fastcall TActuatorButton::SetSenSpace(int    v)
{
    if(FSenSpace != v) {
        FSenSpace = v;
        FDualSenLabel->SenSpace = v;
    }
}

void __fastcall TActuatorButton::SetSenLEDImage(bool v)
{
    if(FSenLEDImage != v) {
        FSenLEDImage = v;
        FDualSenLabel->SenLEDImage = v;
    }
}

void  __fastcall TActuatorButton::SetSenVisibleL(bool v)
{
    if(FSenVisibleL != v) {
        FSenVisibleL = v;
        FDualSenLabel->SenVisibleL = v;
    }
}

void  __fastcall TActuatorButton::SetSenVisibleR(bool v)
{
    if(FSenVisibleR != v) {
        FSenVisibleR = v;
        FDualSenLabel->SenVisibleR = v;
    }
}


void __fastcall TActuatorButton::SetSenOutLine(bool  v)
{
    if(FSenOutLine != v) {
        FSenOutLine = v;
        FDualSenLabel->SenOutLine = v;
    }
}

void __fastcall TActuatorButton::SetSenOutLineColor(TColor    v)
{
    if(FSenOutLineColor != v) {
        FSenOutLineColor = v;
        FDualSenLabel->SenOutLineColor = v;
    }
}

void __fastcall TActuatorButton::SetSenOutLineRound(int       v)
{
    if(FSenOutLineRound != v) {
        FSenOutLineRound = v;
        FDualSenLabel->SenOutLineRound = v;
    }
}

void __fastcall TActuatorButton::SetSenOutSpace(int       v)
{
    if(FSenOutSpace != v) {
        FSenOutSpace = v;
        FDualSenLabel->SenOutSpace = v;
    }
}


void __fastcall TActuatorButton::SetHeightTitle(int v)
{
    if(FHeightTitle != v) {
        FHeightTitle = v;
        DisplayUpdate();
    }
}

void  __fastcall TActuatorButton::SetMyCaption(TCaption v)
{
    if(Caption != v) {
        TWinControl::Caption = v;
        FDualSenLabel->Caption = v;
    }
}

void __fastcall TActuatorButton::SetSenFont(Vcl::Graphics::TFont * p)
{
    FSenFont->Assign(p);
    FDualSenLabel->Font = FSenFont;
}

void     __fastcall TActuatorButton::SenFontChanged(System::TObject* Sender)
{
    FDualSenLabel->Font = FSenFont; 
    FDualSenLabel->Paint();
}

//---------------------------------------------------------------------------
void __fastcall TActuatorButton::SetGlyph(int Index, Vcl::Graphics::TBitmap* Value)
{
    if(Value == NULL) {
        if(Index == 1) {
            FGlyph_Left->Width  = 0;
            FGlyph_Left->Height = 0;
        }
        else {
            FGlyph_Right->Width  = 0;
            FGlyph_Right->Height = 0;
        }
    }
    else {
        if(Index == 1) {
            FGlyph_Left->Assign(Value);
        }
        else {
            FGlyph_Right->Assign(Value);
        }        
    }

    DisplayUpdate();
}

Vcl::Graphics::TBitmap* __fastcall TActuatorButton::GetGlyph(int Index)
{
    if(Index == 1) {
        return FGlyph_Left;
    }
    else {
        return FGlyph_Right;
    }

}

TNumGlyphs __fastcall TActuatorButton::GetNumGlyphs(void)
{
    return FNumGlyphs;
}

void __fastcall TActuatorButton::SetNumGlyphs(TNumGlyphs Value)
{
    if(FNumGlyphs != Value) {
        FNumGlyphs = Value;
        //CreateIndicatorImage();
        DisplayUpdate();
    }
}


void __fastcall TActuatorButton::GlyphChanged(System::TObject* Sender)
{
    //CreateIndicatorImage();
    DisplayUpdate();
}
//---------------------------------------------------------------------------

