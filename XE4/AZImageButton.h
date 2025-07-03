//---------------------------------------------------------------------------

#ifndef AZImageButtonH
#define AZImageButtonH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>



//=============================================================================

class PACKAGE TImageBtnSubCaptionProperty : public TPersistent
{
private:    
    bool                        FVisible;
    String                      FCaption;
    bool                        FVAlign;    // true : Main Caption의 Top 에 위치. false : Main Caption의 Bottom에 위치.
    int                         FVSpace;    // Main Caption과 공간 설정. 
    int                         FHSpace;    // 좌우 공간.
    Vcl::Graphics::TFont    *   FFont;

    void     __fastcall SetVisible(bool b);
    void     __fastcall SetCaption(String s);
    void     __fastcall SetVAlign(bool b);
    void     __fastcall SetVSpace(int n);
    void     __fastcall SetHSpace(int n);
    void     __fastcall SetFont(Vcl::Graphics::TFont * p);
    

    TNotifyEvent        FOnChange;                                  ///< 나를 가져다 쓸 class에서 여기에 Event를 등록할 것이다.
    void     __fastcall DoOnChange(void);                          ///< 내 속성이 변경될 때 마다 이 함수를 호출하여 FOnChange에 등록된 Evnet가 있을 경우 호출 시켜 준다.
    void     __fastcall FontChanged(System::TObject* Sender);    ///< Font의 속성이 변경된 것을 Call Back 받기 위한 Event 함수.

public:
            __fastcall  TImageBtnSubCaptionProperty();
  virtual   __fastcall ~TImageBtnSubCaptionProperty();

  __property TNotifyEvent OnChange = {read = FOnChange, write = FOnChange};
  void      __fastcall Assign(TPersistent* Source);

__published:

    // 내가 새로 만든 속성 -----------------------
    __property bool                     Visible     = { read = FVisible,    write = SetVisible,   default = true    };
    __property String                   Caption     = { read = FCaption,    write = SetCaption,   stored = true, default = 0 };
    __property bool                     VAlignTop   = { read = FVAlign,     write = SetVAlign,    default = false   };
    __property int                      SpaceV      = { read = FVSpace,     write = SetVSpace,    default = 0       };
    __property int                      SpaceH      = { read = FHSpace,     write = SetHSpace,    default = 0       };
    __property Vcl::Graphics::TFont *   Font        = { read = FFont,       write = SetFont                         };

};

//=============================================================================

typedef enum EImageButtonStateType{
    IB_STATE_UP = 0,
    IB_STATE_DISABLE,
    IB_STATE_DOWN,
    IB_STATE_SELECT,
    IB_STATE_MAX
} EImageButtonState;


//=============================================================================

class PACKAGE TAZImageButton : public TImage
{
private:
    bool                        bLoaded;

    // Property value. --------------------------
    Vcl::Graphics::TBitmap*     FGlyph;
    Vcl::Buttons::TNumGlyphs    FNumGlyphs;
    String                      FCaption;
    TImageBtnSubCaptionProperty * FCaptionSub;
    System::Classes::TAlignment FAlignment;
    int                         FSpaceH;
    int                         FSpaceV;
    int                         FGroupIndex;
    bool                        FAllowAllUp;
    bool                        FDown;
    System::Classes::TNotifyEvent FOnResize;
    TColor                      FDefaultColor;
    TColor                      FDefaultLineColor;
    bool                        FDefaultLine;
    TColor                      FDefaultSelectColor;
    TColor                      FDefaultDownColor;

    bool                        FShadowFont;
    TColor                      FShadowFontColor;
    int                         FShadowFontLeft;
    int                         FShadowFontTop;
    bool                        FAutoToggle;

    void                    __fastcall SetCaption(String s);
    void                    __fastcall SetCaptionSub(TImageBtnSubCaptionProperty * p);
    void                    __fastcall SetGlyph(Vcl::Graphics::TBitmap* Value);
    Vcl::Graphics::TBitmap* __fastcall GetGlyph(void);
    void                    __fastcall SetNumGlyphs(Vcl::Buttons::TNumGlyphs Value);
    Vcl::Buttons::TNumGlyphs __fastcall GetNumGlyphs(void);
    void                    __fastcall SetAlignment(System::Classes::TAlignment Value);
    void                    __fastcall SetSpace(int Index, int Value );
    void                    __fastcall SetGroupIndex(int n);
    void                    __fastcall SetAllowAllUp(bool b);
    void                    __fastcall SetDown(bool b);
    void                    __fastcall SetDefaultColor(TColor c);
    void                    __fastcall SetDefaultLineColor(TColor c);
    void                    __fastcall SetDefaultLine(bool b);
    void                    __fastcall SetDefaultSelectColor(TColor c);
    void                    __fastcall SetDefaultDownColor(TColor c);

    void                    __fastcall SetShadowFont(bool b);
    void                    __fastcall SetShadowFontColor(TColor c);
    void                    __fastcall SetShadowFontLeft(int n);
    void                    __fastcall SetShadowFontTop(int n);
    void                    __fastcall SetAutoToggle(bool b);

    // Private value.----------------------------
    EImageButtonState   m_eButtonState;
    bool                m_bButtonDown;
    
    void        __fastcall CaptionSubChanged(System::TObject* Sender);
    void        __fastcall GlyphChanged(System::TObject* Sender);
    void        __fastcall OtherGroupButtonUp();

    System::Classes::TNotifyEvent FOnFontChangeOrg;
    
    void __fastcall CreateDefaultImage(void);
    void __fastcall CreateColorImage(Vcl::Graphics::TBitmap* pBitmap, TColor cColor, int nTopDownGap = 10, int nLineInterval = 1, int nCenterLine = 20);

    void         __fastcall MyResize(TObject *Sender);

    Vcl::Graphics::TBitmap* m_pDefaultImage;

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


    void __fastcall DisplayUpdate(EImageButtonState eState);

    void __fastcall FontChanged(System::TObject* Sender);

protected:
    virtual void    __fastcall WndProc(TMessage& Message);          ///< WndProc 함수 오버라이드.
    virtual void    __fastcall SetEnabled(bool Value);              ///< SetEnabled 함수 오버라이드.
    virtual void    __fastcall Loaded(void);                        ///< Loaded 함수 오버라이드.
    
public:
                    __fastcall TAZImageButton(TComponent* Owner);   ///< 생성자 
    virtual         __fastcall ~TAZImageButton();                   ///< 소멸자.
    virtual void    __fastcall Paint();                             ///< Paint 함수 오버라이드.

    void            __fastcall Up();
    
__published:
    __property String                       Caption     = { read = FCaption,        write = SetCaption                          };
    __property TImageBtnSubCaptionProperty* CaptionSub  = { read = FCaptionSub,     write = SetCaptionSub                       };

    __property System::Classes::TAlignment  Alignment   = { read = FAlignment,      write = SetAlignment,   default = taCenter  };
    __property int                          SpaceH      = { read = FSpaceH,         write = SetSpace,       index = 0           };
    __property int                          SpaceV      = { read = FSpaceV,         write = SetSpace,       index = 1           };
    __property int                          GroupIndex  = { read = FGroupIndex,     write = SetGroupIndex,  default = 0         };
    __property bool                         AllowAllUp  = { read = FAllowAllUp,     write = SetAllowAllUp,  default = false     };
    __property bool                         Down        = { read = FDown,           write = SetDown,        default = false     };

    __property TColor              DefaultColor         = { read = FDefaultColor,          write = SetDefaultColor,       default = (TColor)0xA53E1E     };
    __property TColor              DefaultSelectColor   = { read = FDefaultSelectColor,    write = SetDefaultSelectColor, default = (TColor)0xFF8000     };
    __property TColor              DefaultDownColor     = { read = FDefaultDownColor,      write = SetDefaultDownColor,   default = clAqua               };
    __property TColor              DefaultLineColor     = { read = FDefaultLineColor,      write = SetDefaultLineColor,   default = (TColor)0x952E0E     };
    __property bool                DefaultLineVisible   = { read = FDefaultLine,           write = SetDefaultLine,        default = true     };


    __property bool                ShadowFont           = { read = FShadowFont,           write = SetShadowFont,          default = false     };
    __property TColor              ShadowFontColor      = { read = FShadowFontColor,      write = SetShadowFontColor,     default = clBlack   };
    __property int                 ShadowFontColorLeft  = { read = FShadowFontLeft,       write = SetShadowFontLeft,      default = -1        };
    __property int                 ShadowFontColorTop   = { read = FShadowFontTop,        write = SetShadowFontTop,       default = -1        };
    __property bool                AutoToggle           = { read = FAutoToggle,           write = SetAutoToggle,          default = true      };



    __property Vcl::Graphics::TBitmap*      Glyph       = { read = GetGlyph,        write = SetGlyph                            };
    __property Vcl::Buttons::TNumGlyphs     NumGlyphs   = { read = GetNumGlyphs,    write = SetNumGlyphs,   default=4           };
    
    __property Font;

    __property System::Classes::TNotifyEvent    OnResize = {read=FOnResize, write=FOnResize};

    __property Picture = {stored = false};  // Picture에 Image를 그려 넣을 경우 dfm에 저장되지 않도록 한다.
    
};
//---------------------------------------------------------------------------
#endif
