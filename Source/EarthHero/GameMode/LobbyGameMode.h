// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay();

	//미완성
	//플레이어 들어왔을 때 초기값 false 줘야함
	TMap<APlayerController*, bool> PlayerReadyState;

	int ReadyCount = 0;

public:
	ALobbyGameMode();
	void TogglePlayerReady(APlayerController* Player);
	void AddPlayerReadyState(APlayerController* Player);
	bool PressGameStartButton();


};
