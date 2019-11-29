//---------------------------------------------------------------------------

#ifndef frmInputImageSizeH
#define frmInputImageSizeH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfmInputImageSize : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel3;
    TPanel *Panel4;
    TGroupBox *GroupBox1;
    TPanel *Panel1;
    TLabel *Label1;
    TEdit *Edit_W;
    TPanel *Panel2;
    TLabel *Label2;
    TEdit *Edit_H;
    TCheckBox *CheckBox_Const;
    TButton *Button_Ok;
    TButton *Button_Cancel;
    TEdit *Edit1;
    TEdit *Edit2;
    void __fastcall Button_OkClick(TObject *Sender);
    void __fastcall Button_CancelClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall Edit_WChange(TObject *Sender);
    void __fastcall Edit_HChange(TObject *Sender);
private:	// User declarations
    const int m_nOrgSizeWidth;
    const int m_nOrgSizeHeight;

public:		// User declarations
    __fastcall TfmInputImageSize(TComponent* Owner, int nW, int nH);

    int m_nSizeWidth;
    int m_nSizeHeight;
};
//---------------------------------------------------------------------------
extern PACKAGE TfmInputImageSize *fmInputImageSize;
//---------------------------------------------------------------------------
#endif
