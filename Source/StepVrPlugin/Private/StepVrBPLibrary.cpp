// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "StepVrPluginPrivatePCH.h"
#include "StepVrBPLibrary.h"

UStepVrBPLibrary::UStepVrBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}
void UStepVrBPLibrary::SVConnectServer(UObject* Obj)
{
	StepVrServer::Start()->SetDelegate(Obj);
}

void UStepVrBPLibrary::SVDisconnectServer()
{
	StepVrServer::Destory();
}

void UStepVrBPLibrary::SVSendMessageToRemote(const FString Message)
{
	StepVrServer::Get()->SendMessage(Message);
}