//---------------------------------------------------------------------------

#pragma hdrstop

#include "AZNumShape.h"
#include "Common\\Define.h"
#include "GlobalFunction_AZ.h"
#include <Windowsx.h>   // GET_X_LPARAM macro 사용 위해.
#include <math.h>
//---------------------------------------------------------------------------
/**
    frmNumPad.cpp를 프로젝트에 포함하여 같이 컴파일 해도 되지만
    이렇게 obj 파일을 링키시켜 사용해도 된다.
    해더 파일은 동일하게 include 시켜주면 된다.
 */
#include "frmNumPad_AZ.h"

#pragma link "AZUnitProperty.obj"

//---------------------------------------------------------------------------
#pragma package(smart_init)


#define ROUNDING(x, dig)    ( floor((x) * pow(double(10), (dig)) + 0.5) / pow(double(10), (dig)) )

double Rounding( double x, int digit )
{
    return ( floor( (x) * pow( double(10), (digit) ) + 0.5) / pow( double(10), (digit) ) );
}

#define DEF_HEIGHT_RATIO_2 1.65

bool gAZDigitPixDot[12][7] = {
	{1,1,1,1,1,1,0},	// 0
	{0,0,0,0,1,1,0},    // 1
	{1,0,1,1,0,1,1}, 	// 2
	{1,0,0,1,1,1,1},    // 3
	{0,1,0,0,1,1,1},    // 4
	{1,1,0,1,1,0,1},    // 5
	{1,1,1,1,1,0,1},    // 6
	{1,1,0,0,1,1,0},    // 7
	{1,1,1,1,1,1,1},    // 8
	{1,1,0,0,1,1,1},    // 9
	{0,0,0,0,0,0,0},    // 10 (그림자)
	{0,0,0,0,0,0,1},    // 11 (마이너스)
};

//-----------------------------------------------------------------------

//#define TEXT_ALIGN_TOP      0x01
//#define TEXT_ALIGN_LEFT     0x02
//#define TEXT_ALIGN_RIGHT    0x04
//#define TEXT_ALIGN_BOTTOM   0x08
//#define TEXT_ALIGN_H_CENTER 0x10
//#define TEXT_ALIGN_V_CENTER 0x20
//-----------------------------------------------------------------------
static inline void ValidCtrCheck(TAZNumShape *)
{
    new TAZNumShape(NULL);
}
//-----------------------------------------------------------------------
__fastcall TAZNumShape::TAZNumShape(TComponent* Owner)
: TGraphicControl(Owner)
{
//    //-------------------------------------
//    if(pList == NULL) pList = new TList;
//    pList->Add(this);
//    //-------------------------------------
    m_nMouseDownXPos = 0;
    m_nMouseDownYPos = 0;

    bLoaded                 = false;
    bSelecting              = false;
    bChanged                = false;

    // Event 함수 등록.
    TGraphicControl::OnClick = MyClick;

    // 기본 Property default set.
    Width                   = 200;
    Height                  = 50;
    Caption                 = "";

    // Font Default set.
    Font->Name              = AZ_NUM_SHAPE_FONT;
    Font->Size              = 22;
    Font->Style             = TFontStyles() >> fsBold;

    // Property default set.
    //FType                   = itFloat;
    //FMin                    = -1000;
    //FMax                    = 1000;
    //FUpDownValue            = 1;
    //FDecimalPlaces          = 3 ;

    FValueOld               = 0;
    FValue                  = 0;
    FSpace                  = 0;
    FRound                  = 5;
    FEditWidth              = 100;
    FEditHeight             = 50;

    FOldValueDisplay    = true;
    FOldValueUnderline  = true;
    FOldValueFontSize   = 0;

    FNoEdit                = false;

    FAlignment          = taRightJustify;

    FInvalidateUse      = false;

    bBeginUpdate        = false;

    m_bEncoderButtonEnable  = false;
    m_pEncoderAZNumShape    = NULL;
    m_sEncoderTitle         = "";
    m_sEncoderUnit          = "";
    

    //------------------------------------------------
    // Unit Property 생성.
    FUnit             = new TNumUnitProperty();
    FUnit->OnChange    = UnitChanged;

    //------------------------------------------------
    // Color Property 생성.
    FColorSet = new TNumShapeColorProperty();
    FColorSet->OnChange = ColorChanged;

    //------------------------------------------------
    // Input Edit Property 생성.
    FInputEdit = new TAZEditBoxProperty();
    FInputEdit->OnChange = InputEditChanged;

//    //------------------------------------------------
//    // 제목을 표기할 Label 생성.
//    FLabel = new TBoundLabel(Owner);
//    FLabel->Caption = "TEST";
//    FLabel->Visible = true;
//    TWinControl * pWinCtrl = dynamic_cast<TWinControl *>(Owner);
//    if(pWinCtrl)FLabel->Parent  = pWinCtrl;
//
//
//    FLabelPosition    = lpLeft;
//    FLabelSpacing    = 10;

    tmpBitmap1  = new Graphics::TBitmap;
    tmpBitmap2  = new Graphics::TBitmap;
    pBitmapTemp = new Graphics::TBitmap;
    //-------------------------------------------------
    FTitle = new TNumShapeTitleProperty();
    FTitle->OnChange = TitleChanged;


    m_nTitleWidth  = 0; 
    m_nTitleHeight = 0; 

    //--------------------------
    if(ComponentState.Contains(csDesigning)) {
        bLoaded = true;
    }    
    
}
//-----------------------------------------------------------------------
__fastcall TAZNumShape::~TAZNumShape()
{
    if(FUnit)       delete FUnit;
    if(FColorSet)   delete FColorSet;
    if(FInputEdit)  delete FInputEdit;
    if(tmpBitmap1)  delete tmpBitmap1;
    if(tmpBitmap2)  delete tmpBitmap2;
    if(pBitmapTemp) delete pBitmapTemp;
    if(FTitle)      delete FTitle;
//    if(pList) {
//        for(int i=0; i<pList->Count; i++) {
//            if(pList->Items[i] == this) {
//                pList->Delete(i);
//                if(pList->Count == 0) {
//                    delete pList;
//                    pList = NULL;
//                }
//                break;
//            }
//        }
//    }
    //if(FLabel) delete FLabel;
}
//---------------------------------------------------------------------------
// dfm 으로 부터 속성값을 모두 로드한 후 이 함수가 호출 됨.
void __fastcall TAZNumShape::Loaded(void)
{
    TGraphicControl::Loaded();

    //MessageBox(NULL, "Loaded", "", MB_OK);

    //CreatePanel(FRowCount, FColCount);

    //FLabel->Left = Left - FLabel->Width;
    //FLabel->Top     = Top;

    FValueOld = FValue;

    bLoaded = true;

    Invalidate();
}
//---------------------------------------------------------------------------
// 메시지 트래핑.
void __fastcall TAZNumShape::WndProc(TMessage& Message)
{
    switch(Message.Msg) {
         case WM_LBUTTONDOWN:
            if(FReadOnly) break;
            if(ComponentState.Contains(csDesigning) != true) { // 디자인 타임에는 적용되지 않도록한다.

                m_nMouseDownXPos = GET_X_LPARAM(Message.LParam);
                m_nMouseDownYPos = GET_Y_LPARAM(Message.LParam);

                bSelecting = true; 

                if(FTitle->Visible) {
                    switch(Title->Align) {
                        case caLeft:    if((Width - FEditWidth) > m_nMouseDownXPos)     bSelecting = false; break;
                        case caRight:   if(FEditWidth < m_nMouseDownXPos)               bSelecting = false; break;
                        case caTop:     if((Height - FEditHeight) > m_nMouseDownYPos)   bSelecting = false; break;
                        case caBottom:  if(FEditHeight  < m_nMouseDownYPos)             bSelecting = false; break;
                    }
                }
                
                if(bSelecting) Invalidate();
            }
            break;

         case WM_LBUTTONUP:
            if(FReadOnly) break;
            if(ComponentState.Contains(csDesigning) != true) { // 디자인 타임에는 적용되지 않도록한다.
                if(bSelecting) Invalidate();
                bSelecting = false;
            }
            break;

         case WM_MOUSEHOVER:
            break;

         case WM_MOUSELEAVE:
            if(FReadOnly) break;
            if(ComponentState.Contains(csDesigning) != true) { // 디자인 타임에는 적용되지 않도록한다.
                if(bSelecting) Invalidate();
                bSelecting = false;
            }
            break;

         case WM_NUMSHAPE_RESET:
            if(Message.LParam != (int)this ) {
                if(bChanged) {
                    bChanged = false;
                    Invalidate();
                }
            }
            break;
    }

    switch(Message.Msg) {
        default:
            TGraphicControl::WndProc(Message);
            break;
    }

}


//---------------------------------------------------------------------------
// Enable / Disable 시에 색상 지정.
void __fastcall TAZNumShape::SetEnabled(bool b)
{
    TGraphicControl::SetEnabled(b);
}


//-----------------------------------------------------------------------
//void __fastcall TAZNumShape::WMPaint(TWMPaint& Message)
//{
//
//}

//-----------------------------------------------------------------------
void __fastcall TAZNumShape::Paint()
{
#if 1

    PaintTitle();
    PaintValue();
    
#else
    if(!Visible || bBeginUpdate) return;

    // -------------------------------------------------------

    Vcl::Graphics::TCanvas* BuffCanvas = NULL;
    int nTxtW = 0, nTxtH = 0;

    //FEditWidth = FEditWidth > Width ? Width : FEditWidth;

    // TITLE 그리기.
    if(FTitle->Visible) {
        int nTitleWidth  = 0;       // Title 이 차지하는 전체 크기
        int nTitleHeight = 0;
        int nRoundWidth  = 0;       // Unit과의 간격을 유지하는 크기인 Space 공간을 제외한 크기.
        int nRoundHeight = 0;
        int nMargin      = FTitle->Margin;

        switch(Title->Align) {
            case caLeft:
            case caRight:
                nTitleWidth  = Width - FEditWidth;
                nTitleHeight = Height;
                nRoundWidth  = nTitleWidth - FTitle->Space;
                nRoundHeight = nTitleHeight;
                break;
                
            case caTop:
            case caBottom:
                nTitleWidth  = Width;
                nTitleHeight = Height - FEditHeight;
                nRoundWidth  = Width;
                nRoundHeight = nTitleHeight - FTitle->Space;
                break;
        }
        
        nTitleWidth  = nTitleWidth  < 0 ? 0 : nTitleWidth;
        nTitleHeight = nTitleHeight < 0 ? 0 : nTitleHeight;
        nRoundWidth  = nRoundWidth  < 0 ? 0 : nRoundWidth;
        nRoundHeight = nRoundHeight < 0 ? 0 : nRoundHeight;

        m_nTitleWidth  = nTitleWidth; 
        m_nTitleHeight = nTitleHeight; 

        tmpBitmap1->Width  = m_nTitleWidth;
        tmpBitmap1->Height = m_nTitleHeight;

        TColor clTransparentColor = FTitle->Transparent ? Parent->Brush->Color : MY_TRANSPARENTCOLOR;

        if(tmpBitmap1->Width > 0 && tmpBitmap1->Height > 0) {
            
            BuffCanvas = tmpBitmap1->Canvas;
            SetBkMode(BuffCanvas->Handle, TRANSPARENT);

            tmpBitmap1->Transparent          = true;
            tmpBitmap1->TransparentColor     = clTransparentColor; // Parent->Brush->Color;
            tmpBitmap1->Canvas->Brush->Color = clTransparentColor; //FTitle->Transparent ? Parent->Brush->Color : FTitle->Color;
            tmpBitmap1->Canvas->FillRect(Rect(0,0, nTitleWidth, nTitleHeight));

//            if(FTitle->OutLineVisible) {

                if(FTitle->Transparent) {
                    BuffCanvas->Brush->Color = clTransparentColor;
                }
                else {
                    BuffCanvas->Brush->Color = Enabled ? FTitle->Color : clBtnFace;
                }
                
                BuffCanvas->Brush->Style = bsSolid;

                if(FTitle->OutLineVisible) {
                    BuffCanvas->Pen->Color   =  Enabled ? FTitle->OutLineColor : clGray;
                }
                else {
                    BuffCanvas->Pen->Color   =  BuffCanvas->Brush->Color;
                }
                
                switch(Title->Align) {
                    case caLeft:    BuffCanvas->RoundRect(0, 0, nRoundWidth, nRoundHeight, FTitle->Round, FTitle->Round); break;
                    case caRight:   BuffCanvas->RoundRect(FTitle->Space, 0, nTitleWidth, nRoundHeight, FTitle->Round, FTitle->Round); break;
                    case caTop:     BuffCanvas->RoundRect(0, 0, nRoundWidth, nRoundHeight, FTitle->Round, FTitle->Round); break;
                    case caBottom:  BuffCanvas->RoundRect(0, FTitle->Space, nRoundWidth, nTitleHeight, FTitle->Round, FTitle->Round); break;
                }
                
//            }
//            else {
//                tmpBitmap1->Transparent          = FTitle->Transparent;
//                tmpBitmap1->TransparentColor     = clTransparentColor; //Parent->Brush->Color;
//                tmpBitmap1->Canvas->Brush->Color = FTitle->Transparent ? clTransparentColor : FTitle->Color;
//                tmpBitmap1->Canvas->FillRect(Rect(0,0, nTitleWidth, nTitleHeight));
//            }

            BuffCanvas->Font->Assign(FTitle->Font);

            if(!Enabled) BuffCanvas->Font->Color = clGray;

            nTxtW = BuffCanvas->TextWidth(FTitle->Caption);
            nTxtH = BuffCanvas->TextHeight(FTitle->Caption);

            int nTxtSTartX = 0;
            int nTxtStartY = 0;

            switch(Title->Align) {
                case caLeft:    nTxtSTartX = 0;  nTxtStartY = 0; break;
                case caRight:   nTxtSTartX = FTitle->Space;  nTxtStartY = 0; break;
                case caTop:     nTxtSTartX = 0;  nTxtStartY = 0; break;
                case caBottom:  nTxtSTartX = 0;  nTxtStartY = FTitle->Space; break;
            }
            
            switch(FTitle->Alignment ) {
                case taLeftJustify:
                        BuffCanvas->TextOutA( nTxtSTartX + 1 + nMargin, nTxtStartY + (nRoundHeight - nTxtH)/2, FTitle->Caption);
                        break;
                case taRightJustify:
                        BuffCanvas->TextOutA( nTxtSTartX + nRoundWidth - nTxtW - 1 - nMargin, nTxtStartY + (nRoundHeight - nTxtH)/2, FTitle->Caption);
                        break;
                case taCenter:
                        BuffCanvas->TextOutA( nTxtSTartX + (nRoundWidth - nTxtW) / 2, nTxtStartY + (nRoundHeight - nTxtH)/2, FTitle->Caption);
                        break;
            }

            if(FTitle->OutLineVisible) {
                //BuffCanvas->Brush->Color = FTitle->Color; //static_cast<TColor>(RGB(0, 0, 255));
                BuffCanvas->Brush->Style = bsClear;
                BuffCanvas->Pen->Color = Enabled ? FTitle->OutLineColor : clGray;

                switch(Title->Align) {
                    case caLeft:    BuffCanvas->RoundRect(0, 0, nRoundWidth, nRoundHeight, FTitle->Round, FTitle->Round); break;
                    case caRight:   BuffCanvas->RoundRect(FTitle->Space, 0, nTitleWidth, nRoundHeight, FTitle->Round, FTitle->Round); break;
                    case caTop:     BuffCanvas->RoundRect(0, 0, nRoundWidth, nRoundHeight, FTitle->Round, FTitle->Round); break;
                    case caBottom:  BuffCanvas->RoundRect(0, FTitle->Space, nRoundWidth, nTitleHeight, FTitle->Round, FTitle->Round); break;
                }
            }
        }
    }

    // -------------------------------------------------------
    // EDIT 그리기.
    int nEditWidth  = 0;
    int nEditHeight = 0;
    int nUnitWidth = FUnit->Visible ? FUnit->Width : 0;

    switch(Title->Align) {
        case caLeft:
        case caRight:
            nEditWidth  = FTitle->Visible ? FEditWidth : this->Width;
            nEditHeight = this->Height;
            break;
            
        case caTop:
        case caBottom:
            nEditWidth  = this->Width;
            nEditHeight = FTitle->Visible ? FEditHeight : this->Height;
            break;
    }
    
    tmpBitmap2->Width  = nEditWidth;
    tmpBitmap2->Height = nEditHeight;

    BuffCanvas = tmpBitmap2->Canvas;

    SetBkMode(BuffCanvas->Handle, TRANSPARENT);

    tmpBitmap2->Transparent = true;
    tmpBitmap2->TransparentColor     = MY_TRANSPARENTCOLOR; //Parent->Brush->Color;
    tmpBitmap2->Canvas->Brush->Color = MY_TRANSPARENTCOLOR; //Parent->Brush->Color;
    tmpBitmap2->Canvas->FillRect(Rect(0,0, nEditWidth, nEditHeight));

    // ---------------------------------------
    // 외곽선  및 Unit 그리기.
    if(Enabled) {
        if(bSelecting) {
            BuffCanvas->Pen->Color      = FColorSet->ClickRectColor;
            BuffCanvas->Brush->Color    = FColorSet->ClickBGColor;
            BuffCanvas->Font->Color     = FUnit->Font->Color;
        }
        else if(bNumbering){
            BuffCanvas->Pen->Color      = FColorSet->ClickRectColor;
            BuffCanvas->Brush->Color    = FColorSet->ClickBGColor;
            BuffCanvas->Font->Color     = FUnit->Font->Color;
        }
        else {
            BuffCanvas->Pen->Color      = FColorSet->RectColor;
            BuffCanvas->Brush->Color    = FColorSet->BGColor;
            BuffCanvas->Font->Color     = FUnit->Font->Color;
        }
    }
    else {
        BuffCanvas->Pen->Color          = clGray;
        BuffCanvas->Brush->Color        = clBtnFace;
        BuffCanvas->Font->Color         = clGray;
    }

    BuffCanvas->RoundRect(0, 0, nEditWidth, nEditHeight, FRound, FRound);
    //if(bSelecting || bNumbering) BuffCanvas->DrawFocusRect(Rect(2,2,nEditWidth-2, Height-2));

    // 반사광 그리기. ------------
    int nRGB = ColorToRGB(BuffCanvas->Brush->Color);
    int nR = (nRGB & 0xFF0000) >> 16;
    int nG = (nRGB & 0x00FF00) >> 8;
    int nB = (nRGB & 0x0000FF);

    nR += 15;
    nG += 15;
    nB += 15;

    nR = nR > 255 ? 255 : nR;
    nG = nG > 255 ? 255 : nG;
    nB = nB > 255 ? 255 : nB;

    nRGB = (nR << 16) | (nG << 8) | nB;
    TColor ColorTmp         = (TColor)nRGB;
    TColor ColorBGOld       = BuffCanvas->Brush->Color;
    TColor ColorFTOld       = BuffCanvas->Pen->Color;
    BuffCanvas->Brush->Color    = ColorTmp;
    BuffCanvas->Pen->Color      = ColorTmp;
    BuffCanvas->RoundRect(1, 1, nEditWidth-1, nEditHeight/2, FRound-1, FRound-1);
    BuffCanvas->Brush->Color    = ColorBGOld;
    BuffCanvas->Pen->Color      = ColorFTOld;


    // ---------------------------------------

    if(FUnit->Visible) {
        SetBkMode(BuffCanvas->Handle, TRANSPARENT); // 투명 출력.

        BuffCanvas->Font->Assign(FUnit->Font);

        nTxtW = BuffCanvas->TextWidth(FUnit->Str);
        nTxtH = BuffCanvas->TextHeight(FUnit->Str);
        // Unit Align CENTER.
        BuffCanvas->TextOutA(nEditWidth - nUnitWidth + ((nUnitWidth - nTxtW) / 2) , (nEditHeight - nTxtH)/2, FUnit->Str);
    }
    // ---------------------------------------
    // 이전값 다시 보기 그리기.
    int nStartY = 0;
    AnsiString sOutputText;

    if(FOldValueDisplay) {

        sOutputText = GetDrawValue(FValueOld, FInputEdit->Type, FInputEdit->DecimalPlaces);

        BuffCanvas->Font->Assign(Font);

        int nTotalTxtH = BuffCanvas->TextHeight(sOutputText);             // Value의 폰트 높이.

        if(FOldValueFontSize <= 0) {
            BuffCanvas->Font->Size  = BuffCanvas->Font->Size / 2;
        }
        else {
            BuffCanvas->Font->Size  = FOldValueFontSize;
        }

        if(FOldValueUnderline) {
            BuffCanvas->Font->Style = TFontStyles() >> fsBold << fsUnderline;
        }
        else {
            BuffCanvas->Font->Style = TFontStyles() >> fsBold >> fsUnderline;
        }
        
        //BuffCanvas->Font->Name  = "Arial";

        nTotalTxtH = nTotalTxtH + BuffCanvas->TextHeight(sOutputText);     // Value와 OldValue를 합한 폰트 높이.

        if(Enabled) {
            BuffCanvas->Font->Color     = FColorSet->OldValueColor;
        }
        else {
            BuffCanvas->Font->Color     = clGray;
        }

        int nTemp = nEditHeight - nTotalTxtH;

        int nTop    = (nTemp * 45) / 100;
        //int nMiddle = (nTemp * 10) / 100;
        //int nBottom = nTemp - nTop - nMiddle;

        int SubnTxtW = BuffCanvas->TextWidth(sOutputText);
        int SubnTxtH = BuffCanvas->TextHeight(sOutputText);

        int nStartX = 0;

        switch(FAlignment ) {
            case taLeftJustify:
                    nStartX = 2 + FSpace;
                    break;
            case taRightJustify:
                      nStartX = (nEditWidth - nUnitWidth - SubnTxtW - 2) - FSpace;
                    break;
            case taCenter:
                    nStartX = (nEditWidth - nUnitWidth - SubnTxtW) / 2;
                    break;
        }

        nStartY = nTop+1;

        SetBkMode(BuffCanvas->Handle, TRANSPARENT); // 투명 출력.
        BuffCanvas->TextOutA(nStartX, nStartY, sOutputText);

        nStartY = nStartY + SubnTxtH - 3;
    }


    // ---------------------------------------
    // Value 출력
    BuffCanvas->Font->Assign(Font);

    sOutputText = GetDrawValue(FValue, FInputEdit->Type, FInputEdit->DecimalPlaces);

    if(Enabled) {
        if(FReadOnly) {
            BuffCanvas->Font->Color        = FColorSet->FontColor;
        }
        else if(bSelecting || bNumbering) {
            BuffCanvas->Font->Color        = FColorSet->ClickFontColor;
        }
        else {
            BuffCanvas->Font->Color        = bChanged ? FColorSet->ChangedFontColor : FColorSet->FontColor;
        }
    }
    else {
        BuffCanvas->Font->Color            = clGray;
    }

    nTxtW = BuffCanvas->TextWidth(sOutputText);
    nTxtH = BuffCanvas->TextHeight(sOutputText);

    int nStartX = 0;

    switch(FAlignment ) {
        case taLeftJustify:
            nStartX = 2 + FSpace;
            break;
        case taRightJustify:
              nStartX = (nEditWidth - nUnitWidth - nTxtW - 2) - FSpace;
            break;
        case taCenter:
            nStartX = (nEditWidth - nUnitWidth - nTxtW) / 2;
            break;
    }

    nStartX = nStartX < 0 ? 0 : nStartX;
    
    SetBkMode(BuffCanvas->Handle, TRANSPARENT); // 투명 출력.
    BuffCanvas->TextOutA(nStartX, nStartY + (((nEditHeight - nStartY) - nTxtH)/2), sOutputText);

    if((tmpBitmap1->Width == 0 || tmpBitmap1->Height == 0)
    && (tmpBitmap2->Width == 0 || tmpBitmap2->Height == 0)) {
        return; 
    }
    
    Canvas->Lock();

    if(FTitle->Visible) {

        if(tmpBitmap1->Width == 0 || tmpBitmap1->Height == 0) {
            Canvas->Draw(0,0,tmpBitmap2);
        }
        else {
            switch(Title->Align) {
                case caLeft:
                    Canvas->Draw(0,0,tmpBitmap1);
                    Canvas->Draw(tmpBitmap1->Width,0,tmpBitmap2);
                    break;
                    
                case caRight:
                    Canvas->Draw(0,0,tmpBitmap2);
                    Canvas->Draw(tmpBitmap2->Width,0,tmpBitmap1);
                    break;
                    
                case caTop:
                    Canvas->Draw(0,0,tmpBitmap1);
                    Canvas->Draw(0,tmpBitmap1->Height,tmpBitmap2);
                    break;
                    
                case caBottom:
                    Canvas->Draw(0,0,tmpBitmap2);
                    Canvas->Draw(0,tmpBitmap2->Height,tmpBitmap1);                
                    break;
            }
        }
    }
    else {

        if(tmpBitmap2->Width > 0 && tmpBitmap2->Height > 0) {
            Canvas->Draw(0,0,tmpBitmap2);
        }
    }

    Canvas->Unlock();

#endif 
}

//-----------------------------------------------------------------------
void __fastcall TAZNumShape::PaintTitle()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(bBeginUpdate || !bLoaded) return;
    
    Vcl::Graphics::TCanvas* BuffCanvas = NULL;
    int nTxtW = 0, nTxtH = 0;

    // TITLE 그리기.
    if(FTitle->Visible) {
        int nTitleWidth  = 0;       // Title 이 차지하는 전체 크기
        int nTitleHeight = 0;
        int nRoundWidth  = 0;       // Unit과의 간격을 유지하는 크기인 Space 공간을 제외한 크기.
        int nRoundHeight = 0;
        int nMargin      = FTitle->Margin;

        switch(Title->Align) {
            case caLeft:
            case caRight:
                nTitleWidth  = Width - FEditWidth;
                nTitleHeight = Height;
                nRoundWidth  = nTitleWidth - FTitle->Space;
                nRoundHeight = nTitleHeight;
                break;
                
            case caTop:
            case caBottom:
                nTitleWidth  = Width;
                nTitleHeight = Height - FEditHeight;
                nRoundWidth  = Width;
                nRoundHeight = nTitleHeight - FTitle->Space;
                break;
        }
        
        nTitleWidth  = nTitleWidth  < 0 ? 0 : nTitleWidth;
        nTitleHeight = nTitleHeight < 0 ? 0 : nTitleHeight;
        nRoundWidth  = nRoundWidth  < 0 ? 0 : nRoundWidth;
        nRoundHeight = nRoundHeight < 0 ? 0 : nRoundHeight;

        m_nTitleWidth  = nTitleWidth; 
        m_nTitleHeight = nTitleHeight; 

        tmpBitmap1->Width  = m_nTitleWidth;
        tmpBitmap1->Height = m_nTitleHeight;

        TColor clTransparentColor = FTitle->Transparent ? Parent->Brush->Color : MY_TRANSPARENTCOLOR;

        if(tmpBitmap1->Width > 0 && tmpBitmap1->Height > 0) {
            
            BuffCanvas = tmpBitmap1->Canvas;
            SetBkMode(BuffCanvas->Handle, TRANSPARENT);

            tmpBitmap1->Transparent          = true;
            tmpBitmap1->TransparentColor     = clTransparentColor; // Parent->Brush->Color;
            tmpBitmap1->Canvas->Brush->Color = clTransparentColor; //FTitle->Transparent ? Parent->Brush->Color : FTitle->Color;
            tmpBitmap1->Canvas->FillRect(Rect(0,0, nTitleWidth, nTitleHeight));

//            if(FTitle->OutLineVisible) {

                BuffCanvas->Brush->Color = FTitle->Transparent ? clTransparentColor : FTitle->Color; //static_cast<TColor>(RGB(0, 0, 255));
                BuffCanvas->Brush->Style = bsSolid;
                BuffCanvas->Pen->Color   = FTitle->OutLineVisible ? FTitle->OutLineColor : BuffCanvas->Brush->Color;
                
                switch(Title->Align) {
                    case caLeft:    BuffCanvas->RoundRect(0, 0, nRoundWidth, nRoundHeight, FTitle->Round, FTitle->Round); break;
                    case caRight:   BuffCanvas->RoundRect(FTitle->Space, 0, nTitleWidth, nRoundHeight, FTitle->Round, FTitle->Round); break;
                    case caTop:     BuffCanvas->RoundRect(0, 0, nRoundWidth, nRoundHeight, FTitle->Round, FTitle->Round); break;
                    case caBottom:  BuffCanvas->RoundRect(0, FTitle->Space, nRoundWidth, nTitleHeight, FTitle->Round, FTitle->Round); break;
                }
                
//            }
//            else {
//                tmpBitmap1->Transparent          = FTitle->Transparent;
//                tmpBitmap1->TransparentColor     = clTransparentColor; //Parent->Brush->Color;
//                tmpBitmap1->Canvas->Brush->Color = FTitle->Transparent ? clTransparentColor : FTitle->Color;
//                tmpBitmap1->Canvas->FillRect(Rect(0,0, nTitleWidth, nTitleHeight));
//            }

            BuffCanvas->Font->Assign(FTitle->Font);

            nTxtW = BuffCanvas->TextWidth(FTitle->Caption);
            nTxtH = BuffCanvas->TextHeight(FTitle->Caption);

            int nTxtSTartX = 0;
            int nTxtStartY = 0;

            switch(Title->Align) {
                case caLeft:    nTxtSTartX = 0;  nTxtStartY = 0; break;
                case caRight:   nTxtSTartX = FTitle->Space;  nTxtStartY = 0; break;
                case caTop:     nTxtSTartX = 0;  nTxtStartY = 0; break;
                case caBottom:  nTxtSTartX = 0;  nTxtStartY = FTitle->Space; break;
            }
            
            switch(FTitle->Alignment ) {
                case taLeftJustify:
                        BuffCanvas->TextOutA( nTxtSTartX + 1 + nMargin, nTxtStartY + (nRoundHeight - nTxtH)/2, FTitle->Caption);
                        break;
                case taRightJustify:
                        BuffCanvas->TextOutA( nTxtSTartX + nRoundWidth - nTxtW - 1 - nMargin, nTxtStartY + (nRoundHeight - nTxtH)/2, FTitle->Caption);
                        break;
                case taCenter:
                        BuffCanvas->TextOutA( nTxtSTartX + (nRoundWidth - nTxtW) / 2, nTxtStartY + (nRoundHeight - nTxtH)/2, FTitle->Caption);
                        break;
            }

            if(FTitle->OutLineVisible) {
                //BuffCanvas->Brush->Color = FTitle->Color; //static_cast<TColor>(RGB(0, 0, 255));
                BuffCanvas->Brush->Style = bsClear;
                BuffCanvas->Pen->Color = FTitle->OutLineColor;

                switch(Title->Align) {
                    case caLeft:    BuffCanvas->RoundRect(0, 0, nRoundWidth, nRoundHeight, FTitle->Round, FTitle->Round); break;
                    case caRight:   BuffCanvas->RoundRect(FTitle->Space, 0, nTitleWidth, nRoundHeight, FTitle->Round, FTitle->Round); break;
                    case caTop:     BuffCanvas->RoundRect(0, 0, nRoundWidth, nRoundHeight, FTitle->Round, FTitle->Round); break;
                    case caBottom:  BuffCanvas->RoundRect(0, FTitle->Space, nRoundWidth, nTitleHeight, FTitle->Round, FTitle->Round); break;
                }
            }

            Canvas->Lock();

            switch(Title->Align) {
                case caLeft:
                    Canvas->Draw(0, 0, tmpBitmap1);
                    break;
                    
                case caRight:
                    Canvas->Draw(FEditWidth,0,tmpBitmap1);
                    break;
                    
                case caTop:
                    Canvas->Draw(0,0,tmpBitmap1);
                    break;
                    
                case caBottom:
                    Canvas->Draw(0,FEditHeight,tmpBitmap1);                
                    break;
            }

            Canvas->Unlock();
            
        }

    }

}
//-----------------------------------------------------------------------
void __fastcall TAZNumShape::PaintValue()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(bBeginUpdate || !bLoaded) return;

    // -------------------------------------------------------

    Vcl::Graphics::TCanvas* BuffCanvas = NULL;
    int nTxtW = 0, nTxtH = 0;

    // /////////////////////////////////////////////////////////////////////////
    // EDIT 그리기.
    int nEditWidth  = 0;
    int nEditHeight = 0;
    int nUnitWidth = FUnit->Visible ? FUnit->Width : 0;

    TColor clFontColor;
    TColor clShadowColor;
    TColor clBackColor;

    switch(Title->Align) {
        case caLeft:
        case caRight:
            nEditWidth  = FTitle->Visible ? FEditWidth : this->Width;
            nEditHeight = this->Height;
            break;
            
        case caTop:
        case caBottom:
            nEditWidth  = this->Width;
            nEditHeight = FTitle->Visible ? FEditHeight : this->Height;
            break;
    }
    
    tmpBitmap2->Width  = nEditWidth;
    tmpBitmap2->Height = nEditHeight;

    BuffCanvas = tmpBitmap2->Canvas;

    SetBkMode(BuffCanvas->Handle, TRANSPARENT);

    tmpBitmap2->Transparent = true;
    tmpBitmap2->TransparentColor     = MY_TRANSPARENTCOLOR; //Parent->Brush->Color;
    tmpBitmap2->Canvas->Brush->Color = MY_TRANSPARENTCOLOR; //Parent->Brush->Color;
    tmpBitmap2->Canvas->FillRect(Rect(0,0, nEditWidth, nEditHeight));

    // /////////////////////////////////////////////////////////////////////////
    // 외곽선  및 Unit 그리기.
    if(Enabled) {
        if(bSelecting) {
            BuffCanvas->Pen->Color      = FColorSet->ClickRectColor;
            BuffCanvas->Brush->Color    = FColorSet->ClickBGColor;
            BuffCanvas->Font->Color     = FUnit->Font->Color;
        }
        else if(bNumbering){
            BuffCanvas->Pen->Color      = FColorSet->ClickRectColor;
            BuffCanvas->Brush->Color    = FColorSet->ClickBGColor;
            BuffCanvas->Font->Color     = FUnit->Font->Color;
        }
        else {
            BuffCanvas->Pen->Color      = FColorSet->RectColor;
            BuffCanvas->Brush->Color    = FColorSet->BGColor;
            BuffCanvas->Font->Color     = FUnit->Font->Color;
        }
    }
    else {
		BuffCanvas->Pen->Color          = FColorSet->DisableRectColor; 	// clGray;
		BuffCanvas->Brush->Color        = FColorSet->DisableBGColor; 	// clBtnFace;
		BuffCanvas->Font->Color         = FColorSet->DisableFrontColor; // clGray;
    }

    BuffCanvas->RoundRect(0, 0, nEditWidth, nEditHeight, FRound, FRound);
    //if(bSelecting || bNumbering) BuffCanvas->DrawFocusRect(Rect(2,2,nEditWidth-2, Height-2));

    // 반사광 그리기. ------------
    int nRGB = ColorToRGB(BuffCanvas->Brush->Color);
    int nR = (nRGB & 0xFF0000) >> 16;
    int nG = (nRGB & 0x00FF00) >> 8;
    int nB = (nRGB & 0x0000FF);

    nR += 15;
    nG += 15;
    nB += 15;

    nR = nR > 255 ? 255 : nR;
    nG = nG > 255 ? 255 : nG;
    nB = nB > 255 ? 255 : nB;

    nRGB = (nR << 16) | (nG << 8) | nB;
    TColor ColorTmp         = (TColor)nRGB;
    TColor ColorBGOld       = BuffCanvas->Brush->Color;
    TColor ColorFTOld       = BuffCanvas->Pen->Color;
    BuffCanvas->Brush->Color    = ColorTmp;
    BuffCanvas->Pen->Color      = ColorTmp;
    BuffCanvas->RoundRect(1, 1, nEditWidth-1, nEditHeight/2, FRound-1, FRound-1);
    BuffCanvas->Brush->Color    = ColorBGOld;
    BuffCanvas->Pen->Color      = ColorFTOld;


    // ---------------------------------------

    if(FUnit->Visible) {
        SetBkMode(BuffCanvas->Handle, TRANSPARENT); // 투명 출력.

        BuffCanvas->Font->Assign(FUnit->Font);

        if(!Enabled) BuffCanvas->Font->Color = FColorSet->DisableFrontColor; // clGray;

        nTxtW = BuffCanvas->TextWidth(FUnit->Str);
        nTxtH = BuffCanvas->TextHeight(FUnit->Str);
        // Unit Align CENTER.
        BuffCanvas->TextOutA(nEditWidth - nUnitWidth + ((nUnitWidth - nTxtW) / 2) , (nEditHeight - nTxtH)/2, FUnit->Str);
    }
    
    // /////////////////////////////////////////////////////////////////////////
    // 이전값 다시 보기 그리기.
    int nStartY = 0;
    AnsiString sOutputText;

    if(FOldValueDisplay) {

        if(FFixedDigitUse) {
            int XEndPos    = 0;
            int nTotalTxtH = 0;
            int nTxtWidth  = 0;
            
            int size    = FInputEdit->DigitSize;
            int oldsize = FInputEdit->DigitSize / 2;
            
            size        = size    >= 6 ? size    : 6;
            oldsize     = oldsize >= 6 ? oldsize : 6;
            nTotalTxtH  = GetDigitHeight(oldsize) + GetDigitHeight(size);
            nStartY     = (((nEditHeight - nTotalTxtH) * 30) / 100) + 1;

            switch(FInputEdit->Type) {
                case itInt:
                    nTxtWidth = (FInputEdit->UpNumberOfDecimals * oldsize) + (0 * (FInputEdit->UpNumberOfDecimals - 1));
                    break;
                    
                case itFloat:
                    nTxtWidth = (FInputEdit->UpNumberOfDecimals * oldsize) + (0 * (FInputEdit->UpNumberOfDecimals - 1));
                    if(FInputEdit->DecimalPlaces > 0) {
                        nTxtWidth = nTxtWidth + (FInputEdit->DecimalPlaces * oldsize) + (0 * (FInputEdit->DecimalPlaces - 1));
                        nTxtWidth = nTxtWidth + (0 * 2 + GetDotWidth(oldsize));
                    }
                    break;
            }
            
            switch(FAlignment ) {
                case taLeftJustify:
                        XEndPos = nTxtWidth + 2 + FSpace;
                        break;
                case taRightJustify:
                        XEndPos = nEditWidth - nUnitWidth - 2 - FSpace;
                        break;
                case taCenter:
                        XEndPos = (nEditWidth - nUnitWidth - nTxtWidth) / 2 + nTxtWidth;
                        break;
            }
            
            if(Enabled) {

                if(FReadOnly) {
                    clFontColor     = FColorSet->OldValueColor;
                    clShadowColor   = FColorSet->FontShadowColor;
                }
                else if(bSelecting || bNumbering) {
                    clFontColor     = FColorSet->ClickFontColor;
                    clShadowColor   = FColorSet->ClickBGColor;
                }
                else {
                    clFontColor     = FColorSet->OldValueColor;
                    clShadowColor   = FColorSet->FontShadowColor;
                }
                
                clBackColor     = MY_TRANSPARENTCOLOR;
            }
            else {
                clFontColor     = FColorSet->DisableFrontColor; 		// clGray;
                clShadowColor   = FColorSet->DisableFrontShadowColor; // clWhite;
                clBackColor     = MY_TRANSPARENTCOLOR;
            }
            
            DrawValue2( FValueOld, 
                        XEndPos, 
                        nStartY, 
                        0, 
                        FInputEdit->Type, 
                        FInputEdit->UpNumberOfDecimals, 
                        FInputEdit->DecimalPlaces, 
                        clFontColor, 
                        clShadowColor, 
                        clBackColor, 
                        oldsize,
                        Enabled,
                        tmpBitmap2);

            nStartY = nStartY + GetDigitHeight(oldsize) + 1;    

            if(FOldValueUnderline) {
                tmpBitmap2->Canvas->Pen->Color = clFontColor;
                tmpBitmap2->Canvas->Pen->Width = 1;
                tmpBitmap2->Canvas->MoveTo(XEndPos, nStartY);
                tmpBitmap2->Canvas->LineTo(XEndPos - nTxtWidth, nStartY);

                nStartY = nStartY + 2;
            }
            
        }
        else {
            sOutputText = GetDrawValue(FValueOld, FInputEdit->Type, FInputEdit->DecimalPlaces);

            BuffCanvas->Font->Assign(Font);

            int nTotalTxtH = BuffCanvas->TextHeight(sOutputText);             // Value의 폰트 높이.

            if(FOldValueFontSize <= 0) {
                BuffCanvas->Font->Size  = BuffCanvas->Font->Size / 2;
            }
            else {
                BuffCanvas->Font->Size  = FOldValueFontSize;
            }

            if(FOldValueUnderline) {
                BuffCanvas->Font->Style = TFontStyles() >> fsBold << fsUnderline;
            }
            else {
                BuffCanvas->Font->Style = TFontStyles() >> fsBold >> fsUnderline;
            }
            
            nTotalTxtH = nTotalTxtH + BuffCanvas->TextHeight(sOutputText);     // Value와 OldValue를 합한 폰트 높이.

            if(Enabled) {
                BuffCanvas->Font->Color     = FColorSet->OldValueColor;
            }
            else {
                BuffCanvas->Font->Color     = FColorSet->DisableFrontColor; // clGray;
            }

            int nTemp = nEditHeight - nTotalTxtH;

            int nTop    = (nTemp * 45) / 100;
            //int nMiddle = (nTemp * 10) / 100;
            //int nBottom = nTemp - nTop - nMiddle;

            int SubnTxtW = BuffCanvas->TextWidth(sOutputText);
            int SubnTxtH = BuffCanvas->TextHeight(sOutputText);

            int nStartX = 0;

            switch(FAlignment ) {
                case taLeftJustify:
                        nStartX = 2 + FSpace;
                        break;
                case taRightJustify:
                          nStartX = (nEditWidth - nUnitWidth - SubnTxtW - 2) - FSpace;
                        break;
                case taCenter:
                        nStartX = (nEditWidth - nUnitWidth - SubnTxtW) / 2;
                        break;
            }

            nStartY = nTop+1;

            SetBkMode(BuffCanvas->Handle, TRANSPARENT); // 투명 출력.
            BuffCanvas->TextOutA(nStartX, nStartY, sOutputText);

            nStartY = nStartY + SubnTxtH - 3;
        }
    }


    // /////////////////////////////////////////////////////////////////////////
    // Value 출력

    if(FFixedDigitUse) {
        int XEndPos     = 0;
        int nTxtWidth   = 0;
        int size        = FInputEdit->DigitSize    >= 6 ? FInputEdit->DigitSize    : 6;
        nTxtH           = GetDigitHeight(size);
        
        switch(FInputEdit->Type) {
            case itInt:
                nTxtWidth = (FInputEdit->UpNumberOfDecimals * size) + (FInputEdit->DigitGap * (FInputEdit->UpNumberOfDecimals - 1));
                break;
                
            case itFloat:
                nTxtWidth = (FInputEdit->UpNumberOfDecimals * size) + (FInputEdit->DigitGap * (FInputEdit->UpNumberOfDecimals - 1));
                if(FInputEdit->DecimalPlaces > 0) {
                    nTxtWidth = nTxtWidth + (FInputEdit->DecimalPlaces * size) + (FInputEdit->DigitGap * (FInputEdit->DecimalPlaces - 1));
                    nTxtWidth = nTxtWidth + (FInputEdit->DigitGap * 2 + GetDotWidth(size));
                }
                break;
        }
        
        switch(FAlignment ) {
            case taLeftJustify:
                    XEndPos = nTxtWidth + 2 + FSpace;
                    break;
            case taRightJustify:
                    XEndPos = nEditWidth - nUnitWidth - 2 - FSpace;
                    break;
            case taCenter:
                    XEndPos = (nEditWidth - nUnitWidth - nTxtWidth) / 2 + nTxtWidth;
                    break;
        }
        
        if(Enabled) {
            if(FReadOnly) {
                clFontColor     = FColorSet->FontColor;
                clShadowColor   = FColorSet->FontShadowColor;
            }
            else if(bSelecting || bNumbering) {
                clFontColor     = FColorSet->ClickFontColor;
                clShadowColor   = FColorSet->ClickBGColor;
            }
            else {
                clFontColor     = bChanged ? FColorSet->ChangedFontColor : FColorSet->FontColor;
                clShadowColor   = FColorSet->FontShadowColor;
            }
            
            clBackColor     = MY_TRANSPARENTCOLOR; 
        }
        else {
			clFontColor     = FColorSet->DisableFrontColor; 		// clGray;
            clShadowColor   = FColorSet->DisableFrontShadowColor; 	// clWhite;
            clBackColor     = MY_TRANSPARENTCOLOR;
        }

        int nYTmp = ((nEditHeight - nStartY) - nTxtH)/2;
        nYTmp = FOldValueDisplay ? nYTmp - 1 : nYTmp;
        nStartY   = nYTmp >= 0 ? nStartY + nYTmp : nStartY;

        DrawValue2( FValue, 
                    XEndPos, 
                    nStartY, 
                    FInputEdit->DigitGap, 
                    FInputEdit->Type, 
                    FInputEdit->UpNumberOfDecimals, 
                    FInputEdit->DecimalPlaces, 
                    clFontColor, 
                    clShadowColor, 
                    clBackColor, 
                    size,
                    Enabled,
                    tmpBitmap2);

        
    }    
    else {
        BuffCanvas->Font->Assign(Font);

        sOutputText = GetDrawValue(FValue, FInputEdit->Type, FInputEdit->DecimalPlaces);        

        if(Enabled) {
            if(FReadOnly) {
                BuffCanvas->Font->Color        = FColorSet->FontColor;
            }
            else if(bSelecting || bNumbering) {
                BuffCanvas->Font->Color        = FColorSet->ClickFontColor;
            }
            else {
                BuffCanvas->Font->Color        = bChanged ? FColorSet->ChangedFontColor : FColorSet->FontColor;
            }
        }
        else {
            BuffCanvas->Font->Color            = FColorSet->DisableFrontColor; // clGray;
        }

        nTxtW = BuffCanvas->TextWidth(sOutputText);
        nTxtH = BuffCanvas->TextHeight(sOutputText);

        int nStartX = 0;

        switch(FAlignment ) {
            case taLeftJustify:
                nStartX = 2 + FSpace;
                break;
            case taRightJustify:
                  nStartX = (nEditWidth - nUnitWidth - nTxtW - 2) - FSpace;
                break;
            case taCenter:
                nStartX = (nEditWidth - nUnitWidth - nTxtW) / 2;
                break;
        }

        nStartX = nStartX < 0 ? 0 : nStartX;
        
        SetBkMode(BuffCanvas->Handle, TRANSPARENT); // 투명 출력.
        BuffCanvas->TextOutA(nStartX, nStartY + (((nEditHeight - nStartY) - nTxtH)/2), sOutputText);

    }
    

    if(tmpBitmap2->Width == 0 || tmpBitmap2->Height == 0) {
        return; 
    }

    // /////////////////////////////////////////////////////////////////////////
    // 출력 
    Canvas->Lock();

    if(FTitle->Visible) {

        if(m_nTitleWidth == 0 || m_nTitleHeight == 0) {
            Canvas->Draw(0,0,tmpBitmap2);
        }
        else {
            switch(Title->Align) {
                case caLeft:
                    Canvas->Draw(m_nTitleWidth,0,tmpBitmap2);
                    break;
                    
                case caRight:
                    Canvas->Draw(0,0,tmpBitmap2);
                    break;
                    
                case caTop:
                    Canvas->Draw(0,m_nTitleHeight,tmpBitmap2);
                    break;
                    
                case caBottom:
                    Canvas->Draw(0,0,tmpBitmap2);
                    break;
            }
        }
    }
    else {

        if(tmpBitmap2->Width > 0 && tmpBitmap2->Height > 0) {
            Canvas->Draw(0,0,tmpBitmap2);
        }
    }

    Canvas->Unlock();


}

//---------------------------------------------------------------------------
void __fastcall TAZNumShape::DrawImageTrans(
    Graphics::TBitmap* pDest,
    int X,
    int Y,
    Graphics::TBitmap* srcBmp)
{
    srcBmp->Transparent = true;
    srcBmp->TransparentColor = clFuchsia;
    pDest->Canvas->Draw(X, Y, srcBmp);
}
//---------------------------------------------------------------------------
void __fastcall TAZNumShape::DrawTextTrans(
    Graphics::TBitmap* pDest,
    Graphics::TBitmap* pTextBitmap,
    int             X,
    int             Y,
    AnsiString      sString,
    TColor          cColor,
    unsigned int    Align,
    int             nFontSize,
    bool            bBold)
{
    TRect rc;

    pTextBitmap->FreeImage();
    pTextBitmap->Canvas->Font->Name = "Arial";
    pTextBitmap->Canvas->Font->Size = nFontSize;
    pTextBitmap->Canvas->Font->Style = bBold ? TFontStyles() << fsBold : TFontStyles() >> fsBold;
    pTextBitmap->Width = pTextBitmap->Canvas->TextWidth(sString) + 1;
    pTextBitmap->Height = pTextBitmap->Canvas->TextHeight(sString);
    pTextBitmap->Canvas->Brush->Color = clFuchsia;
    pTextBitmap->Canvas->Font->Color = cColor;

    rc.Top      = 0;
    rc.Left     = 0;
    rc.Right    = pTextBitmap->Width;
    rc.Bottom   = pTextBitmap->Height;

    pTextBitmap->Canvas->FillRect(rc);
    pTextBitmap->Canvas->TextOutA(1 , 0, sString);
    pTextBitmap->Transparent = true;
    pTextBitmap->TransparentColor = clFuchsia;

    if(Align & TEXT_ALIGN_BOTTOM){
        Y = Y - pTextBitmap->Height;
    }
    else if(Align & TEXT_ALIGN_V_CENTER){
        Y = Y - (pTextBitmap->Height/2);
    }

    if(Align & TEXT_ALIGN_RIGHT){
        X = X - pTextBitmap->Width;
    }
    else if(Align & TEXT_ALIGN_H_CENTER){
        X = X - (pTextBitmap->Width/2);
    }

    DrawImageTrans(pDest, X, Y, pTextBitmap);
}
//---------------------------------------------------------------------------
void    __fastcall TAZNumShape::SetUnit(TNumUnitProperty    * p)
{
    FUnit->Assign(p);

    if(bBeginUpdate) return;
    
    if(FInvalidateUse)  Invalidate();
    else                PaintValue(); //Paint();
}

void    __fastcall TAZNumShape::SetTitle(TNumShapeTitleProperty * p)
{
    FTitle->Assign(p);

    if(bBeginUpdate) return;
    
    if(FInvalidateUse)  Invalidate();
    else                PaintTitle(); //Paint();
}
void    __fastcall TAZNumShape::SetColorSet(TNumShapeColorProperty * p)
{
    FColorSet->Assign(p);

    if(bBeginUpdate) return;
    
    if(FInvalidateUse)  Invalidate();
    else                PaintValue(); //Paint();
}

void __fastcall TAZNumShape::UnitChanged(System::TObject* Sender)
{
    if(bBeginUpdate) return;
    
    if(FInvalidateUse)  Invalidate();
    else                PaintValue(); //Paint();
}

void __fastcall TAZNumShape::ColorChanged(System::TObject* Sender)
{
    if(bBeginUpdate) return;
    
    if(FInvalidateUse)  Invalidate();
    else                PaintValue(); //Paint();
}
//---------------------------------------------------------------------------
//void __fastcall TAZNumShape::SetLabelPosition(const TLabelPosition Value)
//{
//    FLabelPosition = Value;
//    Invalidate();     // 화면을 갱신한다.
//}
//
//void __fastcall TAZNumShape::SetLabelSpacing(const int Value)
//{
//    FLabelSpacing = Value;
//    Invalidate();    // 화면을 갱신한다.
//}
//---------------------------------------------------------------------------
/// 델파이/빌더 실수형 프로퍼티의 퍼시스턴트 메카니즘의 디자인 오류 해결.
void __fastcall TAZNumShape::DefineProperties(TFiler *filer)
{
    TGraphicControl::DefineProperties(filer);

//    filer->DefineProperty(L"AZ_Min",             NULL, SetMin, true);
//    filer->DefineProperty(L"AZ_Max",             NULL, SetMax, true);
//    filer->DefineProperty(L"AZ_UpDownValue",     NULL, SetUpDownValue, true);
}
//---------------------------------------------------------------------------
//#if 0
//void    __fastcall TAZNumShape::SetMin(double d)
//{
//    FMin = d;
//    Invalidate();// 화면을 갱신한다.
//}
//
//void    __fastcall TAZNumShape::SetMax(double d)
//{
//    FMax = d;
//    Invalidate();// 화면을 갱신한다.
//}
//
//void    __fastcall TAZNumShape::SetUpDownValue(double d)
//{
//    FUpDownValue = d;
//    Invalidate();// 화면을 갱신한다.
//}
//
//#else // 델파이/빌더 실수형 프로퍼티의 퍼시스턴트 메카니즘의 디자인 오류 해결.
//void __fastcall TAZNumShape::SetMin(TWriter *writer)
//{
//    writer->WriteDouble(FMin);
//}
//
//void __fastcall TAZNumShape::SetMax(TWriter *writer)
//{
//    writer->WriteDouble(FMax);
//}
//
//void __fastcall TAZNumShape::SetUpDownValue(TWriter *writer)
//{
//    writer->WriteDouble(FUpDownValue);
//}
//#endif


//---------------------------------------------------------------------------

void __fastcall TAZNumShape::SetValueOld(double d)
{
    if(FValueOld != d) {
        FValueOld = d;
        
        if(bBeginUpdate) return;
        
        if(FInvalidateUse)  Invalidate();
        else                PaintValue(); //Paint();
    }
}

void __fastcall TAZNumShape::SetValue(double d)
{
    if(FValue != d) {
        FValueOld = FValue;
        FValue = d;

        if(bBeginUpdate) return;
        
        if(FInvalidateUse)  Invalidate();
        else                PaintValue(); //Paint();
    }
}


//void    __fastcall TAZNumShape::SetType(TInputNumType e)
//{
//    FType = e;
//    Invalidate();// 화면을 갱신한다.
//}
//
//
//
//void    __fastcall TAZNumShape::SetDecimalPlaces(int n)
//{
//    FDecimalPlaces = n;
//    Invalidate();// 화면을 갱신한다.
//}

void    __fastcall TAZNumShape::SetRound(int n)
{
    if(FRound != n) {
        FRound = n;

        if(bBeginUpdate) return;
        
        if(FInvalidateUse)  Invalidate();
        else                PaintValue(); //Paint();
    }
}


void    __fastcall TAZNumShape::SetSpace(int n)
{
    if(FSpace != n) {
        FSpace = n;

        if(bBeginUpdate) return;
        
        if(FInvalidateUse)  Invalidate();
        else                PaintValue(); //Paint();
    }
}

void    __fastcall TAZNumShape::SetReadOnly(bool b)
{
    if(FReadOnly != b ) {
        FReadOnly = b;

        if(bBeginUpdate) return;
        
        if(FInvalidateUse)  Invalidate();
        else                PaintValue(); //Paint();
    }
}

void    __fastcall TAZNumShape::SetNoEdit(bool b)
{
    if(FNoEdit != b ) {
        FNoEdit = b;

        if(bBeginUpdate) return;
        
        if(FInvalidateUse)  Invalidate();
        else                PaintValue(); //Paint();
    }
}
//void    __fastcall TAZNumShape::SetColor(int Index, TColor c)
//{
//    switch(Index) {
//        case 1: FClickBGColor       = c;    break;
//        case 2: FClickFontColor     = c;    break;
//        case 3: FClickRectColor     = c;    break;
//        case 4: FBGColor            = c;    break;
//        case 5: FFontColor          = c;    break;
//        case 6: FRectColor          = c;    break;
//        case 7: FOldValueColor      = c;    break;
//        case 8: FChangedFontColor   = c;    break;
//    }
//
//    Invalidate();// 화면을 갱신한다.
//}

void    __fastcall TAZNumShape::SetOldValueDisplay(bool b)
{
    if(FOldValueDisplay != b) {
        FOldValueDisplay = b;

        if(bBeginUpdate) return;
        
        if(FInvalidateUse)  Invalidate();
        else                PaintValue(); //Paint();
    }
}

void    __fastcall TAZNumShape::SetOlfValueUnderline(bool b)
{
    if(FOldValueUnderline != b) {
        FOldValueUnderline = b;

        if(bBeginUpdate) return;
        
        if(FInvalidateUse)  Invalidate();
        else                PaintValue(); //Paint();
    }
}

void    __fastcall TAZNumShape::SetOlfValueFontSize(int n)
{
    if(FOldValueFontSize != n) {
        FOldValueFontSize = n;

        if(bBeginUpdate) return;
        
        if(FInvalidateUse)  Invalidate();
        else                PaintValue(); //Paint();
    }
}

void     __fastcall TAZNumShape::SetAlignment(System::Classes::TAlignment Value)
{
    if(FAlignment != Value) {
        FAlignment = Value;

        if(bBeginUpdate) return;
        
        if(FInvalidateUse)  Invalidate();
        else                PaintValue(); //Paint();
    }
}

void    __fastcall TAZNumShape::SetPath(UnicodeString s)
{
    if(FPath != s) {
        FPath = s;

        if(bBeginUpdate) return;
        
        if(FInvalidateUse)  Invalidate();
        else                Paint();
    }
}

////---------------------------------------------------------------------------
///**
//* @brief : Font의 속성이 변경될 경우 Call Back 함수이다.
//* @param System::TObject* Sender :
//
//* @return void
//* @see
//- history : 1. [2013년 4월 2일 화요일][송신영] 최초 작성
//**/
//void __fastcall TAZNumShape::UnitFontChanged(System::TObject* Sender)
//{
//    Invalidate();// 화면을 갱신한다.
//}


//---------------------------------------------------------------------------
void     __fastcall TAZNumShape::MyClick(TObject *Sender)
{
    if(FReadOnly)                            return;
    if(Sender == NULL)                       return;
    if(ComponentState.Contains(csDesigning)) return;

    bool bRet = false;

    // Title 영역을 Click한 경우 Number Pad Popup 창을 출력하지 않는다.
    if(FTitle->Visible) {
        switch(Title->Align) {
            case caLeft:    if((Width - FEditWidth) > m_nMouseDownXPos)     return; break;
            case caRight:   if(FEditWidth < m_nMouseDownXPos)               return; break;
            case caTop:     if((Height - FEditHeight) > m_nMouseDownYPos)   return; break;
            case caBottom:  if(FEditHeight  < m_nMouseDownYPos)             return; break;
        }
    }
        

    bNumbering = true;
    if(FInvalidateUse)  Invalidate();
    else                Paint();

    if(FNoEdit)
    {
        bRet = true;
    }
    else
    {
        if(InputNumber(Sender)) {
            ResetOthersComponent();
            bChanged = true;
            bRet = true;
        }
    }

    bNumbering = false;
    if(FInvalidateUse)  Invalidate();
    else                Paint();

    //-----------------------------
    // 사용자 Click Event 처리.
    if(FOnClick) FOnClick(this, bRet);

}
//---------------------------------------------------------------------------

bool     __fastcall TAZNumShape::InputNumber(TObject *Sender)
{
    if(Sender == NULL) return false;

    AnsiString sReturn;

    TfmNumPad_AZ  * pForm = new TfmNumPad_AZ((TComponent*)Sender,
                                        FInputEdit->Type,
                                        FInputEdit->Min,
                                        FInputEdit->Max,
                                        FValue,
                                        &sReturn,
                                        FInputEdit->UpDownValue,
                                        FInputEdit->DecimalPlaces);

    if(m_bEncoderButtonEnable) {
        if(m_pEncoderAZNumShape != NULL) {
            pForm->SetEncoderButton(m_bEncoderButtonEnable, m_pEncoderAZNumShape->AZ_Value, m_sEncoderTitle, m_sEncoderUnit);
        }
    }

    int nRet = pForm->ShowModal();

    delete pForm;

    if(nRet == mrOk) {
        FValueOld = FValue;

        switch(FInputEdit->Type) {
            case itInt:
                FValue  = sReturn.ToIntDef(0);
                break;

            case itFloat:
                {
                    double dTemp = sReturn.ToDouble();
                    FValue = DecimalRounding(dTemp, FInputEdit->DecimalPlaces);
                }
                break;

            default:
                FValue = DecimalRounding(FValue, 3);
                break;
        }

        return true;
    }

    return false;

}
////---------------------------------------------------------------------------
///**
//* @brief : nPoint 의 소수점 자리만큼 반올림하여 리턴한다.
//           3.14159 를 nPoint 가 2 라면 3.14를 리턴한다.
//           만약 nPoint가 3이라면 3.142 를 리턴한다.
//* @param double dSrc : 반올림할 원래의 값
//* @param int nPoint :  반올림할 소수점 자리
//* @return double :
//* @see
//- history : 1. [2013년 4월 2일 화요일][송신영] 최초 작성
//**/
//double __fastcall TAZNumShape::DecimalRounding(double dSrc, int nPoint)
//{
//    int k=pow(10,nPoint);
//    double Result=floor(dSrc * k + 0.5) / k;
//    return Result;
//}
//---------------------------------------------------------------------------
void __fastcall TAZNumShape::ResetOthersComponent()
{
    TMessage    Msg;
    Msg.Msg     = WM_NUMSHAPE_RESET;
    Msg.WParam  = 0;
    Msg.LParam  = (int)(this);
    Msg.Result  = 0;

    //---------------------------------------------
    // 자신이 속한 가장 가까운 Parent Form을 찾는다.
    // GetParentForm(this)를 바로 사용하게 되면 Form이 다른 Form 밑에
    // 자식으로 도킹된 경우 가장 상위 Form을 반환하여 원하는 결과자 나오지 않는다.
    TWinControl *WndCtrl = this->Parent;
    TForm * pForm = dynamic_cast<TForm *>(WndCtrl);

    while(pForm == NULL) {
        WndCtrl = WndCtrl->Parent;
        pForm = dynamic_cast<TForm *>(WndCtrl);
        if(WndCtrl == NULL) break;
    }

    // 위에서 못찾은 경우 GetParentForm() 을 사용한다.
    if(pForm == NULL)
        pForm = (TForm *)GetParentForm(this);

    if(pForm == NULL)
        return;
    //---------------------------------------------

    int nCount = pForm->ComponentCount;
    TAZNumShape *p = NULL;

    for ( int i = 0; i < nCount; i++ ) {
        if ( pForm->Components[i]->ClassNameIs("TAZNumShape")){
            p = dynamic_cast<TAZNumShape *>(pForm->Components[i]);
            if((p != NULL) && (p != this)){
                //SendMessage(p->Handle, WM_NUMSHAPE_RESET, 0, (int)(this));
                if(p->IsChanged()) p->ResetChanged();
            }
        }
    }
}

void __fastcall TAZNumShape::ResetChanged()
{
    bChanged = false;

    if(bBeginUpdate) return;
    
    if(FInvalidateUse)  Invalidate();
    else                PaintValue(); //Paint();
}

bool __fastcall TAZNumShape::IsChanged()
{
    return bChanged;
}
//----------------------------------------------------------------------------
void  __fastcall TAZNumShape::InputEditChanged(System::TObject* Sender)
{
    if(bBeginUpdate) return;

    if(FInvalidateUse)  Invalidate();
    else                PaintValue(); //Paint();
}

void __fastcall TAZNumShape::SetInputEdit(TAZEditBoxProperty * p)
{
    if(p) {
        FInputEdit->Assign(p);

        if(bBeginUpdate) return;
        
        if(FInvalidateUse)  Invalidate();
        else                PaintValue(); //Paint();
    }
}
//----------------------------------------------------------------------------
void    __fastcall TAZNumShape::SetInvalidateUse(bool b)
{
    if(FInvalidateUse != b) {
        FInvalidateUse = b;
    }
}
//----------------------------------------------------------------------------
void    __fastcall TAZNumShape::SetEditWidth(int n)
{
    if(FEditWidth != n) {
        FEditWidth = n;

        if(FTitle->Visible) {

            if(bBeginUpdate) return;
            
            //if(FInvalidateUse)  Invalidate();
            //else                Paint();
            // Invalidate()를 사용해야 투명 적용이 정상적으로 적용된다.
            Invalidate();
        }
    }
}
//----------------------------------------------------------------------------
void    __fastcall TAZNumShape::SetEditHeight(int n)
{
    if(FEditHeight!= n) {
        FEditHeight = n;

        if(FTitle->Visible) {

            if(bBeginUpdate) return;
            
            //if(FInvalidateUse)  Invalidate();
            //else                Paint();
            // Invalidate()를 사용해야 투명 적용이 정상적으로 적용된다.
            Invalidate();
        }
    }
}


//----------------------------------------------------------------------------
void    __fastcall TAZNumShape::TitleChanged(System::TObject* Sender)
{
//    if(FInvalidateUse)  Invalidate();
//    else                Paint();

    if(bBeginUpdate) return;
    
    // Invalidate()를 사용해야 투명 적용이 정상적으로 적용된다.
    Invalidate();
}
//----------------------------------------------------------------------------

void            __fastcall TAZNumShape::BeginUpdate()
{
    bBeginUpdate = true;
}

void            __fastcall TAZNumShape::EndUpdate()
{
    bBeginUpdate = false;
}

//----------------------------------------------------------------------------



void    __fastcall TAZNumShape::SetFixedDigit(bool b)
{
    if(FFixedDigitUse != b) {
        FFixedDigitUse = b;

        if(bBeginUpdate) return;
        Invalidate();
    }
}

//----------------------------------------------------------------------------

void  __fastcall TAZNumShape::SetEncoderButton(bool bEnable, AnsiString sTitle, AnsiString unit, TAZNumShape * pAZNumShape) // , TAZNumShape *pAZNumShape, AnsiString sTitle, AnsiString sUnit
{
    m_bEncoderButtonEnable  = bEnable;
    m_pEncoderAZNumShape    = pAZNumShape;
    m_sEncoderTitle         = sTitle;
    m_sEncoderUnit          = unit;
}


//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
void __fastcall TAZNumShape::DrawValue2(    double          Value,
                                            int             XEndPos,
                                            int             YStartPos,
                                            int             nGap,
                                            TInputNumType   eType,
                                            int             nUpNumberOfDecimals,
                                            int             nDownNumberOfDecimals,
                                            TColor          FColor,
                                            TColor          ShadowColor,
                                            TColor          BGColor,
                                            int             size,
                                            bool            bEnabled,
                                            Graphics::TBitmap* pBitMap)
{
    assert(pBitMap);

    //pBitMap->Canvas->Brush->Color = BGColor;
    //pBitMap->Canvas->FillRect(Rect(0,0,pBitMap->Width, pBitMap->Height));

    int nStartPosX = XEndPos - size;
    bool bMinus = Value < 0 ? true : false;
    
    Value = ROUNDING(Value, nDownNumberOfDecimals);

    // 소수점 그리기.
    if(eType == itFloat && (nDownNumberOfDecimals > 0)) {
#if 1        
        AnsiString sTemp;
        AnsiString sFormat = "%0." + IntToStr(nDownNumberOfDecimals) + "f";
        sTemp.printf(sFormat.c_str(), Value);

        for(int i=0; i<nDownNumberOfDecimals; i++) { 
            unsigned char ch = sTemp.c_str()[sTemp.Length()-i-1] - '0';

            DrawOneDigit2(  ch,
                            nStartPosX,
                            YStartPos,
                            FColor,
                            ShadowColor,
                            BGColor,
                            size,
                            pBitMap);

            nStartPosX = nStartPosX - size - nGap;
        }
        
#else
        int  nGop   = pow(10, nDownNumberOfDecimals);
        int  nTmp   = bMinus ? (Value * (double)(nGop * 10)) * (-1.0) : (Value * (double)(nGop * 10)); // 일부러 10을 더 곱한다. 
        nTmp        += 1; // 10을 더 곱한 후 1을 더하는 것은 0.876 값에 1000을 곱할 경우 876이 아니라 875가 되는 경우가 있어서 10을 더 곱한 후 8759 이렇게 될 경우 1을 더해 원래 값인 8760으로 맞추고 다시 10으로 나눈다 
        nTmp        = nTmp / 10;
        nTmp        = nTmp % nGop;

        for(int i=0; i<nDownNumberOfDecimals; i++) {
            unsigned char ch = nTmp % 10;
            nTmp = nTmp / 10;

            DrawOneDigit2(  ch,
                            nStartPosX,
                            YStartPos,
                            FColor,
                            ShadowColor,
                            BGColor,
                            size,
                            pBitMap);

            nStartPosX = nStartPosX - size - nGap;
        }
#endif 
        // 점 그리기 --------------------------
        nStartPosX = nStartPosX + (size - GetDotWidth(size));
        DrawOneDot2(nStartPosX, YStartPos, FColor, BGColor, size, pBitMap);
        nStartPosX = nStartPosX - size - nGap;
    }

    // 정수부 그리기 --------------------------------------------------
    int nTmp = bMinus ? (int)Value * (-1) : (int)Value;

    // 정수부의 자리수를 구한다. 
    int nIntWidth = GetIntWidth(nTmp);

    // 자리수가 정수부는 없고 소수점만 있는 경우. 
    nIntWidth = (nIntWidth == 0) ? 1 : nIntWidth;

    // 값이 음수인 경우.
    nIntWidth = (bMinus)         ? nIntWidth + 1 : nIntWidth;

    // 출력할 값의 자리수를 계산한 것과 사용자가 설정한 자리수를 비교하여 부족한 경우 계산한 것으로 사용 한다. 
    nIntWidth = (nIntWidth > nUpNumberOfDecimals) ? nIntWidth : nUpNumberOfDecimals;
        
    for(int i=0; i<nIntWidth; i++) {
        if((nTmp > 0) || (i == 0)) {
            unsigned char ch = nTmp % 10;
            DrawOneDigit2(  ch,
                            nStartPosX,
                            YStartPos,
                            FColor,
                            ShadowColor,
                            BGColor,
                            size,
                            pBitMap);
        }
        else if(bMinus) {
            bMinus = false;
            DrawOneDigit2(  11,
                            nStartPosX,
                            YStartPos,
                            FColor,
                            ShadowColor,
                            BGColor,
                            size,
                            pBitMap);
        }        
        else {
            DrawOneDigit2(  8,
                            nStartPosX,
                            YStartPos,
                            ShadowColor,
                            ShadowColor,
                            BGColor,
                            size,
                            pBitMap);
        }

        nStartPosX = nStartPosX - size - nGap;

        nTmp = nTmp / 10;
    }


}

double __fastcall TAZNumShape::GetDigitHeight(int size)
{
    double dHeight     = (double)size * DEF_HEIGHT_RATIO_2;

    switch(size){
        case 6:  dHeight = 9; break;
        case 7:  dHeight = 11; break;
        case 8:  dHeight = 13; break;
        case 9:  dHeight = 15; break;
        case 10: dHeight = 18; break;
        case 11: dHeight = 19; break;
        case 12: dHeight = 21; break;
        case 13: dHeight = 23; break;
        case 14: dHeight = 25; break;
        case 15: dHeight = 27; break;
        case 16: dHeight = 28; break;
        case 17: dHeight = 29; break;
        case 18: dHeight = 31; break;
        case 19: dHeight = 32; break;
        case 20: dHeight = 33; break;
        default: 
            break;
    }

    return dHeight;
}

int __fastcall TAZNumShape::GetDotWidth(int size)
{
    int nWidth     = 0;

    switch(size){
        case 0: 
        case 1: 
        case 2: 
        case 3: 
        case 4: 
        case 5:  
        case 6:  nWidth = 3; break;
        case 7:  nWidth = 4; break;
        case 8:  nWidth = 4; break;
        case 9:  nWidth = 5; break;
        case 10: nWidth = 5; break;
        case 11: nWidth = 6; break;
        case 12: nWidth = 6; break;
        case 13: nWidth = 7; break;
        case 14: nWidth = 7; break;
        case 15: nWidth = 8; break;
        case 16: nWidth = 8; break;
        case 17: nWidth = 9; break;
        case 18: nWidth = 9; break;
        case 19: nWidth = 10; break;
        case 20: nWidth = 10; break;
        default: 
            nWidth     = size / 2;
            break;
    }

    return nWidth;
}

void __fastcall TAZNumShape::DrawOneDigit2(unsigned char ch, int nX, int nY, TColor FColor, TColor ShadowColor, TColor BGColor, int size, Graphics::TBitmap* pBitMap)
{
    assert(pBitMap);
    if(ch >= 12) return;

    double dWidth                  = size;
    double dHeight                 = GetDigitHeight(size);

//    pBitmapTemp->PixelFormat     = pf24bit; // AntiAliasing 적용 위해
//    pBitmapTemp->Width          = dWidth;
//    pBitmapTemp->Height         = dHeight;
//
//    pBitmapTemp->Transparent            = true;
//    pBitmapTemp->TransparentColor       = BGColor;
//    pBitmapTemp->Canvas->Brush->Color   = BGColor;
//    pBitmapTemp->Canvas->FillRect(Rect(0,0,pBitmapTemp->Width, pBitmapTemp->Height));
//
//    pBitmapTemp->Canvas->Pen->Color     = BGColor;
    // 1 Top
    pBitMap->Canvas->Brush->Color = gAZDigitPixDot[ch][0] ? FColor : ShadowColor;
    pBitMap->Canvas->Pen->Color   = gAZDigitPixDot[ch][0] ? FColor : ShadowColor;
    DrawTop(nX, nY, size, pBitMap, dWidth, dHeight);

    pBitMap->Canvas->Brush->Color = gAZDigitPixDot[ch][1] ? FColor : ShadowColor;
    pBitMap->Canvas->Pen->Color   = gAZDigitPixDot[ch][1] ? FColor : ShadowColor;
    DrawTopLeft(nX, nY, size, pBitMap, dWidth, dHeight);

    pBitMap->Canvas->Brush->Color = gAZDigitPixDot[ch][2] ? FColor : ShadowColor;
    pBitMap->Canvas->Pen->Color   = gAZDigitPixDot[ch][2] ? FColor : ShadowColor;
    DrawBottomLeft(nX, nY, size, pBitMap, dWidth, dHeight);

    pBitMap->Canvas->Brush->Color = gAZDigitPixDot[ch][3] ? FColor : ShadowColor;
    pBitMap->Canvas->Pen->Color   = gAZDigitPixDot[ch][3] ? FColor : ShadowColor;
    DrawBottom(nX, nY, size, pBitMap, dWidth, dHeight);

    pBitMap->Canvas->Brush->Color = gAZDigitPixDot[ch][4] ? FColor : ShadowColor;
    pBitMap->Canvas->Pen->Color   = gAZDigitPixDot[ch][4] ? FColor : ShadowColor;
    DrawBottomRight(nX, nY, size, pBitMap, dWidth, dHeight);

    pBitMap->Canvas->Brush->Color = gAZDigitPixDot[ch][5] ? FColor : ShadowColor;
    pBitMap->Canvas->Pen->Color   = gAZDigitPixDot[ch][5] ? FColor : ShadowColor;
    DrawTopRight(nX, nY, size, pBitMap, dWidth, dHeight);

    pBitMap->Canvas->Brush->Color = gAZDigitPixDot[ch][6] ? FColor : ShadowColor;
    pBitMap->Canvas->Pen->Color   = gAZDigitPixDot[ch][6] ? FColor : ShadowColor;
    DrawCenter(nX, nY, size, pBitMap, dWidth, dHeight);

    //pBitMap->Canvas->Draw(nX, nY, pBitmapTemp);

}
//---------------------------------------------------------------------------
void __fastcall TAZNumShape::DrawOneDot2(int nX, int nY, TColor FColor, TColor BGColor, int size, Graphics::TBitmap* pBitMap)
{
    assert(pBitMap);

//    pBitmapTemp->PixelFormat      = pf24bit;         // AntiAliasing 적용 위해
//    pBitmapTemp->Width            = dWidth;
//    pBitmapTemp->Height           = dHeight;
//
//    pBitmapTemp->Transparent            = true;
//    pBitmapTemp->TransparentColor       = BGColor;
//    pBitmapTemp->Canvas->Brush->Color   = BGColor;
//    pBitmapTemp->Canvas->FillRect(Rect(0,0,pBitmapTemp->Width, pBitmapTemp->Height));
//    pBitmapTemp->Canvas->Pen->Color     = FColor;

    switch(size) {
        case 6:
            pBitMap->Canvas->Pixels[nX + 1][nY +  7] = FColor;
        break;
        case 7:
            pBitMap->Canvas->Pixels[nX +  2][nY + 8] = FColor;
            pBitMap->Canvas->Pixels[nX +  2][nY + 9] = FColor;
        break;
        case 8:
            pBitMap->Canvas->Pixels[nX +  1][nY +10] = FColor;
            pBitMap->Canvas->Pixels[nX +  2][nY +10] = FColor;
            pBitMap->Canvas->Pixels[nX +  1][nY +11] = FColor;
            pBitMap->Canvas->Pixels[nX +  2][nY +11] = FColor;
        break;
        case 9:
            pBitMap->Canvas->Pixels[nX +  2][nY +11] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +11] = FColor;
            pBitMap->Canvas->Pixels[nX +  2][nY +12] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +12] = FColor;
        break;
        case 10:
            pBitMap->Canvas->Pixels[nX +  2][nY +14] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +14] = FColor;
            pBitMap->Canvas->Pixels[nX +  2][nY +15] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +15] = FColor;
        break;
        case 11:
            pBitMap->Canvas->Pixels[nX +  2][nY +15] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +15] = FColor;
            pBitMap->Canvas->Pixels[nX +  2][nY +16] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +16] = FColor;
        break;
        case 12:
            pBitMap->Canvas->Pixels[nX +  2][nY +17] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +17] = FColor;
            pBitMap->Canvas->Pixels[nX +  2][nY +18] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +18] = FColor;
        break;
        case 13:
            pBitMap->Canvas->Pixels[nX +  3][nY +19] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +19] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +20] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +20] = FColor;
        break;
        case 14:
            pBitMap->Canvas->Pixels[nX +  3][nY +21] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +21] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +22] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +22] = FColor;
        break;
        case 15:
            pBitMap->Canvas->Pixels[nX +  3][nY +22] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +22] = FColor;
            pBitMap->Canvas->Pixels[nX +  5][nY +22] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +23] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +23] = FColor;
            pBitMap->Canvas->Pixels[nX +  5][nY +23] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +24] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +24] = FColor;
            pBitMap->Canvas->Pixels[nX +  5][nY +24] = FColor;
        break;
        case 16:
            pBitMap->Canvas->Pixels[nX +  3][nY +23] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +23] = FColor;
            pBitMap->Canvas->Pixels[nX +  5][nY +23] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +24] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +24] = FColor;
            pBitMap->Canvas->Pixels[nX +  5][nY +24] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +25] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +25] = FColor;
            pBitMap->Canvas->Pixels[nX +  5][nY +25] = FColor;
        break;
        case 17:
            pBitMap->Canvas->Pixels[nX +  3][nY +23] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +23] = FColor;
            pBitMap->Canvas->Pixels[nX +  5][nY +23] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +24] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +24] = FColor;
            pBitMap->Canvas->Pixels[nX +  5][nY +24] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +25] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +25] = FColor;
            pBitMap->Canvas->Pixels[nX +  5][nY +25] = FColor;
        break;
        case 18:
            pBitMap->Canvas->Pixels[nX +  3][nY +25] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +25] = FColor;
            pBitMap->Canvas->Pixels[nX +  5][nY +25] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +26] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +26] = FColor;
            pBitMap->Canvas->Pixels[nX +  5][nY +26] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +27] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +27] = FColor;
            pBitMap->Canvas->Pixels[nX +  5][nY +27] = FColor;
        break;
        case 19:
            pBitMap->Canvas->Pixels[nX +  3][nY +25] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +25] = FColor;
            pBitMap->Canvas->Pixels[nX +  5][nY +25] = FColor;
            pBitMap->Canvas->Pixels[nX +  6][nY +25] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +26] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +26] = FColor;
            pBitMap->Canvas->Pixels[nX +  5][nY +26] = FColor;
            pBitMap->Canvas->Pixels[nX +  6][nY +26] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +27] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +27] = FColor;
            pBitMap->Canvas->Pixels[nX +  5][nY +27] = FColor;
            pBitMap->Canvas->Pixels[nX +  6][nY +27] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +28] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +28] = FColor;
            pBitMap->Canvas->Pixels[nX +  5][nY +28] = FColor;
            pBitMap->Canvas->Pixels[nX +  6][nY +28] = FColor;
        break;
        case 20:
            pBitMap->Canvas->Pixels[nX +  3][nY +26] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +26] = FColor;
            pBitMap->Canvas->Pixels[nX +  5][nY +26] = FColor;
            pBitMap->Canvas->Pixels[nX +  6][nY +26] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +27] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +27] = FColor;
            pBitMap->Canvas->Pixels[nX +  5][nY +27] = FColor;
            pBitMap->Canvas->Pixels[nX +  6][nY +27] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +28] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +28] = FColor;
            pBitMap->Canvas->Pixels[nX +  5][nY +28] = FColor;
            pBitMap->Canvas->Pixels[nX +  6][nY +28] = FColor;
            pBitMap->Canvas->Pixels[nX +  3][nY +29] = FColor;
            pBitMap->Canvas->Pixels[nX +  4][nY +29] = FColor;
            pBitMap->Canvas->Pixels[nX +  5][nY +29] = FColor;
            pBitMap->Canvas->Pixels[nX +  6][nY +29] = FColor;
        break;

        default:
        // Dot
            {
                double dWidth                  = (double)size / 2.0; // 점 크기는 일반 숫자의 절반으로 하자.
                double dHeight                 = GetDigitHeight(size);
                
                pBitMap->Canvas->Brush->Color = FColor;
                TPoint points[8];
                points[0] = Point(nX + dWidth * 0.400, nY + dHeight * 0.788);
                points[1] = Point(nX + dWidth * 0.200, nY + dHeight * 0.848);
                points[2] = Point(nX + dWidth * 0.200, nY + dHeight * 0.879);
                points[3] = Point(nX + dWidth * 0.400, nY + dHeight * 0.939);
                points[4] = Point(nX + dWidth * 0.500, nY + dHeight * 0.939);
                points[5] = Point(nX + dWidth * 0.700, nY + dHeight * 0.879);
                points[6] = Point(nX + dWidth * 0.700, nY + dHeight * 0.848);
                points[7] = Point(nX + dWidth * 0.500, nY + dHeight * 0.788);

                pBitMap->Canvas->Polygon(points, 7);
            }
        break;
    }

    //pBitMap->Canvas->Draw(nX, nY, pBitMap);
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
void __fastcall TAZNumShape::DrawTop(int nX, int nY, int size, Graphics::TBitmap* pBitMap, double dWidth, double dHeight)
{

    // 1: Top ------------------------------------------
    switch(size) {
        case 6:
            pBitMap->Canvas->MoveTo(nX + 2, nY + 1) ; pBitMap->Canvas->LineTo(nX + 4, nY + 1);
            break;
        case 7:
            pBitMap->Canvas->MoveTo(nX + 2, nY + 1) ; pBitMap->Canvas->LineTo(nX + 5, nY + 1);
            break;
        case 8:
            pBitMap->Canvas->MoveTo(nX + 2, nY + 1) ; pBitMap->Canvas->LineTo(nX + 6, nY + 1);
            break;
        case 9:
            pBitMap->Canvas->MoveTo(nX + 2, nY + 1) ; pBitMap->Canvas->LineTo(nX + 7, nY + 1);
            pBitMap->Canvas->MoveTo(nX + 3, nY + 2) ; pBitMap->Canvas->LineTo(nX + 6, nY + 2);
            break;
        case 10:
            pBitMap->Canvas->MoveTo(nX + 2, nY + 1) ; pBitMap->Canvas->LineTo(nX + 8, nY + 1);
            pBitMap->Canvas->MoveTo(nX + 3, nY + 2) ; pBitMap->Canvas->LineTo(nX + 7, nY + 2);
            break;
        case 11:
            pBitMap->Canvas->MoveTo(nX + 2, nY + 1) ; pBitMap->Canvas->LineTo(nX + 9, nY + 1);
            pBitMap->Canvas->MoveTo(nX + 3, nY + 2) ; pBitMap->Canvas->LineTo(nX + 8, nY + 2);
        break;
        case 12:
            pBitMap->Canvas->MoveTo(nX + 2, nY + 1) ; pBitMap->Canvas->LineTo(nX +10, nY + 1);
            pBitMap->Canvas->MoveTo(nX + 3, nY + 2) ; pBitMap->Canvas->LineTo(nX + 9, nY + 2);
            break;
        case 13:
            pBitMap->Canvas->MoveTo(nX + 2, nY + 1) ; pBitMap->Canvas->LineTo(nX +11, nY + 1);
            pBitMap->Canvas->MoveTo(nX + 3, nY + 2) ; pBitMap->Canvas->LineTo(nX +10, nY + 2);
            pBitMap->Canvas->MoveTo(nX + 4, nY + 3) ; pBitMap->Canvas->LineTo(nX + 9, nY + 3);
            break;
        case 14:
            pBitMap->Canvas->MoveTo(nX + 2, nY + 1) ; pBitMap->Canvas->LineTo(nX +12, nY + 1);
            pBitMap->Canvas->MoveTo(nX + 3, nY + 2) ; pBitMap->Canvas->LineTo(nX +11, nY + 2);
            pBitMap->Canvas->MoveTo(nX + 4, nY + 3) ; pBitMap->Canvas->LineTo(nX +10, nY + 3);
            break;
        case 15:
            pBitMap->Canvas->MoveTo(nX + 2, nY + 1) ; pBitMap->Canvas->LineTo(nX +13, nY + 1);
            pBitMap->Canvas->MoveTo(nX + 3, nY + 2) ; pBitMap->Canvas->LineTo(nX +12, nY + 2);
            pBitMap->Canvas->MoveTo(nX + 4, nY + 3) ; pBitMap->Canvas->LineTo(nX +11, nY + 3);
            break;
        case 16:
            pBitMap->Canvas->MoveTo(nX + 2, nY + 1) ; pBitMap->Canvas->LineTo(nX +14, nY + 1);
            pBitMap->Canvas->MoveTo(nX + 3, nY + 2) ; pBitMap->Canvas->LineTo(nX +13, nY + 2);
            pBitMap->Canvas->MoveTo(nX + 4, nY + 3) ; pBitMap->Canvas->LineTo(nX +12, nY + 3);
            pBitMap->Canvas->MoveTo(nX + 5, nY + 4) ; pBitMap->Canvas->LineTo(nX +11, nY + 4);
            break;
        case 17:
            pBitMap->Canvas->MoveTo(nX + 2, nY + 1) ; pBitMap->Canvas->LineTo(nX +15, nY + 1);
            pBitMap->Canvas->MoveTo(nX + 3, nY + 2) ; pBitMap->Canvas->LineTo(nX +14, nY + 2);
            pBitMap->Canvas->MoveTo(nX + 4, nY + 3) ; pBitMap->Canvas->LineTo(nX +13, nY + 3);
            pBitMap->Canvas->MoveTo(nX + 5, nY + 4) ; pBitMap->Canvas->LineTo(nX +12, nY + 4);
            break;
        case 18:
            pBitMap->Canvas->MoveTo(nX + 2, nY + 1) ; pBitMap->Canvas->LineTo(nX +16, nY + 1);
            pBitMap->Canvas->MoveTo(nX + 3, nY + 2) ; pBitMap->Canvas->LineTo(nX +15, nY + 2);
            pBitMap->Canvas->MoveTo(nX + 4, nY + 3) ; pBitMap->Canvas->LineTo(nX +14, nY + 3);
            pBitMap->Canvas->MoveTo(nX + 5, nY + 4) ; pBitMap->Canvas->LineTo(nX +13, nY + 4);
            break;
        case 19:
            pBitMap->Canvas->MoveTo(nX + 2, nY + 1) ; pBitMap->Canvas->LineTo(nX +17, nY + 1);
            pBitMap->Canvas->MoveTo(nX + 3, nY + 2) ; pBitMap->Canvas->LineTo(nX +16, nY + 2);
            pBitMap->Canvas->MoveTo(nX + 4, nY + 3) ; pBitMap->Canvas->LineTo(nX +15, nY + 3);
            pBitMap->Canvas->MoveTo(nX + 5, nY + 4) ; pBitMap->Canvas->LineTo(nX +14, nY + 4);
            pBitMap->Canvas->MoveTo(nX + 6, nY + 5) ; pBitMap->Canvas->LineTo(nX +13, nY + 5);
            break;
        case 20:
            pBitMap->Canvas->MoveTo(nX + 2, nY + 1) ; pBitMap->Canvas->LineTo(nX +18, nY + 1);
            pBitMap->Canvas->MoveTo(nX + 3, nY + 2) ; pBitMap->Canvas->LineTo(nX +17, nY + 2);
            pBitMap->Canvas->MoveTo(nX + 4, nY + 3) ; pBitMap->Canvas->LineTo(nX +16, nY + 3);
            pBitMap->Canvas->MoveTo(nX + 5, nY + 4) ; pBitMap->Canvas->LineTo(nX +15, nY + 4);
            pBitMap->Canvas->MoveTo(nX + 6, nY + 5) ; pBitMap->Canvas->LineTo(nX +14, nY + 5);
            break;

        default: {
            TPoint points[6];
            points[0] = Point(nX + dWidth * 0.100, nY + dHeight * 0.030);
            points[1] = Point(nX + dWidth * 0.300, nY + dHeight * 0.152);
            points[2] = Point(nX + dWidth * 0.650, nY + dHeight * 0.152);
            points[3] = Point(nX + dWidth * 0.850, nY + dHeight * 0.030);
            pBitMap->Canvas->Polygon(points, 3);
        }
        break;
    }
}

void __fastcall TAZNumShape::DrawTopLeft(int nX, int nY, int size, Graphics::TBitmap* pBitMap, double dWidth, double dHeight)
{

// 2 : Top Left --------------------------------------
    switch(size) {

        case 6:
            pBitMap->Canvas->MoveTo(nX +  1,nY +  2) ; pBitMap->Canvas->LineTo(nX +  1,nY +  4);
        break;
        case 7:
            pBitMap->Canvas->MoveTo(nX +  1,nY +  2) ; pBitMap->Canvas->LineTo(nX +  1,nY +  5);
        break;
        case 8:
            pBitMap->Canvas->MoveTo(nX +  1,nY +  2) ; pBitMap->Canvas->LineTo(nX +  1,nY +  6);
        break;
        case 9:
            pBitMap->Canvas->MoveTo(nX +  1,nY +  2) ; pBitMap->Canvas->LineTo(nX +  1,nY +  7);
            pBitMap->Canvas->MoveTo(nX +  2,nY +  3) ; pBitMap->Canvas->LineTo(nX +  2,nY +  6);
        break;
        case 10:
            pBitMap->Canvas->MoveTo(nX +  1,nY +  2) ; pBitMap->Canvas->LineTo(nX +  1,nY +  9);
            pBitMap->Canvas->MoveTo(nX +  2,nY +  3) ; pBitMap->Canvas->LineTo(nX +  2,nY +  8);
        break;
        case 11:
            pBitMap->Canvas->MoveTo(nX +  1,nY +  2) ; pBitMap->Canvas->LineTo(nX +  1,nY +  9);
            pBitMap->Canvas->MoveTo(nX +  2,nY +  3) ; pBitMap->Canvas->LineTo(nX +  2,nY +  8);
        break;
        case 12:
            pBitMap->Canvas->MoveTo(nX +  1,nY +  2) ; pBitMap->Canvas->LineTo(nX +  1,nY + 10);
            pBitMap->Canvas->MoveTo(nX +  2,nY +  3) ; pBitMap->Canvas->LineTo(nX +  2,nY +  9);
        break;
        case 13:
            pBitMap->Canvas->MoveTo(nX +  1,nY +  2) ; pBitMap->Canvas->LineTo(nX +  1,nY + 11);
            pBitMap->Canvas->MoveTo(nX +  2,nY +  3) ; pBitMap->Canvas->LineTo(nX +  2,nY + 10);
            pBitMap->Canvas->MoveTo(nX +  3,nY +  4) ; pBitMap->Canvas->LineTo(nX +  3,nY +  9);
        break;
        case 14:
            pBitMap->Canvas->MoveTo(nX +  1,nY +  2) ; pBitMap->Canvas->LineTo(nX +  1,nY + 12);
            pBitMap->Canvas->MoveTo(nX +  2,nY +  3) ; pBitMap->Canvas->LineTo(nX +  2,nY + 11);
            pBitMap->Canvas->MoveTo(nX +  3,nY +  4) ; pBitMap->Canvas->LineTo(nX +  3,nY + 10);
        break;
        case 15:
            pBitMap->Canvas->MoveTo(nX +  1,nY +  2) ; pBitMap->Canvas->LineTo(nX +  1,nY + 13);
            pBitMap->Canvas->MoveTo(nX +  2,nY +  3) ; pBitMap->Canvas->LineTo(nX +  2,nY + 12);
            pBitMap->Canvas->MoveTo(nX +  3,nY +  4) ; pBitMap->Canvas->LineTo(nX +  3,nY + 11);
        break;
        case 16:
            pBitMap->Canvas->MoveTo(nX +  1,nY +  2) ; pBitMap->Canvas->LineTo(nX +  1,nY + 14);
            pBitMap->Canvas->MoveTo(nX +  2,nY +  3) ; pBitMap->Canvas->LineTo(nX +  2,nY + 13);
            pBitMap->Canvas->MoveTo(nX +  3,nY +  4) ; pBitMap->Canvas->LineTo(nX +  3,nY + 12);
            pBitMap->Canvas->MoveTo(nX +  4,nY +  5) ; pBitMap->Canvas->LineTo(nX +  4,nY + 11);
        break;
        case 17:
            pBitMap->Canvas->MoveTo(nX +  1,nY +  2) ; pBitMap->Canvas->LineTo(nX +  1,nY + 14);
            pBitMap->Canvas->MoveTo(nX +  2,nY +  3) ; pBitMap->Canvas->LineTo(nX +  2,nY + 13);
            pBitMap->Canvas->MoveTo(nX +  3,nY +  4) ; pBitMap->Canvas->LineTo(nX +  3,nY + 12);
            pBitMap->Canvas->MoveTo(nX +  4,nY +  5) ; pBitMap->Canvas->LineTo(nX +  4,nY + 11);
        break;
        case 18:
            pBitMap->Canvas->MoveTo(nX +  1,nY +  2) ; pBitMap->Canvas->LineTo(nX +  1,nY + 15);
            pBitMap->Canvas->MoveTo(nX +  2,nY +  3) ; pBitMap->Canvas->LineTo(nX +  2,nY + 14);
            pBitMap->Canvas->MoveTo(nX +  3,nY +  4) ; pBitMap->Canvas->LineTo(nX +  3,nY + 13);
            pBitMap->Canvas->MoveTo(nX +  4,nY +  5) ; pBitMap->Canvas->LineTo(nX +  4,nY + 12);
        break;
        case 19:
            pBitMap->Canvas->MoveTo(nX +  1,nY +  2) ; pBitMap->Canvas->LineTo(nX +  1,nY + 16);
            pBitMap->Canvas->MoveTo(nX +  2,nY +  3) ; pBitMap->Canvas->LineTo(nX +  2,nY + 16);
            pBitMap->Canvas->MoveTo(nX +  3,nY +  4) ; pBitMap->Canvas->LineTo(nX +  3,nY + 15);
            pBitMap->Canvas->MoveTo(nX +  4,nY +  5) ; pBitMap->Canvas->LineTo(nX +  4,nY + 14);
            pBitMap->Canvas->MoveTo(nX +  5,nY +  6) ; pBitMap->Canvas->LineTo(nX +  5,nY + 13);
        break;
        case 20:
            pBitMap->Canvas->MoveTo(nX +  1,nY +  2) ; pBitMap->Canvas->LineTo(nX +  1,nY + 16);
            pBitMap->Canvas->MoveTo(nX +  2,nY +  3) ; pBitMap->Canvas->LineTo(nX +  2,nY + 16);
            pBitMap->Canvas->MoveTo(nX +  3,nY +  4) ; pBitMap->Canvas->LineTo(nX +  3,nY + 15);
            pBitMap->Canvas->MoveTo(nX +  4,nY +  5) ; pBitMap->Canvas->LineTo(nX +  4,nY + 14);
            pBitMap->Canvas->MoveTo(nX +  5,nY +  6) ; pBitMap->Canvas->LineTo(nX +  5,nY + 13);
        break;

        default:{
            TPoint points[6];
            points[0] = Point(nX + dWidth * 0.050, nY + dHeight * 0.061);
            points[1] = Point(nX + dWidth * 0.050, nY + dHeight * 0.455);
            points[2] = Point(nX + dWidth * 0.100, nY + dHeight * 0.455);
            points[3] = Point(nX + dWidth * 0.250, nY + dHeight * 0.364);
            points[4] = Point(nX + dWidth * 0.250, nY + dHeight * 0.182);
            pBitMap->Canvas->Polygon(points, 4);
        }
        break;
    }
}

void __fastcall TAZNumShape::DrawBottomLeft(int nX, int nY, int size, Graphics::TBitmap* pBitMap, double dWidth, double dHeight)
{

    // 3 : Bottom Left --------------------------------------
    switch(size) {
        case 6:
            pBitMap->Canvas->MoveTo(nX +  1,nY +  5) ; pBitMap->Canvas->LineTo(nX +  1,nY +  7);
        break;
        case 7:
            pBitMap->Canvas->MoveTo(nX +  1,nY +  6) ; pBitMap->Canvas->LineTo(nX +  1,nY +  9);
        break;
        case 8:
            pBitMap->Canvas->MoveTo(nX +  1,nY +  7) ; pBitMap->Canvas->LineTo(nX +  1,nY + 11);
        break;
        case 9:
            pBitMap->Canvas->MoveTo(nX +  1,nY +  8) ; pBitMap->Canvas->LineTo(nX +  1,nY + 13);
            pBitMap->Canvas->MoveTo(nX +  2,nY +  9) ; pBitMap->Canvas->LineTo(nX +  2,nY + 12);
        break;
        case 10:
            pBitMap->Canvas->MoveTo(nX +  1,nY + 10) ; pBitMap->Canvas->LineTo(nX +  1,nY + 16);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 11) ; pBitMap->Canvas->LineTo(nX +  2,nY + 15);
        break;
        case 11:
            pBitMap->Canvas->MoveTo(nX +  1,nY + 10) ; pBitMap->Canvas->LineTo(nX +  1,nY + 17);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 11) ; pBitMap->Canvas->LineTo(nX +  2,nY + 16);
        break;
        case 12:
            pBitMap->Canvas->MoveTo(nX +  1,nY + 11) ; pBitMap->Canvas->LineTo(nX +  1,nY + 19);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 12) ; pBitMap->Canvas->LineTo(nX +  2,nY + 18);
        break;
        case 13:
            pBitMap->Canvas->MoveTo(nX +  1,nY + 12) ; pBitMap->Canvas->LineTo(nX +  1,nY + 21);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 13) ; pBitMap->Canvas->LineTo(nX +  2,nY + 20);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 14) ; pBitMap->Canvas->LineTo(nX +  3,nY + 19);
        break;
        case 14:
            pBitMap->Canvas->MoveTo(nX +  1,nY + 13) ; pBitMap->Canvas->LineTo(nX +  1,nY + 23);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 14) ; pBitMap->Canvas->LineTo(nX +  2,nY + 22);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 15) ; pBitMap->Canvas->LineTo(nX +  3,nY + 21);
        break;
        case 15:
            pBitMap->Canvas->MoveTo(nX +  1,nY + 14) ; pBitMap->Canvas->LineTo(nX +  1,nY + 25);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 15) ; pBitMap->Canvas->LineTo(nX +  2,nY + 24);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 16) ; pBitMap->Canvas->LineTo(nX +  3,nY + 23);
        break;
        case 16:
            pBitMap->Canvas->MoveTo(nX +  1,nY + 15) ; pBitMap->Canvas->LineTo(nX +  1,nY + 26);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 16) ; pBitMap->Canvas->LineTo(nX +  2,nY + 25);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 17) ; pBitMap->Canvas->LineTo(nX +  3,nY + 24);
            pBitMap->Canvas->MoveTo(nX +  4,nY + 18) ; pBitMap->Canvas->LineTo(nX +  4,nY + 23);
        break;
        case 17:
            pBitMap->Canvas->MoveTo(nX +  1,nY + 15) ; pBitMap->Canvas->LineTo(nX +  1,nY + 27);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 16) ; pBitMap->Canvas->LineTo(nX +  2,nY + 26);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 17) ; pBitMap->Canvas->LineTo(nX +  3,nY + 25);
            pBitMap->Canvas->MoveTo(nX +  4,nY + 18) ; pBitMap->Canvas->LineTo(nX +  4,nY + 24);
        break;
        case 18:
            pBitMap->Canvas->MoveTo(nX +  1,nY + 16) ; pBitMap->Canvas->LineTo(nX +  1,nY + 29);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 17) ; pBitMap->Canvas->LineTo(nX +  2,nY + 28);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 18) ; pBitMap->Canvas->LineTo(nX +  3,nY + 27);
            pBitMap->Canvas->MoveTo(nX +  4,nY + 19) ; pBitMap->Canvas->LineTo(nX +  4,nY + 26);
        break;
        case 19:
            pBitMap->Canvas->MoveTo(nX +  1,nY + 17) ; pBitMap->Canvas->LineTo(nX +  1,nY + 30);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 17) ; pBitMap->Canvas->LineTo(nX +  2,nY + 29);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 18) ; pBitMap->Canvas->LineTo(nX +  3,nY + 28);
            pBitMap->Canvas->MoveTo(nX +  4,nY + 19) ; pBitMap->Canvas->LineTo(nX +  4,nY + 27);
            pBitMap->Canvas->MoveTo(nX +  5,nY + 20) ; pBitMap->Canvas->LineTo(nX +  5,nY + 26);
        break;
        case 20:
            pBitMap->Canvas->MoveTo(nX +  1,nY + 17) ; pBitMap->Canvas->LineTo(nX +  1,nY + 31);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 17) ; pBitMap->Canvas->LineTo(nX +  2,nY + 30);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 18) ; pBitMap->Canvas->LineTo(nX +  3,nY + 29);
            pBitMap->Canvas->MoveTo(nX +  4,nY + 19) ; pBitMap->Canvas->LineTo(nX +  4,nY + 28);
            pBitMap->Canvas->MoveTo(nX +  5,nY + 20) ; pBitMap->Canvas->LineTo(nX +  5,nY + 27);
        break;

        default:{
            TPoint points[6];
            points[0] = Point(nX + dWidth * 0.050, nY + dHeight * 0.515);
            points[1] = Point(nX + dWidth * 0.050, nY + dHeight * 0.909);
            points[2] = Point(nX + dWidth * 0.250, nY + dHeight * 0.788);
            points[3] = Point(nX + dWidth * 0.250, nY + dHeight * 0.606);
            pBitMap->Canvas->Polygon(points, 3);
        }
        break;
    }

}

void __fastcall TAZNumShape::DrawBottom(int nX, int nY, int size, Graphics::TBitmap* pBitMap, double dWidth, double dHeight)
{

    // 4 : Bottom  --------------------------------------
    switch(size) {
        case 6:
            pBitMap->Canvas->MoveTo(nX +  2,nY +  7) ; pBitMap->Canvas->LineTo(nX +  4,nY +  7);
        break;
        case 7:
            pBitMap->Canvas->MoveTo(nX +  2,nY +  9) ; pBitMap->Canvas->LineTo(nX +  5,nY +  9);
        break;
        case 8:
            pBitMap->Canvas->MoveTo(nX +  2,nY + 11) ; pBitMap->Canvas->LineTo(nX +  6,nY + 11);
        break;
        case 9:
            pBitMap->Canvas->MoveTo(nX +  3,nY + 12) ; pBitMap->Canvas->LineTo(nX +  6,nY + 12);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 13) ; pBitMap->Canvas->LineTo(nX +  7,nY + 13);
        break;
        case 10:
            pBitMap->Canvas->MoveTo(nX +  3,nY + 15) ; pBitMap->Canvas->LineTo(nX +  7,nY + 15);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 16) ; pBitMap->Canvas->LineTo(nX +  8,nY + 16);
        break;
        case 11:
            pBitMap->Canvas->MoveTo(nX +  3,nY + 16) ; pBitMap->Canvas->LineTo(nX +  8,nY + 16);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 17) ; pBitMap->Canvas->LineTo(nX +  9,nY + 17);
        break;
        case 12:
            pBitMap->Canvas->MoveTo(nX +  3,nY + 18) ; pBitMap->Canvas->LineTo(nX +  9,nY + 18);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 19) ; pBitMap->Canvas->LineTo(nX + 10,nY + 19);
        break;
        case 13:
            pBitMap->Canvas->MoveTo(nX +  4,nY + 19) ; pBitMap->Canvas->LineTo(nX +  9,nY + 19);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 20) ; pBitMap->Canvas->LineTo(nX + 10,nY + 20);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 21) ; pBitMap->Canvas->LineTo(nX + 11,nY + 21);
        break;
        case 14:
            pBitMap->Canvas->MoveTo(nX +  4,nY + 21) ; pBitMap->Canvas->LineTo(nX + 10,nY + 21);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 22) ; pBitMap->Canvas->LineTo(nX + 11,nY + 22);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 23) ; pBitMap->Canvas->LineTo(nX + 12,nY + 23);
        break;
        case 15:
            pBitMap->Canvas->MoveTo(nX +  4,nY + 23) ; pBitMap->Canvas->LineTo(nX + 11,nY + 23);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 24) ; pBitMap->Canvas->LineTo(nX + 12,nY + 24);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 25) ; pBitMap->Canvas->LineTo(nX + 13,nY + 25);
        break;
        case 16:
            pBitMap->Canvas->MoveTo(nX +  5,nY + 23) ; pBitMap->Canvas->LineTo(nX + 11,nY + 23);
            pBitMap->Canvas->MoveTo(nX +  4,nY + 24) ; pBitMap->Canvas->LineTo(nX + 12,nY + 24);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 25) ; pBitMap->Canvas->LineTo(nX + 13,nY + 25);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 26) ; pBitMap->Canvas->LineTo(nX + 14,nY + 26);
        break;
        case 17:
            pBitMap->Canvas->MoveTo(nX +  5,nY + 24) ; pBitMap->Canvas->LineTo(nX + 12,nY + 24);
            pBitMap->Canvas->MoveTo(nX +  4,nY + 25) ; pBitMap->Canvas->LineTo(nX + 13,nY + 25);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 26) ; pBitMap->Canvas->LineTo(nX + 14,nY + 26);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 27) ; pBitMap->Canvas->LineTo(nX + 15,nY + 27);
        break;
        case 18:
            pBitMap->Canvas->MoveTo(nX +  5,nY + 26) ; pBitMap->Canvas->LineTo(nX + 13,nY + 26);
            pBitMap->Canvas->MoveTo(nX +  4,nY + 27) ; pBitMap->Canvas->LineTo(nX + 14,nY + 27);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 28) ; pBitMap->Canvas->LineTo(nX + 15,nY + 28);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 29) ; pBitMap->Canvas->LineTo(nX + 16,nY + 29);
        break;
        case 19:
            pBitMap->Canvas->MoveTo(nX +  6,nY + 26) ; pBitMap->Canvas->LineTo(nX + 13,nY + 26);
            pBitMap->Canvas->MoveTo(nX +  5,nY + 27) ; pBitMap->Canvas->LineTo(nX + 14,nY + 27);
            pBitMap->Canvas->MoveTo(nX +  4,nY + 28) ; pBitMap->Canvas->LineTo(nX + 15,nY + 28);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 29) ; pBitMap->Canvas->LineTo(nX + 16,nY + 29);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 30) ; pBitMap->Canvas->LineTo(nX + 17,nY + 30);
        break;
        case 20:
            pBitMap->Canvas->MoveTo(nX +  6,nY + 27) ; pBitMap->Canvas->LineTo(nX + 14,nY + 27);
            pBitMap->Canvas->MoveTo(nX +  5,nY + 28) ; pBitMap->Canvas->LineTo(nX + 15,nY + 28);
            pBitMap->Canvas->MoveTo(nX +  4,nY + 29) ; pBitMap->Canvas->LineTo(nX + 16,nY + 29);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 30) ; pBitMap->Canvas->LineTo(nX + 17,nY + 30);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 31) ; pBitMap->Canvas->LineTo(nX + 18,nY + 31);
        break;

        default:{
            TPoint points[6];
            points[0] = Point(nX + dWidth * 0.300, nY + dHeight *0.818 );
            points[1] = Point(nX + dWidth * 0.100, nY + dHeight *0.939 );
            points[2] = Point(nX + dWidth * 0.850, nY + dHeight *0.939 );
            points[3] = Point(nX + dWidth * 0.650, nY + dHeight *0.818 );
            pBitMap->Canvas->Polygon(points, 3);
        }
        break;
    }

}

void __fastcall TAZNumShape::DrawBottomRight(int nX, int nY, int size, Graphics::TBitmap* pBitMap, double dWidth, double dHeight)
{

    // 5 : Bottom Right --------------------------------------
    switch(size) {
         case 6:
            pBitMap->Canvas->MoveTo(nX +  4,nY +  5) ; pBitMap->Canvas->LineTo(nX +  4,nY +  7);
        break;
        case 7:
            pBitMap->Canvas->MoveTo(nX +  5,nY +  6) ; pBitMap->Canvas->LineTo(nX +  5,nY +  9);
        break;
        case 8:
            pBitMap->Canvas->MoveTo(nX +  6,nY +  7) ; pBitMap->Canvas->LineTo(nX +  6,nY + 11);
        break;
        case 9:
            pBitMap->Canvas->MoveTo(nX +  6,nY +  9) ; pBitMap->Canvas->LineTo(nX +  6,nY + 12);
            pBitMap->Canvas->MoveTo(nX +  7,nY +  8) ; pBitMap->Canvas->LineTo(nX +  7,nY + 13);
        break;
        case 10:
            pBitMap->Canvas->MoveTo(nX +  7,nY + 11) ; pBitMap->Canvas->LineTo(nX +  7,nY + 15);
            pBitMap->Canvas->MoveTo(nX +  8,nY + 10) ; pBitMap->Canvas->LineTo(nX +  8,nY + 16);
        break;
        case 11:
            pBitMap->Canvas->MoveTo(nX +  8,nY + 11) ; pBitMap->Canvas->LineTo(nX +  8,nY + 16);
            pBitMap->Canvas->MoveTo(nX +  9,nY + 10) ; pBitMap->Canvas->LineTo(nX +  9,nY + 17);
        break;
        case 12:
            pBitMap->Canvas->MoveTo(nX +  9,nY + 12) ; pBitMap->Canvas->LineTo(nX +  9,nY + 18);
            pBitMap->Canvas->MoveTo(nX + 10,nY + 11) ; pBitMap->Canvas->LineTo(nX + 10,nY + 19);
        break;
        case 13:
            pBitMap->Canvas->MoveTo(nX +  9,nY + 14) ; pBitMap->Canvas->LineTo(nX +  9,nY + 19);
            pBitMap->Canvas->MoveTo(nX + 10,nY + 13) ; pBitMap->Canvas->LineTo(nX + 10,nY + 20);
            pBitMap->Canvas->MoveTo(nX + 11,nY + 12) ; pBitMap->Canvas->LineTo(nX + 11,nY + 21);
        break;
        case 14:
            pBitMap->Canvas->MoveTo(nX + 10,nY + 15) ; pBitMap->Canvas->LineTo(nX + 10,nY + 21);
            pBitMap->Canvas->MoveTo(nX + 11,nY + 14) ; pBitMap->Canvas->LineTo(nX + 11,nY + 22);
            pBitMap->Canvas->MoveTo(nX + 12,nY + 13) ; pBitMap->Canvas->LineTo(nX + 12,nY + 23);
        break;
        case 15:
            pBitMap->Canvas->MoveTo(nX + 11,nY + 16) ; pBitMap->Canvas->LineTo(nX + 11,nY + 23);
            pBitMap->Canvas->MoveTo(nX + 12,nY + 15) ; pBitMap->Canvas->LineTo(nX + 12,nY + 24);
            pBitMap->Canvas->MoveTo(nX + 13,nY + 14) ; pBitMap->Canvas->LineTo(nX + 13,nY + 25);
        break;
        case 16:
            pBitMap->Canvas->MoveTo(nX + 11,nY + 18) ; pBitMap->Canvas->LineTo(nX + 11,nY + 23);
            pBitMap->Canvas->MoveTo(nX + 12,nY + 17) ; pBitMap->Canvas->LineTo(nX + 12,nY + 24);
            pBitMap->Canvas->MoveTo(nX + 13,nY + 16) ; pBitMap->Canvas->LineTo(nX + 13,nY + 25);
            pBitMap->Canvas->MoveTo(nX + 14,nY + 15) ; pBitMap->Canvas->LineTo(nX + 14,nY + 26);
        break;
        case 17:
            pBitMap->Canvas->MoveTo(nX + 12,nY + 18) ; pBitMap->Canvas->LineTo(nX + 12,nY + 24);
            pBitMap->Canvas->MoveTo(nX + 13,nY + 17) ; pBitMap->Canvas->LineTo(nX + 13,nY + 25);
            pBitMap->Canvas->MoveTo(nX + 14,nY + 16) ; pBitMap->Canvas->LineTo(nX + 14,nY + 26);
            pBitMap->Canvas->MoveTo(nX + 15,nY + 15) ; pBitMap->Canvas->LineTo(nX + 15,nY + 27);
        break;
        case 18:
            pBitMap->Canvas->MoveTo(nX + 13,nY + 19) ; pBitMap->Canvas->LineTo(nX + 13,nY + 26);
            pBitMap->Canvas->MoveTo(nX + 14,nY + 18) ; pBitMap->Canvas->LineTo(nX + 14,nY + 27);
            pBitMap->Canvas->MoveTo(nX + 15,nY + 17) ; pBitMap->Canvas->LineTo(nX + 15,nY + 28);
            pBitMap->Canvas->MoveTo(nX + 16,nY + 16) ; pBitMap->Canvas->LineTo(nX + 16,nY + 29);
        break;
        case 19:
            pBitMap->Canvas->MoveTo(nX + 13,nY + 20) ; pBitMap->Canvas->LineTo(nX + 13,nY + 26);
            pBitMap->Canvas->MoveTo(nX + 14,nY + 19) ; pBitMap->Canvas->LineTo(nX + 14,nY + 27);
            pBitMap->Canvas->MoveTo(nX + 15,nY + 18) ; pBitMap->Canvas->LineTo(nX + 15,nY + 28);
            pBitMap->Canvas->MoveTo(nX + 16,nY + 17) ; pBitMap->Canvas->LineTo(nX + 16,nY + 29);
            pBitMap->Canvas->MoveTo(nX + 17,nY + 17) ; pBitMap->Canvas->LineTo(nX + 17,nY + 30);
        break;
        case 20:
            pBitMap->Canvas->MoveTo(nX + 14,nY + 20) ; pBitMap->Canvas->LineTo(nX + 14,nY + 27);
            pBitMap->Canvas->MoveTo(nX + 15,nY + 19) ; pBitMap->Canvas->LineTo(nX + 15,nY + 28);
            pBitMap->Canvas->MoveTo(nX + 16,nY + 18) ; pBitMap->Canvas->LineTo(nX + 16,nY + 29);
            pBitMap->Canvas->MoveTo(nX + 17,nY + 17) ; pBitMap->Canvas->LineTo(nX + 17,nY + 30);
            pBitMap->Canvas->MoveTo(nX + 18,nY + 17) ; pBitMap->Canvas->LineTo(nX + 18,nY + 31);
        break;

        default:{
            TPoint points[6];
            points[0] = Point(nX + dWidth * 0.700, nY + dHeight * 0.606);
            points[1] = Point(nX + dWidth * 0.700, nY + dHeight * 0.788);
            points[2] = Point(nX + dWidth * 0.900, nY + dHeight * 0.909);
            points[3] = Point(nX + dWidth * 0.900, nY + dHeight * 0.515);
            points[4] = Point(nX + dWidth * 0.850, nY + dHeight * 0.515);
            pBitMap->Canvas->Polygon(points, 4);
        }
        break;
    }
}

void __fastcall TAZNumShape::DrawTopRight(int nX, int nY, int size, Graphics::TBitmap* pBitMap, double dWidth, double dHeight)
{

    // 6 : Top Right --------------------------------------
    switch(size) {
        case 6:
            pBitMap->Canvas->MoveTo(nX +  4,nY +  2) ; pBitMap->Canvas->LineTo(nX +  4,nY +  4);
        break;
        case 7:
            pBitMap->Canvas->MoveTo(nX +  5,nY +  2) ; pBitMap->Canvas->LineTo(nX +  5,nY +  5);
        break;
        case 8:
            pBitMap->Canvas->MoveTo(nX +  6,nY +  2) ; pBitMap->Canvas->LineTo(nX +  6,nY +  6);
        break;
        case 9:
            pBitMap->Canvas->MoveTo(nX +  6,nY +  3) ; pBitMap->Canvas->LineTo(nX +  6,nY +  6);
            pBitMap->Canvas->MoveTo(nX +  7,nY +  2) ; pBitMap->Canvas->LineTo(nX +  7,nY +  7);
        break;
        case 10:
            pBitMap->Canvas->MoveTo(nX +  7,nY +  3) ; pBitMap->Canvas->LineTo(nX +  7,nY +  8);
            pBitMap->Canvas->MoveTo(nX +  8,nY +  2) ; pBitMap->Canvas->LineTo(nX +  8,nY +  9);
        break;
        case 11:
            pBitMap->Canvas->MoveTo(nX +  8,nY +  3) ; pBitMap->Canvas->LineTo(nX +  8,nY +  8);
            pBitMap->Canvas->MoveTo(nX +  9,nY +  2) ; pBitMap->Canvas->LineTo(nX +  9,nY +  9);
        break;
        case 12:
            pBitMap->Canvas->MoveTo(nX +  9,nY +  3) ; pBitMap->Canvas->LineTo(nX +  9,nY +  9);
            pBitMap->Canvas->MoveTo(nX + 10,nY +  2) ; pBitMap->Canvas->LineTo(nX + 10,nY + 10);
        break;
        case 13:
            pBitMap->Canvas->MoveTo(nX +  9,nY +  4) ; pBitMap->Canvas->LineTo(nX +  9,nY +  9);
            pBitMap->Canvas->MoveTo(nX + 10,nY +  3) ; pBitMap->Canvas->LineTo(nX + 10,nY + 10);
            pBitMap->Canvas->MoveTo(nX + 11,nY +  2) ; pBitMap->Canvas->LineTo(nX + 11,nY + 11);
        break;
        case 14:
            pBitMap->Canvas->MoveTo(nX + 10,nY +  4) ; pBitMap->Canvas->LineTo(nX + 10,nY + 10);
            pBitMap->Canvas->MoveTo(nX + 11,nY +  3) ; pBitMap->Canvas->LineTo(nX + 11,nY + 11);
            pBitMap->Canvas->MoveTo(nX + 12,nY +  2) ; pBitMap->Canvas->LineTo(nX + 12,nY + 12);
        break;
        case 15:
            pBitMap->Canvas->MoveTo(nX + 11,nY +  4) ; pBitMap->Canvas->LineTo(nX + 11,nY + 11);
            pBitMap->Canvas->MoveTo(nX + 12,nY +  3) ; pBitMap->Canvas->LineTo(nX + 12,nY + 12);
            pBitMap->Canvas->MoveTo(nX + 13,nY +  2) ; pBitMap->Canvas->LineTo(nX + 13,nY + 13);
        break;
        case 16:
            pBitMap->Canvas->MoveTo(nX + 11,nY +  5) ; pBitMap->Canvas->LineTo(nX + 11,nY + 11);
            pBitMap->Canvas->MoveTo(nX + 12,nY +  4) ; pBitMap->Canvas->LineTo(nX + 12,nY + 12);
            pBitMap->Canvas->MoveTo(nX + 13,nY +  3) ; pBitMap->Canvas->LineTo(nX + 13,nY + 13);
            pBitMap->Canvas->MoveTo(nX + 14,nY +  2) ; pBitMap->Canvas->LineTo(nX + 14,nY + 14);
        break;
        case 17:
            pBitMap->Canvas->MoveTo(nX + 12,nY +  5) ; pBitMap->Canvas->LineTo(nX + 12,nY + 11);
            pBitMap->Canvas->MoveTo(nX + 13,nY +  4) ; pBitMap->Canvas->LineTo(nX + 13,nY + 12);
            pBitMap->Canvas->MoveTo(nX + 14,nY +  3) ; pBitMap->Canvas->LineTo(nX + 14,nY + 13);
            pBitMap->Canvas->MoveTo(nX + 15,nY +  2) ; pBitMap->Canvas->LineTo(nX + 15,nY + 14);
        break;
        case 18:
            pBitMap->Canvas->MoveTo(nX + 13,nY +  5) ; pBitMap->Canvas->LineTo(nX + 13,nY + 12);
            pBitMap->Canvas->MoveTo(nX + 14,nY +  4) ; pBitMap->Canvas->LineTo(nX + 14,nY + 13);
            pBitMap->Canvas->MoveTo(nX + 15,nY +  3) ; pBitMap->Canvas->LineTo(nX + 15,nY + 14);
            pBitMap->Canvas->MoveTo(nX + 16,nY +  2) ; pBitMap->Canvas->LineTo(nX + 16,nY + 15);
        break;
        case 19:
            pBitMap->Canvas->MoveTo(nX + 13,nY +  6) ; pBitMap->Canvas->LineTo(nX + 13,nY + 13);
            pBitMap->Canvas->MoveTo(nX + 14,nY +  5) ; pBitMap->Canvas->LineTo(nX + 14,nY + 14);
            pBitMap->Canvas->MoveTo(nX + 15,nY +  4) ; pBitMap->Canvas->LineTo(nX + 15,nY + 15);
            pBitMap->Canvas->MoveTo(nX + 16,nY +  3) ; pBitMap->Canvas->LineTo(nX + 16,nY + 16);
            pBitMap->Canvas->MoveTo(nX + 17,nY +  2) ; pBitMap->Canvas->LineTo(nX + 17,nY + 16);
        break;
        case 20:
            pBitMap->Canvas->MoveTo(nX + 14,nY +  6) ; pBitMap->Canvas->LineTo(nX + 14,nY + 13);
            pBitMap->Canvas->MoveTo(nX + 15,nY +  5) ; pBitMap->Canvas->LineTo(nX + 15,nY + 14);
            pBitMap->Canvas->MoveTo(nX + 16,nY +  4) ; pBitMap->Canvas->LineTo(nX + 16,nY + 15);
            pBitMap->Canvas->MoveTo(nX + 17,nY +  3) ; pBitMap->Canvas->LineTo(nX + 17,nY + 16);
            pBitMap->Canvas->MoveTo(nX + 18,nY +  2) ; pBitMap->Canvas->LineTo(nX + 18,nY + 16);
        break;

        default:{
            TPoint points[6];
            points[0] = Point(nX + dWidth * 0.700,nY + dHeight  * 0.182);
            points[1] = Point(nX + dWidth * 0.700,nY + dHeight  * 0.364);
            points[2] = Point(nX + dWidth * 0.850,nY + dHeight  * 0.455);
            points[3] = Point(nX + dWidth * 0.900,nY + dHeight  * 0.455);
            points[4] = Point(nX + dWidth * 0.900,nY + dHeight  * 0.061);
            pBitMap->Canvas->Polygon(points, 4);
        }
        break;
    }
}

void __fastcall TAZNumShape::DrawCenter(int nX, int nY, int size, Graphics::TBitmap* pBitMap, double dWidth, double dHeight)
{

    // 7 : Center --------------------------------------
    switch(size) {
         case 6:
            pBitMap->Canvas->MoveTo(nX +  2,nY +  4); pBitMap->Canvas->LineTo(nX +  4,nY +  4);
        break;
        case 7:
            pBitMap->Canvas->MoveTo(nX +  2,nY +  5); pBitMap->Canvas->LineTo(nX +  5,nY +  5);
        break;
        case 8:
            pBitMap->Canvas->MoveTo(nX +  2,nY +  6); pBitMap->Canvas->LineTo(nX +  6,nY +  6);
        break;
        case 9:
            pBitMap->Canvas->MoveTo(nX +  3,nY +  6); pBitMap->Canvas->LineTo(nX +  6,nY +  6);
            pBitMap->Canvas->MoveTo(nX +  2,nY +  7); pBitMap->Canvas->LineTo(nX +  7,nY +  7);
        break;
        case 10:
            pBitMap->Canvas->MoveTo(nX +  3,nY +  8); pBitMap->Canvas->LineTo(nX +  7,nY +  8);
            pBitMap->Canvas->MoveTo(nX +  2,nY +  9); pBitMap->Canvas->LineTo(nX +  8,nY +  9);
        break;
        case 11:
            pBitMap->Canvas->MoveTo(nX +  3,nY +  8); pBitMap->Canvas->LineTo(nX +  8,nY +  8);
            pBitMap->Canvas->MoveTo(nX +  2,nY +  9); pBitMap->Canvas->LineTo(nX +  9,nY +  9);
        break;
        case 12:
            pBitMap->Canvas->MoveTo(nX +  3,nY +  9); pBitMap->Canvas->LineTo(nX +  9,nY +  9);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 10); pBitMap->Canvas->LineTo(nX + 10,nY + 10);
        break;
        case 13:
            pBitMap->Canvas->MoveTo(nX +  3,nY + 10); pBitMap->Canvas->LineTo(nX + 10,nY + 10);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 11); pBitMap->Canvas->LineTo(nX + 11,nY + 11);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 12); pBitMap->Canvas->LineTo(nX + 10,nY + 12);
        break;
        case 14:
            pBitMap->Canvas->MoveTo(nX +  3,nY + 11); pBitMap->Canvas->LineTo(nX + 11,nY + 11);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 12); pBitMap->Canvas->LineTo(nX + 12,nY + 12);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 13); pBitMap->Canvas->LineTo(nX + 11,nY + 13);
        break;
        case 15:
            pBitMap->Canvas->MoveTo(nX +  3,nY + 12); pBitMap->Canvas->LineTo(nX + 12,nY + 12);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 13); pBitMap->Canvas->LineTo(nX + 13,nY + 13);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 14); pBitMap->Canvas->LineTo(nX + 12,nY + 14);
        break;
        case 16:
            pBitMap->Canvas->MoveTo(nX +  4,nY + 12); pBitMap->Canvas->LineTo(nX + 12,nY + 12);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 13); pBitMap->Canvas->LineTo(nX + 13,nY + 13);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 14); pBitMap->Canvas->LineTo(nX + 14,nY + 14);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 15); pBitMap->Canvas->LineTo(nX + 13,nY + 15);
        break;
        case 17:
            pBitMap->Canvas->MoveTo(nX +  4,nY + 12); pBitMap->Canvas->LineTo(nX + 13,nY + 12);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 13); pBitMap->Canvas->LineTo(nX + 14,nY + 13);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 14); pBitMap->Canvas->LineTo(nX + 15,nY + 14);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 15); pBitMap->Canvas->LineTo(nX + 14,nY + 15);
        break;
        case 18:
            pBitMap->Canvas->MoveTo(nX +  4,nY + 13); pBitMap->Canvas->LineTo(nX + 14,nY + 13);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 14); pBitMap->Canvas->LineTo(nX + 15,nY + 14);
            pBitMap->Canvas->MoveTo(nX +  2,nY + 15); pBitMap->Canvas->LineTo(nX + 16,nY + 15);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 16); pBitMap->Canvas->LineTo(nX + 15,nY + 16);
        break;
        case 19:
            pBitMap->Canvas->MoveTo(nX +  5,nY + 14); pBitMap->Canvas->LineTo(nX + 14,nY + 14);
            pBitMap->Canvas->MoveTo(nX +  4,nY + 15); pBitMap->Canvas->LineTo(nX + 15,nY + 15);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 16); pBitMap->Canvas->LineTo(nX + 16,nY + 16);
            pBitMap->Canvas->MoveTo(nX +  4,nY + 17); pBitMap->Canvas->LineTo(nX + 15,nY + 17);
        break;
        case 20:
            pBitMap->Canvas->MoveTo( 5,14); pBitMap->Canvas->LineTo(15,14);
            pBitMap->Canvas->MoveTo(nX +  4,nY + 15); pBitMap->Canvas->LineTo(nX + 16,nY + 15);
            pBitMap->Canvas->MoveTo(nX +  3,nY + 16); pBitMap->Canvas->LineTo(nX + 17,nY + 16);
            pBitMap->Canvas->MoveTo(nX +  4,nY + 17); pBitMap->Canvas->LineTo(nX + 16,nY + 17);
            pBitMap->Canvas->MoveTo(nX +  5,nY + 18); pBitMap->Canvas->LineTo(nX + 15,nY + 18);
        break;

        default:{
            TPoint points[6];
            points[0] = Point(nX + dWidth * 0.250 ,nY + dHeight  * 0.424);
            points[1] = Point(nX + dWidth * 0.150 ,nY + dHeight  * 0.485);
            points[2] = Point(nX + dWidth * 0.250 ,nY + dHeight  * 0.545);
            points[3] = Point(nX + dWidth * 0.700 ,nY + dHeight  * 0.545);
            points[4] = Point(nX + dWidth * 0.800 ,nY + dHeight  * 0.485);
            points[5] = Point(nX + dWidth * 0.700 ,nY + dHeight  * 0.424);
            pBitMap->Canvas->Polygon(points, 5);
        }
        break;
    }

}
//---------------------------------------------------------------------------




























//----------------------------------------------------------------------------

__fastcall  TNumShapeColorProperty::TNumShapeColorProperty()
{
    FClickBGColor           = clAqua;
    FClickFontColor         = clBlack;
    FClickRectColor         = clBlue;
    FBGColor                = clBlack;
    FFontColor              = clAqua;
    FRectColor              = clAqua;
    FOldValueColor          = clYellow;
    FChangedFontColor       = clRed;
    FFontShadowColor        = (TColor)0x0505050;

	FDisableBGColor				= clBtnFace;
	FDisableFrontColor			= clGray;
	FDisableFrontShadowColor	= clWhite;
	FDisableRectColor			= clGray;

};

__fastcall  TNumShapeColorProperty::~TNumShapeColorProperty()
{
    // None.
};

void     __fastcall TNumShapeColorProperty::DoOnChange(void)
{
    if(OnChange) OnChange(this);
};

void    __fastcall TNumShapeColorProperty::SetColor(int Index, TColor c)
{
    bool bChanbed = false;
    switch(Index) {
        case 1:  bChanbed = (FClickBGColor != c);        FClickBGColor           = c;    break;
        case 2:  bChanbed = (FClickFontColor != c);      FClickFontColor         = c;    break;
        case 3:  bChanbed = (FClickRectColor != c);      FClickRectColor         = c;    break;
        case 4:  bChanbed = (FBGColor != c);             FBGColor                = c;    break;
        case 5:  bChanbed = (FFontColor != c);           FFontColor              = c;    break;
        case 6:  bChanbed = (FRectColor != c);           FRectColor              = c;    break;
        case 7:  bChanbed = (FOldValueColor != c);       FOldValueColor          = c;    break;
        case 8:  bChanbed = (FChangedFontColor != c);    FChangedFontColor       = c;    break;
        case 9:  bChanbed = (FFontShadowColor != c);     FFontShadowColor        = c;    break;

		case 10:  bChanbed = (FDisableBGColor != c);	 		FDisableBGColor		 		= c;	 break;
		case 11:  bChanbed = (FDisableFrontColor != c);	 		FDisableFrontColor		 	= c;	 break;
		case 12:  bChanbed = (FDisableFrontShadowColor != c);	FDisableFrontShadowColor	= c;	 break;
		case 13:  bChanbed = (FDisableRectColor != c);			FDisableRectColor			= c;	 break;
    }

    if(bChanbed) DoOnChange();// Property 가 변경된 것을 보고한다.
};

void    __fastcall TNumShapeColorProperty::Assign(TPersistent* Source)
{
    TNumShapeColorProperty* pFrom = dynamic_cast<TNumShapeColorProperty *>(Source);

    if(pFrom != NULL) //Source 가 NULL 이 아니고 안전하게 타입캐스팅 되었다면
    {
        FClickBGColor       = pFrom->ClickBGColor;
        FClickFontColor     = pFrom->ClickFontColor;
        FClickRectColor     = pFrom->ClickRectColor;
        FBGColor            = pFrom->BGColor;
        FFontColor          = pFrom->FontColor;
        FRectColor          = pFrom->RectColor;
        FOldValueColor      = pFrom->OldValueColor;
        FChangedFontColor   = pFrom->ChangedFontColor;
    }
}
//----------------------------------------------------------------------------










//----------------------------------------------------------------------------
__fastcall  TNumShapeTitleProperty::TNumShapeTitleProperty()
{
    FVisible     = false;
    FCaption     = L"Caption";
    FWidth       = 100;
    FAlignment   = taCenter;
    FAlign       = caLeft;
    FColor       = clBtnFace;
    FTransparent = true;

    FOutLineVisible = true;
    FOutLineColor   = clBlack;
    FRound          = 5;
    FSpace          = 0;
    FMargin         = 0;

    //------------------------------------------------
    // Unit을 표현할 Font를 생성한다.
    FFont           = new Vcl::Graphics::TFont;
    // Font가 변경될 경우 이를 감지할 Call Back 함수를 등록한다.
    FFont->OnChange = FontChanged;

    FFont->Name     = "Arial";
    FFont->Size     = 12;
//    FFont->Style     = TFontStyles() << fsBold;
//    FFont->Color    = clBlack;
}

__fastcall TNumShapeTitleProperty::~TNumShapeTitleProperty()
{
    if(FFont) delete FFont;
}

void     __fastcall TNumShapeTitleProperty::DoOnChange(void)
{
    if(FOnChange) FOnChange(this);  //TNumUnitProperty 와 이벤트 핸들러 연결
}

void     __fastcall TNumShapeTitleProperty::FontChanged(System::TObject* Sender)
{
    DoOnChange();
}

void    __fastcall TNumShapeTitleProperty::Assign(TPersistent* Source)
{
    TNumShapeTitleProperty* pFrom = dynamic_cast<TNumShapeTitleProperty *>(Source);

    if(pFrom != NULL) //Source 가 NULL 이 아니고 안전하게 타입캐스팅 되었다면
    {
        FVisible        = pFrom->Visible;
        FCaption        = pFrom->Caption;
        FWidth          = pFrom->Width;
        FAlignment      = pFrom->Alignment;
        FAlign          = pFrom->FAlign;
        FColor          = pFrom->Color;
        FTransparent    = pFrom->Transparent;
        FOutLineVisible = pFrom->OutLineVisible;
        FOutLineColor   = pFrom->OutLineColor;
        FRound          = pFrom->Round;
        FSpace          = pFrom->Space;
        FFont->Assign(pFrom->Font);
    }
}

void    __fastcall  TNumShapeTitleProperty::SetVisible(bool b)
{
    if(FVisible != b) {
        FVisible = b;
        DoOnChange();
    }
}

void    __fastcall  TNumShapeTitleProperty::SetCaption(UnicodeString s)
{
    if(FCaption != s) {
        FCaption = s;
        DoOnChange();
    }
}

void    __fastcall  TNumShapeTitleProperty::SetWidth(int n)
{
    if(FWidth != n) {
        FWidth = n;
        DoOnChange();
    }
}

void    __fastcall  TNumShapeTitleProperty::SetFont(Vcl::Graphics::TFont * p)
{
    FFont->Assign(p);
    DoOnChange();
}

void    __fastcall  TNumShapeTitleProperty::SetAlignment(System::Classes::TAlignment a)
{
    if(FAlignment != a) {
        FAlignment = a;
        DoOnChange();
    }
}

void    __fastcall  TNumShapeTitleProperty::SetColor(TColor c)
{
    if(FColor != c) {
        FColor = c;
        DoOnChange();
    }
}

void    __fastcall  TNumShapeTitleProperty::SetTransparent(bool b)
{
    if(FTransparent != b) {
        FTransparent = b;
        DoOnChange();
    }
}

void    __fastcall  TNumShapeTitleProperty::SetOutLineVisible(bool b)
{
    if(FOutLineVisible != b) {
        FOutLineVisible = b;
        DoOnChange();
    }
}

void    __fastcall  TNumShapeTitleProperty::SetOutLineColor(TColor c)
{
    if(FOutLineColor != c) {
        FOutLineColor = c;
        DoOnChange();
    }
}

void    __fastcall  TNumShapeTitleProperty::SetRound(int n)
{
    if(FRound != n) {
        FRound = n;
        DoOnChange();
    }
}

void    __fastcall  TNumShapeTitleProperty::SetSpace(int n)
{
    if(FSpace != n) {
        FSpace = n;
        DoOnChange();
    }
}

void    __fastcall  TNumShapeTitleProperty::SetAlign(TCaptionAlign a)
{
    if(FAlign != a) {
        FAlign = a;
        DoOnChange();
    }
}

void    __fastcall  TNumShapeTitleProperty::SetMargin(int n)
{
    if(FMargin != n) {
        FMargin = n;
        DoOnChange();
    }
}








