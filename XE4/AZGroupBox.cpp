//---------------------------------------------------------------------------

#include <vcl.h>
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>

#pragma hdrstop

#include "AZGroupBox.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//


#define AZ_RGB_TO_GRAY(r, g, b) ((r)*0.299 + (g)*0.587 + (b)*0.114)

static inline void ValidCtrCheck(TAZGroupBox *)
{
    new TAZGroupBox(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAZGroupBox::TAZGroupBox(TComponent* Owner)
    : TCustomPanel(Owner) // TCustomGroupBox(Owner)
{
	AnsiString currentStyle = TStyleManager::ActiveStyle->Name;

	if(currentStyle == "Windows") {
        m_bDarkMode = false;
	}
	else {
        m_bDarkMode = true;
	}

    m_pBGImage          = new TImage(this);
    m_pBGImage->Parent  = this;

    FGlyph = new Vcl::Graphics::TBitmap;
    FGlyph->OnChange = GlyphChanged;

    FNumGlyphs          = 3;
    FrameTopMargin      = 4;
    FGlyphDefaultStyle  = gdtColorGreen;
    FGlyphType          = lbsDefault;
    FAlignmentText      = taCenter; // taLeftJustify;
    FAlignmentLabel     = taLeftJustify;

    FTitleMarginX       = 10;
    FTitleMarginY       = 0;
    FTitleWidth         = 150;
    FTitleHeight        = 29;

    FTextMarginX        = 0;

    FTitleBGColor       = clBtnFace;
    FTitleRectColor     = clGray;
    FFrameColor         = clGray;
    FGradientStartColor = clWhite;
    FGradientEndColor   = (TColor)0xDFE7E6; // ( 223<< 16 | 231 << 8 | 230 );
    FTransparentColor   = clFuchsia;
    
    FFrameRound         = 10;
    FTitleRound         = 8;
    FFrameLineBold      = false;

    FShadowFont         = false;
    FShadowFontColor    = clBlack;
    FShadowFontLeft     = -1;
    FShadowFontTop      = -1;

    FTransparent        = false;

    m_bLoaded           = false;

    m_bRefreshChildControl = false;


    FOnFontChangeOrg    = Font->OnChange;
    Font->OnChange      = FontChanged;

    TWinControl::OnResize   = MyResize;

    //--------------------------
    if(ComponentState.Contains(csDesigning)) {
        m_bLoaded       = true;

        DisplayUpdate();
    }        

    CalTitleRect();
    
}
//---------------------------------------------------------------------------
//namespace Azgroupbox
//{
//    void __fastcall PACKAGE Register()
//    {
//         TComponentClass classes[1] = {__classid(TAZGroupBox)};
//         RegisterComponents(L"AZ", classes, 0);
//    }
//}
////---------------------------------------------------------------------------

__fastcall TAZGroupBox::~TAZGroupBox()
{
    if(FGlyph)      delete FGlyph;
    if(m_pBGImage)  delete m_pBGImage;
}
//---------------------------------------------------------------------------
void __fastcall TAZGroupBox::Loaded(void)
{
	TCustomPanel::Loaded(); // TCustomGroupBox::Loaded();
    m_bLoaded = true;

    m_bRefreshChildControl = true;

    CalTitleRect();

	DisplayUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TAZGroupBox::CalTitleRect(void)
{
    //-----------------------------------------
    // Title Label Back Ground 그리기.
    //-----------------------------------------
    switch(FAlignmentLabel) {
        case taLeftJustify:
            m_recTitle = Rect(FTitleMarginX, FTitleMarginY, FTitleMarginX + FTitleWidth, FTitleMarginY + FTitleHeight);
            break;
            
        case taRightJustify:
            m_recTitle = Rect(Width - FTitleMarginX - FTitleWidth, FTitleMarginY, Width - FTitleMarginX, FTitleMarginY + FTitleHeight);
            break;
            
        default:
        case taCenter:
            m_recTitle = Rect((Width - FTitleWidth)/2, FTitleMarginY, (Width - FTitleWidth)/2 + FTitleWidth, FTitleMarginY + FTitleHeight);
            break;
    }

    if(m_bLoaded) {
        switch(FGlyphType) {
            default:
            case lbsDefault:
                m_recTitle.Bottom = m_recTitle.Top + FTitleHeight;
                break;
                
            case lbsRectangle:
                m_recTitle.Bottom = m_recTitle.Top + FTitleHeight;
                break;
                
            case lbsCustom:
                if(FGlyph->Height > 0) {
                    if(FNumGlyphs == 1) {
                        // 커스텀이고 등록한 그림 그대로 나타내는 모드일 경우 그림의 Width, Height를 적용해 줘야 한다. 
                        switch(FAlignmentLabel) {
                            case taLeftJustify:
                                m_recTitle = Rect(FTitleMarginX, FTitleMarginY, FTitleMarginX + FGlyph->Width, FTitleMarginY + FGlyph->Height);
                                break;
                                
                            case taRightJustify:
                                m_recTitle = Rect(Width - FTitleMarginX - FGlyph->Width, FTitleMarginY, Width - FTitleMarginX, FTitleMarginY + FGlyph->Height);
                                break;
                                
                            default:
                            case taCenter:
                                m_recTitle = Rect((Width - FGlyph->Width)/2, FTitleMarginY, (Width - FGlyph->Width)/2 + FGlyph->Width, FTitleMarginY + FGlyph->Height);
                                break;
                        }
                    }
                    else {
                        // 커스텀이고 3등분 이미지를 적용한 경우 Height만 등록한 Image의 것을 적용 한다. 
                        m_recTitle.Bottom = m_recTitle.Top + FGlyph->Height;
                    }
                }
                break;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TAZGroupBox::Paint(void)
{
    // 
}
//---------------------------------------------------------------------------
void __fastcall TAZGroupBox::Changed(void)
{
    if(m_bLoaded != true) return;
#if 0
    Invalidate();
#else
    DisplayUpdate();
#endif
}
//---------------------------------------------------------------------------
void     __fastcall TAZGroupBox::MyResize(TObject *Sender)
{
    if(FOnResize) FOnResize(this);

    m_bRefreshChildControl = true;
    
    DisplayUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TAZGroupBox::FontChanged(System::TObject* Sender)
{
    FOnFontChangeOrg(Sender);
    Changed();
}

//---------------------------------------------------------------------------
void __fastcall TAZGroupBox::DisplayUpdate(void)
{
    //Caption = "Test";
    //Stdctrls::TGroupBox::Paint();

    if(!Visible && (ComponentState.Contains(csDesigning) != true)) return;


    int       H;
    TRect     R;
    Longint   Flags;
    TRect     CaptionRect, OuterRect;
    TSize     Size;
    TThemedButton Box;
    TThemedElementDetails Details;

    int nRoundX         = FFrameRound;    // 모서리 Round 크기 X
    int nRoundY         = FFrameRound;    // 모서리 Round 크기 Y
//    int nTitleBGWidth   = 100;

//    int nTitleOffsetX   = 0;
    int nTitleBG_Width_L = 0;
    int nTitleBG_Width_R = 0;

    TCanvas* pCanvas =  m_pBGImage->Picture->Bitmap->Canvas;

    Graphics::TBitmap * pBitMap             = new Graphics::TBitmap;
    
    R = Rect(0, FFrameTopMargin, Width, Height);

    m_pBGImage->Transparent = true;
    m_pBGImage->Picture->Bitmap->Transparent            = true;
    m_pBGImage->Picture->Bitmap->TransparentColor       = FTransparentColor;
    m_pBGImage->Picture->Bitmap->Canvas->Brush->Color   = FTransparentColor;

    if(m_pBGImage->Width != Width || m_pBGImage->Height != Height) {
        m_pBGImage->Width   = Width;
        m_pBGImage->Height  = Height;
        m_pBGImage->Left    = 0;
        m_pBGImage->Top     = 0;
        m_pBGImage->Picture->Bitmap->Width  = Width;
        m_pBGImage->Picture->Bitmap->Height = Height;
    }

    pCanvas->Font = Font;

    pCanvas->FillRect(Rect(0,0,m_pBGImage->Picture->Bitmap->Width, m_pBGImage->Picture->Bitmap->Height));

    //-----------------------------------------
    // Gradient Round Rect Background 그리기.
    //-----------------------------------------
    if(FTransparent != true) {
        Graphics::TBitmap * pBitMapGradientBG   = new Graphics::TBitmap;
        
        try {

            pBitMap->Width  = R.Width();
            pBitMap->Height = R.Height();

            TColor clStartColor = Enabled ? FGradientStartColor : clWhite;
            TColor clEndColor   = Enabled ? FGradientEndColor   : clSilver;

            if(!Enabled) {
                double dR, dG, dB;
                unsigned int grayColorStart = AZ_RGB_TO_GRAY(GetRValue(FGradientStartColor), GetGValue(FGradientStartColor), GetBValue(FGradientStartColor));
                unsigned int grayColorEnd   = AZ_RGB_TO_GRAY(GetRValue(FGradientEndColor), GetGValue(FGradientEndColor), GetBValue(FGradientEndColor));

                clStartColor = (TColor)(grayColorStart << 16 | grayColorStart << 8 | grayColorStart);
                clEndColor   = (TColor)(grayColorEnd << 16 | grayColorEnd << 8 | grayColorEnd);
            }

            GradientFillCanvas(pBitMap->Canvas, clStartColor, clEndColor, Rect(0, 0, pBitMap->Width, pBitMap->Height), gdVertical);
#if 1
            // 하위 컴포넌트에 영향을 주어 이 방식 사용 안하고
            HRGN hRgn = CreateRoundRectRgn(R.Left, R.Top, R.Right, R.Bottom, nRoundX, nRoundY);
            OffsetRgn(hRgn, 1, 1);
            SelectClipRgn(pCanvas->Handle, hRgn);
            //pCanvas->StretchDraw(Rect(R.Left, 0, R.Right, R.Bottom), pBitMap);
            pCanvas->Draw(R.Left, R.Top, pBitMap);
            SelectClipRgn(pCanvas->Handle, 0);
            DeleteObject(hRgn);
#else
            // Bitmap에 Round Rect에 그라데이션이 적용된 완전한 그림을 하나 그리고 그것을 다시 그리는 방식 사용 한다.
            pBitMapGradientBG->Width  = R.Width();
            pBitMapGradientBG->Height = R.Height();
            pBitMapGradientBG->Canvas->Brush->Color = clRed;
            pBitMapGradientBG->Canvas->FillRect(TRect(0, 0, pBitMapGradientBG->Width, pBitMapGradientBG->Height));
            pBitMapGradientBG->Transparent = true;
            pBitMapGradientBG->TransparentColor = clRed;

            HRGN hRgn = CreateRoundRectRgn(0, 0, pBitMapGradientBG->Width, pBitMapGradientBG->Height, nRoundX-1, nRoundY-1);
            //OffsetRgn(hRgn, 1, 1);
            SelectClipRgn(pBitMapGradientBG->Canvas->Handle, hRgn);
            pBitMapGradientBG->Canvas->StretchDraw(TRect(0, 0, pBitMapGradientBG->Width, pBitMapGradientBG->Height), pBitMap);
            SelectClipRgn(pBitMapGradientBG->Canvas->Handle, 0);
            DeleteObject(hRgn);

            //pCanvas->Draw(0, R.Top, pBitMapGradientBG);
            pCanvas->Draw(0, 0, pBitMapGradientBG);
#endif

        } catch (...) {

        }

        if(pBitMapGradientBG != NULL) delete pBitMapGradientBG;

    }

    //-----------------------------------------
    // Round Rect Frame 그리기.
    //-----------------------------------------
    try {
        pCanvas->Pen->Width      = 1;
        pCanvas->Pen->Color      = Enabled ? FFrameColor : clGray;
        pCanvas->Brush->Style    = bsClear;
        pCanvas->RoundRect(R.Left, R.Top, R.Right, R.Bottom, nRoundX, nRoundY);

        if(FFrameLineBold) {
            pCanvas->RoundRect(R.Left+1, R.Top+1, R.Right-1, R.Bottom-1, nRoundX, nRoundY);
            pCanvas->RoundRect(R.Left+1, R.Top, R.Right-1, R.Bottom, nRoundX, nRoundY);        
        }
    } catch (...) {

    }

    //-----------------------------------------
    // Title Label Back Ground 그리기.
    //-----------------------------------------

    CalTitleRect();
    
    switch(FGlyphType) {
        default:
        case lbsDefault:
            {
                String sImageRSName;

                if(Enabled) {
                    switch(FGlyphDefaultStyle) {
                        default:
                        case gdtColorGreen:     sImageRSName = "TitleLabel_Green";
                            break;

                        case gltColorBlue:      sImageRSName = "TitleLabel_Blue";
                            break;

                        case gltColorRed:       sImageRSName = "TitleLabel_Red";
                            break;

                        case gltColorBlack:     sImageRSName = "TitleLabel_Black";
                            break;

                        case gltColorGray:      sImageRSName = "TitleLabel_Gray";
                            break;

                        case gltColorDisable:   sImageRSName = "TitleLabel_Disable";
                            break;
                    }
                }
                else {
                    sImageRSName = "TitleLabel_Disable";
                }

                Graphics::TBitmap * pBitMapRC   = new Graphics::TBitmap;

                try {
                    pBitMapRC->LoadFromResourceName((int)HInstance, sImageRSName);

                    m_recTitle.Bottom = m_recTitle.Top + pBitMapRC->Height;

                    pBitMap->Width  = pBitMapRC->Width/3;
                    pBitMap->Height = pBitMapRC->Height ;

                    // Left
                    pBitMap->Canvas->CopyRect(Rect(0, 0, pBitMap->Width,pBitMap->Height), pBitMapRC->Canvas, Rect(0, 0, pBitMap->Width, pBitMapRC->Height));
                    pBitMap->Transparent = true ;
                    pBitMap->TransparentColor = FTransparentColor;
                    pCanvas->Draw(m_recTitle.Left, m_recTitle.Top, pBitMap);
                    nTitleBG_Width_L = pBitMap->Width;

                    // Right
                    pBitMap->Canvas->CopyRect(Rect(0, 0, pBitMap->Width,pBitMap->Height), pBitMapRC->Canvas, Rect(pBitMap->Width * 2, 0, pBitMapRC->Width, pBitMapRC->Height));
                    pCanvas->Draw(m_recTitle.Right - pBitMap->Width, m_recTitle.Top, pBitMap);
                    nTitleBG_Width_R = pBitMap->Width;

                    // Center
                    pBitMap->Canvas->CopyRect(Rect(0, 0, pBitMap->Width, pBitMap->Height), pBitMapRC->Canvas, Rect(pBitMap->Width, 0, pBitMap->Width * 2, pBitMapRC->Height));
                    pCanvas->StretchDraw(TRect(m_recTitle.Left + nTitleBG_Width_L, m_recTitle.Top, m_recTitle.Right - nTitleBG_Width_R, m_recTitle.Bottom), pBitMap);

                }
                catch (...) {

                }

                delete pBitMapRC;
            }
            break;

        case lbsRectangle:
            {
                pCanvas->Pen->Width      = 1;

                if(m_bDarkMode) {
                    pCanvas->Pen->Color      = Enabled ? FTitleRectColor : (TColor)0x004A4A4A; // clSilver;
                    pCanvas->Brush->Color    = Enabled ? FTitleBGColor   : (TColor)0x00444444; // clBtnFace;
                }
                else {
                    pCanvas->Pen->Color      = Enabled ? FTitleRectColor : clSilver;
                    pCanvas->Brush->Color    = Enabled ? FTitleBGColor   : clBtnFace;
                }
                pCanvas->Brush->Style    = bsSolid;
                pCanvas->RoundRect(m_recTitle.Left, m_recTitle.Top+1, m_recTitle.Right, m_recTitle.Bottom, FTitleRound, FTitleRound);

            }
            break;

        case lbsCustom:
            try
            {   
                m_recTitle.Bottom = m_recTitle.Top + FGlyph->Height;
                
                if(FNumGlyphs == 3 && (FGlyph->Width > 0)) {
                    pBitMap->Width  = FGlyph->Width/3;
                    pBitMap->Height = FGlyph->Height ;

                    // Left
                    pBitMap->Canvas->CopyRect(Rect(0,0,pBitMap->Width,pBitMap->Height), FGlyph->Canvas, Rect(0, 0, pBitMap->Width, FGlyph->Height));
                    pBitMap->Transparent = true ;
                    pBitMap->TransparentColor = FTransparentColor;
                    pCanvas->Draw(m_recTitle.Left, m_recTitle.Top, pBitMap);
                    nTitleBG_Width_L = pBitMap->Width;

                    // Right
                    pBitMap->Canvas->CopyRect(Rect(0,0,pBitMap->Width,pBitMap->Height), FGlyph->Canvas, Rect(pBitMap->Width * 2, 0, FGlyph->Width, FGlyph->Height));
                    pCanvas->Draw(m_recTitle.Right - pBitMap->Width, m_recTitle.Top, pBitMap);
                    nTitleBG_Width_R = pBitMap->Width;

                    // Center
                    pBitMap->Canvas->CopyRect(Rect(0,0,pBitMap->Width,pBitMap->Height), FGlyph->Canvas, Rect(pBitMap->Width, 0, pBitMap->Width * 2, FGlyph->Height));
                    pCanvas->StretchDraw(TRect(m_recTitle.Left + nTitleBG_Width_L, m_recTitle.Top, m_recTitle.Right - nTitleBG_Width_R, m_recTitle.Bottom), pBitMap);
                }
                else if(FGlyph->Width > 0){
                    FGlyph->Transparent = true;
                    FGlyph->TransparentColor = FTransparentColor;
                    pCanvas->Draw(m_recTitle.Left, m_recTitle.Top, FGlyph);
                }
            }
            catch (...) {

            }
            break;

    }

    delete pBitMap;


    //---------------------------------------
    // Caption
    //---------------------------------------
    pCanvas->Brush->Style = bsClear;

    try {
        InflateRect(m_recTitle, -FTextMarginX, 0);
        AnsiString sCaption = FCaption;
        UINT uiFormat = 0;

        if(FShadowFont || !Enabled) {

            switch(FAlignmentText) {
                case taLeftJustify:     uiFormat = DT_SINGLELINE|DT_VCENTER|DT_LEFT;    break;
                case taRightJustify:    uiFormat = DT_SINGLELINE|DT_VCENTER|DT_RIGHT;   break;
                case taCenter:
                default:                uiFormat = DT_SINGLELINE|DT_VCENTER|DT_CENTER;  break;
            }

            if(m_bDarkMode) {
                pCanvas->Font->Color = Enabled ? FShadowFontColor : (TColor)0x00525252;
                DrawText(pCanvas->Handle, sCaption.c_str() , -1, &m_recTitle, uiFormat);
                OffsetRect(m_recTitle, FShadowFontLeft, FShadowFontTop);
                pCanvas->Font->Color = Enabled ? Font->Color : (TColor)0x00121212;
                DrawText(pCanvas->Handle, sCaption.c_str() , -1, &m_recTitle, uiFormat);

            }
            else {
                pCanvas->Font->Color = Enabled ? FShadowFontColor : clWhite;
                DrawText(pCanvas->Handle, sCaption.c_str() , -1, &m_recTitle, uiFormat);
                OffsetRect(m_recTitle, FShadowFontLeft, FShadowFontTop);
                pCanvas->Font->Color = Enabled ? Font->Color : clGray;
                DrawText(pCanvas->Handle, sCaption.c_str() , -1, &m_recTitle, uiFormat);
            }
            
        }
        else {
            switch(FAlignmentText) {
                case taLeftJustify:     uiFormat = DT_SINGLELINE|DT_VCENTER|DT_LEFT;    break;
                case taRightJustify:    uiFormat = DT_SINGLELINE|DT_VCENTER|DT_RIGHT;   break;
                default:
                case taCenter:          uiFormat = DT_SINGLELINE|DT_VCENTER|DT_CENTER;  break;
            }

            pCanvas->Font->Color = Font->Color;

            DrawText(pCanvas->Handle, sCaption.c_str() , -1, &m_recTitle, uiFormat);
        }
    }
    catch (...) {

    }

    //---------------------------------------
    // 배경 색이 변경될 경우 자식 컴포넌트들을 다시 그려야 한다.
    //---------------------------------------
    if(m_bRefreshChildControl) {
        m_bRefreshChildControl = false;

        for(int i=0; i<ControlCount; i++) {

            TWinControl * pWinCtrl = dynamic_cast<TWinControl *>(Controls[i]);
            if(pWinCtrl != NULL) {
                pWinCtrl->Invalidate();
            }
            else {
                TGraphicControl * pGraphicCtrl = dynamic_cast<TGraphicControl *>(Controls[i]);
                if(pGraphicCtrl != NULL) {
                    pGraphicCtrl->Invalidate();
                }
            }
        }
    }

}

void __fastcall TAZGroupBox::AdjustClientRect(System::Types::TRect &Rect)
{
    // AdjustClientRect는 클라이언트 영역 내에서 자식 컨트롤을 배치 할 위치에 대한 정확한 정보가 컨트롤에 필요한 경우 내부적으로 호출됩니다.
    //Stdctrls::TGroupBox::AdjustClientRect(Rect);

    CalTitleRect();

    Canvas->Font = Font;
    Rect.Top = m_recTitle.Bottom; // Canvas->TextHeight('0');

    // Rect 값을 X, Y로 1씩 줄여 준다.
    InflateRect(Rect, -1, -1);

    if (Ctl3D) {
        // Rect 값을 X, Y로 1씩 줄여 준다.
        InflateRect(Rect, -1, -1);
    }
}

//---------------------------------------------------------------------------
// Enable / Disable 시에 색상 지정.
void __fastcall TAZGroupBox::SetEnabled(bool b)
{
    m_bRefreshChildControl = true;

	// TCustomGroupBox::SetEnabled(b);
	TCustomPanel::SetEnabled(b);

    Changed();
}

void __fastcall TAZGroupBox::SetGlyphDefaultStyle(TGBGlyphDefaultColor e) {
    if(FGlyphDefaultStyle != e) {
        FGlyphDefaultStyle = e;
        Changed();
    }
};

void __fastcall TAZGroupBox::SetAlignmentText(System::Classes::TAlignment n) {
    if(FAlignmentText != n) {
        FAlignmentText = n;
        Changed();
    }
};

void __fastcall TAZGroupBox::SetAlignmentLabel(System::Classes::TAlignment n) {
    if(FAlignmentLabel != n) {
        FAlignmentLabel = n;
        Changed();
    }
};

void __fastcall TAZGroupBox::SetFrameTopMargin(int n) {
    if(FFrameTopMargin != n) {
        FFrameTopMargin = n;
        Changed();
    }
};

void __fastcall TAZGroupBox::SetGlyphType(TGBGlyphType e) {
    if(FGlyphType != e) {
        FGlyphType = e;
        Changed();
    }
};

void  __fastcall TAZGroupBox::SetGlyph(Vcl::Graphics::TBitmap* Value) {
    if(Value == NULL) {
        FGlyph->Width  = 0;
        FGlyph->Height = 0;
        FGlyphType = lbsDefault;
        Changed();
    }
    else {
        FGlyphType = lbsCustom;
        FGlyph->Assign(Value);
        Changed();
    }
};

Vcl::Graphics::TBitmap* __fastcall TAZGroupBox::GetGlyph(void) {
    return FGlyph;
};

void                    __fastcall TAZGroupBox::SetNumGlyphs(TNumGlyphs Value) {
    if(FNumGlyphs != Value) {
        FNumGlyphs = Value;
        Changed();
    }
};

TNumGlyphs              __fastcall TAZGroupBox::GetNumGlyphs(void) {
    return FNumGlyphs;
};


void __fastcall TAZGroupBox::GlyphChanged(System::TObject* Sender)
{
    // Changed();
};

void __fastcall TAZGroupBox::SetTitleAlignValue(int Index, int n) {
    switch(Index) {
        case 0:
            if(FTitleMarginX != n) {
                FTitleMarginX = n;
                Changed();
            }
            break;

        case 1:
            if(FTitleMarginY != n) {
                FTitleMarginY = n;
                Changed();
                
                int nOrgHeight = Height;
                Height = nOrgHeight - 1;
                Height = nOrgHeight;
            }
            break;

        case 2:
            if(FTitleWidth != n) {
                FTitleWidth = n;
                Changed();
            }
            break;

        case 3:
            if(FTitleHeight != n) {
                FTitleHeight = n;
                Changed();
            }
            break;

        case 4:
            if(FTextMarginX != n) {
                FTextMarginX = n;
                Changed();
            }
            break;

        case 5:
            if(FTitleRound != n) {
                FTitleRound = n;
                Changed();
            }
            break;

    }
}

void __fastcall TAZGroupBox::SetColorValue(int Index, TColor c) {
    switch(Index) {
        case 0:
            if(FTitleBGColor != c) {
                FTitleBGColor = c;
                Changed();
            }
            break;

        case 1:
            if(FTitleRectColor != c) {
                FTitleRectColor = c;
                Changed();
            }
            break;

        case 2:
            if(FFrameColor != c) {
                FFrameColor = c;
                Changed();
            }
            break;

        case 3:
            if(FGradientStartColor != c) {
                FGradientStartColor = c;
                m_bRefreshChildControl = true;
                Changed();
            }
            break;

        case 4:
            if(FGradientEndColor != c) {
                FGradientEndColor = c;
                m_bRefreshChildControl = true;
                Changed();
            }
            break;

        case 5:
            if(FTransparentColor != c) {
                FTransparentColor = c;
                m_bRefreshChildControl = true;
                Changed();
            }            
            
            break;
    }
}

void __fastcall TAZGroupBox::SetFrameValue(int Index, int n)
{
    switch(Index) {
        case 0:
            if(FFrameRound != n) {
                FFrameRound = n;
                Changed();
            }
            break;
    }
}

void __fastcall TAZGroupBox::SetFrameLineBold(bool b)
{
    if(FFrameLineBold != b) {
        FFrameLineBold = b;
        Changed();
    }
}

void  __fastcall TAZGroupBox::SetCaption(String s)
{
    if(FCaption != s) {
        FCaption = s;
        Changed();
    }
}

void  __fastcall TAZGroupBox::SetShadowFont(bool b)
{
    if(FShadowFont != b) {
        FShadowFont = b;
        Changed();
    }
}

void  __fastcall TAZGroupBox::SetShadowFontColor(TColor c)
{
    if(FShadowFontColor != c) {
        FShadowFontColor = c;
        Changed();
    }
}

void  __fastcall TAZGroupBox::SetShadowFontLeft(int n)
{
    if(FShadowFontLeft != n) {
        FShadowFontLeft = n;
        Changed();
    }
}

void  __fastcall TAZGroupBox::SetShadowFontTop(int n)
{
    if(FShadowFontTop != n) {
        FShadowFontTop = n;
        Changed();
    }
}

void __fastcall TAZGroupBox::SetTransparent(bool b)
{
    if(FTransparent != b) {
        FTransparent = b;
        m_bRefreshChildControl = true;
        Changed();
    }
}

