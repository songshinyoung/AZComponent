//---------------------------------------------------------------------------

#ifndef AZGroupBoxH
#define AZGroupBoxH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Buttons.hpp>


//---------------------------------------------------------------------------
enum DECLSPEC_DENUM TGBGlyphType : unsigned char { lbsDefault, lbsRectangle, lbsCustom };
enum DECLSPEC_DENUM TGBGlyphDefaultColor : unsigned char { gdtColorGreen, gltColorBlue, gltColorRed, gltColorBlack, gltColorGray, gltColorDisable };


class PACKAGE TAZGroupBox : public TCustomGroupBox
{
private:
    String                  FCaption;
    TGBGlyphDefaultColor    FGlyphDefaultStyle;
    int                     FFrameTopMargin;
    int                     FTitleMarginX;
    int                     FTitleMarginY;
    int                     FTitleWidth;
    int                     FTitleHeight;
    int                     FTextMarginX;
    
    bool                    FShadowFont;
    TColor                  FShadowFontColor;
    int                     FShadowFontLeft;
    int                     FShadowFontTop;

    TGBGlyphType            FGlyphType;
    System::Classes::TAlignment FAlignmentText;     // Caption의 Text의 얼라인
    System::Classes::TAlignment FAlignmentLabel;    // Caption 자체의 Align

    TColor                  FTitleBGColor;
    TColor                  FTitleRectColor;
    TColor                  FFrameColor;
    TColor                  FGradientStartColor;
    TColor                  FGradientEndColor;
    TColor                  FTransparentColor;

    int                     FFrameRound;
    int                     FTitleRound;
    bool                    FFrameLineBold;

    bool                    FTransparent;

    Vcl::Graphics::TBitmap*     FGlyph;
    Vcl::Buttons::TNumGlyphs    FNumGlyphs;

    bool                    m_bLoaded;
    TRect                   m_recTitle;
    bool                    m_bRefreshChildControl;     // 하위 컨트롤을 다시 그려줄 필요가 있는 경우.

    TImage *                m_pBGImage;

    System::Classes::TNotifyEvent FOnFontChangeOrg;

    void                    __fastcall SetGlyphDefaultStyle(TGBGlyphDefaultColor e) ;
    void                    __fastcall SetAlignmentText(System::Classes::TAlignment n) ;
    void                    __fastcall SetAlignmentLabel(System::Classes::TAlignment n) ;
    void                    __fastcall SetFrameTopMargin(int n) ;
    void                    __fastcall SetGlyphType(TGBGlyphType e) ;
    void                    __fastcall SetTitleAlignValue(int Index, int n) ;
    void                    __fastcall SetColorValue(int Index, TColor c) ;
    void                    __fastcall SetFrameValue(int Index, int n) ;
    void                    __fastcall SetFrameLineBold(bool b) ;
    void                    __fastcall SetCaption(String s);

    void                    __fastcall SetShadowFont(bool b);
    void                    __fastcall SetShadowFontColor(TColor c);
    void                    __fastcall SetShadowFontLeft(int n);
    void                    __fastcall SetShadowFontTop(int n);

    void                    __fastcall SetTransparent(bool b);
    
    void                    __fastcall SetGlyph(Vcl::Graphics::TBitmap* Value) ;
    Vcl::Graphics::TBitmap* __fastcall GetGlyph(void) ;
    void                    __fastcall SetNumGlyphs(TNumGlyphs Value) ;
    TNumGlyphs              __fastcall GetNumGlyphs(void) ;

    void __fastcall GlyphChanged(System::TObject* Sender);

    void __fastcall CalTitleRect(void);

    void __fastcall DisplayUpdate(void);

    void __fastcall Changed(void);
    void __fastcall FontChanged(System::TObject* Sender);

    void __fastcall MyResize(TObject *Sender);
    System::Classes::TNotifyEvent FOnResize;

protected:
    virtual void __fastcall Paint(void);
    virtual void __fastcall AdjustClientRect(System::Types::TRect &Rect);
    virtual void __fastcall Loaded(void);
    virtual void __fastcall SetEnabled(bool b);
    
public:
    __fastcall TAZGroupBox(TComponent* Owner);
    virtual __fastcall ~TAZGroupBox();


__published:
    //------------------------------------
    // 내가 작성한 속성     
    __property TGBGlyphDefaultColor         GlyphDefaultStyle   = { read = FGlyphDefaultStyle,  write = SetGlyphDefaultStyle,   default = gdtColorGreen     };
    __property int                          FrameTopMargin      = { read = FFrameTopMargin,     write = SetFrameTopMargin,      default = 4                 };
    __property Vcl::Graphics::TBitmap*      Glyph               = { read = GetGlyph,            write = SetGlyph,               stored=true                 };
    __property TNumGlyphs                   NumGlyphs           = { read = GetNumGlyphs,        write = SetNumGlyphs,           stored=true, default=3      };
    __property TGBGlyphType                 GlyphType           = { read = FGlyphType,          write = SetGlyphType,           default = lbsDefault        };
    __property System::Classes::TAlignment  AlignmentText       = { read = FAlignmentText,      write = SetAlignmentText,       default = taCenter          };
    __property System::Classes::TAlignment  AlignmentLabel      = { read = FAlignmentLabel,     write = SetAlignmentLabel,      default = taLeftJustify     };
    __property int                          TitleMarginX        = { read = FTitleMarginX,       write = SetTitleAlignValue,   index = 0, default = 10       };
    __property int                          TitleMarginY        = { read = FTitleMarginY,       write = SetTitleAlignValue,   index = 1, default = 0        };
    __property int                          TitleWidth          = { read = FTitleWidth,         write = SetTitleAlignValue,   index = 2, default = 150      };
    __property int                          TitleHeight         = { read = FTitleHeight,        write = SetTitleAlignValue,   index = 3, default = 29       };
    __property int                          TitleTextMarginX    = { read = FTextMarginX,        write = SetTitleAlignValue,   index = 4, default = 0        };
    __property int                          TitleRound          = { read = FTitleRound,         write = SetTitleAlignValue,   index = 5, default = 8        };

    __property TColor                       TitleBGColor        = { read = FTitleBGColor,       write = SetColorValue,        index = 0, default = clBtnFace  };
    __property TColor                       TitleRectColor      = { read = FTitleRectColor,     write = SetColorValue,        index = 1, default = clGray     };
    __property TColor                       FrameColor          = { read = FFrameColor,         write = SetColorValue,        index = 2, default = clGray     };
    __property TColor                       GradientStartColor  = { read = FGradientStartColor, write = SetColorValue,        index = 3, default = clWhite    };
    __property TColor                       GradientEndColor    = { read = FGradientEndColor,   write = SetColorValue,        index = 4, default = 0xDFE7E6   };
    __property TColor                       TransparentColor    = { read = FTransparentColor,   write = SetColorValue,        index = 5, default = clFuchsia  };
    
    __property int                          FrameRound          = { read = FFrameRound,         write = SetFrameValue,        index = 0, default = 10       };
    __property bool                         FrameLineBold       = { read = FFrameLineBold,      write = SetFrameLineBold,     default = false     };

    __property bool                         ShadowFont          = { read = FShadowFont,         write = SetShadowFont,        default = false     };
    __property TColor                       ShadowFontColor     = { read = FShadowFontColor,    write = SetShadowFontColor,   default = clBlack   };
    __property int                          ShadowFontColorLeft = { read = FShadowFontLeft,     write = SetShadowFontLeft,    default = -1        };
    __property int                          ShadowFontColorTop  = { read = FShadowFontTop,      write = SetShadowFontTop,     default = -1        };

    __property bool                         Transparent         = { read = FTransparent,        write = SetTransparent,       default = false     };
	__property String                       Caption             = { read = FCaption,            write = SetCaption,           default=0 };
    

    //----------------------------
    // EVENT
    __property System::Classes::TNotifyEvent OnResize = {read=FOnResize, write=FOnResize};

    //------------------------------------
    // GroupBox의 기본 속성 
	__property Align = {default=0};
	__property Anchors = {default=3};
	__property BiDiMode;
	__property Color = {default=-16777211};
	__property Constraints;
	__property Ctl3D;
	__property DockSite = {default=0};
	__property DoubleBuffered;
	__property DragCursor = {default=-12};
	__property DragKind = {default=0};
	__property DragMode = {default=0};
	__property Enabled = {default=1};
	__property Font;
	__property Padding;
	__property ParentBackground = {default=1};
	__property ParentBiDiMode = {default=1};
	__property ParentColor = {default=1};
	__property ParentCtl3D = {default=1};
	__property ParentDoubleBuffered = {default=1};
	__property ParentFont = {default=1};
	__property ParentShowHint = {default=1};
	__property PopupMenu;
	__property ShowHint;
	__property TabOrder = {default=-1};
	__property TabStop = {default=0};
	__property Touch;
	__property Visible = {default=1};
	__property StyleElements = {default=7};
	__property OnAlignInsertBefore;
	__property OnAlignPosition;
	__property OnClick;
	__property OnContextPopup;
	__property OnDblClick;
	__property OnDragDrop;
	__property OnDockDrop;
	__property OnDockOver;
	__property OnDragOver;
	__property OnEndDock;
	__property OnEndDrag;
	__property OnEnter;
	__property OnExit;
	__property OnGesture;
	__property OnGetSiteInfo;
	__property OnMouseActivate;
	__property OnMouseDown;
	__property OnMouseEnter;
	__property OnMouseLeave;
	__property OnMouseMove;
	__property OnMouseUp;
	__property OnStartDock;
	__property OnStartDrag;
	__property OnUnDock;
};
//---------------------------------------------------------------------------
#endif
