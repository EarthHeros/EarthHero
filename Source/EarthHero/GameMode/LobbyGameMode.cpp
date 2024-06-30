// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include <EarthHero/PlayerController/LobbyPlayerController.h>
#include <EarthHero/GameSession/LobbyGameSession.h>

ALobbyGameMode::ALobbyGameMode()
{
	// 캐릭터 설정

	PlayerControllerClass = ALobbyPlayerController::StaticClass();
	GameSessionClass = ALobbyGameSession::StaticClass();
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	AGameModeBase* GameModeBase = GetWorld()->GetAuthGameMode<AGameModeBase>();
	if (IsValid(GameModeBase))
	{
		GameModeBase->bUseSeamlessTravel = true;
	}
}

void ALobbyGameMode::TogglePlayerReady(APlayerController* Player)
{
	if (PlayerReadyState.Contains(Player))
	{
		if (PlayerReadyState[Player]) ReadyCount--;
		else ReadyCount++;

		PlayerReadyState[Player] = !PlayerReadyState[Player];
		
		ALobbyGameSession* LobbyGameSession = Cast<ALobbyGameSession>(GameSession);
		if (LobbyGameSession)
		{
			if (ReadyCount == (LobbyGameSession->MaxNumberOfPlayersInSession - 1)) //방장 제외 모두 레디 시 //수정 필요
			{
				UE_LOG(LogTemp, Warning, TEXT("all player ready!!"));
				UE_LOG(LogTemp, Log, TEXT("Lobby Start!"));
				LobbyGameSession->StartSession();
			}
		}
	}
}

void ALobbyGameMode::AddPlayerReadyState(APlayerController* NewPlayer)
{
	if (PlayerReadyState.Contains(NewPlayer))
	{
		PlayerReadyState.Remove(NewPlayer);
	}
	PlayerReadyState.Add(NewPlayer, false);
}

bool ALobbyGameMode::PressGameStartButton()
{
	ALobbyGameSession* LobbyGameSession = Cast<ALobbyGameSession>(GameSession);
	if (LobbyGameSession)
	{
		if (ReadyCount == (LobbyGameSession->MaxNumberOfPlayersInSession - 1))
		{
			UE_LOG(LogTemp, Log, TEXT("Lobby Start!"));
			LobbyGameSession->StartSession();
			return true;
		}
	}
	return false;
}