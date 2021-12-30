//---------------------------------------------------------------------------

#pragma hdrstop

#include "AZNumShapeComponentEditor.h"
#include "AZNumShape_Dsgn.h"
#include "AZNumShape.h"

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

int __fastcall TAZNumShapeEditor::GetVerbCount()
{
  return 1;  //������ �� �ִ� Context Menu �� ������ 2��
}

System::UnicodeString __fastcall TAZNumShapeEditor::GetVerb(int Index)
{
    AnsiString S;
    switch (Index)
    {
        case 0 : S = "Edit Component"; break;  //ù��° �޴�
    }
    return S;
}

void __fastcall TAZNumShapeEditor::ExecuteVerb(int Index)
{
    ShowEditor();

//    switch (Index)
//    {
//        case 0 : ShowEditor(); break;  //ù��° �޴� �����ϸ� ShowEditor ����
//    }
}

void __fastcall TAZNumShapeEditor::ShowEditor()
{
    // ���õ� Component �����͸� �����´�.
    //TAZNumShape * pNumShape = dynamic_cast<TAZNumShape *>(Component);
    TAZNumShape * pNumShape = (TAZNumShape *)Component;

    if(pNumShape == NULL) return;

    // Component Editor�� �����Ѵ�.

    TfmAZNumShapeEditor * fmEditor = new TfmAZNumShapeEditor(NULL);

    try
    {
        // ���� ������ �°� Editor�� �����Ѵ�.

        TColor          FClickBGColor;      ///< �ش� ������Ʈ�� ����ڰ� ��ġ�� ��� ��� ����
        TColor          FClickFontColor;    ///< �ش� ������Ʈ�� ����ڰ� ��ġ�� ��� ��Ʈ ����
        TColor          FClickRectColor;    ///< �ش� ������Ʈ�� ����ڰ� ��ġ�� ��� �ٱ� �簢�� �׵θ� ����.
        TColor          FBGColor;           ///< �ش� ������Ʈ�� �Ϲ� ���¿��� ��� ����.
        TColor          FFontColor;         ///< �ش� ������Ʈ�� �Ϲ� ���¿��� ��Ʈ ����.
        TColor          FRectColor;         ///< �ٱ� �簢�� �׵θ� ����.
        TColor          FOldValueColor;     ///< ���� �� ǥ�� ��Ʈ ����.
        TColor          FChangedFontColor;  ///< ����ڰ� ���� ������ ��� ��Ʈ ����.

        fmEditor->Panel_ColorValueNormal->Color     = pNumShape->ColorSet->FontColor;
        fmEditor->Panel_ColorValueSelect->Color     = pNumShape->ColorSet->ClickFontColor;
        fmEditor->Panel_ColorValueChanged->Color    = pNumShape->ColorSet->ChangedFontColor;

        fmEditor->Panel_ColorBGNormal->Color        = pNumShape->ColorSet->BGColor;
        fmEditor->Panel_ColorBGSelect->Color        = pNumShape->ColorSet->ClickBGColor;

        fmEditor->Panel_ColorOldValueNormal->Color  = pNumShape->ColorSet->OldValueColor;
        fmEditor->Panel_ColorOldValueSelect->Color  = clBtnFace;

        fmEditor->Panel_ColorUnitNormal->Color      = pNumShape->AZ_Unit->Font->Color;
		fmEditor->Panel_ColorUnitSelect->Color      = clBtnFace;

        fmEditor->Panel_ColorLineNormal->Color      = pNumShape->ColorSet->RectColor;
		fmEditor->Panel_ColorLineSelect->Color      = pNumShape->ColorSet->ClickRectColor;

		fmEditor->Panel_ColorValueShadow->Color     = pNumShape->ColorSet->FontShadowColor;

		fmEditor->RadioGroup_Valuetype->ItemIndex   = pNumShape->InputEdit->Type;
		fmEditor->RadioGroup_FixedDigit->ItemIndex  = pNumShape->FixedDigit ? 1 : 0;
		fmEditor->Edit_DecimalPlaces->Text          = IntToStr(pNumShape->InputEdit->DecimalPlaces);
		fmEditor->Edit_UpDecimals->Text          	= IntToStr(pNumShape->InputEdit->UpNumberOfDecimals);
		fmEditor->Edit_DigitGap->Text          		= IntToStr(pNumShape->InputEdit->DigitGap);
		fmEditor->Edit_DigitSize->Text          	= IntToStr(pNumShape->InputEdit->DigitSize);

        fmEditor->Edit_Space->Text                  = IntToStr(pNumShape->AZ_Space);
        fmEditor->Edit_OutlineRound->Text           = IntToStr(pNumShape->AZ_Round);

        fmEditor->CheckBox_UnitVisible->Checked     = pNumShape->AZ_Unit->Visible;
        fmEditor->Edit_UnitWidth->Text              = IntToStr(pNumShape->AZ_Unit->Width);
        fmEditor->Edit_Unit->Text                   = pNumShape->AZ_Unit->Str;

        fmEditor->CheckBox_OldValueVisible->Checked = pNumShape->AZ_OldValueDisplay;

        fmEditor->Edit_MaxValue->Text               = FormatFloat("#0.000", pNumShape->InputEdit->Max);
        fmEditor->Edit_MinValue->Text               = FormatFloat("#0.000", pNumShape->InputEdit->Min);
        fmEditor->Edit_UpDownValue->Text            = FormatFloat("#0.000", pNumShape->InputEdit->UpDownValue);

        fmEditor->CheckBox_ReadOnly->Checked        = pNumShape->ReadOnly;


        //--------------------
        // Title Property ���� 
        fmEditor->Edit_Title_Caption->Text              = pNumShape->Title->Caption;
        fmEditor->Edit_Title_Round->Text                = pNumShape->Title->Round;
        fmEditor->Edit_Title_Space->Text                = pNumShape->Title->Space;
        fmEditor->CheckBox_Title_Transparent->Checked   = pNumShape->Title->Transparent;
        fmEditor->CheckBox_Title_OutLineVisible->Checked= pNumShape->Title->OutLineVisible;
        fmEditor->CheckBox_Title_Visible->Checked       = pNumShape->Title->Visible;
        fmEditor->ComboBox_Title_Align->ItemIndex       = pNumShape->Title->Align;
        fmEditor->ComboBox_Title_Alignment->ItemIndex   = pNumShape->Title->Alignment;
        fmEditor->Panel_Title_Color->Color              = pNumShape->Title->Color;
        fmEditor->Panel_Title_OutLineColor->Color       = pNumShape->Title->OutLineColor;
        fmEditor->Edit_EditWidth->Text                  = pNumShape->EditWidth;
        fmEditor->Edit_EditHeight->Text                 = pNumShape->EditHeight;


        // Editor Form ����.
        int nRet = fmEditor->ShowModal();

        if(nRet == mrOk) {
            // ����� ������ Component�� �����Ѵ�.

            pNumShape->ColorSet->FontColor          = fmEditor->Panel_ColorValueNormal->Color;
            pNumShape->ColorSet->ClickFontColor     = fmEditor->Panel_ColorValueSelect->Color;
            pNumShape->ColorSet->ChangedFontColor   = fmEditor->Panel_ColorValueChanged->Color;
			pNumShape->ColorSet->BGColor            = fmEditor->Panel_ColorBGNormal->Color;
            pNumShape->ColorSet->ClickBGColor       = fmEditor->Panel_ColorBGSelect->Color;
			pNumShape->ColorSet->OldValueColor      = fmEditor->Panel_ColorOldValueNormal->Color;
			pNumShape->ColorSet->RectColor          = fmEditor->Panel_ColorLineNormal->Color;
			pNumShape->ColorSet->ClickRectColor     = fmEditor->Panel_ColorLineSelect->Color;
			pNumShape->ColorSet->FontShadowColor	= fmEditor->Panel_ColorValueShadow->Color;
			pNumShape->AZ_Unit->Font->Color         = fmEditor->Panel_ColorUnitNormal->Color;

			AnsiString sTemp;

			pNumShape->InputEdit->Type          = (TInputNumType)fmEditor->RadioGroup_Valuetype->ItemIndex;
			pNumShape->FixedDigit				= fmEditor->RadioGroup_FixedDigit->ItemIndex == 0 ? false : true;

			sTemp = fmEditor->Edit_DecimalPlaces->Text;
			pNumShape->InputEdit->DecimalPlaces = sTemp.ToIntDef(0);

			sTemp = fmEditor->Edit_UpDecimals->Text;
			pNumShape->InputEdit->UpNumberOfDecimals = sTemp.ToIntDef(0);

			sTemp = fmEditor->Edit_DigitGap->Text;
			pNumShape->InputEdit->DigitGap = sTemp.ToIntDef(0);

			sTemp = fmEditor->Edit_DigitSize->Text;
			pNumShape->InputEdit->DigitSize = sTemp.ToIntDef(0);


            sTemp = fmEditor->Edit_Space->Text;
            pNumShape->AZ_Space                 = sTemp.ToIntDef(0);

            sTemp = fmEditor->Edit_OutlineRound->Text;
            pNumShape->AZ_Round                 = sTemp.ToIntDef(0);

            pNumShape->AZ_Unit->Visible         = fmEditor->CheckBox_UnitVisible->Checked;

            sTemp = fmEditor->Edit_UnitWidth->Text;
            pNumShape->AZ_Unit->Width           = sTemp.ToIntDef(0);

            pNumShape->AZ_Unit->Str             = fmEditor->Edit_Unit->Text;

            pNumShape->AZ_OldValueDisplay       = fmEditor->CheckBox_OldValueVisible->Checked;

            sTemp = fmEditor->Edit_MaxValue->Text;
            pNumShape->InputEdit->Max           = sTemp.ToDouble();

            sTemp = fmEditor->Edit_MinValue->Text;
            pNumShape->InputEdit->Min           = sTemp.ToDouble();

            sTemp = fmEditor->Edit_UpDownValue->Text;
            pNumShape->InputEdit->UpDownValue   = sTemp.ToDouble();

            pNumShape->ReadOnly                 = fmEditor->CheckBox_ReadOnly->Checked;

            //--------------------
            // Title Property ���� 
            pNumShape->Title->Caption       = fmEditor->Edit_Title_Caption->Text;
            sTemp = fmEditor->Edit_Title_Round->Text;
            pNumShape->Title->Round         = sTemp.ToIntDef(0);
            sTemp = fmEditor->Edit_Title_Space->Text;
            pNumShape->Title->Space         = sTemp.ToIntDef(0);
            pNumShape->Title->Transparent   = fmEditor->CheckBox_Title_Transparent->Checked;
            pNumShape->Title->OutLineVisible= fmEditor->CheckBox_Title_OutLineVisible->Checked;
            pNumShape->Title->Visible       = fmEditor->CheckBox_Title_Visible->Checked;
			pNumShape->Title->Align         = (TCaptionAlign)fmEditor->ComboBox_Title_Align->ItemIndex;
			pNumShape->Title->Alignment     = (System::Classes::TAlignment)fmEditor->ComboBox_Title_Alignment->ItemIndex;
            pNumShape->Title->Color         = fmEditor->Panel_Title_Color->Color;
            pNumShape->Title->OutLineColor  = fmEditor->Panel_Title_OutLineColor->Color;
            sTemp = fmEditor->Edit_EditWidth->Text;
			pNumShape->EditWidth            = sTemp.ToIntDef(0);
            sTemp = fmEditor->Edit_EditHeight->Text;
            pNumShape->EditHeight           = sTemp.ToIntDef(0);
            
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

