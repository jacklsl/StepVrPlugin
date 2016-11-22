// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#include "StepVrPluginPrivatePCH.h"
#include "../../../ThirdParty/include/StepVr.h"
#include "UnrealMathUtility.h"
#include "StepVrInput.h"


#define LOCTEXT_NAMESPACE "StepVR"

StepVR::Manager* StepManager = nullptr;


bool StepManagerIsEnable()
{
	return StepManager ? true : false;
}

void GetStepVrNodeTransform(FTransform& Transform,int32 EquipId)
{
	if (!StepManagerIsEnable())
	{
		return;
	}

	StepVR::Frame tmp = StepManager->GetFrame();

	//通过设备id获取对应的定位信息
	StepVR::Vector4f vec4 = tmp.GetSingleNode().GetQuaternion(SDKNODEID(EquipId));
	vec4 = StepVR::StepVR_EnginAdaptor::toUserQuat(vec4);
	Transform.SetRotation(FQuat(vec4.x, vec4.y, vec4.z, vec4.w));

	//获取位置
	StepVR::Vector3f vec3 = tmp.GetSingleNode().GetPosition(SDKNODEID(EquipId));
	vec3 = StepVR::StepVR_EnginAdaptor::toUserPosition(vec3);
	Transform.SetLocation(FVector(vec3.x * 100, vec3.y * 100, vec3.z * 100));
}
FStepVrInput::FStepVrInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
	:MessageHandler(InMessageHandler)
{
	//binding key
	StartModule();

 	
	FString StepVRMessage[] = {
		"StepvrSDK Open Success!",
		"StepvrSDK Load Matrix Fail!",
		"StepvrSDK Open Port Fail!",
		"StepvrSDK Start Thread Fail!",
		"StepvrSDK Creat Faild!" };

	StepManager = new StepVR::Manager();

	if (!StepManager)
	{
		SHOWLOG(StepVRMessage[4]);
		return;
	}

	//在对应版本引擎中需要矩阵文件，木有会崩溃
	//0 is ok, 
	//1 is load matrix fail, 
	//2 is open port fail, 
	//3 is start thread fail.
	
	int32 StartID = StepManager->Start();
	SHOWLOG(StepVRMessage[StartID]);

 	if (0!= StartID)
 	{
		delete StepManager;
		StepManager = nullptr;
		return;
 	}


	StepVR::StepVR_EnginAdaptor::MapCoordinate(StepVR::Vector3f(0, 0, 1), StepVR::Vector3f(-1, 0, 0), StepVR::Vector3f(0, 1, 0));
	StepVR::StepVR_EnginAdaptor::setEulerOrder(StepVR::EulerOrder_ZYX);

	dBtnRepeatTime = 0.15f;
	IModularFeatures::Get().RegisterModularFeature(GetModularFeatureName(), this);

#if (BEFOPRE_ENGINEVERSION_412)
	GEngine->MotionControllerDevices.AddUnique(this);
#endif	
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
	if ((int32)DeviceHand<0 || 
		((int32)DeviceHand) >= (int32)EStepVrDeviceId::DTotalCount)
	{
		return false;
	}

	FTransform Transform;
	GetStepVrNodeTransform(Transform, ButtonState.Devices[(int32)DeviceHand].EquipId);
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

