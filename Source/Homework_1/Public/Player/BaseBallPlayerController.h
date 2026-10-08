// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseBallPlayerController.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class HOMEWORK_1_API ABaseBallPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable)
	void ServerRPCSendMessage(const FString& InChatMessageString);
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	TObjectPtr<UUserWidget> MainHud = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainHudClass = nullptr;
};
