// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BaseBallGameMode.generated.h"

USTRUCT(BlueprintType)
struct FBaseBallResult
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Strike = 0;

	UPROPERTY()
	int32 Ball = 0;
};

class ABaseBallPlayerController;
class ABaseBallGameState;

struct FChatMessage;

/**
 * 
 */
UCLASS()
class HOMEWORK_1_API ABaseBallGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* LeavingPlayer) override;

public:
	void BroadcastNewChat(ABaseBallPlayerController* InChattingPlayerController, const FString& InChatMessageString);

protected:
	TSet<TObjectPtr<ABaseBallPlayerController>> AllPlayerControllers;

	int32 Uid;

	int32 SecretNumber;

	UPROPERTY()
	TObjectPtr<ABaseBallGameState> BBGS;

protected:
	virtual void BeginPlay() override;

protected:
	void SendChatMessage(FChatMessage& NewChatMessage);

	int32 GeneratedSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);
};
