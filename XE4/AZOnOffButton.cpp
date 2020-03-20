//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "AZOnOffButton.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

#define ON_OFF_BTN_STYLE_COUNT_MAX 15

static inline void ValidCtrCheck(TAZOnOffButton *)
{
    new TAZOnOffButton(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAZOnOffButton::TAZOnOffButton(TComponent* Owner)
    : TImage(Owner)
{
    bLoaded             = false;
    m_bButtonDown       = false;
    ZeroMemory(m_bPNGImageExist, sizeof(m_bPNGImageExist));

//    AutoSize        = true; // 여기서 default로 true를 해주면 AutoSize = false 로 선택하면 보이지가 앖는다. 

    FOn                 = false;
    FButtonStyle        = 0; 
    FNumGlyphs          = 4;
    FGroupIndex         = 0;
    FAllowAllUp         = false;
    FConstProportions   = false;
    FAutoToggle         = true;

    FGlyph              = new Vcl::Graphics::TBitmap;
    FGlyph->OnChange    = GlyphChanged;
    
    for(int i=0; i<ofBtnMax; i++) m_pPNGBtnImage[i] = new TPngImage();
    for(int i=0; i<ofBtnMax; i++) m_pBMPBtnImage[i] = new Graphics::TBitmap;

    //--------------------------
    TImage::OnResize        = MyResize;
    TImage::OnClick         = MyClick;
    TImage::OnMouseDown     = MyMouseDown;
    TImage::OnMouseUp       = MyMouseUp;
    
    //--------------------------
    if(ComponentState.Contains(csDesigning)) {
        bLoaded = true;
        CreateDefaultImage();
        DisplayUpdate();
    }     
}
//---------------------------------------------------------------------------
__fastcall TAZOnOffButton::~TAZOnOffButton()
{
    if(FGlyph) delete FGlyph;
    for(int i=0; i<ofBtnMax; i++) if(m_pPNGBtnImage[i]) delete m_pPNGBtnImage[i];
    for(int i=0; i<ofBtnMax; i++) if(m_pBMPBtnImage[i]) delete m_pBMPBtnImage[i];
}
//---------------------------------------------------------------------------

void __fastcall TAZOnOffButton::Loaded(void)
{
    TGraphicControl::Loaded();

    bLoaded = true;


    String sMsg;
    sMsg.printf(L":oaded : Width = %d, Height = %d", Width, Height);

    CreateDefaultImage();

    DisplayUpdate();

    sMsg.printf(L"Loaded : Width = %d, Height = %d", Width, Height);
}
//---------------------------------------------------------------------------
void __fastcall TAZOnOffButton::SetEnabled(bool b)
{
    TGraphicControl::SetEnabled(b);
    DisplayUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TAZOnOffButton::SetAutoSize(bool b)
{
    TImage::SetAutoSize(b);
    CreateDefaultImage();
    DisplayUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TAZOnOffButton::SetStretch(bool b)
{
    if(b) FSmoothResize = false;
    
    TImage::Stretch = b;
    CreateDefaultImage();
    DisplayUpdate();
}


//-----------------------------------------------------------------------
void __fastcall TAZOnOffButton::Paint()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;
    TImage::Paint();
}

//---------------------------------------------------------------------------

void __fastcall TAZOnOffButton::WndProc(TMessage& Message)
{
    switch(Message.Msg) {
         case WM_LBUTTONDOWN:
            if(ComponentState.Contains(csDesigning) != true) { // 
                if(Enabled && FAutoToggle) {
                    m_bButtonDown = true;
                    DisplayUpdate();
                }
            }
            break;

         case WM_LBUTTONUP:
            if(ComponentState.Contains(csDesigning) != true) { //
                if(Enabled && FAutoToggle) {
                    if(m_bButtonDown){

                        FOn = !FOn;

                        // Group Index 를 사용할 경우 FAllowAllUup이 true가 아니면 On상태의 버튼을 눌러도 Off되지 않고 On 상태를 유지 한다.
                        if((FOn == false) && (FGroupIndex > 0) && (FAllowAllUp != true)) {
                            FOn = true;
                        }
                    }
                    m_bButtonDown = false;
                    if(FOn) OtherGroupButtonUp();
                    DisplayUpdate();
                }
            }
            break;

         case WM_MOUSEHOVER:
            break;

         case WM_MOUSELEAVE:
            if(ComponentState.Contains(csDesigning) != true) { //
            
                if(Enabled && m_bButtonDown) {
                    m_bButtonDown = false;
                    //DisplayUpdate();
                }
            }
            break;

//        case WM_SIZE:
//            CreateDefaultImage();
//            DisplayUpdate(m_eButtonState);
//            break;

//         case WM_SETFONT:
//         case WM_FONTCHANGE:
//            ShowMessage("Font Change");
//             DisplayUpdate(m_eButtonState);
//             break;

//         case WM_NUMSHAPE_RESET:
//            if(Message.LParam != (int)this ) {
//                //if(bChanged) {
//                    //bChanged = false;
//                    DisplayUpdate(m_eButtonState);
//                //}
//            }
//            break;
    }

    switch(Message.Msg) {
        default:
            TGraphicControl::WndProc(Message);
            break;
    }

}

//---------------------------------------------------------------------------
void     __fastcall TAZOnOffButton::MyResize(TObject *Sender)
{
    if(FOnResize) FOnResize(this);
    
    CreateDefaultImage();
            
    DisplayUpdate();

}

//---------------------------------------------------------------------------
void __fastcall TAZOnOffButton::CreateDefaultImage(void)
{
    if((FGlyph->Width > 0) && (FGlyph->Height > 0) && (FNumGlyphs > 0)) {
        // User Define Bitmap Image 사용 -----------------
        int nWidth  = FGlyph->Width / FNumGlyphs;
        int nHeight = FGlyph->Height;
        int nStartX = 0;

        nWidth = nWidth <= 0 ? 1 : nWidth;
        
        for(int i=0; i<FNumGlyphs; i++) {
            m_pBMPBtnImage[i]->Width  = nWidth;
            m_pBMPBtnImage[i]->Height = nHeight;
            m_pBMPBtnImage[i]->Canvas->CopyRect(Rect(0,0,nWidth,nHeight), FGlyph->Canvas, Rect(nStartX,0, nStartX+nWidth, nHeight));
            nStartX += nWidth;
        }

        if((AutoSize == false) && FSmoothResize && (FGlyph->Width % FNumGlyphs == 0)) {
            if(Width != nWidth || Height != nHeight) {
                double dScalH = (double)Height / (double)nHeight;
                double dScalW = (double)Width  / (double)nWidth;
                double fScale = dScalW > dScalH ? dScalH : dScalW ;

//                String sMsg;
//                sMsg.printf(L"fScale=%f", fScale);
//                MessageDlg(sMsg, mtInformation, TMsgDlgButtons() << mbOK, 0);

                Graphics::TBitmap* bmpTemp = new Graphics::TBitmap;
                
                for(int i=0; i<FNumGlyphs; i++) {
                    ScaleImage(m_pBMPBtnImage[i], bmpTemp, fScale);
                    if(bmpTemp->Width > 0) {
                        m_pBMPBtnImage[i]->Width  = bmpTemp->Width;
                        m_pBMPBtnImage[i]->Height = bmpTemp->Height;
                        m_pBMPBtnImage[i]->Assign(bmpTemp);
                    }
                }       

                delete bmpTemp;
            }
        }
    }
    else {
        // Component 내장 PNG Image 사용 -----------------
        
        String sRCName[ofBtnMax];

        sRCName[ofBtnOn]        .printf(L"ofBtn_%d_On",          FButtonStyle);
        sRCName[ofBtnOff]       .printf(L"ofBtn_%d_Off",         FButtonStyle);
        sRCName[ofBtnOnDisable] .printf(L"ofBtn_%d_On_Disable",  FButtonStyle);
        sRCName[ofBtnOffDisable].printf(L"ofBtn_%d_Off_Disable", FButtonStyle);
        sRCName[ofBtnOnDown]    .printf(L"ofBtn_%d_On_Down",     FButtonStyle);
        sRCName[ofBtnOffDown]   .printf(L"ofBtn_%d_Off_Down",    FButtonStyle);

        for(int i=0; i<ofBtnMax; i++) {
            try {
                m_pPNGBtnImage[i]         ->LoadFromResourceName((int)HInstance, sRCName[i]);
                m_bPNGImageExist[i] = true;
            } catch (...) {
                m_bPNGImageExist[i] = false;
            }
        }


        if(AutoSize != true && FSmoothResize) {
            int     nWidth  = Width;
            int     nHeight = Height;

            if(FConstProportions) {
                int     nOrgWidth   = m_pPNGBtnImage[0]->Width;
                int     nOrgHeight  = m_pPNGBtnImage[0]->Height;        

                double  dScalH      = (double)Height / (double)nOrgHeight;
                double  dScalW      = (double)Width  / (double)nOrgWidth;
//                double  fScale      = dScalW > dScalH ? dScalH : dScalW ;


                if(dScalW > dScalH) {
                    nHeight = Height;
                    nWidth  = (double)nOrgWidth * dScalH;
                }   
                else {
                    nWidth  = Width;
                    nHeight = (double)nOrgHeight * dScalW;
                }   
            }
            
            for(int i=0; i<ofBtnMax; i++) {
                SmoothResizePNG(m_pPNGBtnImage[i], nWidth, nHeight);
            }
        }
    }
    
}
//---------------------------------------------------------------------------
void __fastcall TAZOnOffButton::DisplayUpdate()
{
    if((FGlyph->Width > 0) && (FGlyph->Height > 0) && (FNumGlyphs > 0)) {
        // User Define Bitmap Image 사용 -----------------
        DisplayUpdate_BMP();
    }
    else {
        // Component 내장 PNG Image 사용 -----------------
        DisplayUpdate_PNG();
    }
}
//---------------------------------------------------------------------------
void __fastcall TAZOnOffButton::DisplayUpdate_PNG()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(!bLoaded) return;

    //Picture->Graphic =  m_pPNGBtnImage[ofBtnOn];

    if(Enabled) {
        if(FOn) {
            if(m_bButtonDown && m_bPNGImageExist[ofBtnOnDown]) {
                Picture->Graphic =  m_pPNGBtnImage[ofBtnOnDown];
            }
            else {
                Picture->Graphic =  m_pPNGBtnImage[ofBtnOn];
            }
        }
        else {
            if(m_bButtonDown && m_bPNGImageExist[ofBtnOffDown]) {
                Picture->Graphic =  m_pPNGBtnImage[ofBtnOffDown];
            }
            else {
                Picture->Graphic =  m_pPNGBtnImage[ofBtnOff];
            }
        }
    }
    else {
        if(FOn) {
            Picture->Graphic =  m_pPNGBtnImage[ofBtnOnDisable];
        }
        else {
            Picture->Graphic =  m_pPNGBtnImage[ofBtnOffDisable];
        }
    }

    Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TAZOnOffButton::DisplayUpdate_BMP()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(!bLoaded) return;

#if 0    
    Vcl::Graphics::TBitmap* pButtonImg = FGlyph;
    
    int nStartX = 0;
    //int nStartY = 0;
    int nWidth  = pButtonImg->Width / FNumGlyphs;
    int nHeight = pButtonImg->Height;

    nWidth = nWidth <= 0 ? 1 : nWidth;

    switch(FNumGlyphs) {
        case 2:
            if(FOn)                     nStartX = 0;  
            else                        nStartX = nWidth;
            break;

        case 3:
            if(Enabled && FOn)          nStartX = 0;  
            else if(Enabled && !FOn)    nStartX = nWidth;
            else                        nStartX = nWidth * 2; 
            break;

        case 4:
            if(Enabled && FOn)          nStartX = 0;            // On
            else if(!Enabled && FOn)    nStartX = nWidth * 2;   // On Disable
            else if(Enabled && !FOn)    nStartX = nWidth;       // Off
            else                        nStartX = nWidth * 3;   // Off Disable
            break;

        case 6:
            if(Enabled && FOn && !m_bButtonDown)        nStartX = 0;            // On
            else if(Enabled && FOn && m_bButtonDown)    nStartX = nWidth * 2;   // On Down
            else if(!Enabled && FOn)                    nStartX = nWidth * 3;   // On Disable
            else if(Enabled && !FOn && !m_bButtonDown)  nStartX = nWidth * 4;   // Off
            else if(Enabled && !FOn &&  m_bButtonDown)  nStartX = nWidth * 5;   // Off Down
            else                                        nStartX = nWidth * 6;   // Off Disable
            break;

        default:
            nStartX = 0;
            break;
    }

    Picture->Bitmap->Width  = nWidth;
    Picture->Bitmap->Height = nHeight;
    Picture->Bitmap->Canvas->CopyRect(Rect(0,0,nWidth,nHeight), pButtonImg->Canvas, Rect(nStartX,0, nStartX+nWidth, nHeight));
#else
    int nImageIndex = 0;

    switch(FNumGlyphs) {
        case 2:
            if(FOn)                     nImageIndex = 0;  
            else                        nImageIndex = 1;
            break;

        case 3:
            if(Enabled && FOn)          nImageIndex = 0;  
            else if(Enabled && !FOn)    nImageIndex = 1;
            else                        nImageIndex = 2;
            break;

        case 4:
            if(Enabled && FOn)          nImageIndex = 0;            // On
            else if(!Enabled && FOn)    nImageIndex = 1;   // On Disable
            else if(Enabled && !FOn)    nImageIndex = 2;       // Off
            else                        nImageIndex = 3;   // Off Disable
            break;

        case 6:
            if(Enabled && FOn && !m_bButtonDown)        nImageIndex = 0;            // On
            else if(Enabled && FOn && m_bButtonDown)    nImageIndex = 1;   // On Down
            else if(!Enabled && FOn)                    nImageIndex = 2;   // On Disable
            else if(Enabled && !FOn && !m_bButtonDown)  nImageIndex = 3;   // Off
            else if(Enabled && !FOn &&  m_bButtonDown)  nImageIndex = 4;   // Off Down
            else                                        nImageIndex = 5;   // Off Disable
            break;

        default:
            nImageIndex = 0;
            break;
    }

    if(m_pBMPBtnImage[nImageIndex] != NULL) {
        Picture->Bitmap->Width  = m_pBMPBtnImage[nImageIndex]->Width;
        Picture->Bitmap->Height = m_pBMPBtnImage[nImageIndex]->Height;
        Picture->Bitmap->Assign(m_pBMPBtnImage[nImageIndex]);
    }
    
#endif 

    Invalidate();
}
//---------------------------------------------------------------------------
/**
* @fn      : void __fastcall TAZOnOffButton::SmoothResizePNG(TPngImage *apng, int NuWidth, int NuHeight)
* @brief   : PNG Image의 Size를 Resize하는 함수 
* @details : Resize할 때 픽셀 깨짐 없이 부드럽게 Resize 하는 함수이다. 
* @param   : TPngImage *apng : 언본 PNG Image이며 이 Image를 Resize하여 돌려준다. (!주의, 원본이 수정된다)
* @param   : int NuWidth : Resize할 Width
* @param   : int NuHeight : Resize할 Height
* @return  : void : 
* @see     : 
- history  : 1. [2019년 11월 25일 월요일][송신영] 최초 작성 
**/
void __fastcall TAZOnOffButton::SmoothResizePNG(TPngImage *apng, int NuWidth, int NuHeight)
{
    if(apng == NULL) return;

    double      xscale, yscale;
    double      sfrom_y, sfrom_x;
    int         ifrom_y, ifrom_x;
    int         to_y, to_x;
    double      weight_x[2], weight_y[2];
    double      weight;
    int         new_red, new_green;
    int         new_blue, new_alpha;
    int         new_colortype;
    double      total_red, total_green;
    double      total_blue, total_alpha;
    bool        IsAlpha;
    int         ix, iy;
    TPngImage * bTmp;
    pRGBLine    sli, slo;
    pByteArray  ali, alo;

    if(apng->Header->ColorType != COLOR_RGBALPHA && apng->Header->ColorType != COLOR_RGB) {
        // Only COLOR_RGBALPHA and COLOR_RGB formats are supported'
        return;
    }

    if(apng->Width == NuWidth && apng->Height == NuHeight) return;

    IsAlpha = apng->Header->ColorType == COLOR_RGBALPHA ? true : false;

    if(IsAlpha)     new_colortype = COLOR_RGBALPHA;
    else            new_colortype = COLOR_RGB;

    try {
        bTmp = new TPngImage(new_colortype, 8, NuWidth, NuHeight);

        xscale = bTmp->Width / (double)(apng->Width-1);
        yscale = bTmp->Height / (double)(apng->Height-1);

        for (to_y = 0;  to_y < bTmp->Height; to_y++) {
            sfrom_y = to_y / yscale;
            ifrom_y = int(sfrom_y);
            weight_y[1] = sfrom_y - ifrom_y;
            weight_y[0] = 1 - weight_y[1];

            for (to_x = 0; to_x <  bTmp->Width; to_x++ ) {
                sfrom_x = to_x / xscale;
                ifrom_x = int(sfrom_x);
                weight_x[1] = sfrom_x - ifrom_x;
                weight_x[0] = 1 - weight_x[1];

                total_red   = 0.0;
                total_green = 0.0;
                total_blue  = 0.0;
                total_alpha  = 0.0;
                for (ix = 0; ix < 2; ix++ ){
                    for (iy = 0; iy < 2; iy++) {
                        sli = (pRGBLine)apng->Scanline[ifrom_y + iy];
                        if (IsAlpha) ali = apng->AlphaScanline[ifrom_y + iy];

                        new_red   = (*sli)[ifrom_x + ix].rgbtRed;
                        new_green = (*sli)[ifrom_x + ix].rgbtGreen;
                        new_blue  = (*sli)[ifrom_x + ix].rgbtBlue;
                        if (IsAlpha) new_alpha = (*ali)[ifrom_x + ix];
                        weight      = weight_x[ix] * weight_y[iy];
                        total_red   = total_red   + new_red   * weight;
                        total_green = total_green + new_green * weight;
                        total_blue  = total_blue  + new_blue  * weight;
                        if (IsAlpha) total_alpha  = total_alpha  + new_alpha  * weight;
                    }
                }

                slo = (pRGBLine)bTmp->Scanline[to_y];
                if (IsAlpha) alo = bTmp->AlphaScanline[to_y];
                (*slo)[to_x].rgbtRed    = floor(total_red);
                (*slo)[to_x].rgbtGreen  = floor(total_green);
                (*slo)[to_x].rgbtBlue   = floor(total_blue);
                if (IsAlpha) (*alo)[to_x]  = floor(total_alpha);
            }
        }

        apng->Assign(bTmp);
        delete bTmp; 
        bTmp = NULL;
    }
    catch(...) {
        if(bTmp) delete bTmp;
    }

}
//---------------------------------------------------------------------------

void __fastcall  TAZOnOffButton::SetOn(bool b)
{
    if(FOn != b) {
//        bool bOldValue = FOn;
        FOn = b;

        if(FOn) {
            OtherGroupButtonUp();
        }
        DisplayUpdate();
    }
}
//---------------------------------------------------------------------------

void __fastcall  TAZOnOffButton::SetButtonStyle(int n)
{
    if(FButtonStyle != n && (n < ON_OFF_BTN_STYLE_COUNT_MAX) && (n >= 0)) {
        FButtonStyle = n;
        CreateDefaultImage();
        DisplayUpdate();
    }
}

//---------------------------------------------------------------------------
void __fastcall TAZOnOffButton::SetGlyph(Vcl::Graphics::TBitmap* Value)
{
    if(Value == NULL) {
        FGlyph->Width  = 0;
        FGlyph->Height = 0;
    }
    else {
        FGlyph->Assign(Value);
    }

    CreateDefaultImage();
    DisplayUpdate( );
}

Vcl::Graphics::TBitmap* __fastcall TAZOnOffButton::GetGlyph(void)
{
    return FGlyph;
}

TNumGlyphs __fastcall TAZOnOffButton::GetNumGlyphs(void)
{
    return FNumGlyphs;
}

void __fastcall TAZOnOffButton::SetNumGlyphs(TNumGlyphs Value)
{
    if(FNumGlyphs != Value) {
        FNumGlyphs = Value;
        if((FGlyph->Width > 0) && (FGlyph->Height > 0) && (FNumGlyphs > 0)) {
            CreateDefaultImage();
        }
        DisplayUpdate();
    }
}

void __fastcall TAZOnOffButton::GlyphChanged(System::TObject* Sender)
{
    CreateDefaultImage();
    DisplayUpdate();
}

//---------------------------------------------------------------------------

void __fastcall TAZOnOffButton::SetGroupIndex(int n)
{
    if(FGroupIndex != n) {
        FGroupIndex = n;
//        if(FGroupIndex == 0) {
//            FOn = false;
//        }
        if(FOn) OtherGroupButtonUp();
        DisplayUpdate();
    }
}

//---------------------------------------------------------------------------
void  __fastcall TAZOnOffButton::SetAllowAllUp(bool b)
{
    if(FAllowAllUp != b) {
        FAllowAllUp = b;
        DisplayUpdate();
    }
}
//---------------------------------------------------------------------------
void  __fastcall TAZOnOffButton::SetSmoothResize(bool b)
{
    if(FSmoothResize != b) {
        FSmoothResize = b;

        if(FSmoothResize) TImage::Stretch = false;

        CreateDefaultImage();
        DisplayUpdate();
    }
}
//---------------------------------------------------------------------------
void __fastcall TAZOnOffButton::SetConstProportions(bool b)
{
    if(FConstProportions != b) {
        FConstProportions = b;
        CreateDefaultImage();
        DisplayUpdate();
    }
}
//---------------------------------------------------------------------------
void __fastcall TAZOnOffButton::SetAutoToggle(bool b)
{
    if(FAutoToggle != b) {
        FAutoToggle = b;
    }
}

//---------------------------------------------------------------------------
void __fastcall  TAZOnOffButton::MyClick(TObject *Sender)
{
//    FOn = !FOn;
//
//    if(FOn) OtherGroupButtonUp();
//    
//    DisplayUpdate();

    if(FOnClick) {
        FOnClick(this, FOn);
    }
}
//---------------------------------------------------------------------------
void    __fastcall TAZOnOffButton::MyMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(FOnMouseDown) {
        FOnMouseDown(this, Button, Shift, X, Y, FOn);
    }
}

//---------------------------------------------------------------------------
void    __fastcall TAZOnOffButton::MyMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(FOnMouseUp) {
        FOnMouseUp(this, Button, Shift, X, Y, FOn);
    }
}
//---------------------------------------------------------------------------


void __fastcall TAZOnOffButton::OtherGroupButtonUp()
{
    if(FGroupIndex == 0) return;
    
    //---------------------------------------------
    TWinControl *WndCtrl = this->Parent;

    while(WndCtrl == NULL) {
        return;
    }
    //---------------------------------------------

    int nCount = WndCtrl->ControlCount;

    //int nCount = WndCtrl->ComponentCount;
    TAZOnOffButton *p = NULL;

    for ( int i = 0; i < nCount; i++ ) {
        if ( WndCtrl->Controls[i]->ClassNameIs(L"TAZOnOffButton")){
            p = dynamic_cast<TAZOnOffButton *>(WndCtrl->Controls[i]);
            if((p != NULL) && (p != this)){
                if(p->On && (p->GroupIndex == FGroupIndex)) p->On = false;
            }
        }
    }
}
//---------------------------------------------------------------------------

