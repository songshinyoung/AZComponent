//---------------------------------------------------------------------------

#ifndef AZLabelImageComponentEditorH
#define AZLabelImageComponentEditorH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <propsys.hpp>          //Component Editor 사용을 위해 포함
#include <DesignIntf.hpp>       //Component Editor 사용을 위해 포함
#include <DesignEditors.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>      //Component Editor 사용을 위해 포함

//---------------------------------------------------------------------------

//-- type declarations -------------------------------------------------------

// TComponentEditor 를 사용하기 위해서는 Requires 에 designide.bpi를 포함시켜야한다.
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
