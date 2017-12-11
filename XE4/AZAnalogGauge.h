//---------------------------------------------------------------------------

#ifndef AZAnalogGaugeH
#define AZAnalogGaugeH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Vcl.Controls.hpp>
//---------------------------------------------------------------------------




        /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
        *
        *    unt_An4logGauge       ver:                        */
#define AN4_VERSION            0.83 // (dic.2008)
        /*    2008 - Matteo Gattanini (matteo.gattanini@gmail.com)
        *
        *    This control is based on 'A3nalogGauge.pas'
        *                 Antialiased Analog Gauge component for Delphi
        *                 2002, Irnis Haliullin (irnis@irnis.net) http://www.irnis.net
        *    which was based on: .AnalogGauge component by Shteg Vyacheslav
        *                        .Supersampling algoritm of Nacho Urenda
        *
        *    Overview
        *    --------------------------------------------
        *    This is a graphic control reproducing an analog
        *    meter for displaying floating point values.
        *    From 'A3nalogGauge' inherits a supersampling
        *    antialiasing algorithm that (may) improve the
        *    appearance.
        *
        *    Licenses
        *    --------------------------------------------
        *    Use and modify freely
        *
        *    Known bugs
        *    --------------------------------------------
        *    > After changing 'Font' or 'Options' may be
        *      necessary to call manually 'Redraw()' or
        *      change 'AntiAliased' property
        *
        *    Release history
        *    --------------------------------------------
        *    0.83 (dic.2008)
        *        .Skip draw bands when extremes coincide
        *        .Handle NaN Value, don't draw pointer
        *        .Removed wrong range control regarding 'LowBand' and 'HighBand'
        *        .New function for assignment: AssignGauge
        *        .New properties: 'Precision' to round tick numbers
        *                         'TicksFont' to choose tick labels font
        *                         'ValidValue' to tell if entered a 'Value'!=NaN
        *        .Removed properties: 'CaptionColor' substituted by 'Font->Color'
        *                             'ValueColor' substituted by 'TicksFont->Color'
        *    0.82 (18.nov.2008)
        *        .Some changes for compatibility with
        *         newer compilers (not finished)
        *    0.81 (18.jul.2008)
        *        .Min/max markers visible also when AntiAliased
        *    0.8 (16.jul.2008)
        *        .Improved substantially performance when
        *         using external backgrounds
        *        .Min/Max values circle cursors
        *        .Added some things in demo
        *    0.7 (jul.2008)
        *        First release
        *
        *    Example of usage:
        *    --------------------------------------------
        *    #include "unt_An4logGauge.h"
		*    cls_An4logGauge* meter = new cls_An4logGauge(Panel1);
        *    //meter->Parent = Panel1; // Not necessary
        *    // Now the gauge appears in Panel1
        *    // Assign proper dimensions, ex:
        *    meter->Width = 200; // [pix]
        *    meter->Height = 100;  // [pix]
        *    // Or dynamic dimensions:
        *    meter->Align = alClient;
        *    // Setting the Layout
        *    // For centered style call the facility:
        *    meter->SetCenteredLayout(120); // [deg]
        *    // For right or left style, simply call:
        *    meter->SetRightLayout(); // or SetLeftLayout()
        *    // More generally, set AngleMin and AngleRange:
        *    meter->AngleMin = 45; // [deg]
        *    meter->AngleRange = 270; // [deg]
        *    // Other settings
        *    meter->Caption = "A";
        *    meter->FullScaleMin = -50.0;
        *    meter->FullScaleMax = +50.0;
        *    // Displaying values
        *    meter->Value = 10.3;
        *    // You can see value using hints:
        *    meter->ShowHint = true;
        *    // Something to do at mouse click
        *    meter->OnMouseDown = MyMeterMouseDown;
        *    //void __fastcall MyMeterMouseDown(TObject*, TMouseButton, TShiftState, int, int);
        *
        *    Remarks
        *    --------------------------------------------
        *    *) Antialising is restricted to trilinear for
        *       optimization, this seems to be the best
        *       tradeoff (moreover, odd magnifications seems
        *       to be the best choice).
        *       Antialiasing is advisable for small sizes
        *
        *    *) If you experience flickering, try to set
        *       parent control 'DoubleBuffered' property
        *       to 'true', e.g.: Panel1->DoubleBuffered = true;
        *
        *    *) Analog meters have the advantage on digital ones
        *       to give an immediate visual perception of the
        *       magnitude of a physical quantity, it would be
        *       a nonsense showing also the measure number!
        *       If you sometimes want to see actual value,
        *       I recommend use Control 'Hint' feature.
        *
        *    *) Reproducing pointer mechanical dynamic ('EnableDynamic')
        *       is something more than a tawdry effect, because
        *       it can provide a useful low-pass filter on measured
        *       values. Dynamic behaviour can be adjusted modifying
        *       'kT' and 'kF' coefficients; higher values mean lower
        *       inertia. Be careful playing with model coefficients
        *       'kT' and 'kF': negative values lead to instability,
        *       while too high values can imply numeric problems;
        *       in both cases a lot of "invalid floating point operation"
        *       messages will populate your screen.
        *
        *    *) Values swept during pointer mechanical transient
        *       do not influence min/max values and triggering
        *       of 'OnChangingBand' or 'OnExceeding' events; only
        *       numbers written in 'Value' are taken into account.
        *
        *    *) Using 'OnMouseDown' event: clicked angle and
        *       value are available in 'ClickedAngle' and
        *       'ClickedValue'; typically you call 'Reset()'
        *       to reset min and max values, or 'Tap()' to give
        *       a nice click visual feedback.
        *
        *    *) Using bitmap as external background prevents
        *       the normal internal drawing mechanism: only
        *       pointer will be drawn. You must adapt manually
        *       pointer length and pivot to fit fixed background.
        *       In particular you must provide the arguments of
        *       'ManualTune()' method; take advantage of 'OnMouseDown'
        *       event to get required points coordinates.
        *       Note that fixed background should have scale divisions
        *       coherent to internal one (e.g. linear).
        *
        *    *) Angle-Value relation is linear; you can add
        *       other transformation changing 'ToAngle' and
        *       'ToValue' function, providing another kind
        *       of biunique transformation; the best way is
        *       using inheritance and polymorphism on 'cls_AGPointer'.
        *
        *    *) You may adapt scale dynamically to measured
        *       values using 'OnExceeding' event.
        *
        * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Messages.hpp>
#include <Windows.hpp>
#include <SysInit.hpp>
#include <System.hpp>
#include <cmath>


/*  ____________________________________________________________
   |                         MarginTop                          |
   |   ......................................................   |
   |   |                      Padding                       |   |
   | M |     _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _      | M |
   | a |    |                       |                  |    | a |
   | r |                 _..---'''''''---.._  (Ticks)       | r |
 H | g | P  |     \ _.-''.    |       |    /``-._      | P  | g |
 e | i | a       ,,' \    \ (Pointer)           /`..     a  | i |
 i | n | d  |   '          \                        `  | d  | n |
 g |   | d    Full          `.    mV              Full   d  |   |
 h | L | i  | scale           \   (Caption)       scale| i  | R |
 t | e | n    min              `.                 max    n  | i |
   | f | g  |(AngleMin)          \                     | g  | g |
   | t |                          O (Hub)                   | h |
   |   |    |                                          |    | t |
   |   |    | _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _|    |   |
   |   |                      Padding                       |   |
   |   ......................................................   |
   |                        MarginBottom                        |
   |____________________________________________________________|
                              Width

 Actual used geometric qunatities (rescaled for antialiasing)
 .Uppercase means integer quantities
 .Lowercase means floating point quantities
 with the following exceptions:
 .P are ponts (couples of integers)
 .R are rectangles (quartets of integers)
     ___________________________________________________________
 ^  |                          M.Top                            |
 |  |   .....................................................   |
 |  |   |                         D                         |   |
 |  |   |   o- - - - - - - - - - - - - - - - - - - - - -o   |   |
 |  |   |   |                 |       |                 |   |   |
 |  |   |          Plb \ _..---'''''''---.._/   Phb         |   |
 |  | M |   |     \ _o-''.                  ``-o_/      |   | M |
 |  | . |        ,,'      \                      `..        | . |
 |  | L |   |   o_         \ l                      o   |   | R |
 H  | e |     Pn  `-._      `.    O Pm              Pp      | i |
 |  | t |   |        ,._      \                         |   | g |
 |  | f |      amin,'       a _`. -._                       | h |
 |  |   | D |      |         /   \r  `.                 | D | t |
 |  |   |         .:.. . . .:.....O....:                    |   |
 |  |   |   |                      Pc                 Ri|   |   |
 |  |   |   o- - - - - - - - - - - - - - - - - - - - - -o   |   |
 |  |   |                         D                       Rd|   |
 |  |   .....................................................   |
 |  |                          M.Bottom                         |
 v  |___________________________________________________________|
    <---------------------------- W ----------------------------> */

//=========================== Compilation settings ==========================

// Use my 'cls_MMTimer' instead of 'TTimer', leave undef, not ready
// TODO 4: Ehmm, is it worth?
//#define AN4_USE_MMTIMER

// Use '::BitBlt' function instead of Canvas->Draw
#define AN4_USE_BITBLT

// Enable framerate measure: defined in demo, normally should be undefined
//#define AN4_FPS_MEASURE


//============================= Defined classes =============================
#ifdef AN4_USE_MMTIMER
//#include <mmsystem.hpp> //TFNTimeCallBack
///////////////////////////////// cls_MMTimer ///////////////////////////////
// A wrapper of win32 Multimedia Timers API
//---------------------------------------------------------------------------
class cls_MMTimer { // Used to animate pointer mech model

public://                                                            ~~~~~~~~
// . . . Types
    typedef void __stdcall (*TFNTimeCallBack)(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2);

// . . . Public methods
    cls_MMTimer() {
                   i_OnTimer = 0; i_EventID = 0;
                   TIMECAPS MinMaxRes; // [ms]
                   ::timeGetDevCaps(&MinMaxRes, sizeof(MinMaxRes)); // !=T IMERR_NOERROR
                   i_Resolution = min(2*MinMaxRes.wPeriodMin,MinMaxRes.wPeriodMax);
                   ::timeBeginPeriod(i_Resolution); // != TIMERR_NOERROR
                   i_Interval = 100 * i_Resolution;
                  }
    ~cls_MMTimer(){
                   if (i_EventID) ::timeKillEvent(i_EventID);
                   ::timeEndPeriod(i_Resolution);
                  }

// . . . Properties
    __property bool Enabled = {read=get_Enabled, write=set_Enabled};
    __property UINT Interval = {read=i_Interval, write=set_Interval}; // Interval [ms]
    __property TFNTimeCallBack OnTimer = {read=i_OnTimer, write=set_OnTimer}; // Callback

private://                                                           ~~~~~~~~
// . . . Private attributes
    MMRESULT i_EventID;
    UINT i_Resolution; // [ms]

// . . . Properties
    bool get_Enabled() const {return i_EventID;}
    void set_Enabled( bool b )
       {
        if ( i_EventID ) {::timeKillEvent(i_EventID); i_EventID = 0;}
        if ( b != Enabled )
           {
            if(b){
                  if (i_OnTimer) i_EventID = ::timeSetEvent(i_Interval,i_Resolution,i_OnTimer,0,TIME_PERIODIC);
                 }
            else {
                  if (i_EventID) {::timeKillEvent(i_EventID); i_EventID=0;}
                 }
           }
       }
    UINT i_Interval; // [ms]
    void set_Interval( UINT I )
       {
        if ( I!=i_Interval && I>=i_Resolution )
           {
            i_Interval = I;
            if ( i_EventID && i_OnTimer )
               {
                ::timeKillEvent(i_EventID);
                i_EventID = ::timeSetEvent(i_Interval,i_Resolution,i_OnTimer,0,TIME_PERIODIC);
               }
           }
       }
    TFNTimeCallBack i_OnTimer;
    void set_OnTimer( TFNTimeCallBack f )
       {
        if ( f!=i_OnTimer && i_OnTimer ) i_OnTimer = f;
       }
}; // end 'cls_MMTimer'
#endif


/////////////////////////////// cls_AGPointer ///////////////////////////////
// This auxiliary class deals with the relationship between
// physical measured value and pointer position (an angle)
//---------------------------------------------------------------------------
class cls_AGPointer { // Just to get some numbers together...

public://                                                            ~~~~~~~~
// . . . Public methods
    cls_AGPointer() {m=1; q=0; kT=100; kF=10; w=a=aref=0; Ts=0.03;}
    //~cls_AGPointer(){}

    // . . . Linear transformation (a = m*value + q)
    inline double ToAngle(const double v) const {return m*v+q;}
    inline double ToValue(const double a) const {return (a-q)/m;}
    inline void set_m(const double v){if(v)m=v;}
    inline void set_q(const double v){q=v;}

    // . . . Mechanical dynamic model
    inline bool Evolve() { // Return false when transient is over
                          w += Ts * (kT*(aref-a) - kF*w);
                          // Warning: using next value in second equation
                          a += Ts*w; // Angle constraints are applied outside
                          return (std::fabs(aref-a)>1E-2 || std::fabs(w)>1E-3);
                         }
    inline double get_Ts() const {return Ts;}
    inline void set_w(const double v){w=v;}
// . . . Public attributes (!)
    // Mechanical dynamic model
    double a; // State variable: angle [rad]
    double aref; // Inputs: target angle [rad]
    double kT,kF; // Elastic torque effect [1/s?, Friction effect [1/s]
/*
          Elastic torque module [Nm/rad]
     kT = ------------------------------ = Elastic torque effect [1/s?
            Moment of inertia [Kg·m?
          Friction Coefficient [Nm·s/rad]
     kF = ------------------------------ = Viscous Friction effect [1/s]
            Moment of inertia [Kg·m?                                      */

private://                                                           ~~~~~~~~
// . . . Private attributes
    // Linear transformation coeffs (a = m*value + q)
    double m,q; // [rad/phys units] , [rad]
    // Mechanical dynamic model
    double w; // State variables: angular speed [rad/s] and angle [rad]
    double Ts; // Discrete model sampling time [s]
// . . . Private methods
}; // end 'cls_AGPointer'






class PACKAGE TAZAnalogGauge : public TGraphicControl
{
public:
// . . . Types
    enum TBand { bnLow, bnMid, bnHigh };
    enum TQuadrant { qdI=1, qdII, qdIII, qdIV };
    enum TAntialiasing { aaNone=1,        // Warning: don't modify these values
                         aaBilinear=2,    // Not used
                         aaTrilinear=3,   // The only permissible (optimization reasons)
                         aaQuadrilinear=4 // Not used
                       };
    enum TOption { opShowMainTicks,   // Draw tick divisions
                   opShowSubTicks,    // Draw subdivision between main ticks
                   opShowTickNumbers, // Show tick numbers
                   opShowMinBand,     // Show a colored bar for low values
                   opShowMidBand,     // Show a colored bar for mid values
                   opShowHighBand,    // Show a colored bar for high values
                   opShowMinMax,      // Show a marker on min and max of measured value (see 'Reset()')
                   opShowHub,         // Draw a circle on pointer pivot
                   opShowCaption      // Show a label on display center
                  };
    typedef Set<TOption, opShowMainTicks, opShowCaption>  TOptions;
    //typedef void __fastcall (__closure *fOnChangingBand)(System::TObject* Sender); // An event function

    typedef struct // Rescaled geometric quantities
        {
         int W,H; // Overall frame Width and Height [pix]
         TRect M; // Margins
         int D; // Internal padding

         double l; // Pointer length (indicator radius)
         double r; // Hub radius

         int B; // Band thickness
         int St; // Subthicks length
         int Mt; // Main ticks length

         TRect Rd,Ri,Rr; // Display, Inner and Rotation rectangles
         TPoint Pc; // Hub point (pointer pivot)
         TPoint Pm; // Median point (centering caption)
         TPoint Pp,Pn; // Fullscale pos/neg point (max/min measure)
         TPoint Phb,Plb; // High/Low bands points

        } stu_Geometry;


private:
    bool                bLoaded;

// . . . Private attributes
    // State
    int                 nWidth_Old;
    int                 nHeight_Old;
    
    cls_AGPointer       p; // Quantities related to pointer position (updated in 'UpdateDimensions')
    stu_Geometry        g; // Geometric quantities (updated in 'UpdateDimensions')
    bool                Initializing; // Avoid triggering 'Resize()' during constructor
    // Graphics
    Graphics::TBitmap*  i_BodyBitmap; // Drawed bitmap
    Graphics::TBitmap*  i_FaceBitmap; // Face bitmap
    Graphics::TBitmap*  i_AABitmap;   // Used for antialiasing
    // Drawing bags
    TRect               RPp, RTp; // Pointer, Ticks extremes
    // Pointer mechanical transient timer
#ifdef AN4_USE_MMTIMER
    cls_MMTimer *       i_Timer;
#else
    TTimer *             i_Timer;
#endif
    // Other
    int                 i_NumTicks; // A bag where store total ticks number
    TPoint              i_Pc_manual; // Pointer pivot set in 'ManualTune' [pix]
    int                 i_L_manual; // Pointer length set in 'ManualTune' [pix]
#ifdef AN4_FPS_MEASURE
      // Performance tracking
      LARGE_INTEGER     i_PrevCount,i_Freq;
#endif

// . . . Properties
    // Main
    String              i_Caption;
    //void __fastcall set_Caption(const String&);
    void                __fastcall set_Caption(String);
    bool                i_ValidValue;
    double              i_Value, i_MaxValue, i_MinValue;
    void                __fastcall set_Value(double);
    double i_FullScaleMax;
    void __fastcall set_FullScaleMax(double);
    double i_FullScaleMin;
    void __fastcall set_FullScaleMin(double);
    double __fastcall get_ValueRange();
    double i_LowBand;
    void __fastcall set_LowBand(double);
    double i_HighBand;
    void __fastcall set_HighBand(double);
    TBand i_CurrentBand;
    double __fastcall get_ClickedValue();
    double i_clickedangle;
    int __fastcall get_ClickedAngle() const;
    // Settings
    TOptions i_Options;
    void __fastcall set_Options(TOptions);
    TAntialiasing i_AntiAliasing;
    bool __fastcall get_AntiAliased() const;
    void __fastcall set_AntiAliased(bool);
    bool i_EnableDynamic;
    void __fastcall set_EnableDynamic(bool);
    double __fastcall get_kT() const ;
    double __fastcall get_kF() const ;
    void __fastcall set_kT(double v) ;
    void __fastcall set_kF(double v) ;
    // Style (Geometry)
    double i_amin; // [rad]
    int __fastcall get_AngleMin() const;
    void __fastcall set_AngleMin(int);
    double i_Da; // [rad]
    int __fastcall get_AngleRange() const;
    void __fastcall set_AngleRange(int);
    double i_aexceed;
    int __fastcall get_AngleExceed() const;
    void __fastcall set_AngleExceed(int);
    int i_HubRadius;
    void __fastcall set_HubRadius(int I);
    int i_Padding;
    void __fastcall set_Padding(int);
    int i_MarginLeft,i_MarginTop,i_MarginRight,i_MarginBottom;
    void __fastcall set_MarginLeft(int);
    void __fastcall set_MarginTop(int);
    void __fastcall set_MarginRight(int);
    void __fastcall set_MarginBottom(int);
    void __fastcall set_Margin(int);
    int i_PointerWidth;
    void __fastcall set_PointerWidth(int);
    int i_PointerLines;
    void __fastcall set_PointerLines(int);
    int i_Precision;
    void __fastcall set_Precision(int p);
    int i_NumMainTicks;
    void __fastcall set_NumMainTicks(int);
    int i_NumSubTicks;
    void __fastcall set_NumSubTicks(int);
    int i_LengthMainTicks;
    void __fastcall set_LengthMainTicks(int);
    int i_LengthSubTicks;
    void __fastcall set_LengthSubTicks(int);
    int i_BandThickness;
    void __fastcall set_BandThickness(int);
    Controls::TBevelCut i_BevelOuter;
    void __fastcall set_BevelOuter(Controls::TBevelCut);
    Controls::TBevelCut i_BevelInner;
    void __fastcall set_BevelInner(Controls::TBevelCut);
    // Style (Appearance)
    class TFont* i_TicksFont;
    void __fastcall set_TickFont(TFont * p);
    
    Graphics::TBitmap* i_BackgroundBmp;
    void __fastcall set_BackgroundBmp(Graphics::TBitmap*);
    Graphics::TColor i_BodyColor;
    void __fastcall set_BodyColor(Graphics::TColor);
    Graphics::TColor i_DisplayColor;
    void __fastcall set_DisplayColor(Graphics::TColor);
    Graphics::TColor i_TicksColor;
    void __fastcall set_TicksColor(Graphics::TColor);
    Graphics::TColor i_PointerColor;
    void __fastcall set_PointerColor(Graphics::TColor);
    Graphics::TColor i_BorderColor;
    void __fastcall set_BorderColor(Graphics::TColor);
    Graphics::TColor i_HubColor;
    void __fastcall set_HubColor(Graphics::TColor);
    Graphics::TColor i_LowBandColor;
    void __fastcall set_LowBandColor(Graphics::TColor);
    Graphics::TColor i_MidBandColor;
    void __fastcall set_MidBandColor(Graphics::TColor);
    Graphics::TColor i_HighBandColor;
    void __fastcall set_HighBandColor(Graphics::TColor);
    
    // Events
    Classes::TNotifyEvent i_OnChangingBand;
    Classes::TNotifyEvent i_OnExceeding;
    // Performance tracking
#ifdef AN4_FPS_MEASURE
      int i_fps;
      Classes::TNotifyEvent i_OnFPSMeasure;
#endif

// . . . Private methods
    // Call back for pointer mechanical transient evolution task
#ifdef AN4_USE_MMTIMER
      void __stdcall  DynamicModel(UINT, UINT, DWORD, DWORD, DWORD);
#else
      void __fastcall DynamicModel(System::TObject*);
#endif


    inline bool ExtBackground(); //{return(i_BackgroundBmp->Width>0);} // Tell if an external background is defined
    inline bool NoExtBackground(); //{return(i_BackgroundBmp->Width==0);}
    inline void PolarToCartesian(const double r, const double a, TPoint* P);
//            { // Coordinates transformation
//             P->x = g.Pc.x - round(r*std::cos(a));
//             P->y = g.Pc.y - round(r*std::sin(a));
//            }
    inline void PolarToCartesian(const double r1, const double r2, const double a, TRect* R);
//            { // Double coordinates transformation (same angle)
//             double cosa = std::cos(a);
//             double sina = std::sin(a);
//             R->Left = g.Pc.x - round(r1*cosa);
//             R->Top = g.Pc.y - round(r1*sina);
//             R->Right = g.Pc.x - round(r2*cosa);
//             R->Bottom = g.Pc.y - round(r2*sina);
//            }
    inline void CartesianToPolar(const int X, const int Y, double* a);
//            { // Coordinates transformation
//             *a = std::atan2(double(g.Pc.y-Y), double(g.Pc.x-X));
//             if (*a < 0) *a += 2*M_PI;
//             //*r = std::sqrt(std::pow(g.Pc.y-P.y,2)+std::pow(g.Pc.x-P.x,2));
//             // *r = (g.Pc.x-P.x) / std::cos(*a);  *r = (g.Pc.y-P.y) / std::sin(*a);
//            }
    static inline int round (double x); // Rounds to nearest integer
//            {
//             if (x<-MaxInt) x=-MaxInt; else if (x>MaxInt) x=MaxInt;
//             return static_cast<int>(x>0?x+.5:x-.5);
//            }
    static inline void limit_value (double* x, const double m, const double M); // Limit value
//            {
//             if (*x<m) *x=m; else if (*x>M) *x=M;
//            }
    static inline void __fastcall ResizeBitmap(Graphics::TBitmap*, Graphics::TBitmap*); // Used to antialias
    static TQuadrant GetAngleQuadrant(double); // Determine angle quadrant
    //static void __fastcall SetPenStyles(Graphics::TPen*, int, TColor); // An utility to create pen
    static template<typename T> inline T min(const T a, const T b); // {return (a<b) ? a : b;}
    static inline bool AreDifferent(const TPoint& A, const TPoint& B); // {return (A.x!=B.x)||(A.y!=B.y);} // States if two points are not equal
// . . . Windows messages handlers
//    BEGIN_MESSAGE_MAP
//        VCL_MESSAGE_HANDLER(CM_FONTCHANGE, TMessage, CMFontChanged);
//	END_MESSAGE_MAP(TGraphicControl);
    
    System::Classes::TNotifyEvent FOnResize;
    void         __fastcall MyResize(TObject *Sender);
    
    TMouseEvent FOnMouseDown;
    void         __fastcall MyMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

    System::Classes::TNotifyEvent FontOnChange_Org;
    
protected:
    virtual void     __fastcall Loaded(void);                ///< Loaded ÇÔ¼ö ¿À¹ö¶óÀÌµå.
    
// . . . Protected methods
    void __fastcall DrawBody(class Graphics::TCanvas*); // Render instrument on a Canvas
    void __fastcall DrawPointer(class Graphics::TCanvas*); // Render measuring pointer on a Canvas
    void __fastcall Redraw() ;
//        {
//#ifdef AN4_FPS_MEASURE
//                                ::QueryPerformanceCounter(&i_PrevCount);
//#endif
//                              if ( NoExtBackground() ) DrawBody(i_BodyBitmap->Canvas);
//                              RedrawPointer(); // Calls 'Paint()'
//                             }
    void __fastcall RedrawPointer(); // This do the work: prepare control bitmap buffer
    void __fastcall UpdateDimensions(); // Recalculate geometric quantities

    // . . . Inherited (Event handlers)
    virtual void __fastcall Paint();
	//DYNAMIC void __fastcall Click();
    //DYNAMIC void __fastcall _MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
    DYNAMIC void __fastcall _Resize();
    //MESSAGE void __fastcall CMFontChanged(Messages::TMessage &Msg);// {i_BodyBitmap->Canvas->Font->Assign(Font); Redraw();}
    
    void     __fastcall MainFontChanged(System::TObject* Sender);
    void     __fastcall FontChanged(System::TObject* Sender);

public:
	__fastcall TAZAnalogGauge(TComponent* Owner);
	virtual __fastcall  ~TAZAnalogGauge();


// . . . Public methods


    //cls_An4logGauge* __fastcall operator=(const cls_An4logGauge* a) {return Assign(a);} // Assignment operator
	TAZAnalogGauge* __fastcall AssignGauge(const TAZAnalogGauge*); // Assignment operator
    void __fastcall Reset(); // Some reset operations (ex. min-max markers)
    void __fastcall Tap(const double v =0); // {i_Timer->Enabled=true; p.set_w(v?v:kF);} // Knock meter (uses dynamic model)
	// Layout facilities
    void __fastcall SetCenteredLayout(const int); // Given angle range [deg]
    void __fastcall SetRightLayout();
    void __fastcall SetLeftLayout();
    void ManualTune(const TPoint&,const int,const int,const TPoint&,const TPoint&,const TPoint&); // Used to tune pointer position when using external background bitmap

// . . . Public attributes (!)
    static const int MinAngleRange; // Minimum value constraint of 'AngleRange' [deg]
    static const double MinValueRange; // Minimum measured values interval [phys units]



__published:

    // . . . Main
    __property String   Caption         = {read=i_Caption, write=set_Caption}; // A caption
    __property bool     ValidValue      = {read=i_ValidValue, default = true}; // Tell if entered a 'Value'!=NaN
	__property double   Value           = {read=i_Value, write=set_Value, default = 0}; // Current measured value [phys units]
	__property double   MaxValue        = {read=i_MaxValue, default = 0}; // Maximum value reached (not pointer position); see 'Reset()'
    __property double   MinValue        = {read=i_MinValue, default = 0}; // Minimum value reached (not pointer position); see 'Reset()'
    __property double   FullScaleMax    = {read=i_FullScaleMax, write=set_FullScaleMax, nodefault}; // Max measurable value [value]
    __property double   FullScaleMin    = {read=i_FullScaleMin, write=set_FullScaleMin, nodefault}; // Min measurable value [value]
    __property double   ValueRange      = {read=get_ValueRange}; // Get (FullScaleMax-FullScaleMin) [phys units]
    __property double   HighBand        = {read=i_HighBand, write=set_HighBand, nodefault}; // Excess band threshold [value]
    __property double   LowBand         = {read=i_LowBand, write=set_LowBand,  nodefault}; // Lower band threshold [value]
    __property TBand    CurrentBand     = {read=i_CurrentBand, default = bnLow}; // Current band containing value
    __property double   ClickedValue    = {read=get_ClickedValue}; // MouseDown clicked value [phys units]
    __property int      ClickedAngle    = {read=get_ClickedAngle}; // MouseDown clicked angle [deg]
    // . . . Settings
    __property TOptions Options         = {read=i_Options, write=set_Options, nodefault}; // Options
    __property bool     AntiAliased     = {read=get_AntiAliased, write=set_AntiAliased, nodefault}; // Aliasing mode (bitmap fixed magnification factor)
    __property bool     EnableDynamic   = {read=i_EnableDynamic, write=set_EnableDynamic, nodefault}; // Engage pointer mech dynamic
    __property double   kT              = {read=get_kT, write=set_kT, nodefault}; // Mech model: Elastic torque effect [1/s?
    __property double   kF              = {read=get_kF, write=set_kF, nodefault}; // Mech model: Friction effect [1/s]
    // . . . Style (Geometry)
    __property int      AngleMin        = {read=get_AngleMin, write=set_AngleMin, nodefault}; // Angle at minimum value [deg]
    __property int      AngleRange      = {read=get_AngleRange, write=set_AngleRange, nodefault}; // Meter area angle (MinAngleRange?60) [deg]
    __property int      AngleExceed     = {read=get_AngleExceed, write=set_AngleExceed, nodefault}; // Permissible pointer position: excess angle [deg]
    __property int      Padding         = {read=i_Padding, write=set_Padding, nodefault}; // Internal padding [pix]
    __property int      Margin          = {write=set_Margin}; // Body margins [pix]
    __property int      MarginLeft      = {read=i_MarginLeft, write=set_MarginLeft};
    __property int      MarginTop       = {read=i_MarginTop, write=set_MarginTop};
    __property int      MarginRight     = {read=i_MarginRight, write=set_MarginRight};
    __property int      MarginBottom    = {read=i_MarginBottom, write=set_MarginBottom};
    __property int      PointerWidth    = {read=i_PointerWidth, write=set_PointerWidth,     default = 1}; // Moving indicator width [pix]
    __property int      PointerLines    = {read=i_PointerLines, write=set_PointerLines,     default = 1}; // Moving indicator Lines
    
    __property int      HubRadius       = {read=i_HubRadius, write=set_HubRadius, nodefault}; // Pointer pivot size [pix]
    __property int      Precision       = {read=i_Precision, write=set_Precision, nodefault}; // Tick labels rounding
    __property int      NumMainTicks    = {read=i_NumMainTicks, write=set_NumMainTicks, nodefault}; // Total main divisions values
    __property int      NumSubTicks     = {read=i_NumSubTicks, write=set_NumSubTicks, nodefault}; // Number of divisions between main ticks
    __property int      LengthMainTicks = {read=i_LengthMainTicks, write=set_LengthMainTicks, nodefault}; // Main ticks length [pix]
    __property int      LengthSubTicks  = {read=i_LengthSubTicks, write=set_LengthSubTicks, nodefault}; // Subticks length [pix]
    __property int      BandThickness   = {read=i_BandThickness, write=set_BandThickness, nodefault}; // Weight of values band [pix]
    // . . . Style (Appearance)
    __property class TFont*         TicksFont       = {read=i_TicksFont, write=set_TickFont}; // Main ticks labels font
    __property Graphics::TBitmap*   BackgroundBmp   = {read=i_BackgroundBmp, write=set_BackgroundBmp, nodefault}; // A background bitmap that overrides normal drawed canvas
    __property Controls::TBevelCut  BevelOuter      = {read=i_BevelOuter, write=set_BevelOuter, nodefault}; // Outer border style (bvNone, bvLowered, bvRaised, bvSpace)
    __property Controls::TBevelCut  BevelInner      = {read=i_BevelInner, write=set_BevelInner, nodefault}; // Inner border style (bvNone, bvLowered, bvRaised, bvSpace)
    __property Graphics::TColor     BodyColor       = {read=i_BodyColor, write=set_BodyColor, nodefault}; // Meter body (margins)
    __property Graphics::TColor     BorderColor     = {read=i_BorderColor, write=set_BorderColor, nodefault}; // Used only with 'bvSpace'
    __property Graphics::TColor     DisplayColor    = {read=i_DisplayColor, write=set_DisplayColor, default = cl3DLight}; // Meter inner area
    __property Graphics::TColor     TicksColor      = {read=i_TicksColor, write=set_TicksColor, nodefault};
    __property Graphics::TColor     PointerColor    = {read=i_PointerColor, write=set_PointerColor, nodefault};
    __property Graphics::TColor     HubColor        = {read=i_HubColor, write=set_HubColor, nodefault};
    __property Graphics::TColor     LowBandColor    = {read=i_LowBandColor, write=set_LowBandColor, nodefault};
    __property Graphics::TColor     MidBandColor    = {read=i_MidBandColor, write=set_MidBandColor, nodefault};
    __property Graphics::TColor     HighBandColor   = {read=i_HighBandColor, write=set_HighBandColor, nodefault};
    // . . . Events
    __property Classes::TNotifyEvent OnChangingBand = {read=i_OnChangingBand, write=i_OnChangingBand}; // When 'Value' changes interval
    __property Classes::TNotifyEvent OnExceeding    = {read=i_OnExceeding, write=i_OnExceeding}; // When 'Value' exceeds scale
    // . . . Performance tracking
#ifdef AN4_FPS_MEASURE
    __property int fps = {read=i_fps};
    __property Classes::TNotifyEvent OnFPSMeasure   = {read=i_OnFPSMeasure, write=i_OnFPSMeasure};
#endif
    // . . . Inherited
    __property Width    = { default = 225 };
    __property Height   = { default = 180 };
    __property Font;
    __property Tag;
    __property Left;
    __property Top;
    __property Align;
    __property Enabled;
    __property Visible; 
    
	__property TMouseEvent OnMouseDown = {read=FOnMouseDown, write=FOnMouseDown};
    __property System::Classes::TNotifyEvent    OnResize = {read=FOnResize, write=FOnResize};


};
//---------------------------------------------------------------------------
#endif
