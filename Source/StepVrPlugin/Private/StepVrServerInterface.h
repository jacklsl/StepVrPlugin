// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StepVrServerInterface.generated.h"


/**
*   If you control the game with  StepVr server management ,you must inherited interface in Blueprint
*   The class must be StepVrComponent's parentsNode
*/
UINTERFACE(MinimalAPI)
class UStepVrServerInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IStepVrServerInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	/**
	*	receive message Log 
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = StepvrLibrary)
	void ReceiveMessage(const FString& Message);

	/**
	*   server manager call resetHMD
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = StepvrLibrary)
	void CMDResetHMD(const FString& Message);

	/**
	*   server manager return command when arrive target point
	*	you need close game when receive this command
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = StepvrLibrary)
	void CMDArriveCopy(const FString& Message);
};