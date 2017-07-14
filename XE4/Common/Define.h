
#ifndef DefineH
#define DefineH

#define SAFE_DELETE(p)              { if(p) { delete (p);    (p)=NULL; } }      ///< 포인터 해제
#define SAFE_DELETE_ARRAY(p)        { if(p) { delete[] (p);    (p)=NULL; } }    ///< 배열 포인터 해제

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

#endif
