// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BaseBallPlayerController.h"

#include "Blueprint/UserWidget.h"

#include "Game/BaseBallGameMode.h"

void ABaseBallPlayerController::ServerRPCSendMessage_Implementation(const FString& InChatMessageString)
{
	UWorld* World = GetWorld();
	if (!World) return;

	ABaseBallGameMode* BBGM = Cast<ABaseBallGameMode>(World->GetAuthGameMode());
	if (!BBGM) return;

	BBGM->BroadcastNewChat(this, InChatMessageString);
}

void ABaseBallPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController()) return;

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (MainHudClass) {
		MainHud = CreateWidget<UUserWidget>(this, MainHudClass);
		MainHud->AddToViewport();
	}
}
