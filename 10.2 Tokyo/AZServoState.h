//---------------------------------------------------------------------------

#ifndef AZServoStateH
#define AZServoStateH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Vcl.Controls.hpp>
//---------------------------------------------------------------------------
enum {
    _AZ_SERVO_STATE_SERVOON,
    _AZ_SERVO_STATE_HOME,
    _AZ_SERVO_STATE_ALARM,
    _AZ_SERVO_STATE_PLIMIT,
    _AZ_SERVO_STATE_NLIMIT,
    _AZ_SERVO_STATE_MAX
};
//---------------------------------------------------------------------------
class PACKAGE TAZServoStateIndicatorProperty : public TPersistent
{
private:
    
    int     nID;

    bool    FState;
    TColor  FColorOn;
    TColor  FColorOff;
    TColor  FFontColorOn;
    TColor  FFontColorOff;

    TNotifyEvent FOnChange;

    void        __fastcall SetState(bool b);
    void        __fastcall SetColor(int Index, TColor c);
    TColor      __fastcall GetColor(int Index);
    void        __fastcall DoOnChange(void);    ///< 내 속성이 변경될 때 마다 이 함수를 호출하여 FOnChange에 등록된 Evnet가 있을 경우 호출 시켜 준다.

public:
                __fastcall  TAZServoStateIndicatorProperty(int id);
  virtual       __fastcall ~TAZServoStateIndicatorProperty();

  __property TNotifyEvent OnChange = {read = FOnChange, write = FOnChange};
  void          __fastcall Assign(TPersistent* Source);
  int           __fastcall GetID();

__published:

    __property bool     StateOn         = { read = FState, write = SetState, default = false };
    __property TColor   ColorOn         = { read = GetColor, write = SetColor, index = 0, default = clLime  };
    __property TColor   ColorOff        = { read = GetColor, write = SetColor, index = 1, default = clBlack };
    __property TColor   FontColorOn     = { read = GetColor, write = SetColor, index = 2, default = clWhite };
    __property TColor   FontColorOff    = { read = GetColor, write = SetColor, index = 3, default = clGray  };


};

//---------------------------------------------------------------------------
class PACKAGE TAZServoState : public TGraphicControl
{
private:
    bool    bLoaded;
    int     FIndex;
    bool    FIndexVisible;
    bool    FCaptionVisible;
    TColor  FIndicatorRectColor;
    TColor  FOutRectColor;
    TAZServoStateIndicatorProperty  * FIndicator[_AZ_SERVO_STATE_MAX];
    Vcl::Graphics::TFont    *   FFontIndicator;

    void    __fastcall SetIndex(int n);
    void    __fastcall SetIndicatorRectColor(TColor c);
    void    __fastcall SetOutRectColor(TColor c);
    void    __fastcall SetIndexVisible(bool b);
    void    __fastcall SetCaptionVisible(bool b);
    void    __fastcall SetIndicator(int Index, TAZServoStateIndicatorProperty * p);
    TAZServoStateIndicatorProperty * __fastcall GetIndicator(int Index);

    void    __fastcall SetFontIndicator(Vcl::Graphics::TFont    * f);
    void    __fastcall IndicatorFontChanged(System::TObject* Sender);

    void    __fastcall IndicatorChanged(System::TObject* Sender);

    //-----------------------------------
    // 내부 변수.
    Graphics::TBitmap* tmpBitmap1;

    void  __fastcall DrawIndicator(int nID, Vcl::Graphics::TCanvas* pCanvas);

protected:
    void            __fastcall Loaded(void);                ///< Loaded 함수 오버라이드.

public:
    __fastcall TAZServoState(TComponent* Owner);
    virtual __fastcall ~TAZServoState();

    void            __fastcall Paint();                     ///< Paint 함수 오버라이드.

__published:
    // 내가 만든 속성 ----------------------------
    __property int  Index = { read = FIndex, write = SetIndex, default = 0 };
    __property TAZServoStateIndicatorProperty * ID_Servo  = { read = GetIndicator, write = SetIndicator, index =  _AZ_SERVO_STATE_SERVOON};
    __property TAZServoStateIndicatorProperty * ID_Home   = { read = GetIndicator, write = SetIndicator, index =  _AZ_SERVO_STATE_HOME};
    __property TAZServoStateIndicatorProperty * ID_Alarm  = { read = GetIndicator, write = SetIndicator, index =  _AZ_SERVO_STATE_ALARM};
    __property TAZServoStateIndicatorProperty * ID_PLimit = { read = GetIndicator, write = SetIndicator, index =  _AZ_SERVO_STATE_PLIMIT};
    __property TAZServoStateIndicatorProperty * ID_NLimit = { read = GetIndicator, write = SetIndicator, index =  _AZ_SERVO_STATE_NLIMIT};
    __property Vcl::Graphics::TFont    *        Font_Indicator = { read = FFontIndicator, write = SetFontIndicator };
    __property TColor   IndicatorRectColor      = { read = FIndicatorRectColor, write = SetIndicatorRectColor, default = clWhite };
    __property TColor   OutRectColor            = { read = FOutRectColor,   write = SetOutRectColor,   default = clBlack };
    __property bool     IndexVisible            = { read = FIndexVisible,   write = SetIndexVisible,   default = true };
    __property bool     CaptionVisible          = { read = FCaptionVisible, write = SetCaptionVisible, default = true };


    // 상속받은 속성 사용 ------------------------
    __property Font;
    __property Caption = {nodefault};
    __property Color   = {default = clGray };
    __property Enabled = {/*write = SetEnabled, */default = 1};
    __property Tag;
    __property Align;
    __property Left;
    __property Top;
    __property Width    = {default = 110 };
    __property Height   = {default = 60  };
    __property Visible  = {default = true };

    __property OnMouseDown ;
    __property OnMouseMove ;
    __property OnMouseUp ;
    __property OnClick;
    __property OnDblClick;

};
//---------------------------------------------------------------------------
#endif
