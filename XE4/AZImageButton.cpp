//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop
#include "Define.h"
#include "AZImageButton.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAZImageButton *)
{
    new TAZImageButton(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAZImageButton::TAZImageButton(TComponent* Owner)
    : TImage(Owner)
{
    bLoaded         = false;
    
    TImage::OnResize = MyResize;

    //FCaption    = "Caption";

    m_eButtonState = IB_STATE_UP;

    m_bButtonDown = false;
    FAlignment  = taCenter;
    FGroupIndex = 0;
    FAllowAllUp = false;
    
    FOnFontChangeOrg = Font->OnChange;
    Font->OnChange = FontChanged;
    //Font->Size = 16;
    //Font->Color = clWhite;
    
    //--------------------------
    FGlyph = new Vcl::Graphics::TBitmap;
    FGlyph->OnChange = GlyphChanged;

    FNumGlyphs = 4;

    //--------------------------    
    m_pDefaultImage = new Vcl::Graphics::TBitmap;

    FDefaultColor       = (TColor)0xA53E1E;
    FDefaultLineColor   = (TColor)0x952E0E;
    FDefaultLine        = true;

    //--------------------------
    FCaptionSub = new TImageBtnSubCaptionProperty;
    FCaptionSub->OnChange = CaptionSubChanged;
    
    
    //--------------------------
    if(ComponentState.Contains(csDesigning)) {
        bLoaded = true;
        CreateDefaultImage();
    }      
}

//-----------------------------------------------------------------------
__fastcall TAZImageButton::~TAZImageButton()
{
    if(FGlyph)              delete FGlyph;
    if(m_pDefaultImage)     delete m_pDefaultImage;
    if(FCaptionSub)         delete FCaptionSub;
}
//---------------------------------------------------------------------------

void __fastcall TAZImageButton::Loaded(void)
{
    TGraphicControl::Loaded();

    bLoaded = true;
    
    CreateDefaultImage();

    DisplayUpdate(m_eButtonState);
}
//---------------------------------------------------------------------------

void __fastcall TAZImageButton::WndProc(TMessage& Message)
{
    switch(Message.Msg) {
         case WM_LBUTTONDOWN:
            if(ComponentState.Contains(csDesigning) != true) { // 

                if(Enabled) {
                    m_bButtonDown = true;
                    DisplayUpdate(IB_STATE_DOWN);
                }
            }
            break;

         case WM_LBUTTONUP:
            if(ComponentState.Contains(csDesigning) != true) { //
            
                if(Enabled && m_bButtonDown) {
                    if(FGroupIndex != 0) {

                        if(FDown) {
                            if(FAllowAllUp) {
                                m_eButtonState = IB_STATE_UP; 
                                FDown          = false;
                            }
                        }
                        else {
                            OtherGroupButtonUp();
                            m_eButtonState = IB_STATE_SELECT;  
                            FDown          = true;
                        }
                    }
                    else {
                        m_eButtonState = IB_STATE_UP;
                        FDown          = false;
                    }
                    
                    DisplayUpdate(m_eButtonState);
                }
            }
            break;

         case WM_MOUSEHOVER:
            break;

         case WM_MOUSELEAVE:
            if(ComponentState.Contains(csDesigning) != true) { //
            
                if(Enabled && m_bButtonDown) {
                    m_bButtonDown = false;
                    DisplayUpdate(m_eButtonState);
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

void     __fastcall TAZImageButton::MyResize(TObject *Sender)
{
    if(FOnResize) FOnResize(this);
    
    CreateDefaultImage();
            
    DisplayUpdate(m_eButtonState);
}

//---------------------------------------------------------------------------
void __fastcall TAZImageButton::CreateDefaultImage(void)
{
    int nW = Width;
    int nH = Height;

    m_pDefaultImage->Width  = nW * 4;
    m_pDefaultImage->Height = nH;

    Vcl::Graphics::TBitmap* pBitmapBtn[IB_STATE_MAX];

    for(int i=0; i<IB_STATE_MAX; i++) {
        pBitmapBtn[i]           = new Vcl::Graphics::TBitmap;
        pBitmapBtn[i]->Width    = nW;
        pBitmapBtn[i]->Height   = nH;
    }

    CreateColorImage(pBitmapBtn[IB_STATE_UP],       FDefaultColor,      50, 5, 20);
    CreateColorImage(pBitmapBtn[IB_STATE_DISABLE],  clSilver,           20, 1, 20);
    CreateColorImage(pBitmapBtn[IB_STATE_DOWN],     clAqua,             50, 5, 23);
    CreateColorImage(pBitmapBtn[IB_STATE_SELECT],   (TColor)0xFF8000,   50, 5, 23);

    m_pDefaultImage->Canvas->CopyRect(Rect(0,   0,nW,  nH), pBitmapBtn[IB_STATE_UP]->Canvas,        Rect(0, 0, nW, nH));
    m_pDefaultImage->Canvas->CopyRect(Rect(nW,  0,nW*2,nH), pBitmapBtn[IB_STATE_DISABLE]->Canvas,   Rect(0, 0, nW, nH));
    m_pDefaultImage->Canvas->CopyRect(Rect(nW*2,0,nW*3,nH), pBitmapBtn[IB_STATE_DOWN]->Canvas,      Rect(0, 0, nW, nH));
    m_pDefaultImage->Canvas->CopyRect(Rect(nW*3,0,nW*4,nH), pBitmapBtn[IB_STATE_SELECT]->Canvas,    Rect(0, 0, nW, nH));

    delete pBitmapBtn[IB_STATE_UP];
    delete pBitmapBtn[IB_STATE_DISABLE];
    delete pBitmapBtn[IB_STATE_DOWN];       
    delete pBitmapBtn[IB_STATE_SELECT];     
    
}

void __fastcall TAZImageButton::CreateColorImage(Vcl::Graphics::TBitmap* pBitmap, 
                                                 TColor cColor, 
                                                 int nTopDownGap,
                                                 int nLineInterval,
                                                 int nCenterLine)
{
    if(pBitmap == NULL) return;
    
    Vcl::Graphics::TBitmap* pBitmapTmp;
    pBitmapTmp          = new Vcl::Graphics::TBitmap;
    pBitmapTmp->Width   = 1;
    pBitmapTmp->Height  = 40;
    
    int nRGB = ColorToRGB(cColor);
    int nB = (nRGB & 0xFF0000) >> 16;
    int nG = (nRGB & 0x00FF00) >> 8;
    int nR = (nRGB & 0x0000FF);

    pBitmapTmp->Canvas->Pixels[0][nCenterLine] = cColor;

    int nTempR = nR + nTopDownGap;
    int nTempG = nG + nTopDownGap;
    int nTempB = nB + nTopDownGap;

    for(int i=(nCenterLine - 1); i>=0; i--) {

        nTempR += nLineInterval;
        nTempG += nLineInterval;
        nTempB += nLineInterval;

        nTempR = nTempR > 255 ? 255 : nTempR;
        nTempG = nTempG > 255 ? 255 : nTempG;
        nTempB = nTempB > 255 ? 255 : nTempB;

        int nTempRGB = (nTempB << 16) | (nTempG << 8) | nTempR;
        pBitmapTmp->Canvas->Pixels[0][i] = (TColor)nTempRGB;
    }

    nTempR = nR;
    nTempG = nG;
    nTempB = nB;

    for(int i=(nCenterLine + 1); i<pBitmapTmp->Height; i++) {

        nTempR += nLineInterval;
        nTempG += nLineInterval;
        nTempB += nLineInterval;

        nTempR = nTempR > 255 ? 255 : nTempR;
        nTempG = nTempG > 255 ? 255 : nTempG;
        nTempB = nTempB > 255 ? 255 : nTempB;

        int nTempRGB = (nTempB << 16) | (nTempG << 8) | nTempR;
        pBitmapTmp->Canvas->Pixels[0][i] = (TColor)nTempRGB;
    }

    pBitmap->Canvas->StretchDraw(Rect(0,0,pBitmap->Width, pBitmap->Height), pBitmapTmp);

    if(FDefaultLine) {
        pBitmap->Canvas->Pen->Color = FDefaultLineColor;
        pBitmap->Canvas->MoveTo(0,0);
        pBitmap->Canvas->LineTo(pBitmap->Width-1,0);
        pBitmap->Canvas->LineTo(pBitmap->Width-1,pBitmap->Height-1);
        pBitmap->Canvas->LineTo(0,pBitmap->Height-1);
        pBitmap->Canvas->LineTo(0,0);
    }
    
    delete pBitmapTmp;

}
//---------------------------------------------------------------------------
void __fastcall TAZImageButton::SetEnabled(bool b)
{
    TGraphicControl::SetEnabled(b);
    DisplayUpdate(m_eButtonState);
}

//-----------------------------------------------------------------------
void __fastcall TAZImageButton::Paint()
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;
    TImage::Paint();
}

//---------------------------------------------------------------------------
void __fastcall TAZImageButton::DrawImageTrans(
    Graphics::TBitmap* pDest,
    int X,
    int Y,
    Graphics::TBitmap* srcBmp)
{
    srcBmp->Transparent = true;
    srcBmp->TransparentColor = clFuchsia;
    pDest->Canvas->Draw(X, Y, srcBmp);
}
//---------------------------------------------------------------------------
void __fastcall TAZImageButton::DrawTextTrans(
    Graphics::TBitmap* pDest,
    Graphics::TBitmap* pTextBitmap,
    int             X,
    int             Y,
    AnsiString      sString,
    TColor          cColor,
    unsigned int    Align,
    int             nFontSize,
    bool            bBold)
{

    if(pDest == NULL)       return;
    if(pTextBitmap == NULL) return;
    
    TRect rc;

    pTextBitmap->FreeImage();
    pTextBitmap->Canvas->Font->Name = "Arial";
    pTextBitmap->Canvas->Font->Size = nFontSize;
    pTextBitmap->Canvas->Font->Style = bBold ? TFontStyles() << fsBold : TFontStyles() >> fsBold;
    pTextBitmap->Width = pTextBitmap->Canvas->TextWidth(sString) + 1;
    pTextBitmap->Height = pTextBitmap->Canvas->TextHeight(sString);
    pTextBitmap->Canvas->Brush->Color = clFuchsia;
    pTextBitmap->Canvas->Font->Color = cColor;

    rc.Top      = 0;
    rc.Left     = 0;
    rc.Right    = pTextBitmap->Width;
    rc.Bottom   = pTextBitmap->Height;

    pTextBitmap->Canvas->FillRect(rc);
    pTextBitmap->Canvas->TextOutA(1 , 0, sString);
    pTextBitmap->Transparent = true;
    pTextBitmap->TransparentColor = clFuchsia;

    if(Align & TEXT_ALIGN_BOTTOM){
        Y = Y - pTextBitmap->Height;
    }
    else if(Align & TEXT_ALIGN_V_CENTER){
        Y = Y - (pTextBitmap->Height/2);
    }

    if(Align & TEXT_ALIGN_RIGHT){
        X = X - pTextBitmap->Width;
    }
    else if(Align & TEXT_ALIGN_H_CENTER){
        X = X - (pTextBitmap->Width/2);
    }

    DrawImageTrans(pDest, X, Y, pTextBitmap);
}


void  __fastcall TAZImageButton::SetCaption(String s)
{
    if(s != FCaption) {
        FCaption = s;
        DisplayUpdate(m_eButtonState);
    }
}

void __fastcall TAZImageButton::SetCaptionSub(TImageBtnSubCaptionProperty * p)
{
    if(p) {
        FCaptionSub->Assign(p);
        DisplayUpdate(m_eButtonState);
    }
}

void __fastcall TAZImageButton::SetGlyph(Vcl::Graphics::TBitmap* Value)
{
    if(Value == NULL) {
        FGlyph->Width  = 0;
        FGlyph->Height = 0;
    }
    else {
        FGlyph->Assign(Value);
    }

    DisplayUpdate(m_eButtonState);
}

Vcl::Graphics::TBitmap* __fastcall TAZImageButton::GetGlyph(void)
{
    return FGlyph;
}

TNumGlyphs __fastcall TAZImageButton::GetNumGlyphs(void)
{
    return FNumGlyphs;
}

void __fastcall TAZImageButton::SetNumGlyphs(TNumGlyphs Value)
{
    if(FNumGlyphs != Value) {
        FNumGlyphs = Value;
        //CreateIndicatorImage();
        DisplayUpdate(m_eButtonState);
    }
}

void     __fastcall TAZImageButton::SetAlignment(System::Classes::TAlignment Value)
{
    if(FAlignment != Value) {
        FAlignment = Value;
        DisplayUpdate(m_eButtonState);
    }
}

void  __fastcall TAZImageButton::SetSpace(int Index, int Value )
{
    switch(Index) {
        case 0:
            if(FSpaceH != Value) {
                FSpaceH = Value; 
                DisplayUpdate(m_eButtonState);
            } 
            break;

        case 1:
            if(FSpaceV != Value) {
                FSpaceV = Value; 
                DisplayUpdate(m_eButtonState);
            } 
            break;            
    }
}

void __fastcall TAZImageButton::SetGroupIndex(int n)
{
    if(FGroupIndex != n) {
        FGroupIndex = n;
        if(FGroupIndex == 0) FDown = false;
        DisplayUpdate(m_eButtonState);
    }
}

void __fastcall TAZImageButton::SetAllowAllUp(bool b)
{
    if(FAllowAllUp != b) {
        FAllowAllUp = b;
        DisplayUpdate(m_eButtonState);
    }
}

void __fastcall TAZImageButton::SetDown(bool b)
{
    if(FDown != b) {
//        if(FGroupIndex != 0) {
//            FDown = b;
//            m_eButtonState = FDown ? IB_STATE_SELECT : IB_STATE_UP;                                 
//            DisplayUpdate(m_eButtonState);
//        }

        if(FGroupIndex != 0) {

            if(b) {
                OtherGroupButtonUp();
                m_eButtonState = IB_STATE_SELECT; 
                FDown          = true;
            }
            else {
                if(FAllowAllUp) {
                    m_eButtonState = IB_STATE_UP; 
                    FDown          = false;
                }
            }
        }
        else {
            m_eButtonState = IB_STATE_UP;
            FDown          = false;
        }

        DisplayUpdate(m_eButtonState);
        
    }
}

void __fastcall TAZImageButton::SetDefaultColor(TColor c)
{
    if(FDefaultColor != c) {
        FDefaultColor = c;
        CreateDefaultImage();
        DisplayUpdate(m_eButtonState);
    }
}

void __fastcall TAZImageButton::SetDefaultLineColor(TColor c)
{
    if(FDefaultLineColor != c) {
        FDefaultLineColor = c;
        CreateDefaultImage();
        DisplayUpdate(m_eButtonState);
    }
}

void __fastcall TAZImageButton::SetDefaultLine(bool b)
{
    if(FDefaultLine != b) {
        FDefaultLine = b;
        CreateDefaultImage();
        DisplayUpdate(m_eButtonState);
    }
}

void __fastcall TAZImageButton::CaptionSubChanged(System::TObject* Sender)
{
    DisplayUpdate(m_eButtonState);
}

void __fastcall TAZImageButton::GlyphChanged(System::TObject* Sender)
{
    //CreateIndicatorImage();
    DisplayUpdate(m_eButtonState);
}

void __fastcall TAZImageButton::DisplayUpdate(EImageButtonState eState)
{
    if(!ComponentState.Contains(csDesigning) && !Visible) return;

    if(!bLoaded) return;
    
    Vcl::Graphics::TBitmap* pButtonImg = NULL;
    
    if(FGlyph->Width == 0 || FGlyph->Height == 0) {
        pButtonImg = m_pDefaultImage;
    }
    else {
        pButtonImg = FGlyph;
    }
    
    int nStartX = 0;
    int nStartY = 0;
    int nWidth  = pButtonImg->Width / FNumGlyphs;
    int nHeight = pButtonImg->Height;

    nWidth = nWidth <= 0 ? 1 : nWidth;

    eState = Enabled ? eState : IB_STATE_DISABLE;

    switch(FNumGlyphs) {
        case 1: nStartX = 0;
            break;

        case 2:
            switch(eState) {
                case IB_STATE_UP :      nStartX = 0;            break;
                case IB_STATE_DOWN:     nStartX = nWidth;       break;
                default:                nStartX = 0;            break;
            }              
            break;

        case 3:
            switch(eState) {
                case IB_STATE_UP :      nStartX = 0;            break;
                case IB_STATE_DISABLE:  nStartX = nWidth;       break;
                case IB_STATE_DOWN:     nStartX = nWidth * 2;   break;
                case IB_STATE_SELECT:   nStartX = nWidth * 2;   break;
                default:                nStartX = 0;            break;
            }            
            break;

        case 4:
            switch(eState) {
                case IB_STATE_UP :      nStartX = 0;            break;
                case IB_STATE_DISABLE:  nStartX = nWidth;       break;
                case IB_STATE_DOWN:     nStartX = nWidth * 2;   break;
                case IB_STATE_SELECT:   nStartX = nWidth * 3;   break;
                default:                nStartX = 0;            break;
            }            
            break;
    }
    


    Picture->Bitmap->Width  = nWidth;
    Picture->Bitmap->Height = nHeight;
    Picture->Bitmap->Canvas->CopyRect(Rect(0,0,nWidth,nHeight), pButtonImg->Canvas, Rect(nStartX,0, nStartX+nWidth, nHeight));


    // Caption  --------------------------
    int nTxtW = 0, nTxtH = 0;
    nStartY = 0;
    nStartX = 0;
    
    Picture->Bitmap->Canvas->Font->Assign(Font);

    if(!Enabled) {
        Picture->Bitmap->Canvas->Font->Color            = clGray;
    }

    nTxtW = Picture->Bitmap->Canvas->TextWidth(FCaption);
    nTxtH = Picture->Bitmap->Canvas->TextHeight(FCaption);

    switch(FAlignment ) {
        case taLeftJustify:
            nStartX = FSpaceH;
            break;
        case taRightJustify:
            nStartX = (nWidth - nTxtW) - FSpaceH;
            break;
        case taCenter:
            nStartX = (nWidth - nTxtW) / 2 + FSpaceH;
            break;
    }

    nStartY = (nHeight - nTxtH) / 2 + FSpaceV;

    SetBkMode(Picture->Bitmap->Canvas->Handle, TRANSPARENT); //
    Picture->Bitmap->Canvas->TextOutA(nStartX, nStartY, FCaption);

    // Sub Caption -----------------------
    if(FCaptionSub->Visible) {
        
        Picture->Bitmap->Canvas->Font->Assign(FCaptionSub->Font);

        if(!Enabled) {
            Picture->Bitmap->Canvas->Font->Color = clGray;
        }

        int nTxtW_s = Picture->Bitmap->Canvas->TextWidth(FCaptionSub->Caption);
        int nTxtH_s = Picture->Bitmap->Canvas->TextHeight(FCaptionSub->Caption);

        if(FCaptionSub->VAlignTop) {
            nStartY = nStartY - nTxtH_s - FCaptionSub->SpaceV;
        }
        else {
            nStartY = nStartY + nTxtH + FCaptionSub->SpaceV;
        }
        
        switch(FAlignment ) {
            case taLeftJustify:
                nStartX = FSpaceH + FCaptionSub->SpaceH;
                break;
            case taRightJustify:
                nStartX = (nWidth - nTxtW_s) - FSpaceH - FCaptionSub->SpaceH;
                break;
            case taCenter:
                nStartX = (nWidth - nTxtW_s) / 2 + FSpaceH + FCaptionSub->SpaceH;
                break;
        }

        SetBkMode(Picture->Bitmap->Canvas->Handle, TRANSPARENT); //
        Picture->Bitmap->Canvas->TextOutA(nStartX, nStartY, FCaptionSub->Caption);
    }

    Invalidate();
}

void __fastcall TAZImageButton::OtherGroupButtonUp()
{
    //---------------------------------------------
    TWinControl *WndCtrl = this->Parent;

    while(WndCtrl == NULL) {
        return;
    }
    //---------------------------------------------

    int nCount = WndCtrl->ControlCount;

    //int nCount = WndCtrl->ComponentCount;
    TAZImageButton *p = NULL;

    for ( int i = 0; i < nCount; i++ ) {
        if ( WndCtrl->Controls[i]->ClassNameIs("TAZImageButton")){
            p = dynamic_cast<TAZImageButton *>(WndCtrl->Controls[i]);
            if((p != NULL) && (p != this)){
                if(p->Down && (p->GroupIndex == FGroupIndex)) p->Up();
            }
        }
    }
}

void __fastcall TAZImageButton::Up()
{
    m_eButtonState = IB_STATE_UP; 
    FDown          = false;
    DisplayUpdate(m_eButtonState);
}


void __fastcall TAZImageButton::FontChanged(System::TObject* Sender)
{
    FOnFontChangeOrg(Sender);
    DisplayUpdate(m_eButtonState);
}
//-----------------------------------------------------------------------------










//-----------------------------------------------------------------------------

__fastcall TImageBtnSubCaptionProperty::TImageBtnSubCaptionProperty() {
    FVisible    = true;
    //FCaption    = "Sub Caption";
    FVAlign     = false;
    FVSpace     = 0;
    FHSpace     = 0;
    FOnChange   = NULL;

    //------------------------------------------------
    // Unit을 표현할 Font를 생성한다.
    FFont           = new Vcl::Graphics::TFont;
    FFont->OnChange = FontChanged; // Font가 변경될 경우 이를 감지할 Call Back 함수를 등록한다.
    FFont->Color    = clWhite;
}

__fastcall TImageBtnSubCaptionProperty::~TImageBtnSubCaptionProperty() {
    if(FFont) delete FFont;
}

void     __fastcall TImageBtnSubCaptionProperty::SetVisible(bool b) {
    if(FVisible != b) { FVisible = b; DoOnChange(); }
}

void     __fastcall TImageBtnSubCaptionProperty::SetCaption(String s) {
    if(FCaption != s) { FCaption = s; DoOnChange(); }
}

void     __fastcall TImageBtnSubCaptionProperty::SetVAlign(bool b) {
    if(FVAlign != b) { FVAlign = b; DoOnChange(); }
}

void     __fastcall TImageBtnSubCaptionProperty::SetVSpace(int n) {
    if(FVSpace != n) { FVSpace = n; DoOnChange(); }
}

void     __fastcall TImageBtnSubCaptionProperty::SetHSpace(int n) {
    if(FHSpace != n) { FHSpace = n; DoOnChange(); }
}

void     __fastcall TImageBtnSubCaptionProperty::SetFont(Vcl::Graphics::TFont * p) {
    FFont->Assign(p);
    DoOnChange();
}

void    __fastcall TImageBtnSubCaptionProperty::Assign(TPersistent* Source) {
    TImageBtnSubCaptionProperty* pFrom = dynamic_cast<TImageBtnSubCaptionProperty *>(Source);

    if(pFrom != NULL) { //Source 가 NULL 이 아니고 안전하게 타입캐스팅 되었다면
        FVisible    = pFrom->Visible;
        FCaption    = pFrom->Caption;
        FVAlign     = pFrom->VAlignTop;
        FVSpace     = pFrom->SpaceV; 
        FHSpace     = pFrom->SpaceH;  
        FFont->Assign(pFrom->Font);
    }
}

void     __fastcall TImageBtnSubCaptionProperty::DoOnChange(void)
{
    if(FOnChange) FOnChange(this);  // 이벤트 핸들러 연결
}

void     __fastcall TImageBtnSubCaptionProperty::FontChanged(System::TObject* Sender)
{
    DoOnChange();
}
//-----------------------------------------------------------------------------











//-----------------------------------------------------------------------------





    
