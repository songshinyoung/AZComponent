//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include <cfloat> // bool _isnan() used in 'get_ValidValue()'
#include "AZAnalogGauge.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

#ifndef ASSERTING
  #define ASSERTING(x,y)
#endif

#define EPSILON (1E-100) // Used to state when a double is zero


const int TAZAnalogGauge::MinAngleRange = 5; //
const double TAZAnalogGauge::MinValueRange = 1E-50; // [phys units]

#pragma warn -8118 // inline waring °¨Ãß±â
//---------------------------------------------------------------------------

static inline void ValidCtrCheck(TAZAnalogGauge *)
{
	new TAZAnalogGauge(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAZAnalogGauge::TAZAnalogGauge(TComponent* AOwner)
	: Initializing(true), TGraphicControl(AOwner)
{ 
// Constructor

// . . . Inherited props defaults

    Constraints->MaxHeight  = 0;
    Constraints->MaxWidth   = 0;
    Constraints->MinHeight  = 40;
    Constraints->MinWidth   = 40;
    
    //ControlStyle        = TControlStyle() << csReplicatable >> csCaptureMouse << csClickEvents << csDoubleClicks >> csNoStdEvents >> csDisplayDragImage >> csOpaque;
	//Parent              = dynamic_cast<Controls::TWinControl*>(Owner);
    ParentShowHint      = false;
    ParentColor         = true;

    Width               = 225;
    Height              = 180;
    
    //Font->Assign(Parent->Font);

    // . . . Properties defaults
    // Main
    i_Caption           = "[u]";
    i_ValidValue        = true;
    i_Value             = 0.0;
    i_MaxValue          = i_MinValue = i_Value;
    i_FullScaleMax      = 100.0;
    i_FullScaleMin      = 0.0;
    i_HighBand          = 0.8 * i_FullScaleMax;
    i_LowBand           = 0.3 * i_FullScaleMax;
    i_CurrentBand       = bnLow;
    i_clickedangle      = 0;
    // Style (Geometry)
    i_Da                = 2*M_PI/3; // 120?
    i_amin              = M_PI_2 - i_Da/2;
    i_aexceed           = i_Da/20;
    i_PointerWidth      = 1;
    i_PointerLines      = 1;
    i_Padding           = Constraints->MinHeight / 4;
    i_MarginTop         = i_MarginBottom = i_MarginLeft = i_MarginRight = i_Padding;
    i_HubRadius         = 4;
    i_Precision         = 4;
    i_NumMainTicks      = 10;
    i_NumSubTicks       = 5;
    i_NumTicks          = i_NumMainTicks * i_NumSubTicks;
    i_LengthMainTicks   = 15;
    i_LengthSubTicks    = 8;
    i_BandThickness     = 4;

    // Settings
    i_Options = TOptions() << opShowMainTicks
               << opShowSubTicks
               << opShowTickNumbers
               << opShowMinBand
               << opShowMidBand
               << opShowHighBand
               << opShowMinMax
               << opShowHub
               << opShowCaption;
    
    i_AntiAliasing      = aaNone;
    i_EnableDynamic     = false;
    // Style (Appearance)
    FontOnChange_Org = Font->OnChange;

    Font->OnChange =  MainFontChanged;

    i_TicksFont         = new TFont();
    i_TicksFont->OnChange = FontChanged;
    //i_TicksFont->Name = "Tahoma"; // SmallFonts
    //i_TicksFont->Size = 8;

    i_BackgroundBmp     = new Graphics::TBitmap();
    i_BevelOuter        = bvNone; // bvRaised
    i_BevelInner        = bvLowered;
    i_BodyColor         = Color; // clBtnFace
    i_DisplayColor      = cl3DLight;
    i_BorderColor       = clLtGray;
    i_TicksColor        = clBlack;
    i_PointerColor      = clBlack;
    i_HubColor          = clLtGray;
    i_LowBandColor      = TColor(0x13AF13); //clGreen
    i_MidBandColor      = TColor(0x7F7F7); //clYellow
    i_HighBandColor     = TColor(0xF0FEF); //clRed
    // Events
    i_OnChangingBand    = 0;
    i_OnExceeding       = 0;
    
    // Performance tracking
#ifdef AN4_FPS_MEASURE
    i_fps             = 0;
    i_OnFPSMeasure    = 0;
    i_PrevCount.QuadPart = -1;
    if (!::QueryPerformanceFrequency(&i_Freq) ) i_Freq.QuadPart = 0;
#endif

    // . . . Contained stuff
    // State
    p.a = p.aref = i_amin;
    // Background bitmap
    i_BodyBitmap                = new Graphics::TBitmap();
    i_BodyBitmap->Width         = Width;
    i_BodyBitmap->Height        = Height;
    i_BodyBitmap->Canvas->Brush->Style = bsClear;
    i_BodyBitmap->Canvas->Brush->Color = Color;
    i_BodyBitmap->Canvas->Font->Assign(Font);
    i_BodyBitmap->Transparent   = true;
    i_BodyBitmap->TransparentColor = i_BodyColor;

    // Face bitmap
    i_FaceBitmap                = new Graphics::TBitmap();
    i_FaceBitmap->Width         = i_BodyBitmap->Width;
    i_FaceBitmap->Height        = i_BodyBitmap->Height;
    i_FaceBitmap->Canvas->Font->Assign(i_BodyBitmap->Canvas->Font);
    i_FaceBitmap->Transparent   = i_BodyBitmap->Transparent;
    i_FaceBitmap->TransparentColor = i_BodyBitmap->TransparentColor;
    // Antialiasing bitmap
    i_AABitmap          = 0;
    // Drawing bags
    RPp                 = TRect(0,0,0,0);
    RTp                 = TRect(0,0,0,0);

    
    // Pointer mechanical transient timer
#ifdef AN4_USE_MMTIMER
    i_Timer           = new cls_MMTimer();
#else
    i_Timer           = new TTimer(GetParentForm(this));
#endif
    i_Timer->Enabled  = false;
    i_Timer->Interval = round(p.get_Ts()*1000.0);
    i_Timer->OnTimer  = DynamicModel;
    // Other
    i_Pc_manual         = TPoint(0,0);
    i_L_manual          = 0;

    // . . . Other operations
    UpdateDimensions(); // Fills 'p','g'
    //Redraw();

    TGraphicControl::OnResize       = MyResize;
    TGraphicControl::OnMouseDown    = MyMouseDown;

    nWidth_Old          = Width;
    nHeight_Old         = Height;
//    
    // . . . Finally
    Initializing        = false;
    
}
//---------------------------------------------------------------------------
__fastcall TAZAnalogGauge::~TAZAnalogGauge()
{ // Destructor

    delete i_TicksFont;
    delete i_Timer; // (Not strictly necessary)
    delete i_BackgroundBmp;
    delete i_BodyBitmap;
    delete i_FaceBitmap;
    if (i_AABitmap) delete i_AABitmap;
}
//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::Loaded(void) 
{
    TGraphicControl::Loaded();

    _Resize();

    Invalidate();
}


//---------------------------------------------------------------------------
// Antialiasing: resize hires bitmap to control size
// Optimized version: antialias factor fixed to 3
// See bottom of file for original general version
// inlined because called once, is executed at every draw, so has to be fast
inline void __fastcall TAZAnalogGauge::ResizeBitmap(Graphics::TBitmap* Bsrc, Graphics::TBitmap* Bdst)
{
    ASSERTING((Bsrc->PixelFormat==pf24bit && Bdst->PixelFormat==pf24bit),"Must use 24bit bitmaps")
    ASSERTING(((Bsrc->Width/Bdst->Width)==3),"Uncoherent bmp width dimensions")
    ASSERTING(((Bsrc->Height/Bdst->Height)==3),"Uncoherent bmp height dimensions")

// . . . Used variables
    int src_y, src_x_0, src_x_1, src_x_2;
    TRGBTriple *r1,*r2,*r3,*drow; // PRGBTriple arrays

// . . . For each row
  for ( int y=0, yend=Bdst->Height; y<yend; ++y )
      {
       // We compute samples of i_AntiAliasing x i_AntiAliasing pixels
       src_y = 3 * y;

       // Get supersampled bitmap pixels
       r1 = static_cast<TRGBTriple*>(Bsrc->ScanLine[src_y]);
       r2 = static_cast<TRGBTriple*>(Bsrc->ScanLine[src_y+1]);
       r3 = static_cast<TRGBTriple*>(Bsrc->ScanLine[src_y+2]);

       // Get_ a pointer to destination row in output bitmap
       drow = static_cast<TRGBTriple*>(Bdst->ScanLine[y]);

       // . . . For each column
       for ( int x=0, xend=Bdst->Width; x<xend; ++x )
           {
            // We compute samples of 3x3 pixels
            src_x_0 = 3 * x;
            src_x_1 = src_x_0 + 1;
            src_x_2 = src_x_0 + 2;

            drow[x].rgbtRed = (r1[src_x_0].rgbtRed + r2[src_x_0].rgbtRed + r3[src_x_0].rgbtRed +
                               r1[src_x_1].rgbtRed + r2[src_x_1].rgbtRed + r3[src_x_1].rgbtRed +
                               r1[src_x_2].rgbtRed + r2[src_x_2].rgbtRed + r3[src_x_2].rgbtRed  ) / 9;
            drow[x].rgbtGreen = (r1[src_x_0].rgbtGreen + r2[src_x_0].rgbtGreen + r3[src_x_0].rgbtGreen +
                                 r1[src_x_1].rgbtGreen + r2[src_x_1].rgbtGreen + r3[src_x_1].rgbtGreen +
                                 r1[src_x_2].rgbtGreen + r2[src_x_2].rgbtGreen + r3[src_x_2].rgbtGreen  ) / 9;
            drow[x].rgbtBlue = (r1[src_x_0].rgbtBlue + r2[src_x_0].rgbtBlue + r3[src_x_0].rgbtBlue +
                                r1[src_x_1].rgbtBlue + r2[src_x_1].rgbtBlue + r3[src_x_1].rgbtBlue +
                                r1[src_x_2].rgbtBlue + r2[src_x_2].rgbtBlue + r3[src_x_2].rgbtBlue  ) / 9;
           } // end 'for each column'
        } // end 'for each row'
}

//---------------------------------------------------------------------------
TAZAnalogGauge::TQuadrant TAZAnalogGauge::GetAngleQuadrant(double a) //  _,...._
{                                                           //           ,'   |   `.
    a = std::fmod(a,2*M_PI);                                //         .'  I  |  II `.
    if (a>=0 && a<M_PI_2) return qdI;                       //        _|______|______|_
    else if (a>=M_PI_2 && a<M_PI) return qdII;              //         |      |      |
    else if (a>=M_PI && a<3*M_PI_2) return qdIII;           //         \  IV  | III  /
    else return qdIV;                                       //          `.    |    ,'
}                                                           //            `--...-'


//---------------------------------------------------------------------------

static template<typename T> inline T TAZAnalogGauge::min(const T a, const T b)
{
    return (a<b) ? a : b;
}
//---------------------------------------------------------------------------
static inline bool TAZAnalogGauge::AreDifferent(const TPoint& A, const TPoint& B) 
{
    return (A.x!=B.x)||(A.y!=B.y);
} // States if two points are not equal


//============================= Public Methods ==============================






//---------------------------------------------------------------------------
// Assignment operator
TAZAnalogGauge* __fastcall TAZAnalogGauge::AssignGauge(const TAZAnalogGauge* a)
{
// . . . Inherited
    Width       = a->Width;
    Height      = a->Height;
    Color       = a->Color;
    Font->Assign(a->Font);
    ShowHint    = a->ShowHint;

    Tag         = a->Tag;
    Left        = a->Left;
    Top         = a->Top;
    Align       = a->Align;
    //Enabled     = a->Enabled;
    Visible     = a->Visible; 
    
// . . . Properties
    // . . . Main
    Caption = a->Caption;
    Value = a->Value;
    i_MaxValue = a->MaxValue;
    i_MinValue = a->MinValue;
    FullScaleMax = a->FullScaleMax;
    FullScaleMin = a->FullScaleMin;
    HighBand = a->HighBand;
    LowBand = a->LowBand;
    // . . . Settings
    Options = a->Options;
    // Antialiased done finally to get fonts
    EnableDynamic = a->EnableDynamic;
    kT = a->kT;
    kF = a->kF;
    // . . . Style (Geometry)
    AngleMin = a->AngleMin;
    AngleRange = a->AngleRange;
    AngleExceed = a->AngleExceed;
    Padding = a->Padding;
    MarginLeft = a->MarginLeft;
    MarginTop = a->MarginTop;
    MarginRight = a->MarginRight;
    MarginBottom = a->MarginBottom;
    PointerWidth = a->PointerWidth;
    PointerLines = a->PointerLines;
    HubRadius = a->HubRadius;
    NumMainTicks = a->NumMainTicks;
    NumSubTicks = a->NumSubTicks;
    LengthMainTicks = a->LengthMainTicks;
    LengthSubTicks = a->LengthSubTicks;
    BandThickness = a->BandThickness;
    // . . . Style (Appearance)
    TicksFont->Assign(a->TicksFont);
    BackgroundBmp->Assign(a->BackgroundBmp);
    BevelOuter = a->BevelOuter;
    BevelInner = a->BevelInner;
    BodyColor = a->BodyColor;
    BorderColor = a->BorderColor;
    DisplayColor = a->DisplayColor;
    TicksColor = a->TicksColor;
    PointerColor = a->PointerColor;
    HubColor = a->HubColor;
    LowBandColor = a->LowBandColor;
    MidBandColor = a->MidBandColor;
    HighBandColor = a->HighBandColor;
    // . . . Events
    OnChangingBand = a->OnChangingBand;
    OnExceeding = a->OnExceeding;

// . . . Finally
    AntiAliased = a->AntiAliased;

    Redraw();
    return this;
}


//---------------------------------------------------------------------------
// Some reset operations
void __fastcall TAZAnalogGauge::Reset()
{
// . . . Reset Min and Max values
    if ( std::_isnan(i_Value) ) i_MaxValue = i_MinValue = i_Value = 0;
    else i_MaxValue = i_MinValue = i_Value;
}
//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::Tap(const double v)   // Knock meter (uses dynamic model)
{
    i_Timer->Enabled=true; p.set_w(v?v:kF);
}


//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::SetCenteredLayout(const int ARange)
{
    AngleRange = ARange;
    i_amin = M_PI_2 - i_Da/2;
    i_aexceed = i_Da/20;
    UpdateDimensions();
    Redraw();
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::SetRightLayout()
{
    i_amin = 0;
    if ( i_Da > M_PI_2 ) i_Da = M_PI_2;
    i_aexceed = i_Da/20;
    UpdateDimensions();
    Redraw();
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::SetLeftLayout()
{
    i_amin = M_PI_2;
    if ( i_Da > M_PI_2 ) i_Da = M_PI_2;
    i_aexceed = i_Da/20;
    UpdateDimensions();
    Redraw();
}

//---------------------------------------------------------------------------
// Used to tune pointer position when using external background bitmap
// Arguments are: pointer pivot, pointer length, hub radius,
//                Min position, Max position, Exceeding position
void TAZAnalogGauge::ManualTune(const TPoint& C, const int l, const int r,
                                 const TPoint& Pm, const TPoint& PM, const TPoint& E)
{
// . . . Set pointer dimensions
    i_Pc_manual = C;
    g.Pc = TPoint(i_Pc_manual.x * i_AntiAliasing, i_Pc_manual.y * i_AntiAliasing); // Essential for 'CartesianToPolar' transformation
    i_L_manual = l;
    i_HubRadius = r;

// . . . Angles
    CartesianToPolar(Pm.x*i_AntiAliasing, Pm.y*i_AntiAliasing, &i_amin);
    CartesianToPolar(PM.x*i_AntiAliasing, PM.y*i_AntiAliasing, &i_Da);
    CartesianToPolar(E.x*i_AntiAliasing, E.y*i_AntiAliasing, &i_aexceed);
    // Fix the angle increments
    i_aexceed -= i_Da; while(i_aexceed<0) i_aexceed += 2*M_PI;
    i_Da -= i_amin; while(i_Da<0) i_Da += 2*M_PI;

// . . . Finally
    UpdateDimensions();
    Redraw();
}



//============================ Protected Methods ============================


//---------------------------------------------------------------------------
// Render instrument on a Canvas
// Hasn't to be so fast
void __fastcall TAZAnalogGauge::DrawBody(TCanvas* cvs)
{
    if(Initializing) return;

// . . . Overall Frame (Body)
    cvs->Brush->Color = i_BodyColor;
    // TODO 5: What happens with: cvs->Brush->Style = bsClear;
    cvs->FillRect(cvs->ClipRect);

// . . . Inner Frame (display)
    cvs->Brush->Color = i_DisplayColor;
    cvs->FillRect(g.Rd);

    // . . . Borders
    switch ( i_BevelOuter )
           {
            case bvLowered :
                cvs->Pen->Width = 2*i_AntiAliasing;
                cvs->Pen->Color = clBtnShadow; //cl3DDkShadow
                cvs->MoveTo(g.W, 0);
                cvs->LineTo(0, 0);
                cvs->LineTo(0, g.H);
                cvs->Pen->Color = clBtnHighlight; //cl3DLight
                cvs->LineTo(g.W, g.H);
                cvs->LineTo(g.W, 0);
                break;

            case bvRaised :
                cvs->Pen->Width = 2*i_AntiAliasing;
                cvs->Pen->Color = clBtnHighlight; //cl3DLight
                cvs->MoveTo(g.W, 0);
                cvs->LineTo(0, 0);
                cvs->LineTo(0, g.H);
                cvs->Pen->Color = clBtnShadow; //cl3DDkShadow
                cvs->LineTo(g.W, g.H);
                cvs->LineTo(g.W, 0);
                break;

            case bvSpace :
                cvs->Pen->Width = i_AntiAliasing;
                cvs->Pen->Color = i_BorderColor;
                cvs->FrameRect(TRect(0, 0, g.W, g.H));
                break;
            //bvNone
           }

    switch ( i_BevelInner )
           {
            case bvLowered :
                cvs->Pen->Width = i_AntiAliasing;
                cvs->Pen->Color = clBtnShadow; //cl3DDkShadow
                cvs->MoveTo(g.Rd.Right, g.Rd.Top);
                cvs->LineTo(g.Rd.Left, g.Rd.Top);
                cvs->LineTo(g.Rd.Left, g.Rd.Bottom);
                cvs->Pen->Color = clBtnHighlight; //cl3DLight
                cvs->LineTo(g.Rd.Right, g.Rd.Bottom);
                cvs->LineTo(g.Rd.Right, g.Rd.Top);
                break;

            case bvRaised :
                cvs->Pen->Width = i_AntiAliasing;
                cvs->Pen->Color = clBtnHighlight; //cl3DLight
                cvs->MoveTo(g.Rd.Right, g.Rd.Top);
                cvs->LineTo(g.Rd.Left, g.Rd.Top);
                cvs->LineTo(g.Rd.Left, g.Rd.Bottom);
                cvs->Pen->Color = clBtnShadow; //cl3DDkShadow
                cvs->LineTo(g.Rd.Right, g.Rd.Bottom);
                cvs->LineTo(g.Rd.Right, g.Rd.Top);
                break;

            case bvSpace :
                cvs->Pen->Width = i_AntiAliasing;
                cvs->Pen->Color = i_BorderColor;
                cvs->FrameRect(g.Rd);
                break;
            //bvNone
           }

// . . . Caption
    if ( i_Options.Contains(opShowCaption) )
       {
        cvs->Font->Assign(Font);
        if (AntiAliased) cvs->Font->Height = cvs->Font->Height * i_AntiAliasing;
        cvs->TextOut(g.Pm.x - cvs->TextWidth(i_Caption)/2, g.Pm.y, i_Caption);
       }

// . . . High-Mid-Low bands
    cvs->Pen->Width = g.B;
    if ( i_Options.Contains(opShowHighBand) && AreDifferent(g.Pp,g.Phb) )
       {
        cvs->Pen->Color = i_HighBandColor;
        cvs->Arc(g.Rr.Left, g.Rr.Top, g.Rr.Right, g.Rr.Bottom, g.Pp.x, g.Pp.y, g.Phb.x, g.Phb.y);
       }

    if ( i_Options.Contains(opShowMidBand) && AreDifferent(g.Phb,g.Plb) )
       {
        cvs->Pen->Color = i_MidBandColor;
        cvs->Arc(g.Rr.Left, g.Rr.Top, g.Rr.Right, g.Rr.Bottom, g.Phb.x, g.Phb.y, g.Plb.x, g.Plb.y);
       }

    if ( i_Options.Contains(opShowMinBand) && AreDifferent(g.Plb,g.Pn) )
       {
        //::SetArcDirection(cvs->Handle,AD_CLOCKWISE);
        cvs->Pen->Color = i_LowBandColor;
        cvs->Arc(g.Rr.Left, g.Rr.Top, g.Rr.Right, g.Rr.Bottom, g.Plb.x, g.Plb.y, g.Pn.x, g.Pn.y);
       }

// . . . Prepare tools for ticks
    cvs->Pen->Color = i_TicksColor;
    cvs->Pen->Width = i_AntiAliasing;
    cvs->Font->Assign(i_TicksFont);
    if (AntiAliased) cvs->Font->Height = cvs->Font->Height * i_AntiAliasing;

// . . . SubTicks
    double at; // current tick angle
    if ( i_Options.Contains(opShowSubTicks) )
       {
        for ( int i=0; i<=i_NumTicks; ++i )
            {
             at = i_amin + i*i_Da/i_NumTicks; // Angle of current tick
             PolarToCartesian(g.l-g.St, g.l, at, &RTp);
             cvs->MoveTo(RTp.Right, RTp.Bottom);
             cvs->LineTo(RTp.Left, RTp.Top);
            } // end 'for all ticks'
       }

// . . . Main ticks
    for ( int i=0; i<=i_NumMainTicks; ++i )
        {
         at = i_amin + double(i)*i_Da/double(i_NumMainTicks); // Angle of current tick

         // . . . Main Ticks
         if ( i_Options.Contains(opShowMainTicks) )
            {
             PolarToCartesian(g.l-g.Mt, g.l, at, &RTp);
             cvs->MoveTo(RTp.Right, RTp.Bottom);
             cvs->LineTo(RTp.Left, RTp.Top);
            }

         /* . . . Circles on main ticks
         if ( i_Options.Contains(opShowCircles) )
            {
             cvs->Brush->Color = i_CircleColor;
             PolarToCartesian(g.l, a, &P);
             cvs->Ellipse(P.x-g.r, P.y-g.r, P.x+g.r, P.y+g.r);
            } */

         // . . . Tick label
         if ( i_Options.Contains(opShowTickNumbers) )
            {
             TPoint P; // Cartesian point
             PolarToCartesian(g.l-g.Mt-cvs->Font->Size-3, at, &P);
             cvs->Brush->Color = i_DisplayColor;
             String ValLbl = ::FloatToStrF(p.ToValue(at),ffGeneral,i_Precision,2);
             cvs->TextOut(P.x-cvs->TextWidth(ValLbl)/2, P.y-cvs->TextHeight(ValLbl)/2, ValLbl);
            }
        } // end 'for all main ticks'

// . . . Pointer hub
    if ( i_Options.Contains(opShowHub) )
       {
        cvs->Brush->Color = i_HubColor;
        cvs->Ellipse(g.Pc.x-g.r, g.Pc.y-g.r, g.Pc.x+g.r, g.Pc.y + g.r);
       }

// . . . Debug
    /* See points
    #define SEEDOT(q) cvs->Ellipse(q.x-3,q.y-3,q.x+3,q.y+3);
    cvs->Pen->Width = 2;
    cvs->Pen->Color = clBlue;
    SEEDOT(g.Phb)
    SEEDOT(g.Plb)
    cvs->Pen->Color = clAqua;
    SEEDOT(g.Pp)
    SEEDOT(g.Pn) */
}

//---------------------------------------------------------------------------
// Render measuring pointer on a Canvas
// Executed at every draw, so has to be optimized; uses structure 'p'
void __fastcall TAZAnalogGauge::DrawPointer(TCanvas* cvs)
{
// . . . Pointer
    cvs->Pen->Width = i_PointerWidth * i_AntiAliasing;
    cvs->Pen->Color = i_PointerColor;

    /* This could be an alternative to i_BodyBitmap/i_FaceBitmap mechanism:
    cvs->Pen->Mode = pmNotXor;
    cvs->MoveTo(RPp.Right, RPp.Bottom);
    cvs->LineTo(RPp.Left, RPp.Top);  */

    if ( i_ValidValue )
       {
        // . . . Pointer
        // TODO 4: Pointer shape (triangular)
        PolarToCartesian(g.l, g.r, p.a, &RPp);

        if(i_PointerLines <= 1) {
            cvs->MoveTo(RPp.Right, RPp.Bottom);
            cvs->LineTo(RPp.Left, RPp.Top);
        }
        else {
            for(int i=0; i<i_PointerLines; i++) {
                cvs->MoveTo(RPp.Right - (i_PointerLines/2) + i, RPp.Bottom);
                cvs->LineTo(RPp.Left, RPp.Top);
            }
        }
        
        // . . . Min and Max values
        if ( i_Options.Contains(opShowMinMax) )
           {
            // TODO 4: min-max shape and color
            int D = 2*i_AntiAliasing;

            if ( i_MaxValue <= i_FullScaleMax && i_MaxValue>=i_FullScaleMin ) // Skip if outside
               {
                PolarToCartesian(g.l+g.St, g.l, p.ToAngle(i_MaxValue), &RTp);
                // A circle
                //cvs->Ellipse(RTp.Left-D, RTp.Top-D, RTp.Left+D, RTp.Top+D);
                cvs->Ellipse(RTp.Right-D, RTp.Bottom-D, RTp.Right+D, RTp.Bottom+D);
                // A line
                //cvs->MoveTo(RTp.Right, RTp.Bottom);
                //cvs->LineTo(RTp.Left, RTp.Top);
               } // end 'max value visible'

            if ( i_MinValue >= i_FullScaleMin && i_MinValue <= i_FullScaleMax ) // Skip if outside
               {
                PolarToCartesian(g.l+g.St, g.l, p.ToAngle(i_MinValue), &RTp);
                // A circle
                //cvs->Ellipse(RTp.Left-D, RTp.Top-D, RTp.Left+D, RTp.Top+D);
                cvs->Ellipse(RTp.Right-D, RTp.Bottom-D, RTp.Right+D, RTp.Bottom+D);
                // A line
                //cvs->MoveTo(RTp.Right, RTp.Bottom);
                //cvs->LineTo(RTp.Left, RTp.Top);
               } // end 'min value visible'
           } // end 'show min and max'
       } // end 'valid pointer value'
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::Redraw() 
{
    if(Initializing) return;
    
#ifdef AN4_FPS_MEASURE
    ::QueryPerformanceCounter(&i_PrevCount);
#endif
    if ( NoExtBackground() ) {
        DrawBody(i_BodyBitmap->Canvas);
    }
    
    RedrawPointer(); // Calls 'Paint()'
}
                             
//---------------------------------------------------------------------------
// This do the work: prepare control bitmap buffer
void __fastcall TAZAnalogGauge::RedrawPointer()
{
    if(Initializing) return;

#ifdef AN4_FPS_MEASURE
    if (i_PrevCount.QuadPart<0) ::QueryPerformanceCounter(&i_PrevCount);
#endif

// . . . Transfer BodyBitmap to FaceBitmap
#ifdef AN4_USE_BITBLT
    ::BitBlt(i_FaceBitmap->Canvas->Handle, 0, 0,
             i_BodyBitmap->Width, i_BodyBitmap->Height, i_BodyBitmap->Canvas->Handle,
             0, 0, SRCCOPY);
#else
    i_FaceBitmap->Canvas->Draw(0, 0, i_BodyBitmap);
#endif

// . . . Draw pointer
    DrawPointer(i_FaceBitmap->Canvas);

// . . . Resize FaceBitmap to AABitmap if using antialiasing
    if ( AntiAliased ) ResizeBitmap(i_FaceBitmap,i_AABitmap);

// . . . Finally
    Paint();

#ifdef AN4_FPS_MEASURE
    LARGE_INTEGER CurrCount;
    ::QueryPerformanceCounter(&CurrCount);
    __int64 DeltaCount = CurrCount.QuadPart - i_PrevCount.QuadPart;
    i_PrevCount.QuadPart = -1;
    int new_fps = (DeltaCount != 0) ? i_Freq.QuadPart/DeltaCount : MaxInt;
    if ( new_fps != i_fps )
       {
        i_fps = new_fps;
        if (i_OnFPSMeasure) i_OnFPSMeasure(this);
       }
#endif
}

// External background, self tune of pointer length and pivot
// External background and aliasing, ehmm...

//---------------------------------------------------------------------------
// Recalculate geometric quantities rescaled for antialiasing
// Executed only at geometric pars change, so has't to be optimized
void __fastcall TAZAnalogGauge::UpdateDimensions()
{
// . . . Pointer position transform (structure p)
    if ( std::fabs(ValueRange) < MinValueRange ) i_FullScaleMax = i_FullScaleMin + MinValueRange;
    p.set_m( i_Da / ValueRange );
    p.set_q( (i_FullScaleMax*i_amin - i_FullScaleMin*(i_amin+i_Da)) / ValueRange ); // i_amin - (p.m * i_FullScaleMin)
    // Update pointer position in new space
    p.a = p.ToAngle(i_Value);
    limit_value(&(p.a),i_amin-i_aexceed,i_amin+i_Da+i_aexceed);

// . . . Geometry (structure g, some geometric quantities used for internal drawing

    // . . . Frame size, scaled
    g.W = Width * i_AntiAliasing;
    g.H = Height * i_AntiAliasing;

    // . . . Scaled margin
    g.M = TRect(i_MarginLeft * i_AntiAliasing,
                i_MarginTop * i_AntiAliasing,
                i_MarginRight * i_AntiAliasing,
                i_MarginBottom * i_AntiAliasing);
    g.D = i_Padding * i_AntiAliasing;

    // . . . Inner Frame
    g.Rd = TRect(g.M.Left, g.M.Top, g.W-g.M.Right, g.H-g.M.Bottom);
    g.Ri = TRect(g.Rd.Left + g.D, g.Rd.Top + g.D,
                 g.Rd.Right - g.D, g.Rd.Bottom - g.D);

    // . . . Some other scaled quantities
    g.r = i_HubRadius * i_AntiAliasing; // Hub radius
    g.B = i_BandThickness * i_AntiAliasing; // Band thickness
    g.St = i_LengthSubTicks * i_AntiAliasing; // Subthicks length
    g.Mt = i_LengthMainTicks * i_AntiAliasing; // Main ticks length

   //. . . Pointer Radius and Hub
   if ( ExtBackground() )
        { // An external background is defined
         g.Pc.x = i_Pc_manual.x * i_AntiAliasing;
         g.Pc.y = i_Pc_manual.y * i_AntiAliasing;
         g.l = i_L_manual * i_AntiAliasing;
        }
   else {// Background must be internally drawn
        // Determine layout
        double bag1, bag2, bag3;
        double amin_r = std::fmod(i_amin,M_PI_2); // starting angle relative to quadrant
        double amax_r = amin_r + i_Da; // ending angle relative to quadrant
        if ( amax_r <= M_PI_2 )
             { // Single quadrant
              // Compute projection of angles: amax_r,amin_r change their meaning
              amin_r = std::cos(amin_r);
              amax_r = std::cos(M_PI_2-amax_r);
              switch ( GetAngleQuadrant(i_amin) )
                     {
                      case qdI   :
                          // Pointer Radius is the minimum between following sizes:
                          bag1 = (amin_r>EPSILON) ? (g.Ri.Width() / amin_r) : g.Ri.Height();
                          bag2 = (amax_r>EPSILON) ? (g.Ri.Height() / amax_r) : g.Ri.Width();
                          g.l = min(bag1,bag2);
                          // Hub point is on inner frame right bottom
                          g.Pc = TPoint(g.Ri.Right, g.Ri.Bottom);
                          break;

                      case qdII  :
                          // Pointer Radius is the minimum between following sizes:
                          bag1 = (amin_r>EPSILON) ? (g.Ri.Height() / amin_r) : g.Ri.Width();
                          bag2 = (amax_r>EPSILON) ? (g.Ri.Width() / amax_r) : g.Ri.Height();
                          g.l = min(bag1,bag2);
                          // Hub point is on inner frame left bottom
                          g.Pc = TPoint(g.Ri.Left, g.Ri.Bottom);
                          break;

                      case qdIII :
                          // Pointer Radius is the minimum between following sizes:
                          bag1 = (amin_r>EPSILON) ? (g.Ri.Width() / amin_r) : g.Ri.Height();
                          bag2 = (amax_r>EPSILON) ? (g.Ri.Height() / amax_r) : g.Ri.Width();
                          g.l = min(bag1,bag2);
                          // Hub point is on inner frame left top
                          g.Pc = TPoint(g.Ri.Left, g.Ri.Top);
                          break;

                      default :
                          // Pointer Radius is the minimum between following sizes:
                          bag1 = (amin_r>EPSILON) ? (g.Ri.Height() / amin_r) : g.Ri.Width();
                          bag2 = (amax_r>EPSILON) ? (g.Ri.Width() / amax_r) : g.Ri.Height();
                          g.l = min(bag1,bag2);
                          // Hub point is on inner frame right top
                          g.Pc = TPoint(g.Ri.Right, g.Ri.Top);
                     } // end 'switch quadrant'
             } // end 'single quadrant'
        else if ( amax_r <= M_PI )
             { // Two adiacent quadrants
              // Compute projection of angles: amax_r,amin_r change their meaning
              amin_r = 2*std::cos(amin_r);
              amax_r = 2*std::cos(M_PI-amax_r);
              switch ( GetAngleQuadrant(i_amin) )
                     {
                      case qdI   : // (I-II)
                          // Pointer Radius is the minimum between following sizes:
                          bag1 = g.Ri.Height();
                          bag2 = (amin_r>EPSILON) ? (g.Ri.Width() / amin_r) : bag1;
                          bag3 = (amax_r>EPSILON) ? (g.Ri.Width() / amax_r) : bag1;
                          g.l = min(bag2,bag3);
                          g.l = min(g.l,bag1);
                          // Hub point is horizontally centered
                          g.Pc = TPoint(g.W/2, (g.H + round(g.l))/2); // g.Ri.Bottom;
                          break;

                      case qdII  : // (II-III)
                          // Pointer Radius is the minimum between following sizes:
                          bag1 = g.Ri.Width();
                          bag2 = (amin_r>EPSILON) ? (g.Ri.Height() / amin_r) : bag1;
                          bag3 = (amax_r>EPSILON) ? (g.Ri.Height() / amax_r) : bag1;
                          g.l = min(bag2,bag3);
                          g.l = min(g.l,bag1);
                          // Hub point is vertically centered
                          g.Pc = TPoint((g.W - round(g.l))/2, g.H/2);
                          break;

                      case qdIII : // (III-IV)
                          // Pointer Radius is the minimum between following sizes:
                          bag1 = g.Ri.Height();
                          bag2 = (amin_r>EPSILON) ? (g.Ri.Width() / amin_r) : bag1;
                          bag3 = (amax_r>EPSILON) ? (g.Ri.Width() / amax_r) : bag1;
                          g.l = min(bag2,bag3);
                          g.l = min(g.l,bag1);
                          // Hub point is horizontally centered
                          g.Pc = TPoint(g.W/2, (g.H - round(g.l))/2);
                          break;

                      default :   // (IV-I)
                          // Pointer Radius is the minimum between following sizes:
                          bag1 = g.Ri.Width();
                          bag2 = (amin_r>EPSILON) ? (g.Ri.Height() / amin_r) : bag1;
                          bag3 = (amax_r>EPSILON) ? (g.Ri.Height() / amax_r) : bag1;
                          g.l = min(bag2,bag3);
                          g.l = min(g.l,bag1);
                          // Hub point is vertically centered
                          g.Pc = TPoint((g.W + round(g.l))/2, g.H/2);
                          break;
                     } // end 'switch quadrant'
             } // end 'two adiacent quadrants'
        else { // All quadrants: circle layout
              // Pointer Radius is the minimum of inner frame half dimension
              g.l = min(g.Ri.Width()/2,g.Ri.Height()/2);
              // Hub point is the center
              g.Pc = TPoint(g.W/2, g.H/2);
             } // end 'all quadrants'
        } // end 'ExtBackground'

    // . . . Rotation Frame
    g.Rr = TRect(g.Pc.x-g.l, g.Pc.y-g.l, g.Pc.x+g.l, g.Pc.y+g.l);

    // . . . Median point (centering caption)
    PolarToCartesian(g.l/2, i_amin + i_Da/2, &(g.Pm));

    // . . . Fullscale points
    PolarToCartesian(g.l, i_amin + i_Da, &(g.Pp));
    PolarToCartesian(g.l, i_amin, &(g.Pn));

    // . . . Bands points
    PolarToCartesian(g.l, p.ToAngle(i_HighBand), &(g.Phb));
    PolarToCartesian(g.l, p.ToAngle(i_LowBand), &(g.Plb));
}




//============================= Private Methods =============================


//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_Caption(String S)
{
    //if ( S != i_Caption ) { Canvas->Font = Font; i_Caption = S; Redraw(); }
    if ( S != i_Caption ) { i_Caption = S; Redraw(); }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_TickFont(TFont * p)
{
    i_TicksFont->Assign(p);

    Redraw();
}
//---------------------------------------------------------------------------

void     __fastcall TAZAnalogGauge::MainFontChanged(System::TObject* Sender)
{
    if(FontOnChange_Org) FontOnChange_Org(Sender);
    Redraw();
}

//---------------------------------------------------------------------------

void     __fastcall TAZAnalogGauge::FontChanged(System::TObject* Sender)
{
    Redraw();
}


//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_Value(double v)
{
    if ( v != i_Value )
       {
        i_ValidValue = true;
        i_Value = v;

        // The corresponding pointer angle is:
        p.aref = p.ToAngle(i_Value);

        // . . . Handle dynamic
        if ( i_EnableDynamic )
             {
              // Start mechanical transient!
              i_Timer->Enabled = true;
             }
        else {
              // No dynamic: pointer angle is simply the target
              p.a = p.aref;
              // Ehmm, limited, of course!
              limit_value(&(p.a),i_amin-i_aexceed,i_amin+i_Da+i_aexceed);
             }

        // . . . Update Min and Max values
        if ( i_Value > i_MaxValue ) i_MaxValue = i_Value;
        else if ( i_Value < i_MinValue ) i_MinValue = i_Value;

        // . . . Detect band
        if ( i_Value > i_HighBand )
             {
              if ( i_CurrentBand != bnHigh )
                 {
                  i_CurrentBand = bnHigh;
                  if (i_OnChangingBand) i_OnChangingBand(this);
                 }
             }
        else if ( i_Value < i_LowBand )
             {
              if ( i_CurrentBand != bnLow )
                 {
                  i_CurrentBand = bnLow;
                  if (i_OnChangingBand) i_OnChangingBand(this);
                 }
             }
        else {
              if ( i_CurrentBand != bnMid )
                 {
                  i_CurrentBand = bnMid;
                  if (i_OnChangingBand) i_OnChangingBand(this);
                 }
             }

        // . . . Detect exceeding
        if ( i_Value>i_FullScaleMax || i_Value<i_FullScaleMin )
             {
              if (i_OnExceeding) i_OnExceeding(this);
             }

        Hint = String(i_Value);
        RedrawPointer();
       }
    else if ( std::_isnan(v) )
       {
        i_ValidValue = false;
        Hint = "NaN";
        RedrawPointer();
       }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_FullScaleMax(double v)
{
    if ( v!=i_FullScaleMax && v>=(i_FullScaleMin+MinValueRange) )
       {
        i_FullScaleMax = v;
        if ( i_HighBand >= i_FullScaleMax ) i_HighBand = i_FullScaleMax;
        if ( i_LowBand > i_HighBand ) i_LowBand = i_HighBand;
        UpdateDimensions();
        Redraw();
       }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_FullScaleMin(double v)
{
    if ( v!=i_FullScaleMin && v<=(i_FullScaleMax-MinValueRange) )
       {
        i_FullScaleMin = v;
        if ( i_LowBand <= i_FullScaleMin ) i_LowBand = i_FullScaleMin;
        if ( i_HighBand < i_LowBand ) i_HighBand = i_LowBand;
        UpdateDimensions();
        Redraw();
       }
}
//---------------------------------------------------------------------------
double __fastcall TAZAnalogGauge::get_ValueRange()
{
    return i_FullScaleMax-i_FullScaleMin;
}


//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_HighBand(double v)
{
    if ( v!=i_HighBand ) //&& (v>i_FullScaleMin && v<i_FullScaleMax) )
       {
        i_HighBand = v;
        UpdateDimensions();
        Redraw();
       }
}
//---------------------------------------------------------------------------

double __fastcall TAZAnalogGauge::get_ClickedValue() 
{
    return p.ToValue(i_clickedangle);
}


int __fastcall TAZAnalogGauge::get_ClickedAngle() const 
{
    return round(i_clickedangle * (180.0/M_PI));
}


//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_LowBand(double v)
{
  if ( v!=i_LowBand ) //&& (v>i_FullScaleMin && v<i_FullScaleMax) )
     {
      i_LowBand = v;
      UpdateDimensions();
      Redraw();
     }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_BevelOuter(Controls::TBevelCut B)
{
    if ( B != i_BevelOuter )
       {
        i_BevelOuter = B;
        Redraw();
       }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_BevelInner(Controls::TBevelCut B)
{
    if ( B != i_BevelInner )
       {
        i_BevelInner = B;
        Redraw();
       }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_Options(TOptions O)
{
    if ( O != i_Options )
       {
        i_Options = O;
        Redraw();
       }
}
//---------------------------------------------------------------------------
bool __fastcall TAZAnalogGauge::get_AntiAliased() const
{
    return i_AntiAliasing!=aaNone; 
}


//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_AntiAliased(bool b)
{
    if ( b != AntiAliased )
       {
        i_AntiAliasing = b ? aaTrilinear : aaNone;
        UpdateDimensions();

        // . . . Prepare bitmaps
        // Recreating them because returning to aaNone fps decays
        if ( i_AntiAliasing == aaNone )
             {
              delete i_BodyBitmap;
              i_BodyBitmap = new Graphics::TBitmap();
              delete i_FaceBitmap;
              i_FaceBitmap = new Graphics::TBitmap();
             }
        // Background bitmap
        i_BodyBitmap->PixelFormat = (i_AntiAliasing==aaNone) ? pfDevice : pf24bit;
        i_BodyBitmap->Width = g.W;
        i_BodyBitmap->Height = g.H;
        i_BodyBitmap->Canvas->Font->Assign(Font);
        i_BodyBitmap->Transparent = true;
        i_BodyBitmap->TransparentColor = i_BodyColor;
        if ( ExtBackground() )
           {
            i_BodyBitmap->Canvas->Brush->Color = Color;
            i_BodyBitmap->Canvas->FillRect(i_BodyBitmap->Canvas->ClipRect);
            i_BodyBitmap->Canvas->StretchDraw(TRect(0,0,i_BodyBitmap->Width,i_BodyBitmap->Height), i_BackgroundBmp);
           }
        // Face bitmap
        i_FaceBitmap->PixelFormat = i_BodyBitmap->PixelFormat;
        i_FaceBitmap->Width = i_BodyBitmap->Width;
        i_FaceBitmap->Height = i_BodyBitmap->Height;
        i_FaceBitmap->Canvas->Font->Assign(i_BodyBitmap->Canvas->Font);
        i_FaceBitmap->Transparent = i_BodyBitmap->Transparent;
        i_FaceBitmap->TransparentColor = i_BodyBitmap->TransparentColor;
        // Antialiased bitmap
        if ( i_AntiAliasing == aaNone )
             {
              if ( i_AABitmap ) {delete i_AABitmap; i_AABitmap = 0;}
             }
        else {
              if ( !i_AABitmap ) i_AABitmap = new Graphics::TBitmap();
              i_AABitmap->PixelFormat = i_BodyBitmap->PixelFormat;
              i_AABitmap->Width = Width;
              i_AABitmap->Height = Height;
             }

        // . . . Finally
        Redraw();
       }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_EnableDynamic(bool b)
{
    if ( b != i_EnableDynamic )
       {
        i_EnableDynamic = b;
        // . . . Stop transient
        if ( !i_EnableDynamic )
           {
            i_Timer->Enabled = false;
            p.a = p.aref;
           }
        // . . . Finally
        RedrawPointer();
       }
}
//---------------------------------------------------------------------------

double __fastcall TAZAnalogGauge::get_kT() const 
{
    return p.kT;
}
//---------------------------------------------------------------------------
double __fastcall TAZAnalogGauge::get_kF() const 
{
    return p.kF;
}
//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_kT(double v) 
{
    p.kT=v;
}
//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_kF(double v) 
{
    p.kF=v;
}
//---------------------------------------------------------------------------
int __fastcall TAZAnalogGauge::get_AngleMin() const
{
    return round(i_amin*(180.0/M_PI));
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_AngleMin(int I)
{
    if ( I!=AngleMin && (::abs(I)<=360) )
       {
        i_amin = double(I%360)*(M_PI/180.0); // std::fmod(i_amin,2*M_PI)
        UpdateDimensions();
        Redraw();
       }
}

//---------------------------------------------------------------------------
int __fastcall TAZAnalogGauge::get_AngleRange() const
{
    return round(i_Da*(180.0/M_PI));
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_AngleRange(int I)
{
    if ( I!=AngleRange && (I>MinAngleRange && I<=360) )
       {
        i_Da = double(I)*(M_PI/180.0);
        UpdateDimensions();
        Redraw();
       }
}

//---------------------------------------------------------------------------
int __fastcall TAZAnalogGauge::get_AngleExceed() const
{
    return round(i_aexceed*(180.0/M_PI));
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_AngleExceed(int I)
{
    if ( I!=AngleExceed && (::abs(I)<360) )
       {
        i_aexceed = double(I%360)*(M_PI/180.0);
        // std::fmod(i_aexceed,2*M_PI)
        UpdateDimensions();
        Redraw();
       }
}


//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_Padding(int I)
{
    if ( I!=i_Padding && Height>(i_MarginTop+i_MarginBottom+2*i_Padding) &&
                         Width>(i_MarginLeft+i_MarginRight+2*i_Padding)    )
       {
        i_Padding = I;
        UpdateDimensions();
        Redraw();
       }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_MarginLeft(int I)
{
    if ( I!=i_MarginLeft && Width>(I+i_MarginRight+2*i_Padding) )
       {
        i_MarginLeft = I;
        UpdateDimensions();
        Redraw();
       }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_MarginTop(int I)
{
    if ( I!=i_MarginTop && Height>(I+i_MarginBottom+2*i_Padding) )
       {
        i_MarginTop = I;
        UpdateDimensions();
        Redraw();
       }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_MarginRight(int I)
{
    if ( I!=i_MarginRight && Width>(i_MarginLeft+I+2*i_Padding) )
       {
        i_MarginRight = I;
        UpdateDimensions();
        Redraw();
       }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_MarginBottom(int I)
{
    if ( I!=i_MarginBottom && Height>(i_MarginTop+I+2*i_Padding) )
       {
        i_MarginBottom = I;
        UpdateDimensions();
        Redraw();
       }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_Margin(int I)
{
    if ( I!=i_Padding && Height>(2*(i_Padding+I)) && Width>(2*(i_Padding+I)) )
       {
        i_MarginTop = i_MarginBottom = i_MarginLeft = i_MarginRight = I;
        UpdateDimensions();
        Redraw();
       }
}




//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_PointerWidth(int I)
{
    if ( I != i_PointerWidth )
       {
        if ( I < 0 ) i_PointerWidth = 1;
        else i_PointerWidth = I;
        RedrawPointer();
       }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_PointerLines(int I)
{
    if ( I != i_PointerLines )
       {
        if ( I < 0 ) i_PointerLines = 1;
        else i_PointerLines = I;
        RedrawPointer();
    }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_Precision(int p) 
{
    i_Precision=p; 
    Redraw();
}


//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_HubRadius(int I)
{
    if ( I != i_HubRadius )
       {
        i_HubRadius = I;
        UpdateDimensions();
        Redraw();
       }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_NumMainTicks(int I)
{
    if ( I!=i_NumMainTicks && I!=0 )
       {
        i_NumMainTicks = I;
        i_NumTicks = i_NumMainTicks * i_NumSubTicks;
        Redraw();
       }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_NumSubTicks(int I)
{
    if ( I!=i_NumSubTicks && I!=0 )
       {
        i_NumSubTicks = I;
        i_NumTicks = i_NumMainTicks * i_NumSubTicks;
        Redraw();
       }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_LengthMainTicks(int I)
{
    if ( I != i_LengthMainTicks )
       {
        i_LengthMainTicks = I;
        UpdateDimensions();
        Redraw();
       }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_LengthSubTicks(int I)
{
    if ( I != i_LengthSubTicks )
       {
        i_LengthSubTicks = I;
        UpdateDimensions();
        Redraw();
       }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_BandThickness(int I)
{
    if ( I != i_BandThickness )
       {
        i_BandThickness = I;
        UpdateDimensions();
        Redraw();
       }
}


//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_BackgroundBmp(Graphics::TBitmap* b)
{
    if ( b && i_BackgroundBmp != b )
       {
        i_BackgroundBmp->Assign(b);
        i_BackgroundBmp->Transparent = true;
        i_BackgroundBmp->TransparentMode = tmAuto;
        Width = i_BackgroundBmp->Width;
        Height = i_BackgroundBmp->Height;
        //UpdateDimensions(); // Already called by 'Resize()'
        // . . . Prepare BodyBitmap
        i_BodyBitmap->Canvas->Brush->Color = Color;
        i_BodyBitmap->Canvas->FillRect(i_BodyBitmap->Canvas->ClipRect);
        i_BodyBitmap->Canvas->StretchDraw(TRect(0,0,i_BodyBitmap->Width,i_BodyBitmap->Height), i_BackgroundBmp);
        Redraw();
       }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_BodyColor(TColor C)
{
  if ( C!=i_BodyColor ) { i_BodyColor=C; Redraw(); }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_DisplayColor(TColor C)
{
  if ( C!=i_DisplayColor ) { i_DisplayColor=C; Redraw(); }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_TicksColor(TColor C)
{
  if ( C!=i_TicksColor ) { i_TicksColor=C; Redraw(); }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_PointerColor(TColor C)
{
  if ( C!=i_PointerColor ) { i_PointerColor=C; RedrawPointer(); }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_BorderColor(TColor C)
{
  if ( C!=i_BorderColor ) { i_BorderColor=C; Redraw(); }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_HubColor(TColor C)
{
  if ( C!=i_HubColor ) { i_HubColor=C; Redraw(); }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_LowBandColor(TColor C)
{
    if ( C!=i_LowBandColor ) { i_LowBandColor=C; Redraw(); }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_MidBandColor(TColor C)
{
   if ( C!=i_MidBandColor ) { i_MidBandColor=C; Redraw(); }
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::set_HighBandColor(TColor C)
{
  if ( C!=i_HighBandColor ) { i_HighBandColor=C; Redraw(); }
}




//============================== Event handlers =============================

//---------------------------------------------------------------------------
// When receiving WM_PAINT, render Bitmap buffer on Canvas
void __fastcall TAZAnalogGauge::Paint()
{
    if(Initializing) return;
    if(Parent == NULL) return;
    
    Graphics::TBitmap* i_SourceBitmap = ( i_AntiAliasing==aaNone ) ? i_FaceBitmap : i_AABitmap;

    // . . . Transfer Bitmap to Control Canvas
#ifdef AN4_USE_BITBLT
    ::BitBlt(Canvas->Handle, 0, 0,
             i_SourceBitmap->Width, 
             i_SourceBitmap->Height, 
             i_SourceBitmap->Canvas->Handle,
             0, 0, SRCCOPY);
#else
    Canvas->Draw(0, 0, i_SourceBitmap);
#endif   

}

//---------------------------------------------------------------------------
void         __fastcall TAZAnalogGauge::MyMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    CartesianToPolar(X*i_AntiAliasing, Y*i_AntiAliasing, &i_clickedangle);
    if (FOnMouseDown) FOnMouseDown(this, Button, Shift, X, Y);
    Redraw();
}

//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::_Resize()
{
/* . . . Constraints (already done by TControl)
    if ( Constraints->MinWidth && I<Constraints->MinWidth ) Width = Constraints->MinWidth;
    else if ( Constraints->MaxWidth && I>Constraints->MaxWidth ) Width = Constraints->MaxWidth;
    else Width = I;
    if ( Constraints->MinHeight && I<Constraints->MinHeight ) Height = Constraints->MinHeight;
    else if ( Constraints->MaxHeight && I>Constraints->MaxHeight ) Height = Constraints->MaxHeight;
    else Height = I; */

    if (Initializing) return;

// . . . Recalculate geometry (Width changed)
    UpdateDimensions();

// . . . Adjust bitmaps
    // Background bitmap
    i_BodyBitmap->Width  = g.W;
    i_BodyBitmap->Height = g.H;
    // Face bitmap
    i_FaceBitmap->Width  = i_BodyBitmap->Width;
    i_FaceBitmap->Height = i_BodyBitmap->Height;
    
    // Antialiased bitmap
    if ( i_AntiAliasing!=aaNone && i_AABitmap )
         {
          i_AABitmap->Width  = Width;
          i_AABitmap->Height = Height;
         }

// . . . Finally
    Redraw();

    nWidth_Old          = Width;
    nHeight_Old         = Height;

}
//---------------------------------------------------------------------------
//void __fastcall TAZAnalogGauge::CMFontChanged(Messages::TMessage &Msg)
//{
//    i_BodyBitmap->Canvas->Font->Assign(Font);
//    Redraw();
//}
//---------------------------------------------------------------------------

void     __fastcall TAZAnalogGauge::MyResize(TObject *Sender)
{
    if(Initializing) return;
    
    if(FOnResize) FOnResize(this);
    _Resize();
    
}

//---------------------------------------------------------------------------
#ifdef AN4_USE_MMTIMER
void __stdcall TAZAnalogGauge::DynamicModel(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2)
#else
void __fastcall TAZAnalogGauge::DynamicModel(System::TObject* Sender)
#endif
{
    if(ComponentState.Contains(csDesigning) != true) {
    // . . . Calculate transient (detecting if is over)
        i_Timer->Enabled = p.Evolve();
        // Mechanical constraints
        limit_value(&(p.a),i_amin-i_aexceed,i_amin+i_Da+i_aexceed);

    // . . . Update interface
        RedrawPointer();
    }
    
}

//---------------------------------------------------------------------------


inline bool TAZAnalogGauge::ExtBackground()
{
    return(i_BackgroundBmp->Width>0);
} // Tell if an external background is defined
//---------------------------------------------------------------------------

inline bool TAZAnalogGauge::NoExtBackground()
{
    return(i_BackgroundBmp->Width==0);
}
//---------------------------------------------------------------------------
inline void TAZAnalogGauge::PolarToCartesian(const double r, const double a, TPoint* P)
{ // Coordinates transformation
    P->x = g.Pc.x - round(r*std::cos(a));
    P->y = g.Pc.y - round(r*std::sin(a));
}
//---------------------------------------------------------------------------
inline void TAZAnalogGauge::PolarToCartesian(const double r1, const double r2, const double a, TRect* R)
{ // Double coordinates transformation (same angle)
    double cosa     = std::cos(a);
    double sina     = std::sin(a);
    R->Left         = g.Pc.x - round(r1*cosa);
    R->Top          = g.Pc.y - round(r1*sina);
    R->Right        = g.Pc.x - round(r2*cosa);
    R->Bottom       = g.Pc.y - round(r2*sina);
}
//---------------------------------------------------------------------------
inline void TAZAnalogGauge::CartesianToPolar(const int X, const int Y, double* a)
{ // Coordinates transformation
    *a = std::atan2(double(g.Pc.y-Y), double(g.Pc.x-X));
    if (*a < 0) *a += 2*M_PI;
    //*r = std::sqrt(std::pow(g.Pc.y-P.y,2)+std::pow(g.Pc.x-P.x,2));
    // *r = (g.Pc.x-P.x) / std::cos(*a);  *r = (g.Pc.y-P.y) / std::sin(*a);
}
//---------------------------------------------------------------------------
static inline int TAZAnalogGauge::round (double x) // Rounds to nearest integer
{
    if (x<-MaxInt) x=-MaxInt; else if (x>MaxInt) x=MaxInt;
    return static_cast<int>(x>0?x+.5:x-.5);
}
//---------------------------------------------------------------------------
static inline void TAZAnalogGauge::limit_value (double* x, const double m, const double M) // Limit value
{
    if (*x<m) *x=m; else if (*x>M) *x=M;
}


//============================= Discarded methods ===========================
/*
//---------------------------------------------------------------------------
void __fastcall TAZAnalogGauge::SetPenStyles(TPen* Pen, int Width, TColor Color)
{
    HPEN HP;
    LOGBRUSH LB;

    LB.lbStyle = BS_SOLID;
    LB.lbColor = Color;
    LB.lbHatch = 0;
    HP = ::ExtCreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_ROUND, Width, &LB, 0, NULL);
    if ( HP == 0 ) { Pen->Width = Width; Pen->Color = Color; }
    else Pen->Handle = HP;
}


//---------------------------------------------------------------------------
// Antialiasing: resize hires bitmap to control size
// inlined because called once, is executed at every draw, so has to be fast
static inline void __fastcall TAZAnalogGauge::ResizeBitmap(Graphics::TBitmap* Bsrc, Graphics::TBitmap* Bdst)
{
    ASSERTING((Bsrc->PixelFormat==pf24bit && Bdst->PixelFormat==pf24bit),"Must use 24bit bitmaps")
    //ASSERTING(((Bsrc->Width/Bdst->Width)==i_AntiAliasing),"Uncoherent bmp width dimensions")
    //ASSERTING(((Bsrc->Height/Bdst->Height)==i_AntiAliasing),"Uncoherent bmp height dimensions")

// . . . Used variables
    int Ky = Bsrc->Height / Bdst->Height;
    int Kx = Bsrc->Width / Bdst->Width;
    int K = min(Ky,Kx); // Rescaling factor
    int src_y, src_x;
    int Rsum, Gsum, Bsum;
    TRGBTriple *Row1,*Row2,*Row3,*Row4,*DestRow; // PRGBTriple arrays

// . . . For each row
  for ( int y=0, yend=Bdst->Height; y<yend; ++y )
      {
       // We compute samples of i_AntiAliasing x i_AntiAliasing pixels
       src_y = K * y;

       // Get supersampled bitmap pixels
       Row1 = static_cast<TRGBTriple*>(Bsrc->ScanLine[src_y]);
       Row2 = (K>1) ? static_cast<TRGBTriple*>(Bsrc->ScanLine[src_y+1]) : 0;
       Row3 = (K>2) ? static_cast<TRGBTriple*>(Bsrc->ScanLine[src_y+2]) : 0;
       Row4 = (K>3) ? static_cast<TRGBTriple*>(Bsrc->ScanLine[src_y+3]) : 0;

       // Get_ a pointer to destination row in output bitmap
       DestRow = static_cast<TRGBTriple*>(Bdst->ScanLine[y]);

       // . . . For each column
       for ( int x=0, xend=Bdst->Width; x<xend; ++x )
           {
            // We compute samples of K x K pixels
            src_x = K * x;
            // Initialize result color
            Rsum = Gsum = Bsum = 0;

            switch ( K )
                   {
                    case 4 :
                        for ( int i=0; i<4; ++i )
                            {
                             Rsum += Row1[src_x].rgbtRed + Row2[src_x].rgbtRed + Row3[src_x].rgbtRed + Row4[src_x].rgbtRed;
                             Gsum += Row1[src_x].rgbtGreen + Row2[src_x].rgbtGreen + Row3[src_x].rgbtGreen + Row4[src_x].rgbtGreen;
                             Bsum += Row1[src_x].rgbtBlue + Row2[src_x].rgbtBlue + Row3[src_x].rgbtBlue + Row4[src_x].rgbtBlue;
                             ++src_x;
                            }
                        DestRow[x].rgbtRed = Rsum / 16;
                        DestRow[x].rgbtGreen = Gsum / 16;
                        DestRow[x].rgbtBlue = Bsum / 16;
                        break;
                    case 3 :
                        for ( int i=0; i<3; ++i )
                            {
                             Rsum += Row1[src_x].rgbtRed + Row2[src_x].rgbtRed + Row3[src_x].rgbtRed;
                             Gsum += Row1[src_x].rgbtGreen + Row2[src_x].rgbtGreen + Row3[src_x].rgbtGreen;
                             Bsum += Row1[src_x].rgbtBlue + Row2[src_x].rgbtBlue + Row3[src_x].rgbtBlue;
                             ++src_x;
                            }
                        DestRow[x].rgbtRed = Rsum / 9;
                        DestRow[x].rgbtGreen = Gsum / 9;
                        DestRow[x].rgbtBlue = Bsum / 9;
                        break;
                    case 2 :
                        for ( int i=0; i<2; ++i )
                            {
                             Rsum += Row1[src_x].rgbtRed + Row2[src_x].rgbtRed;
                             Gsum += Row1[src_x].rgbtGreen + Row2[src_x].rgbtGreen;
                             Bsum += Row1[src_x].rgbtBlue + Row2[src_x].rgbtBlue;
                             ++src_x;
                            }
                        DestRow[x].rgbtRed = Rsum / 4;
                        DestRow[x].rgbtGreen = Gsum / 4;
                        DestRow[x].rgbtBlue = Bsum / 4;
                        break;
                    default :
                        DestRow[x].rgbtRed   = Row1[src_x].rgbtRed;
                        DestRow[x].rgbtGreen = Row1[src_x].rgbtGreen;
                        DestRow[x].rgbtBlue  = Row1[src_x].rgbtBlue;
                   } // end switch
           } // end 'for each column'
        } // end 'for each row'
}
*/


//=============================== END OF FILE ===============================
