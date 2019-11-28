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
    m_pBitmap = new Vcl::Graphics::TBitmap;
    
    bLoaded             = false;

    m_bLMouseDown       = false;
    m_bRMouseDown       = false;
    m_bLineSelected     = false;
    m_bLabelSelected    = false;
    m_bLineEdegSelected = false;
    
    FLabelMagnetic      = true;
    FLabelLastIndex     = 0;
    FLabelSize          = 20;
    FLabelStyle         = 0;
    FLineStyle          = 0;

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
    if(m_pBitmap) delete m_pBitmap;
    m_listLine.clear();
    m_listLabel.clear();

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

void __fastcall TAZLabelImage::WndProc(TMessage& Message)
{
    switch(Message.Msg) {
         case WM_LBUTTONDOWN:
            if(ComponentState.Contains(csDesigning) != true) { // 

            }
            break;

         case WM_LBUTTONUP:
            if(ComponentState.Contains(csDesigning) != true) { //
            
            }
            break;

         case WM_MOUSEHOVER:
            break;

         case WM_MOUSELEAVE:
            if(ComponentState.Contains(csDesigning) != true) { //

            }
            break;

    }

    switch(Message.Msg) {
        default:
            TGraphicControl::WndProc(Message);
            break;
    }

}
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------

void __fastcall TAZLabelImage::DrawMyLine()
{
//    Picture->OnChange = NULL;
//
//    Picture->Bitmap->Assign(m_pBitmap);
//
//    // 외곽 사각형 테두리
//    Picture->Bitmap->Canvas->Pen->Color = clBlack;
//    Picture->Bitmap->Canvas->Pen->Width = 1;
//    Picture->Bitmap->Canvas->Brush->Style = bsClear;
//    Picture->Bitmap->Canvas->Rectangle(0, 0, Width, Height);
//
//    // Lines
//    Picture->Bitmap->Canvas->Pen->Color = clRed;
//    Picture->Bitmap->Canvas->Pen->Width = 2;
//
//
//    list<TMyLine>::iterator iter = m_listLine.begin();
//#if 0
//    while(iter != m_listLine.end()) {
//
//        Picture->Bitmap->Canvas->MoveTo(iter->Start.x, iter->Start.y);
//        Picture->Bitmap->Canvas->LineTo(iter->End.x, iter->End.y);
//
//        iter++;
//    }
//#else
//    // GDI+를 이용하여 Angi-Aliasing Line 구현
//    Gdiplus::Graphics gp(Picture->Bitmap->Canvas->Handle);
//    Gdiplus::Pen      pen(Gdiplus::Color(200, 255, 0, 0), 2);
//
//    pen.SetDashStyle(FLineStyle);
//    pen.SetStartCap(LineCapRoundAnchor);                            // 시작 부분 화살표 모양
//
//    gp.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);           // Anti-Aliasing
//
//    while(iter != m_listLine.end()) {
//       if(iter->Selected) {
//            gp.SetSmoothingMode(Gdiplus::SmoothingModeDefault);
//
//            Gdiplus::Pen            penSelected(Gdiplus::Color(255, 255, 0, 255), 5);
//            penSelected.SetDashStyle(3);
//            gp.DrawLine(&penSelected, iter->Start.x, iter->Start.y, iter->End.x, iter->End.y);
//
//            Gdiplus::Pen            penEdge(Gdiplus::Color(255, 0, 0, 0), 1);
//            Gdiplus::SolidBrush     brushEdge(Gdiplus::Color(255, 255, 255, 0));
//            Gdiplus::RectF          rectStart(iter->Start.x-3, iter->Start.y-3, 6,6);
//            Gdiplus::RectF          rectEnd(iter->End.x-3, iter->End.y-3, 6,6);
//
//            gp.FillRectangle(&brushEdge, rectStart);
//            gp.DrawRectangle(&penEdge,   rectStart);
//
//            gp.FillRectangle(&brushEdge, rectEnd);
//            gp.DrawRectangle(&penEdge,   rectEnd);
//       }
//       else {
//            gp.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
//            gp.DrawLine(&pen, iter->Start.x, iter->Start.y, iter->End.x, iter->End.y);
//       }
//
//       iter++;
//    }
//#endif
//
//    Picture->OnChange = OnPicktureChanged;

}
//---------------------------------------------------------------------------

bool __fastcall TAZLabelImage::FindSelectLabel(int X, int Y)
{

    bool bRet = false;

    list<TMyLabel>::iterator iter = m_listLabel.begin();

    while(iter != m_listLabel.end()) {

        if(X > iter->Pos.x - (FLabelSize / 2)
        && X < (iter->Pos.x + (FLabelSize / 2) - 1)
        && Y > iter->Pos.y - (FLabelSize / 2)
        && Y < (iter->Pos.y + (FLabelSize / 2) - 1)) {
            iter->Selected = true;
            bRet = true;
        }
        else {
            iter->Selected = false;
        }

       iter++;
    }

    return bRet;
}
//---------------------------------------------------------------------------

bool __fastcall TAZLabelImage::MoveSelectLabel(int X, int Y)
{

    list<TMyLabel>::iterator iter = m_listLabel.begin();

    while(iter != m_listLabel.end()) {

        if(iter->Selected) {
            iter->Pos.x = X;
            iter->Pos.y = Y;
            return true;
        }

       iter++;
    }

    return false;
}
//---------------------------------------------------------------------------

bool __fastcall TAZLabelImage::IsSelectLabel()
{

    list<TMyLabel>::iterator iter = m_listLabel.begin();

    while(iter != m_listLabel.end()) {

        if(iter->Selected) {
            return true;
        }

       iter++;
    }

    return false;
}
//---------------------------------------------------------------------------

bool __fastcall TAZLabelImage::DeleteSelectLabel()
{

    list<TMyLabel>::iterator iter = m_listLabel.begin();

    while(iter != m_listLabel.end()) {

        if(iter->Selected) {
            m_listLabel.erase(iter);
            return true;
        }

       iter++;
    }

    return false;
}
//---------------------------------------------------------------------------

void __fastcall TAZLabelImage::ResetSelectLabel()
{

    list<TMyLabel>::iterator iter = m_listLabel.begin();

    while(iter != m_listLabel.end()) {

        if(iter->Selected) {
            iter->Selected = false;
        }

       iter++;
    }

}
//---------------------------------------------------------------------------

bool __fastcall TAZLabelImage::FindSelectLine(int X, int Y)
{

    bool bRet = false;

//    list<TMyLine>::iterator iter = m_listLine.begin();
//
//    while(iter != m_listLine.end()) {
//
//		GraphicsPath path;
//        Pen strokePen(Gdiplus::Color(200, 255, 0, 0), 5);
//
//        path.AddLine(iter->Start.x, iter->Start.y, iter->End.x, iter->End.y);
//        if(path.IsOutlineVisible(X, Y, &strokePen)) {
//            iter->Selected = true;
//            bRet = true;
//        }
//        else {
//            iter->Selected = false;
//        }
//
//       iter++;
//    }

    return bRet;
}
//---------------------------------------------------------------------------

bool __fastcall TAZLabelImage::MoveSelectLine(int dX, int dY)
{
    list<TMyLine>::iterator iter = m_listLine.begin();

    while(iter != m_listLine.end()) {

        if(iter->Selected) {
            iter->Start.x += dX;
            iter->Start.y += dY;
            iter->End.x += dX;
            iter->End.y += dY;
            return true;
        }

       iter++;
    }

    return false;
}
//---------------------------------------------------------------------------

bool __fastcall TAZLabelImage::MoveSelectLineEdge(int dX, int dY, bool bShiftKeyDown)
{
    list<TMyLine>::iterator iter = m_listLine.begin();

    while(iter != m_listLine.end()) {

        if(iter->Selected) {
//            int nLenX = iter->GetLenX();
//            int nLenY = iter->GetLenY();

            if(iter->SelectStart) {
                iter->Start.x += dX;
                iter->Start.y += dY;
            }
            else if(iter->SelectEnd) {
                iter->End.x += dX;
                iter->End.y += dY;
            }

            return true;
        }

       iter++;
    }

    return false;
}
//---------------------------------------------------------------------------

bool __fastcall TAZLabelImage::MoveSelectLineEdgeShift(int X, int Y, bool bShift)
{
    list<TMyLine>::iterator iter = m_listLine.begin();

    while(iter != m_listLine.end()) {

        if(iter->Selected) {

            if(iter->SelectStart) {

                iter->Start.x = X;
                iter->Start.y = Y;

                if(bShift) {
                    int nLenX = iter->GetLenX();
                    int nLenY = iter->GetLenY();

                    if(nLenX > nLenY * 2) {
                        // Y 고정
                        iter->Start.y = iter->End.y;
                    }
                    else if(nLenY > nLenX * 2) {
                        // X고정
                        iter->Start.x = iter->End.x;;
                    }
                    else {
                        // 45도 고정
                        if(nLenX < nLenY) {
                            if(iter->End.y < Y) {
                                iter->Start.y = iter->End.y + nLenX;
                            }
                            else {
                                iter->Start.y = iter->End.y - nLenX;
                            }
                        }
                        else {
                            if(iter->End.x < X) {
                                iter->Start.x = iter->End.x + nLenY;
                            }
                            else {
                                iter->Start.x = iter->End.x - nLenY;
                            }
                        }

                    }
                }
            }
            else if(iter->SelectEnd) {
                iter->End.x = X;
                iter->End.y = Y;

                if(bShift) {
                    int nLenX = iter->GetLenX();
                    int nLenY = iter->GetLenY();

                    if(nLenX > nLenY * 2) {
                        // Y 고정
                        iter->End.y = iter->Start.y;
                    }
                    else if(nLenY > nLenX * 2) {
                        // X고정
                        iter->End.x = iter->Start.x;;
                    }
                    else {
                        // 45도 고정
                        if(nLenX < nLenY) {
                            if(iter->Start.y < Y) {
                                iter->End.y = iter->Start.y + nLenX;
                            }
                            else {
                                iter->End.y = iter->Start.y - nLenX;
                            }
                        }
                        else {
                            if(iter->Start.x < X) {
                                iter->End.x = iter->Start.x + nLenY;
                            }
                            else {
                                iter->End.x = iter->Start.x - nLenY;
                            }
                        }

                    }
                }
            }

            return true;
        }

       iter++;
    }

    return false;
}
//---------------------------------------------------------------------------

bool __fastcall TAZLabelImage::FindSelectLineEdge(int X, int Y)
{
    list<TMyLine>::iterator iter = m_listLine.begin();

    while(iter != m_listLine.end()) {

        if(iter->Selected) {
            if(iter->Start.x-5 < X
            && iter->Start.x+5 > X
            && iter->Start.y-5 < Y
            && iter->Start.y+5 > Y) {
                iter->SelectStart = true;
                return true;
            }
            else if(iter->End.x-5 < X
            && iter->End.x+5 > X
            && iter->End.y-5 < Y
            && iter->End.y+5 > Y) {
                iter->SelectEnd = true;
                return true;
            }

        }

       iter++;
    }

    return false;
}
//---------------------------------------------------------------------------

bool __fastcall TAZLabelImage::IsSelectLine()
{

    list<TMyLine>::iterator iter = m_listLine.begin();

    while(iter != m_listLine.end()) {

        if(iter->Selected) {
            return true;
        }

       iter++;
    }

    return false;
}

//---------------------------------------------------------------------------

void __fastcall TAZLabelImage::ResetSelectLine()
{

    list<TMyLine>::iterator iter = m_listLine.begin();

    while(iter != m_listLine.end()) {

        if(iter->Selected) {
            iter->Selected = false;
        }

       iter++;
    }

}
//---------------------------------------------------------------------------

void __fastcall TAZLabelImage::ResetSelectLineEdge()
{

    list<TMyLine>::iterator iter = m_listLine.begin();

    while(iter != m_listLine.end()) {

        if(iter->Selected) {
            iter->SelectStart = false;
            iter->SelectEnd   = false;
        }

       iter++;
    }

}
//---------------------------------------------------------------------------

bool __fastcall TAZLabelImage::DeleteSelectLine()
{

    list<TMyLine>::iterator iter = m_listLine.begin();

    while(iter != m_listLine.end()) {

        if(iter->Selected) {
            m_listLine.erase(iter);
            return true;
        }

       iter++;
    }

    return false;
}


//---------------------------------------------------------------------------
//void __fastcall DrawRoundRectangle(Gdiplus::Graphics* g, Pen *p, Gdiplus::Rect& rect, UINT8 radius)
//{//	if (g==NULL) return;
//	GraphicsPath path;
//
//	path.AddLine(rect.X + radius, rect.Y, rect.X + rect.Width - (radius * 2), rect.Y);
//	path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y, radius * 2, radius * 2, 270, 90);
//	path.AddLine(rect.X + rect.Width, rect.Y + radius, rect.X + rect.Width, rect.Y + rect.Height - (radius * 2));
//	path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y + rect.Height - (radius * 2), radius * 2,
//		radius * 2, 0, 90);
//	path.AddLine(rect.X + rect.Width - (radius * 2), rect.Y + rect.Height, rect.X + radius, rect.Y + rect.Height);
//	path.AddArc(rect.X, rect.Y + rect.Height - (radius * 2), radius * 2, radius * 2, 90, 90);
//	path.AddLine(rect.X, rect.Y + rect.Height - (radius * 2), rect.X, rect.Y + radius);
//	path.AddArc(rect.X, rect.Y, radius * 2, radius * 2, 180, 90);
//	path.CloseFigure();
//	g->DrawPath(p, &path);
//}

//void __fastcall FillRoundRectangle(Gdiplus::Graphics* g, Brush *p, Gdiplus::Rect& rect, UINT8 radius)
//{
//	if (g == NULL) return;
//	GraphicsPath path;
//
//	path.AddLine(rect.X + radius, rect.Y, rect.X + rect.Width - (radius * 2), rect.Y);
//	path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y, radius * 2, radius * 2, 270, 90);
//	path.AddLine(rect.X + rect.Width, rect.Y + radius, rect.X + rect.Width, rect.Y + rect.Height - (radius * 2));
//	path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y + rect.Height - (radius * 2), radius * 2, radius * 2, 0, 90);
//	path.AddLine(rect.X + rect.Width - (radius * 2), rect.Y + rect.Height, rect.X + radius, rect.Y + rect.Height);
//	path.AddArc(rect.X, rect.Y + rect.Height - (radius * 2), radius * 2, radius * 2, 90, 90);
//	path.AddLine(rect.X, rect.Y + rect.Height - (radius * 2), rect.X, rect.Y + radius);
//	path.AddArc(rect.X, rect.Y, radius * 2, radius * 2, 180, 90);
//	path.CloseFigure();
//
//	g->FillPath(p, &path);
//}

//---------------------------------------------------------------------------
void __fastcall TAZLabelImage::DrawLabel()
{
//    Picture->OnChange = NULL;
//
//    list<TMyLabel>::iterator iter = m_listLabel.begin();
//
//    int nIndex = 1;
//
//    Gdiplus::Graphics       gp(Picture->Bitmap->Canvas->Handle);
//
//    while(iter != m_listLabel.end()) {
//       DrawLabel(&gp, iter->Index, iter->Pos.x, iter->Pos.y, iter->Selected);
//       nIndex++;
//       iter++;
//    }
//
//    Picture->OnChange = OnPicktureChanged;

}
//---------------------------------------------------------------------------
void __fastcall TAZLabelImage::DrawLabel(Gdiplus::Graphics * pGP, int index, int nX, int nY, bool bSelected)
{
//    Picture->OnChange = NULL;
//
//    // GDI+를 이용하여 Angi-Aliasing 구현
//
//    Gdiplus::Pen            pen(Gdiplus::Color(255, 255, 255, 255), 2);
//    Gdiplus::SolidBrush     sBrush(Gdiplus::Color(200,100,0));
//
//    switch(FLabelStyle) {
//
//        case 0: // Round Rec
//            {
//                pGP->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
//            //    gp.FillRectangle(&sBrush, nX-(FLabelSize/2), nY-(FLabelSize/2), FLabelSize, FLabelSize);
//            //    gp.DrawRectangle(&pen, nX-(FLabelSize/2)-1, nY-(FLabelSize/2)-1, FLabelSize+1, FLabelSize+1);
//
//                Gdiplus::Rect rect(nX-(FLabelSize/2), nY-(FLabelSize/2), FLabelSize, FLabelSize);
//                Gdiplus::Rect rect_Back(nX-(FLabelSize/2), nY-(FLabelSize/2), FLabelSize+1, FLabelSize+1);
//
//                pen.SetColor(Gdiplus::Color(170,170,170));
//                DrawRoundRectangle(pGP, &pen, rect_Back, 5);
//
//                FillRoundRectangle(pGP, &sBrush, rect, 5);
//
//                pen.SetColor(Gdiplus::Color(255,255,255));
//                DrawRoundRectangle(pGP, &pen, rect, 5);
//            }
//            break;
//
//
//        case 1: // Rec
//            {
//                pGP->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
//
//                pen.SetColor(Gdiplus::Color(170,170,170));
//                pGP->DrawRectangle(&pen, nX-(FLabelSize/2)+1, nY-(FLabelSize/2)+1, FLabelSize, FLabelSize);
//
//                pGP->FillRectangle(&sBrush, nX-(FLabelSize/2), nY-(FLabelSize/2), FLabelSize, FLabelSize);
//
//                pen.SetColor(Gdiplus::Color(255,255,255));
//                pGP->DrawRectangle(&pen, nX-(FLabelSize/2), nY-(FLabelSize/2), FLabelSize, FLabelSize);
//
////                if(bSelected) {
////                    pGP->SetSmoothingMode(Gdiplus::SmoothingModeDefault);
////
////                    Gdiplus::Pen    penSelected(Gdiplus::Color(255, 0, 0, 0), 1);
////                    penSelected.SetDashStyle(1);
////                    pGP->DrawRectangle(&penSelected, nX-(FLabelSize/2)-4, nY-(FLabelSize/2)-4, FLabelSize + 8, FLabelSize + 8);
////
////                    pGP->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
////                }
//            }
//            break;
//
//        case 2: // Circle
//            {
//                pGP->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
//
//                pen.SetColor(Gdiplus::Color(170,170,170));
//                pGP->DrawEllipse(&pen, nX-(FLabelSize/2)+1, nY-(FLabelSize/2)+1, FLabelSize, FLabelSize);
//
//                pGP->FillEllipse(&sBrush, nX-(FLabelSize/2), nY-(FLabelSize/2), FLabelSize, FLabelSize);
//
//                pen.SetColor(Gdiplus::Color(255,255,255));
//                pGP->DrawEllipse(&pen, nX-(FLabelSize/2), nY-(FLabelSize/2), FLabelSize, FLabelSize);
//
////                if(bSelected) {
////                    pGP->SetSmoothingMode(Gdiplus::SmoothingModeDefault);
////
////                    Gdiplus::Pen    penSelected(Gdiplus::Color(255, 0, 0, 0), 1);
////                    penSelected.SetDashStyle(1);
////                    pGP->DrawEllipse(&penSelected, nX-(FLabelSize/2)-4, nY-(FLabelSize/2)-4, FLabelSize + 8, FLabelSize + 8);
////
////                    pGP->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
////                }
//            }
//            break;
//    }
//
//
//    if(bSelected) {
//        pGP->SetSmoothingMode(Gdiplus::SmoothingModeDefault);
//
//        Gdiplus::Pen    penSelected(Gdiplus::Color(255, 0, 0, 0), 1);
//        penSelected.SetDashStyle(1);
//        pGP->DrawRectangle(&penSelected, nX-(FLabelSize/2)-4, nY-(FLabelSize/2)-4, FLabelSize + 8, FLabelSize + 8);
//
//        penSelected.SetColor(Gdiplus::Color(255, 200, 200, 200));
//        pGP->DrawRectangle(&penSelected, nX-(FLabelSize/2)-3, nY-(FLabelSize/2)-3, FLabelSize + 6, FLabelSize + 6);
//
//        pGP->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
//    }
//
//    // Label 번호를 출력 한다.
//    //Gdiplus::Font       txtFont(L"Arial", 20, FontStyleRegular, UnitPixel);
//
//    Picture->Bitmap->Canvas->Font->Name = L"Arial";
//    Picture->Bitmap->Canvas->Font->Size = 20;
//    //Picture->Bitmap->Canvas->Font->Style = Picture->Bitmap->Canvas->Font->Style << fsBold;
//
//    Gdiplus::FontStyle  fontStyle;
//
//    if(Picture->Bitmap->Canvas->Font->Style.Contains(fsStrikeOut)) {
//        fontStyle = FontStyleStrikeout;
//    }
//    else if(Picture->Bitmap->Canvas->Font->Style.Contains(fsUnderline)) {
//        fontStyle = FontStyleUnderline;
//    }
//    else if(Picture->Bitmap->Canvas->Font->Style.Contains(fsBold)
//         && Picture->Bitmap->Canvas->Font->Style.Contains(fsItalic)) {
//        fontStyle = FontStyleBoldItalic;
//    }
//    else if(Picture->Bitmap->Canvas->Font->Style.Contains(fsItalic)) {
//        fontStyle = FontStyleItalic;
//    }
//    else if(Picture->Bitmap->Canvas->Font->Style.Contains(fsBold)) {
//        fontStyle = FontStyleBold;
//    }
//    else {
//        fontStyle = FontStyleRegular;
//    }
//
//    Gdiplus::Font       txtFont(Picture->Bitmap->Canvas->Font->Name.w_str(),
//                                Picture->Bitmap->Canvas->Font->Size,
//                                fontStyle,
//                                UnitPixel);
//
//    Gdiplus::PointF     txtPoint((float)(nX-(FLabelSize/2)), (float)(nY-(FLabelSize/2)));
//    SolidBrush          txtBrush(Gdiplus::Color(255,255,255));
//    Gdiplus::RectF      layoutRect(nX-(FLabelSize/2), nY-(FLabelSize/2), FLabelSize, FLabelSize);
//    StringFormat        SF;
//    SF.SetAlignment(StringAlignmentCenter);
//    SF.SetLineAlignment(StringAlignmentCenter);
//
//    pGP->DrawString(IntToStr(index).w_str(),-1, &txtFont, layoutRect, &SF, &txtBrush);
//
//    Picture->OnChange = OnPicktureChanged;
    
}

//---------------------------------------------------------------------------
void __fastcall TAZLabelImage::DrawOverlayText()
{
//    Picture->OnChange = NULL;
//
//    Picture->Bitmap->Canvas->Font->Color = clBlack;
//    Picture->Bitmap->Canvas->Font->Size  = 10;
//    Picture->Bitmap->Canvas->Font->Style =  Picture->Bitmap->Canvas->Font->Style >> fsBold;
//    Picture->Bitmap->Canvas->Font->Style =  Picture->Bitmap->Canvas->Font->Style >> fsItalic;
//    Picture->Bitmap->Canvas->TextOut(10, 1,  L"Line : Left Mouse Drag");
//    Picture->Bitmap->Canvas->TextOut(10, 13, L"Label : Double Click");
//    Picture->Bitmap->Canvas->TextOut(10, 25, L"Delete : Select -> Right Mouse Click");
//
//    Picture->OnChange = OnPicktureChanged;

}
//---------------------------------------------------------------------------





void __fastcall TAZLabelImage::MyMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Button == mbLeft) {

        m_bLMouseDown       = true;

        if(IsSelectLine() && FindSelectLineEdge(X, Y)) {
            m_bLineEdegSelected = true;
        }
        else if(FindSelectLabel(X, Y)) {

            ResetSelectLine();

            m_bLabelSelected    = true;
            m_bLineSelected     = false;
            m_bLineEdegSelected = false;

            DrawMyLine();
            DrawLabel();

        }
        else if(FindSelectLine(X, Y)){

            ResetSelectLabel();

            m_bLineSelected     = true;
            m_bLabelSelected    = false;
            m_bLineEdegSelected = false;

            DrawMyLine();
            DrawLabel();
        }
        else {
            ResetSelectLine();
            ResetSelectLabel();

            m_bLabelSelected    = false;
            m_bLineSelected     = false;
            m_bLineEdegSelected = false;

        }

        m_LineTemp.Start.x  = X;
        m_LineTemp.Start.y  = Y;

        m_LineTemp.End.x  = X;
        m_LineTemp.End.y  = Y;
    }
    else if(Button == mbRight) {

        // 삭제 여부 확인 팝업창 출력.
        if(m_bLabelSelected) {
            if(MessageDlg("Do you want to delete Label ?", mtConfirmation, TMsgDlgButtons() << mbYes<<mbNo, 0) == mrYes) {
                DeleteSelectLabel();

                m_bLabelSelected = false;
                m_bLMouseDown    = false;

                DrawMyLine();
                DrawLabel();
            }
        }
        else if(m_bLineSelected) {
            if(MessageDlg("Do you want to delete Line ?", mtConfirmation, TMsgDlgButtons() << mbYes<<mbNo, 0) == mrYes) {
                DeleteSelectLine();

                m_bLineSelected = false;
                m_bLMouseDown   = false;

                DrawMyLine();
                DrawLabel();
            }
        }
    }
    
    if(FOnMouseDown) {
        FOnMouseDown(Sender, Button, Shift, X, Y);
    }
}

void __fastcall TAZLabelImage::MyMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
    if(m_bLMouseDown) {

        if(m_bLineEdegSelected) {

            m_LineTemp.End.x  = X;
            m_LineTemp.End.y  = Y;

            //bool bSelected = MoveSelectLineEdge(X-m_LineTemp.Start.x, Y-m_LineTemp.Start.y, Shift.Contains(ssShift));
            MoveSelectLineEdgeShift(X, Y, Shift.Contains(ssShift));

            m_LineTemp.Start.x  = X;
            m_LineTemp.Start.y  = Y;

            m_LineTemp.End.x    = X;
            m_LineTemp.End.y    = Y;

            DrawMyLine();
            DrawLabel();

        }
        else if(m_bLabelSelected) {

            if(FLabelMagnetic) {
                int x = X % 10;
                int y = Y % 10;
                X = x < 5 ? X-x : X + (10-x);
                Y = y < 5 ? Y-y : Y + (10-y);
            }

            bool bSelected = MoveSelectLabel(X, Y);

            Gdiplus::Graphics       gp(Picture->Bitmap->Canvas->Handle);

            DrawMyLine();
            DrawLabel();

            if(!bSelected) DrawLabel(&gp, FLabelLastIndex, X, Y, true);
        }
        else if(m_bLineSelected) {

            m_LineTemp.End.x  = X;
            m_LineTemp.End.y  = Y;

            MoveSelectLine(X-m_LineTemp.Start.x, Y-m_LineTemp.Start.y);

            m_LineTemp.Start.x  = X;
            m_LineTemp.Start.y  = Y;

            m_LineTemp.End.x    = X;
            m_LineTemp.End.y    = Y;

//            // TODO: Image 영역 외부로 나갈 경우 삭제 여부 확인 팝업창 출력.
//            if(X < -10 || X > Width + 5 || Y < -10 || Y > Height + 5) {
//                if(MessageDlg("Do you delete Line ?", mtConfirmation, TMsgDlgButtons() << mbYes<<mbNo, 0) == mrYes) {
//                    DeleteSelectLine();
//
//                    m_bLineSelected = false;
//                    m_bLMouseDown   = false;
//                }
//            }

            DrawMyLine();
            DrawLabel();
        }
        else {
            Gdiplus::Graphics       gp(Picture->Bitmap->Canvas->Handle);

            DrawMyLine();
            DrawLabel();

            Picture->OnChange = NULL;
            Picture->Bitmap->Canvas->MoveTo(m_LineTemp.Start.x, m_LineTemp.Start.y);
            Picture->Bitmap->Canvas->LineTo(X, Y);
            Picture->OnChange = OnPicktureChanged;
            
        }
    }
    else if(m_bRMouseDown) {


    }

    Repaint();      // 화면 갱신 속도 향상.
    
    if(FOnMouseMove) {
        FOnMouseMove(Sender, Shift, X, Y);
    }
}

void __fastcall TAZLabelImage::MyMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(m_bLMouseDown) {
        if(m_bLineEdegSelected) {
            m_bLineEdegSelected = false;
            ResetSelectLineEdge();
        }
        else if(m_bLabelSelected) {


        }
        else if(m_bLineSelected) {

        }
        else {
            //------------------------------
            if(FLabelMagnetic) {
                int x = X % 10;
                int y = Y % 10;
                m_LabelTemp.Pos.x = x < 5 ? X-x : X + (10-x);
                m_LabelTemp.Pos.y = y < 5 ? Y-y : Y + (10-y);
            }
            else {
                m_LabelTemp.Pos.x = X;
                m_LabelTemp.Pos.y = Y;
            }

            //------------------------------
            m_LineTemp.End.x = X;
            m_LineTemp.End.y = Y;

            if(m_LineTemp.GetLen() > 10.0) {
                // 길이가 최소 10 이상은 되어야 등록 한다.
                m_listLine.push_back(m_LineTemp);
            }
        }
    }

    if(m_bRMouseDown || m_bLMouseDown) {

        DrawMyLine();

        DrawLabel();

        DrawOverlayText();
    }

    m_bRMouseDown = false;
    m_bLMouseDown = false;

    Repaint();
    
    if(FOnMouseUp) {
        FOnMouseUp(Sender, Button, Shift, X, Y);
    }
}

void __fastcall TAZLabelImage::MyDblClick(TObject *Sender)
{
    if(IsSelectLabel() != true && IsSelectLine() != true ) {
        m_LabelTemp.Index = ++FLabelLastIndex;
        m_listLabel.push_back(m_LabelTemp);
    }
    
    if(FOnDblClick) {
        FOnDblClick(Sender);
    }
}

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





void        __fastcall TAZLabelImage::SetLabelSize(int n)
{
//    if(FLabelSize != n) {
//        FLabelSize = n;
//
//        DrawMyLine();
//        DrawLabel();
//        DrawOverlayText();
//    }
}

void        __fastcall TAZLabelImage::SetLabelLastIndex(int n)
{
//    if(FLabelLastIndex != n) {
//        FLabelLastIndex = n;
//
//        DrawMyLine();
//        DrawLabel();
//        DrawOverlayText();
//    }
}

void        __fastcall TAZLabelImage::SetLabelStyle(int n)     
{
//    if(FLabelStyle != n) {
//        FLabelStyle = n;
//
//        DrawMyLine();
//        DrawLabel();
//        DrawOverlayText();
//    }
}   

void        __fastcall TAZLabelImage::SetLineStyle(int n)   
{
//    if(FLineStyle != n) {
//        FLineStyle = n;
//
//        DrawMyLine();
//        DrawLabel();
//        DrawOverlayText();
//    }
}     

void        __fastcall TAZLabelImage::SetLabelMagnetic(bool b)
{
//    if(FLabelMagnetic != b) {
//        LabelMagnetic = b;
//
//        DrawMyLine();
//        DrawLabel();
//        DrawOverlayText();
//    }
}  

