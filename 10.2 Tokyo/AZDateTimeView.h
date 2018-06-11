//---------------------------------------------------------------------------

#ifndef AZDateTimeViewH
#define AZDateTimeViewH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Vcl.Controls.hpp>
//---------------------------------------------------------------------------

class PACKAGE TAZDateTimeView : public TGraphicControl
{
private:
    TDateTime           FDateTime;
    String              FFormat;
    bool                FTransparent;
    System::Classes::TAlignment FAlignment;
    bool                FOutLineVisible;
    TColor              FOutLineColor;
    int                 FSpace;
    int                 FRound;
    int                 FTimerInterval;
    bool                FTimerEnabled;
    TNotifyEvent        FOnTimer; 
    

    void __fastcall SetDateTime(TDateTime d);
    void __fastcall SetFormat(String s);
    void __fastcall SetTransparent(bool b);
    void __fastcall SetAlignment(System::Classes::TAlignment a);
    void __fastcall SetOutLine(bool b);
    void __fastcall SetOutLineColor(TColor c);
    void __fastcall SetSpace(int n);
    void __fastcall SetRound(int n);
    void __fastcall SetTimerInterval(int n);
    void __fastcall SetTimerEnabled(bool b);
    
    
    Graphics::TBitmap* tmpBitmap1;
    TTimer *        pTimer;
    void __fastcall MyOnTimer(TObject *Sender);
    void __fastcall SetOnTimer(System::Classes::TNotifyEvent Value);
    
protected:
    void    __fastcall Paint();
    void    __fastcall Loaded(void);

public:
                __fastcall TAZDateTimeView(TComponent* Owner);
    virtual     __fastcall ~TAZDateTimeView();



__published:

    __property Width                    = { default = 170 };
    __property Height                   = { default = 28 };
    __property Tag;
    __property Left;
    __property Top;
    __property Align;
    __property Font;
    __property Enabled;
    __property Visible; 
    __property Color                    = { default = clWhite } ;

    __property TDateTime DateTime       = { read = FDateTime,       write = SetDateTime };
    __property String    Format         = { read = FFormat,         write = SetFormat   };
    __property bool      Transparent    = { read = FTransparent,    write = SetTransparent,     default = false };
    __property bool      OutLine        = { read = FOutLineVisible, write = SetOutLine,         default = true };
    __property TColor    OutLineColor   = { read = FOutLineColor,   write = SetOutLineColor,    default = clHotLight };
    __property int       Space          = { read = FSpace,          write = SetSpace,           default = 0 };
    __property int       Round          = { read = FRound,          write = SetRound,           default = 0 };
    __property System::Classes::TAlignment Alignment    = { read = FAlignment,  write = SetAlignment,   default = taRightJustify };

    __property int       TimerInterval  = { read = FTimerInterval,  write = SetTimerInterval,   default = 1000  }; 
    __property bool      TimerEnabled   = { read = FTimerEnabled,   write = SetTimerEnabled,    default = false };

//----------------------------
    // EVENT
    __property OnResize;
    __property OnClick;
    __property OnMouseDown;
    __property OnMouseLeave;
    __property OnMouseUp;

    __property System::Classes::TNotifyEvent OnTimer = {read=FOnTimer, write=SetOnTimer};
    
};
//---------------------------------------------------------------------------
#endif
