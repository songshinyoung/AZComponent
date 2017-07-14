//---------------------------------------------------------------------------
#include <memory>          //For use of auto_ptr
#pragma hdrstop

#include "AZPanel.h"
#include "AZPanel_Dsgn.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TAZPanelEditor::TAZPanelEditor(TComponent* AComponent, _di_IDesigner ADesigner)
                      : TComponentEditor(AComponent, ADesigner) { }

__fastcall TAZPanelEditor::~TAZPanelEditor(void) { }

//---------------------------------------------------------------------------
void __fastcall TAZPanelEditor::EditProperty(const _di_IProperty Prop, bool& Continue)
 {
  String PropName(Prop->GetName());
  if (PropName.CompareIC("ANGLES") == 0){
    Prop->Edit();
    Continue = false;
  }
}


//---------------------------------------------------------------------------
void __fastcall TAZPanelEditor::ChangeFont()
{
  TAZPanel* mylabel = (TAZPanel*)Component;      //mylabel �� ���õ� ������Ʈ

  TFontDialog* FontDlg = new TFontDialog(NULL); //��Ʈ ��ȭ���� ����
  FontDlg->Font = mylabel->Font;                  //��Ʈ ��ȭ���ڿ� ���õ� ������Ʈ�� ��Ʈ�� �˷��ش�.
  FontDlg->Execute();                             //��Ʈ ��ȭ���� ����
  mylabel->Font = FontDlg->Font;                 //��ȭ���ڿ��� ������ ��Ʈ�� ���õ� ������Ʈ�� �����Ѵ�.
  Designer->Modified();                          //���õ� ������Ʈ�� �������� �˷��ش�.
  delete FontDlg;                                 //��Ʈ ��ȭ���� �ı�
}

void __fastcall TAZPanelEditor::ChangeColor()
{
  TAZPanel* mylabel = (TAZPanel*)Component;

  TColorDialog* ColorDlg = new TColorDialog(NULL);
  ColorDlg->Color = mylabel->Color;
  ColorDlg->Execute();
  mylabel->Color = ColorDlg->Color;
  Designer->Modified();
  delete ColorDlg;
}

int __fastcall TAZPanelEditor::GetVerbCount()
{
  return 2;  //������ �� �ִ� Context Menu �� ������ 2��
}

System::UnicodeString __fastcall TAZPanelEditor::GetVerb(int Index)
{
  AnsiString S;
  switch (Index)
  {
    case 0 : S = "Change Font..."; break;  //ù��° �޴�
    case 1 : S = "Change Color..."; break; //�ι�° �޴�
  }
  return S;
}

void __fastcall TAZPanelEditor::ExecuteVerb(int Index)
{
  switch (Index)
  {
    case 0 : ChangeFont(); break;  //ù��° �޴� �����ϸ� ChangeFont ����
    case 1 : ChangeColor(); break; //�ι�° �޴� �����ϸ� ChangeColor ����
  }
}
