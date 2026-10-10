// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Notice.h"

#include "Components/TextBlock.h"

#include "Player/BaseBallPlayerController.h"

void UNotice::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PCRaw = GetOwningPlayer();
	if (PCRaw) {
		ABaseBallPlayerController* BBPC = Cast<ABaseBallPlayerController>(PCRaw);
		if (BBPC) {
			NoticeHandle = BBPC->OnNoticeRecived.AddUObject(this, &UNotice::UpdateNotice);
		}
	}

	if (NoticeTextBlock) {
		NoticeTextBlock->SetAutoWrapText(true);
		NoticeTextBlock->SetWrapTextAt(0.f);
		NoticeTextBlock->SetWrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping);
		NoticeTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	};
}

void UNotice::NativeDestruct()
{
	Super::NativeDestruct();

	APlayerController* PCRaw = GetOwningPlayer();
	if (PCRaw) {
		ABaseBallPlayerController* BBPC = Cast<ABaseBallPlayerController>(PCRaw);
		if (BBPC) {
			BBPC->OnNoticeRecived.Remove(NoticeHandle);
		}
	}

	UWorld* World = GetWorld();
	if (World) {
		if (World->GetTimerManager().IsTimerActive(NoticeTimerHandle)) {
			World->GetTimerManager().ClearTimer(NoticeTimerHandle);
		}
	}
}

void UNotice::UpdateNotice(FText InNotice)
{
	UWorld* World = GetWorld();
	if (!World) return;

	if (World->GetTimerManager().IsTimerActive(NoticeTimerHandle)) {
		World->GetTimerManager().ClearTimer(NoticeTimerHandle);
	}
	NoticeTextBlock->SetText(InNotice);
	NoticeTextBlock->SetVisibility(ESlateVisibility::Visible);

	World->GetTimerManager().SetTimer(NoticeTimerHandle, this, &UNotice::SetNoticeCollapse, 5.f, false);
}

void UNotice::SetNoticeCollapse()
{
	NoticeTextBlock->SetVisibility(ESlateVisibility::Collapsed);
}
