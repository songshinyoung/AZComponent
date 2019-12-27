//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop
USEFORM("AZLabelImage_Dsgn.cpp", fmAZLabelImageEditor);
USEFORM("frmInputImageSize.cpp", fmInputImageSize);
USEFORM("frmInputStrings.cpp", fmInputStrBox);
USEFORM("AZNumShape_Dsgn.cpp", fmAZNumShapeEditor);
USEFORM("Common\frmNumPad_AZ.cpp", fmNumPad_AZ);
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
