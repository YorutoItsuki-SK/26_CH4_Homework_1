// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BaseBallPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class HOMEWORK_1_API ABaseBallPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABaseBallPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
public:
	UPROPERTY(Replicated)
	int32 Uid;
};
