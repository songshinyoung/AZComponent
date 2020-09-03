//---------------------------------------------------------------------------

#ifndef AZOnOffButtonH
#define AZOnOffButtonH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <pngimage.hpp>

#include "Define.h"

//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TOnOffBtnClickEvent)(System::TObject* Sender, bool bOn);
typedef void __fastcall (__closure *TOnOffBtnMouseUpEvent)(System::TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, bool bOn);
typedef void __fastcall (__closure *TOnOffBtnMouseDownEvent)(System::TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, bool bOn);

//---------------------------------------------------------------------------

class PACKAGE TAZOnOffButton : public TImage
{
private:
    bool                        bLoaded;

    bool                        FOn;
    int                         FButtonStyle;           // 버튼 종류 선택 
    Vcl::Graphics::TBitmap*     FGlyph;                 // 사용자 이미지 등록 
    Vcl::Buttons::TNumGlyphs    FNumGlyphs;
    int                         FGroupIndex;
    bool                        FAllowAllUp;
    bool                        FSmoothResize;      
    bool                        FConstProportions;      // 확대 축소시 Width - Hieght 비율제한
	bool                        FAutoToggle;

	TCaption            		FCaption;
	TCaption            		FCaptionOn;
	TCaption            		FCaptionOff;
    
    System::Classes::TNotifyEvent FOnResize;

    void                    __fastcall SetOn(bool b);
    void                    __fastcall SetButtonStyle(int n);
    
    void                    __fastcall SetGlyph(Vcl::Graphics::TBitmap* Value);
    Vcl::Graphics::TBitmap* __fastcall GetGlyph(void);
    void                    __fastcall SetNumGlyphs(Vcl::Buttons::TNumGlyphs Value);
    Vcl::Buttons::TNumGlyphs __fastcall GetNumGlyphs(void);
    void                    __fastcall SetGroupIndex(int n);
    void                    __fastcall SetAllowAllUp(bool b);
    void                    __fastcall SetSmoothResize(bool b);
    void                    __fastcall SetStretch(bool b);                  ///< SetStretch 함수는 오버라이드가 안되어 속성을 다시 만드는 방식 사용 
    void                    __fastcall SetConstProportions(bool b);
	void                    __fastcall SetAutoToggle(bool b);

	void                	__fastcall SetCaption(TCaption s);
	void                	__fastcall SetCaptionOn(TCaption s);
	void                	__fastcall SetCaptionOff(TCaption s);

	bool                        m_bButtonDown;
	TPngImage *                 m_pPNGBtnImage[ofBtnMax]; // Image Button PNG Image 저장 (On, Off, On-Disable, Off-Disalbe)
    Graphics::TBitmap*          m_pBMPBtnImage[ofBtnMax];
    bool                        m_bPNGImageExist[ofBtnMax];

    void            __fastcall  CreateDefaultImage(void);
    void            __fastcall  MyResize(TObject *Sender);  
    void            __fastcall  DisplayUpdate();
    void            __fastcall  DisplayUpdate_PNG();
    void            __fastcall  DisplayUpdate_BMP();
    void            __fastcall  SmoothResizePNG(TPngImage *apng, int NuWidth, int NuHeight);

    TOnOffBtnClickEvent         FOnClick;
    TOnOffBtnMouseDownEvent     FOnMouseDown;
    TOnOffBtnMouseUpEvent       FOnMouseUp;
    
    void            __fastcall  MyClick(TObject *Sender);
    void            __fastcall  MyMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void            __fastcall  MyMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

    void            __fastcall  GlyphChanged(System::TObject* Sender);
    void            __fastcall  OtherGroupButtonUp();

    
protected:
    virtual void    __fastcall  WndProc(TMessage& Message);          ///< WndProc 함수 오버라이드.
    virtual void    __fastcall  SetEnabled(bool Value);              ///< SetEnabled 함수 오버라이드.
    virtual void    __fastcall  SetAutoSize(bool b);                 ///< SetAutoSize 함수 오버라이드.   

    virtual void    __fastcall  Loaded(void);                        ///< Loaded 함수 오버라이드.
public:
                    __fastcall  TAZOnOffButton(TComponent* Owner);
                    __fastcall ~TAZOnOffButton();
    virtual void    __fastcall  Paint();                             ///< Paint 함수 오버라이드.

__published:
    __property bool                     On              = { read = FOn,                 write = SetOn,                          default = false     };
    __property int                      ButtonStyle     = { read = FButtonStyle,        write = SetButtonStyle,                 default = 0         };
    __property Vcl::Graphics::TBitmap*  Glyph           = { read = GetGlyph,            write = SetGlyph                                };
    __property Vcl::Buttons::TNumGlyphs NumGlyphs       = { read = GetNumGlyphs,        write = SetNumGlyphs,                   default = 4         };
    __property int                      GroupIndex      = { read = FGroupIndex,         write = SetGroupIndex,                  default = 0         };
    __property bool                     AllowAllUp      = { read = FAllowAllUp,         write = SetAllowAllUp,                  default = false     };
    __property bool                     SmoothResize    = { read = FSmoothResize,       write = SetSmoothResize,                default = false     };
    __property bool                     ConstProportions= { read = FConstProportions,   write = SetConstProportions,            default = false     };
    __property bool                     AutoToggle      = { read = FAutoToggle,         write = SetAutoToggle,                  default = true      };

	__property TCaption         		Caption  		= { read = FCaption,   	write = SetCaption };
	__property TCaption         		CaptionOn     	= { read = FCaptionOn,	write = SetCaptionOn };
	__property TCaption         		CaptionOff    	= { read = FCaptionOff,	write = SetCaptionOff };


    // 재 정의 Property---------------------------
    //__property                          AutoSize    = { default = true };
    __property                          Width       = { stored = true  };
    __property                          Height      = { stored = true  };
    __property                          Picture     = { stored = false };  // Picture에 Image를 그려 넣을 경우 dfm에 저장되지 않도록 한다.
    __property                          Stretch     = { write = SetStretch, default = false, stored = true };



    // 재 지정한 Event ---------------------------
    __property TOnOffBtnClickEvent      OnClick     = { read = FOnClick,     write = FOnClick        };
    __property TOnOffBtnMouseDownEvent  OnMouseDown = { read = FOnMouseDown, write = FOnMouseDown    };
    __property TOnOffBtnMouseUpEvent    OnMouseUp   = { read = FOnMouseUp,   write = FOnMouseUp      };

    __property System::Classes::TNotifyEvent    OnResize = {read=FOnResize, write=FOnResize};


};
//---------------------------------------------------------------------------
#endif
