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
  return 1;  //������ �� �ִ� Context Menu �� ������ 2��
}

System::UnicodeString __fastcall TAZLabelImageEditor::GetVerb(int Index)
{
    AnsiString S;
    switch (Index)
    {
        case 0 : S = "Edit Component"; break;  //ù��° �޴�
    }
    return S;
}

void __fastcall TAZLabelImageEditor::ExecuteVerb(int Index)
{
    ShowEditor();

//    switch (Index)
//    {
//        case 0 : ShowEditor(); break;  //ù��° �޴� �����ϸ� ShowEditor ����
//    }
}

void __fastcall TAZLabelImageEditor::ShowEditor()
{
    // ���õ� Component �����͸� �����´�.
    TAZLabelImage * pLabelImage = (TAZLabelImage *)Component;

    if(pLabelImage == NULL) return;

    // Component Editor�� �����Ѵ�.

    Vcl::Graphics::TBitmap * pBitmap = new Vcl::Graphics::TBitmap;
    pBitmap->Assign(pLabelImage->Picture->Graphic);

    TfmAZLabelImageEditor * fmEditor = new TfmAZLabelImageEditor(NULL, pBitmap, pLabelImage->Width, pLabelImage->Height);

    try
    {
        // ���� ������ �°� Editor�� �����Ѵ�.
//        fmEditor->m_LabelSize;
//        fmEditor->m_LabelLastIndex;
//        fmEditor->m_LabelStyle;           // 0 : Round Rec, 1 : Rec, 2 : Circle
//        fmEditor->m_LineStyle;            // 0 : ����, 1 : �� ����, 2 : ���� ���� 3 : ��/���� ����
//
//        fmEditor->m_LabelMagnetic           = pLabelImage->LabelMagnetic     ;        // Object�� 10 �ȼ� ������ �ڼ����� �̵� �Ѵ�.
//        fmEditor->m_LabelStyle              = pLabelImage->LabelStyle     ;        // Object�� 10 �ȼ� ������ �ڼ����� �̵� �Ѵ�.
//        fmEditor->m_LineStyle               = pLabelImage->LineStyle     ;        // Object�� 10 �ȼ� ������ �ڼ����� �̵� �Ѵ�.
//        fmEditor->m_LabelSize               = pLabelImage->LabelSize     ;        // Object�� 10 �ȼ� ������ �ڼ����� �̵� �Ѵ�.
//        fmEditor->m_LabelLastIndex          = pLabelImage->LabelLastIndex     ;        // Object�� 10 �ȼ� ������ �ڼ����� �̵� �Ѵ�.

//        fmEditor->m_listLine;
//        fmEditor->m_listLabel;

//        fmEditor->Image1->Picture->Graphic->Assign(pLabelImage->Picture->Graphic);
//        fmEditor->m_pBitmap->Assign(pLabelImage->Picture->Graphic);

        // Editor Form ����.
        int nRet = fmEditor->ShowModal();

        if(nRet == mrOk) {
            // ����� ������ Component�� �����Ѵ�.

//            pLabelImage->m_listLine  = fmEditor->m_listLine;
//            pLabelImage->m_listLabel = fmEditor->m_listLabel;
//
//            pLabelImage->LabelMagnetic        = fmEditor->m_LabelMagnetic;
//            pLabelImage->LabelStyle           = fmEditor->m_LabelStyle     ;
//            pLabelImage->LineStyle            = fmEditor->m_LineStyle     ;
//            pLabelImage->LabelSize            = fmEditor->m_LabelSize     ;
//            pLabelImage->LabelLastIndex       = fmEditor->m_LabelLastIndex     ;

            pLabelImage->Picture->Assign(fmEditor->Image1->Picture);

            Designer->Modified(); //���õ� ������Ʈ�� ����Ǿ����� �˷��ش�.
        }
    }
    __finally
    {
        // Editor�� �����Ѵ�.
        delete fmEditor;
    }


}
//===========================================================================
