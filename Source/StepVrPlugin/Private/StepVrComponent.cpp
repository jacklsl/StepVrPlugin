// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
//
#include "StepVrPluginPrivatePCH.h"
#include "StepVrComponent.h"


bool UStepVrComponent::IsResetOculus = false;
UStepVrComponent::UStepVrComponent():
IsReset(false),
IsMCap(false)
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UStepVrComponent::ResetHMDForStepVr()
{
	IsResetOculus = false;
	ResetControllPawnRotation();
}

void UStepVrComponent::UpdateMotionCapture()
{
	if ((!IsMCap)||(!StepManagerIsEnable()))
	{
		return;
	}

	GetStepVrMCapNodeTransform(MotionCaptuerState.HipRot, StepVR::WholeBody::SkeletonID_Hip);
	GetStepVrMCapNodeTransform(MotionCaptuerState.Head, StepVR::WholeBody::SkeletonID_Head);
	GetStepVrMCapNodeTransform(MotionCaptuerState.Neck, StepVR::WholeBody::SkeletonID_Neck);
	GetStepVrMCapNodeTransform(MotionCaptuerState.LHand, StepVR::WholeBody::SkeletonID_LeftHand);
	GetStepVrMCapNodeTransform(MotionCaptuerState.LLowerarm, StepVR::WholeBody::SkeletonID_LeftForearm);
	GetStepVrMCapNodeTransform(MotionCaptuerState.LUpperarm, StepVR::WholeBody::SkeletonID_LeftArm);
	GetStepVrMCapNodeTransform(MotionCaptuerState.RHand, StepVR::WholeBody::SkeletonID_RightHand);
	GetStepVrMCapNodeTransform(MotionCaptuerState.RLowerarm, StepVR::WholeBody::SkeletonID_RightForearm);
	GetStepVrMCapNodeTransform(MotionCaptuerState.RUpperarm, StepVR::WholeBody::SkeletonID_RightArm);
	GetHipLocation(MotionCaptuerState.HipLoc);
	
	MotionCaptuerState.Head = (MotionCaptuerState.Head.Quaternion()*MotionCaptuerState.Neck.Quaternion()).Rotator();
}

bool UStepVrComponent::ResetControllPawnRotation()
{
	bool Flag = false;
	APlayerController* APC = UGameplayStatics::GetPlayerController(GWorld, 0);
	APawn* AP = APC->GetPawn();
	
	if (IsValid(AP))
	{
		if (!AP->GetActorRotation().IsZero())
		{
			AP->SetActorRotation(FRotator::ZeroRotator);
		}

		ResetOculusRif();

		Flag = true;
	}


	return Flag;
}

void UStepVrComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UStepVrComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
}

void UStepVrComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!StepManagerIsEnable())
	{
		return;
	}

	UpdateMotionCapture();
	GetNodeTransForm(CurrentNodeState.FLeftHand,StepVrInfo::DLeftController);
	GetNodeTransForm(CurrentNodeState.FRightHand, StepVrInfo::DRightController);
	GetNodeTransForm(CurrentNodeState.FHead, StepVrInfo::DHead);
	GetNodeTransForm(CurrentNodeState.FGun, StepVrInfo::DGun);

	//reset
	if (GEngine->HMDDevice.IsValid())
	{
		//Oculus Has Offset When No Senser
		FQuat Temp1;
		FVector  Temp2;
		GEngine->HMDDevice->GetCurrentOrientationAndPosition(Temp1, Temp2);

		CurrentNodeState.FHeadForOculus.SetLocation(CurrentNodeState.FHead.GetLocation() - Temp2);
		CurrentNodeState.FHeadForOculus.SetRotation(CurrentNodeState.FHead.GetRotation());

		if (!IsReset)
		{
			IsReset = ResetControllPawnRotation();
		}

	}

}

bool UStepVrComponent::GetNodeTransForm(FTransform& ts, int32 equipId) const
{
	return GetStepVrNodeTransform(ts,equipId);
}

bool UStepVrComponent::CalibrateMocap()
{
	if (!StepManagerIsEnable())
	{
		return false;
	}

	StepManager->CalibrateMocap();
	IsMCap = true;
	return IsMCap;
}

void UStepVrComponent::SetHeadOffset(float x, float y, float z)
{
	StepManager->SetHeadOffset(x, y, z);
}

bool UStepVrComponent::IsEnable()
{
	return StepManagerIsEnable();
}
void UStepVrComponent::ResetOculusRif()
{
#if WITH_EDITOR
	IsResetOculus = false;
#endif
	if (!IsResetOculus)
	{
		float Yaw = CurrentNodeState.FHead.Rotator().Yaw;
		float NewYaw = Yaw + 90.0;
		NewYaw = NewYaw > 180 ? NewYaw - 360 : NewYaw;

		GEngine->HMDDevice->ResetOrientationAndPosition(NewYaw);
		IsResetOculus = true;
	}
}

void UStepVrComponent::GetHipLocation(FVector& Vec)
{
	StepVR::Vector3f vec3 = StepManager->GetFrame().GetWholeBody().GetHipPostion();
	vec3 = StepVR::StepVR_EnginAdaptor::toUserPosition(vec3);
	Vec.Set(vec3.x * 100, vec3.y * 100, vec3.z * 100);
}

