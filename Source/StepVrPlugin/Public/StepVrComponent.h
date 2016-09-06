// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Components/ActorComponent.h"
#include "StepVrComponent.generated.h"


#define StepVrNodeCateGory StepVrNode
#define StepVrClassGroup   Stepvrcomponent

USTRUCT(BlueprintType)
struct FStepVRNode
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepVrNodeCateGory)
	FTransform FHead;
	 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepVrNodeCateGory)
	FTransform FGun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepVrNodeCateGory)
	FTransform FLeftHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepVrNodeCateGory)
	FTransform FRightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepVrNodeCateGory)
	FTransform FLeftAnkle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepVrNodeCateGory)
	FTransform FRightAnkle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepVrNodeCateGory)
	FTransform FLeftUpperArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepVrNodeCateGory)
	FTransform FRightUpperArm;
};

UCLASS(meta = (BlueprintSpawnableComponent), ClassGroup = StepVrClassGroup)
class STEPVRPLUGIN_API UStepVrComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UStepVrComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	bool GetNodeTransForm(FTransform& ts,uint32 equipId);

	bool IsEnable();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = node)
	FStepVRNode CurrentNodeState;
};