//---------------------------------------------------------------------------

#ifndef AZLabelImageComponentEditorH
#define AZLabelImageComponentEditorH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <propsys.hpp>          //Component Editor ����� ���� ����
#include <DesignIntf.hpp>       //Component Editor ����� ���� ����
#include <DesignEditors.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>      //Component Editor ����� ���� ����

//---------------------------------------------------------------------------

//-- type declarations -------------------------------------------------------

// TComponentEditor �� ����ϱ� ���ؼ��� Requires �� designide.bpi�� ���Խ��Ѿ��Ѵ�.
class  PACKAGE TAZLabelImageEditor : public TComponentEditor
{
private:
    void __fastcall ShowEditor();

public:
    int                     __fastcall GetVerbCount(void);
    System::UnicodeString   __fastcall GetVerb(int Index);
    void                    __fastcall ExecuteVerb(int Index);

};




#endif
