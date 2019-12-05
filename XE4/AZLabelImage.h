//---------------------------------------------------------------------------

#ifndef AZLabelImageH
#define AZLabelImageH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
//#include <gdiplus.h>                // GDI+
#include <list>

#include "Common\\Define.h"

using namespace std;


//---------------------------------------------------------------------------
//class TMyPoint {
//public:
//    int x;
//    int y;
//
//public:
//    __fastcall TMyPoint() {
//        x = 0;
//        y = 0;
//    };
//} ;
//
//class TMyLine {
//public:
//    TMyPoint Start;
//    TMyPoint End;
//    bool     Selected;      // 현재 선택된 경우
//    bool     SelectStart;   // 선택된 상태에서 Start 부분을 클릭하여 드래그 할 경우
//    bool     SelectEnd;     // 선택된 상태에서 End 부분을 클릭하여 드래그 할 경우
//
//public:
//    __fastcall TMyLine() {
//        Selected    = false;
//        SelectStart = false;
//        SelectEnd   = false;
//    };
//
//    double   __fastcall GetLen() {
//        double x    = End.x - Start.x;
//        double y    = End.y - Start.y;
//        double len  = sqrt(x*x + y*y);
//        return len;
//    };
//
//    int     __fastcall GetLenX() {
//        int n = End.x - Start.x;
//        n = n < 0 ? -n : n;
//        return n;
//    };
//
//    int     __fastcall GetLenY() {
//        int n = End.y - Start.y;
//        n = n < 0 ? -n : n;
//        return n;
//    };
//};
////---------------------------------------------------------------------------
//class TMyLabel {
//public:
//    TMyPoint Pos;
//    bool     Selected;
//    int      Index;
//
//public:
//    __fastcall TMyLabel() {
//        Selected = false;
//        Index = 0;
//    };
//};


//---------------------------------------------------------------------------
class PACKAGE TAZLabelImage : public TImage
{
public:
    list<TMyLine>   m_listLine;
    list<TMyLabel>  m_listLabel;

private:
//    Vcl::Graphics::TBitmap * m_pBitmap;

    bool        bLoaded;
//    bool        m_bLMouseDown;
//    bool        m_bRMouseDown;
//    TMyLine     m_LineTemp;
//    TMyLabel    m_LabelTemp;
//
//    bool        m_bLineSelected;        // Line이 선택된  상태다
//    bool        m_bLineEdegSelected;    // Line Edge 선택
//    bool        m_bLabelSelected;       // Label이 선택된 상태다
//
//
//
//
//    int         FLabelSize;
//    int         FLabelLastIndex;
//    int         FLabelStyle;           // 0 : Round Rec, 1 : Rec, 2 : Circle
//    int         FLineStyle;            // 0 : 직선, 1 : 긴 점선, 2 : 작은 점선 3 : 긴/작은 점선
//    bool        FLabelMagnetic;        // Object를 10 픽셀 단위로 자석모드로 이동 한다.
//
//    void        __fastcall SetLabelSize(int n);
//    void        __fastcall SetLabelLastIndex(int n);
//    void        __fastcall SetLabelStyle(int n);         
//    void        __fastcall SetLineStyle(int n);          
//    void        __fastcall SetLabelMagnetic(bool b);     



//    void __fastcall DrawMyLine();
//    void __fastcall DrawLabel();
//    void __fastcall DrawLabel(int n, int nX, int nY, bool bSelected = false);
//    void __fastcall DrawOverlayText();
//
//    bool __fastcall FindSelectLabel(int X, int Y);
//    bool __fastcall MoveSelectLabel(int X, int Y);
//    bool __fastcall IsSelectLabel();
//    void __fastcall ResetSelectLabel();
//    bool __fastcall DeleteSelectLabel();
//
//    bool __fastcall FindSelectLine(int X, int Y);
//    bool __fastcall MoveSelectLine(int dX, int dY);
//    bool __fastcall MoveSelectLineEdge(int dX, int dY, bool bShiftKeyDown);
//    bool __fastcall MoveSelectLineEdgeShift(int X, int Y, bool bShift);
//    bool __fastcall IsSelectLine();
//    bool __fastcall FindSelectLineEdge(int X, int Y);
//    void __fastcall ResetSelectLine();
//    void __fastcall ResetSelectLineEdge();
//    bool __fastcall DeleteSelectLine();


//	TMouseEvent                     FOnMouseDown;
//	TMouseMoveEvent                 FOnMouseMove;
//	TMouseEvent                     FOnMouseUp;
//	System::Classes::TNotifyEvent   FOnDblClick;
    
//    void __fastcall MyMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
//    void __fastcall MyMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
//    void __fastcall MyMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
//    void __fastcall MyDblClick(TObject *Sender);

    void __fastcall OnPicktureChanged(System::TObject* Sender);

protected:
//    virtual void    __fastcall WndProc(TMessage& Message);          ///< WndProc 함수 오버라이드.
    virtual void    __fastcall Loaded(void);                        ///< Loaded 함수 오버라이드.

    
public:
            __fastcall  TAZLabelImage(TComponent* Owner);
    virtual __fastcall ~TAZLabelImage();

    //Vcl::Graphics::TBitmap * GetOrgPictureImage() { return m_pBitmap; };
    
__published:


//    int         ;
//    int         ;
//    int         ;           // 0 : Round Rec, 1 : Rec, 2 : Circle
//    int         ;            // 0 : 직선, 1 : 긴 점선, 2 : 작은 점선 3 : 긴/작은 점선
//    bool        ;        // Object를 10 픽셀 단위로 자석모드로 이동 한다.
//
//    void        __fastcall (int n);
//    void        __fastcall (int n);
//    void        __fastcall (int n);         
//    void        __fastcall (int n);          
//    void        __fastcall (bool b);     


//    __property int                 LabelSize        = { read = FLabelSize,              write = SetLabelSize,           default = 20        };
//    __property int                 LabelLastIndex   = { read = FLabelLastIndex,         write = SetLabelLastIndex,      default = 0         };
//    __property int                 LabelStyle       = { read = FLabelStyle,             write = SetLabelStyle,          default = 0         };
//    __property bool                LabelMagnetic    = { read = FLabelMagnetic,          write = SetLabelMagnetic,       default = true      };
//    __property int                 LineStyle        = { read = FLineStyle,              write = SetLineStyle,           default = 0         };
//
//
//    // 재 지정한 Event ---------------------------
//    __property TMouseEvent                      OnMouseDown     = { read = FOnMouseDown, write = FOnMouseDown    };
//    __property TMouseEvent                      OnMouseUp       = { read = FOnMouseUp,   write = FOnMouseUp      };
//    __property TMouseMoveEvent                  OnMouseMove     = { read = FOnMouseMove, write = FOnMouseMove    };
//    __property System::Classes::TNotifyEvent    OnDblClick      = { read = FOnDblClick,  write = FOnDblClick     };
};
//---------------------------------------------------------------------------
#endif
