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

    for(int i=0; i<11; i++) m_pColorpanels[i]->Tag = i;

    //-----------------------
    // Title Property ฐüทร

    m_pTitleColorPanels[0] = Panel_Title_Color;
    m_pTitleColorPanels[1] = Panel_Title_OutLineColor;

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

void __fastcall TfmAZNumShapeEditor::Button1Click(TObject *Sender)
{
    ModalResult  = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfmAZNumShapeEditor::Button2Click(TObject *Sender)
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
        }
    }

}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::Panel_ColorValueNormalClick(TObject *Sender)
{
    m_pSelectPanel = dynamic_cast<TPanel *>(Sender);

    if(m_pSelectPanel) {
        for(int i=0; i<11; i++) m_pColorpanels[i]->BevelOuter = bvRaised;
        m_pSelectPanel->BevelOuter = bvLowered;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::ColorListBox1Click(TObject *Sender)
{
    if(m_pSelectPanel) {
        m_pSelectPanel->Color = ColorListBox1->Selected;
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
    }
}
//---------------------------------------------------------------------------

void __fastcall TfmAZNumShapeEditor::ColorListBox2DblClick(TObject *Sender)
{
    ColorDialog1->Color = ColorListBox2->Selected;
    if(ColorDialog1->Execute(Handle) == 1) {
        if(m_pTitleSelectPanel) {
            m_pTitleSelectPanel->Color = ColorDialog1->Color;
        }
    }
}
//---------------------------------------------------------------------------


