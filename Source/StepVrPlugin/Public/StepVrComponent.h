// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Components/ActorComponent.h"
#include "StepVrComponent.generated.h"


#define StepvrLibrary
#define StepvrClassGroup


/**
*   EquipID
*	FHeadForOculus To deal with OculusHMD Offset,Not original Data
*	FHead The original Data
*/
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


/**
*   MotionCapture Data
*	The upper part of the joint
*/
USTRUCT(BlueprintType)
struct FStepVRMCapNode
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FVector HipLoc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FRotator HipRot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FRotator Head;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FRotator Neck;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FRotator LHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FRotator LUpperarm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FRotator LLowerarm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FRotator RHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FRotator RUpperarm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FRotator RLowerarm;
};


UCLASS(meta = (BlueprintSpawnableComponent), ClassGroup = StepvrClassGroup)
class STEPVRPLUGIN_API UStepVrComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UStepVrComponent();

	/**
	* Reset OculusHMD Offset  
	* When Auto Reset Something Wrong,You Can Run Function Reset Again
	*/
	UFUNCTION(BlueprintCallable,Category = StepvrLibrary)
	void ResetHMDForStepVr();

	/**
	* Get NodeTransform ,The Data Is Initial
	* equipId : NodeID
	* ts:NodeTransform
	*/
	UFUNCTION(BlueprintCallable, Category = StepvrLibrary)
	bool GetNodeTransForm(FTransform& ts, int32 equipId) const;

	/**
	* Get MotionCapture Data
	* If you want get MotionCaptureData,You Have Run CalibrateMocap First.
	* Then By Property MotionCaptuerState Get Data
	*/
	UFUNCTION(BlueprintCallable, Category = StepvrLibrary)
	bool CalibrateMocap();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	bool IsEnable();

public:
	/**
	* It is not work because the function just for test
	* Developer Don't need this
	*/
	UFUNCTION(BlueprintCallable, Category = StepvrLibrary)
		void SetHeadOffset(float x, float y, float z);
	//headoffset_x = 0.114;
	//headoffset_y = 0;
	//headoffset_z = -0.12;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FStepVRNode CurrentNodeState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StepvrLibrary)
	FStepVRMCapNode MotionCaptuerState;

private:
	void UpdateMotionCapture();

	void ResetControllPawnRotation();

	void ResetOculusRif();

	void GetHipLocation(FVector& Vec);

	bool IsReset;
	bool IsMCap;
	static bool IsResetOculus;
};