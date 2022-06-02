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
// Color property�� ���� class
class PACKAGE TNumShapeColorProperty : public TPersistent
{
private:
    TColor          FClickBGColor;              ///< �ش� ������Ʈ�� ����ڰ� ��ġ�� ��� ��� ����
    TColor          FClickFontColor;            ///< �ش� ������Ʈ�� ����ڰ� ��ġ�� ��� ��Ʈ ����
    TColor          FClickRectColor;            ///< �ش� ������Ʈ�� ����ڰ� ��ġ�� ��� �ٱ� �簢�� �׵θ� ����.
    TColor          FBGColor;                   ///< �ش� ������Ʈ�� �Ϲ� ���¿��� ��� ����.
    TColor          FFontColor;                 ///< �ش� ������Ʈ�� �Ϲ� ���¿��� ��Ʈ ����.
    TColor          FRectColor;                 ///< �ٱ� �簢�� �׵θ� ����.
    TColor          FOldValueColor;             ///< ���� �� ǥ�� ��Ʈ ����.
    TColor          FChangedFontColor;          ///< ����ڰ� ���� ������ ��� ��Ʈ ����.
    TColor          FFontShadowColor;           ///< ���� Digit ��� �� ���� �׸��� ����. 

    void        __fastcall SetColor(int Index, TColor c);

    TNotifyEvent     FOnChange;                 ///< ���� ������ �� class���� ���⿡ Event�� ����� ���̴�.
    void        __fastcall DoOnChange(void);    ///< �� �Ӽ��� ����� �� ���� �� �Լ��� ȣ���Ͽ� FOnChange�� ��ϵ� Evnet�� ���� ��� ȣ�� ���� �ش�.

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
    bool                        FVisible;       ///< Title ��ü ���̱�/�Ⱥ��̱�.
    UnicodeString               FCaption;       ///< ���� ǥ�� (�ܼ� ǥ���)
    int                         FWidth;         ///< Unit�� ǥ��Ǵ� ���� ����
    Vcl::Graphics::TFont    *   FFont;          ///< ��Ʈ Ư��
    System::Classes::TAlignment FAlignment;     ///< Caption ������ ��ġ (Left, Center, Right)
    TCaptionAlign               FAlign;         ///< Edit Box�� �������� Caption�� ��ġ�ϴ� �ڸ�.
    TColor                      FColor;         ///< ����
    bool                        FTransparent;   ///< ���� ����
    bool                        FOutLineVisible;///< Out Line ���̱�
    TColor                      FOutLineColor;  ///< Out Line Color
    int                         FRound;         ///< Round
    int                         FSpace;         ///< Title�� Unit ������ ����
    int                         FMargin;        ///< Title Caption�� Align ���ǿ� ���� ���� ���� �߰�. 

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

    TNotifyEvent        FOnChange;                                  ///< ���� ������ �� class���� ���⿡ Event�� ����� ���̴�.
    void    __fastcall  DoOnChange(void);                           ///< �� �Ӽ��� ����� �� ���� �� �Լ��� ȣ���Ͽ� FOnChange�� ��ϵ� Evnet�� ���� ��� ȣ�� ���� �ش�.
    void    __fastcall  FontChanged(System::TObject* Sender);   ///< Font�� �Ӽ��� ����� ���� Call Back �ޱ� ���� Event �Լ�.

public:
            __fastcall  TNumShapeTitleProperty();
  virtual   __fastcall ~TNumShapeTitleProperty();

  __property TNotifyEvent OnChange = {read = FOnChange, write = FOnChange};
  void      __fastcall  Assign(TPersistent* Source);

__published:

    // ���� ���� ���� �Ӽ� -----------------------
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
* @brief : OnClick Event�� bool Ok ���ڸ� �߰��� Event�� ���� �����Ѵ�.
           TPanel�� ���� OnClick Event�� MyClick() �Լ��� ��ϵǰ�
           MyClick() �Լ� ������ FOnClick Event Property�� ����ڰ� �����
           Event �Լ��� ���� ��� Numer Pad ���� ����� ����
           bool Ok ���ڿ� �ش� ����� ���� �Ѱ��ָ鼭 ���� �����ش�.
* @param : System::TObject* Sender : ������Ʈ ������
* @param : bool Ok : Number Pad�� �Է��� Ok�� ������ ��� true,
                     Cancel�� ������ ��� false�� �־��ش�.
* @return void :
* @see  TNumPanelClickEvent FOnClick
* @see  void     __fastcall MyClick(TObject *Sender)
- history : 1. [2014�� 6�� 17�� ȭ����][�۽ſ�] ���� �ۼ�
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

    double        FValue;               ///< ���� ��.
    double        FValueOld;            ///< ���� �� ��.

//    TInputNumType FType;                  ///< ����������, �Ǽ������� �����Ѵ�.
//    double        FMin;                   ///< �Է� ���� �ּ� ����.
//    double        FMax;                   ///< �Է� ���� �ִ� ����.
//    double        FUpDownValue;           ///< �Է� Number Pad �󿡼� Up/Down ��ư�� �̿��� �� ����/���� ��.
//    int           FDecimalPlaces;         ///< �Ҽ��� �ڸ��� ǥ�� (3�� ��� �Ҽ��� 3�ڸ����� ǥ���Ѵ�)

    int           FRound;                   ///< �𼭸� Round ����.
    bool          FOldValueDisplay;         ///< ���� �� �ٽ� ���� ��� ���.
    bool          FOldValueUnderline;       ///< true�̸� Old Value �� ǥ��� Under line�� ǥ���. 
    int           FOldValueFontSize;        ///< Old Value Font Size�� ������. (0�� ��� Defualt�� �⺻ Font Size�� 1/2 ���� �����)
    int           FSpace;                   ///< Value�� �ܰ� ������ ����. (Alignment�� Right�� ��� ������ ����, Left�� ��� ���� ������ ����ȴ�.)
    int           FEditWidth;               ///< Title�� ����� ��� Edit�� �����ϴ� ũ�⸦ ����. Title�� ������� ������ �ǹ� ���� ���̴�. ���� FAlign ���� Left, Right �� ��츸 ���ȴ�. 
    int           FEditHeight;              ///< Title�� ����� ��� Edit�� �����ϴ� ���� ����. Title�� ������� ������ �ǹ� ����. ���� FAlign ���� Top,Bottom�� ��츸 ��� �ȴ�.
    TNumUnitProperty        * FUnit;        ///< Unit  ���� Property
    TNumShapeColorProperty  * FColorSet;    ///< Color ���� Property
    TAZEditBoxProperty      * FInputEdit;   ///< Input Number Pad Property
    TNumShapeTitleProperty  * FTitle;       ///< Title ���� Property
    bool            FReadOnly;              ///< �б� �������� ���
	bool            FNoEdit;              	///< InputEdit ��� ����
    UnicodeString   FPath;                  ///< Ini File Path;
    bool            FInvalidateUse;         ///< AZ_Value���� ���� ����Ǿ� �ٽ� �׷����� ��� Invalidate() �� ������� �ƴϸ� �ٷ� Paint()�Լ��� ȣ������ �����Ѵ�. true�̸� �ٷ� Paint()�Լ��� ȣ���Ѵ�. (������ �ذ�)

    bool            FFixedDigitUse;         ///< Font�� �׸��� �ʰ� ������ Digit �׸��� ���. 

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
//#else // ������/���� �Ǽ��� ������Ƽ�� �۽ý���Ʈ ��ī������ ������ ���� �ذ�.
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


    bool   bLoaded;        ///< dfm ���Ϸ� ���� ����� ������ ��� �ε� �� ����.
    bool   bSelecting;     ///< ����ڰ� ��ġ�� ����.
    bool   bNumbering;     ///< ���� ���� �Է����� ����
    bool   bChanged;       ///< ����ڰ� FValue�� ������ ��� Ȯ��.
    bool   bBeginUpdate;   ///< Update ���� �˸� �÷���, �̺����� true�̸� ȭ���� �׸��� �ʴ´�. 

    void    __fastcall UnitChanged(System::TObject* Sender);
    void    __fastcall ColorChanged(System::TObject* Sender);
    void    __fastcall InputEditChanged(System::TObject* Sender);
    void    __fastcall TitleChanged(System::TObject* Sender);


    /// ������/���� �Ǽ��� ������Ƽ�� �۽ý���Ʈ ��ī������ ������ ���� �ذ�.
    virtual void __fastcall DefineProperties(TFiler *filer);


    Graphics::TBitmap* tmpBitmap1;
    Graphics::TBitmap* tmpBitmap2;
    Graphics::TBitmap* pBitmapTemp;

    int     m_nMouseDownXPos;
    int     m_nMouseDownYPos;

    int     m_nTitleWidth;      // Title �� �����ϴ� ��ü ũ��
    int     m_nTitleHeight;     // Title �� �����ϴ� ��ü ũ��

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
    void            __fastcall WndProc(TMessage& Message);  ///< WndProc �Լ� �������̵�.
    virtual void    __fastcall SetEnabled(bool Value);        ///< SetEnabled �Լ� �������̵�.
    void            __fastcall Loaded(void);                ///< Loaded �Լ� �������̵�.

public:
                    __fastcall TAZNumShape(TComponent* Owner);      ///< ������.
    virtual         __fastcall ~TAZNumShape();                         ///< �Ҹ���.

    
    void            __fastcall Paint();                             ///< Paint �Լ� �������̵�.
    void            __fastcall PaintTitle();
    void            __fastcall PaintValue();

    void            __fastcall ResetChanged();
    bool            __fastcall IsChanged();

    void            __fastcall BeginUpdate();
    void            __fastcall EndUpdate();

	void            __fastcall SetEncoderButton(bool bEnable, AnsiString sTitle, AnsiString unit, TAZNumShape * pAZNumShape);  // , TAZNumShape *pAZNumShape, , AnsiString sUnit

__published:

	// �Ӽ� -----------------------
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
//#else // ������/���� �Ǽ��� ������Ƽ�� �۽ý���Ʈ ��ī������ ������ ���� �ذ�.
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

    // �� ������ Event ---------------------------
    __property TNumShapeClickEvent OnClick = {read = FOnClick, write = FOnClick};


    // ��ӹ��� �Ӽ� ��� ------------------------
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
