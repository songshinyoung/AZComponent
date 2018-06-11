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
  TAZPanel* mylabel = (TAZPanel*)Component;      //mylabel 은 선택된 컴포넌트

  TFontDialog* FontDlg = new TFontDialog(NULL); //폰트 대화상자 생성
  FontDlg->Font = mylabel->Font;                  //폰트 대화상자에 선택된 컴포넌트의 폰트를 알려준다.
  FontDlg->Execute();                             //폰트 대화상자 실행
  mylabel->Font = FontDlg->Font;                 //대화상자에서 선택한 폰트를 선택된 컴포넌트에 적용한다.
  Designer->Modified();                          //선택된 컴포넌트가 변했음을 알려준다.
  delete FontDlg;                                 //폰트 대화상자 파괴
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
  return 2;  //선택할 수 있는 Context Menu 의 개수는 2개
}

System::UnicodeString __fastcall TAZPanelEditor::GetVerb(int Index)
{
  AnsiString S;
  switch (Index)
  {
    case 0 : S = "Change Font..."; break;  //첫번째 메뉴
    case 1 : S = "Change Color..."; break; //두번째 메뉴
  }
  return S;
}

void __fastcall TAZPanelEditor::ExecuteVerb(int Index)
{
  switch (Index)
  {
    case 0 : ChangeFont(); break;  //첫번째 메뉴 선택하면 ChangeFont 실행
    case 1 : ChangeColor(); break; //두번째 메뉴 선택하면 ChangeColor 실행
  }
}
