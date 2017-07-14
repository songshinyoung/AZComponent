//---------------------------------------------------------------------------

#pragma hdrstop

#include "AZUnitProperty.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall  TNumUnitProperty::TNumUnitProperty()
{
    FVisible        = true;
    FUnit           = "unit";        ///< 단위 표기 (단순 표기용)
    FWidth          = 60;        ///< Unit이 표기되는 영역 넓이

    //------------------------------------------------
    // Unit을 표현할 Font를 생성한다.
    FFont           = new Vcl::Graphics::TFont;
    // Font가 변경될 경우 이를 감지할 Call Back 함수를 등록한다.
    FFont->OnChange = UnitFontChanged;

    FFont->Name     = AZ_NUM_SHAPE_FONT;
    FFont->Size     = 14;
    FFont->Style    = TFontStyles() << fsBold;
    FFont->Color    = clGray;

}
__fastcall TNumUnitProperty::~TNumUnitProperty()
{
      if(FFont) delete FFont;
}

void     __fastcall TNumUnitProperty::SetVisible(bool b)
{
    if(FVisible != b) {
        FVisible = b;
        DoOnChange();
    }
}

void     __fastcall TNumUnitProperty::SetStr(AnsiString s)
{
    if(FUnit != s) {
        FUnit = s;
        DoOnChange();
    }
}

void     __fastcall TNumUnitProperty::SetWidth(int n)
{
    if(FWidth != n) {
        FWidth = n;
        DoOnChange();
    }
}

void     __fastcall TNumUnitProperty::SetFont(Vcl::Graphics::TFont * p)
{
    FFont->Assign(p);
    DoOnChange();
}

void     __fastcall TNumUnitProperty::DoOnChange(void)
{
    if(FOnChange) FOnChange(this);  //TNumUnitProperty 와 이벤트 핸들러 연결
}

//---------------------------------------------------------------------------
/**
* @brief : Font의 속성이 변경될 경우 Call Back 함수이다.
* @param System::TObject* Sender :

* @return void
* @see
- history : 1. [2013년 4월 2일 화요일][송신영] 최초 작성
**/
void     __fastcall TNumUnitProperty::UnitFontChanged(System::TObject* Sender)
{
    DoOnChange();
}

void    __fastcall TNumUnitProperty::Assign(TPersistent* Source)
{
    TNumUnitProperty* pFrom = dynamic_cast<TNumUnitProperty *>(Source);

    if(pFrom != NULL) //Source 가 NULL 이 아니고 안전하게 타입캐스팅 되었다면
    {
        FVisible    = pFrom->Visible;
        FUnit       = pFrom->Str;
        FWidth      = pFrom->Width;
        FFont->Assign(pFrom->Font);
    }
}




//==============================================================================
__fastcall  TAZEditBoxProperty::TAZEditBoxProperty()
{
    // Property default set.
    FType                   = itFloat;
    FMin                    = -1000000.0;
    FMax                    =  1000000.0;
    FUpDownValue            = 1.0;
    FDecimalPlaces          = 3;
    FUpNumberOfDecimals     = 6;
    FDigitSize              = 10;
    FDigitGap               = 1;
    
}

__fastcall TAZEditBoxProperty::~TAZEditBoxProperty()
{

}

//---------------------------------------------------------------------------
/// 델파이/빌더 실수형 프로퍼티의 퍼시스턴트 메카니즘의 디자인 오류 해결.
void __fastcall TAZEditBoxProperty::DefineProperties(TFiler *filer)
{
    TPersistent::DefineProperties(filer);

    filer->DefineProperty(L"Min",             NULL, SetMin, true);
    filer->DefineProperty(L"Max",             NULL, SetMax, true);
    filer->DefineProperty(L"UpDownValue",     NULL, SetUpDownValue, true);
}

void    __fastcall TAZEditBoxProperty::SetType(TInputNumType e)
{
    if(FType != e) {
        FType = e;
        DoOnChange();
    }
}

void    __fastcall TAZEditBoxProperty::SetDecimalPlaces(int n)
{
    if(FDecimalPlaces != n) {
        FDecimalPlaces = n;
        DoOnChange();
    }
}

void    __fastcall TAZEditBoxProperty::SetUpNumberOfDecimals(int n)
{
    if(FUpNumberOfDecimals != n) {
        FUpNumberOfDecimals = n;
        DoOnChange();
    }
}
void    __fastcall TAZEditBoxProperty::SetDigitSize(int n)
{
    if(FDigitSize != n) {
        FDigitSize = n;
        DoOnChange();
    }
}

void    __fastcall TAZEditBoxProperty::SetDigitGap(int n)
{
    if(FDigitGap != n) {
        FDigitGap = n;
        DoOnChange();
    }
}
    
//---------------------------------------------------------------------------
#if 0
void    __fastcall TAZEditBoxProperty::SetMin(double d)
{
    FMin = d;
    DoOnChange();
}

void    __fastcall TAZEditBoxProperty::SetMax(double d)
{
    FMax = d;
    DoOnChange();
}

void    __fastcall TAZEditBoxProperty::SetUpDownValue(double d)
{
    FUpDownValue = d;
    DoOnChange();
}

#else // 델파이/빌더 실수형 프로퍼티의 퍼시스턴트 메카니즘의 디자인 오류 해결.
void __fastcall TAZEditBoxProperty::SetMin(TWriter *writer)
{
    writer->WriteDouble(FMin);
    DoOnChange();
}

void __fastcall TAZEditBoxProperty::SetMax(TWriter *writer)
{
    writer->WriteDouble(FMax);
    DoOnChange();
}

void __fastcall TAZEditBoxProperty::SetUpDownValue(TWriter *writer)
{
    writer->WriteDouble(FUpDownValue);
    DoOnChange();
}
#endif

void     __fastcall TAZEditBoxProperty::DoOnChange(void)
{
    if(FOnChange) FOnChange(this);  // 내 속성이 변경된 경우 상위 컴포넌트에 콜백 이밴트 함수를 호출해 준다.
}

void    __fastcall TAZEditBoxProperty::Assign(TPersistent* Source)
{
    TAZEditBoxProperty* pFrom = dynamic_cast<TAZEditBoxProperty *>(Source);

    if(pFrom != NULL) //Source 가 NULL 이 아니고 안전하게 타입캐스팅 되었다면
    {
        FType           = pFrom->Type;
        FMin            = pFrom->Min;
        FMax            = pFrom->Max;
        FUpDownValue    = pFrom->UpDownValue;
        FDecimalPlaces  = pFrom->DecimalPlaces;
    }
}

