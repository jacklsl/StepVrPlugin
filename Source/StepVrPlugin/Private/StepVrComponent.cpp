// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
//
#include "StepVrPluginPrivatePCH.h"
#include "StepVrComponent.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"

bool UStepVrComponent::IsResetOculus = false;
UStepVrComponent::UStepVrComponent():
IsReset(false)
{
	
	PrimaryComponentTick.bCanEverTick = true;
	bWantsBeginPlay = true;
}


void UStepVrComponent::ResetHMDForStepVr()
{
	IsResetOculus = false;
	ResetControllPawnRotation();
}

void UStepVrComponent::ResetControllPawnRotation()
{
	APlayerController* APC = UGameplayStatics::GetPlayerController(GWorld, 0);
	APawn* AP = APC->GetPawn();
	
	if (UKismetSystemLibrary::IsValid(AP))
	{
		if (!AP->GetActorRotation().IsZero())
		{
			AP->SetActorRotation(FRotator::ZeroRotator);
		}

		ResetOculusRif();
	}

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

	GetNodeTransForm(CurrentNodeState.FLeftHand,StepVrInfo::DLeftController);
	GetNodeTransForm(CurrentNodeState.FRightHand, StepVrInfo::DRightController);
	GetNodeTransForm(CurrentNodeState.FHead, StepVrInfo::DHead);
	GetNodeTransForm(CurrentNodeState.FGun, StepVrInfo::DGun);


	//reset
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		//Oculus Has Offset When No Senser
		FRotator Temp1;
		FVector  Temp2;
		UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(Temp1, Temp2);
		CurrentNodeState.FHeadForOculus.SetLocation(CurrentNodeState.FHead.GetLocation() - Temp2);

		if (!IsReset)
		{
			ResetControllPawnRotation();
			IsReset = true;
		}
	}
}

void UStepVrComponent::GetNodeTransForm(FTransform& ts, int32 equipId) const
{
	GetStepVrNodeTransform(ts,equipId);
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

		UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition(NewYaw);
		IsResetOculus = true;
	}
}

