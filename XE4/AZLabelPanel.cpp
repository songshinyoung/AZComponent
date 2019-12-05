//---------------------------------------------------------------------------

#include <vcl.h>
#include <pngimage.hpp>

#pragma hdrstop

#include "AZLabelPanel.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAZLabelPanel *)
{
    new TAZLabelPanel(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAZLabelPanel::TAZLabelPanel(TComponent* Owner)
    : TCustomPanel(Owner)
{
//    m_pTitleImgCenter = new TImage(this);
//
//    TPngImage * pPNG = new TPngImage();
//    pPNG->LoadFromResourceName((int)HInstance, L"ofBtn_0_On_Down");
//
//    m_pTitleImgCenter->Parent = this;
//    m_pTitleImgCenter->AutoSize     = true;
//    m_pTitleImgCenter->Align        = alTop;
//    m_pTitleImgCenter->Height       = 25;
//    m_pTitleImgCenter->Transparent  = true;
//    m_pTitleImgCenter->Picture->Graphic = pPNG;
//
//    delete pPNG;

}
//---------------------------------------------------------------------------

__fastcall TAZLabelPanel::~TAZLabelPanel(void)
{
//    if(m_pTitleImgCenter) delete m_pTitleImgCenter;
}
//---------------------------------------------------------------------------
