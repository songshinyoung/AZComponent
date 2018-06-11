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
// 컴포넌트 등록.


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

        // Enabled 속성에 TBooleanProperty 속성 타입을 적용 시킴. (Object Inspector에서 bool속성 편집 시 체크 박스 표시되도록)
        PPropInfo PropInfo1 = GetPropInfo(__typeinfo(TAZNumShape), "Enabled");
        RegisterPropertyEditor(*(PropInfo1->PropType), NULL, "", __classid(TBooleanProperty));

        // AZ_Path 속성에 TStringProperty 속성 타입을 적용 시킴.
        PPropInfo PropInfo2 = GetPropInfo(__typeinfo(TAZNumShape), "AZ_Path");
        RegisterPropertyEditor(*(PropInfo2->PropType), NULL, "AZ_Path", __classid(TStringProperty));

        // 컴포넌트 등록.
        int nArraySize = ARRAYSIZE(classes);
         RegisterComponents( L"AZ",           // 팔레트 이름
                            classes,         // 컴포넌트 class id 배열
                            nArraySize - 1); // 컴포넌트 class id 배열 크기에서 1 뺀 수. (ex : 4개 등록할 경우 3을 입력함)

        // 직접 제작한 컴포넌트 에디터 등록.
        //RegisterComponentEditor(__classid(TAZPanel),    __classid(TAZPanelEditor));
        RegisterComponentEditor(__classid(TAZNumShape), __classid(TAZNumShapeEditor));
    }
}

//---------------------------------------------------------------------------

