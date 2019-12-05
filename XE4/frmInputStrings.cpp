//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmInputStrings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmInputStrBox *fmInputStrBox;
//---------------------------------------------------------------------------
__fastcall TfmInputStrBox::TfmInputStrBox(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmInputStrBox::Button_FontClick(TObject *Sender)
{
    FontDialog1->Font = Memo1->Font;
    if(FontDialog1->Execute(this->Handle)) {

        Memo1->Font->Assign(FontDialog1->Font);
    }
}
//---------------------------------------------------------------------------
