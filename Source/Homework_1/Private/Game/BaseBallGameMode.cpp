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
	NewChatMessage.Sender = TEXT("Server");
	NewChatMessage.Message = FString::Printf(TEXT("Player %d has joined"), Uid);
	SendChatMessage(NewChatMessage);

	//플레이어 기본 찬스 설정
	PlayerChance.Add({ Uid, 0 });
	PlayerUids.Push(Uid);

	if (TurnUid < 1) {
		TurnUid = Uid;
	}
}

void ABaseBallGameMode::Logout(AController* LeavingPlayer)
{
	Super::Logout(LeavingPlayer);

	ABaseBallPlayerController* BBPC = Cast<ABaseBallPlayerController>(LeavingPlayer);
	if (!BBPC) return;

	AllPlayerControllers.Remove(BBPC);

	ABaseBallPlayerState* BBPS = BBPC->GetPlayerState<ABaseBallPlayerState>();
	if (!BBPS) return;

	if (TurnUid == BBPS->Uid) {
		TurnChange(BBPS->Uid);
	}

	PlayerChance.Remove(BBPS->Uid);
	PlayerUids.Remove(BBPS->Uid);

	FChatMessage NewChatMessage;
	NewChatMessage.Time = FDateTime::Now();
	NewChatMessage.Sender = TEXT("Server");
	NewChatMessage.Message = FString::Printf(TEXT("Player %d has Logout"), BBPS->Uid);
	SendChatMessage(NewChatMessage);

	IsGameContinue();
}

void ABaseBallGameMode::BroadcastNewChat(ABaseBallPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	if (!InChattingPlayerController || InChatMessageString.IsEmpty()) return;
	ABaseBallPlayerState* BBPS = InChattingPlayerController->GetPlayerState<ABaseBallPlayerState>();

	if (!BBPS) return;

void ABaseBallGameMode::SendChatMessage(FChatMessage& NewChatMessage)
{
	for (ABaseBallPlayerController* BBPC : AllPlayerControllers) {
		BBPC->ClientRPCReciveMessage(NewChatMessage);
	}
}
int32 ABaseBallGameMode::GeneratedSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; i++) {
		Numbers.Push(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	int32 SecretNumberInit = 0;

	for (int32 I = 0; I < MaxSecretNumberLength; I++) {
		SecretNumberInit *= 10;
		int32 Index = FMath::RandRange(0, (Numbers.Num() - 1));
		int32 PickedNumber = Numbers[Index];
		SecretNumberSet.Add(PickedNumber);
		SecretNumberInit += PickedNumber;
		Numbers.RemoveAt(Index);
	}

	UE_LOG(LogTemp, Error, TEXT("SecretNumber : %d"), SecretNumberInit);

	return SecretNumberInit;
}

bool ABaseBallGameMode::IsGuessNumberString(const FString& InNumberString)
{
	if (InNumberString.Len() != MaxSecretNumberLength) return false;
	TSet<int32> UniqueNumberSet;

	for (TCHAR C : InNumberString) {
		if (!FChar::IsDigit(C) || C == '0') return false;
		
		if (UniqueNumberSet.Contains(C)) return false;
		UniqueNumberSet.Add(C);
	}

	return true;
}
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
