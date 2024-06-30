// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include <EarthHero/PlayerController/LobbyPlayerController.h>
#include <EarthHero/GameSession/LobbyGameSession.h>
#include <EarthHero/GameState/LobbyGameState.h>

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

void ALobbyGameMode::AddPlayerReadyState(APlayerController* NewPlayer)
{
	ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();
	if (LobbyGameState)
	{
		int32 PlayerIndex = PlayerControllerArray.IndexOfByKey(NewPlayer);
		if (PlayerIndex != INDEX_NONE)
		{
			PlayerControllerArray.RemoveAt(PlayerIndex);

			LobbyGameState->PlayerReadyStateArray.RemoveAt(PlayerIndex);
		}
		PlayerControllerArray.Add(NewPlayer);
		LobbyGameState->PlayerReadyStateArray.Add(false);

		//LobbyGameState->Multicast_UpdatePlayerReadyState();
	}
}

void ALobbyGameMode::TogglePlayerReady(APlayerController* Player)
{
	ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();
	if (LobbyGameState)
	{
		if (LobbyGameState)
		{
			int32 PlayerIndex = PlayerControllerArray.IndexOfByKey(Player);
			if (PlayerIndex != INDEX_NONE)
			{
				if (LobbyGameState->PlayerReadyStateArray[PlayerIndex]) ReadyCount--;
				else ReadyCount++;

				LobbyGameState->PlayerReadyStateArray[PlayerIndex] = !LobbyGameState->PlayerReadyStateArray[PlayerIndex];
			}
			else UE_LOG(LogTemp, Error, TEXT("%s is not valid for player ready state"), Player);

			//LobbyGameState->Multicast_UpdatePlayerReadyState();
		}
	}
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