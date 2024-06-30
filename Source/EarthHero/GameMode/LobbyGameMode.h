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
	int ReadyCount = 0;
	TArray<APlayerController*> PlayerControllerArray;

public:
	ALobbyGameMode();
	void TogglePlayerReady(APlayerController* Player);
	void AddPlayerReadyState(APlayerController* Player);
	bool PressGameStartButton();


};
