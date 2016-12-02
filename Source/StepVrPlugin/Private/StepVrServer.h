// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Networking.h"
#include "UdpSocketBuilder.h"


class StepVrServer
{
public:
	StepVrServer();
	~StepVrServer();

	/**
	*   Get StepVrServer Singleton
	*	init send socket ,listen socket
	*/
	static StepVrServer* Get();

	/**
	*   Destory StepVrServer Singleton
	*	clear socket
	*/
	static void Destory();
	/**
	*   send message to server manager
	*/
	void SendMessage(const FString Message);

	//set delegate for class IStepVrServerInterface
	void SetDelegate(UObject* Obj) { Delegate = Obj; }
private:
	/**
	* Set Remote Address
	* creat send socket
	*/
	void InitSendSocket();

	/**
	* Set local Address
	* creat listen socket
	*/
	void InitListenSocket();

	void CloseSocket();

	//Send Date to implement
	void SendData(const FString& Data);
private:
	static StepVrServer* S_StepVrServer;

	FSocket* SendScoket;
	FSocket* ListenScoket;
	FUdpSocketReceiver* ReceiveSocket;
	TSharedPtr<FInternetAddr> RemoteAdrr;

	/**
	* Delegate receive date  
	*/
	UObject* Delegate;
};