// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BaseBallPlayerState.h"

#include "Net/UnrealNetwork.h"

ABaseBallPlayerState::ABaseBallPlayerState()
{
	bReplicates = true;
}

void ABaseBallPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Uid);
	DOREPLIFETIME(ThisClass, CurrentGuess);
	DOREPLIFETIME(ThisClass, MaxGuess);
}
