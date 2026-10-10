// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Context/Chat.h"
#include "ChatLog.generated.h"

class UScrollBox;
class UVerticalBox;

/**
 * 
 */
UCLASS()
class HOMEWORK_1_API UChatLog : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddChatToLog(const FString& Chat);

protected:
	FDelegateHandle ChatHandle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ChatScrollBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> ChatLogBox;

	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void ReciveChat(FChatMessage InChat);
};
