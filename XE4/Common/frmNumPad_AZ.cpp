//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmNumPad_AZ.h"
#include "Common\\Define.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#if LKDIGIT_PANEL_USE
#pragma link "LKDIGITLib_OCX"
#endif


enum {
    eMODE_NORMAL = 0,
    eMODE_PLUS,
    eMODE_MINUS,
    eMODE_MAX
};

#pragma resource "*.dfm"
TfmNumPad_AZ *fmNumPad_AZ;

//---------------------------------------------------------------------------
__fastcall TfmNumPad_AZ::TfmNumPad_AZ(TComponent* Owner, int nType, double dMin, double dMax, double dCurnt, AnsiString * sReturn, double dUpDownValue, int nDecimalPlaces)
    : TForm(Owner)
{
    m_dMin          = dMin;
    m_dMax          = dMax;
    m_pReturnStr    = sReturn;
    m_InputType     = nType;
    m_dCurrent      = dCurnt;
    m_dUpDownValue  = dUpDownValue;
    m_nDecimalPlaces= nDecimalPlaces;

    m_nDotDownCnt   = 0;
    m_bDotIntput    = false;
    m_bIsActionExist = false; 
    m_bMinus         = false;

    Panel_InputNum->Font->Name  = AZ_NUM_SHAPE_FONT;
    Panel_Max->Font->Name       = AZ_NUM_SHAPE_FONT;
    Panel_OrgValue->Font->Name  = AZ_NUM_SHAPE_FONT;
    Panel_Min->Font->Name       = AZ_NUM_SHAPE_FONT;

    m_Mode          = eMODE_NORMAL;
    m_dAddValue     = 0;

}
//---------------------------------------------------------------------------

void __fastcall TfmNumPad_AZ::DisplayUpdate(AnsiString sValue)
{
    //Caption = sValue; // TODO: TEST ONLY

    double dValue = 0;

    int nDotCnt = sValue.AnsiPos(".");
    if(nDotCnt > 0) nDotCnt = sValue.Length() - nDotCnt;

#if LKDIGIT_PANEL_USE
    if(nDotCnt > 0) {
        LKNPanel_InputNum->DownNumberOfDecimals = nDotCnt + 1;
    }
    else {
        LKNPanel_InputNum->DownNumberOfDecimals = 0;
    }

    LKNPanel_InputNum->UpNumberOfDecimals = 11 - LKNPanel_InputNum->DownNumberOfDecimals;

    if(sValue == "" || sValue == "-" || sValue == ".") {
        LKNPanel_InputNum->PanelValue = 0;
        return;
    }
    else {
        switch(m_InputType) {
            case DEF_NUM_PAD_INT:
                LKNPanel_InputNum->PanelValue = (int) (sValue.ToDouble());
                break;

            case DEF_NUM_PAD_FLOAT:
                LKNPanel_InputNum->PanelValue = sValue.ToDouble();
                break;            

            default:
                LKNPanel_InputNum->PanelValue = sValue.ToDouble();;
                break;
        }
    }

#else

//    if(nDotCnt > 0) {
//        Panel_InputNum->DownNumberOfDecimals = nDotCnt + 1;
//    }
//    else {
//        Panel_InputNum->DownNumberOfDecimals = 0;
//    }
//
//    Panel_InputNum->UpNumberOfDecimals = 11 - Panel_InputNum->DownNumberOfDecimals;

    switch(m_Mode) {
        case eMODE_PLUS:
            Panel_InputNum->Caption = "+" + m_sAddValue;
            SpeedButton_Minus->Enabled = false;
            SpeedButton_Down->Enabled = false;
            SpeedButton_Up->Enabled = false;
            SpeedButton_OK->Caption = "=";
            break;

        case eMODE_MINUS:
            Panel_InputNum->Caption = "-" + m_sAddValue;
            SpeedButton_Minus->Enabled = false;
            SpeedButton_Down->Enabled = false;
            SpeedButton_Up->Enabled = false;
            SpeedButton_OK->Caption = "=";
            break;

        case eMODE_NORMAL:
        default:
            SpeedButton_Down->Enabled = true;
            SpeedButton_Up->Enabled = true;
            SpeedButton_Minus->Enabled = (m_dMin < 0) ? true : false;
            SpeedButton_OK->Caption = "OK";

            if(sValue == "" || sValue == "-" || sValue == ".") {
                if(m_bMinus) {
                    Panel_InputNum->Caption = "-";
                }
                else if(m_bDotIntput){
                    Panel_InputNum->Caption = "0.";
                }
                else {
                    Panel_InputNum->Caption = "0";
                }

                return;
            }
            else {
                switch(m_InputType) {
                    case DEF_NUM_PAD_INT:
                        Panel_InputNum->Caption = sValue.ToIntDef(0);
                        break;

                    case DEF_NUM_PAD_FLOAT:

                        //Caption = sValue; // TODO: TEST ONLY

                        //Panel_InputNum->Caption = sValue.ToDouble();

                        dValue = sValue.ToDouble();

                        if(dValue == 0.0) {
                            //Caption = "Value is zero!";    // TODO: TEST ONLY

                            AnsiString sTemp;

                            if(m_bMinus)                            sTemp = "-";
                            else                                    sTemp = "0";
                            if(m_bMinus && (sValue.Length() > 1))   sTemp = sTemp + "0";
                            if(m_bDotIntput)                        sTemp = sTemp + ".";

                            if(nDotCnt > 0) {
                                AnsiString sDot;
                                sDot = sDot.StringOfChar('0', nDotCnt);
                                sTemp = sTemp + sDot;
                            }

                            Panel_InputNum->Caption = sTemp;
                        }
                        else {
                            if(sValue.SubString(sValue.Length(), 1) == ".") {
                                Panel_InputNum->Caption = IntToStr((int) (dValue)) + ".";
                            }
                            else {
                                if(nDotCnt > 0) {
                                    AnsiString sDot;
                                    AnsiString sFormat;
                                    sDot = sDot.StringOfChar('0', nDotCnt);
                                    sFormat = "#0." + sDot;
                                    Panel_InputNum->Caption = FormatFloat(sFormat, dValue);
                                }
                                else {
                                    Panel_InputNum->Caption = dValue;
                                }
                            }
                        }
                        break;

                    default:
                        Panel_InputNum->Caption = sValue.ToDouble();
                        break;
                }
            }
            break;
    }

    switch(m_Mode) {
        case eMODE_NORMAL:  Panel_InputNum->Font->Color = clAqua;       break;
        case eMODE_PLUS:    Panel_InputNum->Font->Color = clHighlight;  break;
        case eMODE_MINUS:   Panel_InputNum->Font->Color = clSilver;     break;
        default:            Panel_InputNum->Font->Color = clAqua;       break;
    }

#endif
    
}
//---------------------------------------------------------------------------
bool __fastcall TfmNumPad_AZ::CheckMinMaxValule(AnsiString sValue)
{
    //bool bMaxValueOver = false;
    bool bMinValueOver = false;

    double dValue = sValue.ToDouble();

#if LKDIGIT_PANEL_USE
    switch(m_InputType) {
        case DEF_NUM_PAD_INT:
            if(dValue >= 2147483648.0) {
                LKNPanel_Max->DigitColor = 3;
                return false;
            }
            else if(dValue <= -2147483648.0){
                LKNPanel_Min->DigitColor = 3;
                return false;
            }
            break;
    }
    
    if(dValue > m_dMax) { // Max 값 초과. 
        LKNPanel_Max->DigitColor = 3;
        return false;
    }
    else if(dValue < m_dMin) {
        if(m_bMinus) {
            LKNPanel_Min->DigitColor = 3;
            return false;
        }
        else {
            bMinValueOver = true;
        }
    }    
    
    if(bMinValueOver) { // Max 값 초과. 
        LKNPanel_Min->DigitColor = 3;
    }
    else {
        LKNPanel_Min->DigitColor = 4;
    }

    LKNPanel_Max->DigitColor = 4;

#else

    switch(m_InputType) {
        case DEF_NUM_PAD_INT:
            if(dValue >= 2147483648.0) {
                Panel_Max->Font->Color = clRed;
                return false;
            }
            else if(dValue <= -2147483648.0){
                Panel_Min->Font->Color = clRed;
                return false;
            }
            break;
    }

    if((m_dMax >= 0) && dValue > m_dMax) { // Max 값 초과.
        Panel_Max->Font->Color = clRed;
        return false;
    }
    else if((m_dMax < 0) && (dValue > 0)) {
        Panel_Max->Font->Color = clRed;
        return false;
    }
    else if(dValue < m_dMin) {
        if(m_bMinus) {
            Panel_Min->Font->Color = clRed;
            return false;
        }
        else {
            bMinValueOver = true;
        }
    }

    if(bMinValueOver) { // Max 값 초과.
        Panel_Min->Font->Color = clRed;
    }
    else {
        Panel_Min->Font->Color = clYellow;
    }

    Panel_Max->Font->Color = clYellow;


#endif

    m_sInputNum = sValue;
    
    DisplayUpdate(m_sInputNum);

    m_bIsActionExist = true;

    return true;
}
//---------------------------------------------------------------------------
void __fastcall TfmNumPad_AZ::InputNumber(int nNumber)
{
    AnsiString sTemp = m_sInputNum + IntToStr(nNumber);

    if(m_bDotIntput) {
        if(m_nDotDownCnt < m_nDecimalPlaces) {
            m_nDotDownCnt++;
        }
        else {
            return;
        }
    }

    CheckMinMaxValule(sTemp);
}
//---------------------------------------------------------------------------
void __fastcall TfmNumPad_AZ::SpeedButton1Click(TObject *Sender)
{
    TSpeedButton * pBtn = dynamic_cast<TSpeedButton *>(Sender);

    if(pBtn == NULL) {
        return;
    }

    int nNum = pBtn->Tag;


    switch(m_Mode) {
        case eMODE_PLUS:
        case eMODE_MINUS:
            {
                m_sAddValue += IntToStr(nNum);
                m_dAddValue  = CheckNumString(m_sAddValue).ToDouble();
                double dTemp = CheckNumString(m_sInputNum).ToDouble();
                dTemp = (m_Mode == eMODE_PLUS) ? dTemp + m_dAddValue : dTemp - m_dAddValue;

                if(dTemp < m_dMin) {
                    m_sAddValue = m_sAddValue.Delete(m_sAddValue.Length(), 1);
                    Panel_Min->Font->Color = clRed;
                }
                else if(dTemp > m_dMax) {
                    m_sAddValue = m_sAddValue.Delete(m_sAddValue.Length(), 1);
                    Panel_Max->Font->Color = clRed;
                }
                else {
                    Panel_Min->Font->Color = clYellow;
                    Panel_Max->Font->Color = clYellow;
                    DisplayUpdate(m_sAddValue);
                }
            }
            break;

        default:
            if(m_bIsActionExist == false) {
                m_sInputNum     = "";
                m_bDotIntput    = false;
                m_bMinus        = false;
                m_nDotDownCnt   = 0;
            }

            InputNumber(nNum);

            break;
    }

}
//---------------------------------------------------------------------------

void __fastcall TfmNumPad_AZ::FormClose(TObject *Sender, TCloseAction &Action)
{
    Action = caFree;
}
//---------------------------------------------------------------------------

void __fastcall TfmNumPad_AZ::FormCreate(TObject *Sender)
{
    Init(m_dCurrent);
}
//---------------------------------------------------------------------------

void __fastcall TfmNumPad_AZ::Init(double Current)
{
    switch(m_InputType) {
        case DEF_NUM_PAD_INT:
            m_sInputNum = IntToStr((int)Current);
            SpeedButton_Dot->Enabled = false;
            break;

        case DEF_NUM_PAD_FLOAT:
            {
                AnsiString sDot;
                AnsiString sFormat;
                sDot = sDot.StringOfChar('0', m_nDecimalPlaces);
                sFormat = "#0." + sDot;
                m_sInputNum = FormatFloat(sFormat, Current) ;
            }
            break;
    }

    if(m_dMin >= 0) SpeedButton_Minus->Enabled = false;

#if LKDIGIT_PANEL_USE

    int nUpNumberDigit = 0;
    int nDownNumberDigit = 0;

    switch(m_InputType) {
        case DEF_NUM_PAD_INT:
            nUpNumberDigit = 10;
            nDownNumberDigit = 0;
            break;

        case DEF_NUM_PAD_FLOAT:
            nUpNumberDigit = 7;
            nDownNumberDigit = 3;
            break;
    }
    
    LKNPanel_InputNum->DigitColor = 1;
    LKNPanel_InputNum->DigitSize = 1;
    LKNPanel_InputNum->DownNumberOfDecimals = 0;
    LKNPanel_InputNum->UpNumberOfDecimals = 11;

    LKNPanel_Max->DigitColor = 4;
    LKNPanel_Max->DigitSize = 0;
    LKNPanel_Max->DownNumberOfDecimals = nDownNumberDigit;
    LKNPanel_Max->UpNumberOfDecimals = nUpNumberDigit;

    LKNPanel_Min->DigitColor = 4;
    LKNPanel_Min->DigitSize = 0;
    LKNPanel_Min->DownNumberOfDecimals = nDownNumberDigit;
    LKNPanel_Min->UpNumberOfDecimals = nUpNumberDigit;

    LKNPanel_OrgValue->DigitColor = 0;
    LKNPanel_OrgValue->DigitSize = 0;
    LKNPanel_OrgValue->DownNumberOfDecimals = nDownNumberDigit;
    LKNPanel_OrgValue->UpNumberOfDecimals = nUpNumberDigit;
#endif

    if(m_sInputNum.AnsiPos(".")) {
        int nPos = m_sInputNum.AnsiPos(".");
        int nLen = m_sInputNum.Length();
        m_nDotDownCnt = nLen - nPos;

        m_bDotIntput = true;
    }

    if(m_sInputNum.AnsiPos("-")) {
        m_bMinus = true;
    }

    DisplayUpdate(m_sInputNum);
}
//---------------------------------------------------------------------------

void __fastcall TfmNumPad_AZ::FormDestroy(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TfmNumPad_AZ::FormShow(TObject *Sender)
{
#if LKDIGIT_PANEL_USE
    switch(m_InputType) {
        case DEF_NUM_PAD_INT:
            LKNPanel_Max->PanelValue        = (int)m_dMax;
            LKNPanel_OrgValue->PanelValue   = (int)m_dCurrent;
            LKNPanel_Min->PanelValue        = (int)m_dMin ;
            break;

        case DEF_NUM_PAD_FLOAT:
            LKNPanel_Max->PanelValue        = m_dMax;
            LKNPanel_OrgValue->PanelValue   = m_dCurrent;
            LKNPanel_Min->PanelValue        = m_dMin ;
            break;
    }
#else
    switch(m_InputType) {
        case DEF_NUM_PAD_INT:
            Panel_Max->Caption        = IntToStr((int)m_dMax);
            Panel_OrgValue->Caption   = IntToStr((int)m_dCurrent);
            Panel_Min->Caption        = IntToStr((int)m_dMin);
            break;

        case DEF_NUM_PAD_FLOAT:
            {
                AnsiString sDot;
                AnsiString sFormat;
                sDot = sDot.StringOfChar('0', m_nDecimalPlaces);
                sFormat = "#0." + sDot;

                Panel_Max->Caption        = FormatFloat(sFormat, m_dMax);
                Panel_OrgValue->Caption   = FormatFloat(sFormat, m_dCurrent);
                Panel_Min->Caption        = FormatFloat(sFormat, m_dMin);                
            }
            break;
    }
#endif

//    this->Left  = (Screen->Width - this->Width) / 2;
//    this->Top   = (Screen->Height - this->Height) / 2;
}
//---------------------------------------------------------------------------

void __fastcall TfmNumPad_AZ::SpeedButton_CancelClick(TObject *Sender)
{
    switch(m_Mode) {
        case eMODE_PLUS:
        case eMODE_MINUS:
            m_Mode      = eMODE_NORMAL;
            m_dAddValue = 0.0;
            m_sAddValue = "";

            Panel_Min->Font->Color = clYellow;
            Panel_Max->Font->Color = clYellow;

            DisplayUpdate(m_sInputNum);
            break;

        default:
            ModalResult = mrCancel;
            break;
    }
}
//---------------------------------------------------------------------------
AnsiString __fastcall TfmNumPad_AZ::CheckNumString(AnsiString s)
{
    if(s == "")  s = "0";
    if(s == ".") s = "0";
    if(s == "-") s = "0";

    return s;
}
//---------------------------------------------------------------------------
void __fastcall TfmNumPad_AZ::SpeedButton_OKClick(TObject *Sender)
{
    AnsiString sTemp;
    AnsiString sPrint;

    switch(m_Mode) {
        case eMODE_PLUS:
        case eMODE_MINUS:
            {
                Panel_Min->Font->Color = clYellow;
                Panel_Max->Font->Color = clYellow;

                m_sAddValue     = CheckNumString(m_sAddValue);
                m_dAddValue     = m_sAddValue.ToDouble();
                sTemp           = CheckNumString(m_sInputNum);
                double dTemp    = sTemp.ToDouble();

                dTemp           = (eMODE_PLUS == m_Mode) ? dTemp + m_dAddValue : dTemp - m_dAddValue;
                sPrint          = "%." + IntToStr(m_nDecimalPlaces) + "f";
                sTemp.printf(sPrint.c_str(), dTemp);

                m_Mode           = eMODE_NORMAL;

                m_sAddValue      = "";
                m_dAddValue      = 0;

                if(CheckMinMaxValule(sTemp)) {
                    m_bDotIntput     = false;
                    m_bMinus         = false;
                    m_nDotDownCnt    = 0;

                    Init(dTemp);
                }
                else {
                    DisplayUpdate(m_sInputNum);
                }

                m_bIsActionExist = false;
            }
            break;

        default:
            {
                m_sInputNum = CheckNumString(m_sInputNum);

                double dValue = m_sInputNum.ToDouble();
                bool bValueChange = false;

                if(dValue > m_dMax) {
                    dValue = m_dMax;
                    bValueChange = true;
                }

                if(dValue < m_dMin){
                    dValue = m_dMin;
                    bValueChange = true;
                }

                if(bValueChange) {
                    switch(m_InputType) {
                        case DEF_NUM_PAD_INT:
                            m_sInputNum = IntToStr((int)dValue);
                            break;

                        case DEF_NUM_PAD_FLOAT:
                            m_sInputNum = dValue;
                            break;
                    }
                }

                *m_pReturnStr = m_sInputNum;
                ModalResult  = mrOk;
            }
            break;
    }



}
//---------------------------------------------------------------------------


void __fastcall TfmNumPad_AZ::SpeedButton_MinusClick(TObject *Sender)
{
    if(m_bIsActionExist == true) {
        if(m_sInputNum != "") return;
        if(m_dMin >= 0) return;
    }

    m_bIsActionExist = true;
    m_sInputNum = "-";
    m_nDotDownCnt = 0;
    m_bDotIntput = false;
    m_bMinus = true;
    DisplayUpdate(m_sInputNum);
}
//---------------------------------------------------------------------------

void __fastcall TfmNumPad_AZ::SpeedButton_ClearClick(TObject *Sender)
{
    switch(m_Mode) {
        case eMODE_PLUS:
        case eMODE_MINUS:
            m_dAddValue = 0;
            m_sAddValue = "";
            break;

        default:
            m_bDotIntput = false;
            m_nDotDownCnt = 0;
            m_sInputNum = "";
            m_bMinus = false;
            break;
    }

    DisplayUpdate(m_sInputNum);
}
//---------------------------------------------------------------------------

void __fastcall TfmNumPad_AZ::SpeedButton_BackClick(TObject *Sender)
{
    switch(m_Mode) {
        case eMODE_PLUS:
        case eMODE_MINUS:
            if(m_sAddValue.Length() > 0) {
                m_sAddValue = m_sAddValue.Delete(m_sAddValue.Length(), 1);
            }
            break;

        default:
            if(m_nDotDownCnt > 0) {
                m_nDotDownCnt--;
            }

            if(m_sInputNum != "") {
                if(m_sInputNum.SubString(m_sInputNum.Length(), 1) == ".") {
                    m_sInputNum = m_sInputNum.Delete(m_sInputNum.Length(), 1);
                    m_bDotIntput = false;
                }
                else if(m_sInputNum != "") {
                    m_sInputNum = m_sInputNum.Delete(m_sInputNum.Length(), 1);
                }

                if(m_sInputNum == "") {
                    m_bMinus = false;
                }

        #if LKDIGIT_PANEL_USE
                if(m_sInputNum != "" && m_sInputNum != "." && m_sInputNum != "-") {
                    double dValue = m_sInputNum.ToDouble();
                    if(dValue > m_dMax) { // Max 값 초과.
                        LKNPanel_Max->DigitColor = 3;
                        return;
                    }
                    else {
                        LKNPanel_Max->DigitColor = 4;
                    }

                    if(dValue < m_dMin) { // Min 값 초과.
                        LKNPanel_Min->DigitColor = 3;
                        return;
                    }
                    else {
                        LKNPanel_Min->DigitColor = 4;
                    }
                }
                else {
                    LKNPanel_Max->DigitColor = 4;
                    LKNPanel_Min->DigitColor = 4;
                }
        #else
                if(m_sInputNum != "" && m_sInputNum != "." && m_sInputNum != "-") {
                    double dValue = m_sInputNum.ToDouble();
                    if(dValue > m_dMax) { // Max 값 초과.
                        Panel_Max->Font->Color = clRed;
                        return;
                    }
                    else {
                        Panel_Max->Font->Color = clYellow;
                    }

                    if(dValue < m_dMin) { // Min 값 초과.
                        Panel_Min->Font->Color = clRed;
                        return;
                    }
                    else {
                        Panel_Min->Font->Color = clYellow;
                    }
                }
                else {
                    Panel_Max->Font->Color = clYellow;
                    Panel_Min->Font->Color = clYellow;
                }
        #endif
            }
            else {
                m_bDotIntput = false;
                m_nDotDownCnt = 0;
                m_bMinus = false;
            }
            break;
    }


    
    DisplayUpdate(m_sInputNum);
}
//----------------------------------------------------
void __fastcall TfmNumPad_AZ::SpeedButton_DotClick(TObject *Sender)
{

    switch(m_Mode) {
        case eMODE_PLUS:
        case eMODE_MINUS:
            if(m_sAddValue.AnsiPos(".") == 0) {
                m_sAddValue += ".";
            }
            break;

        default:
            {
                //Caption = m_sInputNum;  // TODO: TEST ONLY

                if(m_bIsActionExist == false) {
                    m_bDotIntput    = false;
                    m_bMinus        = false;
                    m_nDotDownCnt   = 0;
                    m_sInputNum     = "0";
                    m_bIsActionExist = true;
                }

                //Caption = Caption +  ", " + m_sInputNum;  // TODO: TEST ONLY

                if(m_bDotIntput) return;

                if(m_InputType == DEF_NUM_PAD_INT) return;

                if(m_sInputNum == "-") return;

            #if LKDIGIT_PANEL_USE
                if(m_sInputNum != "") {
                    double dValue = m_sInputNum.ToDouble();
                    if(dValue > m_dMax) { // Max 값 초과.
                        LKNPanel_Max->DigitColor = 3;
                        return;
                    }
                    else {
                        LKNPanel_Max->DigitColor = 4;
                    }

                    if(dValue < m_dMin) { // Min 값 초과.
                        LKNPanel_Min->DigitColor = 3;
                        //return;
                    }
                    else {
                        LKNPanel_Min->DigitColor = 4;
                    }

                    m_sInputNum = m_sInputNum + ".";
                }

            #else
                if(m_sInputNum != "") {
                    double dValue = m_sInputNum.ToDouble();
                    if(dValue > m_dMax) { // Max 값 초과.
                        Panel_Max->Font->Color = clRed;
                        return;
                    }
                    else {
                        Panel_Max->Font->Color = clYellow;
                    }

                    if(dValue < m_dMin) { // Min 값 초과.
                        Panel_Min->Font->Color = clRed;
                        //return;
                    }
                    else {
                        Panel_Min->Font->Color = clYellow;
                    }

                    m_sInputNum = m_sInputNum + ".";
                }

            #endif

                m_bDotIntput = true;

            }
            break;
    }

    DisplayUpdate(m_sInputNum);
}
//---------------------------------------------------------------------------

void __fastcall TfmNumPad_AZ::FormKeyPress(TObject *Sender, wchar_t &Key)
{
    switch(Key) {
        case '\x1b':    // ESC key
            ModalResult = mrCancel;
            break;

        case '\r':      // Enter Key
            SpeedButton_OKClick(NULL);
            break;

        case '\b':      // Backspace
            SpeedButton_BackClick(NULL);
            break;

        case '.':
            SpeedButton_DotClick(NULL);
            break;

        case '-':
            if(m_bIsActionExist == false) {
                m_bDotIntput = false;
                m_nDotDownCnt = 0;
                m_sInputNum = "";
                m_bMinus = false;
            } 
            
            SpeedButton_MinusClick(NULL);
            break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if(m_bIsActionExist == false) {
                m_bDotIntput = false;
                m_nDotDownCnt = 0;
                m_sInputNum = "";
                m_bMinus = false;
            } 
            
            InputNumber(Key - '0');
            break;

        default:
             Key = 0 ;
            break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfmNumPad_AZ::SpeedButton_UpClick(TObject *Sender)
{
#if 1

    switch(m_Mode) {
        case eMODE_NORMAL:
            m_Mode      = eMODE_PLUS;
            m_dAddValue = 0.0;
            m_sAddValue = "";

            DisplayUpdate(m_sAddValue);
            break;

        default:
            break;
    }

#else
    double dCurrnetValue = 0;
    AnsiString sTemp;
    
    if(m_sInputNum == "" || m_sInputNum == "." || m_sInputNum == "-") 
        dCurrnetValue = 0;
    else 
        dCurrnetValue = m_sInputNum.ToDouble();

    // --------------------------------------------
    // UpDown 버튼 이용시 소수점이 존재할 경우 소수점 표기 위해 . 
    double dTemp = m_dUpDownValue - (double)((int)m_dUpDownValue);
    int    nDownDigitCnt = 0;
    
    for(int i=0; i<3; i++) {
        if(dTemp > 0) {
            nDownDigitCnt++;
            dTemp = dTemp * 10.0;
            dTemp = dTemp - (double)((int)dTemp);
        }
    }

    if(nDownDigitCnt > 0) {
        m_bDotIntput = true;
        m_nDotDownCnt = nDownDigitCnt > m_nDotDownCnt ? nDownDigitCnt : m_nDotDownCnt;
    }
    //---------------------------------------------
    
    dCurrnetValue = dCurrnetValue + m_dUpDownValue;

    sTemp = dCurrnetValue;

    CheckMinMaxValule(sTemp);
#endif
}
//---------------------------------------------------------------------------

void __fastcall TfmNumPad_AZ::SpeedButton_DownClick(TObject *Sender)
{
#if 1

    switch(m_Mode) {
        case eMODE_NORMAL:
            m_Mode      = eMODE_MINUS;
            m_dAddValue = 0.0;
            m_sAddValue = "";

            DisplayUpdate(m_sAddValue);
            break;

        default:
            break;
    }

#else

    double dCurrnetValue = 0;
    AnsiString sTemp;
    
    if(m_sInputNum == "" || m_sInputNum == "." || m_sInputNum == "-") 
        dCurrnetValue = 0;
    else 
        dCurrnetValue = m_sInputNum.ToDouble();

    // --------------------------------------------
    // UpDown 버튼 이용시 소수점이 존재할 경우 소수점 표기 위해 . 
    double dTemp = m_dUpDownValue - (double)((int)m_dUpDownValue);
    int    nDownDigitCnt = 0;
    
    for(int i=0; i<3; i++) {
        if(dTemp > 0) {
            nDownDigitCnt++;
            dTemp = dTemp * 10.0;
            dTemp = dTemp - (double)((int)dTemp);
        }
    }

    if(nDownDigitCnt > 0) {
        m_bDotIntput = true;
        m_nDotDownCnt = nDownDigitCnt > m_nDotDownCnt ? nDownDigitCnt : m_nDotDownCnt;
    }
    //---------------------------------------------
    
    dCurrnetValue = dCurrnetValue - m_dUpDownValue;

    sTemp = dCurrnetValue;

    CheckMinMaxValule(sTemp);
#endif
}
//---------------------------------------------------------------------------

