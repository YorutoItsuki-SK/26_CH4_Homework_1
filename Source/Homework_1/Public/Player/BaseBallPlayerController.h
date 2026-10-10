// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Context/Chat.h"
#include "BaseBallPlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChatRecived, FChatMessage);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNoticeRecived, FText);

class UUserWidget;

/**
 * 
 */
UCLASS()
class HOMEWORK_1_API ABaseBallPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	FOnChatRecived OnChatRecived;
	FOnNoticeRecived OnNoticeRecived;

public:
	UFUNCTION(Server, Reliable)
	void ServerRPCSendMessage(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPCReciveMessage(const FChatMessage& InMessage);

	UFUNCTION(Client, Reliable)
	void ClientRPCReciveNotice(const FText& InNotice);
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	TObjectPtr<UUserWidget> MainHud = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainHudClass = nullptr;
};
