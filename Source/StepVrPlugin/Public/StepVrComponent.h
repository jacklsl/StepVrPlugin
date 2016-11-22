// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Components/ActorComponent.h"
#include "StepVrComponent.generated.h"


#define StepvrLibrary
#define StepvrClassGroup

USTRUCT(BlueprintType)
struct FStepVRNode
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FTransform FHeadForOculus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FTransform FHead;
	 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FTransform FGun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FTransform FLeftHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FTransform FRightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FTransform FLeftAnkle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FTransform FRightAnkle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FTransform FLeftUpperArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FTransform FRightUpperArm;
};

UCLASS(meta = (BlueprintSpawnableComponent), ClassGroup = StepvrClassGroup)
class STEPVRPLUGIN_API UStepVrComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UStepVrComponent();

	UFUNCTION(BlueprintCallable,Category = StepvrLibrary)
	void ResetHMDForStepVr();

	UFUNCTION(BlueprintCallable, Category = StepvrLibrary)
	void GetNodeTransForm(FTransform& ts, int32 equipId) const;

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	bool IsEnable();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FStepVRNode CurrentNodeState;

private:
	void ResetControllPawnRotation();
	void ResetOculusRif();
	bool IsReset;
	static bool IsResetOculus;
};