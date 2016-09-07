// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#include "StepVrPluginPrivatePCH.h"
#include "StepVrInput.h"
#include "UnrealMathUtility.h"
#include "../../../ThirdParty/include/StepVr.h"


#define LOCTEXT_NAMESPACE "StepVR"

StepVR::Manager* StepManager = nullptr;


bool StepManagerIsEnable()
{
	return StepManager ? true : false;
}


bool EquipIsVaild(StepVR::Frame& Frame, StepVR::SingleNode::NodeID EquipId)
{
	StepVR::Vector3f vec3 = Frame.GetSingleNode().GetPosition(EquipId);
	if (vec3.x==0.f&&vec3.y==0.f&&vec3.z==0.f)
	{
		return false;
	}
	return true;
}
FStepVrInput::FStepVrInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
	:MessageHandler(InMessageHandler)
{
	//binding key
	StartModule();

 	

	StepManager = new StepVR::Manager();

	if (!StepManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("stepvrSDK open faild!"));
		return;
	}

	//在对应版本引擎中需要矩阵文件，木有会崩溃
 	bool StepStart = StepManager->Start();
 	if (!StepStart)
 	{
		delete StepManager;
		StepManager = nullptr;

 		UE_LOG(LogTemp, Warning, TEXT("stepvrSDK Start faild!"));
		return;
 	}

	StepVR::StepVR_EnginAdaptor::MapCoordinate(StepVR::Vector3f(0, 0, 1), StepVR::Vector3f(-1, 0, 0), StepVR::Vector3f(0, 1, 0));
	StepVR::StepVR_EnginAdaptor::setEulerOrder(StepVR::EulerOrder_ZYX);

	dBtnRepeatTime = 0.15f;
	IModularFeatures::Get().RegisterModularFeature(GetModularFeatureName(), this);

#if (BEFOPRE_ENGINEVERSION_412)
	GEngine->MotionControllerDevices.AddUnique(this);
#endif
	
	UE_LOG(LogTemp, Warning, TEXT("stepvrSDK Open Success!"));
}

FStepVrInput::~FStepVrInput()
{
	for (auto& dllHandle : DLLHandles)
	{
		FPlatformProcess::FreeDllHandle(dllHandle);
	}

	if (StepManagerIsEnable())
	{
		delete StepManager;
		StepManager = nullptr;
	}

	IModularFeatures::Get().UnregisterModularFeature(GetModularFeatureName(), this);
}

void FStepVrInput::Tick(float DeltaTime)
{

}


void FStepVrInput::SendControllerEvents()
{
	if (!StepManagerIsEnable())
	{
		return;
	}
	const double CurrentTime = FPlatformTime::Seconds();

	StepVR::Frame tmp = StepManager->GetFrame();
	uint8 flag = 0x0;
	FString ButtonName = TEXT("ABCDEF");

	for (int32 i = 0; i < (int32)EStepVrDeviceId::DTotalCount; i++)
	{
		FStepVrDeviceState& device = ButtonState.Devices[i];

		if (!EquipIsVaild(tmp,device.EquipId))
		{
			continue;
		}

		for (int32 j = 0; j < device.TBtnKey.Num(); j++)
		{
			FStepVrButtonState& btnState = device.TBtnKey[j];
			flag = 0x0;

			//按键状态
			flag = (tmp.GetSingleNode().GetKeyUp(device.EquipId, SDKKEYID(j + 1)) ? SButton_Release : 0x0) | flag;
			flag = (tmp.GetSingleNode().GetKeyDown(device.EquipId, SDKKEYID(j + 1)) ? SButton_Press : 0x0) | flag;
			flag = (tmp.GetSingleNode().GetKey(device.EquipId, SDKKEYID(j + 1)) ? SButton_Repeat : 0x0) | flag;

			
			if (flag != btnState.PressedState)
			{
				btnState.PressedState = flag;
				if (btnState.PressedState == SButton_Release)
				{
					UE_LOG(LogTemp, Warning, TEXT("EquipID:%d,Button%c Relese!"),(int32)device.EquipId,ButtonName.GetCharArray()[j]);
					MessageHandler->OnControllerButtonReleased(btnState.key, 0, false);
				}
				if (btnState.PressedState == SButton_Press)
				{
					UE_LOG(LogTemp, Warning, TEXT("EquipID:%d,Button%c Press!"), (int32)device.EquipId, ButtonName.GetCharArray()[j]);
					MessageHandler->OnControllerButtonPressed(btnState.key, 0, false);
					btnState.NextRepeatTime = dBtnRepeatTime + CurrentTime;
				}
			}
			if (btnState.PressedState == SButton_Repeat && btnState.NextRepeatTime <= CurrentTime)
			{
				UE_LOG(LogTemp, Warning, TEXT("EquipID:%d,Button%c Repeat!"), (int32)device.EquipId, ButtonName.GetCharArray()[j]);
				MessageHandler->OnControllerButtonPressed(btnState.key, 0, false);
				btnState.NextRepeatTime = dBtnRepeatTime + CurrentTime;
			}
		}
	}	
}

void FStepVrInput::SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler)
{
	MessageHandler = InMessageHandler;
}

bool FStepVrInput::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	return false;
}

void FStepVrInput::SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value)
{
}

void FStepVrInput::SetChannelValues(int32 ControllerId, const FForceFeedbackValues &values)
{
}

bool FStepVrInput::GetControllerOrientationAndPosition(const int32 ControllerIndex, const EControllerHand DeviceHand, FRotator& OutOrientation, FVector& OutPosition) const
{
	if (!StepManagerIsEnable())
	{
		return false;
	}

	StepVR::Frame tmp = StepManager->GetFrame();
	if (!EquipIsVaild(tmp, ButtonState.Devices[(int32)DeviceHand].EquipId))
	{
		return false;
	}

	//通过设备id获取对应的定位信息
	StepVR::Vector4f vec4 = tmp.GetSingleNode().GetQuaternion(ButtonState.Devices[(int32)DeviceHand].EquipId);

 	StepVR::Vector3f vec3 = StepVR::StepVR_EnginAdaptor::toUserEuler(vec4);
 	OutOrientation.Yaw = vec3.x;
 	OutOrientation.Pitch = -vec3.y;
 	OutOrientation.Roll = -vec3.z;

	//UE_LOG(LogTemp, Warning, TEXT("euqipID:%d,Yaw:%f,Pitch:%f,roll:%f"), (int32)ButtonState.Devices[(int32)DeviceHand].EquipId,OutOrientation.Yaw,OutOrientation.Pitch,OutOrientation.Roll);

	//获取位置
	vec3 = tmp.GetSingleNode().GetPosition(ButtonState.Devices[(int32)DeviceHand].EquipId);
	vec3 = StepVR::StepVR_EnginAdaptor::toUserPosition(vec3);
 	OutPosition.Z = vec3.z * 100;
 	OutPosition.X = vec3.x * 100;
 	OutPosition.Y = vec3.y * 100;

	//UE_LOG(LogTemp, Warning, TEXT("x:%f,y:%f,z:%f"), OutPosition.X, OutPosition.Y, OutPosition.Z);
	return true;
}

// ETrackingStatus FStepVrInput::GetControllerTrackingStatus(const int32 ControllerIndex, const EControllerHand DeviceHand) const
// {
// 	ETrackingStatus TrackingStatus = ETrackingStatus::NotTracked;
// 	return TrackingStatus;
// }

void FStepVrInput::StartModule()
{
	// Concatenate the plugins folder and the DLL file.
	FString filePath = FPaths::Combine(*FPaths::GamePluginsDir(), TEXT("StepVrPlugin"), TEXT("ThirdParty"), TEXT("lib"));

	FPlatformProcess::PushDllDirectory(*filePath);
	void *DLLHandle = FPlatformProcess::GetDllHandle(*(filePath + "/StepVr.dll"));
	if (!DLLHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load StepVr.dll!"));
	}
	else
	{
		DLLHandles.Add(DLLHandle);
	}

	DLLHandle = FPlatformProcess::GetDllHandle(*(filePath + "/hidapi.dll"));
	FPlatformProcess::PopDllDirectory(*filePath);
	if (!DLLHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load StepVRSDK.dll!"));
	}
	else
	{
		DLLHandles.Add(DLLHandle);
	}


	//add key binding
	EKeys::AddKey(FKeyDetails(ButtonState.MyKey.StepVR_GunBtn_A_Trigger, LOCTEXT("StepVR_GunBtn_A_Trigger", "StepVR_GunBtn_A_Trigger"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(ButtonState.MyKey.StepVR_GunBtn_B_Trigger, LOCTEXT("StepVR_GunBtn_B_Trigger", "StepVR_GunBtn_B_Trigger"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(ButtonState.MyKey.StepVR_GunBtn_C_Trigger, LOCTEXT("StepVR_GunBtn_C_Trigger", "StepVR_GunBtn_C_Trigger"), FKeyDetails::GamepadKey));

	EKeys::AddKey(FKeyDetails(ButtonState.MyKey.StepVR_LeftBtn_A_Trigger, LOCTEXT("StepVR_LeftBtn_A_Trigger", "StepVR_LeftBtn_A_Trigger"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(ButtonState.MyKey.StepVR_LeftBtn_B_Trigger, LOCTEXT("StepVR_LeftBtn_B_Trigger", "StepVR_LeftBtn_B_Trigger"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(ButtonState.MyKey.StepVR_LeftBtn_C_Trigger, LOCTEXT("StepVR_LeftBtn_C_Trigger", "StepVR_LeftBtn_C_Trigger"), FKeyDetails::GamepadKey));

	EKeys::AddKey(FKeyDetails(ButtonState.MyKey.StepVR_RightBtn_A_Trigger, LOCTEXT("StepVR_RightBtn_A_Trigger", "StepVR_RightBtn_A_Trigger"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(ButtonState.MyKey.StepVR_RightBtn_B_Trigger, LOCTEXT("StepVR_RightBtn_B_Trigger", "StepVR_RightBtn_B_Trigger"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(ButtonState.MyKey.StepVR_RightBtn_C_Trigger, LOCTEXT("StepVR_RightBtn_C_Trigger", "StepVR_RightBtn_C_Trigger"), FKeyDetails::GamepadKey));
}

#undef LOCTEXT_NAMESPACE

