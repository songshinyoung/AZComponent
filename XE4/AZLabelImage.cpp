//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "AZLabelImage.h"
#pragma package(smart_init)

//using namespace Gdiplus;            // GDI+
//namespace gdp = Gdiplus;            // GDI+
//
//ULONG_PTR    gdpToken;              // GDI+
//Gdiplus::GdiplusStartupInput gdpSI; // GDI+

//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAZLabelImage *)
{
    new TAZLabelImage(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAZLabelImage::TAZLabelImage(TComponent* Owner)
    : TImage(Owner)
{
//    m_pBitmap = new Vcl::Graphics::TBitmap;
    
    bLoaded             = false;

//    m_bLMouseDown       = false;
//    m_bRMouseDown       = false;
//    m_bLineSelected     = false;
//    m_bLabelSelected    = false;
//    m_bLineEdegSelected = false;
//    
//    FLabelMagnetic      = true;
//    FLabelLastIndex     = 0;
//    FLabelSize          = 20;
//    FLabelStyle         = 0;
//    FLineStyle          = 0;

    //--------------------------
//    TImage::OnMouseDown     = MyMouseDown;
//    TImage::OnMouseUp       = MyMouseUp;    
//    TImage::OnMouseMove     = MyMouseMove;    
//    TImage::OnDblClick      = MyDblClick;   
//
//    Picture->OnChange       = OnPicktureChanged;

    
}
//-----------------------------------------------------------------------
__fastcall TAZLabelImage::~TAZLabelImage()
{
//    if(m_pBitmap) delete m_pBitmap;
//    m_listLine.clear();
//    m_listLabel.clear();

//    Gdiplus::GdiplusShutdown(gdpToken);
}
//---------------------------------------------------------------------------

void __fastcall TAZLabelImage::Loaded(void)
{
    TGraphicControl::Loaded();

    bLoaded = true;

//    if(Picture->Graphic->Width == 0) {
//        Picture->Bitmap->Width  = Width;
//        Picture->Bitmap->Height = Height;
//    }
//
//    m_pBitmap->Assign(Picture->Graphic);
//
//    if (Gdiplus::GdiplusStartup(&gdpToken, &gdpSI, NULL) != Gdiplus::Ok)
//    {
//        // init error.
//    }
//
//    DrawMyLine();
//    DrawLabel();
//    DrawOverlayText();
}
//---------------------------------------------------------------------------

//void __fastcall TAZLabelImage::WndProc(TMessage& Message)
//{
//    switch(Message.Msg) {
//         case WM_LBUTTONDOWN:
//            if(ComponentState.Contains(csDesigning) != true) { // 
//
//            }
//            break;
//
//         case WM_LBUTTONUP:
//            if(ComponentState.Contains(csDesigning) != true) { //
//            
//            }
//            break;
//
//         case WM_MOUSEHOVER:
//            break;
//
//         case WM_MOUSELEAVE:
//            if(ComponentState.Contains(csDesigning) != true) { //
//
//            }
//            break;
//
//    }
//
//    switch(Message.Msg) {
//        default:
//            TGraphicControl::WndProc(Message);
//            break;
//    }
//
//}
//---------------------------------------------------------------------------


void __fastcall TAZLabelImage::OnPicktureChanged(System::TObject* Sender)
{
//    m_pBitmap->Assign(Picture->Graphic);
//
//    Picture->OnChange = NULL;
//
//    DrawMyLine();
//    DrawLabel();
//    DrawOverlayText();
//
//    Picture->OnChange       = OnPicktureChanged;

    Invalidate();

}




