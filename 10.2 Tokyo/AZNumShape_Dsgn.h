//---------------------------------------------------------------------------

#ifndef AZNumShape_DsgnH
#define AZNumShape_DsgnH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>



//---------------------------------------------------------------------------
class TfmAZNumShapeEditor : public TForm
{
__published:    // IDE-managed Components
    TPanel *Panel25;
    TButton *Button2;
    TButton *Button1;
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TPanel *Panel1;
    TColorListBox *ColorListBox1;
    TPanel *Panel9;
    TPanel *Panel2;
    TPanel *Panel7;
    TPanel *Panel8;
    TPanel *Panel10;
    TPanel *Panel15;
    TPanel *Panel3;
    TPanel *Panel6;
    TPanel *Panel_ColorValueNormal;
    TPanel *Panel_ColorValueSelect;
    TPanel *Panel_ColorValueChanged;
    TPanel *Panel16;
    TPanel *Panel17;
    TPanel *Panel_ColorBGSelect;
    TPanel *Panel_ColorBGNormal;
    TPanel *Panel4;
    TPanel *Panel11;
    TPanel *Panel_ColorOldValueSelect;
    TPanel *Panel_ColorOldValueNormal;
    TPanel *Panel5;
    TPanel *Panel12;
    TPanel *Panel_ColorUnitSelect;
    TPanel *Panel_ColorUnitNormal;
    TPanel *Panel13;
    TPanel *Panel14;
    TPanel *Panel_ColorLineSelect;
    TPanel *Panel_ColorLineNormal;
    TPanel *Panel18;
    TPanel *Panel19;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TPanel *Panel20;
    TEdit *Edit_MaxValue;
    TEdit *Edit_MinValue;
    TEdit *Edit_UpDownValue;
    TCheckBox *CheckBox_ReadOnly;
    TPanel *Panel21;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TPanel *Panel22;
    TRadioGroup *RadioGroup_Valuetype;
    TEdit *Edit_DecimalPlaces;
    TEdit *Edit_OutlineRound;
    TEdit *Edit_Space;
    TPanel *Panel23;
    TLabel *Label_UnitWidth;
    TLabel *Label7;
    TPanel *Panel24;
    TEdit *Edit_UnitWidth;
    TCheckBox *CheckBox_UnitVisible;
    TEdit *Edit_Unit;
    TPanel *Panel26;
    TCheckBox *CheckBox_OldValueVisible;
    TColorDialog *ColorDialog1;
    TPanel *Panel27;
    TPanel *Panel28;
    TLabel *Label8;
    TCheckBox *CheckBox_Title_Visible;
    TLabel *Label9;
    TEdit *Edit_Title_Caption;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
    TEdit *Edit_Title_Round;
    TEdit *Edit_Title_Space;
    TCheckBox *CheckBox_Title_Transparent;
    TCheckBox *CheckBox_Title_OutLineVisible;
    TLabel *Label14;
    TLabel *Label15;
    TComboBox *ComboBox_Title_Align;
    TComboBox *ComboBox_Title_Alignment;
    TLabel *Label16;
    TLabel *Label17;
    TPanel *Panel_Title_Color;
    TPanel *Panel_Title_OutLineColor;
    TColorListBox *ColorListBox2;
    TBevel *Bevel2;
    TPanel *Panel29;
    TPanel *Panel30;
    TPanel *Panel31;
    TLabel *Label18;
    TLabel *Label19;
    TEdit *Edit_EditWidth;
    TEdit *Edit_EditHeight;
    TLabel *Label20;
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall ColorListBox1DblClick(TObject *Sender);
    void __fastcall Panel_ColorValueNormalClick(TObject *Sender);
    void __fastcall ColorListBox1Click(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall Panel_Title_ColorClick(TObject *Sender);
    void __fastcall ColorListBox2Click(TObject *Sender);
    void __fastcall ColorListBox2DblClick(TObject *Sender);
private:    // User declarations
    TPanel * m_pSelectPanel;
    TPanel * m_pColorpanels[11];

    //--------------------------
    TPanel * m_pTitleSelectPanel;
    TPanel * m_pTitleColorPanels[2];

public:        // User declarations
    __fastcall TfmAZNumShapeEditor(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmAZNumShapeEditor *fmAZNumShapeEditor;
//---------------------------------------------------------------------------
#endif
