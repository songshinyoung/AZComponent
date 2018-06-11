//---------------------------------------------------------------------------

#pragma hdrstop

#include "GlobalFunction_AZ.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

/**
* @brief : String src�� sTarget�� ���Ե� ��� ��� sChange�� �����ϴ� �Լ��̴�.
* @param const String &src     : ���� String
* @param const String &sTarget : ã�� �ܾ�
* @param const String &sChange : ������ �ܾ�
* @return String : ã�� �ܾ ã�Ƽ� ������ �ܾ�� ���� �� ���� �ۼ��� String�� �����Ѵ�.
* @see
- history : 1. [2014�� 7�� 2�� ������][�۽ſ�] ���� �ۼ�
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
* @brief : nPoint �� �Ҽ��� �ڸ���ŭ �ݿø��Ͽ� �����Ѵ�.
           3.14159 �� nPoint �� 2 ��� 3.14�� �����Ѵ�.
           ���� nPoint�� 3�̶�� 3.142 �� �����Ѵ�.
* @param double dSrc : �ݿø��� ������ ��
* @param int nPoint :  �ݿø��� �Ҽ��� �ڸ�
* @return double :
* @see
- history : 1. [2013�� 4�� 2�� ȭ����][�۽ſ�] ���� �ۼ�
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
* @brief   : ���ڷ� �־��� int ���� �ڸ����� ���ϴ� �Լ�. 
* @details : 
* @param   : int n : 
* @return  : int : 
* @see     : 
- history  : 1. [2016�� 8�� 5�� �ݿ���][�۽ſ�] ���� �ۼ� 
**/
int         __fastcall GetIntWidth(int num)
{
    int ret = 0;
    if (num < 0) num = -num;
    for (ret = 0; num > 0; num /= 10, ret++){;} /* NULL */;
    return ret;
}
