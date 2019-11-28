
#ifndef DefineH
#define DefineH

#ifndef  SAFE_DELETE
#define SAFE_DELETE(p)              { if(p) { delete (p);    (p)=NULL; } }      ///< 포인터 해제
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)        { if(p) { delete[] (p);    (p)=NULL; } }    ///< 배열 포인터 해제
#endif


#define AZ_NUM_SHAPE_FONT     "Let's go Digital"
//-----------------------------------------------------------------------
#define WM_NUMSHAPE_RESET   WM_APP + 100
//-----------------------------------------------------------------------

/**
* @brief : itInt   : Number Pad를 정수형 타입을 입력하도록 생성한다.
           itFloat : Number Pad를 실수형 타입을 입력하도록 생성한다.
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


enum DECLSPEC_DENUM TSensorLedType : unsigned char { slCircle, slRect };

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
    bool     Selected;      // 현재 선택된 경우
    bool     SelectStart;   // 선택된 상태에서 Start 부분을 클릭하여 드래그 할 경우
    bool     SelectEnd;     // 선택된 상태에서 End 부분을 클릭하여 드래그 할 경우

public:
    __fastcall TMyLine() {
        Selected    = false;
        SelectStart = false;
        SelectEnd   = false;
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
};
//---------------------------------------------------------------------------
class TMyLabel {
public:
    TMyPoint Pos;
    bool     Selected;
    int      Index;

public:
    __fastcall TMyLabel() {
        Selected = false;
        Index = 0;
    };
};



//---------------------------------------------------------------------------






#endif
