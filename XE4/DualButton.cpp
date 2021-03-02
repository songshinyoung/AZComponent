//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "DualButton.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TDualButton *)
{
    new TDualButton(NULL);
}
//---------------------------------------------------------------------------
__fastcall TDualButton::TDualButton(TComponent* Owner)
    : TWinControl(Owner)
{
    bLoaded                 = false;
    TWinControl::OnResize   = MyResize;

    FAllowAllUp             = false;
    FMargin                 = -1;
    FSpacing                = 4;

    FSBLeft                 = new TSpeedButton(this);
    FSBRight                = new TSpeedButton(this);

    FSBLeft->Parent         = this;
    FSBRight->Parent        = this;

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
    FColorDisableSelected   = clGray;           // Disable 상태 색상.(선택 버튼)
    FColorDown              = clRed;            // 사용자가 Mouse Down 중인 상태 색상.
    FColorSelect            = clLime;           // 선택된 상태 색상.

    FGlyphRound             = 2;

    FLayout                 = dbHorizontal;
    FGlyphLayout            = blGlyphTop;

    this->Width             = 200;
    this->Height            = 50;

    //--------------------------------
    BMP_Indicator           = new Graphics::TBitmap;
    BMP_IndicatorSelected   = new Graphics::TBitmap;

    //Font->OnChange = FontChanged;

    FGlyph_Left  = new Vcl::Graphics::TBitmap;
    FGlyph_Right = new Vcl::Graphics::TBitmap;
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

__fastcall TDualButton::~TDualButton()
{
    if(FSBLeft)                 delete FSBLeft;
    if(FSBRight)                delete FSBRight;
    if(BMP_Indicator)           delete BMP_Indicator;
    if(BMP_IndicatorSelected)   delete BMP_IndicatorSelected;
    if(FGlyph_Left)             delete FGlyph_Left;
    if(FGlyph_Right)            delete FGlyph_Right;
}
//---------------------------------------------------------------------------
//namespace Dualbutton
//{
//    void __fastcall PACKAGE Register()
//    {
//         TComponentClass classes[1] = {__classid(TDualButton)};
//         RegisterComponents(L"AZ", classes, 0);
//    }
//}
//---------------------------------------------------------------------------
void __fastcall TDualButton::Loaded(void)
{
    TWinControl::Loaded();

    bLoaded = true;

    DisplayUpdate();
}
//---------------------------------------------------------------------------
void     __fastcall TDualButton::SetEnabled(bool Value)
{
    TWinControl::SetEnabled(Value);

    FSBLeft->Enabled  = Value;
    FSBRight->Enabled = Value;

    //DisplayUpdate();
}
//---------------------------------------------------------------------------
//void     __fastcall TDualButton::FontChanged(System::TObject* Sender)
//{
//    DisplayUpdate();
//}
//---------------------------------------------------------------------------
//void    __fastcall TDualButton::SetParent(Vcl::Controls::TWinControl* AParent)
//{
//    if(AParent == NULL) return;
//
//
//}
//---------------------------------------------------------------------------
void     __fastcall TDualButton::MyResize(TObject *Sender)
{
    if(FOnResize) FOnResize(this);

    DisplayUpdate();
}
//---------------------------------------------------------------------------
void     __fastcall TDualButton::MyClick(TObject *Sender)
{
    TSpeedButton * pBtn  =dynamic_cast<TSpeedButton *>(Sender);

    if(pBtn == NULL) return;

    if(FSBLeft->Down)         FState = dbLeft;
    else if(FSBRight->Down)   FState = dbRight;
    else                      FState = dbNone;

    SetBtnIndicatorImg(FState);

    if(FOnClick) FOnClick(this, FState);
}
//---------------------------------------------------------------------------
void __fastcall TDualButton::CreateIndicatorImage()
{
    if(BMP_Indicator == NULL || BMP_IndicatorSelected == NULL) return;

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

    //-------------------------------------------------
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


    //-------------------------------------------------
    BMP_IndicatorSelected->Width  = nImg1Width * 4;
    BMP_IndicatorSelected->Height = nImg1Height;

    BMP_IndicatorSelected->Canvas->Brush->Color = clBtnFace;
    BMP_IndicatorSelected->Canvas->FillRect(Rect(0,0,BMP_IndicatorSelected->Width, BMP_IndicatorSelected->Height));

    BMP_IndicatorSelected->Canvas->Pen->Color   = FColorLine;               // Line.
    BMP_IndicatorSelected->Canvas->Brush->Color = FColorUp;                 // Normal
    BMP_IndicatorSelected->Canvas->RoundRect(1, 1, nImg1Width-2, nImg1Height-1, FGlyphRound, FGlyphRound);

    BMP_IndicatorSelected->Canvas->Brush->Color = FColorDisableSelected;    // Disable
    BMP_IndicatorSelected->Canvas->RoundRect(nImg1Width   + 1,  1, nImg1Width*2 -2, nImg1Height-1, FGlyphRound, FGlyphRound);

    BMP_IndicatorSelected->Canvas->Brush->Color = FColorDown;               // Down Click
    BMP_IndicatorSelected->Canvas->RoundRect(nImg1Width*2 + 1,  1, nImg1Width*3 -2, nImg1Height-1, FGlyphRound, FGlyphRound);

    BMP_IndicatorSelected->Canvas->Brush->Color = FColorSelect;             // Selected
    BMP_IndicatorSelected->Canvas->RoundRect(nImg1Width*3 + 1,  1, nImg1Width*4 -2, nImg1Height-1, FGlyphRound, FGlyphRound);

    SetBtnIndicatorImg(FState);

}
//---------------------------------------------------------------------------
void __fastcall TDualButton::SetBtnIndicatorImg(TDualButtonState eState)
{
    switch(eState) {
        case dbLeft:
            if(FGlyph_Left != NULL && ((FGlyph_Left->Width > 0) && (FGlyph_Left->Height > 0))) {
                FSBLeft->Glyph->Assign(FGlyph_Left);
                FSBLeft->NumGlyphs = FNumGlyphs;
            }
            else {
                FSBLeft->Glyph->Assign(BMP_IndicatorSelected);
                FSBLeft->NumGlyphs = 4;
            }

            if(FGlyph_Right != NULL && ((FGlyph_Right->Width > 0) && (FGlyph_Right->Height > 0))) {
                FSBRight->Glyph->Assign(FGlyph_Right);
            }
            else {
                FSBRight->Glyph->Assign(BMP_Indicator);
                FSBRight->NumGlyphs = 4;
            }
            break;
        
        case dbRight:
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
                FSBRight->NumGlyphs = FNumGlyphs;
            }
            else {
                FSBRight->Glyph->Assign(BMP_IndicatorSelected);
                FSBRight->NumGlyphs = 4;
            }        
            break;
        
        default:
        case dbNone:
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
                FSBRight->NumGlyphs = FNumGlyphs;
            }
            else {
                FSBRight->Glyph->Assign(BMP_Indicator);
                FSBRight->NumGlyphs = 4;
            }           
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TDualButton::DisplayUpdate()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(!bLoaded) return;
    
    int nWidth = FLayout == dbHorizontal ? this->Width / 2 : this->Width;

    //FSBLeft->Font->Assign(this->Font);
    //FSBRight->Font->Assign(this->Font);

    if(FLayout == dbHorizontal) {
        FSBLeft->Align   = alLeft;
        FSBRight->Align  = alClient;

        FSBLeft->Width   = nWidth;
        FSBRight->Width  = nWidth;
    }
    else {
        FSBLeft->Align   = alTop;
        FSBRight->Align  = alClient;

        FSBLeft->Height  = this->Height / 2;
        FSBRight->Height = this->Height / 2;
    }

    FSBLeft->Layout  = FGlyphLayout;
    FSBRight->Layout = FGlyphLayout;

    FSBLeft->NumGlyphs  = 4;
    FSBRight->NumGlyphs = 4;

    FSBLeft->Caption  =  FCaptionLeft;
    FSBRight->Caption =  FCaptionRight;

    CreateIndicatorImage();

//    AnsiString sTemp = IntToStr(this->Font->Size);
//    MessageBox(NULL, sTemp.c_str(), "", MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TDualButton::SetCaptionLeft(TCaption s)
{
    if(FCaptionLeft != s) {
        FCaptionLeft = s;
        DisplayUpdate();
    }
}

void __fastcall TDualButton::SetCaptionRight(TCaption s)
{
    if(FCaptionRight != s) {
        FCaptionRight = s;
        DisplayUpdate();
    }
}

void __fastcall TDualButton::SetState(TDualButtonState e)
{
    if(FState != e) {
        FState = e;

        SetBtnIndicatorImg(FState);
            
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
void __fastcall TDualButton::SetAllowAllUp(bool b)
{
    if(FAllowAllUp != b) {
        FAllowAllUp = b;

        FSBLeft->AllowAllUp     = FAllowAllUp;
        FSBRight->AllowAllUp    = FAllowAllUp;

        DisplayUpdate();
    }
}

void __fastcall TDualButton::SetMargin(int n)
{
    if(FMargin != n) {
        FMargin = n;

        FSBLeft->Margin = FMargin;
        FSBRight->Margin = FMargin;
    }
}

void __fastcall TDualButton::SetSpacing(int n)
{
    if(FSpacing != n) {
        FSpacing = n;

        FSBLeft->Spacing = FSpacing;
        FSBRight->Spacing = FSpacing;
    }
}

void __fastcall TDualButton::SetColor(int Index, TColor c)
{
    switch(Index) {
        case 1: FColorUp                = c;  break;
        case 2: FColorDisable           = c;  break;
        case 3: FColorDown              = c;  break;
        case 4: FColorSelect            = c;  break;
        case 5: FColorLine              = c;  break;
        case 6: FColorDisableSelected   = c;  break;
    }

    DisplayUpdate();
}

void __fastcall TDualButton::SetLayout(TDualButtonLayout e)
{
    if(FLayout != e) {
        FLayout = e;

        DisplayUpdate();
    }
}

void __fastcall TDualButton::SetGlyphLayout(TButtonLayout e)
{
    if(FGlyphLayout != e) {
        FGlyphLayout = e;

        DisplayUpdate();
    }
}

void __fastcall TDualButton::SetGlyphRound(int n)
{
    if(FGlyphRound != n) {
        FGlyphRound = n;
        FGlyphRound = FGlyphRound > 10 ? 10 : FGlyphRound;

        DisplayUpdate();
    }
}
//---------------------------------------------------------------------------
void __fastcall TDualButton::SetGlyph(int Index, Vcl::Graphics::TBitmap* Value)
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

Vcl::Graphics::TBitmap* __fastcall TDualButton::GetGlyph(int Index)
{
    if(Index == 1) {
        return FGlyph_Left;
    }
    else {
        return FGlyph_Right;
    }

}

TNumGlyphs __fastcall TDualButton::GetNumGlyphs(void)
{
    return FNumGlyphs;
}

void __fastcall TDualButton::SetNumGlyphs(TNumGlyphs Value)
{
    if(FNumGlyphs != Value) {
        FNumGlyphs = Value;
        //CreateIndicatorImage();
        DisplayUpdate();
    }
}


void __fastcall TDualButton::GlyphChanged(System::TObject* Sender)
{
    //CreateIndicatorImage();
    DisplayUpdate();
}

