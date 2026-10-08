// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BaseBallGameState.generated.h"

DECLARE_DELEGATE_OneParam(FOnChatChanged, FChatMessage)

USTRUCT(BlueprintType)
struct FChatMessage
{
	GENERATED_BODY()

	UPROPERTY()
	FDateTime Time;

	UPROPERTY()
	FString Sender;

	UPROPERTY()
	FString Message;
};

/**
 * 
 */
UCLASS()
class HOMEWORK_1_API ABaseBallGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentChat)
	FChatMessage CurrentChat;

public:
	FOnChatChanged OnChatChanged;

public:
	void OnRep_CurrentChat();
};
