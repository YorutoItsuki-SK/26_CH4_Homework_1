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

	if (!IsGuessNumberString(InChatMessageString)) {
		//일반 채팅 처리
		FChatMessage NewChatMessage;
		NewChatMessage.Time = FDateTime::Now();
		NewChatMessage.Sender = FString::Printf(TEXT("Player %d"), BBPS->Uid);
		NewChatMessage.Message = InChatMessageString;

		SendChatMessage(NewChatMessage);
		return;
	}

	ProcessGuessRequest(InChattingPlayerController, FCString::Atoi(*InChatMessageString));
}
void ABaseBallGameMode::SendChatMessage(FChatMessage& NewChatMessage)
{
	for (ABaseBallPlayerController* BBPC : AllPlayerControllers) {
		BBPC->ClientRPCReciveMessage(NewChatMessage);
	}
}

void ABaseBallGameMode::SendNoticeMessage(const FText& InNotice)
{
	for (ABaseBallPlayerController* BBPC : AllPlayerControllers) {
		BBPC->ClientRPCReciveNotice(InNotice);
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

FBaseBallResult ABaseBallGameMode::GetBaseBallResult(const int32& GuessNumber)
{
	FBaseBallResult Result = { 0,0 };

	UE_LOG(LogTemp, Error, TEXT("ABaseBallGameMode::GetBaseBallResult, SN : %d, GN : %d"), SecretNumber, GuessNumber);
	if (GuessNumber == SecretNumber) {
		Result.Strike = MaxSecretNumberLength;
		return Result;
	}
	UE_LOG(LogTemp, Error, TEXT("ABaseBallGameMode::GetBaseBallResult, false"));


	int32 GuessNumberRaw = GuessNumber;
	int32 SecretNumberRaw = SecretNumber;

	while (GuessNumberRaw > 0)
	{
		int32 GuessNumberE = GuessNumberRaw % 10;
		GuessNumberRaw /= 10;

		int32 SecretNumberE = SecretNumberRaw % 10;
		SecretNumberRaw /= 10;

		if (GuessNumberE == SecretNumberE) {
			Result.Strike++;
			continue;
		}

		if (SecretNumberSet.Contains(GuessNumberE)) {
			Result.Ball++;
			continue;
		}
	}

	return Result;
}

FString ABaseBallGameMode::GetResultString(const FBaseBallResult& Result)
{
	if (Result.Strike == 0 && Result.Ball == 0) {
		return TEXT("Out");
	}
	return FString::Printf(TEXT(" [ %d S %d B ]"), Result.Strike, Result.Ball);
}

void ABaseBallGameMode::TurnChange(const int32& CurreuntUid)
{
	FChatMessage TurnChangeMessage;
	TurnChangeMessage.Time = FDateTime::Now();
	TurnChangeMessage.Sender = TEXT("Server");
	int32 CurreuntUidIndex = PlayerUids.IndexOfByKey(CurreuntUid);
	if (CurreuntUidIndex < 0) {
		if (PlayerUids.IsEmpty()) {
			TurnUid = 0;
			TurnChangeMessage.Message = TEXT("턴 초기화");
		}
		else {
			TurnUid = PlayerUids[0];
			TurnChangeMessage.Message = FString::Printf(TEXT("턴 재설정, 현재 턴 : Player %d"), TurnUid);
		}
		SendChatMessage(TurnChangeMessage);
		return;
	}

	CurreuntUidIndex++;
	if (PlayerUids.IsValidIndex(CurreuntUidIndex)) {
		TurnUid = PlayerUids[CurreuntUidIndex];
	}
	else if (PlayerUids.IsEmpty()) {
		TurnUid = 0;
	}
	else {
		TurnUid = PlayerUids[0];
	}
	TurnChangeMessage.Message = FString::Printf(TEXT("현재 턴 : Player %d"), TurnUid);
	SendChatMessage(TurnChangeMessage);
}

bool ABaseBallGameMode::IsGameContinue()
{
	bool bIsReset = false;
	for (const auto& Itr : PlayerChance) {
		if (Itr.Value < MaxChance) {
			bIsReset = true;
			break;
		}
	}
	return bIsReset;
}

void ABaseBallGameMode::ResetGame()
{
	for (auto& Itr : PlayerChance) {
		Itr.Value = 0;
	}
	SecretNumberSet.Empty();
	SecretNumber = GeneratedSecretNumber();
}

