//---------------------------------------------------------------------------

#include <vcl.h>
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>

#pragma hdrstop

#include "DualSensorLabel.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TDualSensorLabel *)
{
    new TDualSensorLabel(NULL);
}
//---------------------------------------------------------------------------
__fastcall TDualSensorLabel::TDualSensorLabel(TComponent* Owner)
    : TCustomLabel(Owner)
{
	AnsiString currentStyle = TStyleManager::ActiveStyle->Name;

	if(currentStyle == "Windows") 	m_bDarkMode = false;
	else							m_bDarkMode = true;


    bLoaded         = false;
    
    pBitmapBase     = NULL;

    AutoSize        = false;

    ParentColor     = false;
    Width           = 200;
    Height          = 24;
    Alignment       = taCenter;

	FSenLineColor   = clBlack ;
    FSenOnColor     = clLime ;
    FSenOffColor    = clSilver ;
    FSenType        = slRect;
    FSenColorType   = slColorGreen;
    FSenWidth       = 15 ;
    FSenHeight      = 15 ;
    FSenRectRound   = 3;
    FSenOnL         = false ;
    FSenOnR         = false ;
    FSenSpace       = 5 ;    
    FSenLEDImage    = true;
    FSenVisibleL    = true;
    FSenVisibleR    = true;

    FSenOnL2            = false;
    FSenOnR2            = false;
    FSenVisibleL2       = false;
    FSenVisibleR2       = false;

    FSenOnL3            = false;
    FSenOnR3            = false;
    FSenVisibleL3       = false;
    FSenVisibleR3       = false;

    FSenOnL4            = false;
    FSenOnR4            = false;
    FSenVisibleL4       = false;
    FSenVisibleR4       = false;
    
    FSenOutLine         = true;
    FSenOutLineColor    = clBlack;
    FSenOutLineRound    = 0;
    FSenOutSpace        = 5;
	FSenBetweenSpace    = 3;

	if(m_bDarkMode) {
		// Dark Mode
		Color           = (TColor)0x00272727;
		Font->Color     = clSilver;
	}
	else {
        // Normal Windows(Light) Mode
		Color           = clBtnFace;
		Transparent     = true;
	}

    pBmpSenOn       = new Graphics::TBitmap;
    pBmpSenOff      = new Graphics::TBitmap;
    pBmpSenDisable  = new Graphics::TBitmap;

    LoadLEDImage();

    //--------------------------
    if(ComponentState.Contains(csDesigning)) {
        bLoaded = true;
    }      
}

__fastcall TDualSensorLabel::~TDualSensorLabel()
{
    if(pBitmapBase)     delete pBitmapBase;
    if(pBmpSenOn)       delete pBmpSenOn;
    if(pBmpSenOff)      delete pBmpSenOff;
    if(pBmpSenDisable)  delete pBmpSenDisable;
    
}

//---------------------------------------------------------------------------
//namespace Titlelabel
//{
//    void __fastcall PACKAGE Register()
//    {
//         TComponentClass classes[1] = {__classid(TDualSensorLabel)};
//         RegisterComponents(L"AZ", classes, 0);
//    }
//}

//---------------------------------------------------------------------------

void __fastcall TDualSensorLabel::Loaded(void)
{
	TGraphicControl::Loaded();

    bLoaded = true;

    LoadLEDImage();

	Invalidate();
}

void            __fastcall TDualSensorLabel::SetLoaded(bool b)
{ 
    bLoaded = b;
}

void     __fastcall TDualSensorLabel::SetEnabled(bool Value)
{
    if(Enabled != Value) {
        TCustomLabel::SetEnabled(Value);
        Invalidate();
    }

}
void __fastcall TDualSensorLabel::Paint()
{
    DrawOutLine();
    
    DrawSensorL();
    DrawSensorL2();
    DrawSensorL3();
    DrawSensorL4();

    DrawSensorR();
    DrawSensorR2();
    DrawSensorR3();
    DrawSensorR4();
    
    DrawTitle();
}

//---------------------------------------------------------------------------
void __fastcall TDualSensorLabel::LoadLEDImage(void)
{
    if(FSenType == slCircle) {
        switch(FSenColorType) {
            case slColorGreen:
            default:
                pBmpSenOn       ->LoadFromResourceName((int)HInstance, "SensorLED_On");
                pBmpSenOff      ->LoadFromResourceName((int)HInstance, "SensorLED_Off");
                pBmpSenDisable  ->LoadFromResourceName((int)HInstance, "SensorLED_Disable");
                break;
                
            case slColorRed:
                pBmpSenOn       ->LoadFromResourceName((int)HInstance, "SensorLED_Red_On");
                pBmpSenOff      ->LoadFromResourceName((int)HInstance, "SensorLED_Red_Off");
                pBmpSenDisable  ->LoadFromResourceName((int)HInstance, "SensorLED_Disable");
                break;
                
            case slColorBlue:
                pBmpSenOn       ->LoadFromResourceName((int)HInstance, "SensorLED_Blue_On");
                pBmpSenOff      ->LoadFromResourceName((int)HInstance, "SensorLED_Blue_Off");
                pBmpSenDisable  ->LoadFromResourceName((int)HInstance, "SensorLED_Disable");
                break;
        }
    }
    else {
        switch(FSenColorType) {
            case slColorGreen:
            default:
                pBmpSenOn       ->LoadFromResourceName((int)HInstance, "SquareLED_On");
                pBmpSenOff      ->LoadFromResourceName((int)HInstance, "SquareLED_Off");
                if(m_bDarkMode) {
                    pBmpSenDisable  ->LoadFromResourceName((int)HInstance, "SquareLED_Disable_Dark");
                }
                else {
                    pBmpSenDisable  ->LoadFromResourceName((int)HInstance, "SquareLED_Disable");
                }
                break;
                
            case slColorRed:
                pBmpSenOn       ->LoadFromResourceName((int)HInstance, "SquareLED_Red_On");
                pBmpSenOff      ->LoadFromResourceName((int)HInstance, "SquareLED_Red_Off");
                if(m_bDarkMode) {
                    pBmpSenDisable  ->LoadFromResourceName((int)HInstance, "SquareLED_Disable_Dark");
                }
                else {
                    pBmpSenDisable  ->LoadFromResourceName((int)HInstance, "SquareLED_Disable");
                }
                break;
                
            case slColorBlue:
                pBmpSenOn       ->LoadFromResourceName((int)HInstance, "SquareLED_Blue_On");
                pBmpSenOff      ->LoadFromResourceName((int)HInstance, "SquareLED_Blue_Off");
                if(m_bDarkMode) {
                    pBmpSenDisable  ->LoadFromResourceName((int)HInstance, "SquareLED_Disable_Dark");
                }
                else {
                    pBmpSenDisable  ->LoadFromResourceName((int)HInstance, "SquareLED_Disable");
                }
                break;
        }
    }

}

//---------------------------------------------------------------------------

void __fastcall TDualSensorLabel::DrawOutLine()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(!bLoaded) return;
    
    if(this->Width <= 0 || this->Height <=0) return;
    
    Canvas->Lock();
    
    SetBkMode(Canvas->Handle, TRANSPARENT);

    //-------------------------
    // Out-Line
    if(FSenOutLine) {
        if(Transparent) {
            Canvas->Brush->Style = bsClear;
        }
        else {
            Canvas->Brush->Style = bsSolid;
        }

		if(m_bDarkMode) {
			Canvas->Brush->Color = Enabled ? Color            : (TColor)0x00272727;
    		Canvas->Pen->Color   = Enabled ? FSenOutLineColor : (TColor)0x00171411;
		}
		else {
			Canvas->Brush->Color = Enabled ? Color : clBtnFace;
    		Canvas->Pen->Color   = Enabled ? FSenOutLineColor : clGray;
		}

        
        Canvas->RoundRect(1, 0, this->Width-1, this->Height-1, FSenOutLineRound, FSenOutLineRound); 
    }
    else {
        if(!Transparent) {
			Canvas->Brush->Style = bsSolid;

			if(m_bDarkMode) {
				Canvas->Brush->Color = Enabled ? Color : (TColor)0x00272727;
				Canvas->Pen->Color   = Enabled ? Color : (TColor)0x00272727;
			}
			else {
				Canvas->Brush->Color = Enabled ? Color : clBtnFace;
				Canvas->Pen->Color   = Enabled ? Color : clBtnFace;
			}

            Canvas->RoundRect(1, 0, this->Width-1, this->Height-1, FSenOutLineRound, FSenOutLineRound); 
        }
    }

    Canvas->Unlock();
    
}

//---------------------------------------------------------------------------

void __fastcall TDualSensorLabel::DrawSensorL()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(!bLoaded) return;

    if(!FSenVisibleL) return;
    if(Width <= 0 || Height <= 0 || FSenWidth <= 0 || FSenHeight <= 0) return;
    
    //Vcl::Graphics::TCanvas* BuffCanvas = NULL;

    int nX1 = FSenOutLine ? FSenOutSpace + 2 : FSenOutSpace;
    int nY1 = 0;
    int nX2 = nX1 + FSenWidth;
    int nY2 = Height;

    Canvas->Lock();

    if(FSenLEDImage) {
        int nSenWidth  = 20;
        int nSenHeight = 20;
        TColor clTransP = clWhite;
        Graphics::TBitmap* pLEDBitmap = NULL;

        if(!Enabled) {
            pLEDBitmap  = pBmpSenDisable;
        }
        else if(FSenOnL) {
            pLEDBitmap  = pBmpSenOn;
        }
        else {
            pLEDBitmap  = pBmpSenOff;
        }

        nSenWidth  = pLEDBitmap->Width;
        nSenHeight = pLEDBitmap->Height;
        clTransP   = pLEDBitmap->Canvas->Pixels[0][0];
    
        nY1 = nSenWidth >= Height ? 0            : ((Height - nSenWidth) / 2);
        //nY2 = nSenWidth >= Height ? Height       : (nY1 + nSenHeight);
        //nX2 = nX1 + nSenWidth;

        TransparentBlt(
            Canvas->Handle,
            nX1,
            nY1,
            nSenWidth,
            nSenHeight,
            pLEDBitmap->Canvas->Handle,
            0,
            0,
            nSenWidth,
            nSenHeight,
            clTransP);


    }
    else {
        SetBkMode(Canvas->Handle, TRANSPARENT);

        if(Enabled) {
            Canvas->Brush->Color = FSenOnL ? FSenOnColor : FSenOffColor;
            Canvas->Pen->Color   = FSenLineColor;
        }
        else {
            Canvas->Brush->Color = clSilver;
            Canvas->Pen->Color   = clGray;
        }
        
        Canvas->Brush->Style = bsSolid;

        switch(FSenType) {
            case slCircle: 
                nY1 = FSenWidth >= Height ? 0            : ((Height - FSenWidth) / 2);
                nY2 = FSenWidth >= Height ? Height       : (nY1 + FSenWidth);
                nX2 = FSenWidth >= Height ? nX1 + Height : nX1 + FSenWidth;
                
                Canvas->Ellipse(nX1, nY1, nX2, nY2); 
                break;
                
            case slRect:   
                nY1 = FSenHeight >= Height ? 0            : ((Height - FSenHeight) / 2);
                nY2 = FSenHeight >= Height ? Height       : (nY1 + FSenHeight);
                
                Canvas->RoundRect(nX1, nY1, nX2, nY2, FSenRectRound, FSenRectRound); 
                break;
        }
    }
    
    Canvas->Unlock();
}
//---------------------------------------------------------------------------

void __fastcall TDualSensorLabel::DrawSensorL2()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(!bLoaded) return;

    if(!FSenVisibleL2) return;
    if(Width <= 0 || Height <= 0 || FSenWidth <= 0 || FSenHeight <= 0) return;
    
    //Vcl::Graphics::TCanvas* BuffCanvas = NULL;

    int nX1 = FSenOutLine ? FSenOutSpace + 2 : FSenOutSpace;
    int nY1 = 0;
    int nX2 = nX1 + FSenWidth;
    int nY2 = Height;

    Canvas->Lock();

    if(FSenLEDImage) {
        int nSenWidth  = 20;
        int nSenHeight = 20;
        TColor clTransP = clWhite;
        Graphics::TBitmap* pLEDBitmap = NULL;

        if(!Enabled) {
            pLEDBitmap  = pBmpSenDisable;
        }
        else if(FSenOnL2) {
            pLEDBitmap  = pBmpSenOn;
        }
        else {
            pLEDBitmap  = pBmpSenOff;
        }

        nSenWidth  = pLEDBitmap->Width;
        nSenHeight = pLEDBitmap->Height;
        clTransP   = pLEDBitmap->Canvas->Pixels[0][0];
    
        nY1 = nSenWidth >= Height ? 0            : ((Height - nSenWidth) / 2);
        //nY2 = nSenWidth >= Height ? Height       : (nY1 + nSenHeight);
        //nX2 = nX1 + nSenWidth;

        if(FSenVisibleL) {
            nX1 = nX1 + nSenWidth + FSenBetweenSpace;
        }

        TransparentBlt(
            Canvas->Handle,
            nX1,
            nY1,
            nSenWidth,
            nSenHeight,
            pLEDBitmap->Canvas->Handle,
            0,
            0,
            nSenWidth,
            nSenHeight,
            clTransP);


    }
    else {
        SetBkMode(Canvas->Handle, TRANSPARENT);

        if(Enabled) {
            Canvas->Brush->Color = FSenOnL2 ? FSenOnColor : FSenOffColor;
            Canvas->Pen->Color   = FSenLineColor;
        }
        else {
            Canvas->Brush->Color = clSilver;
            Canvas->Pen->Color   = clGray;
        }
        
        Canvas->Brush->Style = bsSolid;

        int nSenWidth = FSenWidth;

        switch(FSenType) {
            case slCircle: 
                nY1 = FSenWidth >= Height ? 0            : ((Height - FSenWidth) / 2);
                nY2 = FSenWidth >= Height ? Height       : (nY1 + FSenWidth);

                nSenWidth = FSenWidth >= Height ? Height : FSenWidth;

                if(FSenVisibleL) {
                    nX1 = nX1 + nSenWidth + FSenBetweenSpace;
                }      

                nX2 = nX1 + nSenWidth;
                
                Canvas->Ellipse(nX1, nY1, nX2, nY2); 
                break;
                
            case slRect:   
                nY1 = FSenHeight >= Height ? 0            : ((Height - FSenHeight) / 2);
                nY2 = FSenHeight >= Height ? Height       : (nY1 + FSenHeight);

                if(FSenVisibleL) {
                    nX1 = nX1 + nSenWidth + FSenBetweenSpace;
                }      

                nX2 = nX1 + nSenWidth;
                
                Canvas->RoundRect(nX1, nY1, nX2, nY2, FSenRectRound, FSenRectRound); 
                break;
        }
    }
    
    Canvas->Unlock();
}

//---------------------------------------------------------------------------

void __fastcall TDualSensorLabel::DrawSensorL3()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(!bLoaded) return;

    if(!FSenVisibleL3) return;
    if(Width <= 0 || Height <= 0 || FSenWidth <= 0 || FSenHeight <= 0) return;
    
    //Vcl::Graphics::TCanvas* BuffCanvas = NULL;

    int nX1 = FSenOutLine ? FSenOutSpace + 2 : FSenOutSpace;
    int nY1 = 0;
    int nX2 = nX1 + FSenWidth;
    int nY2 = Height;
    int nVisible = (int)FSenVisibleL + (int)FSenVisibleL2;

    Canvas->Lock();

    if(FSenLEDImage) {
        int nSenWidth  = 20;
        int nSenHeight = 20;
        TColor clTransP = clWhite;
        Graphics::TBitmap* pLEDBitmap = NULL;

        if(!Enabled) {
            pLEDBitmap  = pBmpSenDisable;
        }
        else if(FSenOnL3) {
            pLEDBitmap  = pBmpSenOn;
        }
        else {
            pLEDBitmap  = pBmpSenOff;
        }

        nSenWidth  = pLEDBitmap->Width;
        nSenHeight = pLEDBitmap->Height;
        clTransP   = pLEDBitmap->Canvas->Pixels[0][0];
    
        nY1 = nSenWidth >= Height ? 0            : ((Height - nSenWidth) / 2);
        //nY2 = nSenWidth >= Height ? Height       : (nY1 + nSenHeight);
        //nX2 = nX1 + nSenWidth;

        nX1 = nX1 + (nSenWidth * nVisible) + (FSenBetweenSpace * nVisible);

        TransparentBlt(
            Canvas->Handle,
            nX1,
            nY1,
            nSenWidth,
            nSenHeight,
            pLEDBitmap->Canvas->Handle,
            0,
            0,
            nSenWidth,
            nSenHeight,
            clTransP);


    }
    else {
        SetBkMode(Canvas->Handle, TRANSPARENT);

        if(Enabled) {
            Canvas->Brush->Color = FSenOnL3 ? FSenOnColor : FSenOffColor;
            Canvas->Pen->Color   = FSenLineColor;
        }
        else {
            Canvas->Brush->Color = clSilver;
            Canvas->Pen->Color   = clGray;
        }
        
        Canvas->Brush->Style = bsSolid;

        int nSenWidth = FSenWidth;

        switch(FSenType) {
            case slCircle: 
                nY1 = FSenWidth >= Height ? 0            : ((Height - FSenWidth) / 2);
                nY2 = FSenWidth >= Height ? Height       : (nY1 + FSenWidth);

                nSenWidth = FSenWidth >= Height ? Height : FSenWidth;

                nX1 = nX1 + (nSenWidth * nVisible) + (FSenBetweenSpace * nVisible);
                nX2 = nX1 + nSenWidth;
                
                Canvas->Ellipse(nX1, nY1, nX2, nY2); 
                break;
                
            case slRect:   
                nY1 = FSenHeight >= Height ? 0            : ((Height - FSenHeight) / 2);
                nY2 = FSenHeight >= Height ? Height       : (nY1 + FSenHeight);

                nX1 = nX1 + (nSenWidth * nVisible) + (FSenBetweenSpace * nVisible);
                nX2 = nX1 + nSenWidth;
                
                Canvas->RoundRect(nX1, nY1, nX2, nY2, FSenRectRound, FSenRectRound); 
                break;
        }
    }
    
    Canvas->Unlock();
}

//---------------------------------------------------------------------------

void __fastcall TDualSensorLabel::DrawSensorL4()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(!bLoaded) return;

    if(!FSenVisibleL4) return;
    if(Width <= 0 || Height <= 0 || FSenWidth <= 0 || FSenHeight <= 0) return;
    
    //Vcl::Graphics::TCanvas* BuffCanvas = NULL;

    int nX1 = FSenOutLine ? FSenOutSpace + 2 : FSenOutSpace;
    int nY1 = 0;
    int nX2 = nX1 + FSenWidth;
    int nY2 = Height;

    Canvas->Lock();

    int nVisible = (int)FSenVisibleL + (int)FSenVisibleL2 + (int)FSenVisibleL3;

    if(FSenLEDImage) {
        int nSenWidth  = 20;
        int nSenHeight = 20;
        TColor clTransP = clWhite;
        Graphics::TBitmap* pLEDBitmap = NULL;

        if(!Enabled) {
            pLEDBitmap  = pBmpSenDisable;
        }
        else if(FSenOnL4) {
            pLEDBitmap  = pBmpSenOn;
        }
        else {
            pLEDBitmap  = pBmpSenOff;
        }

        nSenWidth  = pLEDBitmap->Width;
        nSenHeight = pLEDBitmap->Height;
        clTransP   = pLEDBitmap->Canvas->Pixels[0][0];
    
        nY1 = nSenWidth >= Height ? 0            : ((Height - nSenWidth) / 2);
        //nY2 = nSenWidth >= Height ? Height       : (nY1 + nSenHeight);
        //nX2 = nX1 + nSenWidth;

        nX1 = nX1 + (nSenWidth * nVisible) + (FSenBetweenSpace * nVisible);

        TransparentBlt(
            Canvas->Handle,
            nX1,
            nY1,
            nSenWidth,
            nSenHeight,
            pLEDBitmap->Canvas->Handle,
            0,
            0,
            nSenWidth,
            nSenHeight,
            clTransP);


    }
    else {
        SetBkMode(Canvas->Handle, TRANSPARENT);

        if(Enabled) {
            Canvas->Brush->Color = FSenOnL4 ? FSenOnColor : FSenOffColor;
            Canvas->Pen->Color   = FSenLineColor;
        }
        else {
            Canvas->Brush->Color = clSilver;
            Canvas->Pen->Color   = clGray;
        }
        
        Canvas->Brush->Style = bsSolid;

        int nSenWidth = FSenWidth;

        switch(FSenType) {
            case slCircle: 
                nY1 = FSenWidth >= Height ? 0            : ((Height - FSenWidth) / 2);
                nY2 = FSenWidth >= Height ? Height       : (nY1 + FSenWidth);

                nSenWidth = FSenWidth >= Height ? Height : FSenWidth;

                nX1 = nX1 + (nSenWidth * nVisible) + (FSenBetweenSpace * nVisible);

                nX2 = nX1 + nSenWidth;
                
                Canvas->Ellipse(nX1, nY1, nX2, nY2); 
                break;
                
            case slRect:   
                nY1 = FSenHeight >= Height ? 0            : ((Height - FSenHeight) / 2);
                nY2 = FSenHeight >= Height ? Height       : (nY1 + FSenHeight);

                nX1 = nX1 + (nSenWidth * nVisible) + (FSenBetweenSpace * nVisible);

                nX2 = nX1 + nSenWidth;
                
                Canvas->RoundRect(nX1, nY1, nX2, nY2, FSenRectRound, FSenRectRound); 
                break;
        }
    }
    
    Canvas->Unlock();
}

//---------------------------------------------------------------------------


void __fastcall TDualSensorLabel::DrawSensorR()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(!bLoaded) return;

    if(!FSenVisibleR) return;
    if(Width <= 0 || Height <= 0 || FSenWidth <= 0 || FSenHeight <= 0) return;
    
    //Vcl::Graphics::TCanvas* BuffCanvas = NULL;

    int nX1 = FSenOutLine ? (this->Width - FSenWidth - FSenOutSpace - 3) : (this->Width - FSenWidth - FSenOutSpace - 1);
    int nY1 = 0;
    int nX2 = nX1 + FSenWidth;
    int nY2 = Height;

    Canvas->Lock();

    if(FSenLEDImage) {
        int nSenWidth  = 20;
        int nSenHeight = 20;
        TColor clTransP = clWhite;
        Graphics::TBitmap* pLEDBitmap = NULL;

        if(!Enabled) {
            pLEDBitmap  = pBmpSenDisable;
        }
        else if(FSenOnR) {
            pLEDBitmap  = pBmpSenOn;
        }
        else {
            pLEDBitmap  = pBmpSenOff;
        }

        nSenWidth  = pLEDBitmap->Width;
        nSenHeight = pLEDBitmap->Height;
        clTransP   = pLEDBitmap->Canvas->Pixels[0][0];

        nX1 = FSenOutLine ? (this->Width - nSenWidth - FSenOutSpace - 3) : (this->Width - nSenWidth - FSenOutSpace - 1);
        nX2 = nX1 + nSenWidth;
        nY1 = nSenWidth >= Height ? 0            : ((Height - nSenWidth) / 2);

        TransparentBlt(
            Canvas->Handle,
            nX1,
            nY1,
            nSenWidth,
            nSenHeight,
            pLEDBitmap->Canvas->Handle,
            0,
            0,
            nSenWidth,
            nSenHeight,
            clTransP);


    }
    else {
        SetBkMode(Canvas->Handle, TRANSPARENT);

        if(Enabled) {
            Canvas->Brush->Color = FSenOnR ? FSenOnColor : FSenOffColor;
            Canvas->Pen->Color   = FSenLineColor;
        }
        else {
            Canvas->Brush->Color = clSilver;
            Canvas->Pen->Color   = clGray;
        }
        
        Canvas->Brush->Style = bsSolid;

        switch(FSenType) {
            case slCircle: 
                nX1 = FSenWidth >= Height ? (this->Width - Height - 1) : (this->Width - FSenWidth - 1);
                nX1 = FSenOutLine ? (nX1 - FSenOutSpace - 2) : (nX1 - FSenOutSpace);
                nY1 = FSenWidth >= Height ? 0            : ((Height - FSenWidth) / 2);
                nY2 = FSenWidth >= Height ? Height       : (nY1 + FSenWidth);
                nX2 = FSenWidth >= Height ? nX1 + Height : nX1 + FSenWidth;
                
                Canvas->Ellipse(nX1, nY1, nX2, nY2); 
                break;
                
            case slRect:   
                nY1 = FSenHeight >= Height ? 0            : ((Height - FSenHeight) / 2);
                nY2 = FSenHeight >= Height ? Height       : (nY1 + FSenHeight);
                
                Canvas->RoundRect(nX1, nY1, nX2, nY2, FSenRectRound, FSenRectRound); 
                break;
        }
    }
    
    Canvas->Unlock();
}
//---------------------------------------------------------------------------

void __fastcall TDualSensorLabel::DrawSensorR2()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(!bLoaded) return;

    if(!FSenVisibleR2) return;
    if(Width <= 0 || Height <= 0 || FSenWidth <= 0 || FSenHeight <= 0) return;
    
    //Vcl::Graphics::TCanvas* BuffCanvas = NULL;

    int nX1 = FSenOutLine ? (this->Width - FSenWidth - FSenOutSpace - 3) : (this->Width - FSenWidth - FSenOutSpace - 1);
    int nY1 = 0;
    int nX2 = nX1 + FSenWidth;
    int nY2 = Height;

    Canvas->Lock();

    if(FSenLEDImage) {
        int nSenWidth  = 20;
        int nSenHeight = 20;
        TColor clTransP = clWhite;
        Graphics::TBitmap* pLEDBitmap = NULL;

        if(!Enabled) {
            pLEDBitmap  = pBmpSenDisable;
        }
        else if(FSenOnR2) {
            pLEDBitmap  = pBmpSenOn;
        }
        else {
            pLEDBitmap  = pBmpSenOff;
        }

        nSenWidth  = pLEDBitmap->Width;
        nSenHeight = pLEDBitmap->Height;
        clTransP   = pLEDBitmap->Canvas->Pixels[0][0];

        nX1 = FSenOutLine ? (this->Width - nSenWidth - FSenOutSpace - 3) : (this->Width - nSenWidth - FSenOutSpace - 1);

        if(FSenVisibleR) {
            nX1 = nX1 - nSenWidth - FSenBetweenSpace;
        }
        
        nX2 = nX1 + nSenWidth;
        nY1 = nSenWidth >= Height ? 0            : ((Height - nSenWidth) / 2);

        TransparentBlt(
            Canvas->Handle,
            nX1,
            nY1,
            nSenWidth,
            nSenHeight,
            pLEDBitmap->Canvas->Handle,
            0,
            0,
            nSenWidth,
            nSenHeight,
            clTransP);


    }
    else {
        SetBkMode(Canvas->Handle, TRANSPARENT);

        if(Enabled) {
            Canvas->Brush->Color = FSenOnR2 ? FSenOnColor : FSenOffColor;
            Canvas->Pen->Color   = FSenLineColor;
        }
        else {
            Canvas->Brush->Color = clSilver;
            Canvas->Pen->Color   = clGray;
        }
        
        Canvas->Brush->Style = bsSolid;

        int nSenWidth  = FSenWidth;

        switch(FSenType) {
            case slCircle: 
                nX1 = FSenWidth >= Height ? (this->Width - Height - 1) : (this->Width - FSenWidth - 1);
                nX1 = FSenOutLine ? (nX1 - FSenOutSpace - 2) : (nX1 - FSenOutSpace);
                nY1 = FSenWidth >= Height ? 0            : ((Height - FSenWidth) / 2);
                nY2 = FSenWidth >= Height ? Height       : (nY1 + FSenWidth);

                nSenWidth = FSenWidth >= Height ? Height : FSenWidth;

                if(FSenVisibleR) {
                    nX1 = nX1 - nSenWidth - FSenBetweenSpace;
                }       
                
                nX2 = nX1 + nSenWidth;
                
                Canvas->Ellipse(nX1, nY1, nX2, nY2); 
                break;
                
            case slRect:   
                nY1 = FSenHeight >= Height ? 0            : ((Height - FSenHeight) / 2);
                nY2 = FSenHeight >= Height ? Height       : (nY1 + FSenHeight);

                if(FSenVisibleR) {
                    nX1 = nX1 - nSenWidth - FSenBetweenSpace;
                } 

                nX2 = nX1 + nSenWidth;
                
                Canvas->RoundRect(nX1, nY1, nX2, nY2, FSenRectRound, FSenRectRound); 
                break;
        }
    }
    
    Canvas->Unlock();
}
//---------------------------------------------------------------------------

void __fastcall TDualSensorLabel::DrawSensorR3()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(!bLoaded) return;

    if(!FSenVisibleR3) return;
    if(Width <= 0 || Height <= 0 || FSenWidth <= 0 || FSenHeight <= 0) return;
    
    //Vcl::Graphics::TCanvas* BuffCanvas = NULL;

    int nX1 = FSenOutLine ? (this->Width - FSenWidth - FSenOutSpace - 3) : (this->Width - FSenWidth - FSenOutSpace - 1);
    int nY1 = 0;
    int nX2 = nX1 + FSenWidth;
    int nY2 = Height;
    int nVisible = (int)FSenVisibleR + (int)FSenVisibleR2;

    Canvas->Lock();

    if(FSenLEDImage) {
        int nSenWidth  = 20;
        int nSenHeight = 20;
        TColor clTransP = clWhite;
        Graphics::TBitmap* pLEDBitmap = NULL;

        if(!Enabled) {
            pLEDBitmap  = pBmpSenDisable;
        }
        else if(FSenOnR3) {
            pLEDBitmap  = pBmpSenOn;
        }
        else {
            pLEDBitmap  = pBmpSenOff;
        }

        nSenWidth  = pLEDBitmap->Width;
        nSenHeight = pLEDBitmap->Height;
        clTransP   = pLEDBitmap->Canvas->Pixels[0][0];

        nX1 = FSenOutLine ? (this->Width - nSenWidth - FSenOutSpace - 3) : (this->Width - nSenWidth - FSenOutSpace - 1);

        nX1 = nX1 - (nSenWidth * nVisible) - (FSenBetweenSpace * nVisible);
        
        nX2 = nX1 + nSenWidth;
        nY1 = nSenWidth >= Height ? 0            : ((Height - nSenWidth) / 2);

        TransparentBlt(
            Canvas->Handle,
            nX1,
            nY1,
            nSenWidth,
            nSenHeight,
            pLEDBitmap->Canvas->Handle,
            0,
            0,
            nSenWidth,
            nSenHeight,
            clTransP);


    }
    else {
        SetBkMode(Canvas->Handle, TRANSPARENT);

        if(Enabled) {
            Canvas->Brush->Color = FSenOnR3 ? FSenOnColor : FSenOffColor;
            Canvas->Pen->Color   = FSenLineColor;
        }
        else {
            Canvas->Brush->Color = clSilver;
            Canvas->Pen->Color   = clGray;
        }
        
        Canvas->Brush->Style = bsSolid;

        int nSenWidth  = FSenWidth;

        switch(FSenType) {
            case slCircle: 
                nX1 = FSenWidth >= Height ? (this->Width - Height - 1) : (this->Width - FSenWidth - 1);
                nX1 = FSenOutLine ? (nX1 - FSenOutSpace - 2) : (nX1 - FSenOutSpace);
                nY1 = FSenWidth >= Height ? 0            : ((Height - FSenWidth) / 2);
                nY2 = FSenWidth >= Height ? Height       : (nY1 + FSenWidth);

                nSenWidth = FSenWidth >= Height ? Height : FSenWidth;

                nX1 = nX1 - (nSenWidth * nVisible) - (FSenBetweenSpace * nVisible);
                nX2 = nX1 + nSenWidth;
                
                Canvas->Ellipse(nX1, nY1, nX2, nY2); 
                break;
                
            case slRect:   
                nY1 = FSenHeight >= Height ? 0            : ((Height - FSenHeight) / 2);
                nY2 = FSenHeight >= Height ? Height       : (nY1 + FSenHeight);

                nX1 = nX1 - (nSenWidth * nVisible) - (FSenBetweenSpace * nVisible);
                nX2 = nX1 + nSenWidth;
                
                Canvas->RoundRect(nX1, nY1, nX2, nY2, FSenRectRound, FSenRectRound); 
                break;
        }
    }
    
    Canvas->Unlock();
}
//---------------------------------------------------------------------------

void __fastcall TDualSensorLabel::DrawSensorR4()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(!bLoaded) return;

    if(!FSenVisibleR4) return;
    if(Width <= 0 || Height <= 0 || FSenWidth <= 0 || FSenHeight <= 0) return;
    
    //Vcl::Graphics::TCanvas* BuffCanvas = NULL;

    int nX1 = FSenOutLine ? (this->Width - FSenWidth - FSenOutSpace - 3) : (this->Width - FSenWidth - FSenOutSpace - 1);
    int nY1 = 0;
    int nX2 = nX1 + FSenWidth;
    int nY2 = Height;

    int nVisible = (int)FSenVisibleR + (int)FSenVisibleR2 + (int)FSenVisibleR3;
 
    Canvas->Lock();

    if(FSenLEDImage) {
        int nSenWidth  = 20;
        int nSenHeight = 20;
        TColor clTransP = clWhite;
        Graphics::TBitmap* pLEDBitmap = NULL;

        if(!Enabled) {
            pLEDBitmap  = pBmpSenDisable;
        }
        else if(FSenOnR4) {
            pLEDBitmap  = pBmpSenOn;
        }
        else {
            pLEDBitmap  = pBmpSenOff;
        }

        nSenWidth  = pLEDBitmap->Width;
        nSenHeight = pLEDBitmap->Height;
        clTransP   = pLEDBitmap->Canvas->Pixels[0][0];

        nX1 = FSenOutLine ? (this->Width - nSenWidth - FSenOutSpace - 3) : (this->Width - nSenWidth - FSenOutSpace - 1);

        nX1 = nX1 - (nSenWidth * nVisible) - (FSenBetweenSpace * nVisible);
        
        nX2 = nX1 + nSenWidth;
        nY1 = nSenWidth >= Height ? 0            : ((Height - nSenWidth) / 2);

        TransparentBlt(
            Canvas->Handle,
            nX1,
            nY1,
            nSenWidth,
            nSenHeight,
            pLEDBitmap->Canvas->Handle,
            0,
            0,
            nSenWidth,
            nSenHeight,
            clTransP);


    }
    else {
        SetBkMode(Canvas->Handle, TRANSPARENT);

        if(Enabled) {
            Canvas->Brush->Color = FSenOnR4 ? FSenOnColor : FSenOffColor;
            Canvas->Pen->Color   = FSenLineColor;
        }
        else {
            Canvas->Brush->Color = clSilver;
            Canvas->Pen->Color   = clGray;
        }
        
        Canvas->Brush->Style = bsSolid;

        int nSenWidth  = FSenWidth;

        switch(FSenType) {
            case slCircle: 
                nX1 = FSenWidth >= Height ? (this->Width - Height - 1) : (this->Width - FSenWidth - 1);
                nX1 = FSenOutLine ? (nX1 - FSenOutSpace - 2) : (nX1 - FSenOutSpace);
                nY1 = FSenWidth >= Height ? 0            : ((Height - FSenWidth) / 2);
                nY2 = FSenWidth >= Height ? Height       : (nY1 + FSenWidth);

                nSenWidth = FSenWidth >= Height ? Height : FSenWidth;

                nX1 = nX1 - (nSenWidth * nVisible) - (FSenBetweenSpace * nVisible);
                nX2 = nX1 + nSenWidth;
                
                Canvas->Ellipse(nX1, nY1, nX2, nY2); 
                break;
                
            case slRect:   
                nY1 = FSenHeight >= Height ? 0            : ((Height - FSenHeight) / 2);
                nY2 = FSenHeight >= Height ? Height       : (nY1 + FSenHeight);

                nX1 = nX1 - (nSenWidth * nVisible) - (FSenBetweenSpace * nVisible);
                nX2 = nX1 + nSenWidth;
                
                Canvas->RoundRect(nX1, nY1, nX2, nY2, FSenRectRound, FSenRectRound); 
                break;
        }
    }
    
    Canvas->Unlock();
}

//---------------------------------------------------------------------------

void __fastcall TDualSensorLabel::DrawTitle()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(!bLoaded) return;

    if(Width <= 0 || Height <= 0 || (Width < FSenWidth + FSenSpace)) return;

    Canvas->Lock();
    
    SetBkMode(Canvas->Handle, TRANSPARENT);

    Canvas->Font->Assign(Font);

    if(!Enabled) {
        Canvas->Font->Color = m_bDarkMode ? (TColor)0x00454545 : clGray;
    }

    int nTxtW = Canvas->TextWidth(Caption);
    int nTxtH = Canvas->TextHeight(Caption);

    int nSenWidth = 0;

    if(FSenLEDImage) {
        if(!Enabled)     nSenWidth = pBmpSenDisable->Width;
        else if(FSenOnL) nSenWidth = pBmpSenOn->Width;
        else             nSenWidth = pBmpSenOff->Width;
    }
    else {
        nSenWidth = FSenWidth;
    }

    int nStartX = nSenWidth + FSenSpace + FSenOutSpace;
    nStartX = FSenOutLine ? nStartX + 1 : nStartX;
    int nStartY = (Height - nTxtH) / 2;

    switch(Alignment ) {
        case taLeftJustify:     
            if(FSenVisibleL && FSenVisibleL2) {
                nStartX = nSenWidth * 2 + FSenBetweenSpace + FSenSpace + FSenOutSpace;
            }
            else if(FSenVisibleL || FSenVisibleL2) {
                nStartX = nSenWidth + FSenSpace + FSenOutSpace;
            }
            else {
                nStartX = FSenSpace;
            }
            nStartX = FSenOutLine ? nStartX + 1 : nStartX;
            break;
            
        case taRightJustify:    
            //nStartX = (this->Width - nTxtW - nStartX);
            if(FSenVisibleL && FSenVisibleL2) {
                nStartX = (this->Width - nTxtW - ((nSenWidth * 2) + FSenBetweenSpace + FSenSpace + FSenOutSpace));
            }
            else if(FSenVisibleL || FSenVisibleL2) {
                nStartX = (this->Width - nTxtW - (nSenWidth + FSenSpace + FSenOutSpace));
            }
            else {
                nStartX = FSenSpace;
            }    
            nStartX = FSenOutLine ? nStartX - 1 : nStartX;
            break;
            
        case taCenter:          
            nStartX = ((this->Width - nTxtW) / 2);
            break;
    }

    Canvas->Brush->Style = bsClear;
    Canvas->TextOutA(nStartX, nStartY, Caption);

    Canvas->Unlock();
    
}
//---------------------------------------------------------------------------


void __fastcall TDualSensorLabel::SetSenLineColor(TColor v)
{
    if(FSenLineColor != v) {
        FSenLineColor = v;
        Invalidate();
    }
}

void __fastcall TDualSensorLabel::SetSenOnColor(TColor v)
{
    if(FSenOnColor != v) {
        FSenOnColor = v;
        DrawSensorL();
        DrawSensorR();
        DrawSensorL2();
        DrawSensorR2();
        DrawSensorL3();
        DrawSensorR3();
        DrawSensorL4();
        DrawSensorR4();
    }
}

void __fastcall TDualSensorLabel::SetSenOffColor(TColor v)
{
    if(FSenOffColor != v) {
        FSenOffColor = v;
        DrawSensorL();
        DrawSensorR();
        DrawSensorL2();
        DrawSensorR2(); 
        DrawSensorL3();
        DrawSensorR3();
        DrawSensorL4();
        DrawSensorR4();
        
    } 
}

void __fastcall TDualSensorLabel::SetSenType(TSensorLedType  v)
{
    if(FSenType != v) {
        FSenType = v;

        LoadLEDImage(); 
    
        Invalidate();
    }
}

void __fastcall TDualSensorLabel::SetSenWidth(int    v)
{
    if(FSenWidth != v) {
        FSenWidth = v;
        Invalidate();
    }
}

void __fastcall TDualSensorLabel::SetSenHeight(int    v)
{
    if(FSenHeight != v) {
        FSenHeight = v;
        Invalidate();
    }
}

void __fastcall TDualSensorLabel::SetSenRectRound(int v)
{
    if(FSenRectRound != v) {
        FSenRectRound = v;
        Invalidate();
    }
}

void __fastcall TDualSensorLabel::SetSenOnL(int Index, bool   v)
{
    switch(Index) {
        case 0:
            if(FSenOnL != v) {
                FSenOnL = v;
                DrawSensorL();
            }
            break;

        case 1:
            if(FSenOnL2 != v) {
                FSenOnL2 = v;
                DrawSensorL2();
            }
            break;

        case 2:
            if(FSenOnL3 != v) {
                FSenOnL3 = v;
                DrawSensorL3();
            }
            break;

        case 3:
            if(FSenOnL4 != v) {
                FSenOnL4 = v;
                DrawSensorL4();
            }
            break;            
    }

}

void __fastcall TDualSensorLabel::SetSenOnR(int Index, bool   v)
{
    switch(Index) {
        case 0:
            if(FSenOnR != v) {
                FSenOnR = v;
                DrawSensorR();
            }
            break;

        case 1:
            if(FSenOnR2 != v) {
                FSenOnR2 = v;
                DrawSensorR2();
            }
            break;

        case 2:
            if(FSenOnR3 != v) {
                FSenOnR3 = v;
                DrawSensorR3();
            }
            break;

        case 3:
            if(FSenOnR4 != v) {
                FSenOnR4 = v;
                DrawSensorR4();
            }
            break;            
    }
}

void __fastcall TDualSensorLabel::SetSenSpace(int    v)
{
    if(FSenSpace != v) {
        FSenSpace = v;
        Invalidate();
    }
}


void  __fastcall TDualSensorLabel::SetSenLEDImage(bool v)
{
    if(FSenLEDImage != v) {
        FSenLEDImage = v;
        Invalidate();
    }
}

void  __fastcall TDualSensorLabel::SetSenVisibleL(int Index, bool v)
{
    switch(Index) {
        case 0:
            if(FSenVisibleL != v) {
                FSenVisibleL = v;
                Invalidate();
            }
            break;
            
        case 1:
            if(FSenVisibleL2 != v) {
                FSenVisibleL2 = v;
                Invalidate();
            }
            break;

        case 2:
            if(FSenVisibleL3 != v) {
                FSenVisibleL3 = v;
                Invalidate();
            }
            break;

        case 3:
            if(FSenVisibleL4 != v) {
                FSenVisibleL4 = v;
                Invalidate();
            }
            break;            
    }
  
}

void  __fastcall TDualSensorLabel::SetSenVisibleR(int Index, bool v)
{
    switch(Index) {
        case 0:
            if(FSenVisibleR != v) {
                FSenVisibleR = v;
                Invalidate();
            }
            break;
            
        case 1:
            if(FSenVisibleR2 != v) {
                FSenVisibleR2 = v;
                Invalidate();
            }
            break;

        case 2:
            if(FSenVisibleR3 != v) {
                FSenVisibleR3 = v;
                Invalidate();
            }
            break;

        case 3:
            if(FSenVisibleR4 != v) {
                FSenVisibleR4 = v;
                Invalidate();
            }
            break;            
    }
  
}


void __fastcall TDualSensorLabel::SetSenOutLine(bool  v)
{
    if(FSenOutLine != v) {
        FSenOutLine = v;
        Invalidate();
    }
}

void __fastcall TDualSensorLabel::SetSenOutLineColor(TColor    v)
{
    if(FSenOutLineColor != v) {
        FSenOutLineColor = v;
        Invalidate();
    }
}

void __fastcall TDualSensorLabel::SetSenOutLineRound(int       v)
{
    if(FSenOutLineRound != v) {
        FSenOutLineRound = v;
        Invalidate();
    }
}

void __fastcall TDualSensorLabel::SetSenOutSpace(int       v)
{
    if(FSenOutSpace != v) {
        FSenOutSpace = v;
        Invalidate();
    }
}

void __fastcall TDualSensorLabel::SetSenBetweenSpace(int v)
{
    if(FSenBetweenSpace != v) {
        FSenBetweenSpace = v;
        Invalidate();
    }
}

void __fastcall TDualSensorLabel::SetSenColorType(TSensorLedColorType  v)
{
    if(FSenColorType != v) {
        FSenColorType = v;

        LoadLEDImage(); 
    
        Invalidate();
    }
}

