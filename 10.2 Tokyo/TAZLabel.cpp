//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "TAZLabel.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAZLabel *)
{
    new TAZLabel(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAZLabel::TAZLabel(TComponent* Owner)
    : TLabel(Owner)
{
}
//---------------------------------------------------------------------------
namespace Tazlabel
{
    void __fastcall PACKAGE Register()
    {
        TComponentClass classes[1] = {__classid(TAZLabel)};
        RegisterComponents(L"Samples", classes, 0);
    }
}
//---------------------------------------------------------------------------
