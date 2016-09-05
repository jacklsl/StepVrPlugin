// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
//
#include "StepVrPluginPrivatePCH.h"
#include "StepVrComponent.h"


UStepVrComponent::UStepVrComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
	bWantsBeginPlay = true;
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
	if (!StepManager)
	{
		return;
	}

	/**  标准件标准信息  */
//  	GetNodeTransForm(CurrentNodeState.FLeftHand, 1);
//  	GetNodeTransForm(CurrentNodeState.FRightHand, 2);
//  	GetNodeTransForm(CurrentNodeState.FHead, 5);

//Han Guo ID
	GetNodeTransForm(CurrentNodeState.FLeftHand,1);
	GetNodeTransForm(CurrentNodeState.FRightHand,2);
	GetNodeTransForm(CurrentNodeState.FLeftAnkle,3);
	GetNodeTransForm(CurrentNodeState.FRightAnkle,4);
	GetNodeTransForm(CurrentNodeState.FHead,6);
	GetNodeTransForm(CurrentNodeState.FGun,5);
}

bool UStepVrComponent::GetNodeTransForm(FTransform& ts, uint32 equipId)
{
	if (!StepManager)
	{
		return false;
	}

	StepVR::Frame tmp = StepManager->GetFrame();

	//通过设备id获取对应的定位信息
	StepVR::Vector4f vec4 = tmp.GetSingleNode().GetQuaternion((StepVR::SingleNode::NodeID)equipId);

	FQuat CurQuat(vec4.x, vec4.y, vec4.z, vec4.w);
	if (CurQuat==FQuat::Identity)
	{
		ts.SetRotation(FQuat::Identity);
		ts.SetLocation(FVector::ZeroVector);
		return false;
	}

	vec4 = StepVR::StepVR_EnginAdaptor::toUserQuat(vec4);
	ts.SetRotation(FQuat(vec4.x,vec4.y,vec4.z,vec4.w));

	//获取位置
	StepVR::Vector3f vec3 = tmp.GetSingleNode().GetPosition((StepVR::SingleNode::NodeID)equipId);
	vec3 = StepVR::StepVR_EnginAdaptor::toUserPosition(vec3);
	ts.SetLocation(FVector(vec3.x*100,vec3.y*100,vec3.z*100));

	return true;
}

bool UStepVrComponent::IsEnable()
{
	return StepManager ? true : false;
}

