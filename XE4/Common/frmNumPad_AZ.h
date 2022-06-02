//---------------------------------------------------------------------------

#ifndef frmNumPad_AZH
#define frmNumPad_AZH
//---------------------------------------------------------------------------
#define LKDIGIT_PANEL_USE     0

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <OleCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

#if LKDIGIT_PANEL_USE
#include "LKDIGITLib_OCX.h"
#endif

//---------------------------------------------------------------------------
#define DEF_NUM_PAD_INT             0
#define DEF_NUM_PAD_FLOAT           1

//#define DEF_MAX_DOTDOWNDIGIT_CNT    5

class TfmNumPad_AZ : public TForm
{
__published:    // IDE-managed Components
    TStaticText *StaticText1;
    TStaticText *StaticText2;
    TStaticText *StaticText3;
    TSpeedButton *SpeedButton1;
    TSpeedButton *SpeedButton2;
    TSpeedButton *SpeedButton3;
    TSpeedButton *SpeedButton4;
    TSpeedButton *SpeedButton5;
    TSpeedButton *SpeedButton6;
    TSpeedButton *SpeedButton7;
    TSpeedButton *SpeedButton8;
    TSpeedButton *SpeedButton9;
    TSpeedButton *SpeedButton10;
    TSpeedButton *SpeedButton_Dot;
    TSpeedButton *SpeedButton_Minus;
    TSpeedButton *SpeedButton_Cancel;
    TSpeedButton *SpeedButton_OK;
    TSpeedButton *SpeedButton_Clear;
    TSpeedButton *SpeedButton_Back;
    TSpeedButton *SpeedButton_Up;
    TSpeedButton *SpeedButton_Down;
    TPanel *Panel_InputNum;
    TPanel *Panel_Min;
    TPanel *Panel_Max;
    TPanel *Panel_OrgValue;
	TGroupBox *GroupBox_Encoder;
	TPanel *Panel_Encoder_Title;
	TPanel *Panel2;
	TLabel *Label_Encoder_Value;
	TLabel *Label_Encoder_unit;
	TSpeedButton *SpeedButton_EnterEncoderValue;
	TBitBtn *BitBtn_EnterEncoderValue;
    void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall SpeedButton_CancelClick(TObject *Sender);
    void __fastcall SpeedButton_OKClick(TObject *Sender);
    void __fastcall SpeedButton_MinusClick(TObject *Sender);
    void __fastcall SpeedButton_ClearClick(TObject *Sender);
    void __fastcall SpeedButton_BackClick(TObject *Sender);
    void __fastcall SpeedButton_DotClick(TObject *Sender);
    void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
    void __fastcall SpeedButton_UpClick(TObject *Sender);
    void __fastcall SpeedButton_DownClick(TObject *Sender);
	void __fastcall SpeedButton_EnterEncoderValueClick(TObject *Sender);
	void __fastcall BitBtn_EnterEncoderValueClick(TObject *Sender);
private:    // User declarations
    AnsiString m_sInputNum;
    bool    m_bDotIntput;
    bool    m_bMinus;
    short   m_nDotDownCnt;
    bool    m_bIsActionExist;
    int     m_nDecimalPlaces;     ///< 소수점 자리 수 표기 (3일 경우 소수점 3자리까지 표기한다)
    
    int  m_InputType;
    double m_dMin;
    double m_dMax;
    double m_dCurrent;
    double m_dUpDownValue;
    AnsiString * m_pReturnStr;

    int         m_Mode;
    double      m_dAddValue;
    AnsiString  m_sAddValue;

    bool        m_bEnableEncoderButton;
    double      m_dEncoderValue;
    AnsiString  m_sEncoderUnit;
    AnsiString  m_sEncoderTitle;

    void __fastcall DisplayUpdate(AnsiString sValue);
    void __fastcall InputNumber(int nNumber);
    bool __fastcall CheckMinMaxValule(AnsiString sValue);
    void __fastcall Init(double Current);
    AnsiString __fastcall CheckNumString(AnsiString s);
    

public:        // User declarations
    __fastcall TfmNumPad_AZ(TComponent* Owner, int nType, double dMin, double dMax, double dCurnt, AnsiString * sReturn, double dUpDownValue = 1.0, int nDecimalPlaces = 3);

    void __fastcall SetEncoderButton(bool bEnable, double dEncoder, AnsiString sTitle, AnsiString sUnit = "mm");
    
};
//---------------------------------------------------------------------------
extern PACKAGE TfmNumPad_AZ *fmNumPad_AZ;
//---------------------------------------------------------------------------
#endif
