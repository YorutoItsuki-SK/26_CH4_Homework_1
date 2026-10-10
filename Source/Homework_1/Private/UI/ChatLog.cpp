// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChatLog.h"

#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Blueprint/WidgetTree.h"

#include "Game/BaseBallGameState.h"
#include "Player/BaseBallPlayerController.h"
#include "Player/BaseBallPlayerState.h"

void UChatLog::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PCRaw = GetOwningPlayer();
	if (PCRaw) {
		ABaseBallPlayerController* BBPC = Cast<ABaseBallPlayerController>(PCRaw);
		if (BBPC) {
			ChatHandle = BBPC->OnChatRecived.AddUObject(this, &UChatLog::ReciveChat);
		}
	}

}

void UChatLog::NativeDestruct()
{
	Super::NativeDestruct();
	APlayerController* PCRaw = GetOwningPlayer();
	if (PCRaw) {
		ABaseBallPlayerController* BBPC = Cast<ABaseBallPlayerController>(PCRaw);
		if (BBPC) {
			BBPC->OnChatRecived.Remove(ChatHandle);
		}
	}
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
	NewChat->SetAutoWrapText(true);
	NewChat->SetWrapTextAt(0.f);
	NewChat->SetWrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping);

	ChatLogBox->AddChildToVerticalBox(NewChat);

	ChatScrollBox->ScrollToEnd();
}
