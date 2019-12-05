//---------------------------------------------------------------------------

#ifndef frmInputStringsH
#define frmInputStringsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TfmInputStrBox : public TForm
{
__published:	// IDE-managed Components
    TMemo *Memo1;
    TPanel *Panel1;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TButton *Button_Font;
    TFontDialog *FontDialog1;
    void __fastcall Button_FontClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfmInputStrBox(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmInputStrBox *fmInputStrBox;
//---------------------------------------------------------------------------
#endif
