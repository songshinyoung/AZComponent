//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "AZPanel.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAZPanel *)
{
    new TAZPanel(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAZPanel::TAZPanel(TComponent* Owner)
    : TPanel(Owner)
{
    TPanel::OnResize = MyResize;

    nCreatePanelCount = 0;

    FColCount = 3;
    FRowCount = 3;

    TPanel::Width  = 200;
    TPanel::Height = 150;
    //TPanel::Color = clSilver;

    FCaptions = new TStringList;

    CreatePanel(FRowCount, FColCount);
}
//---------------------------------------------------------------------------
// dfm 으로 부터 속성값을 모두 로드한 후 이 함수가 호출 됨.
void __fastcall TAZPanel::Loaded(void)
{
    TPanel::Loaded();

    //CreatePanel(FRowCount, FColCount);
}

__fastcall TAZPanel::~TAZPanel()
{
    DeletePanel();

    if(FCaptions) delete FCaptions;
}
//---------------------------------------------------------------------------
// 메시지 트래핑.
void __fastcall TAZPanel::WndProc(TMessage& Message)
{
    switch(Message.Msg) {
         case WM_SIZE:
            break;

         case WM_APP:
             break;
    }

    TWinControl::WndProc(Message);
}
//---------------------------------------------------------------------------
// 컴포넌트 등록.
//namespace Azpanel
//{
//    void __fastcall PACKAGE Register()
//    {
//        TComponentClass classes[1] = {__classid(TAZPanel)};
//        RegisterComponents(L"Samples", classes, 0);
//    }
//}
//---------------------------------------------------------------------------

void __fastcall TAZPanel::Notification(
    Classes::TComponent *AComponent,
    Classes::TOperation Operation)
{
    // Sub 컴포넌트의 FreeNotification()을 설정하 경우.
    // 해당 컴포넌트가 삭제될 때 발생되는 이밴트를 오버라이드한 것이다.
    // 여기서 해당 컴포넌트가 삭제된 경우의 처리를 하면 된다.

    TComponent::Notification(AComponent, Operation);
    // { call inherited method }
    if ((Operation == opRemove) && (AComponent == (TComponent *)FppPanel)) {
        //FppPanel = NULL;
    }
}

//---------------------------------------------------------------------------

int        __fastcall TAZPanel::GetColCount()
{
    return FColCount;
}
void      __fastcall TAZPanel::SetColCount(int n)
{
    if(FColCount != n) {

        FColCount = n <= 0 ? 1 : n;
        CreatePanel(FRowCount, FColCount);
    }
}

int        __fastcall TAZPanel::GetRowCount()
{
    return FRowCount;
}

void      __fastcall TAZPanel::SetRowCount(int n)
{
    if(FRowCount != n) {
        FRowCount = n <= 0 ? 1 : n;
        CreatePanel(FRowCount, FColCount);
    }
}

void    __fastcall TAZPanel::SetWidth(int value)
{
    if(TPanel::Width != value) {
        TPanel::Width = value;
        ResizePanel(FRowCount, FColCount);
    }
}

int     __fastcall TAZPanel::GetWidth()
{
    return TPanel::Width;
}

void    __fastcall TAZPanel::SetHeight(int value)
{
    if(TPanel::Height != value) {
        TPanel::Height = value;
        ResizePanel(FRowCount, FColCount);
    }
}

int     __fastcall TAZPanel::GetHeight()
{
    return TPanel::Height;
}

//------------------------------------------------------------------------------

void __fastcall TAZPanel::CreatePanel(int nR, int nC)
{
    // 이전에 생성된것 모두 삭제.
    DeletePanel();

    // 새로 생성.
    if(nR <= 0 || nC <= 0) return;

    FppPanel = new TPanel* [nR * nC];

    nCreatePanelCount = 0;

    int nIndex = 0;
    for(int i=0; i<nR; i++) {
        for(int j=0; j<nC; j++) {
            FppPanel[nIndex] = new TPanel(this);

            //FppPanel[nIndex]->SetSubComponent(true);  // 속성 컴포넌트임을 설정 함.
            //FppPanel[nIndex]->FreeNotification(this);    // 소멸 될 때 이밴트 발생하도록 함.

            FppPanel[nIndex]->Parent  = this;
            FppPanel[nIndex]->ParentBackground = false;
            FppPanel[nIndex]->BevelOuter = bvNone;
            FppPanel[nIndex]->BevelKind  = bkFlat;
            FppPanel[nIndex]->Color      = clSilver;

            FppPanel[nIndex]->OnClick    = SubPanelClick;

            nIndex += 1;
            nCreatePanelCount += 1;
        }
    }

    ResizePanel(nR, nC);
}

void __fastcall TAZPanel::DeletePanel()
{
    // 이전에 생성된것 모두 삭제.
    int nCount = nCreatePanelCount;
    nCreatePanelCount = 0;

    for(int i=0; i<nCount; i++) {
        if(FppPanel[i])delete FppPanel[i];
        FppPanel[i] = NULL;
    }

    if(FppPanel) delete[] FppPanel;

    FppPanel = NULL;
}

//---------------------------------------------------------------------------
void __fastcall TAZPanel::ResizePanel(int nR, int nC)
{
    if(nR <=0 || nC <= 0) return;

    int nW = (TPanel::Width  - 4  - ((nC-1)*2)) / nC;
    int nH = (TPanel::Height - 4  - ((nR-1)*2)) / nR;

    if(nW <= 0 || nH <= 0) return ;

    int nIndex = 0;
    for(int i=0; i<nR; i++) {
        for(int j=0; j<nC; j++) {
            if(nIndex < nCreatePanelCount) {
                if(FppPanel && FppPanel[nIndex]) {
                    FppPanel[nIndex]->Width   = nW;
                    FppPanel[nIndex]->Height  = nH;
                    FppPanel[nIndex]->Top     = 2 + ((nH+2) * i);
                    FppPanel[nIndex]->Left    = 2 + ((nW+2) * j);
                    FppPanel[nIndex]->Visible = true;
                }
            }
            nIndex += 1;
        }
    }
}

//---------------------------------------------------------------------------
TPanel* __fastcall TAZPanel::GetSubPanel(int Index_R, int Index_C)
{
    TPanel * pPanel;

    if(Index_R < 0 || Index_R >= FRowCount) return NULL;
    if(Index_C < 0 || Index_C >= FColCount) return NULL;

    int nIdx = Index_R * FColCount + Index_C;
    pPanel = FppPanel[nIdx];
    return pPanel;
}

//---------------------------------------------------------------------------
//bool __fastcall TAZPanel::CanResize(int &NewWidth, int &NewHeight)
//{
//    bool bRet = TPanel::CanResize(NewWidth, NewHeight);
//    ResizePanel(FRowCount, FColCount);
//    return bRet;
//}

//void __fastcall TAZPanel::Resize(TObject *Sender)
//{
//    if(TPanel::OnResize) TPanel::OnResize(Sender);
//
//    ResizePanel(FRowCount, FColCount);
//
//    TPanel::Caption = TPanel::Width;
//}

void     __fastcall TAZPanel::MyResize(TObject *Sender)
{
    if(FOnResize) FOnResize(Sender);

    ResizePanel(FRowCount, FColCount);
}
//---------------------------------------------------------------------------

void __fastcall TAZPanel::SubPanelClick(TObject *Sender)
{
    TPanel * pPan = dynamic_cast<TPanel *>(Sender);
    if(pPan == NULL) return;

    pPan->Tag += 1;
    pPan->Caption = pPan->Tag;
}
//---------------------------------------------------------------------------

void    __fastcall TAZPanel::SetCaptions(System::Classes::TStrings* p)
{
    if(FCaptions == NULL) FCaptions = new TStringList;
    FCaptions->Clear() ;
    FCaptions->Assign(p);

    for(int i=0; i<FCaptions->Count; i++) {
        if(i < nCreatePanelCount) {
            if(FppPanel[i]) FppPanel[i]->Caption = FCaptions->Strings[i];
        }
        else {
            break;
        }
    }
}

//---------------------------------------------------------------------------

