
#ifndef DefineH
#define DefineH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
//#include <Vcl.StdCtrls.hpp>
//#include <Vcl.Forms.hpp>
//#include <Vcl.ExtCtrls.hpp>

#ifndef  SAFE_DELETE
#define SAFE_DELETE(p)              { if(p) { delete (p);    (p)=NULL; } }      ///< ������ ����
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)        { if(p) { delete[] (p);    (p)=NULL; } }    ///< �迭 ������ ����
#endif


#define AZ_NUM_SHAPE_FONT     "Let's go Digital"
//-----------------------------------------------------------------------
#define WM_NUMSHAPE_RESET   WM_APP + 100
//-----------------------------------------------------------------------

/**
* @brief : itInt   : Number Pad�� ������ Ÿ���� �Է��ϵ��� �����Ѵ�.
           itFloat : Number Pad�� �Ǽ��� Ÿ���� �Է��ϵ��� �����Ѵ�.
**/
enum DECLSPEC_DENUM TInputNumType : unsigned char { itInt, itFloat };
//-----------------------------------------------------------------------

enum DECLSPEC_DENUM TDualButtonState : unsigned char { dbNone, dbLeft, dbRight, dbTop = dbLeft, dbDown = dbRight };
//-----------------------------------------------------------------------

enum DECLSPEC_DENUM TDualButtonLayout : unsigned char { dbVertical, dbHorizontal };
//-----------------------------------------------------------------------
// Arrow Button
enum DECLSPEC_DENUM TArrowButtonState : unsigned char { abLeft, abRight, abTop, abBottom, abMax };
enum DECLSPEC_DENUM TArrowButtomView : unsigned char { avVertical, avHorizontal, avBoth };

//-----------------------------------------------------------------------
// AZNumShape
enum DECLSPEC_DENUM TCaptionAlign : unsigned char { caLeft, caRight, caTop, caBottom };
//-----------------------------------------------------------------------

enum DECLSPEC_DENUM TIndexButtonLayout : unsigned char { ibVertical, ibHorizontal };
//-----------------------------------------------------------------------
#define TEXT_ALIGN_TOP      0x01
#define TEXT_ALIGN_LEFT     0x02
#define TEXT_ALIGN_RIGHT    0x04
#define TEXT_ALIGN_BOTTOM   0x08
#define TEXT_ALIGN_H_CENTER 0x10
#define TEXT_ALIGN_V_CENTER 0x20


#define MY_TRANSPARENTCOLOR ((TColor)0xFF01FF)


//-----------------------------------------------------------------------


enum DECLSPEC_DENUM TSensorLedType      : unsigned char { slCircle, slRect };
enum DECLSPEC_DENUM TSensorLedColorType  : unsigned char { slColorGreen, slColorRed, slColorBlue };

//-----------------------------------------------------------------------

// TAZOnOffButton 
enum DECLSPEC_DENUM TOnOffButtonType : unsigned char { ofBtnOn, ofBtnOff, ofBtnOnDisable, ofBtnOffDisable, ofBtnOnDown, ofBtnOffDown, ofBtnMax };
//-----------------------------------------------------------------------




//---------------------------------------------------------------------------
class TMyPoint {
public:
    int x;
    int y;

public:
    __fastcall TMyPoint() {
        x = 0;
        y = 0;
    };
} ;

class TMyLine {
public:
    TMyPoint Start;
    TMyPoint End;
    int      ObjType;       // 0 : Line, 1:Rect, 2:Circle
    bool     Selected;      // ���� ���õ� ���
    bool     SelectStart;   // ���õ� ���¿��� Start �κ��� Ŭ���Ͽ� �巡�� �� ���
    bool     SelectEnd;     // ���õ� ���¿��� End �κ��� Ŭ���Ͽ� �巡�� �� ���
    DWORD    Color;         // ARGB
    DWORD    FillColor;     // ä��� ����
    bool     Fill;          // ä��� ��� ����
    int      DashStyle;     // Line Dash Style
    int      CapStartStyle;
    int      CapEndStyle;
    int      LineWidth;
    Vcl::Graphics::TFont * Font;
    String   Text;

public:
    __fastcall TMyLine() {
        Selected        = false;
        SelectStart     = false;
        SelectEnd       = false;
        Fill            = false;
        ObjType         = 0;
        Color           = 0xFFFFFFFF;
        FillColor       = 0xFFFFFFFF;
        DashStyle       = 0;
        CapStartStyle   = 0;
        CapEndStyle     = 0;
        LineWidth       = 1;
        Text            = "";
        Font            = new Vcl::Graphics::TFont;
        Font->Size      = 10;
        Font->Name      = "Arial";
    };

    // ���� ������ (Font ������ ���� ���縦 ����Ѵ�.)
    __fastcall TMyLine(const TMyLine & from) {
        Start           = from.Start;
        End             = from.End;

        Selected        = from.Selected;
        SelectStart     = from.SelectStart;
        SelectEnd       = from.SelectEnd;
        Fill            = from.Fill;
        ObjType         = from.ObjType;
        Color           = from.Color;
        FillColor       = from.FillColor;
        DashStyle       = from.DashStyle;
        CapStartStyle   = from.CapStartStyle;
        CapEndStyle     = from.CapEndStyle;
        LineWidth       = from.LineWidth;
        Text            = from.Text;

        Font            = new Vcl::Graphics::TFont;
        Font->Assign(from.Font);
    };

    virtual __fastcall ~TMyLine() {
        if(Font) delete Font;
    };

    void __fastcall Assign(const TMyLine * pfrom)
    {
        if(pfrom == NULL) return;

        Start           = pfrom->Start;
        End             = pfrom->End;
        Selected        = pfrom->Selected;
        SelectStart     = pfrom->SelectStart;
        SelectEnd       = pfrom->SelectEnd;
        Fill            = pfrom->Fill;
        ObjType         = pfrom->ObjType;
        Color           = pfrom->Color;
        FillColor       = pfrom->FillColor;
        DashStyle       = pfrom->DashStyle;
        CapStartStyle   = pfrom->CapStartStyle;
        CapEndStyle     = pfrom->CapEndStyle;
        LineWidth       = pfrom->LineWidth;
        Text            = pfrom->Text;

        Font->Assign(pfrom->Font);
    };


    double   __fastcall GetLen() {
        double x    = End.x - Start.x;
        double y    = End.y - Start.y;
        double len  = sqrt(x*x + y*y);
        return len;
    };

    int     __fastcall GetLenX() {
        int n = End.x - Start.x;
        n = n < 0 ? -n : n;
        return n;
    };

    int     __fastcall GetLenY() {
        int n = End.y - Start.y;
        n = n < 0 ? -n : n;
        return n;
    };

    int __fastcall GetCenterX() {
        return Start.x + (End.x - Start.x)/ 2;
    }

    int __fastcall GetCenterY() {
        return Start.y + (End.y - Start.y)/ 2;
    }

    void __fastcall MoveStartEdgeShift(int X, int Y, bool bShift)
    {
        Start.x = X;
        Start.y = Y;

        if(bShift) {    // Shift Ű ���� ���. ���� ������ ������ �ش�.
            int nLenX = GetLenX();
            int nLenY = GetLenY();

            if(ObjType == 0 && (nLenX > nLenY * 2)) {       // Line�� ��츸 ����
                // Y ����
                Start.y = End.y;
            }
            else if(ObjType == 0 && (nLenY > nLenX * 2)) {  // Line�� ��츸 ����
                // X����
                Start.x = End.x;;
            }
            else {
                // 45�� ����
                if(nLenX < nLenY) {
                    if(End.y < Y) {
                        Start.y = End.y + nLenX;
                    }
                    else {
                        Start.y = End.y - nLenX;
                    }
                }
                else {
                    if(End.x < X) {
                        Start.x = End.x + nLenY;
                    }
                    else {
                        Start.x = End.x - nLenY;
                    }
                }
            }
        }
    };

    void __fastcall MoveEndEdgeShift(int X, int Y, bool bShift)
    {
        End.x = X;
        End.y = Y;

        if(bShift) {    // Shift Ű ���� ���. ���� ������ ������ �ش�.
            int nLenX = GetLenX();
            int nLenY = GetLenY();

            if(ObjType == 0 && (nLenX > nLenY * 2)) {         // Line�� ��츸 ����
                // Y ����
                End.y = Start.y;
            }
            else if(ObjType == 0 && (nLenY > nLenX * 2)) {    // Line�� ��츸 ����
                // X����
                End.x = Start.x;;
            }
            else {
                // 45�� ����
                if(nLenX < nLenY) {
                    if(Start.y < Y) {
                        End.y = Start.y + nLenX;
                    }
                    else {
                        End.y = Start.y - nLenX;
                    }
                }
                else {
                    if(Start.x < X) {
                        End.x = Start.x + nLenY;
                    }
                    else {
                        End.x = Start.x - nLenY;
                    }
                }

            }
        }
    };

};
//---------------------------------------------------------------------------
class TMyLabel {
public:
    TMyPoint        Pos;
    bool            Selected;
    int             Index;
    int             Size;           // Label ��ü Size
    int             OuterSize;
    int             Style;          // 0 : Round Rec, 1 : Rec, 2 : Circle
    DWORD           BaseColor;      // �� ���� ��� ����
    DWORD           OutRecColor;    // �� �ܰ� ����.
    Vcl::Graphics::TFont * Font;

public:
    // �⺻ ������.
    __fastcall TMyLabel() {
        Selected    = false;
        Index       = 0;
        Size        = 10;
        OuterSize   = 2;
        Style       = 0;
        BaseColor   = (255 << 24) | (200 < 16) | (100 << 8) | 0;      // ��Ȳ��
        OutRecColor = (255 << 24) | (255 < 16) | (255 << 8) | 255;    // ���
        Font = new Vcl::Graphics::TFont;
    };

    // ���� ������ (stl::list�� ����ϱ� ���ؼ� Font ������ ���� ���縦 ����ؾ� �Ѵ�.)
    __fastcall TMyLabel(const TMyLabel & from) {
        Pos         = from.Pos;
        Selected    = from.Selected;
        Index       = from.Index;
        Size        = from.Size;
        OuterSize   = from.OuterSize;
        Style       = from.Style;
        BaseColor   = from.BaseColor;
        OutRecColor = from.OutRecColor;
        Font = new Vcl::Graphics::TFont;
        Font->Assign(from.Font);
    };

    // �Ҹ���.
    virtual __fastcall ~TMyLabel() {
        if(Font != NULL) delete Font;
        Font = NULL;
    };

    void __fastcall Assign(const TMyLabel * pfrom) {
        Pos         = pfrom->Pos;
        Selected    = pfrom->Selected;
        Index       = pfrom->Index;
        Size        = pfrom->Size;
        OuterSize   = pfrom->OuterSize;
        Style       = pfrom->Style;
        BaseColor   = pfrom->BaseColor;
        OutRecColor = pfrom->OutRecColor;

        Font->Assign(pfrom->Font);
    };
};



//---------------------------------------------------------------------------





//class TFrequencyTimer
//{
//private:
//
//    // PerformanceCounter �� PerformanceFrequency ������ ������ �ʴ����� ��� �ð��� ���� �� �ִ�.
//    LARGE_INTEGER m_liFrequency;        // clock ���ļ� ���ϴ� ����
//    LARGE_INTEGER m_liPerfCounter;      // PC�� On �� �� ���� ���� ������ clock ���� ���ϴ� ����
//
//    bool    m_bStarted;         //  Timer ���� ��Ȳ
//    UINT    m_nLimitTime;       //  ���� : msec
//    double  m_dwStartTime;      //  ���� : msec
//    double  m_dwCrntTime;       //  ���� : msec
//    double  m_dwStartTimeSec;   //  ���� : sec.
//
//    UINT    m_nDelayTime;
//    double  m_dwStartDelayTime;
//
//public:
//    TFrequencyTimer()
//    {
//        QueryPerformanceFrequency(&m_liFrequency);
//        m_bStarted = false;
//        m_nLimitTime  = 0;
//        m_dwStartTime = 0;
//        m_dwCrntTime  = 0;
//        m_dwStartTimeSec = 0;
//        m_dwStartDelayTime = 0;
//        m_nDelayTime = 0;
//    };
//
//    //-----------------------------------------------------------------
//    void StartDelay(UINT nDelayMSEC)
//    {
//        QueryPerformanceCounter(&m_liPerfCounter);
//		m_nDelayTime        = nDelayMSEC;
//		m_bStarted       	= true;
//		m_dwStartDelayTime  = (((double)(m_liPerfCounter.QuadPart * 1000.0) / (double)(m_liFrequency.QuadPart)));
//	}
//
//	bool IsDelayEnd()
//	{
//		QueryPerformanceCounter(&m_liPerfCounter);
//		double m_dwCrntTimeDelay = (((double)(m_liPerfCounter.QuadPart * 1000.0) / (double)(m_liFrequency.QuadPart))); //
//
//		if((m_dwCrntTimeDelay - m_dwStartDelayTime) > m_nDelayTime)
//		{
//            m_bStarted       = false;
//			return true;
//		}
//
//		return false;
//	}
//
//	//-----------------------------------------------------------------
//
//	double  StartTimer(UINT nTimeOutMSEC)   // timer�� �����Ѵ�. (���� �ð��� msec������ �����Ѵ�)
//	{
//        QueryPerformanceCounter(&m_liPerfCounter);
//        m_bStarted       = true;
//        m_nLimitTime     = nTimeOutMSEC;
//        m_dwStartTime    = (((double)(m_liPerfCounter.QuadPart * 1000.0) / (double)(m_liFrequency.QuadPart)));
//        m_dwStartTimeSec = ((double)(m_liPerfCounter.QuadPart / (double)(m_liFrequency.QuadPart)));
//        return m_dwStartTime;
//    };
//
//    double  StartTimerSEC(UINT nTimeOutSEC)   // timer�� �����Ѵ�. (���� �ð��� sec ������ ���� �Ѵ�.)
//    {
//        QueryPerformanceCounter(&m_liPerfCounter);
//        m_bStarted       = true;
//        m_nLimitTime     = nTimeOutSEC * 1000.0;
//        m_dwStartTime    = (((double)(m_liPerfCounter.QuadPart * 1000.0) / (double)(m_liFrequency.QuadPart)));
//        m_dwStartTimeSec = ((double)(m_liPerfCounter.QuadPart / (double)(m_liFrequency.QuadPart)));
//        return m_dwStartTimeSec;
//    };
//
//	double  GetLapTimeMM()              // msec ������ ��� �ð��� ���´�.
//	{
//        QueryPerformanceCounter(&m_liPerfCounter);
//        m_dwCrntTime = (((double)(m_liPerfCounter.QuadPart * 1000.0) / (double)(m_liFrequency.QuadPart))); //
//
//        return (m_dwCrntTime - m_dwStartTime);
//    };
//
//    double  GetLapTimeMM(double dStartTime)              // msec ������ ��� �ð��� ���´�.
//    {
//        QueryPerformanceCounter(&m_liPerfCounter);
//        m_dwCrntTime = (((double)(m_liPerfCounter.QuadPart * 1000.0) / (double)(m_liFrequency.QuadPart))); //
//
//        return (m_dwCrntTime - dStartTime);
//    };
//
//    double  GetLapTimeSEC()             // sec ������ ��� �ð��� ���´�.
//    {
//        QueryPerformanceCounter(&m_liPerfCounter);
//        m_dwCrntTime = (((double)(m_liPerfCounter.QuadPart) / (double)(m_liFrequency.QuadPart))); //
//
//        return (m_dwCrntTime - m_dwStartTimeSec);
//    };
//
//    double  GetLapTimeSEC(double dStartTimeSec)             // sec ������ ��� �ð��� ���´�.
//    {
//        QueryPerformanceCounter(&m_liPerfCounter);
//        m_dwCrntTime = (((double)(m_liPerfCounter.QuadPart) / (double)(m_liFrequency.QuadPart))); //
//
//        return (m_dwCrntTime - dStartTimeSec);
//    };
//
//
//    bool    IsTimeOut()                 // timeout �Ǿ����� Ȯ���Ѵ�.
//    {
//        QueryPerformanceCounter(&m_liPerfCounter);
//        m_dwCrntTime = (((double)(m_liPerfCounter.QuadPart * 1000.0) / (double)(m_liFrequency.QuadPart))); //
//
//        if((m_dwCrntTime - m_dwStartTime) > m_nLimitTime)
//		{
//			m_bStarted       = false;
//			return true;
//        }
//
//        return false;
//    };
//
//    bool    IsTimeOut(double dStartTime)                 // timeout �Ǿ����� Ȯ���Ѵ�.
//    {
//        QueryPerformanceCounter(&m_liPerfCounter);
//        m_dwCrntTime = (((double)(m_liPerfCounter.QuadPart * 1000.0) / (double)(m_liFrequency.QuadPart))); //
//
//        if((m_dwCrntTime - dStartTime) > m_nLimitTime)
//        {
//            m_bStarted       = false;
//            return true;
//        }
//
//        return false;
//    };
//
//    void    StopTimer()
//    {
//    	m_bStarted = false;
//    };
//
//    bool    IsStarted()
//    {
//    	return m_bStarted;
//    };
//
//    bool    IsStopped()
//    {
//    	return !m_bStarted;
//    };
//
//};




#endif
