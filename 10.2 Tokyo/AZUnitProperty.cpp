//---------------------------------------------------------------------------

#pragma hdrstop

#include "AZUnitProperty.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall  TNumUnitProperty::TNumUnitProperty()
{
    FVisible        = true;
    FUnit           = "unit";        ///< ���� ǥ�� (�ܼ� ǥ���)
    FWidth          = 60;        ///< Unit�� ǥ��Ǵ� ���� ����

    //------------------------------------------------
    // Unit�� ǥ���� Font�� �����Ѵ�.
    FFont           = new Vcl::Graphics::TFont;
    // Font�� ����� ��� �̸� ������ Call Back �Լ��� ����Ѵ�.
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
    if(FOnChange) FOnChange(this);  //TNumUnitProperty �� �̺�Ʈ �ڵ鷯 ����
}

//---------------------------------------------------------------------------
/**
* @brief : Font�� �Ӽ��� ����� ��� Call Back �Լ��̴�.
* @param System::TObject* Sender :

* @return void
* @see
- history : 1. [2013�� 4�� 2�� ȭ����][�۽ſ�] ���� �ۼ�
**/
void     __fastcall TNumUnitProperty::UnitFontChanged(System::TObject* Sender)
{
    DoOnChange();
}

void    __fastcall TNumUnitProperty::Assign(TPersistent* Source)
{
    TNumUnitProperty* pFrom = dynamic_cast<TNumUnitProperty *>(Source);

    if(pFrom != NULL) //Source �� NULL �� �ƴϰ� �����ϰ� Ÿ��ĳ���� �Ǿ��ٸ�
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
/// ������/���� �Ǽ��� ������Ƽ�� �۽ý���Ʈ ��ī������ ������ ���� �ذ�.
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

#else // ������/���� �Ǽ��� ������Ƽ�� �۽ý���Ʈ ��ī������ ������ ���� �ذ�.
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
    if(FOnChange) FOnChange(this);  // �� �Ӽ��� ����� ��� ���� ������Ʈ�� �ݹ� �̹�Ʈ �Լ��� ȣ���� �ش�.
}

void    __fastcall TAZEditBoxProperty::Assign(TPersistent* Source)
{
    TAZEditBoxProperty* pFrom = dynamic_cast<TAZEditBoxProperty *>(Source);

    if(pFrom != NULL) //Source �� NULL �� �ƴϰ� �����ϰ� Ÿ��ĳ���� �Ǿ��ٸ�
    {
        FType           = pFrom->Type;
        FMin            = pFrom->Min;
        FMax            = pFrom->Max;
        FUpDownValue    = pFrom->UpDownValue;
        FDecimalPlaces  = pFrom->DecimalPlaces;
    }
}

