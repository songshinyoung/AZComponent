//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop
#include <Define.h>

#include "AZDateTimeView.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAZDateTimeView *)
{
    new TAZDateTimeView(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAZDateTimeView::TAZDateTimeView(TComponent* Owner)
    : TGraphicControl(Owner)
{
    tmpBitmap1          = new Graphics::TBitmap;
    FOutLineVisible     = true;
    FTransparent        = false;
    FDateTime           = Now();
    FOutLineColor       = clHotLight;
    FSpace              = 0;
    FRound              = 0;
    FAlignment          = taCenter;
    FFormat             = "YYYY-MM-DD HH:MM:SS";

    this->Font->Size    = 12;
    this->Font->Color   = clBlack;

    Color               = clWhite;
    Width               = 170;
    Height              = 28;

    pTimer              = new TTimer(GetParentForm(this));
    FTimerEnabled       = false;
    FTimerInterval      = 1000;

    pTimer->Interval    = FTimerInterval;
    pTimer->Enabled     = FTimerEnabled;
    pTimer->OnTimer     = MyOnTimer;

    FOnTimer            = NULL;

}   

__fastcall TAZDateTimeView::~TAZDateTimeView()
{
    if(pTimer) {
        pTimer->Enabled = false;
        delete pTimer;
    }
    
    if(tmpBitmap1) delete tmpBitmap1;
}

//---------------------------------------------------------------------------
// dfm ���� ���� �Ӽ����� ��� �ε��� �� �� �Լ��� ȣ�� ��.
void __fastcall TAZDateTimeView::Loaded(void)
{
    TGraphicControl::Loaded();

    Invalidate();
}

//-----------------------------------------------------------------------
void __fastcall TAZDateTimeView::Paint()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    String sDateTime = FormatDateTime(FFormat, FDateTime); 
    Vcl::Graphics::TCanvas* BuffCanvas = NULL;
    
    tmpBitmap1->Width  = this->Width;
    tmpBitmap1->Height = this->Height;

    TColor clTransparentColor = FTransparent ? Parent->Brush->Color : MY_TRANSPARENTCOLOR;
    
    if(tmpBitmap1->Width > 0 && tmpBitmap1->Height > 0) {
        
        BuffCanvas = tmpBitmap1->Canvas;
        SetBkMode(BuffCanvas->Handle, TRANSPARENT);

        // �ϴ� ��� ��ü�� ��������� ĥ�Ѵ�. (�𼭸� Round ������ ������ Transparent�� �����Ѵ�.)
        tmpBitmap1->Transparent         = true;
        tmpBitmap1->TransparentColor    = clTransparentColor; 
        BuffCanvas->Brush->Color        = clTransparentColor; 
        BuffCanvas->FillRect(Rect(0, 0, this->Width, this->Height));

        // Round Rect�� �׸���.(���� ���� ������ ��� �����, ������ �ƴ� ��� Color�� �����Ѵ�.)
        BuffCanvas->Brush->Color        = FTransparent ? clTransparentColor : this->Color; 
        BuffCanvas->Brush->Style        = bsSolid;
        BuffCanvas->Pen->Color          = FOutLineVisible ? FOutLineColor : BuffCanvas->Brush->Color;
        BuffCanvas->RoundRect(0, 0, this->Width, this->Height, FRound, FRound);

        // ��¥�� ����Ѵ�. 
        BuffCanvas->Font->Assign(this->Font);
        int nTxtW = BuffCanvas->TextWidth(sDateTime);
        int nTxtH = BuffCanvas->TextHeight(sDateTime);

        int nStartX = 0;
        int nStartY = (this->Height - nTxtH) / 2;

        switch(FAlignment) {
            case taLeftJustify:
                nStartX = FSpace;
                break;
            case taRightJustify:
                nStartX = (this->Width - nTxtW) - FSpace;
                break;
            case taCenter:
                nStartX = (this->Width - nTxtW) / 2;
                break;
        }
        
        BuffCanvas->TextOutA( nStartX , nStartY, sDateTime);

        // ��¥ �� �� ��� ��� OutLine�� ��ĥ ��츦 ����Ͽ� OutLine�� �ٽ� �׷��ش�. 
        if(FOutLineVisible) {
            BuffCanvas->Brush->Style = bsClear;
            BuffCanvas->Pen->Color = FOutLineColor;
            BuffCanvas->RoundRect(0, 0, this->Width, this->Height, FRound, FRound);
        }

        this->Canvas->Lock();
        this->Canvas->Draw(0, 0, tmpBitmap1);
        this->Canvas->Unlock();        
    }
    
    
}
//-----------------------------------------------------------------------

void __fastcall TAZDateTimeView::SetDateTime(TDateTime d)
{
    if(FDateTime != d) {
        FDateTime = d;
        FTransparent ? Invalidate() : Paint();
    }
}

void __fastcall TAZDateTimeView::SetFormat(String s)
{
    if(FFormat != s) {
        FFormat = s;
        //Paint();
        Invalidate();
    }
}

void __fastcall TAZDateTimeView::SetTransparent(bool b)
{
    if(FTransparent != b) {
        FTransparent = b;
        //Paint();
        Invalidate();
    }
}

void __fastcall TAZDateTimeView::SetAlignment(System::Classes::TAlignment a)
{
    if(FAlignment != a) {
        FAlignment = a;
        //Paint();
        Invalidate();
    }
}

void __fastcall TAZDateTimeView::SetOutLine(bool b)
{
    if(FOutLineVisible != b) {
        FOutLineVisible = b;
        //Paint();
        Invalidate();
    }
}

void __fastcall TAZDateTimeView::SetOutLineColor(TColor c)
{
    if(FOutLineColor != c) {
        FOutLineColor = c;
        //Paint();
        Invalidate();
    }
}

void __fastcall TAZDateTimeView::SetSpace(int n)
{
    if(FSpace != n) {
        FSpace = n;
        //Paint();
        Invalidate();
    }
}

void __fastcall TAZDateTimeView::SetRound(int n)
{
    if(FRound != n) {
        FRound = n;
        //Paint();
        Invalidate();
    }
}

void __fastcall TAZDateTimeView::SetTimerInterval(int n)
{
    if(FTimerInterval != n) {
        FTimerInterval = n;
        if(pTimer) pTimer->Interval = FTimerInterval;
    }
}

void __fastcall TAZDateTimeView::SetTimerEnabled(bool b)
{
    if(FTimerEnabled != b) {
        FTimerEnabled = b;
        if(pTimer) pTimer->Enabled = FTimerEnabled;
    }
}

//---------------------------------------------------------------------------
void __fastcall TAZDateTimeView::MyOnTimer(TObject *Sender)
{
    if(ComponentState.Contains(csDesigning) != true) { // ������ Ÿ�ӿ��� ������� �ʵ��� �Ѵ�. 
        FDateTime = Now();
        FTransparent ? Invalidate() : Paint();

        if(FOnTimer) FOnTimer(this);
    }
}

void __fastcall TAZDateTimeView::SetOnTimer(System::Classes::TNotifyEvent Value)
{   
    FOnTimer = Value;
}

