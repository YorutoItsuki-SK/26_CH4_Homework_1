// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChatLog.h"

#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Blueprint/WidgetTree.h"

#include "Game/BaseBallGameState.h"

void UChatLog::NativeConstruct()
{
	Super::NativeConstruct();
	ABaseBallGameState* BBGameState = GetWorld()->GetGameState<ABaseBallGameState>();
	if (!BBGameState) return;

	ChatHandle = BBGameState->OnChatChanged.AddUObject(this, &UChatLog::ReciveChat);
}

void UChatLog::NativeDestruct()
{
	Super::NativeDestruct();

	ABaseBallGameState* BBGameState = GetWorld()->GetGameState<ABaseBallGameState>();
	if (!BBGameState) return;

	BBGameState->OnChatChanged.Remove(ChatHandle);
}

void UChatLog::ReciveChat(FChatMessage InChat)
{
	FDateTime publishedDateTime = InChat.Time;
	FString publishedTime = publishedDateTime.ToString(TEXT("%y:%m:%d %H:%M"));
	FString CombinedMessage = TEXT("[") + publishedTime + TEXT("] ") + InChat.Sender + TEXT(" : ") + InChat.Message;
	AddChatToLog(CombinedMessage);
}

void UChatLog::AddChatToLog(const FString& Chat)
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *Chat);

	if (!ChatLogBox) return;
	UTextBlock* NewChat = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());

	if (!NewChat) return;
	NewChat->SetText(FText::FromString(Chat));

	ChatLogBox->AddChildToVerticalBox(NewChat);

	ChatScrollBox->ScrollToEnd();
}
