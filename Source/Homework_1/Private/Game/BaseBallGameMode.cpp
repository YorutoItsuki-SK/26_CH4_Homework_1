// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BaseBallGameMode.h"

#include "Game/BaseBallGameState.h"
#include "Player/BaseBallPlayerController.h"
#include "Player/BaseBallPlayerState.h"

void ABaseBallGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ABaseBallPlayerController* BBPC = Cast<ABaseBallPlayerController>(NewPlayer);
	if (!BBPC) return;

	AllPlayerControllers.Add(BBPC);

	ABaseBallPlayerState* BBPS = NewPlayer->GetPlayerState<ABaseBallPlayerState>();
	if (!BBPS) return;

	Uid++;

	BBPS->Uid = Uid;

	FChatMessage NewChatMessage;
	NewChatMessage.Time = FDateTime::Now();
	NewChatMessage.Sender = "Server";
	NewChatMessage.Message = FString::Printf(TEXT("Player %d has joined"), Uid);
	SendChatMessage(NewChatMessage);
}

void ABaseBallGameMode::Logout(AController* LeavingPlayer)
{
	Super::Logout(LeavingPlayer);

	ABaseBallPlayerController* BBPC = Cast<ABaseBallPlayerController>(LeavingPlayer);
	if (!BBPC) return;

	AllPlayerControllers.Remove(BBPC);

	ABaseBallPlayerState* BBPS = BBPC->GetPlayerState<ABaseBallPlayerState>();
	if (!BBPS) return;

	FChatMessage NewChatMessage;
	NewChatMessage.Time = FDateTime::Now();
	NewChatMessage.Sender = "Server";
	NewChatMessage.Message = FString::Printf(TEXT("Player %d has Logout"), BBPS->Uid);
	SendChatMessage(NewChatMessage);
}

void ABaseBallGameMode::BroadcastNewChat(ABaseBallPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	if (!InChattingPlayerController || InChatMessageString.IsEmpty()) return;
	ABaseBallPlayerState* BBPS = InChattingPlayerController->GetPlayerState<ABaseBallPlayerState>();

	if (!BBPS) return;

	FChatMessage NewChatMessage;
	NewChatMessage.Time = FDateTime::Now();
	NewChatMessage.Sender = FString::Printf(TEXT("Player %d"), BBPS->Uid);
	NewChatMessage.Message = InChatMessageString;
	SendChatMessage(NewChatMessage);
}

void ABaseBallGameMode::BeginPlay()
{
	Super::BeginPlay();

	ABaseBallGameState* BBGameState = GetGameState<ABaseBallGameState>();
	if (BBGameState) {
		BBGS = BBGameState;
	}
}

void ABaseBallGameMode::SendChatMessage(FChatMessage& NewChatMessage)
{
	if (!BBGS) {
		UE_LOG(LogTemp, Error, TEXT("ABaseBallGameMode::SendChatMessage, GameState is Null"));
		return;
	}

	BBGS->CurrentChat = NewChatMessage;
}

int32 ABaseBallGameMode::GeneratedSecretNumber()
{
	return 0;
}

bool ABaseBallGameMode::IsGuessNumberString(const FString& InNumberString)
{
	return false;
}
