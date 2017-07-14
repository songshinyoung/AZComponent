//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "IndexButton.h"
#include "GlobalFunction_AZ.h"

#pragma package(smart_init)

#pragma resource "TAZPanel_resources.res"

//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//


//---------------------------------------------------------------------------


static inline void ValidCtrCheck(TIndexButton *)
{
    new TIndexButton(NULL);
}
//---------------------------------------------------------------------------
__fastcall TIndexButton::TIndexButton(TComponent* Owner)
    : TWinControl(Owner)
{
    TWinControl::OnResize   = MyResize;

    FList = new System::Classes::TStringList;
    //FList->Add("Button1\ntest");
    //FList->Add("Button2");
    //FList->Add("Button3");

    ((System::Classes::TStringList *)FList)->OnChange = ListChanged;

    BtnList = new TList;

    this->Width             = 200;
    this->Height            = 300;

    FBtnHeight              = 70;
    FBtnWidth               = 100;
    FGlyphRound             = 5;
    FMargin                 = 0;
    FSpacing                = 5;
    FGlyphWidth             = -1;
    FGlyphHeight            = -1;
    FIndex                  = -1;

    FColorUp                = (TColor)0x09B9B9B;            // 선택되지 않은 상태 색상. ( Enable)
    FColorDisable           = (TColor)0x0CDCDCD;            // Disable 상태 색상.
    FColorDown              = (TColor)0x04080FF;            // 사용자가 Mouse Down 중인 상태 색상.
    FColorSelect            = (TColor)0x0DBDB00;            // 선택된 상태 색상.
    FColorLine              = clGray;

    FGlyph                  = new Vcl::Graphics::TBitmap;
    FGlyph->OnChange        = GlyphChanged;
    FGlyphDefault           = false;
    FGlyphLayout            = blGlyphLeft;
    
    Font->Size              = 20;
    Font->Name              = "Arial";

    //--------------------------------
    BMP_Indicator           = new Graphics::TBitmap;
    BMP_IndicatorNumbering  = new Graphics::TBitmap;

    //--------------------------------

    FMatrixButton = new TMatrixButtonProperty();
    FMatrixButton->OnChange = MatrixPropertyChanged;
    
    //--------------------------------
    FNumbering = new TIndexBtnNumberingProperty();
    FNumbering->OnChange = NumberingPropertyChanged;
    
    //--------------------------------

    CreateButton();
    DisplayUpdate();

}

__fastcall TIndexButton::~TIndexButton()
{
    if(FList) delete FList;
    if(BtnList) {

        for(; BtnList->Count; ) {
            delete ((TSpeedButton * )BtnList->Items[0]);
            BtnList->Delete(0);
        }
    }

    if(FGlyph) delete FGlyph;
    if(FMatrixButton) delete FMatrixButton;
    if(BMP_Indicator) delete BMP_Indicator;
    if(BMP_IndicatorNumbering) delete BMP_IndicatorNumbering;
    
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
void __fastcall TIndexButton::Loaded(void)
{
    TWinControl::Loaded();

    //AnsiString sTemp;
    //sTemp.printf("Loaded : List Count = %d", FList->Count);
    //MessageBox(NULL, sTemp.c_str(), "", MB_OK);

    CreateButton();
    DisplayUpdate();
}
//---------------------------------------------------------------------------
void     __fastcall TIndexButton::SetEnabled(bool Value)
{
    TWinControl::SetEnabled(Value);

    for(int i=0; i<BtnList->Count; i++) {
        TSpeedButton * pBtn = (TSpeedButton *)BtnList->Items[i];
        pBtn->Enabled = Value;
    }
}

//---------------------------------------------------------------------------
void     __fastcall TIndexButton::MyResize(TObject *Sender)
{
    if(FOnResize) FOnResize(this);

    DisplayUpdate();
}
//---------------------------------------------------------------------------
int     __fastcall TIndexButton::FindDownIndex()
{
    TSpeedButton * pBtn = NULL;
    FIndex = -1;
    for(int i=0; i<BtnList->Count; i++) {
        pBtn = (TSpeedButton *)BtnList->Items[i];
        if(pBtn) {
            if(pBtn->Down) {
                FIndex = i;
                break;
            }
        }
    }

    return FIndex;
}
//---------------------------------------------------------------------------
int     __fastcall TIndexButton::FindIndex(__in int nRow, __in int nCol)
{
    int  nIndex  = -1;

    if(FMatrixButton->Enabled) {
        int  nRowMax = FMatrixButton->RowCount;
        int  nColMax = FMatrixButton->ColCount;

        if(nRow >= nRowMax || nCol >= nColMax) return -1;
        
        switch(FLayout) {
            case ibVertical:
                nIndex = nCol * nRowMax + nRow;
                break;
            case ibHorizontal:
                nIndex = nRow * nColMax + nCol;
                break;
        }
    }
    else {
        switch(FLayout) {
            case ibVertical:
                nIndex = nRow;
                break;
            case ibHorizontal:
                nIndex = nCol;
                break;
        }

        if(nIndex >= BtnList->Count) return -1;
    }

    return nIndex;
}
//---------------------------------------------------------------------------

int    __fastcall TIndexButton::GetDownButtonMatrix(__out int &nRow, __out int &nCol)
{
    int nIndex = FindDownIndex();
    int RowTemp = 0;
    int ColTemp = 0;
    
    if(nIndex >= 0) {
        FindMatrix(nIndex, RowTemp, ColTemp);
        nRow = RowTemp;
        nCol = ColTemp; 
    }
    else {
        nRow = -1;
        nCol = -1;
    }
    
    return nIndex;
}
    
//---------------------------------------------------------------------------
void  __fastcall TIndexButton::FindMatrix(int nIndex, __out int &nRow, __out int &nCol)
{
    nCol = 0;
    nRow = 0;
    
    if(FMatrixButton->Enabled) {
        int nMaxRow = FMatrixButton->RowCount;
        int nMaxCol = FMatrixButton->ColCount;
        
        switch(FLayout) {
            case ibVertical:
                nCol = nIndex / nMaxRow;
                nRow = nIndex % nMaxRow;
                break;
                
            case ibHorizontal:
                nRow = nIndex / nMaxCol;
                nCol = nIndex % nMaxCol;
                break;
        }
    }
    else {
        switch(FLayout) {
            case ibVertical:
                nRow = nIndex;
                break;
                
            case ibHorizontal:
                nCol = nIndex;
                break;
        }
    }
}

//---------------------------------------------------------------------------
void     __fastcall TIndexButton::MyClick(TObject *Sender)
{
    TSpeedButton * pBtn  =dynamic_cast<TSpeedButton *>(Sender);

    if(pBtn == NULL) return;

    FindDownIndex();

    int nIndex = pBtn->Tag;
    
    if(FOnClick) FOnClick(this, nIndex);
    
    if(FOnMatrixClick) {

        int nRow = 0;
        int nCol = 0;

        FindMatrix(nIndex, nRow, nCol);
        
        FOnMatrixClick(this, nIndex, nRow, nCol);
    }
}
//---------------------------------------------------------------------------
void __fastcall TIndexButton::MyMouseDown(TObject *Sender,
                                          TMouseButton Button,
                                          TShiftState Shift,
                                          int X, int Y)
{
    TSpeedButton * pBtn  =dynamic_cast<TSpeedButton *>(Sender);

    if(pBtn == NULL) return;

    int nIndex = pBtn->Tag;
    int nRow = 0;
    int nCol = 0;
    FindMatrix(nIndex, nRow, nCol);
        
    if(FOnMouseDown) FOnMouseDown(this, Button, Shift, nIndex, nRow, nCol);
}
//---------------------------------------------------------------------------
void __fastcall TIndexButton::MyMouseLeave(TObject *Sender)
{
    TSpeedButton * pBtn  =dynamic_cast<TSpeedButton *>(Sender);

    if(pBtn == NULL) return;

    int nIndex = pBtn->Tag;
    int nRow = 0;
    int nCol = 0;
    FindMatrix(nIndex, nRow, nCol);
        
    if(FOnMouseLeave) FOnMouseLeave(this, nIndex, nRow, nCol);
}
//---------------------------------------------------------------------------
void __fastcall TIndexButton::MyMouseUp(  TObject *Sender,
                                            TMouseButton Button,
                                            TShiftState Shift,
                                            int X, int Y)
{
    TSpeedButton * pBtn  =dynamic_cast<TSpeedButton *>(Sender);

    if(pBtn == NULL) return;

    int nIndex = pBtn->Tag;
    int nRow = 0;
    int nCol = 0;
    FindMatrix(nIndex, nRow, nCol);
        
    if(FOnMouseUp) FOnMouseUp(this, Button, Shift, nIndex, nRow, nCol);
}
//---------------------------------------------------------------------------
void __fastcall TIndexButton::ListChanged(System::TObject* Sender)
{
    CreateButton();
    DisplayUpdate();
}
//---------------------------------------------------------------------------
TColor __fastcall TIndexButton::MakeBrightColor(TColor c, int Add)
{
    int nRGB = ColorToRGB(c);

    int nB = ((nRGB & 0xFF0000) >> 16)    + Add;
    int nG = ((nRGB & 0x00FF00) >> 8)     + Add;
    int nR = (nRGB & 0x0000FF)            + Add;

    nB = nB > 255 ? 255 : nB;
    nG = nG > 255 ? 255 : nG;
    nR = nR > 255 ? 255 : nR;

    nB = nB < 0 ? 0 : nB;
    nG = nG < 0 ? 0 : nG;
    nR = nR < 0 ? 0 : nR;

    nRGB = (nB << 16) + (nG << 8) + nR;

    return (TColor)nRGB;
}
//---------------------------------------------------------------------------
void __fastcall TIndexButton::CreateIndicatorImage()
{
    if(FGlyph->Width > 0 && FGlyph->Height > 0) {
        for(int i=0; i<BtnList->Count; i++) {
            TSpeedButton * pBtn = (TSpeedButton *)BtnList->Items[i];
            pBtn->Glyph->Assign(FGlyph);
        }
    }
    else if(FGlyphDefault) {
        //int nResI = FindClassHInstance(__classid(TIndexButton));
        BMP_Indicator->LoadFromResourceName((int)HInstance, "CheckIndicator");

        for(int i=0; i<BtnList->Count; i++) {
            TSpeedButton * pBtn = (TSpeedButton *)BtnList->Items[i];
            pBtn->Glyph->Assign(BMP_Indicator);
        }        
    }
    else {
        if(BMP_Indicator == NULL) return;
        
        int nImg1Width  = 0;
        int nImg1Height = 0;

        if(FMatrixButton->Enabled) {
            if(FGlyphLayout == blGlyphLeft || FGlyphLayout == blGlyphRight) {

                int nCellH  = (FMatrixButton->RowCount > 0) ? (this->Height / FMatrixButton->RowCount) : 10;
                
                nImg1Width  = FGlyphWidth  >= 0 ? FGlyphWidth  : 25;
                nImg1Height = FGlyphHeight >= 0 ? FGlyphHeight : (nCellH - 6);
                nImg1Height = nImg1Height  > (nCellH - 6) ? (nCellH - 6) : nImg1Height;
            }
            else {
                int nCellW  = (FMatrixButton->ColCount > 0) ? (this->Width / FMatrixButton->ColCount) : 10;

                nImg1Width  = FGlyphWidth  >= 0 ? FGlyphWidth : (nCellW - 6);
                nImg1Width  = nImg1Width   > (nCellW - 6) ? (nCellW - 6) : nImg1Width;
                nImg1Height = FGlyphHeight >= 0 ? FGlyphHeight  : 25;
            }            
        }
        else {
            if(FGlyphLayout == blGlyphLeft || FGlyphLayout == blGlyphRight) { //if(FLayout == ibVertical) {
                nImg1Width  = FGlyphWidth  >= 0 ? FGlyphWidth  : 25;

                switch(FLayout) {
                    case ibVertical:
                        nImg1Height = FGlyphHeight >= 0 ? FGlyphHeight : (FBtnHeight - 6);
                        nImg1Height = nImg1Height  > (FBtnHeight - 6) ? (FBtnHeight - 6) : nImg1Height;                        
                        break;
                    case ibHorizontal:
                        nImg1Height = FGlyphHeight >= 0 ? FGlyphHeight : (Height - 6);
                        nImg1Height = nImg1Height  > (Height - 6) ? (Height - 6) : nImg1Height;                        
                        break;
                }
            }
            else {
                nImg1Height = FGlyphHeight >= 0 ? FGlyphHeight  : 25;

                switch(FLayout) {
                    case ibVertical:
                        nImg1Width  = FGlyphWidth  >= 0 ? FGlyphWidth : (Width - 6);
                        nImg1Width  = nImg1Width   > (Width - 6) ? (Width - 6) : nImg1Width;
                        break;
                    case ibHorizontal:
                        nImg1Width  = FGlyphWidth  >= 0 ? FGlyphWidth : (FBtnWidth - 6);
                        nImg1Width  = nImg1Width   > (FBtnWidth - 6) ? (FBtnWidth - 6) : nImg1Width;
                        break;
                }
            }
        }


        nImg1Width  = nImg1Width  <= 0 ? 1 : nImg1Width;
        nImg1Height = nImg1Height <= 0 ? 1 : nImg1Height;

        BMP_Indicator->Width  = nImg1Width * 4;
        BMP_Indicator->Height = nImg1Height;

        BMP_Indicator->Canvas->Brush->Color = (TColor)0xFFFFFE;
        BMP_Indicator->Canvas->FillRect(Rect(0, 0, BMP_Indicator->Width, BMP_Indicator->Height));
        
        DrawIndicatorPeice(BMP_Indicator->Canvas, 0, nImg1Width, nImg1Height, FColorUp, false);
        DrawIndicatorPeice(BMP_Indicator->Canvas, 1, nImg1Width, nImg1Height, FColorDisable, false);
        DrawIndicatorPeice(BMP_Indicator->Canvas, 2, nImg1Width, nImg1Height, FColorDown, true);
        DrawIndicatorPeice(BMP_Indicator->Canvas, 3, nImg1Width, nImg1Height, FColorSelect, true);

        if(FNumbering->Enabled) {
            BMP_IndicatorNumbering->Width  = nImg1Width * 4;
            BMP_IndicatorNumbering->Height = nImg1Height;
        }
        
        for(int i=0; i<BtnList->Count; i++) {
            TSpeedButton * pBtn = (TSpeedButton *)BtnList->Items[i];

            if(FNumbering->Enabled) {
                DrawIndicatorNumber(BMP_IndicatorNumbering, BMP_Indicator, FNumbering->StartNumber + i);
                pBtn->Glyph->Assign(BMP_IndicatorNumbering);
            }
            else {
                pBtn->Glyph->Assign(BMP_Indicator);
            }
        }

    }
}
//---------------------------------------------------------------------------

void __fastcall TIndexButton::DrawIndicatorPeice(
    TCanvas *   pCnavas,
    int         Index,
    int         nWidth,
    int         nHeight,
    TColor      MyColor,
    bool        bDualColor)
{
    if(pCnavas == NULL) return;

    bool bLayoutLeft = false;

    bLayoutLeft = (FGlyphLayout == blGlyphLeft || FGlyphLayout == blGlyphRight) ? true : false;
        
    if(bLayoutLeft) {
        pCnavas->Brush->Color = bDualColor ? MakeBrightColor(MyColor, 0x30) : MyColor;
        pCnavas->FillRect(Rect(nWidth*Index, 0, (nWidth*(Index + 1)) - 1, (nHeight / 2) + 2));

        pCnavas->Brush->Color = MyColor;
        pCnavas->FillRect(Rect(nWidth*Index, (nHeight / 2) + 2, (nWidth*(Index + 1))-1, nHeight));
//        pCnavas->Pixels[nWidth*Index][0] = clWhite;
//        pCnavas->Pixels[nWidth*Index][nHeight-1] = clWhite;
    }
    else {
        pCnavas->Brush->Color = bDualColor ? MakeBrightColor(MyColor, 0x30) : MyColor;
        pCnavas->FillRect(Rect(nWidth*Index, 0, (nWidth * Index) + (nWidth / 2), nHeight));

        pCnavas->Brush->Color = MyColor;
        pCnavas->FillRect(Rect((nWidth * Index) + (nWidth / 2), 0, (nWidth*(Index + 1))-1, nHeight));
//        pCnavas->Pixels[nWidth*Index][0] = clWhite;
//        pCnavas->Pixels[nWidth*Index + nWidth - 1][0] = clWhite;
//        pCnavas->Pixels[nWidth*Index][nHeight-1] = clWhite;
    }

    pCnavas->Pixels[nWidth*Index][0]                        = (TColor)0xFFFFFE;
    pCnavas->Pixels[nWidth*Index][nHeight-1]                = (TColor)0xFFFFFE;    
    pCnavas->Pixels[nWidth*Index + nWidth - 1][0]           = (TColor)0xFFFFFE;
    pCnavas->Pixels[nWidth*Index + nWidth - 1][nHeight-1]   = (TColor)0xFFFFFE;    
}
//---------------------------------------------------------------------------
void __fastcall TIndexButton::DrawIndicatorNumber( Vcl::Graphics::TBitmap *   pBMP_Target,
                                                   Vcl::Graphics::TBitmap *   pBMP_BackGr,
                                                   int         Index)
{
    if(pBMP_Target == NULL || pBMP_BackGr == NULL) return;

	//bool bLayoutLeft = false;
    int nWidth  = pBMP_BackGr->Width / 4;
    int nHeight = pBMP_BackGr->Height;
    AnsiString sStr = IntToStr(Index);
    
	//int nTexW = pBMP_BackGr->Canvas->TextWidth(sStr);
	//int nTexH = pBMP_BackGr->Canvas->TextHeight(sStr);

    pBMP_Target->Assign(pBMP_BackGr);

    pBMP_Target->Canvas->Brush->Style   = bsClear; // Brush Style을 투명으로 해야지 Text 가정상 출력된다.
//    pBMP_Target->Canvas->Font->Style    = TFontStyles() << fsBold;
//    pBMP_Target->Canvas->Font->Name     = "Arial";
//    pBMP_Target->Canvas->Font->Color    = clWhite;
//    pBMP_Target->Canvas->Font->Size     = 10;
    pBMP_Target->Canvas->Font->Assign(FNumbering->Font);

    TRect Rect;
    Rect.Top    = 0;
    Rect.Left   = 0;
    Rect.Right  = nWidth;
    Rect.Bottom = nHeight;
    
    for(int i=0; i<4; i++) {
        DrawText(pBMP_Target->Canvas->Handle, sStr.c_str() , -1, &Rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
        Rect.Left   += nWidth;
        Rect.Right  += nWidth;
    }
}
//---------------------------------------------------------------------------
void __fastcall TIndexButton::DisplayUpdate()
{
    CreateIndicatorImage();

    if(FMatrixButton->Enabled) {
        SetMatrixButtonAlign(FMatrixButton->RowCount, FMatrixButton->ColCount);
    }
    else {
        for(int i=0; i<BtnList->Count; i++) {
            TSpeedButton * pBtn = (TSpeedButton *)BtnList->Items[i];
            if(FLayout == ibVertical)   pBtn->Height  = FBtnHeight;
            if(FLayout == ibHorizontal) pBtn->Width   = FBtnWidth;
            pBtn->Margin  = FMargin;
            pBtn->Spacing = FSpacing;
            if(FList) if(i < FList->Count) pBtn->Caption = FList->Strings[i];
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TIndexButton::CreateButton()
{
    // Speed Button 모두 삭제.
    for(; BtnList->Count > 0; ) {
        TSpeedButton * pBtn = (TSpeedButton *)BtnList->Items[0];
        delete pBtn;
        BtnList->Delete(0);
    }

    if(FMatrixButton->Enabled) {

        if((FMatrixButton->ColCount > 0) && (FMatrixButton->RowCount > 0)) {

            int nBtnCount = FMatrixButton->ColCount * FMatrixButton->RowCount;

            for(int i=0; i<nBtnCount; i++) {
                TSpeedButton * pBtn = new TSpeedButton(this);
                pBtn->Parent        = this;
                pBtn->Layout        = FGlyphLayout; //ibVertical ? blGlyphLeft : blGlyphTop;
                pBtn->Tag           = i;
                pBtn->GroupIndex    = 1;
                pBtn->AllowAllUp    = FAllowAllUp;
                pBtn->NumGlyphs     = 4;
                pBtn->OnClick       = MyClick;
                pBtn->OnMouseDown   = MyMouseDown;
                pBtn->OnMouseLeave  = MyMouseLeave;
                pBtn->OnMouseUp     = MyMouseUp;
                BtnList->Add(pBtn);
            }
            
            SetMatrixButtonAlign(FMatrixButton->RowCount, FMatrixButton->ColCount);

            ((System::Classes::TStringList *)FList)->OnChange = NULL;
            while(BtnList->Count > FList->Count) {
                FList->Add("");
            }
            ((System::Classes::TStringList *)FList)->OnChange = ListChanged;
        }
    }
    else {
        // 다시 StringList에 맞게 생성.
        if(FList->Count > BtnList->Count) {
            for(; FList->Count > BtnList->Count; ) {
                TSpeedButton * pBtn = new TSpeedButton(this);
                pBtn->Parent        = this;

                switch(FLayout) {
                    case ibHorizontal:
                        pBtn->Left          = 1000;
                        //pBtn->Align         = alRight;
                        pBtn->Align         = alLeft;
                        break;

                    case ibVertical:
                    default:
                        pBtn->Top           = 1000;
                        //pBtn->Align         = alBottom;
                        pBtn->Align         = alTop;
                        break;
                }
                pBtn->Layout        = FGlyphLayout; //FLayout == ibVertical ? blGlyphLeft : blGlyphTop;
                pBtn->Height        = 50;
                pBtn->Tag           = BtnList->Count;
                pBtn->GroupIndex    = 1;
                pBtn->AllowAllUp    = FAllowAllUp;
                pBtn->NumGlyphs     = 4;
                pBtn->OnClick       = MyClick;
                pBtn->OnMouseDown   = MyMouseDown;
                pBtn->OnMouseLeave  = MyMouseLeave;
                pBtn->OnMouseUp     = MyMouseUp;
                
                BtnList->Add(pBtn);

            }
        }
    }

    for(int i=0; i<BtnList->Count; i++) {
        TSpeedButton * pBtn = (TSpeedButton *)BtnList->Items[i];
        pBtn->Caption = ChangeString(FList->Strings[i], L"\\n", L"\n");
    }

    if((FIndex >= 0) && (BtnList->Count > 0) && (FIndex < BtnList->Count)) {
        GetSpeedButton(FIndex)->Down = true;
    }
    else {
        FIndex = -1;
    }
    
}
//---------------------------------------------------------------------------
void __fastcall TIndexButton::SetMatrixButtonAlign(int nMaxRow, int nMaxCol)
{
    if(nMaxRow <= 0 || nMaxCol <= 0) return;
    
    int nCellW      = this->Width / nMaxCol;
    int nCellLastW  = this->Width - (nCellW * (nMaxCol - 1));
    int nCellH      = this->Height / nMaxRow;
    int nCellLastH  = this->Height - (nCellH * (nMaxRow - 1));

    int nPosX = 0;
    int nPosY = 0;

    switch(FLayout) {
        case ibHorizontal:
            for(int row=0; row<nMaxRow; row++) {
                nPosX = 0;
                for(int col=0; col<nMaxCol; col++) {
                    int Index = (row * nMaxCol) + col;
                    
                    if(Index < BtnList->Count) {
                        TSpeedButton * pBtn = (TSpeedButton *)BtnList->Items[Index];

                        if(pBtn) {
                            pBtn->Width         = ((col + 1) == nMaxCol) ? nCellLastW : nCellW;
                            pBtn->Height        = ((row + 1) == nMaxRow) ? nCellLastH : nCellH;
                            pBtn->Top           = nPosY;
                            pBtn->Left          = nPosX;
                            pBtn->Margin        = FMargin;
                            pBtn->Spacing       = FSpacing;
                            if(FList) if(Index < FList->Count) pBtn->Caption = FList->Strings[Index];
                        }
                    }

                    nPosX += nCellW;
                }

                nPosY += nCellH;
            }
            break;

        case ibVertical:
        default:
            for(int col=0; col<nMaxCol; col++) {
                nPosY = 0;
                for(int row=0; row<nMaxRow; row++) {
                    int Index = (col * nMaxRow) + row;
                    
                    if(Index < BtnList->Count) {
                        TSpeedButton * pBtn = (TSpeedButton *)BtnList->Items[Index];

                        if(pBtn) {
                            pBtn->Width         = ((col + 1) == nMaxCol) ? nCellLastW : nCellW;
                            pBtn->Height        = ((row + 1) == nMaxRow) ? nCellLastH : nCellH;
                            
                            pBtn->Height        = nCellH;
                            pBtn->Width         = nCellW;
                            pBtn->Top           = nPosY;
                            pBtn->Left          = nPosX;

                            pBtn->Margin        = FMargin;
                            pBtn->Spacing       = FSpacing; 
                            if(FList) if(Index < FList->Count) pBtn->Caption = FList->Strings[Index];
                        }
                    }
                    nPosY += nCellH;
                }
                nPosX += nCellW;
            }
            break;
    }
}
//---------------------------------------------------------------------------
TSpeedButton *    __fastcall TIndexButton::GetSpeedButton(int Index)
{
    TSpeedButton * pBtn = NULL;

    if(BtnList->Count > Index) {
        pBtn = (TSpeedButton *)BtnList->Items[Index];
    }

    return pBtn;
}

TSpeedButton *    __fastcall TIndexButton::GetSpeedButtonMatrix(int nRow, int nCol)
{
    TSpeedButton * pBtn = NULL;

    if(FMatrixButton->Enabled) {
        int  nRowMax = FMatrixButton->RowCount;
        int  nColMax = FMatrixButton->ColCount;
        int  nIndex  = -1;
        
        switch(FLayout) {
            case ibVertical:
                nIndex = nCol * nRowMax + nRow;
                break;
            case ibHorizontal:
                nIndex = nRow * nColMax + nCol;
                break;
        }
        
        if(BtnList) {
            if((nIndex < BtnList->Count) && (nIndex >= 0)) {
                pBtn = (TSpeedButton *)BtnList->Items[Index];
            }
        }
    }

    return pBtn;
    
}

void __fastcall TIndexButton::SetList(System::Classes::TStrings * s)
{
    if(s == NULL) return;

    if(FList)FList->Assign(s);

    CreateButton();
    DisplayUpdate();

}

void __fastcall TIndexButton::SetColor(int Index, TColor c)
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

void __fastcall TIndexButton::SetBtnHeight(int n)
{
    if(FBtnHeight != n) {
        FBtnHeight = n;
        DisplayUpdate();
    }
}

void __fastcall TIndexButton::SetBtnWidth(int n)
{
    if(FBtnWidth != n) {
        FBtnWidth = n;
        DisplayUpdate();
    }
}

void __fastcall TIndexButton::SetMargin(int n)
{
    if(FMargin != n) {
        FMargin = n;
        DisplayUpdate();
    }
}

void __fastcall TIndexButton::SetSpacing(int n)
{
    if(FSpacing != n) {
        FSpacing = n;
        DisplayUpdate();
    }
}

void __fastcall TIndexButton::SetGlyphRound(int n)
{
    if(FGlyphRound != n) {
        FGlyphRound = n;
        DisplayUpdate();
    }
}

void __fastcall TIndexButton::SetGlyphWidth(int n)
{
    if(FGlyphWidth != n) {
        FGlyphWidth = n;
        //CreateButton();
        DisplayUpdate();
    }
}

void __fastcall TIndexButton::SetGlyphHeight(int n)
{
    if(FGlyphHeight != n) {
        FGlyphHeight = n;
        //CreateButton();
        DisplayUpdate();
    }
}

void __fastcall TIndexButton::SetGlyph(Vcl::Graphics::TBitmap* Value)
{
    if(Value == NULL) {
        FGlyph->Width  = 0;
        FGlyph->Height = 0;
    }
    else {
        FGlyph->Assign(Value);
    }

    DisplayUpdate();
}

Vcl::Graphics::TBitmap* __fastcall TIndexButton::GetGlyph(void)
{
    return FGlyph;
}


void __fastcall TIndexButton::GlyphChanged(System::TObject* Sender)
{
    DisplayUpdate();
}

void __fastcall TIndexButton::SetGlyphDefault(bool b)
{
    if(FGlyphDefault != b) {
        FGlyphDefault = b;
        DisplayUpdate();
    }
}

void    __fastcall  TIndexButton::SetGlyphLayout(TButtonLayout e)
{
    if(FGlyphLayout != e) {
        FGlyphLayout = e;
        CreateButton();
        DisplayUpdate();
    }
}


void __fastcall TIndexButton::SetLayout(TIndexButtonLayout e)
{
    if(FLayout != e) {
        FLayout = e;
        CreateButton();
        DisplayUpdate();
    }
}

int __fastcall TIndexButton::GetIndex()
{
    // Index를 Down된 Button을 찾아 맞춰준다. 
    TSpeedButton * pBtn = NULL;

    FIndex = -1;

    for(int i=0; i<BtnList->Count; i++) {
        pBtn = (TSpeedButton *)BtnList->Items[i];
        if(pBtn) {
            if(pBtn->Down) {
                FIndex = i;
                break;
            }
        }
    }

    return FIndex;
}

void __fastcall TIndexButton::SetIndex(int n)
{
    bool bApply = false;
    
    if(BtnList) {
        if((n < BtnList->Count) && (n >= 0)) {
            FIndex = n;
            GetSpeedButton(FIndex)->Down = true;
            bApply = true;
        }
        else if(n < 0) {
            FIndex = -1;
            
            if(FAllowAllUp) {
                // 모든 버튼 All Up 시킨다. 
                TSpeedButton * pBtn = NULL;

                for(int i=0; i<BtnList->Count; i++) {
                    pBtn = (TSpeedButton *)BtnList->Items[i];
                    if(pBtn) {
                        pBtn->Down = false;
                    }
                }
            }
            else {
                // FAllowAllUp 이 false일 경우 모든 버튼을 다시 만들어야 Down 모양이 없어진다. 
                CreateButton();
                DisplayUpdate();
            }

            bApply = true;
        }
    }

    // Index를 Down된 Button을 찾아 맞춰준다. 
    if(!bApply) {
        FIndex = -1;
        TSpeedButton * pBtn = NULL;

        for(int i=0; i<BtnList->Count; i++) {
            pBtn = (TSpeedButton *)BtnList->Items[i];
            if(pBtn) {
                if(pBtn->Down) {
                    FIndex = i;
                    break;
                }
            }
        }
    }
}

int  __fastcall TIndexButton::GetCount(void)
{
    int nCount = 0;
    
    if(BtnList) {
        nCount = BtnList->Count;
    }

    return nCount;
}

void __fastcall TIndexButton::SetButtonEnabled(int Index, bool b)
{
    if(BtnList) {
        if((Index < BtnList->Count) && (Index >= 0)) {
            GetSpeedButton(Index)->Enabled = b;
        }
    }
}

bool __fastcall TIndexButton::GetButtonEnabled(int Index)
{
    if(BtnList) {
        if((Index < BtnList->Count) && (Index >= 0)) {
            return GetSpeedButton(Index)->Enabled;
        }
    }

    return false;
}

void __fastcall TIndexButton::SetButtonEnabledMatrix(int nRow, int nCol, bool b)
{
    if(FMatrixButton->Enabled) {
        int  nRowMax = FMatrixButton->RowCount;
        int  nColMax = FMatrixButton->ColCount;
        int  nIndex  = -1;
        
        switch(FLayout) {
            case ibVertical:
                nIndex = nCol * nRowMax + nRow;
                break;
            case ibHorizontal:
                nIndex = nRow * nColMax + nCol;
                break;
        }
        
        if(BtnList) {
            if((nIndex < BtnList->Count) && (nIndex >= 0)) {
                GetSpeedButton(nIndex)->Enabled = b;
            }
        }
    }
}

bool __fastcall TIndexButton::GetButtonEnabledMatrix(int nRow, int nCol)
{
    if(FMatrixButton->Enabled) {
        int  nRowMax = FMatrixButton->RowCount;
        int  nColMax = FMatrixButton->ColCount;
        int  nIndex  = -1;
        
        switch(FLayout) {
            case ibVertical:
                nIndex = nCol * nRowMax + nRow;
                break;
            case ibHorizontal:
                nIndex = nRow * nColMax + nCol;
                break;
        }
        
        if(BtnList) {
            if((nIndex < BtnList->Count) && (nIndex >= 0)) {
                return GetSpeedButton(nIndex)->Enabled;
            }
        }
    }

    return false;
}

void __fastcall TIndexButton::SetDown(int nIndex, bool b)
{
    if(BtnList) {
        if((nIndex < BtnList->Count) && (nIndex >= 0)) {
            GetSpeedButton(nIndex)->Down = b;
            FindDownIndex();
        }
    }
}

bool __fastcall TIndexButton::GetDown(int nIndex)
{
    bool bDown = false;
    
    if(BtnList) {
        if((nIndex < BtnList->Count) && (nIndex >= 0)) {
            bDown = GetSpeedButton(nIndex)->Down;
            FindDownIndex();
        }
    }

    return bDown;
}

void __fastcall TIndexButton::SetDownMatrix(int nRow, int nCol, bool b)
{
    if(FMatrixButton->Enabled) {
        if(BtnList) {
            int nIndex = FindIndex(nRow, nCol);
            
            if((nIndex < BtnList->Count) && (nIndex >= 0)) {
                GetSpeedButton(nIndex)->Down = b;
                FindDownIndex();
            }
        }
    }
}

bool __fastcall TIndexButton::GetDownMatrix(int nRow, int nCol)
{
    bool bDown = false;
    
    if(FMatrixButton->Enabled) {
        if(BtnList) {
            int nIndex = FindIndex(nRow, nCol);
            
            if((nIndex < BtnList->Count) && (nIndex >= 0)) {
                bDown = GetSpeedButton(nIndex)->Down;
                FindDownIndex();
            }
        }
    }

    return bDown;
}

void    __fastcall TIndexButton::SetCaption(int n, String s)
{
    if(n < FList->Count && n >= 0) {
        ((System::Classes::TStringList *)FList)->OnChange = NULL;
        FList->Strings[n] = s;
        ((System::Classes::TStringList *)FList)->OnChange = ListChanged;
    }

    if(n < BtnList->Count) {
        TSpeedButton * pBtn = GetSpeedButton(n);
        if(pBtn) {
            pBtn->Caption = s;
        }
    }
}

String  __fastcall TIndexButton::GetCaption(int n)
{
    String sRet;
    
    if(n < BtnList->Count) {
        TSpeedButton * pBtn = GetSpeedButton(n);
        if(pBtn) {
            sRet = pBtn->Caption;
        }
    }

    return sRet;
}

void    __fastcall TIndexButton::SetCaptionMatrix(int nRow, int nCol, String s)
{
    if(FMatrixButton->Enabled) {
        int  nRowMax = FMatrixButton->RowCount;
        int  nColMax = FMatrixButton->ColCount;
        int  nIndex  = -1;
        
        switch(FLayout) {
            case ibVertical:
                nIndex = nCol * nRowMax + nRow;
                break;
            case ibHorizontal:
                nIndex = nRow * nColMax + nCol;
                break;
        }
       
        if(FList) {
            if((nIndex < FList->Count) && (nIndex >=0)) {
                ((System::Classes::TStringList *)FList)->OnChange = NULL;
                FList->Strings[nIndex] = s;
                ((System::Classes::TStringList *)FList)->OnChange = ListChanged;                
            }
        }
        
        if(BtnList) {
            if((nIndex < BtnList->Count) && (nIndex >= 0)) {
                GetSpeedButton(nIndex)->Caption = s;
            }
        }
    }
}

String  __fastcall TIndexButton::GetCaptionMatrix(int nRow, int nCol)
{
    String sRet;

    if(FMatrixButton->Enabled) {
        int  nRowMax = FMatrixButton->RowCount;
        int  nColMax = FMatrixButton->ColCount;
        int  nIndex  = -1;
        
        switch(FLayout) {
            case ibVertical:
                nIndex = nCol * nRowMax + nRow;
                break;
            case ibHorizontal:
                nIndex = nRow * nColMax + nCol;
                break;
        }
        
        if(BtnList) {
            if((nIndex < BtnList->Count) && (nIndex >= 0)) {
                sRet = GetSpeedButton(nIndex)->Caption;
            }
        }
    }

    return sRet;
}

void    __fastcall TIndexButton::SetAllowAllUp(bool b)
{
    if(FAllowAllUp != b) {
        FAllowAllUp = b;

        TSpeedButton * pBtn = NULL;
        
        for(int i=0; i<BtnList->Count; i++) {
            pBtn = (TSpeedButton *)BtnList->Items[i];
            if(pBtn) {
                pBtn->AllowAllUp = FAllowAllUp;
            }
        }
    }
}

   
void __fastcall TIndexButton::MatrixPropertyChanged(System::TObject* Sender)
{
    CreateButton();
    DisplayUpdate();
}

void __fastcall TIndexButton::SetMatrixButton(TMatrixButtonProperty * p)
{
    if(p) {
        FMatrixButton->Assign(p);
    }
}
//-----------------------------------------------------------------------------

void __fastcall TIndexButton::NumberingPropertyChanged(System::TObject* Sender)
{
    DisplayUpdate();
}

void __fastcall TIndexButton::SetNumbering(TIndexBtnNumberingProperty * p)
{
    if(p) {
        FNumbering->Assign(p);
    }
}
//=============================================================================
__fastcall  TMatrixButtonProperty::TMatrixButtonProperty()
{
    FEnabled     = false;
    FColCount    = 0;
    FRowCount    = 0;
}

__fastcall TMatrixButtonProperty::~TMatrixButtonProperty()
{
    //
}

void    __fastcall  TMatrixButtonProperty::SetEnabled(bool b)
{
    if(FEnabled != b) {
        FEnabled = b;
        DoOnChange();
    }
}

void    __fastcall  TMatrixButtonProperty::SetColCount(int n)
{
    if(FColCount != n) {
        FColCount = n;
        DoOnChange();
    }
}

void    __fastcall  TMatrixButtonProperty::SetRowCount(int n)
{
    if(FRowCount != n) {
        FRowCount = n;
        DoOnChange();
    }
}

void    __fastcall  TMatrixButtonProperty::DoOnChange(void)
{
    if(FOnChange) FOnChange(this);
}

void      __fastcall  TMatrixButtonProperty::Assign(TPersistent* Source)
{
    TMatrixButtonProperty* pFrom = dynamic_cast<TMatrixButtonProperty *>(Source);

    if(pFrom != NULL) //Source 가 NULL 이 아니고 안전하게 타입캐스팅 되었다면
    {
        FEnabled    = pFrom->FEnabled;
        FColCount   = pFrom->FColCount;
        FRowCount   = pFrom->FRowCount;

        DoOnChange();
    }
}

//=============================================================================
__fastcall  TIndexBtnNumberingProperty::TIndexBtnNumberingProperty()

{
    FEnabled         = false;
    FStartNumber     = 0;
    FFont = new Vcl::Graphics::TFont;          ///< 폰트 특성
    FFont->OnChange  = FontChanged;
}

 __fastcall TIndexBtnNumberingProperty::~TIndexBtnNumberingProperty()
 {
    if(FFont) delete FFont;
 }

 
void    __fastcall  TIndexBtnNumberingProperty::DoOnChange(void)
{
    if(FOnChange) FOnChange(this);
}

void    __fastcall  TIndexBtnNumberingProperty::FontChanged(System::TObject* Sender)
{
    DoOnChange();
}

void    __fastcall  TIndexBtnNumberingProperty::SetEnabled(bool b)
{
    if(FEnabled != b) {
        FEnabled = b;
        DoOnChange();
    }
}

void    __fastcall  TIndexBtnNumberingProperty::SetStartNumber(int n)
{
    if(FStartNumber != n) {
        FStartNumber = n;
        DoOnChange();
    }
}

void    __fastcall  TIndexBtnNumberingProperty::SetFont(Vcl::Graphics::TFont * p)
{
    if(p) {
        FFont->Assign(p);
        DoOnChange();
    }
}

void    __fastcall  TIndexBtnNumberingProperty::Assign(TPersistent* Source)
{
    TIndexBtnNumberingProperty* pFrom = dynamic_cast<TIndexBtnNumberingProperty *>(Source);

    if(pFrom) {
        FEnabled     = pFrom->Enabled;
        FStartNumber = pFrom->StartNumber;
        FFont->Assign(pFrom->Font);
    }
}



