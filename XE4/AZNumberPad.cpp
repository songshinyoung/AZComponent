//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "AZNumberPad.h"
#include "frmNumPad_AZ.h"
#include "Common\\Define.h"
#include "Common\\GlobalFunction_AZ.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAZNumberPad *)
{
    new TAZNumberPad(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAZNumberPad::TAZNumberPad(TComponent* Owner)
    : TComponent(Owner)
{
    FSetting = new TAZEditBoxProperty;
    FSetting->OnChange = SettingChanged;
}
//---------------------------------------------------------------------------
__fastcall TAZNumberPad::~TAZNumberPad()
{
    if(FSetting) delete FSetting;
}
//---------------------------------------------------------------------------
//namespace Aznumberpad
//{
//    void __fastcall PACKAGE Register()
//    {
//         TComponentClass classes[1] = {__classid(TAZNumberPad)};
//         RegisterComponents(L"AZ", classes, 0);
//    }
//}

//---------------------------------------------------------------------------

bool __fastcall TAZNumberPad::Execute()
{
    bool Result = false;

    try
    {
        Result = InputNumber();
    }
    catch(...)
    {
        Result = false; // if it fails, set Result to
    }

    return Result;
}
//----------------------------------------------------------------------------
bool __fastcall TAZNumberPad::Execute(TInputNumType eType, double dMin, double dMax, double dCurnt, double dUpDownValue)
{
    FSetting->Type          = eType;
    FSetting->Min           = dMin;
    FSetting->Max           = dMax;
    FCurrentValue           = dCurnt;
    FSetting->UpDownValue   = dUpDownValue;

    return Execute();
}
//----------------------------------------------------------------------------
bool     __fastcall TAZNumberPad::InputNumber()
{
    AnsiString sReturn;

    TfmNumPad_AZ  * pForm = new TfmNumPad_AZ(this->Owner,
                                        FSetting->Type,
                                        FSetting->Min,
                                        FSetting->Max,
                                        FCurrentValue,
                                        &sReturn,
                                        FSetting->UpDownValue,
                                        FSetting->DecimalPlaces);

    int nRet = pForm->ShowModal();

    delete pForm;

    if(nRet == mrOk) {

        switch(FSetting->Type) {
            case itInt:
                FValue  = sReturn.ToIntDef(0);
                break;

            case itFloat:
                {
                    double dTemp = sReturn.ToDouble();
                    FValue = DecimalRounding(dTemp, FSetting->DecimalPlaces);
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

//----------------------------------------------------------------------------
void __fastcall TAZNumberPad::SetInputEdit(TAZEditBoxProperty * p)
{
    if(p) {
        FSetting->Assign(p);
        //Invalidate();// 화면을 갱신한다.
    }
}

//----------------------------------------------------------------------------
void  __fastcall TAZNumberPad::SettingChanged(System::TObject* Sender)
{
    //Invalidate();// 화면을 갱신한다.
}
//----------------------------------------------------------------------------
void    __fastcall TAZNumberPad::SetCurrentValue(double d)
{
    FCurrentValue = d;
}
//----------------------------------------------------------------------------
