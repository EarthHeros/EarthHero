// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include <EarthHero/GameSession/LobbyGameSession.h>
#include "GameFramework/PlayerState.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"

ALobbyGameMode::ALobbyGameMode()
{
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
	ALobbyPlayerController* LobbyNewPlayerController = Cast<ALobbyPlayerController>(NewPlayer);

	int32 PlayerIndex = LobbyPlayerControllerArray.IndexOfByKey(LobbyNewPlayerController);

	if (PlayerIndex != INDEX_NONE)
	{
		LobbyPlayerControllerArray.RemoveAt(PlayerIndex);
		PlayerNameArray.RemoveAt(PlayerIndex);
		PlayerReadyStateArray.RemoveAt(PlayerIndex);
	}

	FString NewPlayerName;

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
		if (Identity)
		{
			TSharedPtr<const FUniqueNetId> PlayerId = LobbyNewPlayerController->PlayerState->GetUniqueId().GetUniqueNetId();
			if (PlayerId)
			{
				NewPlayerName = Identity->GetPlayerNickname(0); //*PlayerId
				UE_LOG(LogTemp, Log, TEXT("Player Steam Nickname: %s %s"), *NewPlayerName, *(PlayerId->ToString()));
			}
		}
	}

	LobbyPlayerControllerArray.Add(LobbyNewPlayerController);
	PlayerNameArray.Add(NewPlayerName);
	PlayerReadyStateArray.Add(false);
}

void ALobbyGameMode::TogglePlayerReady(APlayerController* Player)
{
	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(Player);

	int32 PlayerIndex = LobbyPlayerControllerArray.IndexOfByKey(LobbyPlayerController);

	if (PlayerIndex != INDEX_NONE)
	{
		if (PlayerReadyStateArray[PlayerIndex]) ReadyCount--;
		else ReadyCount++;

		PlayerReadyStateArray[PlayerIndex] = !PlayerReadyStateArray[PlayerIndex];

		UpdatePlayerReadyState();
	}
	else UE_LOG(LogTemp, Error, TEXT("%s is not valid for player ready state"), LobbyPlayerController);
}

//모든 클라이언트에게 플레이어 이름 리스트 전송 및 UpdatePlayerReadyState() 호출
void ALobbyGameMode::UpdatePlayerNameyListAndReadyState()
{
	int32 NumberOfPlayers = LobbyPlayerControllerArray.Num();

	UE_LOG(LogTemp, Log, TEXT("Send player name list to client. (%d players)"), NumberOfPlayers);

	for (int i = 0; i < NumberOfPlayers; i++)
	{
		if (LobbyPlayerControllerArray[i])
		{
			LobbyPlayerControllerArray[i]->Client_UpdatePlayerNameList(PlayerNameArray);
		}
	}

	UpdatePlayerReadyState();
}

//모든 클라이언트에게 바뀐 레디 상태 배열 전송
void ALobbyGameMode::UpdatePlayerReadyState()
{
	int32 NumberOfPlayers = LobbyPlayerControllerArray.Num();
	//ALobbyPlayerController로 for문은 안되나봐...

	UE_LOG(LogTemp, Log, TEXT("Send Ready state to client. (%d players)"), NumberOfPlayers);

	for (int i = 0; i < NumberOfPlayers; i++)
	{
		if (LobbyPlayerControllerArray[i])
		{
			LobbyPlayerControllerArray[i]->Client_UpdateReadyState(PlayerReadyStateArray);
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