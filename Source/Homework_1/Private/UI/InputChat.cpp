// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InputChat.h"

#include "Components/EditableTextBox.h"

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
}
