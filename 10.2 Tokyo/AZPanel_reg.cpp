//---------------------------------------------------------------------------

#include "AZPanel.h"
//#include "AZPanel_Dsgn.h"
//#include "AZNumPanel.h"
#include "AZNumShape.h"
#include "DualButton.h"
#include "TitleLabel.h"
#include "AZNumShapeComponentEditor.h"
#include "IndexButton.h"
#include "AZFlowShape.h"
#include "AZArrowButton.h"
#include "AZCalendr.h"
#include "AZNumberPad.h"
#include "AZImageButton.h"
#include "AZServoState.h"
#include "AZDateTimeView.h"
#include "AZAnalogGauge.h"
#include "AZGauges.h"
#include "SensorLabel.h"
#include "DualSensorLabel.h"
#include "ActuatorButton.h"

//#include <PropertyCategories.hpp>    // Pascal unit
#include <VCLEditors.hpp>

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ������Ʈ ���.


namespace Azpanel_reg
{
    void __fastcall PACKAGE Register()
    {
        TComponentClass classes[] = {    //__classid(TNumPanel),
                                        //__classid(TAZPanel),
                                        __classid(TAZNumShape),
                                        __classid(TDualButton),
                                        __classid(TIndexButton),
                                        __classid(TTitleLabel),
                                        __classid(TSensorLabel),
                                        __classid(TDualSensorLabel),
                                        __classid(TAZFlowShape),
                                        __classid(TAZArrowButton),
                                        __classid(TAZCalendar),
                                        __classid(TAZNumberPad),
                                        __classid(TAZImageButton),
                                        __classid(TAZServoState),
										__classid(TAZDateTimeView),
										__classid(TAZAnalogGauge),
                                        __classid(TActuatorButton),
										__classid(TAZGauge)
                                        };

        // Enabled �Ӽ��� TBooleanProperty �Ӽ� Ÿ���� ���� ��Ŵ. (Object Inspector���� bool�Ӽ� ���� �� üũ �ڽ� ǥ�õǵ���)
        PPropInfo PropInfo1 = GetPropInfo(__typeinfo(TAZNumShape), "Enabled");
        RegisterPropertyEditor(*(PropInfo1->PropType), NULL, "", __classid(TBooleanProperty));

        // AZ_Path �Ӽ��� TStringProperty �Ӽ� Ÿ���� ���� ��Ŵ.
        PPropInfo PropInfo2 = GetPropInfo(__typeinfo(TAZNumShape), "AZ_Path");
        RegisterPropertyEditor(*(PropInfo2->PropType), NULL, "AZ_Path", __classid(TStringProperty));

        // ������Ʈ ���.
        int nArraySize = ARRAYSIZE(classes);
         RegisterComponents( L"AZ",           // �ȷ�Ʈ �̸�
                            classes,         // ������Ʈ class id �迭
                            nArraySize - 1); // ������Ʈ class id �迭 ũ�⿡�� 1 �� ��. (ex : 4�� ����� ��� 3�� �Է���)

        // ���� ������ ������Ʈ ������ ���.
        //RegisterComponentEditor(__classid(TAZPanel),    __classid(TAZPanelEditor));
        RegisterComponentEditor(__classid(TAZNumShape), __classid(TAZNumShapeEditor));
    }
}

//---------------------------------------------------------------------------

