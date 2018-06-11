//---------------------------------------------------------------------------

#ifndef AZUnitPropertyH
#define AZUnitPropertyH
//---------------------------------------------------------------------------

#include <sysutils.hpp>
#include <controls.hpp>
#include <classes.hpp>
#include "Common\\Define.h"

//---------------------------------------------------------------------------
//Unit property�� ���� class
// ���� : http://cbuilder.borlandforum.com/impboard/impboard.dll?action=read&db=bcb_tutorial&no=135
class PACKAGE TNumUnitProperty : public TPersistent
{
private:
    bool                        FVisible;
    AnsiString                  FUnit;        ///< ���� ǥ�� (�ܼ� ǥ���)
    int                         FWidth;        ///< Unit�� ǥ��Ǵ� ���� ����
    Vcl::Graphics::TFont    *   FFont;

    void     __fastcall SetVisible(bool b);
    void     __fastcall SetStr(AnsiString s);
    void     __fastcall SetWidth(int n);
    void     __fastcall SetFont(Vcl::Graphics::TFont * p);

    TNotifyEvent     FOnChange;              ///< ���� ������ �� class���� ���⿡ Event�� ����� ���̴�.
    void     __fastcall DoOnChange(void);      ///< �� �Ӽ��� ����� �� ���� �� �Լ��� ȣ���Ͽ� FOnChange�� ��ϵ� Evnet�� ���� ��� ȣ�� ���� �ش�.
    void     __fastcall UnitFontChanged(System::TObject* Sender);  ///< Font�� �Ӽ��� ����� ���� Call Back �ޱ� ���� Event �Լ�.

public:
            __fastcall  TNumUnitProperty();
  virtual     __fastcall ~TNumUnitProperty();

  __property TNotifyEvent OnChange = {read = FOnChange, write = FOnChange};
  void         __fastcall Assign(TPersistent* Source);

__published:

    // ���� ���� ���� �Ӽ� -----------------------
    __property bool                     Visible      = { read = FVisible,  write = SetVisible,  stored = true, default = true  };
    __property AnsiString               Str          = { read = FUnit,     write = SetStr,      stored = true                  };
    __property int                      Width        = { read = FWidth,    write = SetWidth,    stored = true, default = 60    };
    __property Vcl::Graphics::TFont *   Font         = { read = FFont,     write = SetFont,     stored = true                  };

};








//---------------------------------------------------------------------------
//Input Number Box�� ��µǴ� Eidt Box�� ���� Property
class PACKAGE TAZEditBoxProperty : public TPersistent
{
private:
    TInputNumType FType;              ///< ����������, �Ǽ������� �����Ѵ�.
    double        FMin;               ///< �Է� ���� �ּ� ����.
    double        FMax;               ///< �Է� ���� �ִ� ����.
    double        FUpDownValue;       ///< �Է� Number Pad �󿡼� Up/Down ��ư�� �̿��� �� ����/���� ��.
    int           FDecimalPlaces;     ///< �Ҽ��� �ڸ� �� ǥ�� (3�� ��� �Ҽ��� 3�ڸ����� ǥ���Ѵ�)
    int           FUpNumberOfDecimals;///< ���� Digit ��� �� ���� �ڸ� �� ǥ�� 
    int           FDigitSize;         ///< ���� Digit ��� �� Digit ũ��   
    int           FDigitGap;          ///< ���� Digit ��� �� Digit ���� 

    TNotifyEvent     FOnChange;              ///< ���� ������ �� class���� ���⿡ Event�� ����� ���̴�.
    void     __fastcall DoOnChange(void);      ///< �� �Ӽ��� ����� �� ���� �� �Լ��� ȣ���Ͽ� FOnChange�� ��ϵ� Evnet�� ���� ��� ȣ�� ���� �ش�.

    void    __fastcall SetType(TInputNumType e);
    void    __fastcall SetDecimalPlaces(int n);
    void    __fastcall SetUpNumberOfDecimals(int n);
    void    __fastcall SetDigitSize(int n);
    void    __fastcall SetDigitGap(int n);
#if 0
    void    __fastcall SetMin(double d);
    void    __fastcall SetMax(double d);
    void    __fastcall SetUpDownValue(double d);
#else // ������/���� �Ǽ��� ������Ƽ�� �۽ý���Ʈ ��ī������ ������ ���� �ذ�.
    void    __fastcall SetMin(TWriter *writer);
    void    __fastcall SetMax(TWriter *writer);
    void    __fastcall SetUpDownValue(TWriter *writer);
#endif
    /// ������/���� �Ǽ��� ������Ƽ�� �۽ý���Ʈ ��ī������ ������ ���� �ذ�.
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
#else // ������/���� �Ǽ��� ������Ƽ�� �۽ý���Ʈ ��ī������ ������ ���� �ذ�.
    __property double   Min         = { read = FMin,            write = FMin,           default = -1000};
    __property double   Max         = { read = FMax,            write = FMax,           default = 1000};
    __property double   UpDownValue = { read = FUpDownValue,    write = FUpDownValue,   default = 1};
#endif

};


#endif
