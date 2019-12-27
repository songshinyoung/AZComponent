//---------------------------------------------------------------------------
#include <vcl.h>
#include <Clipbrd.hpp>
#include <memory>                   //for STL auto_ptr class
#include <pngimage.hpp>

#pragma hdrstop

#include "AZLabelImage_Dsgn.h"
#include "frmInputImageSize.h"
#include "frmInputStrings.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#pragma link "gdiplus.lib"

using namespace Gdiplus;            // GDI+
namespace gdp = Gdiplus;            // GDI+

ULONG_PTR    gdpToken;              // GDI+
Gdiplus::GdiplusStartupInput gdpSI; // GDI+

#define PI 3.14159265359

TfmAZLabelImageEditor *fmAZLabelImageEditor;
//---------------------------------------------------------------------------
__fastcall TfmAZLabelImageEditor::TfmAZLabelImageEditor(TComponent* Owner)
    : TForm(Owner)
{
    Init();

    m_pBitmap           = new Vcl::Graphics::TBitmap;
    m_pBitmapCopy       = new Vcl::Graphics::TBitmap;

    m_pBitmap->Width    = 500;
    m_pBitmap->Height   = 300;
}
//---------------------------------------------------------------------------
__fastcall TfmAZLabelImageEditor::TfmAZLabelImageEditor(TComponent* Owner, Vcl::Graphics::TBitmap * pBitmap, int nW, int nH)
    : TForm(Owner)
{
    Init();

    m_ParentWidth  = nW;
    m_ParentHeight = nH;

    m_pBitmapCopy       = new Vcl::Graphics::TBitmap;
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
    m_LabelMagneticSize = 10;
    m_LabelLastIndex    = 1;

    m_bLineSelected     = false;
    m_bLabelSelected    = false;
    m_bLineEdegSelected = false;

    m_LabelSize         = 25;
    m_LabelOuterSize    = 2;

    m_LabelStyle        = 0;
    m_LineStyle         = 0;
    m_LineWidth         = 2;
    m_LineObjStyle      = 0;

    m_LineStartCapStyle = 0;
    m_LineEndCapStyle   = 0;
    m_LineCapScal       = 1.0;

    m_LabelBaseColor    = Gdiplus::Color(200, 100, 0);          // 라벨 안쪽 배경 색상
    m_LabelOutRecColor  = Gdiplus::Color(255, 255, 255);        // 라벨 외곽 색상.
    m_LineColor         = Gdiplus::Color(255, 0,   0);          // Line Color
    m_LineFillColor     = Gdiplus::Color(255, 255, 255);        // Line Color

    m_FrameView         = true;
    m_FrameWidth        = 1;
    m_FrameColor        = clBlack;

    m_CopyImageExist        = false;
    m_bCopyImageSelected    = false;

    m_bLineTempDrawing  = false;


}
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::FormDestroy(TObject *Sender)
{
    if(m_pBitmap)       delete m_pBitmap;
    if(m_pBitmapCopy)   delete m_pBitmapCopy;
    m_listLine.clear();
    m_listLabel.clear();

    Gdiplus::GdiplusShutdown(gdpToken);

    TClipboard *cb = Clipboard();

    if(cb->HasFormat(CF_TEXT)) {
        String sText = cb->AsText;

        if(sText == L"TMyLine" || sText == L"TMyLabel") {
            cb->Clear();
        }
    }

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
    Image1->Picture->Bitmap->Canvas->Font->Size  = 12;
    Image1->Picture->Bitmap->Canvas->Font->Color = clWhite;

    if (Gdiplus::GdiplusStartup(&gdpToken, &gdpSI, NULL) != Gdiplus::Ok)
    {
        // init error.
    }

}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::WndProc(TMessage& Message)
{
    switch(Message.WParam) {
         case 37:   // Left
         case 38:   // Up
         case 39:   // Right
         case 40:   // Down


            // Ctrl Key가 눌린 상태에서 방향키 누를 경우 선택 오브젝트 이동 가능.
            if(GetAsyncKeyState(VK_CONTROL) & 0x8000) {
                if(m_bLineSelected) {
                    int nOffsetX = 0;
                    int nOffsetY = 0;

                    switch(Message.WParam) {
                         case 37:   nOffsetX = -1; break; // Left
                         case 38:   nOffsetY = -1; break; // Up
                         case 39:   nOffsetX =  1; break; // Right
                         case 40:   nOffsetY =  1; break; // Down
                    }

                    MoveSelectLine(nOffsetX, nOffsetY);

                    DrawMyLine();
                    DrawLabel();
                    Image1->Repaint();
                }
                else if(m_bLabelSelected && (GetAsyncKeyState(VK_CONTROL) & 0x8000)) {

                    int nOffsetX = 0;
                    int nOffsetY = 0;

                    switch(Message.WParam) {
                         case 37:   nOffsetX = -1; break; // Left
                         case 38:   nOffsetY = -1; break; // Up
                         case 39:   nOffsetX =  1; break; // Right
                         case 40:   nOffsetY =  1; break; // Down
                    }

                    MoveSelectLabelOffset(nOffsetX, nOffsetY);

                    DrawMyLine();
                    DrawLabel();
                    Image1->Repaint();

                }
            }

            break;
    }

    switch(Message.Msg) {
        default:
            TForm::WndProc(Message);
            break;
    }

}
//---------------------------------------------------------------------------
// MOUSE DOWN ///////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{

    if(Button == mbLeft) {

        m_bLMouseDown           = true;

        m_bLabelSelected        = false;
        m_bLineSelected         = false;
        m_bLineEdegSelected     = false;
        m_bCopyImageSelected    = false;

        if(m_CopyImageExist) {
            if(X > m_CopyImagePos.x
            && Y > m_CopyImagePos.y
            && X < (m_CopyImagePos.x + m_pBitmapCopy->Width)
            && Y < (m_CopyImagePos.y + m_pBitmapCopy->Height)) {
                m_bCopyImageSelected = true;
            }
            else {
                m_CopyImageExist        = false;
                m_pBitmap->Canvas->Draw(m_CopyImagePos.x, m_CopyImagePos.y, m_pBitmapCopy);
                m_pBitmapCopy->Width  = 0;
                m_pBitmapCopy->Height = 0;
                m_bLMouseDown           = false;

                DrawMyLine();
                DrawLabel();
            }
        }
        else if(IsSelectLine() && FindSelectLineEdge(X, Y)) {
            m_bLineEdegSelected = true;
            Caption = "Select Edge";
        }
        else if(FindSelectLabel(X, Y)) {

            ResetSelectLine();

            m_bLabelSelected    = true;
            Caption = "Select Label";

            DrawMyLine();
            DrawLabel();

        }
        else if(FindSelectLine(X, Y)){

            ResetSelectLabel();

            m_bLineSelected     = true;
            Caption = "Select Line";

            DrawMyLine();
            DrawLabel();
        }
        else {
            ResetSelectLine();
            ResetSelectLabel();

            // 길이가 최소 10 이상은 되어야 등록 한다.
            m_LineTemp.ObjType          = m_LineObjStyle;
            m_LineTemp.Color            = m_LineColor.GetValue();
            m_LineTemp.DashStyle        = m_LineStyle;
            m_LineTemp.CapStartStyle    = m_LineStartCapStyle;
            m_LineTemp.CapEndStyle      = m_LineEndCapStyle;
            m_LineTemp.LineWidth        = m_LineWidth;
            m_LineTemp.Selected         = true;
            m_LineTemp.Fill             = CheckBox_LineFillColorUse->Checked;
            m_LineTemp.FillColor        = m_LineFillColor.GetValue();
        }

        m_LineTemp.Start.x  = X;
        m_LineTemp.Start.y  = Y;

        m_LineTemp.End.x  = X;
        m_LineTemp.End.y  = Y;


        m_LabelTemp.Pos.x = X;
        m_LabelTemp.Pos.y = Y;


        if(m_bLabelSelected
        || m_bLineSelected
        || m_bLineEdegSelected
        || m_bCopyImageSelected ) {
            Image1->Cursor = crHandPoint;
        }
        else {
            Image1->Cursor = crCross;
        }

    }
    else if(Button == mbRight) {

        m_bRMouseDown = true;


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

        if(m_bCopyImageSelected) {
            int deltaX = X - m_LineTemp.Start.x;
            int deltaY = Y - m_LineTemp.Start.y;

            m_CopyImagePos.x += deltaX;
            m_CopyImagePos.y += deltaY;

            m_LineTemp.Start.x  = X;
            m_LineTemp.Start.y  = Y;


            DrawMyLine();
            DrawLabel();

        }
        else if(m_bLineEdegSelected) {

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
                int x = X % m_LabelMagneticSize;
                int y = Y % m_LabelMagneticSize;
                X = x < (m_LabelMagneticSize/2) ? X-x : X + (m_LabelMagneticSize-x);
                Y = y < (m_LabelMagneticSize/2) ? Y-y : Y + (m_LabelMagneticSize-y);
            }

            bool bSelected = MoveSelectLabel(X, Y);

//            Gdiplus::Graphics       gp(Image1->Picture->Bitmap->Canvas->Handle);

            DrawMyLine();
            DrawLabel();

            //if(!bSelected) DrawLabel(&gp, m_LabelLastIndex, X, Y, true);
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
            m_bLineTempDrawing = true;

            m_LineTemp.End.x    = X;
            m_LineTemp.End.y    = Y;

            m_LineTemp.MoveEndEdgeShift(X, Y, Shift.Contains(ssShift));

//            Gdiplus::Graphics       gp(Image1->Picture->Bitmap->Canvas->Handle);

            DrawMyLine();
            DrawLabel();

//            switch(m_LineObjStyle) {
//                case 0:
//                    Image1->Picture->Bitmap->Canvas->MoveTo(m_LineTemp.Start.x, m_LineTemp.Start.y);
//                    Image1->Picture->Bitmap->Canvas->LineTo(X, Y);
//                    break;
//
//                case 1:
//                    Image1->Picture->Bitmap->Canvas->Brush->Style = bsClear;
//                    Image1->Picture->Bitmap->Canvas->Rectangle(m_LineTemp.Start.x, m_LineTemp.Start.y, X, Y);
//                    break;
//
//                case 2:
//                    Image1->Picture->Bitmap->Canvas->Brush->Style = bsClear;
//                    Image1->Picture->Bitmap->Canvas->Ellipse(m_LineTemp.Start.x, m_LineTemp.Start.y, X, Y);
//                    break;
//            }

        }

        Image1->Repaint();      // 화면 갱신 속도 향상.
    }
    else if(m_bRMouseDown) {
        // 오른쪽 마우스 Down 후 Move

    }
    else {
        bool bOnSelectedObject = false;

        TCursor crChangeCursor = crCross;

        // 아무런 마우스 Down 없이 Move
        TMyLabel * pLabel = NULL;
        TMyLine *  pLine  = NULL;

        if(IsSelectLabel(&pLabel)) {
            if(pLabel) {
                if(IsRegionOnLabel(pLabel, X, Y)) {
                    crChangeCursor = crHandPoint;
                }
            }
        }
        else if(IsSelectLine(&pLine)) {
            if(pLine) {
                if(IsRegionOnLineEdge(pLine, X, Y)) {
                    crChangeCursor = crSizeAll;
                }
                else if(IsRegionOnLine(pLine, X, Y)) {
                    crChangeCursor = crHandPoint;
                }
            }
        }
        else if(m_CopyImageExist) {
            if(X > m_CopyImagePos.x
            && Y > m_CopyImagePos.y
            && X < (m_CopyImagePos.x + m_pBitmapCopy->Width)
            && Y < (m_CopyImagePos.y + m_pBitmapCopy->Height)) {
                crChangeCursor = crHandPoint;
            }
        }

        Image1->Cursor = crChangeCursor;


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

            m_bLineTempDrawing          = false;

            //------------------------------
            m_LineTemp.End.x = X;
            m_LineTemp.End.y = Y;

            if(m_LineTemp.GetLen() > 10.0) {
                // 길이가 최소 10 이상은 되어야 등록 한다.
                m_LineTemp.ObjType          = m_LineObjStyle;
                m_LineTemp.Color            = m_LineColor.GetValue();
                m_LineTemp.DashStyle        = m_LineStyle;
                m_LineTemp.CapStartStyle    = m_LineStartCapStyle;
                m_LineTemp.CapEndStyle      = m_LineEndCapStyle;
                m_LineTemp.LineWidth        = m_LineWidth;
                m_LineTemp.Selected         = true;
                m_LineTemp.Fill             = CheckBox_LineFillColorUse->Checked;
                m_LineTemp.FillColor        = m_LineFillColor.GetValue();

                m_bLineSelected             = true;



                m_LineTemp.MoveEndEdgeShift(X, Y, Shift.Contains(ssShift));

//                switch(m_LineObjStyle) {
//                    case 0: // Line
//                        break;
//
//                    case 1: // Rect
//                        if(m_LineTemp.End.x < m_LineTemp.Start.x) swap(m_LineTemp.End.x, m_LineTemp.Start.x);
//                        if(m_LineTemp.End.y < m_LineTemp.Start.y) swap(m_LineTemp.End.y, m_LineTemp.Start.y);
//                        break;
//
//                    case 2: // Circle
//                        if(m_LineTemp.End.x < m_LineTemp.Start.x) swap(m_LineTemp.End.x, m_LineTemp.Start.x);
//                        if(m_LineTemp.End.y < m_LineTemp.Start.y) swap(m_LineTemp.End.y, m_LineTemp.Start.y);
//                        break;
//                }

                m_listLine.push_back(m_LineTemp);

                DrawMyLine();
                DrawLabel();

            }
        }
    }
    else if(m_bRMouseDown){
        if(IsSelectLabel() || IsSelectLine()) {
            POINT CurserPos;
            GetCursorPos(&CurserPos);
            PopupMenu1->Popup(CurserPos.x, CurserPos.y);
        }
    }

    if(m_bRMouseDown || m_bLMouseDown) {

        DrawMyLine();

        DrawLabel();

        DrawOverlayText();
    }

    m_bRMouseDown = false;
    m_bLMouseDown = false;

    m_bLineTempDrawing = false;

    Image1->Repaint();

}
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::Image1DblClick(TObject *Sender)
{
    if(IsSelectLabel() != true && IsSelectLine() != true ) {
        m_LabelTemp.Index       = m_LabelLastIndex;
        m_LabelLastIndex       += 1;

        m_LabelTemp.Size        = m_LabelSize;
        m_LabelTemp.OuterSize   = m_LabelOuterSize;
        m_LabelTemp.Style       = m_LabelStyle;
        m_LabelTemp.BaseColor   = m_LabelBaseColor.GetValue();
        m_LabelTemp.OutRecColor = m_LabelOutRecColor.GetValue();
        m_LabelTemp.Font->Assign(Image1->Picture->Bitmap->Canvas->Font);

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
//    Gdiplus::Graphics gp(Image1->Picture->Bitmap->Canvas->Handle);
    Gdiplus::Pen      pen(m_LineColor, m_LineWidth);

    // 기존 Line Object 그리기.
    while(iter != m_listLine.end()) {
        DrawMyLine((TMyLine *)&(*iter));
        iter++;
    }

    // 현재 마우스 드래그로 그리고 있는 Line Object
    if(m_bLineTempDrawing) {
        if(m_LineTemp.Selected) {
            DrawMyLine(&m_LineTemp);
        }
    }


    StatusBar1->Panels->Items[1]->Text = "Line Obj : " + IntToStr((int)m_listLine.size());

}
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::DrawMyLine(TMyLine *pLine)
{
    if(pLine == NULL) return;

    Gdiplus::Graphics gp(Image1->Picture->Bitmap->Canvas->Handle);
    Gdiplus::Graphics *pGP = &gp;

    Gdiplus::Pen      pen(Gdiplus::Color(pLine->Color), pLine->LineWidth);

    pGP->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    pen.SetDashStyle((DashStyle)pLine->DashStyle);

    // 시작 부분 화살표 모양
    switch(pLine->CapStartStyle) {
        case 0:
            pen.SetStartCap(LineCapNoAnchor);
            break;

        case 1:     // 삭가형
            // pen.SetStartCap(LineCapSquareAnchor);
            {
                GraphicsPath capPath;
                double dLen = 1.5 * m_LineCapScal;
                //capPath.AddLines(points, 5);
                capPath.AddRectangle(Gdiplus::RectF(-dLen,-dLen,dLen*2.0,dLen*2.0));
                CustomLineCap custCap(&capPath, NULL);
                custCap.SetStrokeCaps(LineCapTriangle, LineCapRound);
                pen.SetStartCap(LineCapCustom);
                pen.SetCustomStartCap(&custCap);
            }
            break
            ;
        case 2:     // 원
            // pen.SetStartCap(LineCapRoundAnchor);
            {
//                    Gdiplus::Point points[5] = {Gdiplus::Point(-2, 0),  Gdiplus::Point(-2, -4), Gdiplus::Point(2, -4), Gdiplus::Point(2, 0), Gdiplus::Point(-2, 0) };
                GraphicsPath capPath;
                double dLen = 1.5 * m_LineCapScal;
                capPath.AddEllipse(Gdiplus::RectF(-dLen,-dLen,dLen*2.0,dLen*2.0));
                CustomLineCap custCap(&capPath, NULL);
                custCap.SetStrokeCaps(LineCapTriangle, LineCapRound);
                pen.SetStartCap(LineCapCustom);
                pen.SetCustomStartCap(&custCap);
            }
            break
            ;
        case 3: // 다이아 몬드
            // pen.SetStartCap(LineCapDiamondAnchor);
            {
//                    Gdiplus::Point points[5] = {Gdiplus::Point(-sq, 0),  Gdiplus::Point(-2, -4), Gdiplus::Point(2, -4), Gdiplus::Point(2, 0), Gdiplus::Point(-2, 0) };
                double dLen = 1.5 * m_LineCapScal;
                dLen = sqrt(dLen*dLen + dLen*dLen);
                Gdiplus::PointF points[5] = {Gdiplus::PointF(-dLen, 0),  Gdiplus::PointF(0, -dLen), Gdiplus::PointF(dLen, 0), Gdiplus::PointF(0, dLen), Gdiplus::PointF(-dLen, 0) };

                GraphicsPath capPath;
                capPath.AddPolygon(points, 5);
                CustomLineCap custCap(&capPath, NULL);
                custCap.SetStrokeCaps(LineCapTriangle, LineCapRound);
                pen.SetStartCap(LineCapCustom);
                pen.SetCustomStartCap(&custCap);
            }
            break;

        case 4:     // Arrow Cap (화살표)
            //pen.SetStartCap(LineCapArrowAnchor);
            {
                AdjustableArrowCap myArrow(4.0*m_LineCapScal, 3.0*m_LineCapScal, true);
                pen.SetStartCap(LineCapCustom);
                pen.SetCustomStartCap(&myArrow);
            }
            break;

        case 5:     // Line Cap (치수선)
            {
                Gdiplus::PointF points[2] = {Gdiplus::PointF(-4.0*m_LineCapScal, 0),  Gdiplus::PointF(4.0*m_LineCapScal, 0) };
                GraphicsPath capPath;
                capPath.AddLines(points, 2);
                CustomLineCap custCap(NULL, &capPath);
                custCap.SetStrokeCaps(LineCapTriangle, LineCapRound);
                pen.SetStartCap(LineCapCustom);
                pen.SetCustomStartCap(&custCap);
            }
            break;
        default: pen.SetStartCap(LineCapNoAnchor);       break;
    }

    // 끝 부분 화살표 모양
    switch(pLine->CapEndStyle) {
        case 0:
            pen.SetEndCap(LineCapNoAnchor);
            break;

        case 1:     // 삭가형
            // pen.SetStartCap(LineCapSquareAnchor);
            {
                GraphicsPath capPath;
                double dLen = 1.5 * m_LineCapScal;
                //capPath.AddLines(points, 5);
                capPath.AddRectangle(Gdiplus::RectF(-dLen,-dLen,dLen*2.0,dLen*2.0));
                CustomLineCap custCap(&capPath, NULL);
                custCap.SetStrokeCaps(LineCapTriangle, LineCapRound);
                pen.SetEndCap(LineCapCustom);
                pen.SetCustomEndCap(&custCap);
            }
            break;

        case 2:     // 원
            // pen.SetStartCap(LineCapRoundAnchor);
            {
//                    Gdiplus::Point points[5] = {Gdiplus::Point(-2, 0),  Gdiplus::Point(-2, -4), Gdiplus::Point(2, -4), Gdiplus::Point(2, 0), Gdiplus::Point(-2, 0) };
                GraphicsPath capPath;
                double dLen = 1.5 * m_LineCapScal;
                capPath.AddEllipse(Gdiplus::RectF(-dLen,-dLen,dLen*2.0,dLen*2.0));
                CustomLineCap custCap(&capPath, NULL);
                custCap.SetStrokeCaps(LineCapTriangle, LineCapRound);
                pen.SetEndCap(LineCapCustom);
                pen.SetCustomEndCap(&custCap);
            }
            break;

        case 3: // 다이아 몬드
            // pen.SetStartCap(LineCapDiamondAnchor);
            {
//                    Gdiplus::Point points[5] = {Gdiplus::Point(-sq, 0),  Gdiplus::Point(-2, -4), Gdiplus::Point(2, -4), Gdiplus::Point(2, 0), Gdiplus::Point(-2, 0) };
                double dLen = 1.5 * m_LineCapScal;
                dLen = sqrt(dLen*dLen + dLen*dLen);
                Gdiplus::PointF points[5] = {Gdiplus::PointF(-dLen, 0),  Gdiplus::PointF(0, -dLen), Gdiplus::PointF(dLen, 0), Gdiplus::PointF(0, dLen), Gdiplus::PointF(-dLen, 0) };

                GraphicsPath capPath;
                capPath.AddPolygon(points, 5);
                CustomLineCap custCap(&capPath, NULL);
                custCap.SetStrokeCaps(LineCapTriangle, LineCapRound);
                pen.SetEndCap(LineCapCustom);
                pen.SetCustomEndCap(&custCap);
            }
            break;

        case 4:     // Arrow Cap (화살표)
            //pen.SetStartCap(LineCapArrowAnchor);
            {
                AdjustableArrowCap myArrow(4.0*m_LineCapScal, 3.0*m_LineCapScal, true);
                pen.SetEndCap(LineCapCustom);
                pen.SetCustomEndCap(&myArrow);
            }
            break;

        case 5:     // Line Cap (치수선)
            {
                Gdiplus::PointF points[2] = {Gdiplus::PointF(-4.0*m_LineCapScal, 0),  Gdiplus::PointF(4.0*m_LineCapScal, 0) };
                GraphicsPath capPath;
                capPath.AddLines(points, 2);
                CustomLineCap custCap(NULL, &capPath);
                custCap.SetStrokeCaps(LineCapTriangle, LineCapRound);
                pen.SetEndCap(LineCapCustom);
                pen.SetCustomEndCap(&custCap);
            }
            break;

        default: pen.SetEndCap(LineCapNoAnchor);       break;
    }

    switch(pLine->ObjType) {
        case 0: // Line
            pGP->DrawLine(&pen, pLine->Start.x, pLine->Start.y, pLine->End.x, pLine->End.y);
            break;

        case 1: // Rect
            {
                int nStartX = (pLine->End.x > pLine->Start.x) ? pLine->Start.x : pLine->End.x;
                int nStartY = (pLine->End.y > pLine->Start.y) ? pLine->Start.y : pLine->End.y;

                pen.SetLineJoin(LineJoinRound);  // 끝 모서리의 두 선이 만나는 지점을 약간 둥근 모양으로 한다.

                if(pLine->Fill) {
//                    if(RadioButton_FillTypeBrush->Checked) {
//                        int Hatch=5;
//                        Gdiplus::HatchBrush *pHatchBrush = new Gdiplus::HatchBrush((HatchStyle)Hatch, Gdiplus::Color(0,0,0), Gdiplus::Color(pLine->FillColor));
//                        pGP->FillRectangle(pHatchBrush, nStartX, nStartY, pLine->GetLenX(), pLine->GetLenY());
//                        delete pHatchBrush;
//                    }
//                    else {
                        Gdiplus::SolidBrush     brush(Gdiplus::Color(pLine->FillColor));
                        pGP->FillRectangle(&brush, nStartX, nStartY, pLine->GetLenX(), pLine->GetLenY());
//                    }
                }

                pGP->DrawRectangle(&pen, nStartX, nStartY, pLine->GetLenX(), pLine->GetLenY());
            }
            break;

        case 2: // Circle
            if(pLine->Fill) {
                Gdiplus::SolidBrush     brush(Gdiplus::Color(pLine->FillColor));
                pGP->FillEllipse(&brush, pLine->Start.x, pLine->Start.y, pLine->End.x - pLine->Start.x, pLine->End.y - pLine->Start.y);
            }

            pGP->DrawEllipse(&pen, pLine->Start.x, pLine->Start.y, pLine->End.x - pLine->Start.x, pLine->End.y - pLine->Start.y);
            break;

    }

    //-----------------------------------------------------------
    // 선택된 라인인 경우 외고가 선택 선 보이기.
    //-----------------------------------------------------------
    if(pLine->Selected) {
        // 끝점 사각형 -----------------
        Gdiplus::Pen            penEdge(Gdiplus::Color(255, 0, 0, 0), 1);
        Gdiplus::SolidBrush     brushEdge(Gdiplus::Color(255, 255, 255, 0));
        Gdiplus::RectF          rectStart(pLine->Start.x-3, pLine->Start.y-3, 6,6);
        Gdiplus::RectF          rectEnd(pLine->End.x-3, pLine->End.y-3, 6,6);

        pGP->FillRectangle(&brushEdge, rectStart);
        pGP->DrawRectangle(&penEdge,   rectStart);

        pGP->FillRectangle(&brushEdge, rectEnd);
        pGP->DrawRectangle(&penEdge,   rectEnd);

        // 외곽 점선 라인.--------------
        penEdge.SetWidth(1.5);
        penEdge.SetColor(Gdiplus::Color(255, 0, 0, 0));

        // Dash Pattern
        REAL dashValues[2] = {2, 2};
        penEdge.SetDashStyle(DashStyleCustom);
        penEdge.SetDashPattern(dashValues, 2);

        switch(pLine->ObjType) {
            case 0: // Line
                {
                    double dTheta = 0;
                    if((pLine->End.x - pLine->Start.x) == 0) {
                        dTheta = PI / 2.0;
                    }
                    else {
                        dTheta = atan(((pLine->End.y - pLine->Start.y))/(double)(pLine->End.x - pLine->Start.x));
                    }

                    dTheta = dTheta + (PI / 2.0);

                    int n90X1       = cos(dTheta)    * 6.0;
                    int n90Y1       = sin(dTheta)    * 6.0;
                    int n90X2       = -n90X1;
                    int n90Y2       = -n90Y1;

                    int nStartX1    = pLine->Start.x + n90X1;
                    int nStartY1    = pLine->Start.y + n90Y1;
                    int nStartX2    = pLine->Start.x + n90X2;
                    int nStartY2    = pLine->Start.y + n90Y2;

                    int nEndX1      = pLine->End.x + n90X1;
                    int nEndY1      = pLine->End.y + n90Y1;
                    int nEndX2      = pLine->End.x + n90X2;
                    int nEndY2      = pLine->End.y + n90Y2;



                    pGP->DrawLine(&penEdge, nStartX1, nStartY1, nStartX2, nStartY2);
                    pGP->DrawLine(&penEdge, nStartX1, nStartY1, nEndX1, nEndY1);
                    pGP->DrawLine(&penEdge, nEndX1, nEndY1, nEndX2, nEndY2);
                    pGP->DrawLine(&penEdge, nEndX2, nEndY2, nStartX2, nStartY2);

                    penEdge.SetColor(Gdiplus::Color(255, 255, 255, 255));
                    penEdge.SetDashOffset(2);

                    pGP->DrawLine(&penEdge, nStartX1, nStartY1, nStartX2, nStartY2);
                    pGP->DrawLine(&penEdge, nStartX1, nStartY1, nEndX1, nEndY1);
                    pGP->DrawLine(&penEdge, nEndX1, nEndY1, nEndX2, nEndY2);
                    pGP->DrawLine(&penEdge, nEndX2, nEndY2, nStartX2, nStartY2);
                }
                break;

            case 1: // Rect
                {
                    int nStartX = (pLine->End.x > pLine->Start.x) ? pLine->Start.x : pLine->End.x;
                    int nStartY = (pLine->End.y > pLine->Start.y) ? pLine->Start.y : pLine->End.y;

                    pGP->DrawRectangle(&penEdge, nStartX-4, nStartY-4, pLine->GetLenX()+8, pLine->GetLenY()+8);

                    penEdge.SetColor(Gdiplus::Color(255, 255, 255, 255));
                    penEdge.SetDashOffset(2);

                    pGP->DrawRectangle(&penEdge, nStartX-4, nStartY-4, pLine->GetLenX()+8, pLine->GetLenY()+8);

                }
                break;

            case 2: // Circle
                {
                    int nStartX = (pLine->End.x > pLine->Start.x) ? pLine->Start.x : pLine->End.x;
                    int nStartY = (pLine->End.y > pLine->Start.y) ? pLine->Start.y : pLine->End.y;

                    pGP->DrawRectangle(&penEdge, nStartX-4, nStartY-4, pLine->GetLenX()+8, pLine->GetLenY()+8);

                    penEdge.SetColor(Gdiplus::Color(255, 255, 255, 255));
                    penEdge.SetDashOffset(2);

                    pGP->DrawRectangle(&penEdge, nStartX-4, nStartY-4, pLine->GetLenX()+8, pLine->GetLenY()+8);

                }

//                pGP->DrawEllipse(&penEdge, pLine->Start.x-3, pLine->Start.y-3, pLine->GetLenX()+6, pLine->GetLenY()+6);
//
//                penEdge.SetDashStyle(DashStyleDash);
//                penEdge.SetColor(Gdiplus::Color(255, 0, 255, 255));
//                pGP->DrawEllipse(&penEdge, pLine->Start.x-3, pLine->Start.y-3, pLine->GetLenX()+6, pLine->GetLenY()+6);
                break;
        }

    }

    //-----------------------------------------------------------
    // Text 출력.
    //-----------------------------------------------------------
    if(pLine->Text.Length()) {

        Gdiplus::FontStyle  fontStyle;

        if(pLine->Font->Style.Contains(fsStrikeOut)) {
            fontStyle = FontStyleStrikeout;
        }
        else if(pLine->Font->Style.Contains(fsUnderline)) {
            fontStyle = FontStyleUnderline;
        }
        else if(pLine->Font->Style.Contains(fsBold)
             && pLine->Font->Style.Contains(fsItalic)) {
            fontStyle = FontStyleBoldItalic;
        }
        else if(pLine->Font->Style.Contains(fsItalic)) {
            fontStyle = FontStyleItalic;
        }
        else if(pLine->Font->Style.Contains(fsBold)) {
            fontStyle = FontStyleBold;
        }
        else {
            fontStyle = FontStyleRegular;
        }

        Gdiplus::Font       txtFont(pLine->Font->Name.w_str(),
                                    pLine->Font->Size,
                                    fontStyle,
                                    UnitPoint // UnitPixel
                                    );

        TColor clFontColor = pLine->Font->Color;

        // 도형이 거꾸로 그려진 경우도 Text를 출력하기 위해 Start와 End 중에 값이 작은 것을 Start로 여기로 Text를 출력해야한다.
        int nStartX = pLine->GetCenterX() - pLine->GetLen()/2;
        int nStartY = 0;

        switch(pLine->ObjType) {
            case 0:     // Line
                nStartY = pLine->GetCenterY() - pLine->GetLen()/2 - 10; // Y 축으로 Center에서 10 px 정도 위로 뜨게 출력
                break;

            default:    // Rect, Circle
                nStartY = pLine->GetCenterY() - pLine->GetLen()/2;      // 정 Center에 출력
                break;
        }

        Gdiplus::RectF      layoutRect(nStartX, nStartY, pLine->GetLen(), pLine->GetLen());
        SolidBrush          txtBrush(Gdiplus::Color(GetRValue(clFontColor), GetGValue(clFontColor), GetBValue(clFontColor)));

        StringFormat        SF;
        SF.SetAlignment(StringAlignmentCenter);
        SF.SetLineAlignment(StringAlignmentCenter);

        // Line인 경우 선의 각도에 따라 글자도 Rotate를 적용해 준다.
        if(pLine->ObjType == 0) {
            Gdiplus::GraphicsState gpState = pGP->Save();

            double dDeltaX = pLine->End.x - pLine->Start.x;
            double dDeltaY = pLine->End.y - pLine->Start.y;

            double dTheta = 0;

            if(dDeltaX != 0) {
                dTheta = atan( dDeltaY / dDeltaX) * 180.0 / PI;
            }
            else {
                dTheta = 90.0;
            }



            Matrix M;
            double rad=dTheta * PI / 180.0;

            M.SetElements(  (REAL)cos(rad),
                            (REAL)sin(rad),
                           -(REAL)sin(rad),
                            (REAL)cos(rad),
                           -pLine->GetCenterX() * (REAL)cos(rad) + pLine->GetCenterY() * (REAL)sin(rad) + pLine->GetCenterX(),
                           -pLine->GetCenterX() * (REAL)sin(rad) - pLine->GetCenterY() * (REAL)cos(rad) + pLine->GetCenterY());


            pGP->SetTransform(&M);

            pGP->DrawString(pLine->Text.w_str(), -1, &txtFont, layoutRect, &SF, &txtBrush);

            pGP->Restore(gpState);

            String sMsg;
            sMsg.printf(L"T=%d, cX=%d, cY=%d", (int)dTheta, pLine->GetCenterX(), pLine->GetCenterY());
            StatusBar1->Panels->Items[3]->Text = sMsg;
        }
        else {
            pGP->DrawString(pLine->Text.w_str(), -1, &txtFont, layoutRect, &SF, &txtBrush);
        }

    }


}
//---------------------------------------------------------------------------
bool __fastcall TfmAZLabelImageEditor::IsRegionOnLabel(TMyLabel *pLabel, int X, int Y)
{
    if(pLabel == NULL) return false;

    bool bRet = false;

    if((X > (pLabel->Pos.x  - (m_LabelSize / 2)))
    && (X < (pLabel->Pos.x  + (m_LabelSize / 2) - 1))
    && (Y > (pLabel->Pos.y  - (m_LabelSize / 2)))
    && (Y < (pLabel->Pos.y  + (m_LabelSize / 2) - 1))) {
        bRet = true;
    }

    return bRet;

}
//---------------------------------------------------------------------------
bool __fastcall TfmAZLabelImageEditor::FindSelectLabel(int X, int Y)
{

    bool bRet = false;

    list<TMyLabel>::reverse_iterator  iter = m_listLabel.rbegin();  // 마지막 list 요소가 가장 위에 존재하는 녀석이니 역순으로 탐색을 한다.

    while(iter != m_listLabel.rend()) {

        if(!bRet && IsRegionOnLabel((TMyLabel*)&(*iter), X, Y)) {
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

bool __fastcall TfmAZLabelImageEditor::MoveSelectLabelOffset(int dX, int dY)
{

    list<TMyLabel>::iterator iter = m_listLabel.begin();

    while(iter != m_listLabel.end()) {

        if(iter->Selected) {
            iter->Pos.x += dX;
            iter->Pos.y += dY;
            return true;
        }

       iter++;
    }

    return false;
}
//---------------------------------------------------------------------------

bool __fastcall TfmAZLabelImageEditor::IsSelectLabel(__out TMyLabel ** ppLabelObj)
{

    list<TMyLabel>::iterator iter = m_listLabel.begin();

    while(iter != m_listLabel.end()) {

        if(iter->Selected) {
            if(ppLabelObj != NULL) {
                *ppLabelObj = (TMyLabel *)&(*iter);
            }
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
bool __fastcall TfmAZLabelImageEditor::IsRegionOnLine(TMyLine *pLine, int X, int Y)
{

    if(pLine == NULL) return false;

    bool bRet = false;

    GraphicsPath path;
    Pen strokePen(Gdiplus::Color(200, 255, 0, 0), pLine->LineWidth + 5);

    if(pLine->Selected) {
        // 이미 선택되어있는 경우 선택 범위를 점도 넓게 해서 드래그를 편하게 할 수 있도록 한다
        strokePen.SetWidth(pLine->LineWidth + 15);
    }

    switch(pLine->ObjType) {
        case 0:     // Line
            path.AddLine(pLine->Start.x, pLine->Start.y, pLine->End.x, pLine->End.y);

            if(path.IsOutlineVisible(X, Y, &strokePen)) {
                bRet = true;
            }
            break;

        case 1:     // Rect
            {
                int nStartX = (pLine->End.x > pLine->Start.x) ? pLine->Start.x : pLine->End.x;
                int nStartY = (pLine->End.y > pLine->Start.y) ? pLine->Start.y : pLine->End.y;

                path.AddRectangle(Gdiplus::RectF(nStartX, nStartY, pLine->GetLenX(), pLine->GetLenY()));
                Gdiplus::Region  region1(&path);

                if(pLine->Fill && region1.IsVisible(X, Y)) {
                    bRet = true;
                }
                else if(!bRet && path.IsOutlineVisible(X, Y, &strokePen)) {
                    bRet = true;
                }
            }
            break;

        case 2:     // Circle
            {
                int nStartX = (pLine->End.x > pLine->Start.x) ? pLine->Start.x : pLine->End.x;
                int nStartY = (pLine->End.y > pLine->Start.y) ? pLine->Start.y : pLine->End.y;
                path.AddEllipse(nStartX, nStartY, pLine->GetLenX(), pLine->GetLenY());
                Gdiplus::Region  region1(&path);

                if(pLine->Fill && region1.IsVisible(X, Y)) {
                    bRet = true;
                }
                else if(!bRet && path.IsOutlineVisible(X, Y, &strokePen)) {
                    bRet = true;
                }
            }
            break;
    }

    return bRet;
}


//---------------------------------------------------------------------------
// 왼쪽 마우스로 Object를 클릭할 때 어떤 Object가 선택되었는지 확인하는 함수.
// 선택된 Object는 Selected 변수가 true가 된다.

bool __fastcall TfmAZLabelImageEditor::FindSelectLine(int X, int Y)
{
    bool bRet = false;

    list<TMyLine>::reverse_iterator  iter = m_listLine.rbegin(); // 마지막 list 요소가 가장 위에 존재하는 녀석이니 역순으로 탐색을 한다.

    while(iter != m_listLine.rend()) {

        if(!bRet && IsRegionOnLine((TMyLine *)&(*iter), X, Y)) {
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

                iter->MoveStartEdgeShift(X,Y, bShift);

            }
            else if(iter->SelectEnd) {
                iter->MoveEndEdgeShift(X,Y, bShift);
            }

            return true;
        }

       iter++;
    }

    return false;
}
//---------------------------------------------------------------------------

int __fastcall TfmAZLabelImageEditor::IsRegionOnLineEdge(TMyLine* pLine, int X, int Y)
{
    if(pLine == NULL) return false;

    if(pLine->Start.x-5 < X
    && pLine->Start.x+5 > X
    && pLine->Start.y-5 < Y
    && pLine->Start.y+5 > Y) {
        return 1;   // Start Edge 부분과 일치할 경우 1 리턴
    }
    else if(pLine->End.x-5 < X
    && pLine->End.x+5 > X
    && pLine->End.y-5 < Y
    && pLine->End.y+5 > Y) {
        return 2;   // End Edge 부분과 일치할 경우 2 리턴
    }

    return 0;       // 아무것도 일치하지 않을 경우 0 리턴
}
//---------------------------------------------------------------------------

bool __fastcall TfmAZLabelImageEditor::FindSelectLineEdge(int X, int Y)
{
    list<TMyLine>::reverse_iterator iter = m_listLine.rbegin();

    while(iter != m_listLine.rend()) {

        if(iter->Selected) {
            if(IsRegionOnLineEdge((TMyLine *)&(*iter), X, Y) == 1) {
                // Start 부분
                iter->SelectStart = true;
                return true;
            }
            else if(IsRegionOnLineEdge((TMyLine *)&(*iter), X, Y) == 2) {
                // End 부분
                iter->SelectEnd = true;
                return true;
            }
        }

       iter++;
    }

    return false;
}
//---------------------------------------------------------------------------

bool __fastcall TfmAZLabelImageEditor::IsSelectLine(__out TMyLine ** ppLineObj)
{

    list<TMyLine>::iterator iter = m_listLine.begin();

    while(iter != m_listLine.end()) {

        if(iter->Selected) {
            if(ppLineObj != NULL) *ppLineObj = (TMyLine *)&(*iter);
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
// LABEL 그리기  ////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::DrawLabel()
{
    list<TMyLabel>::iterator iter = m_listLabel.begin();

    int nIndex = 1;

    Gdiplus::Graphics       gp(Image1->Picture->Bitmap->Canvas->Handle);

    while(iter != m_listLabel.end()) {
       DrawLabel((TMyLabel *)&(*iter), iter->Index, iter->Pos.x, iter->Pos.y, iter->Selected);
       nIndex++;
       iter++;
    }

    // Copy Paste Image가 존재할 경우.
    if(m_CopyImageExist) {
        // 복사한 이미지가 더클 경우 복사한 이미지 크기에 맞춘다.
        if(m_pBitmap->Width == 0 && m_pBitmap->Height == 0) {
            m_pBitmap->Width  = m_pBitmapCopy->Width;
            m_pBitmap->Height = m_pBitmapCopy->Height;
            Image1->Picture->Bitmap->Width  = m_pBitmapCopy->Width;
            Image1->Picture->Bitmap->Height = m_pBitmapCopy->Height;
        }

        Image1->Picture->Bitmap->Canvas->Draw(m_CopyImagePos.x, m_CopyImagePos.y, m_pBitmapCopy);

        // 복사된 이미지란 것을 인지시키기 위해 이미지 주위에 선택 점선을 그려 준다.
        gp.SetSmoothingMode(Gdiplus::SmoothingModeDefault);

        Gdiplus::Pen    penSelected(Gdiplus::Color(255, 0, 0, 0), 1);
        REAL dashValues[2] = {2, 2};
        penSelected.SetDashStyle(DashStyleCustom);
        penSelected.SetDashPattern(dashValues, 2);

        gp.DrawRectangle(&penSelected,m_CopyImagePos.x, m_CopyImagePos.y, m_pBitmapCopy->Width-1, m_pBitmapCopy->Height-1 );

        penSelected.SetColor(Gdiplus::Color(255, 255, 255, 255));
        penSelected.SetDashOffset(2);

        gp.DrawRectangle(&penSelected, m_CopyImagePos.x, m_CopyImagePos.y, m_pBitmapCopy->Width-1, m_pBitmapCopy->Height-1 );

        gp.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

    }

    StatusBar1->Panels->Items[2]->Text = "Label Obj : " + IntToStr((int)m_listLabel.size());

}
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::DrawLabel(TMyLabel * pLabel, int index, int nX, int nY, bool bSelected)
{

    Gdiplus::Graphics gp(Image1->Picture->Bitmap->Canvas->Handle);
    Gdiplus::Graphics *pGP = &gp;

    // GDI+를 이용하여 Angi-Aliasing 구현

    Gdiplus::Pen            pen(pLabel->OutRecColor, pLabel->OuterSize);
    Gdiplus::SolidBrush     sBrush(pLabel->BaseColor);

    int nHalfSize = pLabel->Size / 2;
    nHalfSize = nHalfSize <=0 ? 1 : nHalfSize;

    switch(pLabel->Style) {

        case 0: // Round Rec
            {
                pGP->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

                Gdiplus::Rect rect(nX-nHalfSize, nY-nHalfSize, pLabel->Size, pLabel->Size);
                Gdiplus::Rect rect_Back(nX-nHalfSize, nY-nHalfSize, pLabel->Size+1, pLabel->Size+1);

                // 그림자 그리기
                pen.SetColor(Gdiplus::Color(170,170,170));
                DrawRoundRectangle(pGP, &pen, rect_Back, 5);

                // 가운데 배경 그리기
                FillRoundRectangle(pGP, &sBrush, rect, 5);

                // 외곽 라운드 그리기.
                pen.SetColor(pLabel->OutRecColor);
                DrawRoundRectangle(pGP, &pen, rect, 5);
            }
            break;


        case 1: // Rec
            {
                pGP->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

                pen.SetColor(Gdiplus::Color(170,170,170));
                pGP->DrawRectangle(&pen, nX-nHalfSize+1, nY-nHalfSize+1, pLabel->Size, pLabel->Size);

                pGP->FillRectangle(&sBrush, nX-nHalfSize, nY-nHalfSize, pLabel->Size, pLabel->Size);

                pen.SetColor(pLabel->OutRecColor);
                pGP->DrawRectangle(&pen, nX-nHalfSize, nY-nHalfSize, pLabel->Size, pLabel->Size);

            }
            break;

        case 2: // Circle
            {
                pGP->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

                pen.SetColor(Gdiplus::Color(170,170,170));
                pGP->DrawEllipse(&pen, nX-nHalfSize+1, nY-nHalfSize+1, pLabel->Size, pLabel->Size);

                pGP->FillEllipse(&sBrush, nX-nHalfSize, nY-nHalfSize, pLabel->Size, pLabel->Size);

                pen.SetColor(pLabel->OutRecColor);
                pGP->DrawEllipse(&pen, nX-nHalfSize, nY-nHalfSize, pLabel->Size, pLabel->Size);

            }
            break;
    }


    if(bSelected) {
        pGP->SetSmoothingMode(Gdiplus::SmoothingModeDefault);

        Gdiplus::Pen    penSelected(Gdiplus::Color(255, 0, 0, 0), 1.5);

        // Dash Pattern
        REAL dashValues[2] = {2, 2};
        penSelected.SetDashStyle(DashStyleCustom);
        penSelected.SetDashPattern(dashValues, 2);

        pGP->DrawRectangle(&penSelected, nX-nHalfSize-4, nY-nHalfSize-4, pLabel->Size + 9, pLabel->Size + 9);

        penSelected.SetColor(Gdiplus::Color(255, 255, 255, 255));
        penSelected.SetDashOffset(2);

        pGP->DrawRectangle(&penSelected, nX-nHalfSize-4, nY-nHalfSize-4, pLabel->Size + 9, pLabel->Size + 9);

        pGP->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    }

    // Label 번호를 출력 한다.


    Gdiplus::FontStyle  fontStyle;

    if(pLabel->Font->Style.Contains(fsStrikeOut)) {
        fontStyle = FontStyleStrikeout;
    }
    else if(pLabel->Font->Style.Contains(fsUnderline)) {
        fontStyle = FontStyleUnderline;
    }
    else if(pLabel->Font->Style.Contains(fsBold)
         && pLabel->Font->Style.Contains(fsItalic)) {
        fontStyle = FontStyleBoldItalic;
    }
    else if(pLabel->Font->Style.Contains(fsItalic)) {
        fontStyle = FontStyleItalic;
    }
    else if(pLabel->Font->Style.Contains(fsBold)) {
        fontStyle = FontStyleBold;
    }
    else {
        fontStyle = FontStyleRegular;
    }

    Gdiplus::Font       txtFont(pLabel->Font->Name.w_str(),
                                pLabel->Font->Size,
                                fontStyle,
                                UnitPoint //UnitPixel
                                );

    TColor clFontColor = pLabel->Font->Color;

    Gdiplus::PointF     txtPoint((float)(nX-nHalfSize), (float)(nY-nHalfSize));
    SolidBrush          txtBrush(Gdiplus::Color(GetRValue(clFontColor), GetGValue(clFontColor), GetBValue(clFontColor)));
    Gdiplus::RectF      layoutRect(nX-nHalfSize, nY-nHalfSize, pLabel->Size, pLabel->Size);
    StringFormat        SF;
    SF.SetAlignment(StringAlignmentCenter);
    SF.SetLineAlignment(StringAlignmentCenter);

    pGP->DrawString(IntToStr(pLabel->Index).w_str(),-1, &txtFont, layoutRect, &SF, &txtBrush);
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

    TMyLabel * pLabel = NULL;

    if(IsSelectLabel(&pLabel)) {
        if(pLabel) pLabel->Style = m_LabelStyle;
    }

    DrawMyLine();
    DrawLabel();

}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::Button_FontClick(TObject *Sender)
{
    FontDialog1->Font = Image1->Picture->Bitmap->Canvas->Font;
    if(FontDialog1->Execute(this->Handle)) {

        Image1->Picture->Bitmap->Canvas->Font->Assign(FontDialog1->Font);

        TMyLabel * pLabel = NULL;

        if(IsSelectLabel(&pLabel)) {
            if(pLabel) pLabel->Font->Assign(Image1->Picture->Bitmap->Canvas->Font);
        }

        DrawMyLine();
        DrawLabel();
    }
}
//---------------------------------------------------------------------------








void __fastcall TfmAZLabelImageEditor::Edit_LabelStartNumChange(TObject *Sender)
{
    m_LabelLastIndex = Edit_LabelStartNum->Text.ToIntDef(0);
}
//---------------------------------------------------------------------------






void __fastcall TfmAZLabelImageEditor::Button_LineBaseColorClick(TObject *Sender)
{

    ColorDialog1->Color = (TColor)(m_LabelBaseColor.GetB() << 16 | m_LabelBaseColor.GetG() << 8 | m_LabelBaseColor.GetR());

    if(ColorDialog1->Execute(this->Handle)) {
        TColor clColor   = ColorDialog1->Color;
        m_LabelBaseColor = Gdiplus::Color(GetRValue(clColor), GetGValue(clColor), GetBValue(clColor));
        Panel_LabelBaseColor->Color = clColor;

        TMyLabel * pLabel = NULL;

        if(IsSelectLabel(&pLabel)) {
            if(pLabel) pLabel->BaseColor = m_LabelBaseColor.GetValue();
        }

        DrawMyLine();
        DrawLabel();
        DisplaySettings();
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

        TMyLabel * pLabel = NULL;

        if(IsSelectLabel(&pLabel)) {
            if(pLabel) pLabel->OutRecColor = m_LabelOutRecColor.GetValue();
        }

        DrawMyLine();
        DrawLabel();
        DisplaySettings();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::Button_LineColorClick(TObject *Sender)
{
    ColorDialog1->Color = (TColor)(m_LineColor.GetB() << 16 | m_LineColor.GetG() << 8 | m_LineColor.GetR());

    if(ColorDialog1->Execute(this->Handle)) {
        double dAlpha   = (double)TrackBar_LineAlpha->Position;
        TColor cl       = ColorDialog1->Color;
        m_LineColor     = Gdiplus::Color((int)(255 * dAlpha / 10.0), GetRValue(cl), GetGValue(cl), GetBValue(cl));
        Panel_LineColor->Color = cl;

        TMyLine * pSelectLine = NULL;

        if(IsSelectLine(&pSelectLine)) {
            if(pSelectLine) pSelectLine->Color = m_LineColor.GetValue();
        }

        DrawMyLine();
        DrawLabel();
        DisplaySettings();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::Panel_LineFillColorClick(TObject *Sender)
{
    ColorDialog1->Color = (TColor)(m_LineFillColor.GetB() << 16 | m_LineFillColor.GetG() << 8 | m_LineFillColor.GetR());

    if(ColorDialog1->Execute(this->Handle)) {
        double dAlpha   = (double)TrackBar_LineFillAlpha->Position;
        TColor cl       = ColorDialog1->Color;
        m_LineFillColor = Gdiplus::Color((int)(255 * dAlpha / 10.0), GetRValue(cl), GetGValue(cl), GetBValue(cl));
        Panel_LineFillColor->Color = cl;

        TMyLine * pSelectLine = NULL;

        if(IsSelectLine(&pSelectLine)) {
            if(pSelectLine) pSelectLine->FillColor = m_LineFillColor.GetValue();
        }

        DrawMyLine();
        DrawLabel();
        DisplaySettings();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::TrackBar_LineAlphaChange(TObject *Sender)
{
    double dPos = (double)TrackBar_LineAlpha->Position;
    int nAlpha = (int)(255 * dPos / 10.0);
    m_LineColor = Gdiplus::Color(nAlpha,  m_LineColor.GetR(), m_LineColor.GetG(), m_LineColor.GetB());

    TMyLine * pSelectLine = NULL;

    if(IsSelectLine(&pSelectLine)) {
            // 선택된 오브젝트의 Color는 유지하고 Alpha 값만 변경 시켜 준다.
            pSelectLine->Color = pSelectLine->Color & 0x00FFFFFF;
            pSelectLine->Color = pSelectLine->Color | (nAlpha << 24);
    }

    DrawMyLine();
    DrawLabel();
    DisplaySettings();
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::TrackBar_LineFillAlphaChange(TObject *Sender)
{
    double dPos = (double)TrackBar_LineFillAlpha->Position;
    int nAlpha = (int)(255 * dPos / 10.0);
    m_LineFillColor = Gdiplus::Color(nAlpha,  m_LineFillColor.GetR(), m_LineFillColor.GetG(), m_LineFillColor.GetB());

    TMyLine * pSelectLine = NULL;

    if(IsSelectLine(&pSelectLine)) {

        if(pSelectLine) {
            // 선택된 오브젝트의 Color는 유지하고 Alpha 값만 변경 시켜 준다.
            pSelectLine->FillColor = pSelectLine->FillColor & 0x00FFFFFF;
            pSelectLine->FillColor = pSelectLine->FillColor | (nAlpha << 24);
        }
    }

    DrawMyLine();
    DrawLabel();
    DisplaySettings();
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::CheckBox_LineFillColorUseClick(TObject *Sender)
{
    TMyLine * pSelectLine = NULL;

    if(IsSelectLine(&pSelectLine)) {
        if(pSelectLine) pSelectLine->Fill = CheckBox_LineFillColorUse->Checked;
    }

    DrawMyLine();
    DrawLabel();
    DisplaySettings();
}
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::DisplayPanelFontColor(TPanel * pPanel)
{
    int clR = GetRValue(pPanel->Color);
    int clG = GetGValue(pPanel->Color);
    int clB = GetBValue(pPanel->Color);

    if((clR + clG + clB)/3 < (255/2)) pPanel->Font->Color = clWhite;
    else                              pPanel->Font->Color = clBlack;
}
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::DisplaySettings()
{
    Panel_LabelBaseColor->Color     = (TColor)((m_LabelBaseColor.GetB() << 16)   | (m_LabelBaseColor.GetG() << 8)   | m_LabelBaseColor.GetR());
    Panel_LabelOutRecColor->Color   = (TColor)((m_LabelOutRecColor.GetB() << 16) | (m_LabelOutRecColor.GetG() << 8) | m_LabelOutRecColor.GetR());
    Panel_LineColor->Color          = (TColor)((m_LineColor.GetB() << 16)        | (m_LineColor.GetG() << 8)        | m_LineColor.GetR());
    Panel_LineFillColor->Color      = (TColor)((m_LineFillColor.GetB() << 16)    | (m_LineFillColor.GetG() << 8)    | m_LineFillColor.GetR());

    DisplayPanelFontColor(Panel_LabelBaseColor);
    DisplayPanelFontColor(Panel_LabelOutRecColor);
    DisplayPanelFontColor(Panel_LineColor);
    DisplayPanelFontColor(Panel_LineFillColor);


    Edit_LabelStartNum->OnChange    = NULL;
    Edit_LabelStartNum->Text        = m_LabelLastIndex;
    Edit_LabelStartNum->OnChange    = Edit_LabelStartNumChange;

    Edit_LabelSize->OnChange        = NULL;
    Edit_LabelSize->Text            = m_LabelSize;
    Edit_LabelSize->OnChange        = Edit_LabelSizeChange;

    Edit_LabelOuterLine->OnChange   = NULL;
    Edit_LabelOuterLine->Text       = m_LabelOuterSize;
    Edit_LabelOuterLine->OnChange   = Edit_LabelOuterLineChange;

    Edit_LineWidth->OnChange        = NULL;
    Edit_LineWidth->Text            = m_LineWidth;
    Edit_LineWidth->OnChange        = Edit_LineWidthChange;

    Edit_Reset_W->Text              = m_pBitmap->Width;
    Edit_Reset_H->Text              = m_pBitmap->Height;

    CheckBox_Magnetic->OnClick      = NULL;
    CheckBox_Magnetic->Checked      = m_LabelMagnetic;
    CheckBox_Magnetic->OnClick      = CheckBox_MagneticClick;

    switch(m_LineObjStyle) {
        case 0: SpeedButton_ObjStyle_0->Down = true; break;
        case 1: SpeedButton_ObjStyle_1->Down = true; break;
        case 2: SpeedButton_ObjStyle_2->Down = true; break;
    }

    ComboBoxEx_StartCap->OnChange   = NULL;
    ComboBoxEx_StartCap->ItemIndex  = m_LineStartCapStyle;
    ComboBoxEx_StartCap->OnChange   = ComboBoxEx_StartCapChange;

    ComboBoxEx_EndCap->OnChange     = NULL;
    ComboBoxEx_EndCap->ItemIndex    = m_LineEndCapStyle;
    ComboBoxEx_EndCap->OnChange     = ComboBoxEx_EndCapChange;

    ComboBoxEx_Dash->OnChange       = NULL;
    ComboBoxEx_Dash->ItemIndex      = m_LineStyle;
    ComboBoxEx_Dash->OnChange       = ComboBoxEx_DashChange;

    RadioButton_FillTypeColor->Enabled  = CheckBox_LineFillColorUse->Checked;
    //dioButton_FillTypeBrush->Enabled  = CheckBox_LineFillColorUse->Checked;
    Panel_LineFillColor->Enabled        = CheckBox_LineFillColorUse->Checked;
    //nel_LineFillBrush->Enabled        = CheckBox_LineFillColorUse->Checked;

    Edit_Temp->SetFocus();

    StatusBar1->Panels->Items[3]->Text = "Parent Component Size ( Width : " + IntToStr(m_ParentWidth) + ", Height : " + IntToStr(m_ParentHeight) + ")";
}
//---------------------------------------------------------------------------




void __fastcall TfmAZLabelImageEditor::LoadImageFile1Click(TObject *Sender)
{
    OpenDialog1->Filter = "AllFile | *.bmp; *.JPG; *.PNG;";

    if(OpenDialog1->Execute(this->Handle)) {
        TImage * pImage = new TImage(this);
//

        String sFileName = OpenDialog1->FileName.UpperCase();
        sFileName = sFileName.SubString(sFileName.Length()-4, 4);
        if(sFileName == ".PNG") {
            // PNG Image일 경우 로딩 부분 처리.
            TPngImage * pPNGImage = new TPngImage();
            pPNGImage->LoadFromFile(OpenDialog1->FileName);
            pImage->Picture->Graphic->Assign(pPNGImage);
            delete pPNGImage;
        }
        else {
            pImage->Picture->LoadFromFile(OpenDialog1->FileName);
        }


        if(m_pBitmap) {
            delete m_pBitmap;
            m_pBitmap = new Vcl::Graphics::TBitmap;
            m_pBitmap->Assign(pImage->Picture->Graphic);
        }

        Image1->Picture->Bitmap->Assign(m_pBitmap);

        delete pImage;

        DrawMyLine();
        DrawLabel();
        DisplaySettings();
    }
}
//---------------------------------------------------------------------------
bool __fastcall TfmAZLabelImageEditor::ChangeImageSize(  Vcl::Graphics::TBitmap * pSrcBitmap,
                                                         Vcl::Graphics::TBitmap * pDestBitmap,
                                                         int nDestW,
                                                         int nDestH,
                                                         bool bGrayScale)
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

            // 품질 선택
            gp.SetInterpolationMode(InterpolationModeHighQuality);

            if(bGrayScale) {
                // Gray Scale관련 참고 Site : https://stackoverflow.com/questions/51836869/c-gdi-convert-image-to-grayscale
                // Gray scale conversion:
                Gdiplus::ColorMatrix matrix =
                {
                    0.3f, 0.3f, 0.3f,   0,   0,
                    0.6f, 0.6f, 0.6f,   0,   0,
                    0.1f, 0.1f, 0.1f,   0,   0,
                    0,    0,    0,      1,   0,
                    0,    0,    0,      0,   1
                };

                Gdiplus::ImageAttributes attr;
                attr.SetColorMatrix(&matrix, Gdiplus::ColorMatrixFlagsDefault, Gdiplus::ColorAdjustTypeBitmap);

                Gdiplus::RectF rect(0, 0, nDestW, nDestH);
                gp.DrawImage(bitmap, rect, 0, 0, nDestW, nDestH, Gdiplus::UnitPixel, &attr);
            }
            else {
                // Onely Size Conversion
                gp.DrawImage(bitmap, 0, 0, nDestW, nDestH);
            }

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

        DeselectAction();

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
void __fastcall TfmAZLabelImageEditor::Button_CanvasSizeClick(TObject *Sender)
{
    int nRet = MessageDlg("Canvas Size를 변경하시겠습니까?", mtConfirmation, TMsgDlgButtons() << mbYes<<mbNo, 0);

    if(nRet == mrYes) {

        int nWidth  = Edit_Reset_W->Text.ToIntDef(0);
        int nHeight = Edit_Reset_H->Text.ToIntDef(0);

        Vcl::Graphics::TBitmap * pBitmapTemp = new Vcl::Graphics::TBitmap;

        int nDeltaSizeW = nWidth  - m_pBitmap->Width;
        int nDeltaSizeH = nHeight - m_pBitmap->Height;

        pBitmapTemp->Width  = m_pBitmap->Width  + nDeltaSizeW;
        pBitmapTemp->Height = m_pBitmap->Height + nDeltaSizeH;

        pBitmapTemp->Canvas->Brush->Color = clWhite;
        pBitmapTemp->Canvas->Brush->Style = bsSolid;
        pBitmapTemp->Canvas->FillRect(Rect(0,0,pBitmapTemp->Width, pBitmapTemp->Height));
        pBitmapTemp->Canvas->Draw(nDeltaSizeW/2, nDeltaSizeH/2, m_pBitmap);

        m_pBitmap->Assign(pBitmapTemp);

        DrawMyLine();
        DrawLabel();
        DisplaySettings();

        delete pBitmapTemp;
    }

    delete fmInputImageSize;
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
        m_LabelLastIndex = 1;
        m_listLine.clear();
        m_listLabel.clear();
        DrawMyLine();
        DisplaySettings();

    }
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::ObjectAllMergeClick(TObject *Sender)
{
    DeselectAction();

    m_pBitmap->Assign(Image1->Picture->Graphic);

    m_listLine.clear();
    m_listLabel.clear();
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::ObjectAllClearClick(TObject *Sender)
{
    m_LabelLastIndex = 1;

    m_listLine.clear();
    m_listLabel.clear();


    DrawMyLine();
    DrawLabel();
    DisplaySettings();
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
void __fastcall TfmAZLabelImageEditor::DeselectAction()
{
    ResetSelectLine();
    ResetSelectLabel();

    if(m_CopyImageExist) {
        m_CopyImageExist        = false;
        m_pBitmap->Canvas->Draw(m_CopyImagePos.x, m_CopyImagePos.y, m_pBitmapCopy);
        m_pBitmapCopy->Width  = 0;
        m_pBitmapCopy->Height = 0;
    }

    DrawMyLine();
    DrawLabel();
}
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::FormKeyPress(TObject *Sender, System::WideChar &Key)
{
    if(Key == 0x03) {   // Ctrl + C
        TClipboard *cb = Clipboard();

        TMyLine  * pLine  = NULL;
        TMyLabel * pLabel = NULL;

        if(IsSelectLine(&pLine)) {
            m_CopyLine.Assign(pLine);

            cb->SetTextBuf(L"TMyLine");
        }
        else if(IsSelectLabel(&pLabel)) {
            m_CopyLabel.Assign(pLabel);
            cb->SetTextBuf(L"TMyLabel");
        }
    }
    else if(Key == 0x16) // Ctrl + V
    {

        if(m_CopyImageExist) {
            m_CopyImageExist        = false;
            m_pBitmap->Canvas->Draw(m_CopyImagePos.x, m_CopyImagePos.y, m_pBitmapCopy);
            m_pBitmapCopy->Width    = 0;
            m_pBitmapCopy->Height   = 0;
            m_bLMouseDown           = false;

            DrawMyLine();
            DrawLabel();
        }


        TClipboard *cb = Clipboard();

        if(cb->HasFormat(CF_TEXT)) {
            String sText = cb->AsText;

            if(sText == L"TMyLine") {

                ResetSelectLine();
                ResetSelectLabel();

                m_bLineSelected  = true;
                m_bLabelSelected = false;

                m_CopyLine.Start.x += 10;
                m_CopyLine.Start.y += 10;
                m_CopyLine.End.x   += 10;
                m_CopyLine.End.y   += 10;

                m_CopyLine.Selected = true;

                m_listLine.push_back(m_CopyLine);

                DrawMyLine();
                DrawLabel();
            }
            else if(sText == L"TMyLabel") {
                ResetSelectLine();
                ResetSelectLabel();

                m_bLineSelected  = false;
                m_bLabelSelected = true;

                m_CopyLabel.Pos.x   += 10;
                m_CopyLabel.Pos.y   += 10;

                m_CopyLabel.Selected = true;

                m_listLabel.push_back(m_CopyLabel);

                DrawMyLine();
                DrawLabel();
            }
        }
        else if (cb->HasFormat(CF_BITMAP))
        {
            try
            {
                ResetSelectLine();
                ResetSelectLabel();

                m_bLineSelected  = false;
                m_bLabelSelected = false;

                m_pBitmapCopy->LoadFromClipboardFormat(CF_BITMAP, cb->GetAsHandle(CF_BITMAP), 0);

                if(m_pBitmapCopy->Width > m_pBitmap->Width || m_pBitmapCopy->Height > m_pBitmap->Height) {

                    int nRet = mrYes;
                    
                    if(m_pBitmap->Width > 0) {
                        nRet = MessageDlg("복사된 이미지가 Canvas보다 큽니다. \nCanvas를 키우시겠습니까?", mtConfirmation, TMsgDlgButtons() << mbYes<<mbNo, 0);
                    }

                    if(nRet == mrYes) {
                        Vcl::Graphics::TBitmap * pBitmapTemp = new Vcl::Graphics::TBitmap;

                        int nDeltaSizeW = m_pBitmapCopy->Width  > m_pBitmap->Width  ? m_pBitmapCopy->Width  - m_pBitmap->Width  : 0;
                        int nDeltaSizeH = m_pBitmapCopy->Height > m_pBitmap->Height ? m_pBitmapCopy->Height - m_pBitmap->Height : 0;

                        pBitmapTemp->Width  = m_pBitmapCopy->Width   > m_pBitmap->Width   ? m_pBitmapCopy->Width   : m_pBitmap->Width;
                        pBitmapTemp->Height = m_pBitmapCopy->Height  > m_pBitmap->Height  ? m_pBitmapCopy->Height  : m_pBitmap->Height;

                        pBitmapTemp->Canvas->Brush->Color = clWhite;
                        pBitmapTemp->Canvas->Brush->Style = bsSolid;
                        pBitmapTemp->Canvas->FillRect(Rect(0,0,pBitmapTemp->Width, pBitmapTemp->Height));
                        pBitmapTemp->Canvas->Draw(0, 0, m_pBitmap);

                        m_pBitmap->Assign(pBitmapTemp);

                        delete pBitmapTemp;
                    }
                }

                m_CopyImageExist = true;
                m_CopyImagePos.x = 0;
                m_CopyImagePos.y = 0;

                DrawMyLine();
                DrawLabel();
                DisplaySettings();
            }
            catch (...)
            {
                MessageBeep(0);
                ShowMessage("Error reading image from clipboard!");
            }
            cb->Clear();
        }
        else
        {
            MessageBeep(0);
            ShowMessage("Cannot find image in clipboard!");
        }
    }
    else if(Key == 0x1B) { // ESC Key
        DeselectAction();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
    switch(Key)
    {
        case 0x2E:
        { // Delete Key
            if(m_CopyImageExist) {
                m_CopyImageExist        = false;

                m_pBitmapCopy->Width  = 0;
                m_pBitmapCopy->Height = 0;

                DrawMyLine();
                DrawLabel();
            }
            // 삭제 여부 확인 팝업창 출력.
            if(m_bLabelSelected) {
                DeleteSelectLabel();

                m_bLabelSelected = false;
                m_bLMouseDown    = false;

                DrawMyLine();
                DrawLabel();
            }
            else if(m_bLineSelected) {
                DeleteSelectLine();

                m_bLineSelected = false;
                m_bLMouseDown   = false;

                DrawMyLine();
                DrawLabel();
            }
        }
        break;

    }

}
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::ScrollBox1Click(TObject *Sender)
{
    DeselectAction();
}
//---------------------------------------------------------------------------


void __fastcall TfmAZLabelImageEditor::ImageSize1Click(TObject *Sender)
{
    fmInputImageSize = new TfmInputImageSize(this, m_pBitmap->Width, m_pBitmap->Height);

    if(fmInputImageSize->ShowModal() == mrOk) {
        ResetSelectLine();
        ResetSelectLabel();

        DrawMyLine();
        DrawLabel();

        // 현재까지 작업하던것을 모두 이미지하 하고 Object를 삭제 한다.
        m_pBitmap->Assign(Image1->Picture->Graphic);

        m_listLine.clear();
        m_listLabel.clear();

        // Image Size를 변경.
        int nWidth  = fmInputImageSize->m_nSizeWidth;
        int nHeight = fmInputImageSize->m_nSizeHeight;

        Vcl::Graphics::TBitmap * pDestBitmap = new Vcl::Graphics::TBitmap;

        if(ChangeImageSize(m_pBitmap, pDestBitmap, nWidth, nHeight, false)) {

            Image1->Picture->Bitmap->Width  = pDestBitmap->Width;
            Image1->Picture->Bitmap->Height = pDestBitmap->Height;

            Image1->Picture->Bitmap->Assign(pDestBitmap);

            Image1->Repaint();
            m_pBitmap->Assign(pDestBitmap);
        }

        delete pDestBitmap;

        DisplaySettings();
        
    }

    delete fmInputImageSize;

}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::CanvasSize1Click(TObject *Sender)
{
    fmInputImageSize = new TfmInputImageSize(this, m_pBitmap->Width, m_pBitmap->Height);

    fmInputImageSize->Caption = "Canvas Size";

    if(fmInputImageSize->ShowModal() == mrOk) {
        Vcl::Graphics::TBitmap * pBitmapTemp = new Vcl::Graphics::TBitmap;

        int nDeltaSizeW = fmInputImageSize->m_nSizeWidth  - m_pBitmap->Width;
        int nDeltaSizeH = fmInputImageSize->m_nSizeHeight - m_pBitmap->Height;

        pBitmapTemp->Width  = m_pBitmap->Width  + nDeltaSizeW;
        pBitmapTemp->Height = m_pBitmap->Height + nDeltaSizeH;

        pBitmapTemp->Canvas->Brush->Color = clWhite;
        pBitmapTemp->Canvas->Brush->Style = bsSolid;
        pBitmapTemp->Canvas->FillRect(Rect(0,0,pBitmapTemp->Width, pBitmapTemp->Height));
        pBitmapTemp->Canvas->Draw(nDeltaSizeW/2, nDeltaSizeH/2, m_pBitmap);

        m_pBitmap->Assign(pBitmapTemp);

        DrawMyLine();
        DrawLabel();
        DisplaySettings();

        delete pBitmapTemp;
    }

    delete fmInputImageSize;
}
//---------------------------------------------------------------------------


void __fastcall TfmAZLabelImageEditor::SpeedButton_ObjStyle_0Click(TObject *Sender)
{
    TSpeedButton * pBtn = dynamic_cast<TSpeedButton *>(Sender);
    if(pBtn == NULL) return;

    m_LineObjStyle = pBtn->Tag;

}
//---------------------------------------------------------------------------


void __fastcall TfmAZLabelImageEditor::MagnaticSize1Click(TObject *Sender)
{
    TfmInputImageSize *fmInputImageSize = new TfmInputImageSize(this, m_LabelMagneticSize, 0);
    fmInputImageSize->Edit_H->Enabled = false;
    fmInputImageSize->CheckBox_Const->Enabled = false;

    if(fmInputImageSize->ShowModal() == mrOk) {
        m_LabelMagneticSize = fmInputImageSize->m_nSizeWidth;
    }

}
//---------------------------------------------------------------------------


void __fastcall TfmAZLabelImageEditor::Delete1Click(TObject *Sender)
{
    // 삭제 여부 확인 팝업창 출력.
    if(m_bLabelSelected) {
//        if(MessageDlg("Do you want to delete Label ?", mtConfirmation, TMsgDlgButtons() << mbYes<<mbNo, 0) == mrYes) {
            DeleteSelectLabel();

            m_bLabelSelected = false;
            m_bLMouseDown    = false;

            DrawMyLine();
            DrawLabel();
//        }
    }
    else if(m_bLineSelected) {
//        if(MessageDlg("Do you want to delete Line ?", mtConfirmation, TMsgDlgButtons() << mbYes<<mbNo, 0) == mrYes) {
            DeleteSelectLine();

            m_bLineSelected = false;
            m_bLMouseDown   = false;

            DrawMyLine();
            DrawLabel();
//        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfmAZLabelImageEditor::AddTextClick(TObject *Sender)
{
    TMyLine  * pLine  = NULL;
    TMyLabel * pLabel = NULL;

    if(IsSelectLine(&pLine)) {
        if(pLine != NULL) {

            POINT CurserPos;
            GetCursorPos(&CurserPos);

            TfmInputStrBox *fmInputStrBox = new TfmInputStrBox(this);
            fmInputStrBox->Left = CurserPos.x;
            fmInputStrBox->Top  = CurserPos.y - fmInputStrBox->Height;
            fmInputStrBox->Memo1->Text = pLine->Text;
            fmInputStrBox->Memo1->Font->Assign(pLine->Font);

            if(fmInputStrBox->ShowModal() == mrOk) {
                pLine->Text = fmInputStrBox->Memo1->Text;
                pLine->Font->Assign(fmInputStrBox->Memo1->Font);

                DrawMyLine();
                DrawLabel();
            }

            delete fmInputStrBox;
        }
    }
    else if(IsSelectLabel(&pLabel)) {
        if(pLabel != NULL) {
            fmInputImageSize = new TfmInputImageSize(this, pLabel->Index, 0);
            fmInputImageSize->Edit_H->Visible           = false;
            fmInputImageSize->Edit2->Visible            = false;
            fmInputImageSize->CheckBox_Const->Visible   = false;
            fmInputImageSize->Label1->Visible           = false;
            fmInputImageSize->Label2->Visible           = false;

            fmInputImageSize->GroupBox1->Caption = "Label Index";
            fmInputImageSize->Caption = "Label Index";

            if(fmInputImageSize->ShowModal() == mrOk) {
                int nInde = fmInputImageSize->Edit_W->Text.ToIntDef(-1);
                if(nInde >= 0) {
                    pLabel->Index = nInde;

                    DrawMyLine();
                    DrawLabel();
                }
            }

            delete fmInputImageSize;

        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::GrayScale1Click(TObject *Sender)
{
    // 현재까지 작업하던것을 모두 이미지하 하고 Object를 삭제 한다.
    m_pBitmap->Assign(Image1->Picture->Graphic);

    // Image Size를 변경.
    int nWidth  = m_pBitmap->Width;
    int nHeight = m_pBitmap->Height;

    Vcl::Graphics::TBitmap * pDestBitmap = new Vcl::Graphics::TBitmap;

    if(ChangeImageSize(m_pBitmap, pDestBitmap, nWidth, nHeight, true)) {

        Image1->Picture->Bitmap->Assign(pDestBitmap);

        Image1->Repaint();
        m_pBitmap->Assign(pDestBitmap);
    }

    delete pDestBitmap;

}
//---------------------------------------------------------------------------



void __fastcall TfmAZLabelImageEditor::ComboBoxEx_StartCapChange(TObject *Sender)
{
    int nIndex = ComboBoxEx_StartCap->ItemIndex;
    if(nIndex < 0 || nIndex > 5) return;

    m_LineStartCapStyle = nIndex;

    TMyLine * pSelectLine = NULL;

    if(IsSelectLine(&pSelectLine)) {
        if(pSelectLine) pSelectLine->CapStartStyle = m_LineStartCapStyle;
    }

    DrawMyLine();
    DrawLabel();
}
//---------------------------------------------------------------------------


void __fastcall TfmAZLabelImageEditor::ComboBoxEx_EndCapChange(TObject *Sender)
{
    int nIndex = ComboBoxEx_EndCap->ItemIndex;
    if(nIndex < 0 || nIndex > 5) return;

    m_LineEndCapStyle = nIndex;

    TMyLine * pSelectLine = NULL;

    if(IsSelectLine(&pSelectLine)) {
        if(pSelectLine) pSelectLine->CapEndStyle = m_LineEndCapStyle;
    }

    DrawMyLine();
    DrawLabel();
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::ComboBoxEx_DashChange(TObject *Sender)
{
    int nIndex = ComboBoxEx_Dash->ItemIndex;
    if(nIndex < 0 || nIndex > 4) return;

    m_LineStyle = nIndex;

    TMyLine * pSelectLine = NULL;

    if(IsSelectLine(&pSelectLine)) {
        if(pSelectLine) pSelectLine->DashStyle = m_LineStyle;
    }

    DrawMyLine();
    DrawLabel();
}
//---------------------------------------------------------------------------



void __fastcall TfmAZLabelImageEditor::Edit_LabelSizeChange(TObject *Sender)
{
    int nSize = Edit_LabelSize->Text.ToIntDef(0);

    if(nSize > 0 && nSize < 100) {
        m_LabelSize = nSize;

        TMyLabel * pLabel = NULL;

        if(IsSelectLabel(&pLabel)) {
            if(pLabel) pLabel->Size = m_LabelSize;
        }

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

        TMyLabel * pLabel = NULL;

        if(IsSelectLabel(&pLabel)) {
            if(pLabel) pLabel->OuterSize = m_LabelOuterSize;
        }

        DrawMyLine();
        DrawLabel();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfmAZLabelImageEditor::Edit_LineWidthChange(TObject *Sender)
{
    int nSize = Edit_LineWidth->Text.ToIntDef(0);

    if(nSize >= 1 && nSize <= 100) {
        m_LineWidth = nSize;

        TMyLine * pSelectLine = NULL;

        if(IsSelectLine(&pSelectLine)) {
            if(pSelectLine) pSelectLine->LineWidth = m_LineWidth;
        }

        DrawMyLine();
        DrawLabel();
    }
}
//---------------------------------------------------------------------------

