// Fill out your copyright notice in the Description page of Project Settings.
#include "StepVrPluginPrivatePCH.h"
#include "StepVrServer.h"

#define SENDSIZE		64
#define RECEIVESIZE		64

StepVrServer* StepVrServer::S_StepVrServer = nullptr;
StepVrServer::StepVrServer():
	SendScoket(nullptr),
	ReceiveSocket(nullptr),
	ListenScoket(nullptr)
{

}

StepVrServer::~StepVrServer()
{
	CloseSocket();
}

StepVrServer* StepVrServer::Get()
{
	if (S_StepVrServer==nullptr)
	{
		S_StepVrServer = new StepVrServer();
		S_StepVrServer->InitSendSocket();
		S_StepVrServer->InitListenSocket();
	}


	return S_StepVrServer;
}

void StepVrServer::Destory()
{
	
	if (S_StepVrServer)
	{
		S_StepVrServer->CloseSocket();

		delete S_StepVrServer;
		S_StepVrServer = nullptr;
	}
}
void StepVrServer::InitSendSocket()
{
	FString SockeName = TEXT("StepVrSocketSend");
	//Ping IP
	FString Address = TEXT("127.0.0.1");
	//Port
	int32 SendPort = 21701;

	//Remote IP And Port
	FIPv4Address ip;
	FIPv4Address::Parse(Address, ip);

	//Remote Address
	RemoteAdrr = ISocketSubsystem::Get()->CreateInternetAddr();
	RemoteAdrr->SetIp(ip.Value);
	RemoteAdrr->SetPort(SendPort);

	//creat send socket
	SendScoket = FUdpSocketBuilder(SockeName)
		.AsReusable()
		.WithBroadcast()
		.WithSendBufferSize(SENDSIZE)
		.Build();
}

void StepVrServer::InitListenSocket()
{

	FString SocketName = TEXT("StepVrSocketListen");
	FString ReceiveName = TEXT("StepVrSocketReceive");
	//Ping IP
	FString Address = TEXT("127.0.0.1");
	//Port
	int32 ReceivePort = 21703;

	//Remote IP And Port
	FIPv4Address ip;
	FIPv4Address::Parse(Address, ip);
	FIPv4Endpoint Endpoint(ip, ReceivePort);

	ListenScoket = FUdpSocketBuilder(SocketName)
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.WithReceiveBufferSize(RECEIVESIZE)
		.Build();
	ListenScoket->Listen(10);

	/**
	* Receive Remote Data  
	*/
	ReceiveSocket = new FUdpSocketReceiver(ListenScoket, FTimespan::FromMilliseconds(100), *ReceiveName);
	ReceiveSocket->OnDataReceived().BindLambda([this](const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt)
	{
		char ReceiveData[RECEIVESIZE+1];
		//Get Receive Data
		memcpy(ReceiveData, ArrayReaderPtr->GetData(), ArrayReaderPtr->Num());

		//Cut Message
		ReceiveData[ArrayReaderPtr->Num()] = 0;

		//Convert to FString
		FString debugData = ANSI_TO_TCHAR(ReceiveData);

		SendData(debugData);
	});

	ReceiveSocket->Start();
}

void StepVrServer::SendMessage(const FString Message)
{
	FString serialized = TEXT("Command:Ready");
	TCHAR *serializedChar = serialized.GetCharArray().GetData();
	int32 size = FCString::Strlen(serializedChar);
	int32 sent = 0;

	SendScoket->SendTo((uint8*)TCHAR_TO_UTF8(serializedChar), size, sent, *RemoteAdrr);
}

void StepVrServer::CloseSocket()
{
	if (SendScoket)
	{
		SendScoket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SendScoket);
	}
	if (ListenScoket)
	{
		ListenScoket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenScoket);
	}
	if (ReceiveSocket)
	{
		delete ReceiveSocket;
		ReceiveSocket = nullptr;
	}
}

void StepVrServer::SendData(const FString & Data)
{
	if (Delegate->IsValidLowLevel()&& Delegate->GetClass()->ImplementsInterface(UStepVrServerInterface::StaticClass()))
	{
		if (Data.Compare(TEXT("Command:Roger"))==0)
		{
			IStepVrServerInterface::Execute_CMDArriveCopy(Delegate,Data);
		}
		if (Data.Compare(TEXT("Command:Reset_HMD")) == 0)
		{
			IStepVrServerInterface::Execute_CMDResetHMD(Delegate,Data);
		}

		IStepVrServerInterface::Execute_ReceiveMessage(Delegate, Data);
	}
}
