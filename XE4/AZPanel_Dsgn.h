//---------------------------------------------------------------------------

#ifndef AZPanel_DsgnH
#define AZPanel_DsgnH
//---------------------------------------------------------------------------
#include <propsys.hpp>          //Component Editor 사용을 위해 포함
#include <DesignIntf.hpp>       //Component Editor 사용을 위해 포함
#include <DesignEditors.hpp>    //Component Editor 사용을 위해 포함

//---------------------------------------------------------------------------

//-- type declarations -------------------------------------------------------

// TComponentEditor 를 사용하기 위해서는 Requires 에 designide.bpi를 포함시켜야한다.
class  PACKAGE TAZPanelEditor : public TComponentEditor
{
protected:
    // TODO: 이건 뭐지..???
    virtual void __fastcall EditProperty(const  _di_IProperty Prop, bool& Continue);

public:
    // 인덱스에 해당되는 스트링을 넘겨줄 함수 오바라이딩
    virtual System::UnicodeString    __fastcall GetVerb(int Index);

    // 오른쪽 마우스 팝업 메뉴에 등록할 아이탬 수를 리턴할 함수 오버라이딩.
    virtual int                      __fastcall GetVerbCount(void);

    // 인덱스에 해당되는 Editor 기능을 수행할 함수 오버라이딩.
    virtual void                     __fastcall ExecuteVerb(int Index);

    void                             __fastcall ChangeFont();
    void                             __fastcall ChangeColor();

public:
    // 생성자.
    __fastcall virtual TAZPanelEditor(TComponent* AComponent, _di_IDesigner ADesigner);

public:
    // 소멸자.
    __fastcall virtual ~TAZPanelEditor(void);
};

#endif

