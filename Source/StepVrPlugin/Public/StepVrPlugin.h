// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"
#include "IInputDeviceModule.h"


class FStepVrPluginModule : public IInputDeviceModule
{
public:
	static inline FStepVrPluginModule& Get()
	{
		return FModuleManager::LoadModuleChecked< FStepVrPluginModule >("StepVrPlugin");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("StepVrPlugin");
	}

	virtual TSharedPtr< class IInputDevice > CreateInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler);

};