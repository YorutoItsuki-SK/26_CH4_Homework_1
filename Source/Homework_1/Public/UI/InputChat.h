// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputChat.generated.h"

class UEditableTextBox;

/**
 * 
 */
UCLASS()
class HOMEWORK_1_API UInputChat : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;

protected:
	UFUNCTION()
	void OnChatCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
