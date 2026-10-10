// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Context/Chat.h"
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
	UPROPERTY()
	TSet<TObjectPtr<ABaseBallPlayerController>> AllPlayerControllers;
	
	TArray<int32> PlayerUids;

	TMap<int32, int32> PlayerChance;

	int32 MaxChance = 3;

	int32 MaxSecretNumberLength = 3;

	int32 TurnUid = 0;

	int32 Uid = 0;

	int32 SecretNumber;

	TSet<int32> SecretNumberSet;

protected:
	virtual void BeginPlay() override;

protected:

	void SendChatMessage(FChatMessage& NewChatMessage);

	void SendNoticeMessage(const FText& InNotice);

	int32 GeneratedSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	void ProcessGuessRequest(ABaseBallPlayerController* InChattingPlayerController, const int32& GuessNumber);

	FBaseBallResult GetBaseBallResult(const int32& GuessNumber);

	FString GetResultString(const FBaseBallResult& Result);

	void TurnChange(const int32& CurreuntUid);

	bool IsGameContinue();

	void ResetGame();
};
