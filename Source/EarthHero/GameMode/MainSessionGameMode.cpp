// Fill out your copyright notice in the Description page of Project Settings.

#include "MainSessionGameMode.h"
#include <EarthHero/PlayerController/MainSessionPlayerController.h>
#include <EarthHero/GameSession/MainSessionGameSession.h>
#include "UObject/ConstructorHelpers.h"

AMainSessionGameMode::AMainSessionGameMode()
{
	//캐릭터 설정

	PlayerControllerClass = AMainSessionPlayerController::StaticClass();
	GameSessionClass = AMainSessionGameSession::StaticClass();
}

void AMainSessionGameMode::BeginPlay()
{
	Super::BeginPlay();
}