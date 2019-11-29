//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop
USEFORM("AZLabelImage_Dsgn.cpp", fmAZLabelImageEditor);
USEFORM("AZNumShape_Dsgn.cpp", fmAZNumShapeEditor);
USEFORM("Common\frmNumPad_AZ.cpp", fmNumPad_AZ);
USEFORM("frmInputImageSize.cpp", fmInputImageSize);
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Package source.
//---------------------------------------------------------------------------


#pragma argsused
extern "C" int _libmain(unsigned long reason)
{
    return 1;
}
//---------------------------------------------------------------------------
