//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "AZFlowShape.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAZFlowShape *)
{
    new TAZFlowShape(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAZFlowShape::TAZFlowShape(TComponent* Owner)
    : TGraphicControl(Owner)
{
    FCol                = 3;
    FRow                = 3;
    FRound              = 2;
    FSpace              = 2;
    FOutLineSpace       = 1;
    FCaptionVisible     = true;
    FTransparent        = true;
    FColorOutLine       = clGray;
    FCellLineColorUse   = true;
    FCellLineColor      = clGray;
    FInvalidateUse      = false;

    Width               = 200;
    Height              = 200;

    TGraphicControl::OnClick        = MyClick;
    TGraphicControl::OnMouseDown    = MyMouseDown;
    TGraphicControl::OnMouseUp      = MyMouseUp;

    FTitle  = new TAZTitleProperty();
    FTitle->OnChange = TitleChanged;

    Font->Name = "Arail";

    bLoaded = false;
    bBeginUpdate = false;
    nCreateCellCount = 0;
    CreateCells();

    //--------------------------
    if(ComponentState.Contains(csDesigning)) {
        bLoaded = true;
        Invalidate();
    }      
    
}
//---------------------------------------------------------------------------
__fastcall TAZFlowShape::~TAZFlowShape()
{
    DeleteCells();
    if(FTitle) delete FTitle;
}
//---------------------------------------------------------------------------
void __fastcall TAZFlowShape::Loaded(void)
{
    TGraphicControl::Loaded();

    bLoaded = true;

    CreateCells();

    Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TAZFlowShape::WndProc(TMessage& Message)
{
    switch(Message.Msg) {
         case WM_LBUTTONDOWN:
            {
                int nY = (Message.LParam & 0xFFFF0000) >> 16;
                int nX = (Message.LParam & 0x0000FFFF);

                nClickRow = -1;
                nClickCol = -1;

                if(FindCell(nX, nY, nClickRow, nClickCol)) {
                    //AnsiString sTemp;
                    //sTemp.printf("Row=%d, Col=%d", nClickRow, nClickCol);
                    //MessageBox(NULL, sTemp.c_str(), "", MB_OK);
                }
            }
            break;

         case WM_LBUTTONUP:
            break;

         case WM_MOUSEHOVER:
            break;

         case WM_MOUSELEAVE:
            break;
    }

    switch(Message.Msg) {
        default:
            TGraphicControl::WndProc(Message);
            break;
    }

}
//---------------------------------------------------------------------------
bool __fastcall TAZFlowShape::FindCell(const int x, const int y, int &Row, int &Col)
{
    bool bFound = false;


    int nTitleHeight = FTitle->Visible ? (FTitle->Height + FTitle->Space) : 0;

    int nCellW = ((Width  - (FOutLineSpace * 2)) / FCol) - FSpace;
    int nCellH = ((Height - (FOutLineSpace * 2) - nTitleHeight) / FRow) - FSpace;

    //int nIndex = 0;
    int nTxtW;
    int nTxtH;
    int nX = 0;
    int nY = 0;
    int nW;
    int nH;

    //int nRealWidth  = (nCellW + FSpace) * FCol + (FOutLineSpace * 2);
    //int nRealHeight = (nCellH + FSpace) * FRow + (FOutLineSpace * 2);

    if( y <= nTitleHeight) {
        return false;
    }

    for(int i=0; i<FRow; i++) {
        nY = FOutLineSpace + (FSpace / 2) + (i * (nCellH + FSpace)) + nTitleHeight;

        if((nY <= y) && ((nY + nCellH) >= y)) {
            for(int j=0; j<FCol; j++) {

                nX = FOutLineSpace + (FSpace / 2) + (j * (nCellW + FSpace));

                if((nX <= x) && ((nX + nCellW) >= x)) {
                    Row = i;
                    Col = j;
                    bFound = true;
                    break;
                }
            }

            break;
        }
    }

    return bFound;
}
//---------------------------------------------------------------------------
void    __fastcall TAZFlowShape::MyClick(TObject *Sender)
{
    if(FOnClick) {
        if(nClickRow >=0 && nClickCol >= 0) {
            // 정확하게 Cell을 클릭한 경우만 Click Event 발생한다.
            FOnClick(this, nClickRow, nClickCol);
        }
    }
}

//---------------------------------------------------------------------------
void    __fastcall TAZFlowShape::MyMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(FOnMouseDown) {
        if(nClickRow >=0 && nClickCol >= 0) {
            // 정확하게 Cell을 클릭한 경우만 Down Event 발생한다.
            FOnMouseDown(this, Button, Shift, X, Y, nClickRow, nClickCol);
        }
    }
}

//---------------------------------------------------------------------------
void    __fastcall TAZFlowShape::MyMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(FOnMouseUp) {
        if(nClickRow >=0 && nClickCol >= 0) {
            // 정확하게 Cell을 클릭한 경우만 Down Event 발생한다.
            FOnMouseUp(this, Button, Shift, X, Y, nClickRow, nClickCol);
        }
    }
}
//---------------------------------------------------------------------------
// Enable / Disable 시에 색상 지정.
void __fastcall TAZFlowShape::SetEnabled(bool b)
{
    TGraphicControl::SetEnabled(b);
}

void     __fastcall TAZFlowShape::CellChanged(System::TObject* Sender)
{
    if(!bBeginUpdate){
        if(FInvalidateUse)  Invalidate();
        else                DrawPaint(true);
    }
}

void     __fastcall TAZFlowShape::TitleChanged(System::TObject* Sender)
{
    if(!bBeginUpdate){
        Invalidate();
    }
}
//---------------------------------------------------------------------------

void __fastcall TAZFlowShape::Paint()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;
    DrawPaint(false);

}
//---------------------------------------------------------------------------
void __fastcall TAZFlowShape::DrawPaint(bool bOnlyCellUpdate)
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(nCreateCellCount <= 0)   return;
    if(bBeginUpdate)            return;
    if(!bLoaded)                return;

    int nTitleHeight = FTitle->Visible ? (FTitle->Height + FTitle->Space) : 0;

    int nCellW = ((Width  - (FOutLineSpace * 2)) / FCol) - FSpace;
    int nCellH = ((Height - (FOutLineSpace * 2) - nTitleHeight) / FRow) - FSpace;

    int nIndex = 0;
    int nTxtW;
    int nTxtH;
    int nX = 0;
    int nY = 0;
    int nW;
    int nH;

    int nRealWidth  = (nCellW + FSpace) * FCol + (FOutLineSpace * 2);
    int nRealHeight = (nCellH + FSpace) * FRow + (FOutLineSpace * 2);


    if(FTitle->Visible) {
        Canvas->Font->Assign(FTitle->Font);
        Canvas->Pen->Color      = FTitle->LineColor;
        Canvas->Brush->Color    = FTitle->Color;

        if(FTitle->Transparent != true) {
            Canvas->Rectangle(0, 0, nRealWidth, FTitle->Height);
        }


        nTxtW = Canvas->TextWidth(FTitle->Caption);
        nTxtH = Canvas->TextHeight(FTitle->Caption);

        nX = 0;
        nY = (FTitle->Height - nTxtH) / 2;

        switch(FTitle->Alignment) {
            case taLeftJustify:     nX = 2;
                break;
            case taRightJustify:    nX = nRealWidth - nTxtW - 2;
                break;
            case taCenter:          nX = (nRealWidth - nTxtW)/2;   break;
        }

        SetBkMode(Canvas->Handle, TRANSPARENT);     // 투명 출력.
        Canvas->TextOutA(nX, nY, FTitle->Caption);

    }

    Canvas->Font->Assign(Font);

    if(!FTransparent && !bOnlyCellUpdate) {
        Canvas->Pen->Color      = FColorOutLine;
        Canvas->Brush->Color    = Color;

        Canvas->RoundRect(  0,
                            nTitleHeight,
                            nRealWidth,
                            nRealHeight + nTitleHeight,  // RoundRect은 절대 좌표를 사용한다.
                            FRound,
                            FRound);
    }

    for(int i=0; i<FRow; i++) {
        for(int j=0; j<FCol; j++) {
            nIndex = (i*FCol) + j;

            if(!bOnlyCellUpdate || FppCells[nIndex]->bChanged) {
                FppCells[nIndex]->bChanged = false;

                nX = FOutLineSpace + (FSpace / 2) + (j * (nCellW + FSpace));
                nY = FOutLineSpace + (FSpace / 2) + (i * (nCellH + FSpace)) + nTitleHeight;
                nW = nCellW;
                nH = nCellH;

                if(FCellLineColorUse) {
                    Canvas->Pen->Color   = FCellLineColor;
                }
                else {
                    Canvas->Pen->Color   = (FppCells[nIndex]) != NULL ? FppCells[nIndex]->LineColor : FCellLineColor;
                }

                Canvas->Brush->Color = (FppCells[nIndex]) != NULL ? FppCells[nIndex]->BGColor   : clWhite;
                Canvas->RoundRect(nX, nY, nX + nW, nY + nH, FRound, FRound);

                if(FCaptionVisible && (FppCells[nIndex] != NULL)) {
                    if(FppCells[nIndex]->Caption != "") {
                        if(FppCells[nIndex]->SpecialFontColor) {
                            Canvas->Font->Color = FppCells[nIndex]->FontColor;
                            if(FppCells[nIndex]->FontSize > 0) Canvas->Font->Size  = FppCells[nIndex]->FontSize;
                        }
                        
                        nTxtW = Canvas->TextWidth(FppCells[nIndex]->Caption);
                        nTxtH = Canvas->TextHeight(FppCells[nIndex]->Caption);

                        SetBkMode(Canvas->Handle, TRANSPARENT);     // 투명 출력.

                        Canvas->TextOutA(nX + (nW - nTxtW) / 2 , nY + (nH - nTxtH) / 2 , FppCells[nIndex]->Caption);

                        if(FppCells[nIndex]->SpecialFontColor) {
                            Canvas->Font->Color = Font->Color;
                            Canvas->Font->Size  = Font->Size;
                        }
                    }
                }
            }
        }
    }
}


void __fastcall TAZFlowShape::DeleteCells()
{
    int nCount = nCreateCellCount;
    nCreateCellCount = 0;

    if(FppCells) {
        for(int i=0; i<nCount; i++) {
            if(FppCells[i]) {
                delete FppCells[i];
                FppCells[i] = NULL;
            }
        }

        delete[] FppCells;
        FppCells = NULL;
    }
}

void __fastcall TAZFlowShape::CreateCells()
{
    DeleteCells();

    FppCells = new TFlowShapeCell * [FCol * FRow];

#if 0
    for(int r=0; r<FRow; r++) {
        for(int c=0; c<FCol; c++) {
            FppCells[r*FCol + c] = new TFlowShapeCell(r, c);
            FppCells[r*FCol + c]->OnChange   = CellChanged;
        }
    }
#else
    for(int i=0; i<(FCol * FRow); i++) {
        FppCells[i] = new TFlowShapeCell();
        FppCells[i]->OnChange   = CellChanged;
    }
#endif
    nCreateCellCount = FCol * FRow;
}
//---------------------------------------------------------------------------
TFlowShapeCell * __fastcall TAZFlowShape::GetCells(int nRow, int nCol)
{
    if(nRow >= FRow || nCol >= FCol) return NULL;

    int nIndex = (nRow * FCol) + nCol;

    return FppCells[nIndex];
}

//---------------------------------------------------------------------------
void    __fastcall  TAZFlowShape::BeginUpdate()
{
    bBeginUpdate = true;
}
//---------------------------------------------------------------------------
void    __fastcall  TAZFlowShape::EndUpdate()
{
    bBeginUpdate = false;

    if(FInvalidateUse)  Invalidate();
    else                DrawPaint(true);

}
//---------------------------------------------------------------------------
//void __fastcall TAZFlowShape::SetCells(int nRow, int nCol)
//{
//
//}

void __fastcall TAZFlowShape::SetCol(int n)
{
    if(FCol != n) {
        FCol = n;
        CreateCells();
        if(!bBeginUpdate) Invalidate();
    }
}

void __fastcall TAZFlowShape::SetRow(int n)
{
    if(FRow != n) {
        FRow = n;
        CreateCells();
        if(!bBeginUpdate) Invalidate();
    }
}
void    __fastcall TAZFlowShape::SetSpace(int n)
{
    if(FSpace != n) {
        FSpace = n;
        if(!bBeginUpdate) Invalidate();
    }
}

void    __fastcall TAZFlowShape::SetRound(int n)
{
    if(FRound != n) {
        FRound = n;
        if(!bBeginUpdate) Invalidate();
    }
}

void    __fastcall TAZFlowShape::SetCaptionVisible(bool b)
{
    if(FCaptionVisible != b) {
        FCaptionVisible = b;
        if(!bBeginUpdate) Invalidate();
    }
}

void    __fastcall TAZFlowShape::SetTransparent(bool b)
{
    if(FTransparent != b) {
        FTransparent = b;
        if(!bBeginUpdate) Invalidate();
    }
}

void    __fastcall TAZFlowShape::SetColorOutLine(TColor c)
{
    if(FColorOutLine != c) {
        FColorOutLine = c;
        if(!bBeginUpdate) Invalidate();
    }
}

void    __fastcall TAZFlowShape::SetOutLineSpace(int n)
{
    if(FOutLineSpace != n) {
        FOutLineSpace = n;
        if(!bBeginUpdate) Invalidate();
    }
}

void    __fastcall TAZFlowShape::SetCellLineColorUse(bool b)
{
    if(FCellLineColorUse != b) {
        FCellLineColorUse = b;
        if(!bBeginUpdate) Invalidate();
    }
}

void    __fastcall TAZFlowShape::SetCellLineColor(TColor c)
{
    if(FCellLineColor != c) {
        FCellLineColor = c;
        if(!bBeginUpdate) Invalidate();
    }
}

void    __fastcall TAZFlowShape::SetTitle(TAZTitleProperty * p)
{
    if(p) {
        FTitle->Assign(p);
        if(!bBeginUpdate) Invalidate();
    }
}

void    __fastcall TAZFlowShape::SetInvalidateUse(bool b)
{
    if(FInvalidateUse != b) {
        FInvalidateUse = b;
    }
}

TAZTitleProperty * __fastcall TAZFlowShape::GetTitle()
{
    return FTitle;
}
//---------------------------------------------------------------------------








//---------------------------------------------------------------------------
//__fastcall TFlowShapeCell::TFlowShapeCell(int r, int c)
//: Row(r), Col(c)
__fastcall TFlowShapeCell::TFlowShapeCell()
{
    FTag        = 0;
    FBGColor    = clWhite;
    FLineColor  = clGray;
    FFontColor  = clBlack;
    FFontSize   = 0;
    bChanged    = false;
    
}

void __fastcall TFlowShapeCell::SetTag(int n)
{
    if(FTag != n) {
        FTag = n;
        DoOnChange();
    }
}

void __fastcall TFlowShapeCell::SetCaption(String s)
{
    if(FCaption != s) {
        FCaption = s;
        DoOnChange();
    }
}

void __fastcall TFlowShapeCell::SetBGColor(TColor c)
{
    if(FBGColor != c) {
        FBGColor = c;
        DoOnChange();
    }
}

void __fastcall TFlowShapeCell::SetLineColor(TColor c)
{
    if(FLineColor != c) {
        FLineColor = c;
        DoOnChange();
    }
}

void __fastcall TFlowShapeCell::SetFontColor(TColor c)
{
    if(FFontColor != c) {
        FFontColor = c;
        DoOnChange();
    }
}

void __fastcall TFlowShapeCell::SetFontSize(int n)
{
    if(FFontSize != n) {
        FFontSize = n;
        DoOnChange();
    }
}

void __fastcall TFlowShapeCell::SetSpecialFontColor(bool b)
{
    if(FSpecialFontColor != b) {
        FSpecialFontColor = b;
        DoOnChange();
    }
}

void __fastcall TFlowShapeCell::DoOnChange(void)
{
    bChanged = true;
    if(FOnChange) FOnChange(this);
}
//---------------------------------------------------------------------------











//---------------------------------------------------------------------------

__fastcall  TAZTitleProperty::TAZTitleProperty()
{
    FVisible        = true;
    FCaption        = L"Title";
    FAlignment      = taCenter;
    FHeight         = 20;
    FSpace          = 1;
    FColor          = clWhite;
    FLineColor      = clBlack;
    FTransparent    = false;

    //------------------------------------------------
    // Unit을 표현할 Font를 생성한다.
    FFont           = new Vcl::Graphics::TFont;
    // Font가 변경될 경우 이를 감지할 Call Back 함수를 등록한다.
    FFont->OnChange = FontChanged;

    FFont->Name     = "Arial";
    FFont->Size     = 10;
    FFont->Style    = TFontStyles() << fsBold;
    FFont->Color    = clBlack;

}
__fastcall TAZTitleProperty::~TAZTitleProperty()
{
    if(FFont) delete FFont;
}

void  __fastcall TAZTitleProperty::Assign(TPersistent* Source)
{
    TAZTitleProperty * pFrom = dynamic_cast<TAZTitleProperty *>(Source);

    if(pFrom) {
        FVisible        = pFrom->Visible;
        FCaption        = pFrom->Caption;
        FAlignment      = pFrom->Alignment;
        FTransparent    = pFrom->Transparent;
        FHeight         = pFrom->Height;
        FSpace          = pFrom->Space;
        FColor          = pFrom->Color;
        FLineColor      = pFrom->LineColor;
        FFont->Assign(pFrom->Font);

        DoOnChange();
    }
}

void     __fastcall TAZTitleProperty::SetVisible(bool b)
{
    if(FVisible != b) {
        FVisible = b;
        DoOnChange();
    }
}

void     __fastcall TAZTitleProperty::SetCaption(String s)
{
    if(FCaption != s) {
        FCaption = s;
        DoOnChange();
    }
}

void     __fastcall TAZTitleProperty::SetAlignment(TAlignment e)
{
    if(FAlignment != e) {
        FAlignment = e;
        DoOnChange();
    }
}

void     __fastcall TAZTitleProperty::SetFont(Vcl::Graphics::TFont * p)
{
    FFont->Assign(p);
    DoOnChange();
}

void    __fastcall TAZTitleProperty::SetHeight(int n)
{
    if(FHeight != n) {
        FHeight = n;
        DoOnChange();
    }
}

void    __fastcall TAZTitleProperty::SetSpace(int n)
{
    if(FSpace != n) {
        FSpace = n;
        DoOnChange();
    }
}

void    __fastcall TAZTitleProperty::SetColor(int Index, TColor c)
{
    bool bChanged = false;

    switch(Index) {
        case 1: bChanged = FColor != c;      FColor = c;        break;
        case 2: bChanged = FLineColor != c;  FLineColor = c;    break;
    }

    if(bChanged) DoOnChange();
}

void    __fastcall TAZTitleProperty::SetTransparent(bool b)
{
    if(FTransparent != b) {
        FTransparent = b;
        DoOnChange();
    }
}

void     __fastcall TAZTitleProperty::DoOnChange(void)
{
    if(FOnChange) FOnChange(this);  //TAZTitleProperty 와 이벤트 핸들러 연결
}

//--------------------------------------
/**
* @brief : Font의 속성이 변경될 경우 Call Back 함수이다.
* @param System::TObject* Sender :

* @return void
* @see
- history : 1. [2013년 4월 2일 화요일][송신영] 최초 작성
**/
void     __fastcall TAZTitleProperty::FontChanged(System::TObject* Sender)
{
    DoOnChange();
}
//---------------------------------------------------------------------------









//---------------------------------------------------------------------------

