//---------------------------------------------------------------------------

#ifndef AZNumShapeH
#define AZNumShapeH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <sysutils.hpp>
#include <controls.hpp>
#include <classes.hpp>
#include <forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "AZUnitProperty.h"
#include "Common\\Define.h"



typedef enum {
	DIGIT_COLOR_DISALBE,
	DIGIT_COLOR_NORMAL,
	DIGIT_COLOR_CLICK,
	DIGIT_COLOR_CHANGE,
	DIGIT_COLOR_MAX
} eAZNumShapeDigitColorType;

//===========================================================================
// Color property를 위한 class
class PACKAGE TNumShapeColorProperty : public TPersistent
{
private:
    TColor          FClickBGColor;              ///< 해당 컴포넌트를 사용자가 터치한 경우 배경 색상
    TColor          FClickFontColor;            ///< 해당 컴포넌트를 사용자가 터치한 경우 폰트 색상
    TColor          FClickRectColor;            ///< 해당 컴포넌트를 사용자가 터치한 경우 바깥 사각형 테두리 색상.
    TColor          FBGColor;                   ///< 해당 컴포넌트의 일반 상태에서 배경 색상.
    TColor          FFontColor;                 ///< 해당 컴포넌트의 일반 상태에서 폰트 색상.
    TColor          FRectColor;                 ///< 바깥 사각형 테두리 색상.
    TColor          FOldValueColor;             ///< 이전 값 표기 폰트 색상.
    TColor          FChangedFontColor;          ///< 사용자가 값을 변경한 경우 폰트 색상.
    TColor          FFontShadowColor;           ///< 고정 Digit 사용 시 문자 그림자 색상. 

    void        __fastcall SetColor(int Index, TColor c);

    TNotifyEvent     FOnChange;                 ///< 나를 가져다 쓸 class에서 여기에 Event를 등록할 것이다.
    void        __fastcall DoOnChange(void);    ///< 내 속성이 변경될 때 마다 이 함수를 호출하여 FOnChange에 등록된 Evnet가 있을 경우 호출 시켜 준다.

public:
                __fastcall  TNumShapeColorProperty();
  virtual       __fastcall ~TNumShapeColorProperty();

  __property TNotifyEvent OnChange = {read = FOnChange, write = FOnChange};
  void          __fastcall Assign(TPersistent* Source);

__published:

    __property TColor ClickBGColor          = { read = FClickBGColor,           write = SetColor,     index = 1,   default = clAqua     };
    __property TColor ClickFontColor        = { read = FClickFontColor,         write = SetColor,     index = 2,   default = clBlack    };
    __property TColor ClickRectColor        = { read = FClickRectColor,         write = SetColor,     index = 3,   default = clBlue     };
    __property TColor BGColor               = { read = FBGColor,                write = SetColor,     index = 4,   default = clBlack    };
    __property TColor FontColor             = { read = FFontColor,              write = SetColor,     index = 5,   default = clAqua     };
    __property TColor RectColor             = { read = FRectColor,              write = SetColor,     index = 6,   default = clAqua     };
    __property TColor OldValueColor         = { read = FOldValueColor,          write = SetColor,     index = 7,   default = clYellow   };
    __property TColor ChangedFontColor      = { read = FChangedFontColor,       write = SetColor,     index = 8,   default = clRed      };
    __property TColor FontShadowColor       = { read = FFontShadowColor,        write = SetColor,     index = 9,   default = (TColor)0x0505050};

};

//------------------------------------------------------------------------------
class PACKAGE TNumShapeTitleProperty : public TPersistent
{
private:
    bool                        FVisible;       ///< Title 전체 보이기/안보이기.
    UnicodeString               FCaption;       ///< 단위 표기 (단순 표기용)
    int                         FWidth;         ///< Unit이 표기되는 영역 넓이
    Vcl::Graphics::TFont    *   FFont;          ///< 폰트 특성
    System::Classes::TAlignment FAlignment;     ///< Caption 글자의 위치 (Left, Center, Right)
    TCaptionAlign               FAlign;         ///< Edit Box를 기준으로 Caption이 위치하는 자리.
    TColor                      FColor;         ///< 배경색
    bool                        FTransparent;   ///< 배경색 투명
    bool                        FOutLineVisible;///< Out Line 보이기
    TColor                      FOutLineColor;  ///< Out Line Color
    int                         FRound;         ///< Round
    int                         FSpace;         ///< Title과 Unit 사이의 공간
    int                         FMargin;        ///< Title Caption의 Align 조건에 따른 공백 여백 추가. 

    void    __fastcall  SetVisible(bool b);
    void    __fastcall  SetCaption(UnicodeString s);
    void    __fastcall  SetWidth(int n);
    void    __fastcall  SetFont(Vcl::Graphics::TFont * p);
    void    __fastcall  SetAlignment(System::Classes::TAlignment a);
    void    __fastcall  SetColor(TColor c);
    void    __fastcall  SetTransparent(bool b);
    void    __fastcall  SetOutLineVisible(bool b);
    void    __fastcall  SetOutLineColor(TColor c);
    void    __fastcall  SetRound(int n);
    void    __fastcall  SetSpace(int n);
    void    __fastcall  SetAlign(TCaptionAlign a);
    void    __fastcall  SetMargin(int n);

    TNotifyEvent        FOnChange;                                  ///< 나를 가져다 쓸 class에서 여기에 Event를 등록할 것이다.
    void    __fastcall  DoOnChange(void);                           ///< 내 속성이 변경될 때 마다 이 함수를 호출하여 FOnChange에 등록된 Evnet가 있을 경우 호출 시켜 준다.
    void    __fastcall  FontChanged(System::TObject* Sender);   ///< Font의 속성이 변경된 것을 Call Back 받기 위한 Event 함수.

public:
            __fastcall  TNumShapeTitleProperty();
  virtual   __fastcall ~TNumShapeTitleProperty();

  __property TNotifyEvent OnChange = {read = FOnChange, write = FOnChange};
  void      __fastcall  Assign(TPersistent* Source);

__published:

    // 내가 새로 만든 속성 -----------------------
    __property bool                         Visible     = { read = FVisible,    write = SetVisible,  stored = true, default = false     };
    __property UnicodeString                Caption     = { read = FCaption,    write = SetCaption,  stored = true };
    __property int                          Width       = { read = FWidth,      write = SetWidth,    stored = true, default = 100       };
    __property Vcl::Graphics::TFont *       Font        = { read = FFont,       write = SetFont,     stored = true                      };
    __property System::Classes::TAlignment  Alignment   = { read = FAlignment,  write = SetAlignment,stored = true, default = taCenter  };
    __property TCaptionAlign                Align       = { read = FAlign,      write = SetAlign,                   default = caLeft };
    __property TColor                       Color       = { read = FColor,      write = SetColor,    stored = true, default = clBtnFace };
    __property bool                         Transparent = { read = FTransparent,write = SetTransparent, stored = true, default = true   };
    __property bool                         OutLineVisible = { read = FOutLineVisible, write = SetOutLineVisible, default = true };
    __property TColor                       OutLineColor   = { read = FOutLineColor,   write = SetOutLineColor,   default = clBlack };
    __property int                          Round       = { read = FRound,      write = SetRound,    default = 5 };
    __property int                          Space       = { read = FSpace,      write = SetSpace,    default = 0 };
    __property int                          Margin      = { read = FMargin,     write = SetMargin,   default = 0 };
};
//===========================================================================



//===========================================================================
/**
* @brief : OnClick Event에 bool Ok 인자를 추가한 Event로 새로 생성한다.
           TPanel의 기존 OnClick Event는 MyClick() 함수가 등록되고
           MyClick() 함수 내에서 FOnClick Event Property에 사용자가 등록한
           Event 함수가 있을 경우 Numer Pad 실행 결과에 따라
           bool Ok 인자에 해당 결과를 같이 넘겨주면서 실행 시켜준다.
* @param : System::TObject* Sender : 컴포넌트 포인터
* @param : bool Ok : Number Pad의 입력을 Ok로 종료한 경우 true,
                     Cancel로 종료한 경우 false를 넣어준다.
* @return void :
* @see  TNumPanelClickEvent FOnClick
* @see  void     __fastcall MyClick(TObject *Sender)
- history : 1. [2014년 6월 17일 화요일][송신영] 최초 작성
**/
typedef void __fastcall (__closure *TNumShapeClickEvent)(System::TObject* Sender, bool Ok);

//---------------------------------------------------------------------------

class PACKAGE TAZNumShape : public TGraphicControl
{
private:
    //----------------------------------
    // Label Property
//    TBoundLabel *     FLabel;
//    TLabelPosition     FLabelPosition;
//    int             FLabelSpacing;
//    void __fastcall SetLabelPosition(const TLabelPosition Value);
//    void __fastcall SetLabelSpacing(const int Value);

    //----------------------------------
    System::Classes::TAlignment FAlignment;

    double        FValue;               ///< 현재 값.
    double        FValueOld;            ///< 변경 전 값.

//    TInputNumType FType;                  ///< 정수형인지, 실수형인지 지정한다.
//    double        FMin;                   ///< 입력 값의 최소 범위.
//    double        FMax;                   ///< 입력 값의 최대 범위.
//    double        FUpDownValue;           ///< 입력 Number Pad 상에서 Up/Down 버튼을 이용할 때 감소/증가 분.
//    int           FDecimalPlaces;         ///< 소수점 자리수 표기 (3일 경우 소수점 3자리까지 표기한다)

    int           FRound;                   ///< 모서리 Round 정도.
    bool          FOldValueDisplay;         ///< 이전 값 다시 보기 기능 사용.
    bool          FOldValueUnderline;       ///< true이면 Old Value 값 표기시 Under line이 표기됨. 
    int           FOldValueFontSize;        ///< Old Value Font Size를 결정함. (0일 경우 Defualt로 기본 Font Size의 1/2 값이 적용됨)
    int           FSpace;                   ///< Value와 외곽 사이의 공간. (Alignment가 Right인 경우 오른쪽 공간, Left인 경우 왼쪽 공간에 적용된다.)
    int           FEditWidth;               ///< Title을 사용할 경우 Edit가 차지하는 크기를 설정. Title을 사용하지 않으면 의미 없는 값이다. 또한 FAlign 값이 Left, Right 일 경우만 사용된다. 
    int           FEditHeight;              ///< Title을 사용할 경우 Edit가 차지하는 높이 설정. Title을 사용하지 않으면 의미 없다. 또한 FAlign 값이 Top,Bottom일 경우만 사용 된다.
    TNumUnitProperty        * FUnit;        ///< Unit  관련 Property
    TNumShapeColorProperty  * FColorSet;    ///< Color 관련 Property
    TAZEditBoxProperty      * FInputEdit;   ///< Input Number Pad Property
    TNumShapeTitleProperty  * FTitle;       ///< Title 관련 Property
    bool            FReadOnly;              ///< 읽기 전용으로 사용
	bool            FNoEdit;              	///< InputEdit 사용 안함
    UnicodeString   FPath;                  ///< Ini File Path;
    bool            FInvalidateUse;         ///< AZ_Value등의 값이 변경되어 다시 그려야할 경우 Invalidate() 를 사용할지 아니면 바로 Paint()함수를 호출할지 경정한다. true이면 바로 Paint()함수를 호출한다. (깜빡임 해결)

    bool            FFixedDigitUse;         ///< Font로 그리지 않고 고정된 Digit 그림을 사용. 

    void    __fastcall SetUnit(TNumUnitProperty    *  p);
    void    __fastcall SetColorSet(TNumShapeColorProperty * p);
    void    __fastcall SetTitle(TNumShapeTitleProperty * p);

    void    __fastcall SetAlignment(System::Classes::TAlignment Value);
    void    __fastcall SetValueOld(double d);
    void    __fastcall SetValue(double d);
    void    __fastcall SetRound(int n);

    void    __fastcall SetInputEdit(TAZEditBoxProperty * p);

    void    __fastcall SetReadOnly(bool b);

	void    __fastcall SetNoEdit(bool b);

    void    __fastcall SetInvalidateUse(bool b);
    void    __fastcall SetEditWidth(int n);
    void    __fastcall SetEditHeight(int n);

    void    __fastcall SetFixedDigit(bool b);

//    void    __fastcall SetType(TInputNumType e);
//    void    __fastcall SetDecimalPlaces(int n);
//#if 0
//    void    __fastcall SetMin(double d);
//    void    __fastcall SetMax(double d);
//    void    __fastcall SetUpDownValue(double d);
//#else // 델파이/빌더 실수형 프로퍼티의 퍼시스턴트 메카니즘의 디자인 오류 해결.
//    void    __fastcall SetMin(TWriter *writer);
//    void    __fastcall SetMax(TWriter *writer);
//    void    __fastcall SetUpDownValue(TWriter *writer);
//#endif


    //void    __fastcall SetColor(int Index, TColor c);
    void    __fastcall SetOldValueDisplay(bool b);
    void    __fastcall SetOlfValueUnderline(bool b);
    void    __fastcall SetOlfValueFontSize(int n);

    void    __fastcall SetSpace(int n);

    void    __fastcall SetPath(UnicodeString s);

    TNumShapeClickEvent FOnClick;
    void    __fastcall MyClick(TObject *Sender);


    bool   bLoaded;        ///< dfm 파일로 부터 저장된 내용을 모두 로딩 된 상태.
    bool   bSelecting;     ///< 사용자가 터치한 상태.
    bool   bNumbering;     ///< 현재 값을 입력중인 상태
    bool   bChanged;       ///< 사용자가 FValue를 변경한 경우 확인.
    bool   bBeginUpdate;   ///< Update 시작 알림 플래그, 이변수가 true이면 화면을 그리지 않는다. 

    void    __fastcall UnitChanged(System::TObject* Sender);
    void    __fastcall ColorChanged(System::TObject* Sender);
    void    __fastcall InputEditChanged(System::TObject* Sender);
    void    __fastcall TitleChanged(System::TObject* Sender);


    /// 델파이/빌더 실수형 프로퍼티의 퍼시스턴트 메카니즘의 디자인 오류 해결.
    virtual void __fastcall DefineProperties(TFiler *filer);


    Graphics::TBitmap* tmpBitmap1;
    Graphics::TBitmap* tmpBitmap2;
    Graphics::TBitmap* pBitmapTemp;

    int     m_nMouseDownXPos;
    int     m_nMouseDownYPos;

    int     m_nTitleWidth;      // Title 이 차지하는 전체 크기
    int     m_nTitleHeight;     // Title 이 차지하는 전체 크기

    bool            m_bEncoderButtonEnable;
    TAZNumShape *   m_pEncoderAZNumShape;
    AnsiString      m_sEncoderTitle;
    AnsiString      m_sEncoderUnit;


private:
    void __fastcall DrawImageTrans(    Graphics::TBitmap* pDest,
                                    int X,
                                    int Y,
                                    Graphics::TBitmap* srcBmp);

    void __fastcall DrawTextTrans(  Graphics::TBitmap* pDest,
                                    Graphics::TBitmap* pTextBitmap,
                                    int             X,
                                    int             Y,
                                    AnsiString      sString,
                                    TColor          cColor,
                                    unsigned int    Align,
                                    int             nFontSize,
                                    bool            bBold);

    bool            __fastcall InputNumber(TObject *Sender);
    //double          __fastcall DecimalRounding(double dSrc, int nPoint);
    //AnsiString      __fastcall GetDrawValue(double dValue);
    void            __fastcall ResetOthersComponent();


    //-----------------------------------------------------------
	void __fastcall DrawValue2(	double 	Value,
								int 	XPos,
								int 	YPos,
								int 	nGap,
								TInputNumType eType,
								int 	nUpNumberOfDecimals,
								int 	nDownNumberOfDecimals,
								TColor 	FColor,
                                TColor  ShadowColor,
								TColor 	BColor,
								int 	size,
								bool    bEnabled,
								Graphics::TBitmap* pBitMap);

    double      __fastcall GetDigitHeight(int size);
    int         __fastcall GetDotWidth(int size);


	void __fastcall DrawOneDot2(int nX, int nY, TColor FColor, TColor BGColor, int size, Graphics::TBitmap* pBitMap);
	void __fastcall DrawOneDigit2(unsigned char ch, int nX, int nY, TColor FColor, TColor ShadowColor, TColor BGColor, int size, Graphics::TBitmap* pBitMap);
    void __fastcall DrawTop(int nX, int nY, int size, Graphics::TBitmap* pBitMap, double dW, double dH);
    void __fastcall DrawTopLeft(int nX, int nY, int size, Graphics::TBitmap* pBitMap, double dW, double dH);
    void __fastcall DrawBottomLeft(int nX, int nY, int size, Graphics::TBitmap* pBitMap, double dW, double dH);
    void __fastcall DrawBottom(int nX, int nY, int size, Graphics::TBitmap* pBitMap, double dW, double dH);
    void __fastcall DrawBottomRight(int nX, int nY, int size, Graphics::TBitmap* pBitMap, double dW, double dH);
    void __fastcall DrawTopRight(int nX, int nY, int size, Graphics::TBitmap* pBitMap, double dW, double dH);
    void __fastcall DrawCenter(int nX, int nY, int size, Graphics::TBitmap* pBitMap, double dW, double dH);
    //-----------------------------------------------------------

    

protected:
    void            __fastcall WndProc(TMessage& Message);  ///< WndProc 함수 오버라이드.
    virtual void    __fastcall SetEnabled(bool Value);        ///< SetEnabled 함수 오버라이드.
    void            __fastcall Loaded(void);                ///< Loaded 함수 오버라이드.

public:
                    __fastcall TAZNumShape(TComponent* Owner);      ///< 생성자.
    virtual         __fastcall ~TAZNumShape();                         ///< 소멸자.

    
    void            __fastcall Paint();                             ///< Paint 함수 오버라이드.
    void            __fastcall PaintTitle();
    void            __fastcall PaintValue();

    void            __fastcall ResetChanged();
    bool            __fastcall IsChanged();

    void            __fastcall BeginUpdate();
    void            __fastcall EndUpdate();

	void            __fastcall SetEncoderButton(bool bEnable, AnsiString sTitle, AnsiString unit, TAZNumShape * pAZNumShape);  // , TAZNumShape *pAZNumShape, , AnsiString sUnit

__published:

	// 속성 -----------------------
    __property TNumUnitProperty       * AZ_Unit     = { read = FUnit,       write = SetUnit };
    __property TNumShapeColorProperty * ColorSet    = { read = FColorSet,   write = SetColorSet };
	__property TAZEditBoxProperty     * InputEdit   = { read = FInputEdit,  write = SetInputEdit };
    __property TNumShapeTitleProperty * Title       = { read = FTitle,      write = SetTitle    };

    __property double               AZ_ValueOld     = { read = FValueOld,   write = SetValueOld, stored = false, nodefault};
	__property double               AZ_Value        = { read = FValue,      write = SetValue,   default = 0};
    __property int                  AZ_Round        = { read = FRound,      write = SetRound,   default = 5};

    __property bool                 ReadOnly        = { read = FReadOnly,   write=SetReadOnly,  default = false };

	__property bool                 NoEdit        	= { read = FNoEdit,   	write=SetNoEdit,  default = false };

//    __property TInputNumType        AZ_Type             = { read = FType,               write = SetType,                 default = itFloat};
//    __property int                  AZ_DecimalPlaces    = { read = FDecimalPlaces,      write = SetDecimalPlaces,     default = 3 };
//#if 0
//    __property double               AZ_Min              = { read = FMin,               write = SetMin,                 default = -1000};
//    __property double               AZ_Max              = { read = FMax,               write = SetMax,                 default = 1000};
//    __property double               AZ_UpDownValue      = { read = FUpDownValue,       write = SetUpDownValue,         default = 1};
//#else // 델파이/빌더 실수형 프로퍼티의 퍼시스턴트 메카니즘의 디자인 오류 해결.
//    __property double               AZ_Min              = { read = FMin,               write = FMin,                 default = -1000};
//    __property double               AZ_Max              = { read = FMax,               write = FMax,                 default = 1000};
//    __property double               AZ_UpDownValue      = { read = FUpDownValue,       write = FUpDownValue,         default = 1};
//#endif


    __property int                  AZ_Space            = { read = FSpace,              write = SetSpace,             default = 0 };
    __property bool                 AZ_OldValueDisplay  = { read = FOldValueDisplay,    write = SetOldValueDisplay,   default = true };
    __property bool                 AZ_OldValueUnderline= { read = FOldValueUnderline,  write = SetOlfValueUnderline, default = true };
    __property int                  AZ_OldValueFontSize = { read = FOldValueFontSize ,  write = SetOlfValueFontSize,  default = 0 };

    __property UnicodeString        AZ_Path             = { read = FPath,               write = SetPath };
    __property System::Classes::TAlignment Alignment    = { read = FAlignment,          write = SetAlignment,         default = taRightJustify };
    __property bool                 InvalidateUs        = { read = FInvalidateUse,      write = SetInvalidateUse,     default = false };
    __property int                  EditWidth           = { read = FEditWidth,          write = SetEditWidth,         default = 100   };
    __property int                  EditHeight          = { read = FEditHeight,         write = SetEditHeight,        default = 50    };
    __property bool                 FixedDigit          = { read = FFixedDigitUse,      write = SetFixedDigit,        default = false };

    // 재 지정한 Event ---------------------------
    __property TNumShapeClickEvent OnClick = {read = FOnClick, write = FOnClick};


    // 상속받은 속성 사용 ------------------------
    __property Font;
    //__property Caption = {nodefault};
    __property Color;
    __property Enabled = {/*write = SetEnabled, */default = 1};
    __property Tag;
    __property Align;
    __property Left;
    __property Top;
    __property Width;
    __property Height;
    __property Visible;

    __property DragCursor ;
    __property DragMode ;
    __property OnDragDrop ;
    __property OnDragOver ;
    __property OnEndDrag ;
    __property OnMouseDown ;
    __property OnMouseMove ;
    __property OnMouseUp ;


};
//-----------------------------------------------------------------------

#endif
