// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainSessionGameMode.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API AMainSessionGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay();

public:
	AMainSessionGameMode();
};


