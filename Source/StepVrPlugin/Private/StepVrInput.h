// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "StepVrInputState.h"
#include "StepVrPluginPrivatePCH.h"



class FStepVrInput : public IInputDevice ,public IMotionController
{
public:
	FStepVrInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler);
	~FStepVrInput();

	//InputDevice delegate

	virtual void Tick(float DeltaTime);

	/** Poll for controller state and send events if needed */
	virtual void SendControllerEvents();

	/** Set which MessageHandler will get the events from SendControllerEvents. */
	virtual void SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler);

	/** Exec handler to allow console commands to be passed through for debugging */
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar);

	/**
	* IForceFeedbackSystem pass through functions
	*/
	virtual void SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value);
	virtual void SetChannelValues(int32 ControllerId, const FForceFeedbackValues &values);

	//ImotionController Delegate

	virtual bool GetControllerOrientationAndPosition(const int32 ControllerIndex, const EControllerHand DeviceHand, FRotator& OutOrientation, FVector& OutPosition) const;
	virtual ETrackingStatus GetControllerTrackingStatus(const int32 ControllerIndex, const EControllerHand DeviceHand) const;
private:
	void StartModule();
protected:
	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;

	FStepVrStateController	ButtonState;

	double	dBtnRepeatTime;

	TArray<void*>   DLLHandles;
};