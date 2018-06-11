//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "AZServoState.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAZServoState *)
{
    new TAZServoState(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAZServoState::TAZServoState(TComponent* Owner)
    : TGraphicControl(Owner)
{
    bLoaded             = false;
    FIndex              = 0;
    Color               = clGray;
    FIndicatorRectColor = clWhite;
    OutRectColor        = clBlack;
    Width               = 110;
    Height              = 60;
    FIndexVisible       = true;
    FCaptionVisible     = true;
    Visible             = true;

    for(int i=0; i<_AZ_SERVO_STATE_MAX; i++ ) {
        FIndicator[i] = new TAZServoStateIndicatorProperty(i);
    }

    FIndicator[_AZ_SERVO_STATE_SERVOON]->ColorOn        = clLime;
    FIndicator[_AZ_SERVO_STATE_SERVOON]->ColorOff       = clBlack;
    FIndicator[_AZ_SERVO_STATE_SERVOON]->FontColorOn    = clWhite;
    FIndicator[_AZ_SERVO_STATE_SERVOON]->FontColorOff   = clGray;

    FIndicator[_AZ_SERVO_STATE_HOME]->ColorOn           = clBlue;
    FIndicator[_AZ_SERVO_STATE_HOME]->ColorOff          = clBlack;
    FIndicator[_AZ_SERVO_STATE_HOME]->FontColorOn       = clWhite;
    FIndicator[_AZ_SERVO_STATE_HOME]->FontColorOff      = clGray;

    FIndicator[_AZ_SERVO_STATE_ALARM]->ColorOn          = clRed;
    FIndicator[_AZ_SERVO_STATE_ALARM]->ColorOff         = clBlack;
    FIndicator[_AZ_SERVO_STATE_ALARM]->FontColorOn      = clWhite;
    FIndicator[_AZ_SERVO_STATE_ALARM]->FontColorOff     = clGray;

    FIndicator[_AZ_SERVO_STATE_PLIMIT]->ColorOn         = clMaroon;
    FIndicator[_AZ_SERVO_STATE_PLIMIT]->ColorOff        = clBlack;
    FIndicator[_AZ_SERVO_STATE_PLIMIT]->FontColorOn     = clWhite;
    FIndicator[_AZ_SERVO_STATE_PLIMIT]->FontColorOff    = clGray;

    FIndicator[_AZ_SERVO_STATE_NLIMIT]->ColorOn         = clMaroon;
    FIndicator[_AZ_SERVO_STATE_NLIMIT]->ColorOff        = clBlack;
    FIndicator[_AZ_SERVO_STATE_NLIMIT]->FontColorOn     = clWhite;
    FIndicator[_AZ_SERVO_STATE_NLIMIT]->FontColorOff    = clGray;

    // 동적 생성 프로퍼티의 속성들의 Default 값을 설정 한 후에
    // OnChange Event를 등록해야한다. 먼저 등록 후 Property 속성을 변경하면
    // 컴포넌트 생성 시 Error 발생한다.
    for(int i=0; i<_AZ_SERVO_STATE_MAX; i++ ) {
        FIndicator[i]->OnChange = IndicatorChanged;
    }

    FFontIndicator              = new Vcl::Graphics::TFont;
    FFontIndicator->Name        = "Arial";
    FFontIndicator->Style       = TFontStyles() << fsBold;
    FFontIndicator->OnChange    = IndicatorFontChanged;

    Font->Name  = "Arial";
    Font->Style = TFontStyles() << fsBold;

    tmpBitmap1 = new Graphics::TBitmap;

    //--------------------------
    if(ComponentState.Contains(csDesigning)) {
        bLoaded = true;
    }      
}
//---------------------------------------------------------------------------
__fastcall TAZServoState::~TAZServoState()
{
    for(int i=0; i<_AZ_SERVO_STATE_MAX; i++ ) {
        if(FIndicator[i]) delete FIndicator[i];
    }

    if(FFontIndicator) delete FFontIndicator;

    if(tmpBitmap1)  delete tmpBitmap1;
}
//---------------------------------------------------------------------------
//namespace Azservostate
//{
//    void __fastcall PACKAGE Register()
//    {
//         TComponentClass classes[1] = {__classid(TAZServoState)};
//         RegisterComponents(L"AZ", classes, 0);
//    }
//}
//---------------------------------------------------------------------------
void  __fastcall TAZServoState::Loaded(void)
{
    TGraphicControl::Loaded();

    bLoaded = true;
}

//---------------------------------------------------------------------------
void  __fastcall TAZServoState::Paint()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(!bLoaded) return;

    // -------------------------------------------------------
    // 외곽 그리기
    int nTitleH     = Height * 60 / 100; // 전체 높이의 60 % 차지;
    //int nIndicatorH = Height - nTitleH;
    //int nIndicatorW = (Width - 2) / _AZ_SERVO_STATE_MAX;
    int nRound      = 0;
    //int nRoundIndicator = 0;
    int nTxtW = 0, nTxtH = 0;

    tmpBitmap1->Width  = Width;
    tmpBitmap1->Height = Height;

    tmpBitmap1->Canvas->Pen->Color = Enabled ? FOutRectColor : (TColor)0x00505050;
    tmpBitmap1->Canvas->Pen->Width = 1;
    tmpBitmap1->Canvas->Brush->Style = bsSolid;
    tmpBitmap1->Canvas->Brush->Color = Enabled ? Color : clSilver;
    tmpBitmap1->Canvas->RoundRect(0, 0, Width, Height, nRound, nRound);

    // Title 그리기 ---------------------------------
    tmpBitmap1->Canvas->Font->Assign(Font);
    if(!Enabled) tmpBitmap1->Canvas->Font->Color = clWhite;

    // Index 출력
    String sIndex = "[" + IntToStr(FIndex) + "]";
    nTxtW = tmpBitmap1->Canvas->TextWidth(sIndex);
    nTxtH = tmpBitmap1->Canvas->TextHeight(sIndex);
    int nTitleGap = (nTitleH - (nTxtH*2))/3;
    int nTextStartY = nTitleGap;

    if(FIndexVisible) {
        nTextStartY = FCaptionVisible ? nTitleGap : (nTitleH - nTxtH) / 2;
        tmpBitmap1->Canvas->TextOutA( (Width - nTxtW) / 2, nTextStartY, sIndex);
    }

    // Caption 출력.
    if(FCaptionVisible) {
        nTxtW = tmpBitmap1->Canvas->TextWidth(Caption);
        nTxtH = tmpBitmap1->Canvas->TextHeight(Caption);
        nTextStartY = FIndexVisible ? (nTitleGap * 2 + nTxtH) - 1 : (nTitleH - nTxtH) / 2;
        tmpBitmap1->Canvas->TextOutA( (Width - nTxtW) / 2, nTextStartY, Caption);
    }

    // Indicator 그리기 --------------------------------

    //int nX      = 2;
    int nY      = nTitleH;
    //int nX_End  = 0;
    //int nY_End  = nY + nIndicatorH - 2;

    // Indicator 외곽 라인 그리기.
    tmpBitmap1->Canvas->Brush->Color = Enabled ? FIndicatorRectColor : clGray;
    tmpBitmap1->Canvas->FillRect(Rect(1, nY-1, Width - 1, Height - 1));

    // Indicator 내부 그리기.
    for(int i=0; i<_AZ_SERVO_STATE_MAX; i++){
        DrawIndicator(i, tmpBitmap1->Canvas);
    }

    Canvas->Lock();

    Canvas->Draw(0,0,tmpBitmap1);

    Canvas->Unlock();
}
//---------------------------------------------------------------------------
void  __fastcall TAZServoState::DrawIndicator(int nID, Vcl::Graphics::TCanvas* pCanvas)
{
    if(pCanvas == NULL) return;
    if(nID < 0 || nID >= _AZ_SERVO_STATE_MAX) return;

    int nIndicatorW = (Width - 2) / _AZ_SERVO_STATE_MAX;
    int nX          = 2 + (nIndicatorW * nID);
    int nY          = (Height * 60) / 100;
    int nX_End      = 0;
    int nY_End      = Height - 2;
    int nTxtW       = 0;
    int nTxtH       = 0;
    int nIndicatorH = Height - nY;

    pCanvas->Lock();

    // Indicator Background 그리기.
    nX_End = nX + nIndicatorW - 1;
    nX_End = ((nID + 1) == _AZ_SERVO_STATE_MAX) ? Width - 2 : nX_End;  // 마지막 Indicator의 경우 오른쪽 면에 붙게 하기 위해.

    if(Enabled) {
        pCanvas->Brush->Color = FIndicator[nID]->StateOn ? FIndicator[nID]->ColorOn : FIndicator[nID]->ColorOff;
    }
    else {
        pCanvas->Brush->Color = clBtnFace;
    }

    pCanvas->FillRect(Rect(nX, nY, nX_End, nY_End));

    // Indicator Text 출력
    String sIndicatorName;

    switch(nID) {
        case _AZ_SERVO_STATE_SERVOON:   sIndicatorName = "S"; break;
        case _AZ_SERVO_STATE_HOME:      sIndicatorName = "H"; break;
        case _AZ_SERVO_STATE_ALARM:     sIndicatorName = "A"; break;
        case _AZ_SERVO_STATE_PLIMIT:    sIndicatorName = "P"; break;
        case _AZ_SERVO_STATE_NLIMIT:    sIndicatorName = "N"; break;
        default :                       sIndicatorName = "";  break;
    }

    pCanvas->Font->Assign(FFontIndicator);
    pCanvas->Brush->Style = bsSolid;

    if(Enabled) {
        pCanvas->Font->Color  = FIndicator[nID]->StateOn ? FIndicator[nID]->FontColorOn : FIndicator[nID]->FontColorOff;
    }
    else {
        pCanvas->Font->Color  = clGray;
    }

    nTxtW = pCanvas->TextWidth(sIndicatorName);
    nTxtH = pCanvas->TextHeight(sIndicatorName);
    pCanvas->TextOutA( nX + ((nIndicatorW - nTxtW) / 2), nY + ((nIndicatorH - nTxtH)/2), sIndicatorName);

    pCanvas->Unlock();
}
//---------------------------------------------------------------------------

void    __fastcall TAZServoState::SetIndex(int n)
{
    if(FIndex != n) {
        FIndex = n;
        Paint();
    }
}
//---------------------------------------------------------------------------
void    __fastcall TAZServoState::SetIndicator(int Index, TAZServoStateIndicatorProperty * p)
{
    if(p != NULL && Index < _AZ_SERVO_STATE_MAX ){
        FIndicator[Index]->Assign(p);
        //Paint();
        DrawIndicator(Index, Canvas);
    }
}
//---------------------------------------------------------------------------
TAZServoStateIndicatorProperty * __fastcall TAZServoState::GetIndicator(int Index)
{
    if(Index < _AZ_SERVO_STATE_MAX ){
        return FIndicator[Index];
    }
    return NULL;
}
//---------------------------------------------------------------------------
void    __fastcall TAZServoState::SetFontIndicator(Vcl::Graphics::TFont    * f)
{
    if(f != NULL){
        FFontIndicator->Assign(f);
        //Paint();
        DrawIndicator(Index, Canvas);
    }
}
//---------------------------------------------------------------------------
void    __fastcall TAZServoState::SetIndicatorRectColor(TColor c)
{
    if(FIndicatorRectColor != c){
        FIndicatorRectColor = c;
        Paint();
    }
}
//---------------------------------------------------------------------------
void    __fastcall TAZServoState::SetOutRectColor(TColor c)
{
    if(FOutRectColor != c){
        FOutRectColor = c;
        Paint();
    }
}
//---------------------------------------------------------------------------
void    __fastcall TAZServoState::SetIndexVisible(bool b)
{
    if(FIndexVisible != b){
        FIndexVisible = b;
        Paint();
    }
}
//---------------------------------------------------------------------------
void    __fastcall TAZServoState::SetCaptionVisible(bool b)
{
    if(FCaptionVisible != b){
        FCaptionVisible = b;
        Paint();
    }
}
//---------------------------------------------------------------------------
void    __fastcall TAZServoState::IndicatorFontChanged(System::TObject* Sender)
{
    //Paint();
    DrawIndicator(Index, Canvas);
}
//---------------------------------------------------------------------------
void    __fastcall TAZServoState::IndicatorChanged(System::TObject* Sender)
{
    TAZServoStateIndicatorProperty * pProperty = dynamic_cast<TAZServoStateIndicatorProperty *>(Sender);
    if(pProperty == NULL) return;
    //Paint();
    DrawIndicator(pProperty->GetID(), Canvas);
}
//---------------------------------------------------------------------------











//---------------------------------------------------------------------------

__fastcall  TAZServoStateIndicatorProperty::TAZServoStateIndicatorProperty(int id)
{
    nID = id;
    FState          = false;
    FColorOn        = clLime;
    FColorOff       = clBlack;
    FFontColorOn    = clWhite;
    FFontColorOff   = clGray;
}

__fastcall TAZServoStateIndicatorProperty::~TAZServoStateIndicatorProperty()
{
//
}

void        __fastcall TAZServoStateIndicatorProperty::SetState(bool b)
{
    if(FState != b) {
        FState = b;
        DoOnChange();
    }
}

void        __fastcall TAZServoStateIndicatorProperty::SetColor(int Index, TColor c)
{
    bool bChanged = false;
    switch(Index) {
        case 0: if(FColorOn != c)      {  FColorOn      = c; bChanged = true; } break;
        case 1: if(FColorOff != c)     {  FColorOff     = c; bChanged = true; } break;
        case 2: if(FFontColorOn != c)  {  FFontColorOn  = c; bChanged = true; } break;
        case 3: if(FFontColorOff != c) {  FFontColorOff = c; bChanged = true; } break;
    }

    if(bChanged) DoOnChange();
}

TColor      __fastcall TAZServoStateIndicatorProperty::GetColor(int Index)
{
    switch(Index) {
        case 0:   return FColorOn;
        case 1:   return FColorOff;
        case 2:   return FFontColorOn;
        case 3:   return FFontColorOff;
        default : return clBlack;
    }
}

void    __fastcall TAZServoStateIndicatorProperty::DoOnChange(void)
{
    if(FOnChange) FOnChange(this);  // 내 속성이 변경된 경우 상위 컴포넌트에 콜백 이밴트 함수를 호출해 준다.
}

int     __fastcall TAZServoStateIndicatorProperty::GetID()
{
    return nID;
}

void    __fastcall TAZServoStateIndicatorProperty::Assign(TPersistent* Source)
{
    TAZServoStateIndicatorProperty* pFrom = dynamic_cast<TAZServoStateIndicatorProperty *>(Source);
    if(pFrom == NULL) return;

    FState          = pFrom->StateOn;
    FColorOn        = pFrom->ColorOn;
    FColorOff       = pFrom->ColorOff;
    FFontColorOn    = pFrom->FontColorOn;
    FFontColorOff   = pFrom->FontColorOff;
}
//---------------------------------------------------------------------------

