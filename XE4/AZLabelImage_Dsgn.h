//---------------------------------------------------------------------------

#ifndef AZLabelImage_DsgnH
#define AZLabelImage_DsgnH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <gdiplus.h>	// GDI+
#include <Vcl.Buttons.hpp>
#include <Vcl.Dialogs.hpp>
#include <VCLTee.TeCanvas.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <list>
#include "Common\\Define.h"

using namespace std;











////---------------------------------------------------------------------------
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
//


//---------------------------------------------------------------------------


class TfmAZLabelImageEditor : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TGroupBox *GroupBox1;
    TCheckBox *CheckBox_Magnetic;
    TGroupBox *GroupBox2;
    TPanel *Panel2;
    TLabel *Label1;
    TSpeedButton *SpeedButton_LabelStyle_1;
    TSpeedButton *SpeedButton_LabelStyle_2;
    TSpeedButton *SpeedButton_LabelStyle_3;
    TPanel *Panel3;
    TLabel *Label2;
    TPanel *Panel4;
    TLabel *Label3;
    TEdit *Edit_LabelStartNum;
    TFontDialog *FontDialog1;
    TButton *Button_Font;
    TGroupBox *GroupBox3;
    TPanel *Panel5;
    TLabel *Label4;
    TPanel *Panel7;
    TScrollBox *ScrollBox1;
    TImage *Image1;
    TColorDialog *ColorDialog1;
    TPanel *Panel11;
    TPanel *Panel_LabelBaseColor;
    TPanel *Panel13;
    TPanel *Panel10;
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TMenuItem *LoadImageFile1;
    TOpenDialog *OpenDialog1;
    TEdit *Edit_Reset_W;
    TLabel *Label5;
    TLabel *Label6;
    TEdit *Edit_Reset_H;
    TButton *Button_ImageSize;
    TMenuItem *N1;
    TMenuItem *SaveExit1;
    TMenuItem *Cancel_Exit;
    TMenuItem *N2;
    TMenuItem *Edit1;
    TMenuItem *ObjectAllClear;
    TMenuItem *ObjectAllMerge;
    TMenuItem *N3;
    TStatusBar *StatusBar1;
    TMenuItem *Frame1;
    TMenuItem *FrameView;
    TMenuItem *FrameColor;
    TMenuItem *FrameWidth1;
    TMenuItem *FrameWidth2;
    TMenuItem *FrameWidth3;
    TMenuItem *N4;
    TPanel *Panel8;
    TLabel *Label10;
    TMenuItem *Size1;
    TMenuItem *ImageSize1;
    TMenuItem *CanvasSize1;
    TMenuItem *N5;
    TMenuItem *ComponentSizeImage1;
    TButton *Button_CanvasSize;
    TPanel *Panel15;
    TPanel *Panel12;
    TSpeedButton *SpeedButton_ObjStyle_0;
    TSpeedButton *SpeedButton_ObjStyle_1;
    TSpeedButton *SpeedButton_ObjStyle_2;
    TTrackBar *TrackBar_LineAlpha;
    TTrackBar *TrackBar_LineFillAlpha;
    TMenuItem *Options1;
    TMenuItem *MagnaticSize1;
    TPopupMenu *PopupMenu1;
    TMenuItem *Delete1;
    TImageList *ImageList_PopupMenu;
    TMenuItem *AddText;
    TMenuItem *N8;
    TMenuItem *N6;
    TMenuItem *GrayScale1;
    TImageList *ImageList_LineCap;
    TPanel *Panel14;
    TLabel *Label7;
    TComboBoxEx *ComboBoxEx_StartCap;
    TPanel *Panel16;
    TLabel *Label8;
    TComboBoxEx *ComboBoxEx_EndCap;
    TPanel *Panel17;
    TLabel *Label9;
    TComboBoxEx *ComboBoxEx_Dash;
    TImageList *ImageList_Dash;
    TPanel *Panel_LineColor;
    TPanel *Panel_LabelOutRecColor;
    TBevel *Bevel1;
    TBevel *Bevel2;
    TBevel *Bevel3;
    TBevel *Bevel4;
    TBevel *Bevel5;
    TEdit *Edit_Temp;
    TCheckBox *CheckBox_LineFillColorUse;
    TPanel *Panel18;
    TPanel *Panel_LineFillColor;
    TPanel *Panel_LineFillBrush;
    TPanel *Panel19;
    TRadioButton *RadioButton_FillTypeBrush;
    TRadioButton *RadioButton_FillTypeColor;
    TEdit *Edit_LabelSize;
    TEdit *Edit_LabelOuterLine;
    TEdit *Edit_LineWidth;
    void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall Image1MouseLeave(TObject *Sender);
    void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
    void __fastcall Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall CheckBox_MagneticClick(TObject *Sender);
    void __fastcall Image1DblClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall SpeedButton_LabelStyle_1Click(TObject *Sender);
    void __fastcall Button_FontClick(TObject *Sender);
    void __fastcall Edit_LabelStartNumChange(TObject *Sender);
    void __fastcall Button_LineBaseColorClick(TObject *Sender);
    void __fastcall Button_LineOutRecColorClick(TObject *Sender);
    void __fastcall Button_LineColorClick(TObject *Sender);
    void __fastcall LoadImageFile1Click(TObject *Sender);
    void __fastcall Button_ImageSizeClick(TObject *Sender);
    void __fastcall SaveExit1Click(TObject *Sender);
    void __fastcall Cancel_ExitClick(TObject *Sender);
    void __fastcall ComponentSizeImageClick(TObject *Sender);
    void __fastcall ObjectAllMergeClick(TObject *Sender);
    void __fastcall ObjectAllClearClick(TObject *Sender);
    void __fastcall FrameViewClick(TObject *Sender);
    void __fastcall FrameColorClick(TObject *Sender);
    void __fastcall FrameWidth1Click(TObject *Sender);
    void __fastcall FormKeyPress(TObject *Sender, System::WideChar &Key);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall ImageSize1Click(TObject *Sender);
    void __fastcall CanvasSize1Click(TObject *Sender);
    void __fastcall ScrollBox1Click(TObject *Sender);
    void __fastcall Button_CanvasSizeClick(TObject *Sender);
    void __fastcall SpeedButton_ObjStyle_0Click(TObject *Sender);
    void __fastcall Panel_LineFillColorClick(TObject *Sender);
    void __fastcall TrackBar_LineAlphaChange(TObject *Sender);
    void __fastcall TrackBar_LineFillAlphaChange(TObject *Sender);
    void __fastcall CheckBox_LineFillColorUseClick(TObject *Sender);
    void __fastcall MagnaticSize1Click(TObject *Sender);
    void __fastcall Delete1Click(TObject *Sender);
    void __fastcall AddTextClick(TObject *Sender);
    void __fastcall GrayScale1Click(TObject *Sender);
    void __fastcall ComboBoxEx_StartCapChange(TObject *Sender);
    void __fastcall ComboBoxEx_EndCapChange(TObject *Sender);
    void __fastcall ComboBoxEx_DashChange(TObject *Sender);
    void __fastcall Edit_LabelSizeChange(TObject *Sender);
    void __fastcall Edit_LabelOuterLineChange(TObject *Sender);
    void __fastcall Edit_LineWidthChange(TObject *Sender);

private:
    void __fastcall Init();
    void __fastcall DisplayPanelFontColor(TPanel * pPanel);
    void __fastcall DisplaySettings();
    void __fastcall DeselectAction();

    int             m_ParentWidth;
    int             m_ParentHeight;

    bool            m_bLineTempDrawing; // Mouse Down하여 Line 최초 그리는 중인 경우 true

    TMyLine         m_CopyLine;
    TMyLabel        m_CopyLabel;


protected:
    virtual void    __fastcall WndProc(TMessage& Message);          ///< WndProc 함수 오버라이드.

public:	// User declarations
    Vcl::Graphics::TBitmap * m_pBitmap;
    Vcl::Graphics::TBitmap * m_pBitmapCopy;

    bool        m_CopyImageExist;
    TMyPoint    m_CopyImagePos;

    bool        m_bLMouseDown;
    bool        m_bRMouseDown;
    TMyLine     m_LineTemp;
    TMyLabel    m_LabelTemp;
    int         m_LabelSize;
    int         m_LabelOuterSize;
    int         m_LabelLastIndex;
    int         m_LabelStyle;           // 0 : Round Rec, 1 : Rec, 2 : Circle
    int         m_LineStyle;            // 0 : 직선, 1 : 긴 점선, 2 : 작은 점선 3 : 긴/작은 점선
    int         m_LineWidth;
    int         m_LineStartCapStyle;
    int         m_LineEndCapStyle;
    int         m_LineObjStyle;         // 0:Line, 1:Rect, 2:Circle
    double      m_LineCapScal;
    Gdiplus::Color m_LabelBaseColor;    // 라벨 안쪽 배경 색상
    Gdiplus::Color m_LabelOutRecColor;  // 라벨 외곽 색상.
    Gdiplus::Color m_LineColor;         // Line Color
    Gdiplus::Color m_LineFillColor;     // Line Fill Color


    bool        m_FrameView;
    int         m_FrameWidth;
    TColor      m_FrameColor;


    bool        m_LabelMagnetic;        // Object를 10 픽셀 단위로 자석모드로 이동 한다.
    int         m_LabelMagneticSize;    //
    bool        m_bLineSelected;        // Line이 선택된  상태다
    bool        m_bLineEdegSelected;    // Line Edge 선택
    bool        m_bLabelSelected;       // Label이 선택된 상태다
    bool        m_bCopyImageSelected;   // 복사한 이미지 선택 상태.

    list<TMyLine>   m_listLine;
    list<TMyLabel>  m_listLabel;

    void __fastcall DrawMyLine();
    void __fastcall DrawMyLine(TMyLine *pLine);
    void __fastcall DrawLabel();
    void __fastcall DrawLabel(TMyLabel * pLabel, int n, int nX, int nY, bool bSelected = false);
    void __fastcall DrawOverlayText();

    bool __fastcall IsRegionOnLabel(TMyLabel *pLabel, int X, int Y);
    bool __fastcall FindSelectLabel(int X, int Y);
    bool __fastcall MoveSelectLabel(int X, int Y);
    bool __fastcall MoveSelectLabelOffset(int dX, int dY);
    bool __fastcall IsSelectLabel(__out TMyLabel ** ppLabelObj = NULL);
    void __fastcall ResetSelectLabel();
    bool __fastcall DeleteSelectLabel();

    bool __fastcall IsRegionOnLine(TMyLine *pLine, int X, int Y);
    bool __fastcall FindSelectLine(int X, int Y);
    bool __fastcall MoveSelectLine(int dX, int dY);
    bool __fastcall MoveSelectLineEdge(int dX, int dY, bool bShiftKeyDown);
    bool __fastcall MoveSelectLineEdgeShift(int X, int Y, bool bShift);
    bool __fastcall IsSelectLine(__out TMyLine ** pLineObj = NULL);
    int  __fastcall IsRegionOnLineEdge(TMyLine* pLine, int X, int Y);
    bool __fastcall FindSelectLineEdge(int X, int Y);
    void __fastcall ResetSelectLine();
    void __fastcall ResetSelectLineEdge();
    bool __fastcall DeleteSelectLine();

    bool __fastcall ChangeImageSize(Vcl::Graphics::TBitmap * pSrcBitmap,
                                    Vcl::Graphics::TBitmap * pDestBitmap,
                                    int nDestW,
                                    int nDestH,
                                    bool bGrayScale = false);

//  TFrequencyTimer tmTimer;

public:		// User declarations
    __fastcall TfmAZLabelImageEditor(TComponent* Owner);
    __fastcall TfmAZLabelImageEditor(TComponent* Owner, Vcl::Graphics::TBitmap * pBitmap, int nW, int nH);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmAZLabelImageEditor *fmAZLabelImageEditor;
//---------------------------------------------------------------------------
#endif
