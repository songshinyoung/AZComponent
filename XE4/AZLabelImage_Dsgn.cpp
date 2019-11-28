//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AZLabelImage_Dsgn.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#pragma link "gdiplus.lib"

using namespace Gdiplus;            // GDI+
namespace gdp = Gdiplus;            // GDI+

ULONG_PTR    gdpToken;              // GDI+
Gdiplus::GdiplusStartupInput gdpSI; // GDI+

TfmAZLabelImageEditor *fmAZLabelImageEditor;
//---------------------------------------------------------------------------
__fastcall TfmAZLabelImageEditor::TfmAZLabelImageEditor(TComponent* Owner)
    : TForm(Owner)
{
    Init();

    m_pBitmap           = new Vcl::Graphics::TBitmap;
}
//---------------------------------------------------------------------------
__fastcall TfmAZLabelImageEditor::TfmAZLabelImageEditor(TComponent* Owner, Vcl::Graphics::TBitmap * pBitmap, int nW, int nH)
    : TForm(Owner)
{
    Init();

    m_ParentWidth  = nW;
    m_ParentHeight = nH;

    m_pBitmap           = new Vcl::Graphics::TBitmap;
    if(m_pBitmap) m_pBitmap->Assign(pBitmap);
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::Init()
{
    m_pBitmap           = NULL;

    m_bLMouseDown       = false;
    m_bRMouseDown       = false;

    m_LabelMagnetic     = true;
    m_LabelLastIndex    = 1;

    m_bLineSelected     = false;
    m_bLabelSelected    = false;
    m_bLineEdegSelected = false;

    m_LabelSize         = 25;
    m_LabelOuterSize    = 3;

    m_LabelStyle        = 0;
    m_LineStyle         = 0;
    m_LineWidth         = 2;

    m_LineStartCapStyle = 4;
    m_LineEndCapStyle   = 1;

    m_LabelBaseColor    = Gdiplus::Color(200, 100, 0);    // 라벨 안쪽 배경 색상
    m_LabelOutRecColor  = Gdiplus::Color(255, 255, 255);  // 라벨 외곽 색상.
    m_LineColor         = Gdiplus::Color(255, 0,   0);    // Line Color

    m_FrameView         = true;
    m_FrameWidth        = 1;
    m_FrameColor        = clBlack;
}
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::FormDestroy(TObject *Sender)
{
    if(m_pBitmap) delete m_pBitmap;
    m_listLine.clear();
    m_listLabel.clear();

    Gdiplus::GdiplusShutdown(gdpToken);
}
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::FormCreate(TObject *Sender)
{
//    Image1->Picture->Bitmap->Width  = Image1->Width;
//    Image1->Picture->Bitmap->Height = Image1->Height;

//    m_pBitmap->Assign(Image1->Picture->Graphic);

    Image1->Picture->Bitmap->Assign(m_pBitmap);

    Image1->Picture->Bitmap->Canvas->Font->Name  = L"Arial";
    Image1->Picture->Bitmap->Canvas->Font->Style = Image1->Picture->Bitmap->Canvas->Font->Style << fsBold;
    Image1->Picture->Bitmap->Canvas->Font->Size  = 16;
    Image1->Picture->Bitmap->Canvas->Font->Color = clWhite;

    if (Gdiplus::GdiplusStartup(&gdpToken, &gdpSI, NULL) != Gdiplus::Ok)
    {
        // init error.
    }

}
//---------------------------------------------------------------------------
// MOUSE DOWN ///////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{

    if(Button == mbLeft) {

        m_bLMouseDown       = true;

        if(IsSelectLine() && FindSelectLineEdge(X, Y)) {
            m_bLineEdegSelected = true;
            Caption = "Select Edge";
        }
        else if(FindSelectLabel(X, Y)) {

            ResetSelectLine();

            m_bLabelSelected    = true;
            m_bLineSelected     = false;
            m_bLineEdegSelected = false;

            Caption = "Select Label";

            DrawMyLine();
            DrawLabel();

        }
        else if(FindSelectLine(X, Y)){

            ResetSelectLabel();

            m_bLineSelected     = true;
            m_bLabelSelected    = false;
            m_bLineEdegSelected = false;

            Caption = "Select Line";

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
}
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::Image1MouseLeave(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
// MOUSE MOVE ///////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::Image1MouseMove(TObject *Sender, TShiftState Shift, int X,  int Y)
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

            if(m_LabelMagnetic) {
                int x = X % 10;
                int y = Y % 10;
                X = x < 5 ? X-x : X + (10-x);
                Y = y < 5 ? Y-y : Y + (10-y);
            }

            bool bSelected = MoveSelectLabel(X, Y);

            Gdiplus::Graphics       gp(Image1->Picture->Bitmap->Canvas->Handle);

            DrawMyLine();
            DrawLabel();

            if(!bSelected) DrawLabel(&gp, m_LabelLastIndex, X, Y, true);
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
//            if(X < -10 || X > Image1->Width + 5 || Y < -10 || Y > Image1->Height + 5) {
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
            Gdiplus::Graphics       gp(Image1->Picture->Bitmap->Canvas->Handle);

            DrawMyLine();
            DrawLabel();

            Image1->Picture->Bitmap->Canvas->MoveTo(m_LineTemp.Start.x, m_LineTemp.Start.y);
            Image1->Picture->Bitmap->Canvas->LineTo(X, Y);
        }

        Image1->Repaint();      // 화면 갱신 속도 향상.
    }
    else if(m_bRMouseDown) {


    }

    StatusBar1->Panels->Items[0]->Text = "X : " + IntToStr(X) + ", Y : " + IntToStr(Y);
}
//---------------------------------------------------------------------------
// MOUSE UP ///////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
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
            if(m_LabelMagnetic) {
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

    Image1->Repaint();

}
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::Image1DblClick(TObject *Sender)
{
    if(IsSelectLabel() != true && IsSelectLine() != true ) {
        m_LabelTemp.Index = m_LabelLastIndex;
        m_LabelLastIndex += 1;
        m_listLabel.push_back(m_LabelTemp);

        DisplaySettings();
    }
}
//---------------------------------------------------------------------------
// LINE 그리기  ////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::DrawMyLine()
{

    Image1->Picture->Bitmap->Assign(m_pBitmap);

    // 외곽 사각형 테두리
    if(m_FrameView) {
        Image1->Picture->Bitmap->Canvas->Pen->Color = m_FrameColor;
        Image1->Picture->Bitmap->Canvas->Pen->Width = m_FrameWidth;
        Image1->Picture->Bitmap->Canvas->Brush->Style = bsClear;
        // 이미지 외곽 프레임 라인을 그릴 때 라인의 굵기가 커짐에 따라 밖을 빠져 나가는 부분을 잡아주기 위해
        // Left, Top 쪽은 라인이 왼쪽과 윗쪽으로 먼저 증가하기 때문에 m_FrameWidth/2 를 더해 주고
        // Width 와 Height는 왼쪽과 윗쪽으로 증가하기 때문에 (m_FrameWidth-1)/2 를 빼주게 된다.
        Image1->Picture->Bitmap->Canvas->Rectangle(0 + (m_FrameWidth/2),  + (m_FrameWidth/2), Image1->Width-((m_FrameWidth-1)/2), Image1->Height-((m_FrameWidth-1)/2));
    }

    // Lines

    Image1->Picture->Bitmap->Canvas->Pen->Color = (TColor)(m_LineColor.GetB() << 16 | m_LineColor.GetG() << 8 | m_LineColor.GetR());
    Image1->Picture->Bitmap->Canvas->Pen->Width = m_LineWidth;


    list<TMyLine>::iterator iter = m_listLine.begin();

    // GDI+를 이용하여 Angi-Aliasing Line 구현
    Gdiplus::Graphics gp(Image1->Picture->Bitmap->Canvas->Handle);
    Gdiplus::Pen      pen(m_LineColor, m_LineWidth);

    pen.SetDashStyle((DashStyle)m_LineStyle);

    // 시작 부분 화살표 모양
    switch(m_LineStartCapStyle) {
        case 0:  pen.SetStartCap(LineCapNoAnchor);       break;
        case 1:  pen.SetStartCap(LineCapSquareAnchor);   break;
        case 2:  pen.SetStartCap(LineCapRoundAnchor);    break;
        case 3:  pen.SetStartCap(LineCapDiamondAnchor);  break;
        case 4:  pen.SetStartCap(LineCapArrowAnchor);    break;
        default: pen.SetStartCap(LineCapNoAnchor);       break;
    }

    // 끝 부분 화살표 모양
    switch(m_LineEndCapStyle) {
        case 0:  pen.SetEndCap(LineCapNoAnchor);       break;
        case 1:  pen.SetEndCap(LineCapSquareAnchor);   break;
        case 2:  pen.SetEndCap(LineCapRoundAnchor);    break;
        case 3:  pen.SetEndCap(LineCapDiamondAnchor);  break;
        case 4:  pen.SetEndCap(LineCapArrowAnchor);    break;
        default: pen.SetEndCap(LineCapNoAnchor);       break;
    }

    gp.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);           // Anti-Aliasing

    while(iter != m_listLine.end()) {
       if(iter->Selected) {
            gp.SetSmoothingMode(Gdiplus::SmoothingModeDefault);

            Gdiplus::Pen            penSelected(Gdiplus::Color(255, 255, 0, 255), 3);
            penSelected.SetDashStyle(DashStyleDashDot);
            gp.DrawLine(&penSelected, iter->Start.x, iter->Start.y, iter->End.x, iter->End.y);

            Gdiplus::Pen            penEdge(Gdiplus::Color(255, 0, 0, 0), 1);
            Gdiplus::SolidBrush     brushEdge(Gdiplus::Color(255, 255, 255, 0));
            Gdiplus::RectF          rectStart(iter->Start.x-3, iter->Start.y-3, 6,6);
            Gdiplus::RectF          rectEnd(iter->End.x-3, iter->End.y-3, 6,6);

            gp.FillRectangle(&brushEdge, rectStart);
            gp.DrawRectangle(&penEdge,   rectStart);

            gp.FillRectangle(&brushEdge, rectEnd);
            gp.DrawRectangle(&penEdge,   rectEnd);
       }
       else {
            gp.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
            gp.DrawLine(&pen, iter->Start.x, iter->Start.y, iter->End.x, iter->End.y);
       }

       iter++;
    }

}
//---------------------------------------------------------------------------

bool __fastcall TfmAZLabelImageEditor::FindSelectLabel(int X, int Y)
{

    bool bRet = false;

    list<TMyLabel>::iterator iter = m_listLabel.begin();

    while(iter != m_listLabel.end()) {

        if(X > iter->Pos.x - (m_LabelSize / 2)
        && X < (iter->Pos.x + (m_LabelSize / 2) - 1)
        && Y > iter->Pos.y - (m_LabelSize / 2)
        && Y < (iter->Pos.y + (m_LabelSize / 2) - 1)) {
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

bool __fastcall TfmAZLabelImageEditor::MoveSelectLabel(int X, int Y)
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

bool __fastcall TfmAZLabelImageEditor::IsSelectLabel()
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

bool __fastcall TfmAZLabelImageEditor::DeleteSelectLabel()
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

void __fastcall TfmAZLabelImageEditor::ResetSelectLabel()
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

bool __fastcall TfmAZLabelImageEditor::FindSelectLine(int X, int Y)
{

    bool bRet = false;

    list<TMyLine>::iterator iter = m_listLine.begin();

    while(iter != m_listLine.end()) {

		GraphicsPath path;
        Pen strokePen(Gdiplus::Color(200, 255, 0, 0), 5);

        path.AddLine(iter->Start.x, iter->Start.y, iter->End.x, iter->End.y);
        if(path.IsOutlineVisible(X, Y, &strokePen)) {
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

bool __fastcall TfmAZLabelImageEditor::MoveSelectLine(int dX, int dY)
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

bool __fastcall TfmAZLabelImageEditor::MoveSelectLineEdge(int dX, int dY, bool bShiftKeyDown)
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

bool __fastcall TfmAZLabelImageEditor::MoveSelectLineEdgeShift(int X, int Y, bool bShift)
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

bool __fastcall TfmAZLabelImageEditor::FindSelectLineEdge(int X, int Y)
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

bool __fastcall TfmAZLabelImageEditor::IsSelectLine()
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

void __fastcall TfmAZLabelImageEditor::ResetSelectLine()
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

void __fastcall TfmAZLabelImageEditor::ResetSelectLineEdge()
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

bool __fastcall TfmAZLabelImageEditor::DeleteSelectLine()
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
void __fastcall DrawRoundRectangle(Gdiplus::Graphics* g, Pen *p, Gdiplus::Rect& rect, UINT8 radius)
{
	if (g==NULL) return;
	GraphicsPath path;

	path.AddLine(rect.X + radius, rect.Y, rect.X + rect.Width - (radius * 2), rect.Y);
	path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y, radius * 2, radius * 2, 270, 90);
	path.AddLine(rect.X + rect.Width, rect.Y + radius, rect.X + rect.Width, rect.Y + rect.Height - (radius * 2));
	path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y + rect.Height - (radius * 2), radius * 2,
		radius * 2, 0, 90);
	path.AddLine(rect.X + rect.Width - (radius * 2), rect.Y + rect.Height, rect.X + radius, rect.Y + rect.Height);
	path.AddArc(rect.X, rect.Y + rect.Height - (radius * 2), radius * 2, radius * 2, 90, 90);
	path.AddLine(rect.X, rect.Y + rect.Height - (radius * 2), rect.X, rect.Y + radius);
	path.AddArc(rect.X, rect.Y, radius * 2, radius * 2, 180, 90);
	path.CloseFigure();
	g->DrawPath(p, &path);
}

void __fastcall FillRoundRectangle(Gdiplus::Graphics* g, Brush *p, Gdiplus::Rect& rect, UINT8 radius)
{
	if (g == NULL) return;
	GraphicsPath path;

	path.AddLine(rect.X + radius, rect.Y, rect.X + rect.Width - (radius * 2), rect.Y);
	path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y, radius * 2, radius * 2, 270, 90);
	path.AddLine(rect.X + rect.Width, rect.Y + radius, rect.X + rect.Width, rect.Y + rect.Height - (radius * 2));
	path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y + rect.Height - (radius * 2), radius * 2, radius * 2, 0, 90);
	path.AddLine(rect.X + rect.Width - (radius * 2), rect.Y + rect.Height, rect.X + radius, rect.Y + rect.Height);
	path.AddArc(rect.X, rect.Y + rect.Height - (radius * 2), radius * 2, radius * 2, 90, 90);
	path.AddLine(rect.X, rect.Y + rect.Height - (radius * 2), rect.X, rect.Y + radius);
	path.AddArc(rect.X, rect.Y, radius * 2, radius * 2, 180, 90);
	path.CloseFigure();

	g->FillPath(p, &path);
}

//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::DrawLabel()
{
    list<TMyLabel>::iterator iter = m_listLabel.begin();

    int nIndex = 1;

    Gdiplus::Graphics       gp(Image1->Picture->Bitmap->Canvas->Handle);

    while(iter != m_listLabel.end()) {
       DrawLabel(&gp, iter->Index, iter->Pos.x, iter->Pos.y, iter->Selected);
       nIndex++;
       iter++;
    }

}
//---------------------------------------------------------------------------
// LABEL 그리기  ////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::DrawLabel(Gdiplus::Graphics * pGP, int index, int nX, int nY, bool bSelected)
{
    // GDI+를 이용하여 Angi-Aliasing 구현

    Gdiplus::Pen            pen(Gdiplus::Color(255, 255, 255, 255), m_LabelOuterSize);
    Gdiplus::SolidBrush     sBrush(m_LabelBaseColor);

    switch(m_LabelStyle) {

        case 0: // Round Rec
            {
                pGP->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

                Gdiplus::Rect rect(nX-(m_LabelSize/2), nY-(m_LabelSize/2), m_LabelSize, m_LabelSize);
                Gdiplus::Rect rect_Back(nX-(m_LabelSize/2), nY-(m_LabelSize/2), m_LabelSize+1, m_LabelSize+1);

                // 그림자 그리기
                pen.SetColor(Gdiplus::Color(170,170,170));
                DrawRoundRectangle(pGP, &pen, rect_Back, 5);

                // 가운데 배경 그리기
                FillRoundRectangle(pGP, &sBrush, rect, 5);

                // 외곽 라운드 그리기.
                pen.SetColor(m_LabelOutRecColor);
                DrawRoundRectangle(pGP, &pen, rect, 5);
            }
            break;


        case 1: // Rec
            {
                pGP->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

                pen.SetColor(Gdiplus::Color(170,170,170));
                pGP->DrawRectangle(&pen, nX-(m_LabelSize/2)+1, nY-(m_LabelSize/2)+1, m_LabelSize, m_LabelSize);

                pGP->FillRectangle(&sBrush, nX-(m_LabelSize/2), nY-(m_LabelSize/2), m_LabelSize, m_LabelSize);

                pen.SetColor(m_LabelOutRecColor);
                pGP->DrawRectangle(&pen, nX-(m_LabelSize/2), nY-(m_LabelSize/2), m_LabelSize, m_LabelSize);

            }
            break;

        case 2: // Circle
            {
                pGP->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

                pen.SetColor(Gdiplus::Color(170,170,170));
                pGP->DrawEllipse(&pen, nX-(m_LabelSize/2)+1, nY-(m_LabelSize/2)+1, m_LabelSize, m_LabelSize);

                pGP->FillEllipse(&sBrush, nX-(m_LabelSize/2), nY-(m_LabelSize/2), m_LabelSize, m_LabelSize);

                pen.SetColor(m_LabelOutRecColor);
                pGP->DrawEllipse(&pen, nX-(m_LabelSize/2), nY-(m_LabelSize/2), m_LabelSize, m_LabelSize);

            }
            break;
    }


    if(bSelected) {
        pGP->SetSmoothingMode(Gdiplus::SmoothingModeDefault);

        Gdiplus::Pen    penSelected(Gdiplus::Color(255, 0, 0, 0), 1);
        penSelected.SetDashStyle(DashStyleDash);
        pGP->DrawRectangle(&penSelected, nX-(m_LabelSize/2)-4, nY-(m_LabelSize/2)-4, m_LabelSize + 8, m_LabelSize + 8);

        penSelected.SetColor(Gdiplus::Color(255, 200, 200, 200));
        pGP->DrawRectangle(&penSelected, nX-(m_LabelSize/2)-3, nY-(m_LabelSize/2)-3, m_LabelSize + 6, m_LabelSize + 6);

        pGP->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    }

    // Label 번호를 출력 한다.


    Gdiplus::FontStyle  fontStyle;

    if(Image1->Picture->Bitmap->Canvas->Font->Style.Contains(fsStrikeOut)) {
        fontStyle = FontStyleStrikeout;
    }
    else if(Image1->Picture->Bitmap->Canvas->Font->Style.Contains(fsUnderline)) {
        fontStyle = FontStyleUnderline;
    }
    else if(Image1->Picture->Bitmap->Canvas->Font->Style.Contains(fsBold)
         && Image1->Picture->Bitmap->Canvas->Font->Style.Contains(fsItalic)) {
        fontStyle = FontStyleBoldItalic;
    }
    else if(Image1->Picture->Bitmap->Canvas->Font->Style.Contains(fsItalic)) {
        fontStyle = FontStyleItalic;
    }
    else if(Image1->Picture->Bitmap->Canvas->Font->Style.Contains(fsBold)) {
        fontStyle = FontStyleBold;
    }
    else {
        fontStyle = FontStyleRegular;
    }

    Gdiplus::Font       txtFont(Image1->Picture->Bitmap->Canvas->Font->Name.w_str(),
                                Image1->Picture->Bitmap->Canvas->Font->Size,
                                fontStyle,
                                UnitPixel);

    TColor clFontColor = Image1->Picture->Bitmap->Canvas->Font->Color;

    Gdiplus::PointF     txtPoint((float)(nX-(m_LabelSize/2)), (float)(nY-(m_LabelSize/2)));
    SolidBrush          txtBrush(Gdiplus::Color(GetRValue(clFontColor), GetGValue(clFontColor), GetBValue(clFontColor)));
    Gdiplus::RectF      layoutRect(nX-(m_LabelSize/2), nY-(m_LabelSize/2), m_LabelSize, m_LabelSize);
    StringFormat        SF;
    SF.SetAlignment(StringAlignmentCenter);
    SF.SetLineAlignment(StringAlignmentCenter);

    pGP->DrawString(IntToStr(index).w_str(),-1, &txtFont, layoutRect, &SF, &txtBrush);
}

//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::DrawOverlayText()
{
//    Image1->Picture->Bitmap->Canvas->Font->Color = clBlack;
//    Image1->Picture->Bitmap->Canvas->Font->Size  = 10;
//    Image1->Picture->Bitmap->Canvas->Font->Style =  Image1->Picture->Bitmap->Canvas->Font->Style >> fsBold;
//    Image1->Picture->Bitmap->Canvas->Font->Style =  Image1->Picture->Bitmap->Canvas->Font->Style >> fsItalic;
//    Image1->Picture->Bitmap->Canvas->TextOut(10, 1,  "Line : Left Mouse Drag");
//    Image1->Picture->Bitmap->Canvas->TextOut(10, 13, "Label : Double Click");
//    Image1->Picture->Bitmap->Canvas->TextOut(10, 25, "Delete : Select -> Right Mouse Click");

}
//---------------------------------------------------------------------------



void __fastcall TfmAZLabelImageEditor::CheckBox_MagneticClick(TObject *Sender)
{
    m_LabelMagnetic = CheckBox_Magnetic->Checked;

    DisplaySettings();
}
//---------------------------------------------------------------------------


void __fastcall TfmAZLabelImageEditor::FormShow(TObject *Sender)
{
//    DrawMyLine();
//    DrawOverlayText();

    DrawMyLine();
    DrawLabel();

    DisplaySettings();
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::SpeedButton_LabelStyle_1Click(TObject *Sender)
{
    TSpeedButton * pBtn = dynamic_cast<TSpeedButton *>(Sender);
    if(pBtn == NULL) return;

    m_LabelStyle = pBtn->Tag;

    DrawMyLine();
    DrawLabel();

}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::Button_FontClick(TObject *Sender)
{
    FontDialog1->Font = Image1->Picture->Bitmap->Canvas->Font;
    FontDialog1->Execute(this->Handle);
    Image1->Picture->Bitmap->Canvas->Font = FontDialog1->Font;

    DrawMyLine();
    DrawLabel();
}
//---------------------------------------------------------------------------





void __fastcall TfmAZLabelImageEditor::SpeedButton_LineStyle_1Click(TObject *Sender)
{
    TSpeedButton * pBtn = dynamic_cast<TSpeedButton *>(Sender);
    if(pBtn == NULL) return;

    m_LineStyle = pBtn->Tag;

    DrawMyLine();
    DrawLabel();
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::SpeedButton_LineCapStart_1Click(TObject *Sender)
{
    TSpeedButton * pBtn = dynamic_cast<TSpeedButton *>(Sender);
    if(pBtn == NULL) return;

    m_LineStartCapStyle = pBtn->Tag;

    DrawMyLine();
    DrawLabel();
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::SpeedButton_LineCapEnd_1Click(TObject *Sender)
{
    TSpeedButton * pBtn = dynamic_cast<TSpeedButton *>(Sender);
    if(pBtn == NULL) return;

    m_LineEndCapStyle = pBtn->Tag;

    DrawMyLine();
    DrawLabel();
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::Edit_LabelStartNumChange(TObject *Sender)
{
    m_LabelLastIndex = Edit_LabelStartNum->Text.ToIntDef(0);
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::Edit_LabelSizeChange(TObject *Sender)
{
    int nSize = Edit_LabelSize->Text.ToIntDef(0);

    if(nSize > 0 && nSize < 100) {
        m_LabelSize = nSize;

        DrawMyLine();
        DrawLabel();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::Edit_LabelOuterLineChange(TObject *Sender)
{
    int nSize = Edit_LabelOuterLine->Text.ToIntDef(0);

    if(nSize > 0 && nSize < m_LabelSize) {
        m_LabelOuterSize = nSize;

        DrawMyLine();
        DrawLabel();
    }
}
//---------------------------------------------------------------------------


void __fastcall TfmAZLabelImageEditor::Edit_LineWidthChange(TObject *Sender)
{
    int nSize = Edit_LineWidth->Text.ToIntDef(0);

    if(nSize >= 1 && nSize <= 10) {
        m_LineWidth = nSize;

        DrawMyLine();
        DrawLabel();
    }
}
//---------------------------------------------------------------------------


void __fastcall TfmAZLabelImageEditor::Button_LineBaseColorClick(TObject *Sender)
{

    ColorDialog1->Color = (TColor)(m_LabelBaseColor.GetB() << 16 | m_LabelBaseColor.GetG() << 8 | m_LabelBaseColor.GetR());

    if(ColorDialog1->Execute(this->Handle)) {
        TColor clColor   = ColorDialog1->Color;
        m_LabelBaseColor = Gdiplus::Color(GetRValue(clColor), GetGValue(clColor), GetBValue(clColor));
        Panel_LabelBaseColor->Color = clColor;

        DrawMyLine();
        DrawLabel();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::Button_LineOutRecColorClick(TObject *Sender)
{
    ColorDialog1->Color = (TColor)(m_LabelOutRecColor.GetB() << 16 | m_LabelOutRecColor.GetG() << 8 | m_LabelOutRecColor.GetR());

    if(ColorDialog1->Execute(this->Handle)) {
        TColor cl = ColorDialog1->Color;
        m_LabelOutRecColor = Gdiplus::Color(GetRValue(cl), GetGValue(cl), GetBValue(cl));
        Panel_LabelOutRecColor->Color = cl;

        DrawMyLine();
        DrawLabel();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::Button_LineColorClick(TObject *Sender)
{
    ColorDialog1->Color = (TColor)(m_LineColor.GetB() << 16 | m_LineColor.GetG() << 8 | m_LineColor.GetR());

    if(ColorDialog1->Execute(this->Handle)) {
        TColor cl = ColorDialog1->Color;
        m_LineColor = Gdiplus::Color(GetRValue(cl), GetGValue(cl), GetBValue(cl));
        Panel_LineColor->Color = cl;

        DrawMyLine();
        DrawLabel();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::DisplaySettings()
{
    Panel_LabelBaseColor->Color     = (TColor)((m_LabelBaseColor.GetB() << 16)   | (m_LabelBaseColor.GetG() << 8)   | m_LabelBaseColor.GetR());
    Panel_LabelOutRecColor->Color   = (TColor)((m_LabelOutRecColor.GetB() << 16) | (m_LabelOutRecColor.GetG() << 8) | m_LabelOutRecColor.GetR());
    Panel_LineColor->Color          = (TColor)((m_LineColor.GetB() << 16)        | (m_LineColor.GetG() << 8)        | m_LineColor.GetR());

    Edit_LabelStartNum->OnChange    = NULL;
    Edit_LabelStartNum->Text        = m_LabelLastIndex;
    Edit_LabelStartNum->OnChange    = Edit_LabelStartNumChange;

    Edit_LabelSize->OnChange        = NULL;
    Edit_LabelSize->Text            = m_LabelSize;
    Edit_LabelOuterLine->Text       = m_LabelOuterSize;
    Edit_LabelSize->OnChange        = Edit_LabelSizeChange;

    Edit_LineWidth->OnChange        = NULL;
    Edit_LineWidth->Text            = m_LineWidth;
    Edit_LineWidth->OnChange        = Edit_LineWidthChange;

    Edit_Reset_W->Text              = Image1->Width;
    Edit_Reset_H->Text              = Image1->Height;

    Edit_ParentSize_W->Text         = m_ParentWidth;
    Edit_ParentSize_H->Text         = m_ParentHeight;

    CheckBox_Magnetic->OnClick      = NULL;
    CheckBox_Magnetic->Checked      = m_LabelMagnetic;
    CheckBox_Magnetic->OnClick      = CheckBox_MagneticClick;

}
//---------------------------------------------------------------------------




void __fastcall TfmAZLabelImageEditor::LoadImageFile1Click(TObject *Sender)
{
    OpenDialog1->Filter = "AllFile | *.bmp; *.JPG; *.PNG;";

    if(OpenDialog1->Execute(this->Handle)) {
        TImage * pImage = new TImage(this);
        pImage->Picture->LoadFromFile(OpenDialog1->FileName);

        if(m_pBitmap) {
            delete m_pBitmap;
            m_pBitmap = new Vcl::Graphics::TBitmap;
            m_pBitmap->Assign(pImage->Picture->Graphic);
        }

        Image1->Picture->Bitmap->Assign(m_pBitmap);

        delete pImage;

        DrawMyLine();
        DrawLabel();
    }
}
//---------------------------------------------------------------------------
bool __fastcall TfmAZLabelImageEditor::ChangeImageSize(  Vcl::Graphics::TBitmap * pSrcBitmap,
                                                         Vcl::Graphics::TBitmap * pDestBitmap,
                                                         int nDestW,
                                                         int nDestH)
{
    if(pSrcBitmap == NULL
    || pSrcBitmap == NULL
    || pSrcBitmap->Width == 0
    || pSrcBitmap->Height == 0) {
        return false;
    }

//    int nWidth  = m_pBitmap->Width;
//    int nHeight = m_pBitmap->Height;

    try {
        // TBitmap 을 GDI+ Bitmap으로 변환 하기.
        Gdiplus::Bitmap *   bitmap      = new Gdiplus::Bitmap(pSrcBitmap->Handle, pSrcBitmap->Palette);

        if(bitmap->GetLastStatus() == Ok) {
            pDestBitmap->Width  = nDestW;
            pDestBitmap->Height = nDestH;

            Gdiplus::Graphics gp(pDestBitmap->Canvas->Handle);
            gp.SetInterpolationMode(InterpolationModeHighQuality);
            gp.DrawImage(bitmap, 0, 0, nDestW, nDestH);

            Image1->Repaint();
            m_pBitmap->Assign(Image1->Picture->Graphic);
        }

        if(bitmap)  delete   bitmap;
    }
    catch (...) {
        return false;
    }

    return true;

}
//---------------------------------------------------------------------------




void __fastcall TfmAZLabelImageEditor::Button_ImageSizeClick(TObject *Sender)
{
    int nRet = MessageDlg("모든 작업을 Image화 한 후 Image Size를 변경하시겠습니까?", mtConfirmation, TMsgDlgButtons() << mbYes<<mbNo, 0);

    if(nRet == mrYes) {
        ResetSelectLine();
        ResetSelectLabel();

        DrawMyLine();
        DrawLabel();

        // 현재까지 작업하던것을 모두 이미지하 하고 Object를 삭제 한다.
        m_pBitmap->Assign(Image1->Picture->Graphic);

        m_listLine.clear();
        m_listLabel.clear();

        // Image Size를 변경.
        int nWidth  = Edit_Reset_W->Text.ToIntDef(0);
        int nHeight = Edit_Reset_H->Text.ToIntDef(0);

        Vcl::Graphics::TBitmap * pDestBitmap = new Vcl::Graphics::TBitmap;

        if(ChangeImageSize(m_pBitmap, pDestBitmap, nWidth, nHeight)) {

            Image1->Picture->Bitmap->Width  = pDestBitmap->Width;
            Image1->Picture->Bitmap->Height = pDestBitmap->Height;

            Image1->Picture->Bitmap->Assign(pDestBitmap);

            Image1->Repaint();
            m_pBitmap->Assign(pDestBitmap);
        }

        delete pDestBitmap;
    }
}
//---------------------------------------------------------------------------


void __fastcall TfmAZLabelImageEditor::SaveExit1Click(TObject *Sender)
{
    int nRet = MessageDlg("편집을 완료하고 나가시겠습니까?\r\n완료 후에는 Ojbect를 수정할 수 없습니다.", mtConfirmation, TMsgDlgButtons() << mbYes<<mbNo, 0);
    if(nRet == mrYes) {

        ResetSelectLine();
        ResetSelectLabel();

        DrawMyLine();
        DrawLabel();

        ModalResult = mrOk;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::Cancel_ExitClick(TObject *Sender)
{
    int nRet = MessageDlg("편집을 취소하시겠습니까?", mtConfirmation, TMsgDlgButtons() << mbYes<<mbNo, 0);
    if(nRet == mrYes) {
        ModalResult = mrCancel;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::ComponentSizeImageClick(TObject *Sender)
{
int nRet = MessageDlg("모든 작업을 Image화 한 후 Image Size를 변경하시겠습니까?", mtConfirmation, TMsgDlgButtons() << mbYes<<mbNo, 0);

    if(nRet == mrYes) {
        ResetSelectLine();
        ResetSelectLabel();

        DrawMyLine();
        DrawLabel();

        // 현재까지 작업하던것을 모두 이미지하 하고 Object를 삭제 한다.
        m_pBitmap->Assign(Image1->Picture->Graphic);

        m_listLine.clear();
        m_listLabel.clear();

//        int nWidth  = m_pBitmap->Width;
//        int nHeight = m_pBitmap->Height;

        Vcl::Graphics::TBitmap * pDestBitmap = new Vcl::Graphics::TBitmap;

        if(ChangeImageSize(m_pBitmap, pDestBitmap, m_ParentWidth, m_ParentHeight)) {

            Image1->Picture->Bitmap->Width  = pDestBitmap->Width;
            Image1->Picture->Bitmap->Height = pDestBitmap->Height;

            Image1->Picture->Bitmap->Assign(pDestBitmap);

            Image1->Repaint();
            m_pBitmap->Assign(pDestBitmap);
        }

        delete pDestBitmap;

//#if 0
//    BYTE *              pBuffer     = new BYTE[m_pBitmap->Width * m_pBitmap->Height * 4];
//    TMemoryStream  *    pMemStream  = new TMemoryStream;
//    Gdiplus::Bitmap *   bitmap      = NULL;
//    IStream *           pStream     = NULL;
//
//    ZeroMemory(pBuffer, sizeof(pBuffer));
//
//    m_pBitmap->SaveToStream(pMemStream);
//    int nSize = pMemStream->Size;
//
//    pMemStream->Position = 0;
//    pMemStream->Read(pBuffer, nSize);
//
//    HRESULT hr = CreateStreamOnHGlobal(NULL, TRUE, &pStream);
//
//    if(SUCCEEDED(hr)) {
//
//        ULONG act = 0;
//
//        hr = pStream->Write(pBuffer, nSize, &act);
//
//        if(SUCCEEDED(hr)) {
//            bitmap = new Gdiplus::Bitmap(pStream);
//        }
//
//        pStream->Release();
//    }
//
//    if(pMemStream)     delete   pMemStream;
//    if(pBuffer)        delete[] pBuffer;
//#else
//    Gdiplus::Bitmap *   bitmap      = new Gdiplus::Bitmap(m_pBitmap->Handle, m_pBitmap->Palette);
//#endif
//
//    if(bitmap->GetLastStatus() == Ok) {
//        Image1->Picture->Bitmap->Width  = nDestW;
//        Image1->Picture->Bitmap->Height = nDestH;
//
//        Gdiplus::Graphics gp(Image1->Picture->Bitmap->Canvas->Handle);
//        gp.SetInterpolationMode(InterpolationModeHighQuality);
//        gp.DrawImage(bitmap, 0, 0, nDestW, nDestH);
//
//        Image1->Repaint();
//        m_pBitmap->Assign(Image1->Picture->Graphic);
//    }
//
//    if(bitmap)  delete   bitmap;

        // Setting All Clear
        m_LabelLastIndex = 0;
        m_listLine.clear();
        m_listLabel.clear();
        DrawMyLine();
        DisplaySettings();

    }
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::ObjectAllMergeClick(TObject *Sender)
{
    m_pBitmap->Assign(Image1->Picture->Graphic);

    m_listLine.clear();
    m_listLabel.clear();
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::ObjectAllClearClick(TObject *Sender)
{
    m_LabelLastIndex = 0;

    m_listLine.clear();
    m_listLabel.clear();


    DrawMyLine();
}
//---------------------------------------------------------------------------


void __fastcall TfmAZLabelImageEditor::FrameViewClick(TObject *Sender)
{
    m_FrameView = !m_FrameView;
    FrameView->Checked = m_FrameView;

    DrawMyLine();
    DrawLabel();
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::FrameColorClick(TObject *Sender)
{
    ColorDialog1->Color = m_FrameColor;

    if(ColorDialog1->Execute(this->Handle)) {
        m_FrameColor = ColorDialog1->Color;

        DrawMyLine();
        DrawLabel();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::FrameWidth1Click(TObject *Sender)
{
    TMenuItem * pMenuItem = dynamic_cast<TMenuItem *>(Sender);
    if(pMenuItem == NULL) return;

    m_FrameWidth = pMenuItem->Tag;

    DrawMyLine();
    DrawLabel();
}
//---------------------------------------------------------------------------


