//---------------------------------------------------------------------------

#ifndef AZPanel_DsgnH
#define AZPanel_DsgnH
//---------------------------------------------------------------------------
#include <propsys.hpp>          //Component Editor ����� ���� ����
#include <DesignIntf.hpp>       //Component Editor ����� ���� ����
#include <DesignEditors.hpp>    //Component Editor ����� ���� ����

//---------------------------------------------------------------------------

//-- type declarations -------------------------------------------------------

// TComponentEditor �� ����ϱ� ���ؼ��� Requires �� designide.bpi�� ���Խ��Ѿ��Ѵ�.
class  PACKAGE TAZPanelEditor : public TComponentEditor
{
protected:
    // TODO: �̰� ����..???
    virtual void __fastcall EditProperty(const  _di_IProperty Prop, bool& Continue);

public:
    // �ε����� �ش�Ǵ� ��Ʈ���� �Ѱ��� �Լ� ���ٶ��̵�
    virtual System::UnicodeString    __fastcall GetVerb(int Index);

    // ������ ���콺 �˾� �޴��� ����� ������ ���� ������ �Լ� �������̵�.
    virtual int                      __fastcall GetVerbCount(void);

    // �ε����� �ش�Ǵ� Editor ����� ������ �Լ� �������̵�.
    virtual void                     __fastcall ExecuteVerb(int Index);

    void                             __fastcall ChangeFont();
    void                             __fastcall ChangeColor();

public:
    // ������.
    __fastcall virtual TAZPanelEditor(TComponent* AComponent, _di_IDesigner ADesigner);

public:
    // �Ҹ���.
    __fastcall virtual ~TAZPanelEditor(void);
};

#endif

