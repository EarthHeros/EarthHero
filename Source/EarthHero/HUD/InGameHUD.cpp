// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUD.h"

#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/Stat/StatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ProgressBar.h"

void UInGameHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UWorld *World = GetWorld();
	
	if (World && World->GetFirstPlayerController() && World->GetFirstPlayerController()->GetPawn())
	{
		AEHCharacter *MyCharacter = Cast<AEHCharacter>(World->GetFirstPlayerController()->GetPawn());
		// FString Message = FString::Printf(TEXT("HealthPercent : %f"), MyCharacter->StatComponent->GetHealthPercent());
		// GEngine->AddOnScreenDebugMessage(-1, 1233223.f, FColor::Red, Message);
		HealthBar->SetPercent(MyCharacter->StatComponent->GetHealthPercent());
	}
}
