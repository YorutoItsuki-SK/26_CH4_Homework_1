// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BaseBallGameState.h"

void ABaseBallGameState::OnRep_CurrentChat()
{
	OnChatChanged.ExecuteIfBound(CurrentChat);
}
