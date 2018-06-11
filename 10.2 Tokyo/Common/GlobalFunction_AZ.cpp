//---------------------------------------------------------------------------

#pragma hdrstop

#include "GlobalFunction_AZ.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

/**
* @brief : String src에 sTarget이 포함된 경우 모두 sChange로 변경하는 함수이다.
* @param const String &src     : 원본 String
* @param const String &sTarget : 찾을 단어
* @param const String &sChange : 변경할 단어
* @return String : 찾을 단어를 찾아서 변경할 단어로 변경 후 새로 작성된 String을 리턴한다.
* @see
- history : 1. [2014년 7월 2일 수요일][송신영] 최초 작성
*/
String __fastcall ChangeString(
    const String &src,
    const String &sTarget,
    const String &sChange)
{
    String sTemp = src;
    int nSize = sTarget.Length();
    int nPos  = sTemp.Pos(sTarget);

    while(nPos) {
        sTemp.Delete(nPos, nSize);
        sTemp.Insert(sChange, nPos);
        nPos = sTemp.Pos(sTarget);
    }

    return sTemp;
}

//---------------------------------------------------------------------------
/**
* @brief : nPoint 의 소수점 자리만큼 반올림하여 리턴한다.
           3.14159 를 nPoint 가 2 라면 3.14를 리턴한다.
           만약 nPoint가 3이라면 3.142 를 리턴한다.
* @param double dSrc : 반올림할 원래의 값
* @param int nPoint :  반올림할 소수점 자리
* @return double :
* @see
- history : 1. [2013년 4월 2일 화요일][송신영] 최초 작성
**/
double __fastcall DecimalRounding(double dSrc, int nPoint)
{
    int k=pow((double)10,nPoint);
    double Result=floor(dSrc * k + 0.5) / k;
    return Result;
}


//---------------------------------------------------------------------------
AnsiString __fastcall GetDrawValue(double dValue, TInputNumType Type, int DecimalPlaces)
{
    AnsiString sOutputText;

    switch(Type) {
        case itInt:     sOutputText = IntToStr((int)dValue);
            break;

        case itFloat:
            {
                AnsiString sParam;
                AnsiString sTemp;
                if(DecimalPlaces < 1) {
                    DecimalPlaces = 1;
                }
                //else if(DecimalPlaces > 4){
                //    DecimalPlaces = 4;
                //}

                sParam = "%0." + IntToStr(DecimalPlaces) + "f";
                sTemp.printf(sParam.c_str(), dValue);

                sOutputText = sTemp;
            }
            break;

        default:        sOutputText = dValue;
            break;
    }

    return sOutputText;

}

/**
* @fn      : int         __fastcall GetIntWidth(int n);
* @brief   : 인자로 주어진 int 값의 자리수를 구하는 함수. 
* @details : 
* @param   : int n : 
* @return  : int : 
* @see     : 
- history  : 1. [2016년 8월 5일 금요일][송신영] 최초 작성 
**/
int         __fastcall GetIntWidth(int num)
{
    int ret = 0;
    if (num < 0) num = -num;
    for (ret = 0; num > 0; num /= 10, ret++){;} /* NULL */;
    return ret;
}
