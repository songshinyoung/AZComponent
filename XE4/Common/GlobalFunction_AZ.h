//---------------------------------------------------------------------------

#ifndef GlobalFunction_AZH
#define GlobalFunction_AZH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Common\\Define.h"
//---------------------------------------------------------------------------
String      __fastcall ChangeString(const String &src, const String &sTarget, const String &sChange);
double      __fastcall DecimalRounding(double dSrc, int nPoint);
AnsiString  __fastcall GetDrawValue(double dValue, TInputNumType Type, int DecimalPlaces);
int         __fastcall GetIntWidth(int n);
#endif
