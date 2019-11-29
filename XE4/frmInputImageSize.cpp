//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmInputImageSize.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmInputImageSize *fmInputImageSize;
//---------------------------------------------------------------------------
__fastcall TfmInputImageSize::TfmInputImageSize(TComponent* Owner, int nW, int nH)
    : m_nOrgSizeWidth(nW), m_nOrgSizeHeight(nH), TForm(Owner)
{
    m_nSizeWidth  = nW;
    m_nSizeHeight = nH;

}
//---------------------------------------------------------------------------
void __fastcall TfmInputImageSize::Button_OkClick(TObject *Sender)
{
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfmInputImageSize::Button_CancelClick(TObject *Sender)
{
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfmInputImageSize::FormCreate(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TfmInputImageSize::FormShow(TObject *Sender)
{
    Edit_W->Text = m_nSizeWidth;
    Edit_H->Text = m_nSizeHeight;
}
//---------------------------------------------------------------------------
void __fastcall TfmInputImageSize::FormClose(TObject *Sender, TCloseAction &Action)

{
//
}
//---------------------------------------------------------------------------
void __fastcall TfmInputImageSize::FormDestroy(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TfmInputImageSize::Edit_WChange(TObject *Sender)
{
    m_nSizeWidth = Edit_W->Text.ToIntDef(0);

    if(CheckBox_Const->Checked) {
        m_nSizeHeight =  (double)m_nOrgSizeHeight * (double)m_nSizeWidth / (double)m_nOrgSizeWidth;
        Edit_H->OnChange    = NULL;
        Edit_H->Text        = m_nSizeHeight;
        Edit_H->OnChange    = Edit_HChange;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfmInputImageSize::Edit_HChange(TObject *Sender)
{
    m_nSizeHeight = Edit_H->Text.ToIntDef(0);

    if(CheckBox_Const->Checked) {
        m_nSizeWidth =  (double)m_nOrgSizeWidth * (double)m_nSizeHeight / (double)m_nOrgSizeHeight;
        Edit_W->OnChange    = NULL;
        Edit_W->Text        = m_nSizeWidth;
        Edit_W->OnChange    = Edit_WChange;
    }
}
//---------------------------------------------------------------------------
