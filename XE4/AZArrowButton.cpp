//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "AZArrowButton.h"
#include "frmNumPad_AZ.h"
#include "GlobalFunction_AZ.h"

#pragma package(smart_init)

#pragma resource "TAZPanel_resources.res"

//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAZArrowButton *)
{
    new TAZArrowButton(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAZArrowButton::TAZArrowButton(TComponent* Owner)
    : TWinControl(Owner)
{
    TWinControl::OnResize   = MyResize;

    Width  = 200;
    Height = 200;

    for(int i=0; i<abMax; i++) {
        FBitBtn[i]               = new TBitBtn(this);
        FBitBtn[i]->Parent       = this;
        FBitBtn[i]->Visible      = true;
        FBitBtn[i]->Caption      = "X";
        FBitBtn[i]->ParentFont   = true;
        FBitBtn[i]->NumGlyphs    = 4;
        FBitBtn[i]->OnClick      = MyClick;
        FBitBtn[i]->OnMouseDown  = MyMouseDown;
        FBitBtn[i]->OnMouseLeave = MyMouseLeave;
        FBitBtn[i]->OnMouseUp    = MyMouseUp;
    }

    FButtonView = avBoth;
    FEditBoxVisible = true;
    FColorBox   = clBlack;
    FColorEdit  = (TColor)0x373737;
    FGlyphType  = 1;

    //FCaptionTitle   = L"";
    FCaptionValue   = 0.0;
    //FCaptionUnit    = L"";

    //FBtnCaptionLeft     = L"";
    //FBtnCaptionRight    = L"";
    //FBtnCaptionTop      = L"";
    //FBtnCaptionBottom   = L"";

    //--------------------------
    // Input Number Property
    FInputEdit = new TAZEditBoxProperty;
    FInputEdit->OnChange = InputEditChanged;

    //--------------------------
    // Edit Box 관련 초기 설정.
    FBasePanel = new TPanel(this);
    FBasePanel->Parent      = this;
    FBasePanel->Height      = Height / 3;
    FBasePanel->Width       = Width  / 3;
    FBasePanel->ParentBackground = false;
    FBasePanel->ParentFont = false;
    FBasePanel->BevelOuter  = bvNone;
    FBasePanel->BevelKind   = bkFlat;
    FBasePanel->OnClick     = MyEditClick;

    FLabelTitle = new TLabel(FBasePanel);
    FLabelTitle->Parent     = FBasePanel;
    //FLabelTitle->Top        = Height;
    FLabelTitle->Align      = alTop;
    FLabelTitle->ParentFont = false;
    //FLabelTitle->Top        = Height;
    FLabelTitle->Alignment  = taCenter;
    FLabelTitle->Layout     = tlCenter;
    FLabelTitle->AlignWithMargins = true;
    FLabelTitle->Margins->Top       = 1;
    FLabelTitle->Margins->Bottom    = 1;
    FLabelTitle->OnClick     = MyEditClick;

    FLabelValue = new TLabel(FBasePanel);
    FLabelValue->Parent     = FBasePanel;
    //FLabelValue->Top        = Height;
    FLabelValue->Align      = alClient;
    FLabelValue->ParentFont = false;
    //FLabelValue->Top        = Height;
    FLabelValue->Alignment  = taCenter;
    FLabelValue->Layout     = tlCenter;
    FLabelValue->Transparent= false;
    FLabelValue->AlignWithMargins = true;
    FLabelValue->Margins->Top    = 1;
    FLabelValue->Margins->Bottom = 0;
    FLabelValue->OnClick     = MyEditClick;

    FLabelUnit = new TLabel(FBasePanel);
    FLabelUnit->Parent      = FBasePanel;
    //FLabelUnit->Top         = Height;
    FLabelUnit->Align       = alBottom;
    FLabelUnit->ParentFont  = false;
    //FLabelUnit->Top         = Height;
    FLabelUnit->Alignment   = taCenter;
    FLabelUnit->Layout      = tlCenter;
    FLabelUnit->AlignWithMargins = true;
    FLabelUnit->Margins->Top    = 0;
    FLabelUnit->Margins->Bottom = 1;
    FLabelUnit->OnClick     = MyEditClick;

    //--------------------------
    // Font 생성.

    FFontTitle  = new Vcl::Graphics::TFont;
    FFontValue  = new Vcl::Graphics::TFont;
    FFontUnit   = new Vcl::Graphics::TFont;

    FFontTitle->OnChange = FontChanged;
    FFontValue->OnChange = FontChanged;
    FFontUnit->OnChange  = FontChanged;

    FFontValue->Size    = 20;
    FFontValue->Color   = clAqua;
    FFontValue->Name    = AZ_NUM_SHAPE_FONT;

    FFontUnit->Color    = clSilver;
    FFontTitle->Color   = clSilver;

    //--------------------------
    FGlyph = new Vcl::Graphics::TBitmap;
    FGlyph->OnChange = GlyphChanged;

    FNumGlyphs = 4;

    //--------------------------

    CreateIndicatorImage();
    DisplayUpdate();
}
//---------------------------------------------------------------------------
__fastcall TAZArrowButton::~TAZArrowButton()
{
    if(FBitBtn[abLeft])      delete FBitBtn[abLeft];
    if(FBitBtn[abRight])     delete FBitBtn[abRight];
    if(FBitBtn[abTop])       delete FBitBtn[abTop];
    if(FBitBtn[abBottom])    delete FBitBtn[abBottom];

    if(FLabelUnit)           delete FLabelUnit;
    if(FLabelValue)          delete FLabelValue;
    if(FLabelTitle)          delete FLabelTitle;
    if(FBasePanel)           delete FBasePanel;

    if(FFontTitle)           delete FFontTitle;
    if(FFontValue)           delete FFontValue;
    if(FFontUnit)            delete FFontUnit;

    if(FGlyph)               delete FGlyph;
    if(FInputEdit)           delete FInputEdit;
}
//---------------------------------------------------------------------------
//namespace Azarrowbutton
//{
//    void __fastcall PACKAGE Register()
//    {
//         TComponentClass classes[1] = {__classid(TAZArrowButton)};
//         RegisterComponents(L"AZ", classes, 0);
//    }
//}
//---------------------------------------------------------------------------
void     __fastcall TAZArrowButton::MyResize(TObject *Sender)
{
    if(FOnResize) FOnResize(this);

    DisplayUpdate();
}
//---------------------------------------------------------------------------
void     __fastcall TAZArrowButton::MyClick(TObject *Sender)
{
    TBitBtn * pBtn  =dynamic_cast<TBitBtn *>(Sender);

    if(pBtn == NULL) return;

    TArrowButtonState eClickBtn;

    if(pBtn == FBitBtn[abLeft])         eClickBtn = abLeft;
    else if(pBtn == FBitBtn[abRight])   eClickBtn = abRight;
    else if(pBtn == FBitBtn[abTop])     eClickBtn = abTop;
    else if(pBtn == FBitBtn[abBottom])  eClickBtn = abBottom;
    else                                return;

    if(FOnClick) FOnClick(this, eClickBtn);
}
//---------------------------------------------------------------------------
void __fastcall TAZArrowButton::MyMouseDown(TObject *Sender,
                                            TMouseButton Button,
                                            TShiftState Shift,
                                            int X, int Y)
{
    TBitBtn * pBtn  =dynamic_cast<TBitBtn *>(Sender);

    if(pBtn == NULL) return;

    TArrowButtonState eClickBtn;

    if(pBtn == FBitBtn[abLeft])         eClickBtn = abLeft;
    else if(pBtn == FBitBtn[abRight])   eClickBtn = abRight;
    else if(pBtn == FBitBtn[abTop])     eClickBtn = abTop;
    else if(pBtn == FBitBtn[abBottom])  eClickBtn = abBottom;
    else                                return;

    if(FOnMouseDown) FOnMouseDown(this, Button, Shift, eClickBtn);
}
//---------------------------------------------------------------------------
void __fastcall TAZArrowButton::MyMouseLeave(TObject *Sender)
{
    TBitBtn * pBtn  =dynamic_cast<TBitBtn *>(Sender);

    if(pBtn == NULL) return;

    TArrowButtonState eClickBtn;

    if(pBtn == FBitBtn[abLeft])         eClickBtn = abLeft;
    else if(pBtn == FBitBtn[abRight])   eClickBtn = abRight;
    else if(pBtn == FBitBtn[abTop])     eClickBtn = abTop;
    else if(pBtn == FBitBtn[abBottom])  eClickBtn = abBottom;
    else                                return;

    if(FOnMouseLeave) FOnMouseLeave(this, eClickBtn);
}
//---------------------------------------------------------------------------
void __fastcall TAZArrowButton::MyMouseUp(  TObject *Sender,
                                            TMouseButton Button,
                                            TShiftState Shift,
                                            int X, int Y)
{
    TBitBtn * pBtn  =dynamic_cast<TBitBtn *>(Sender);

    if(pBtn == NULL) return;

    TArrowButtonState eClickBtn;

    if(pBtn == FBitBtn[abLeft])         eClickBtn = abLeft;
    else if(pBtn == FBitBtn[abRight])   eClickBtn = abRight;
    else if(pBtn == FBitBtn[abTop])     eClickBtn = abTop;
    else if(pBtn == FBitBtn[abBottom])  eClickBtn = abBottom;
    else                                return;

    if(FOnMouseUp) FOnMouseUp(this, Button, Shift, eClickBtn);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void     __fastcall TAZArrowButton::MyEditClick(TObject *Sender)
{
    if(Sender == NULL)     return;

    if(InputNumber(Sender)) {
        DisplayUpdate();
    }

    //-----------------------------
    // 사용자 Click Event 처리.
    if(OnEditClick) OnEditClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TAZArrowButton::Loaded(void)
{
    TWinControl::Loaded();

    CreateIndicatorImage();
    DisplayUpdate();
}
//---------------------------------------------------------------------------
void     __fastcall TAZArrowButton::SetEnabled(bool Value)
{
    TWinControl::SetEnabled(Value);

    if(FBitBtn[abLeft])      FBitBtn[abLeft]->Enabled     = Value;
    if(FBitBtn[abRight])     FBitBtn[abRight]->Enabled    = Value;
    if(FBitBtn[abTop])       FBitBtn[abTop]->Enabled      = Value;
    if(FBitBtn[abBottom])    FBitBtn[abBottom]->Enabled   = Value;

    DisplayUpdate();
}
//---------------------------------------------------------------------------
TBitBtn * __fastcall TAZArrowButton::GetBitBtn(int n)
{
    if(n >= abLeft && n < abMax) return FBitBtn[n];

    return NULL;
}
//---------------------------------------------------------------------------

void __fastcall TAZArrowButton::DisplayUpdate()
{
    //CreateIndicatorImage();

    int nOneBtnW = 0;
    int nOneBtnH = 0;

    for(int i=0; i<abMax; i++) FBitBtn[i]->Visible = true;

    switch(FButtonView) {
        case avVertical:
            nOneBtnW = Width;
            nOneBtnH = FEditBoxVisible ? (Height / 3) : (Height / 2);

            // 사용하지 않는 것 안보이게 한다.
            FBitBtn[abLeft]->Visible   = false;
            FBitBtn[abRight]->Visible  = false;
            FBitBtn[abLeft]->Left      = Width;
            FBitBtn[abRight]->Left     = Width;
            //-------------------------------

            FBitBtn[abTop]->Top  = 0;
            FBitBtn[abTop]->Left = 0;
            FBitBtn[abBottom]->Top  = FEditBoxVisible ? (nOneBtnH * 2) : nOneBtnH;
            FBitBtn[abBottom]->Left = 0;
            break;

        case avHorizontal:
            nOneBtnW = FEditBoxVisible ? (Width  / 3) : (Width  / 2);
            nOneBtnH = Height;

          // 사용하지 않는 것 안보이게 한다.
            FBitBtn[abTop]->Visible    = false;
            FBitBtn[abBottom]->Visible = false;
            FBitBtn[abTop]->Left       = Width;
            FBitBtn[abBottom]->Left    = Width;
            //-------------------------------

            FBitBtn[abLeft]->Top  = 0;
            FBitBtn[abLeft]->Left = 0;
            FBitBtn[abRight]->Top  = 0;
            FBitBtn[abRight]->Left = FEditBoxVisible ? (nOneBtnW * 2) : nOneBtnW;
            break;

        case avBoth:
            nOneBtnW = Width  / 3;
            nOneBtnH = Height / 3;

            FBitBtn[abTop]->Top  = 0;
            FBitBtn[abTop]->Left = nOneBtnW;
            FBitBtn[abBottom]->Top  = nOneBtnH * 2;
            FBitBtn[abBottom]->Left = nOneBtnW;

            FBitBtn[abLeft]->Top  = nOneBtnH;
            FBitBtn[abLeft]->Left = 0;
            FBitBtn[abRight]->Top  = nOneBtnH;
            FBitBtn[abRight]->Left = nOneBtnW * 2;
            break;
    }

    // Button의 화살표 모양 위치 지정.
    FBitBtn[abTop]->Layout       = blGlyphTop;
    FBitBtn[abBottom]->Layout    = blGlyphBottom;
    FBitBtn[abLeft]->Layout      = blGlyphLeft;
    FBitBtn[abRight]->Layout     = blGlyphRight;

    // Button Caption 설정.
    FBitBtn[abTop]->Caption       = ChangeString(FBtnCaptionTop, L"\\n", L"\n");
    FBitBtn[abBottom]->Caption    = ChangeString(FBtnCaptionBottom, L"\\n", L"\n");
    FBitBtn[abLeft]->Caption      = ChangeString(FBtnCaptionLeft, L"\\n", L"\n");
    FBitBtn[abRight]->Caption     = ChangeString(FBtnCaptionRight, L"\\n", L"\n");

    // Button의 크기 설정.
    for(int i=0; i<abMax; i++) {
        FBitBtn[i]->Width  = nOneBtnW;
        FBitBtn[i]->Height = nOneBtnH;
    }

    //-----------------------------
    // Edit Box Shape
    DisplayEditBox(nOneBtnW, nOneBtnH);

}
//---------------------------------------------------------------------------
void __fastcall TAZArrowButton::DisplayEditBox(int nWidth, int nHeight)
{
    FBasePanel->Visible = FEditBoxVisible;

    if(FEditBoxVisible) {
        FBasePanel->Width  = nWidth - 2;
        FBasePanel->Height = nHeight - 2;

        switch(FButtonView) {
            case avVertical:
                FBasePanel->Top  = nHeight + 1;
                FBasePanel->Left = 1;
                break;

            case avHorizontal:
                FBasePanel->Top  = 1;
                FBasePanel->Left = nWidth + 1;
                break;

            case avBoth:
                FBasePanel->Top  = nHeight + 1;
                FBasePanel->Left = nWidth + 1;
                break;
        }

        FBasePanel->Caption     = "";

        FLabelTitle->Font->Assign(FFontTitle);
        FLabelTitle->Caption    = FCaptionTitle;
        FLabelTitle->Visible    = FEditBoxVisible;

        FLabelValue->Font->Assign(FFontValue);
        FLabelValue->Caption    = GetDrawValue(FCaptionValue, FInputEdit->Type, FInputEdit->DecimalPlaces) ;
        FLabelValue->Visible    = FEditBoxVisible;

        FLabelUnit->Font->Assign(FFontUnit);
        FLabelUnit->Caption     = FCaptionUnit;
        FLabelUnit->Visible     = FEditBoxVisible;

        if(Enabled) {
            FBasePanel->Color       = FColorBox;
            FLabelValue->Color      = FColorEdit;
        }
        else {
            FBasePanel->Color           = clBtnFace;
            FLabelTitle->Font->Color    = clSilver;
            FLabelValue->Color          = clSilver;
            FLabelValue->Font->Color    = clWhite;
            FLabelUnit->Font->Color     = clSilver;
        }
    }
    else {
        FBasePanel->Left = Width;
    }

}

void __fastcall TAZArrowButton::CreateIndicatorImage()
{
    Graphics::TBitmap* pBmpRight = new Graphics::TBitmap;
    Graphics::TBitmap* pBmpDown  = new Graphics::TBitmap;
    Graphics::TBitmap* pBmpUp    = new Graphics::TBitmap;
    Graphics::TBitmap* pBmpLeft  = new Graphics::TBitmap;

    if(IsCustom()) {
        pBmpRight->Assign(FGlyph);
    }
    else {
        String sGlyphName;

        switch(FGlyphType) {
            case 1:  sGlyphName = "RightArrowGlyph"; break;
            case 2:  sGlyphName = "RightArrowGlyph2"; break;
            case 3:  sGlyphName = "RightArrowGlyph3"; break;
            case 4:  sGlyphName = "RightArrowGlyph4"; break;
            case 5:  sGlyphName = "RightArrowGlyph5"; break;
            case 6:  sGlyphName = "RightArrowGlyph6"; break;
            default: sGlyphName = "RightArrowGlyph"; break;
        }
        //pBmpRight->LoadFromResourceName((int)HInstance, sGlyphName);
        //int nResI = FindClassHInstance(__classid(TAZArrowButton));
        pBmpRight->LoadFromResourceName((int)HInstance, sGlyphName);
        FNumGlyphs = 4;

    }

    int nImageW  = pBmpRight->Width;
    int nImageH  = pBmpRight->Height;
    int nImage1W = nImageW / FNumGlyphs;

    pBmpDown->Width     = nImageH * FNumGlyphs;
    pBmpDown->Height    = nImage1W;

    pBmpUp->Width       = nImageH * FNumGlyphs;
    pBmpUp->Height      = nImage1W;

    pBmpLeft->Width     = nImageW;
    pBmpLeft->Height    = nImageH;

    for(int i=0; i<FNumGlyphs; i++) {
        for(int y=0; y<nImageH; y++) {
            for(int x=0; x<nImage1W; x++) {
                // 시계 방향 90도 회전하여 복사함. (Down)
                pBmpDown->Canvas->Pixels[nImageH * i + y][x]
                = pBmpRight->Canvas->Pixels[nImage1W * i + x][y];

                //  수평으로 뒤집어서 복사함. (Left)
                pBmpLeft->Canvas->Pixels[nImage1W * i + (nImage1W-x-1)][y]
                = pBmpRight->Canvas->Pixels[nImage1W * i + x][y];
            }
        }
    }

    for(int y=0; y<pBmpDown->Height; y++) {
        for(int x=0; x<pBmpDown->Width; x++) {
            // Down 이미지를 수직 방향으로 뒤집어서 복사함 (Up)
            pBmpUp->Canvas->Pixels[x][y]
            = pBmpDown->Canvas->Pixels[x][pBmpDown->Height - y - 1];
        }
    }

    // 투명 색상 지정. 왼쪽 가장 아래 픽셀의 색상이 투명 색상으로 적용된다.
    TColor cTranColor = pBmpRight->Canvas->Pixels[0][pBmpRight->Height - 1];
    pBmpDown->Canvas->Pixels[0][pBmpDown->Height - 1] = cTranColor;
    pBmpLeft->Canvas->Pixels[0][pBmpLeft->Height - 1] = cTranColor;
    pBmpUp->Canvas->Pixels[0][pBmpUp->Height - 1]     = cTranColor;

    if(FBitBtn[abLeft])      FBitBtn[abLeft]->Glyph->Assign(pBmpLeft);
    if(FBitBtn[abRight])     FBitBtn[abRight]->Glyph->Assign(pBmpRight);
    if(FBitBtn[abTop])       FBitBtn[abTop]->Glyph->Assign(pBmpUp);
    if(FBitBtn[abBottom])    FBitBtn[abBottom]->Glyph->Assign(pBmpDown);

    if(pBmpDown)    delete pBmpDown;
    if(pBmpLeft)    delete pBmpLeft;
    if(pBmpUp)      delete pBmpUp;
    if(pBmpRight)   delete pBmpRight;



}

//---------------------------------------------------------------------------
void __fastcall TAZArrowButton::SetButtonView(TArrowButtomView e)
{
    if(FButtonView != e) {
        FButtonView = e;
        DisplayUpdate();
    }
}

void __fastcall TAZArrowButton::SetEditBoxVisible(bool b)
{
    if(FEditBoxVisible != b) {
        FEditBoxVisible = b;
        DisplayUpdate();
    }
}

void __fastcall TAZArrowButton::SetMyFont(int Index, TFont * p)
{
    switch(Index) {
        case 1:     FFontTitle->Assign(p);  break;
        case 2:     FFontValue->Assign(p);  break;
        case 3:     FFontUnit->Assign(p);   break;
    }

    //MessageBox(NULL, "SetFont", "", MB_OK);

    DisplayUpdate();
}

void     __fastcall TAZArrowButton::FontChanged(System::TObject* Sender)
{
    DisplayUpdate();
}


void __fastcall TAZArrowButton::SetMyCaption(int Index, String s)
{
    bool bChanged = false;
    switch(Index) {
        case 1: bChanged = (s != FCaptionTitle); FCaptionTitle = s; break;
        //case 2: bChanged = (s != FCaptionValue); FCaptionValue = s; break;
        case 3: bChanged = (s != FCaptionUnit);  FCaptionUnit  = s; break;
        case 4: bChanged = (s != FBtnCaptionLeft);      FBtnCaptionLeft     = s; break;
        case 5: bChanged = (s != FBtnCaptionRight);     FBtnCaptionRight    = s; break;
        case 6: bChanged = (s != FBtnCaptionTop);       FBtnCaptionTop      = s; break;
        case 7: bChanged = (s != FBtnCaptionBottom);    FBtnCaptionBottom   = s; break;
    }

    if(bChanged) DisplayUpdate();
}

void __fastcall TAZArrowButton::SetCaptionValue(double d)
{
    if(FCaptionValue != d) {
        FCaptionValue = d;
        DisplayUpdate();
    }
}

void __fastcall TAZArrowButton::SetColor(int Index, TColor c)
{
    bool bChanged = false;
    switch(Index) {
        case 1: bChanged = (c != FColorBox);  FColorBox = c;  break;
        case 2: bChanged = (c != FColorEdit); FColorEdit = c; break;
    }

    if(bChanged) DisplayUpdate();
}

void __fastcall TAZArrowButton::SetGlyphType(int n)
{
    if(FGlyphType != n) {
        FGlyphType = n;

        CreateIndicatorImage();
        DisplayUpdate();
    }
}


void __fastcall TAZArrowButton::SetGlyph(Vcl::Graphics::TBitmap* Value)
{
    if(Value == NULL) {
        FGlyph->Width  = 0;
        FGlyph->Height = 0;
    }
    else {
        FGlyph->Assign(Value);
    }

    CreateIndicatorImage();
}

Vcl::Graphics::TBitmap* __fastcall TAZArrowButton::GetGlyph(void)
{
    return FGlyph;
}

TNumGlyphs __fastcall TAZArrowButton::GetNumGlyphs(void)
{
    return FNumGlyphs;
}

void __fastcall TAZArrowButton::SetNumGlyphs(TNumGlyphs Value)
{
    if(FNumGlyphs != Value) {
        FNumGlyphs = Value;
        CreateIndicatorImage();
        DisplayUpdate();
    }
}

void __fastcall TAZArrowButton::GlyphChanged(System::TObject* Sender)
{
    CreateIndicatorImage();
    DisplayUpdate();
}

bool __fastcall TAZArrowButton::IsCustom(void)
{
    if(FGlyph->Width > 0 && FGlyph->Height > 0) return true;
    return false;
}


//---------------------------------------------------------------------------

bool     __fastcall TAZArrowButton::InputNumber(TObject *Sender)
{
    if(Sender == NULL) return false;

    AnsiString sReturn;

    TfmNumPad_AZ  * pForm = new TfmNumPad_AZ((TComponent*)Sender,
                                        InputEdit->Type,
                                        InputEdit->Min,
                                        InputEdit->Max,
                                        FCaptionValue,
                                        &sReturn,
                                        InputEdit->UpDownValue,
                                        FInputEdit->DecimalPlaces);

    int nRet = pForm->ShowModal();

    delete pForm;

    if(nRet == mrOk) {
        double dValue = sReturn.ToDouble();

        switch(FInputEdit->Type) {
            case itInt:
                FCaptionValue = (int)dValue;
                break;

            case itFloat:
                FCaptionValue = DecimalRounding(dValue, FInputEdit->DecimalPlaces);
                break;

            default:
                FCaptionValue = DecimalRounding(dValue, 3);
                break;
        }

        return true;
    }

    return false;
}

//----------------------------------------------------------------------------
void  __fastcall TAZArrowButton::InputEditChanged(System::TObject* Sender)
{
    DisplayUpdate();
}

void __fastcall TAZArrowButton::SetInputEdit(TAZEditBoxProperty * p)
{
    if(p) {
        FInputEdit->Assign(p);
        DisplayUpdate();
    }
}
//----------------------------------------------------------------------------

