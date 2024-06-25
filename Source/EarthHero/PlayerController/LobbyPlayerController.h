// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay();

protected:
	UFUNCTION(Server, Reliable)
	void Server_ChangeAdvertiseState(bool bAdvertise);

public:
	bool bHost = false;

	UFUNCTION(Client, Reliable)
	void Client_HostAssignment();
	
};
