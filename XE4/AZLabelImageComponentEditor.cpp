//---------------------------------------------------------------------------

#pragma hdrstop

#include "AZLabelImageComponentEditor.h"
#include "AZLabelImage_Dsgn.h"
#include "AZLabelImage.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)





//===========================================================================
//__fastcall TAZNumShapeEditor::TAZNumShapeEditor(TComponent* AComponent, _di_IDesigner ADesigner)
//                      : TComponentEditor(AComponent, ADesigner) { }
//
//__fastcall TAZNumShapeEditor::~TAZNumShapeEditor(void)
//{
//
//}
//
////---------------------------------------------------------------------------
//void __fastcall TAZNumShapeEditor::EditProperty(const _di_IProperty Prop, bool& Continue)
//{
//    String PropName(Prop->GetName());
//    if (PropName.CompareIC("ANGLES") == 0){
//        Prop->Edit();
//        Continue = false;
//    }
//}

int __fastcall TAZLabelImageEditor::GetVerbCount()
{
  return 1;  //선택할 수 있는 Context Menu 의 개수는 2개
}

System::UnicodeString __fastcall TAZLabelImageEditor::GetVerb(int Index)
{
    AnsiString S;
    switch (Index)
    {
        case 0 : S = "Edit Component"; break;  //첫번째 메뉴
    }
    return S;
}

void __fastcall TAZLabelImageEditor::ExecuteVerb(int Index)
{
    ShowEditor();

//    switch (Index)
//    {
//        case 0 : ShowEditor(); break;  //첫번째 메뉴 선택하면 ShowEditor 실행
//    }
}

void __fastcall TAZLabelImageEditor::ShowEditor()
{
    // 선택된 Component 포인터를 가져온다.
    TAZLabelImage * pLabelImage = (TAZLabelImage *)Component;

    if(pLabelImage == NULL) return;

    // Component Editor를 생성한다.

    Vcl::Graphics::TBitmap * pBitmap = new Vcl::Graphics::TBitmap;
    pBitmap->Assign(pLabelImage->Picture->Graphic);

    TfmAZLabelImageEditor * fmEditor = new TfmAZLabelImageEditor(NULL, pBitmap, pLabelImage->Width, pLabelImage->Height);

    try
    {
        // 현재 설정에 맞게 Editor를 세팅한다.
//        fmEditor->m_LabelSize;
//        fmEditor->m_LabelLastIndex;
//        fmEditor->m_LabelStyle;           // 0 : Round Rec, 1 : Rec, 2 : Circle
//        fmEditor->m_LineStyle;            // 0 : 직선, 1 : 긴 점선, 2 : 작은 점선 3 : 긴/작은 점선
//
//        fmEditor->m_LabelMagnetic           = pLabelImage->LabelMagnetic     ;        // Object를 10 픽셀 단위로 자석모드로 이동 한다.
//        fmEditor->m_LabelStyle              = pLabelImage->LabelStyle     ;        // Object를 10 픽셀 단위로 자석모드로 이동 한다.
//        fmEditor->m_LineStyle               = pLabelImage->LineStyle     ;        // Object를 10 픽셀 단위로 자석모드로 이동 한다.
//        fmEditor->m_LabelSize               = pLabelImage->LabelSize     ;        // Object를 10 픽셀 단위로 자석모드로 이동 한다.
//        fmEditor->m_LabelLastIndex          = pLabelImage->LabelLastIndex     ;        // Object를 10 픽셀 단위로 자석모드로 이동 한다.

//        fmEditor->m_listLine;
//        fmEditor->m_listLabel;

//        fmEditor->Image1->Picture->Graphic->Assign(pLabelImage->Picture->Graphic);
//        fmEditor->m_pBitmap->Assign(pLabelImage->Picture->Graphic);

        // Editor Form 실행.
        int nRet = fmEditor->ShowModal();

        if(nRet == mrOk) {
            // 변경된 내용을 Component에 적용한다.

//            pLabelImage->m_listLine  = fmEditor->m_listLine;
//            pLabelImage->m_listLabel = fmEditor->m_listLabel;
//
//            pLabelImage->LabelMagnetic        = fmEditor->m_LabelMagnetic;
//            pLabelImage->LabelStyle           = fmEditor->m_LabelStyle     ;
//            pLabelImage->LineStyle            = fmEditor->m_LineStyle     ;
//            pLabelImage->LabelSize            = fmEditor->m_LabelSize     ;
//            pLabelImage->LabelLastIndex       = fmEditor->m_LabelLastIndex     ;

            pLabelImage->Picture->Assign(fmEditor->Image1->Picture);

            Designer->Modified(); //선택된 컴포넌트가 변경되었음을 알려준다.
        }
    }
    __finally
    {
        // Editor를 삭제한다.
        delete fmEditor;
    }


}
//===========================================================================
