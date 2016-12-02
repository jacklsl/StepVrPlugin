// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "Engine.h"
#include "StepVrPlugin.h"

// You should place include statements to your module's private header files here.  You only need to
// add includes for headers that are used in most of your module's source files though.
#include "InputDevice.h"
#include "IHapticDevice.h"
#include "IInputInterface.h"
#include "IMotionController.h"
#include "Features/IModularFeatures.h"

#include "LocalDefine.h"
#include "StepVr.h"


/**
*   StepVr Server
*/
#include "StepVrServerInterface.h"
#include "SocketSubsystem.h"
#include "StepVrServer.h"

/**
* Get equipment transform with node id  
*/
extern bool GetStepVrNodeTransform(FTransform& Transform,int32 EquipId);

/**
* MotionCapture
* Get MCap NodeTransform 
*/
extern bool GetStepVrMCapNodeTransform(FRotator& Rotator, int32 EquipId);

/**
* Local Manager controll  
*/
extern StepVR::Manager* StepManager;

/**
* check StepVrManger isVaild  
*/
extern bool StepManagerIsEnable();