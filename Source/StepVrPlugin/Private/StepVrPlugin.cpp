// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#include "StepVrPluginPrivatePCH.h"
#include "StepVrInput.h"

#define LOCTEXT_NAMESPACE "FStepVrPluginModule"
TSharedPtr< class IInputDevice > FStepVrPluginModule::CreateInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler)
{
	return TSharedRef<class IInputDevice>(new FStepVrInput(InMessageHandler));
}
#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FStepVrPluginModule, StepVrPlugin)


