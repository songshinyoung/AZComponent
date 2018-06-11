//---------------------------------------------------------------------------

#ifndef AZUnitPropertyH
#define AZUnitPropertyH
//---------------------------------------------------------------------------

#include <sysutils.hpp>
#include <controls.hpp>
#include <classes.hpp>
#include "Common\\Define.h"

//---------------------------------------------------------------------------
//Unit property를 위한 class
// 참고 : http://cbuilder.borlandforum.com/impboard/impboard.dll?action=read&db=bcb_tutorial&no=135
class PACKAGE TNumUnitProperty : public TPersistent
{
private:
    bool                        FVisible;
    AnsiString                  FUnit;        ///< 단위 표기 (단순 표기용)
    int                         FWidth;        ///< Unit이 표기되는 영역 넓이
    Vcl::Graphics::TFont    *   FFont;

    void     __fastcall SetVisible(bool b);
    void     __fastcall SetStr(AnsiString s);
    void     __fastcall SetWidth(int n);
    void     __fastcall SetFont(Vcl::Graphics::TFont * p);

    TNotifyEvent     FOnChange;              ///< 나를 가져다 쓸 class에서 여기에 Event를 등록할 것이다.
    void     __fastcall DoOnChange(void);      ///< 내 속성이 변경될 때 마다 이 함수를 호출하여 FOnChange에 등록된 Evnet가 있을 경우 호출 시켜 준다.
    void     __fastcall UnitFontChanged(System::TObject* Sender);  ///< Font의 속성이 변경된 것을 Call Back 받기 위한 Event 함수.

public:
            __fastcall  TNumUnitProperty();
  virtual     __fastcall ~TNumUnitProperty();

  __property TNotifyEvent OnChange = {read = FOnChange, write = FOnChange};
  void         __fastcall Assign(TPersistent* Source);

__published:

    // 내가 새로 만든 속성 -----------------------
    __property bool                     Visible      = { read = FVisible,  write = SetVisible,  stored = true, default = true  };
    __property AnsiString               Str          = { read = FUnit,     write = SetStr,      stored = true                  };
    __property int                      Width        = { read = FWidth,    write = SetWidth,    stored = true, default = 60    };
    __property Vcl::Graphics::TFont *   Font         = { read = FFont,     write = SetFont,     stored = true                  };

};








//---------------------------------------------------------------------------
//Input Number Box가 출력되는 Eidt Box를 위한 Property
class PACKAGE TAZEditBoxProperty : public TPersistent
{
private:
    TInputNumType FType;              ///< 정수형인지, 실수형인지 지정한다.
    double        FMin;               ///< 입력 값의 최소 범위.
    double        FMax;               ///< 입력 값의 최대 범위.
    double        FUpDownValue;       ///< 입력 Number Pad 상에서 Up/Down 버튼을 이용할 때 감소/증가 분.
    int           FDecimalPlaces;     ///< 소수점 자리 수 표기 (3일 경우 소수점 3자리까지 표기한다)
    int           FUpNumberOfDecimals;///< 고정 Digit 사용 시 정수 자리 수 표기 
    int           FDigitSize;         ///< 고정 Digit 사용 시 Digit 크기   
    int           FDigitGap;          ///< 고정 Digit 사용 시 Digit 간격 

    TNotifyEvent     FOnChange;              ///< 나를 가져다 쓸 class에서 여기에 Event를 등록할 것이다.
    void     __fastcall DoOnChange(void);      ///< 내 속성이 변경될 때 마다 이 함수를 호출하여 FOnChange에 등록된 Evnet가 있을 경우 호출 시켜 준다.

    void    __fastcall SetType(TInputNumType e);
    void    __fastcall SetDecimalPlaces(int n);
    void    __fastcall SetUpNumberOfDecimals(int n);
    void    __fastcall SetDigitSize(int n);
    void    __fastcall SetDigitGap(int n);
#if 0
    void    __fastcall SetMin(double d);
    void    __fastcall SetMax(double d);
    void    __fastcall SetUpDownValue(double d);
#else // 델파이/빌더 실수형 프로퍼티의 퍼시스턴트 메카니즘의 디자인 오류 해결.
    void    __fastcall SetMin(TWriter *writer);
    void    __fastcall SetMax(TWriter *writer);
    void    __fastcall SetUpDownValue(TWriter *writer);
#endif
    /// 델파이/빌더 실수형 프로퍼티의 퍼시스턴트 메카니즘의 디자인 오류 해결.
    virtual void __fastcall DefineProperties(TFiler *filer);

public:
            __fastcall  TAZEditBoxProperty();
  virtual   __fastcall ~TAZEditBoxProperty();

  __property TNotifyEvent OnChange = {read = FOnChange, write = FOnChange};
  void      __fastcall Assign(TPersistent* Source);

__published:

    __property TInputNumType    Type                = { read = FType,               write = SetType,                    default = itFloat};
    __property int              DecimalPlaces       = { read = FDecimalPlaces,      write = SetDecimalPlaces,           default = 3 };
    __property int              UpNumberOfDecimals  = { read = FUpNumberOfDecimals, write = SetUpNumberOfDecimals,      default = 6 };
    __property int              DigitSize           = { read = FDigitSize,          write = SetDigitSize,               default = 10 };
    __property int              DigitGap            = { read = FDigitGap,           write = SetDigitGap,                default = 1 };

#if 0
    __property double   Min         = { read = FMin,            write = SetMin,         default = -1000};
    __property double   Max         = { read = FMax,            write = SetMax,         default = 1000};
    __property double   UpDownValue = { read = FUpDownValue,    write = SetUpDownValue, default = 1};
#else // 델파이/빌더 실수형 프로퍼티의 퍼시스턴트 메카니즘의 디자인 오류 해결.
    __property double   Min         = { read = FMin,            write = FMin,           default = -1000};
    __property double   Max         = { read = FMax,            write = FMax,           default = 1000};
    __property double   UpDownValue = { read = FUpDownValue,    write = FUpDownValue,   default = 1};
#endif

};


#endif
