//---------------------------------------------------------------------------

#ifndef AZNumShapeComponentEditorH
#define AZNumShapeComponentEditorH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

//---------------------------------------------------------------------------
//#include <propsys.hpp>          //Component Editor ����� ���� ����
#include <DesignIntf.hpp>       //Component Editor ����� ���� ����
#include <DesignEditors.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>      //Component Editor ����� ���� ����

//---------------------------------------------------------------------------

//-- type declarations -------------------------------------------------------
#if 1 ///////////////////////////////////////////////////////////////////////
// TComponentEditor �� ����ϱ� ���ؼ��� Requires �� designide.bpi�� ���Խ��Ѿ��Ѵ�.
class  PACKAGE TAZNumShapeEditor : public TComponentEditor
{
private:
    void __fastcall ShowEditor();

public:
    int                     __fastcall GetVerbCount(void);
    System::UnicodeString   __fastcall GetVerb(int Index);
    void                    __fastcall ExecuteVerb(int Index);

};

#else ///////////////////////////////////////////////////////////////////////

//class DELPHICLASS TAZNumShapeEditor;
#pragma pack(push,4)
class PACKAGE /*PASCALIMPLEMENTATION*/ TAZNumShapeEditor : public TComponentEditor //Designeditors::TComponentEditor
{
    typedef /*Designeditors::*/TComponentEditor inherited;

private:
    void __fastcall ShowEditor();

public:
    virtual System::UnicodeString __fastcall GetVerb(int Index);
    virtual int __fastcall GetVerbCount(void);
    virtual void __fastcall ExecuteVerb(int Index);
public:
    /* TComponentEditor.Create */ inline __fastcall virtual TAZNumShapeEditor(System::Classes::TComponent* AComponent, Designintf::_di_IDesigner ADesigner) : Designeditors::TComponentEditor(AComponent, ADesigner) { }

public:
    /* TObject.Destroy */ inline __fastcall virtual ~TAZNumShapeEditor(void) { }

};

#pragma pack(pop)

#endif ///////////////////////////////////////////////////////////////////////


#endif
