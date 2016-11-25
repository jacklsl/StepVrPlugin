// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "../Launch/Resources/Version.h"
#include "StepVrInputState.h"
#include "StepVrPluginPrivatePCH.h"




class FStepVrInput : public IInputDevice ,public IMotionController/* ,public IHapticDevice*/
{
public:
	FStepVrInput(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler);
	~FStepVrInput();

	//InputDevice delegate

	virtual void Tick(float DeltaTime) override;

	//IHapticDevice* GetHapticDevice() override { return (IHapticDevice*)this; }
	/**
	*   unfinished
	*/
	virtual bool IsGamepadAttached() const override;

	/** Poll for controller state and send events if needed */
	virtual void SendControllerEvents() override;

	/** Set which MessageHandler will get the events from SendControllerEvents. */
	virtual void SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override;

	/** Exec handler to allow console commands to be passed through for debugging */
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;

	/**
	* IForceFeedbackSystem pass through functions
	*/
	virtual void SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value) override;
	virtual void SetChannelValues(int32 ControllerId, const FForceFeedbackValues &values) override;


	/************************************************************************/
	//ImotionController Delegate
	/************************************************************************/
	virtual bool GetControllerOrientationAndPosition(const int32 ControllerIndex, const EControllerHand DeviceHand, FRotator& OutOrientation, FVector& OutPosition) const override;

	/**
	* unfinished  
	*/
#if(AFTER_ENGINEVERSION_410)
	virtual ETrackingStatus GetControllerTrackingStatus(const int32 ControllerIndex, const EControllerHand DeviceHand) const override
	{
		//SHOWLOG(FString("TrackingStatus"));
		return ETrackingStatus::NotTracked;
	}
#endif

	/**
	* IHapticDevice delegate  
	*/
	//virtual void SetHapticFeedbackValues(int32 ControllerId, int32 Hand, const FHapticFeedbackValues& Values) override;

	/**
	* Determines the valid range of frequencies this haptic device supports, to limit input ranges from UHapticFeedbackEffects
	*/
	//virtual void GetHapticFrequencyRange(float& MinFrequency, float& MaxFrequency) const override;

	/**
	* Returns the scaling factor to map the amplitude of UHapticFeedbackEvents from [0.0, 1.0] to the actual range handled by the device
	*/
	//virtual float GetHapticAmplitudeScale() const override;
private:
	void StartModule();
protected:
	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;

	FStepVrStateController	ButtonState;

	double	dBtnRepeatTime;

	TArray<void*>   DLLHandles;
};