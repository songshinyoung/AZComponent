//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AZNumShape_Dsgn.h"
#include "AZNumShape.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmAZNumShapeEditor *fmAZNumShapeEditor;



//===========================================================================

__fastcall TfmAZNumShapeEditor::TfmAZNumShapeEditor(TComponent* Owner)
    : TForm(Owner)
{
    m_pSelectPanel = NULL;
    m_pTitleSelectPanel = NULL;
    ZeroMemory(m_pColorpanels, sizeof(m_pColorpanels));
	ZeroMemory(m_pTitleColorPanels, sizeof(m_pTitleColorPanels));
	ZeroMemory(m_pPanelTitleAlgin, sizeof(m_pPanelTitleAlgin));
}
//---------------------------------------------------------------------------
void __fastcall TfmAZNumShapeEditor::FormCreate(TObject *Sender)
{
    m_pColorpanels[0] = Panel_ColorValueNormal;
    m_pColorpanels[1] = Panel_ColorValueSelect;
    m_pColorpanels[2] = Panel_ColorValueChanged;

	m_pColorpanels[3] = Panel_ColorBGNormal;
    m_pColorpanels[4] = Panel_ColorBGSelect;

	m_pColorpanels[5] = Panel_ColorOldValueNormal;
    m_pColorpanels[6] = Panel_ColorOldValueSelect;

    m_pColorpanels[7] = Panel_ColorUnitNormal;
    m_pColorpanels[8] = Panel_ColorUnitSelect;

	m_pColorpanels[9] = Panel_ColorLineNormal;
	m_pColorpanels[10] = Panel_ColorLineSelect;

	m_pColorpanels[11] = Panel_ColorValueShadow;
	m_pColorpanels[12] = Panel_ColorValueShadow_Select;

	for(int i=0; i<13; i++) m_pColorpanels[i]->Tag = i;

    //-----------------------
    // Title Property 관련

    m_pTitleColorPanels[0] = Panel_Title_Color;
	m_pTitleColorPanels[1] = Panel_Title_OutLineColor;


	m_pPanelTitleAlgin[0] = Panel_Title_Align_Left;
	m_pPanelTitleAlgin[1] = Panel_Title_Align_Right;
	m_pPanelTitleAlgin[2] = Panel_Title_Align_Top;
	m_pPanelTitleAlgin[3] = Panel_Title_Align_Btm;

	for(int i=0; i<4; i++) {
    	if(m_pPanelTitleAlgin[i]) m_pPanelTitleAlgin[i]->Tag = i;
	}


    PageControl1->ActivePageIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfmAZNumShapeEditor::FormShow(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::FormDestroy(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::Button_OkClick(TObject *Sender)
{
	ModalResult  = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfmAZNumShapeEditor::Button_CancelClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfmAZNumShapeEditor::ColorListBox1DblClick(TObject *Sender)
{
	ColorDialog1->Color = ColorListBox1->Selected;
    if(ColorDialog1->Execute(Handle) == 1) {
        if(m_pSelectPanel) {
			m_pSelectPanel->Color = ColorDialog1->Color;

			if(m_pSelectPanel->Name == "Panel_ColorBGNormal") {
				Panel_UnitFont->Color = ColorListBox1->Selected;
				Panel_PreView->Color  = ColorListBox1->Selected;
			}
			else if(m_pSelectPanel->Name == "Panel_ColorValueNormal") {
				Panel_PreView->Font->Color  = ColorListBox1->Selected;
			}
        }
    }

}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::Panel_ColorValueNormalClick(TObject *Sender)
{
    m_pSelectPanel = dynamic_cast<TPanel *>(Sender);

    if(m_pSelectPanel) {
		for(int i=0; i<13; i++) {
			m_pColorpanels[i]->BevelKind  = bkNone;
			m_pColorpanels[i]->BevelOuter = bvNone;
			m_pColorpanels[i]->Font->Style = TFontStyles() >> fsBold;
		}

		m_pSelectPanel->BevelKind 	= bkFlat;
		m_pSelectPanel->BevelOuter 	= bvLowered;
		m_pSelectPanel->Font->Style = TFontStyles() << fsBold;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::ColorListBox1Click(TObject *Sender)
{
    if(m_pSelectPanel) {
		m_pSelectPanel->Color = ColorListBox1->Selected;

		if(m_pSelectPanel->Name == "Panel_ColorBGNormal") {
			Panel_UnitFont->Color = ColorListBox1->Selected;
			Panel_PreView->Color  = ColorListBox1->Selected;
		}
		else if(m_pSelectPanel->Name == "Panel_ColorValueNormal") {
			Panel_PreView->Font->Color  = ColorListBox1->Selected;
		}
	}
}
//---------------------------------------------------------------------------



void __fastcall TfmAZNumShapeEditor::Panel_Title_ColorClick(TObject *Sender)
{
    m_pTitleSelectPanel = dynamic_cast<TPanel *>(Sender);

    if(m_pTitleSelectPanel) {
        for(int i=0; i<2; i++) m_pTitleColorPanels[i]->BevelOuter = bvRaised;
		m_pTitleSelectPanel->BevelOuter = bvLowered;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::ColorListBox2Click(TObject *Sender)
{
	if(m_pTitleSelectPanel) {
		m_pTitleSelectPanel->Color = ColorListBox2->Selected;
		DisplayTitle();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::ColorListBox2DblClick(TObject *Sender)
{
	ColorDialog1->Color = ColorListBox2->Selected;
    if(ColorDialog1->Execute(Handle) == 1) {
        if(m_pTitleSelectPanel) {
			m_pTitleSelectPanel->Color = ColorDialog1->Color;
			DisplayTitle();
        }
    }
}
//---------------------------------------------------------------------------


void __fastcall TfmAZNumShapeEditor::FormKeyPress(TObject *Sender, System::WideChar &Key)

{
	switch(Key) {
		case  VK_ESCAPE:
			Key = 0;
			ModalResult = mrCancel;
			break;

		case VK_RETURN:
			Key = 0;
			ModalResult  = mrOk;
			break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::BitBtn_UnitFontClick(TObject *Sender)
{
	FontDialog1->Font = Panel_UnitFont->Font;

	if(FontDialog1->Execute(this->Handle)) {
		Panel_UnitFont->Font 	= FontDialog1->Font;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::Edit_UnitChange(TObject *Sender)
{
	Panel_UnitFont->Caption = Edit_Unit->Text;
}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::Panel_UnitFontClick(TObject *Sender)
{
	BitBtn_UnitFontClick(BitBtn_UnitFont);
}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::Edit_UpDecimalsChange(TObject *Sender)
{
	AnsiString sUpValue 	= Edit_UpDecimals->Text;
	AnsiString sDownValue 	= Edit_DecimalPlaces->Text;

	sUpValue 		= sUpValue.Trim();
	sDownValue 		= sDownValue.Trim();

	int nUpValue 	= sUpValue.ToIntDef(0);
	int nDownValue 	= sDownValue.ToIntDef(0);

	// 정수
	AnsiString sMsg;
	for(int i=0; i<nUpValue; i++) {
		sMsg = sMsg + "0";
	}

	// Float
	if(RadioGroup_Valuetype->ItemIndex == 1) {
		sMsg = sMsg + ".";
		for(int i=0; i<nDownValue; i++) {
			sMsg = sMsg + "0";
		}
	}

	Panel_PreView->Caption 		= sMsg;
	Panel_PreView->Color 		= Panel_ColorBGNormal->Color;
	Panel_PreView->Font->Color 	= Panel_ColorValueNormal->Color;
}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::RadioGroup_ValuetypeClick(TObject *Sender)
{
	Edit_UpDecimalsChange(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::PageControl1Change(TObject *Sender)
{
	switch(PageControl1->ActivePageIndex) {
		case 0:
			Panel_PreView->Visible = true;
			break;

		case 1:
			Panel_Caption_ValuePreview->Color 	= Panel_PreView->Color;
			Panel_Caption_ValuePreview->Caption = Panel_PreView->Caption;
			Panel_Caption_ValuePreview->Font	= Panel_PreView->Font;
			Panel_PreView->Visible = false;

			DisplayTitle(true);
			break;
	}

}
//---------------------------------------------------------------------------
void __fastcall TfmAZNumShapeEditor::DisplayTitle(bool UpdateAll)
{
	for(int i=0; i<4; i++) {
		if(m_pPanelTitleAlgin[i]) {
			m_pPanelTitleAlgin[i]->Visible = CheckBox_Title_Visible->Checked;
		}
	}

	if(UpdateAll) {
		for(int i=0; i<4; i++) {
			if(m_pPanelTitleAlgin[i]) {
				if(m_pPanelTitleAlgin[i]->Tag != ComboBox_Title_Align->ItemIndex) {
					m_pPanelTitleAlgin[i]->Caption 		= "";
					m_pPanelTitleAlgin[i]->Color 		= clBtnFace;
					m_pPanelTitleAlgin[i]->BevelOuter 	= bvRaised;
					m_pPanelTitleAlgin[i]->BevelKind  	= bkNone;
				}
			}
		}
	}

	int nSelectIndex = ComboBox_Title_Align->ItemIndex;
	nSelectIndex = nSelectIndex < 0 ? 0 : nSelectIndex;
	nSelectIndex = nSelectIndex >= 4 ? 0 : nSelectIndex;

	TPanel * pPanel = m_pPanelTitleAlgin[nSelectIndex];

	pPanel->Caption 		= Edit_Title_Caption->Text;
	pPanel->Color 			= CheckBox_Title_Transparent->Checked ? clBtnFace : Panel_Title_Color->Color;
	pPanel->BevelOuter 		= bvNone;
	pPanel->BevelKind  		= CheckBox_Title_OutLineVisible->Checked ? bkFlat : bkNone;
	pPanel->Font			= BitBtn_TitleFont->Font;

	switch(ComboBox_Title_Alignment->ItemIndex) {
		case 0: pPanel->Alignment  = taLeftJustify; 	break;
		case 1: pPanel->Alignment  = taRightJustify; 	break;
		case 2: pPanel->Alignment  = taCenter; 			break;
	}

}

void __fastcall TfmAZNumShapeEditor::BitBtn_TitleFontClick(TObject *Sender)
{
	FontDialog1->Font = BitBtn_TitleFont->Font;

	if(FontDialog1->Execute(this->Handle)) {
		BitBtn_TitleFont->Font 	= FontDialog1->Font;
		DisplayTitle();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::ComboBox_Title_AlignChange(TObject *Sender)
{
	DisplayTitle(true);
}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::ComboBox_Title_AlignmentChange(TObject *Sender)
{
	DisplayTitle();
}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::CheckBox_Title_TransparentClick(TObject *Sender)
{
	DisplayTitle();
}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::CheckBox_Title_OutLineVisibleClick(TObject *Sender)
{
	DisplayTitle();
}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::Edit_Title_CaptionChange(TObject *Sender)
{
	DisplayTitle();
}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::Panel_Title_Align_LeftClick(TObject *Sender)
{
	TPanel * pPanel = dynamic_cast<TPanel *>(Sender);
	if(pPanel == NULL) return;

	int  nIndex = pPanel->Tag;

	ComboBox_Title_Align->ItemIndex = nIndex;

	DisplayTitle(true);

}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::CheckBox_Title_VisibleClick(TObject *Sender)
{
	DisplayTitle(true);
}
//---------------------------------------------------------------------------

