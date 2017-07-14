//---------------------------------------------------------------------------

#ifndef AZNumberPadH
#define AZNumberPadH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include "AZUnitProperty.h"
//---------------------------------------------------------------------------
class PACKAGE TAZNumberPad : public TComponent
{
private:
    double FValue;
    double FCurrentValue;
    TAZEditBoxProperty      * FSetting;   ///< Input Number Pad Property

    void    __fastcall SetCurrentValue(double d);
    void    __fastcall SetInputEdit(TAZEditBoxProperty * p);

    void    __fastcall SettingChanged(System::TObject* Sender);
    bool    __fastcall InputNumber();

protected:
public:
    __fastcall TAZNumberPad(TComponent* Owner);
    virtual __fastcall ~TAZNumberPad();

    bool __fastcall Execute();
    bool __fastcall Execute(TInputNumType eType, double dMin, double dMax, double dCurnt, double dUpDownValue);

__published:
    __property TAZEditBoxProperty     * Setting   = { read = FSetting,  write = SetInputEdit };
    __property double Value = { read = FValue };
    __property double CurrentValue = { read = FCurrentValue, write = SetCurrentValue };

};
//---------------------------------------------------------------------------
#endif
