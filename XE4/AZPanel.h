//---------------------------------------------------------------------------

#ifndef AZPanelH
#define AZPanelH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TAZPanel : public TPanel
{
private:
    TPanel **   FppPanel;
    int         FColCount;
    int         FRowCount;
    bool        FCustomColorUse;
    System::Classes::TStrings* FCaptions;

    int        nCreatePanelCount;
    int        __fastcall GetColCount();
    void       __fastcall SetColCount(int n);


    int        __fastcall GetRowCount();
    void       __fastcall SetRowCount(int n);

    void    __fastcall SetWidth(int value);
    int     __fastcall GetWidth();
    void    __fastcall SetHeight(int value);
    int     __fastcall GetHeight();

    void     __fastcall CreatePanel(int nR, int nC);
    void     __fastcall ResizePanel(int nR, int nC);
    void     __fastcall DeletePanel();
    TPanel* __fastcall GetSubPanel(int index_R, int Index_C);

    void    __fastcall SetCaptions(System::Classes::TStrings* p);

    void     __fastcall Notification(Classes::TComponent *AComponent, Classes::TOperation Operation);

    void     __fastcall MyResize(TObject *Sender);
    System::Classes::TNotifyEvent FOnResize;

protected:

    //virtual bool __fastcall CanResize(int &NewWidth, int &NewHeight);
    //DYNAMIC void __fastcall Resize(TObject *Sender);

    virtual void __fastcall Loaded(void);

    void __fastcall WndProc(TMessage& Message);

    void __fastcall SubPanelClick(TObject *Sender);

public:
    __fastcall TAZPanel(TComponent* Owner);
    virtual __fastcall ~TAZPanel(); // ¼Ò¸êÀÚ.

    __property TPanel * SubPanel[int index_R][int index_C] = {read=GetSubPanel};



__published:
    __property int CountCol = { read = GetColCount, write = SetColCount,     default=3     };
    __property int CountRow = { read = GetRowCount, write = SetRowCount,     default=3     };
    __property int Width    = { read = GetWidth,      write = SetWidth          };
    __property int Height   = { read = GetHeight,     write = SetHeight         };

    __property bool CustomColor = { read = FCustomColorUse, write = FCustomColorUse, default = 0};

    __property System::Classes::TStrings* Captions = {read=FCaptions, write=SetCaptions};

    __property System::Classes::TNotifyEvent OnResize = {read=FOnResize, write=FOnResize};

};
//---------------------------------------------------------------------------
#endif
