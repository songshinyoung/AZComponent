# AZComponent
C++ Builder VCL Component Project. <br>
It is a component specialized for touch monitors when developing GUI programs for semiconductor production equipment.<br>
It was compiled in C++ Builder XE4 32-bit. <a href="https://www.embarcadero.com/" target="_blank"><img alt="C++ Builder" src="https://img.shields.io/badge/-C++ Builder-45b8d8?style=flat-square&logo=cplusplusbuilder&logoColor=white" /> </a>

## Build Step.
1. Run "Open Project" in C++ Builder XE4. Select **AZPanelProjectGroup.groupproj** in the **XE4** folder to open the project group.
2. In the Project Manager, select **TAZPanel.bpl**, right-click it, and then run **Build**.
3. Using the same method, select **TAZPanel_Dsgn.bpl**, right-click it, and run **Build**.
4. Select **TAZPanel_Dsgn.bpl**, right-click it, and then run **Install**.

    ![image](https://github.com/user-attachments/assets/f181d040-7b4f-400a-b217-39b38dd3c8d7)

5. Once the Build and Install are successfully completed, close the project and create a new VCL project.
6. You can check the **Tool Palette** to confirm that the **AZ** components have been added.

    ![image](https://github.com/user-attachments/assets/fbbbdc96-0db9-4971-ad95-f1ae697c2d9c)

<br>

## IDE Path Setting
Tools / Options / Environment Options / C++ Options / Paths and Directories
 - System Include Path
   
   ![image](https://github.com/user-attachments/assets/04c28c0a-a09d-48b8-a1e2-c46d649c8e09)

 - Library path

   ![image](https://github.com/user-attachments/assets/a4dd6100-3eab-44c1-aa80-0a47ed3e7400)

<br>

## Introduction to Several AZComponents
### ✔ TAZNumShape
![image](https://github.com/user-attachments/assets/6f1b57d0-37c9-48c6-a8df-c344a5c08ba1)
```C++
void __fastcall TForm1::AZNumShape1Click(TObject *Sender, bool Ok)
{
	if(Ok) {
		Caption = AZNumShape1->AZ_Value;
	}
}
```

### ✔ TAZFlowShape
![image](https://github.com/user-attachments/assets/28f120f5-ac61-42f8-a0b0-9c474cd4aa93)
```C++
void __fastcall TForm1::FormShow(TObject *Sender)
{
	AZFlowShape1->Cells[0][0]->BGColor = clLime;
	AZFlowShape1->Cells[0][1]->BGColor = clYellow;
	AZFlowShape1->Cells[1][1]->BGColor = clLime;
	AZFlowShape1->Cells[2][2]->BGColor = clLime;

	for(int y=0; y<AZFlowShape1->AZ_Row; y++) {
		for(int x=0; x<AZFlowShape1->AZ_Col; x++) {
			AZFlowShape1->Cells[y][x]->Caption = IntToStr(y) + IntToStr(x) ;
		}
	}

}
```


### ✔ TAZGroupBox
![image](https://github.com/user-attachments/assets/6d08ea31-20cb-4de6-a950-6a9c4446080e)

### ✔ TAZActuatorButton
![image](https://github.com/user-attachments/assets/dd09b35b-8c2d-4471-bbef-2afaecbce305)
```C++
void __fastcall TForm1::ActuatorButton1Click(TObject *Sender, TDualButtonState eState)
{
	switch(eState) {
		case dbLeft:  Caption = "Left Click";	break;
		case dbRight: Caption = "Right Click";	break;
	}
}
```

### ✔ AZArrowButton
![image](https://github.com/user-attachments/assets/90a91df0-8437-4dd0-82df-415d9c66492e)
```C++
void __fastcall TForm1::AZArrowButton1Click(TObject *Sender, TArrowButtonState eState)
{
	switch(eState) {
		case abLeft: 	Caption = "Left";   break;
		case abRight: 	Caption = "Right";  break;
		case abTop: 	Caption = "Top";    break;
		case abBottom: 	Caption = "Bottom"; break;
	}
}
```
