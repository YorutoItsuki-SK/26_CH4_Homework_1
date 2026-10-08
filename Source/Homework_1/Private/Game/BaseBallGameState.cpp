// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BaseBallGameState.h"

#include "Net/UnrealNetwork.h"

ABaseBallGameState::ABaseBallGameState()
{
	bReplicates = true;
}

void ABaseBallGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentChat);
}

void ABaseBallGameState::OnRep_CurrentChat()
{
	FDateTime publishedDateTime = CurrentChat.Time;
	FString publishedTime = publishedDateTime.ToString(TEXT("%y:%m:%d %H:%M"));
	FString CombinedMessage = TEXT("[") + publishedTime + TEXT("] ") + CurrentChat.Sender + TEXT(" : ") + CurrentChat.Message;
	UE_LOG(LogTemp, Warning, TEXT("%s"), *CombinedMessage);
	OnChatChanged.Broadcast(CurrentChat);
}
