// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InputChat.h"

#include "Components/EditableTextBox.h"

#include "Player/BaseBallPlayerController.h"

void UInputChat::NativeConstruct()
{
	Super::NativeConstruct();

	if (!EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatCommitted)) {
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatCommitted);
	}
}

void UInputChat::NativeDestruct()
{
	Super::NativeDestruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatCommitted)) {
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatCommitted);
	}
}

void UInputChat::OnChatCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (!(CommitMethod == ETextCommit::OnEnter)) return;

	APlayerController* OwningPC = GetOwningPlayer();
	if (!OwningPC) return;

	ABaseBallPlayerController* BBPC = Cast<ABaseBallPlayerController>(OwningPC);
	if (!BBPC) return;

	BBPC->ServerRPCSendMessage(Text.ToString());

	EditableTextBox_ChatInput->SetText(FText());
}
