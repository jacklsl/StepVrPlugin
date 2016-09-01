#pragma once
#include "StepVrPluginPrivatePCH.h"
//#include "NameTypes.h"


enum class EStepVrDeviceId
{
	DLeft,
	Dright,
	DGun,


	DTotalCount
};

//regiest key
struct FStepVRCapacitiveKey
{
	static const FKey StepVR_GunBtn_A_Trigger;
	static const FKey StepVR_GunBtn_B_Trigger;
	static const FKey StepVR_GunBtn_C_Trigger;

	static const FKey StepVR_LeftBtn_A_Trigger;
	static const FKey StepVR_LeftBtn_B_Trigger;
	static const FKey StepVR_LeftBtn_C_Trigger;

	static const FKey StepVR_RightBtn_A_Trigger;
	static const FKey StepVR_RightBtn_B_Trigger;
	static const FKey StepVR_RightBtn_C_Trigger;
};

const FKey FStepVRCapacitiveKey::StepVR_GunBtn_A_Trigger("StepVR_GunBtnA_Press");
const FKey FStepVRCapacitiveKey::StepVR_GunBtn_B_Trigger("StepVR_GunBtnB_Press");
const FKey FStepVRCapacitiveKey::StepVR_GunBtn_C_Trigger("StepVR_GunBtnC_Press");
const FKey FStepVRCapacitiveKey::StepVR_LeftBtn_A_Trigger("StepVR_LeftA_Press");
const FKey FStepVRCapacitiveKey::StepVR_LeftBtn_B_Trigger("StepVR_LeftB_Press");
const FKey FStepVRCapacitiveKey::StepVR_LeftBtn_C_Trigger("StepVR_LeftC_Press");
const FKey FStepVRCapacitiveKey::StepVR_RightBtn_A_Trigger("StepVR_RightA_Press");
const FKey FStepVRCapacitiveKey::StepVR_RightBtn_B_Trigger("StepVR_RightB_Press");
const FKey FStepVRCapacitiveKey::StepVR_RightBtn_C_Trigger("StepVR_RightC_Press");

const uint8 SButton_Release = (int32)FMath::Pow(2, 0);
const uint8 SButton_Press = (int32)FMath::Pow(2, 1);
const uint8 SButton_Repeat = (int32)FMath::Pow(2, 2);
struct FStepVrButtonState
{
	FName key;

	uint8  PressedState;

	double NextRepeatTime;

	FStepVrButtonState()
		:key(NAME_None),
		PressedState(SButton_Release),
		NextRepeatTime(0.0)
	{

	}
};

struct FStepVrDeviceState
{
	StepVR::SingleNode::NodeID EquipId;

	TArray<FStepVrButtonState> TBtnKey;
};

struct FStepVrStateController
{
	FStepVrDeviceState Devices[(int32)EStepVrDeviceId::DTotalCount];

	FStepVrStateController()
	{

		//≥ı ºªØ
		FStepVrButtonState btn;

		Devices[(int32)EStepVrDeviceId::DLeft].EquipId = StepVR::SingleNode::NodeID_LeftWrist/*NodeID_Neck*/;
		btn.key = FStepVRCapacitiveKey::StepVR_LeftBtn_A_Trigger.GetFName();
		Devices[(int32)EStepVrDeviceId::DLeft].TBtnKey.Add(btn);
		btn.key = FStepVRCapacitiveKey::StepVR_LeftBtn_B_Trigger.GetFName();
		Devices[(int32)EStepVrDeviceId::DLeft].TBtnKey.Add(btn);
		btn.key = FStepVRCapacitiveKey::StepVR_LeftBtn_C_Trigger.GetFName();
		Devices[(int32)EStepVrDeviceId::DLeft].TBtnKey.Add(btn);

		Devices[(int32)EStepVrDeviceId::Dright].EquipId = StepVR::SingleNode::NodeID_RightWrist;
		btn.key = FStepVRCapacitiveKey::StepVR_RightBtn_A_Trigger.GetFName();
		Devices[(int32)EStepVrDeviceId::Dright].TBtnKey.Add(btn);
		btn.key = FStepVRCapacitiveKey::StepVR_RightBtn_B_Trigger.GetFName();
		Devices[(int32)EStepVrDeviceId::Dright].TBtnKey.Add(btn);
		btn.key = FStepVRCapacitiveKey::StepVR_RightBtn_C_Trigger.GetFName();
		Devices[(int32)EStepVrDeviceId::Dright].TBtnKey.Add(btn);

		Devices[(int32)EStepVrDeviceId::DGun].EquipId = StepVR::SingleNode::NodeID_RightAnkle;
		btn.key = FStepVRCapacitiveKey::StepVR_GunBtn_A_Trigger.GetFName();
		Devices[(int32)EStepVrDeviceId::DGun].TBtnKey.Add(btn);
		btn.key = FStepVRCapacitiveKey::StepVR_GunBtn_B_Trigger.GetFName();
		Devices[(int32)EStepVrDeviceId::DGun].TBtnKey.Add(btn);
		btn.key = FStepVRCapacitiveKey::StepVR_GunBtn_C_Trigger.GetFName();
		Devices[(int32)EStepVrDeviceId::DGun].TBtnKey.Add(btn);
	}
};