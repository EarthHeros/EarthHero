// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "GameFramework/PlayerController.h"
#include <EarthHero/EHGameInstance.h>
#include <Kismet/GameplayStatics.h>
#include <EarthHero/GameSession/LobbyGameSession.h>
#include <EarthHero/GameMode/LobbyGameMode.h>


void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsRunningDedicatedServer())
	{
		UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(GetWorld()->GetGameInstance());

		//일단 로비에 오면 공개여부 설정 요청을 보냄
		if (EHGameInstance)
		{
			UE_LOG(LogTemp, Log, TEXT("Lobby mode request private? : %s"), EHGameInstance->IsCheckedPrivate);

			Server_ChangeAdvertiseState(EHGameInstance->IsCheckedPrivate);
		}
	}
}

//클라이언트가 방장인지 검사 + 방장이면 advertise 상태 변경
void ALobbyPlayerController::Server_ChangeAdvertiseState_Implementation(bool bAdvertise)
{
	//로비게임세션에서 이미 플레이어 접속을 감지하고 bHost에 할당해주었음
	if (bHost)
	{
		Client_HostAssignment();

		//광고 상태 변경
		ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
		if (LobbyGameMode)
		{
			ALobbyGameSession* LobbyGameSession = Cast<ALobbyGameSession>(LobbyGameMode->GameSession);
			if (LobbyGameSession)
			{
				UE_LOG(LogTemp, Log, TEXT("Change advertise state..."));

				LobbyGameSession->ChangeAdvertiseState(bAdvertise);
			}
		}

	}
}

//방장임을 클라이언트에게 알림
void ALobbyPlayerController::Client_HostAssignment_Implementation()
{
	bHost = true; //클라이언트에도 알리는 만큼, 서버에서 호스트 확인 항상하기

	UE_LOG(LogTemp, Log, TEXT("Host Assignmented!"));

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, FString::Printf(TEXT("You are host!!!!!!!!!!")));
}

void ALobbyPlayerController::Server_ClientReady_Implementation()
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode)
	{
		//방장은 게임 시작버튼으로 처리
		if (bHost)
		{
			if (LobbyGameMode->PressGameStartButton())
			{
				Client_SendToDebugMessage("Game Start!");
			}
			else
			{
				Client_SendToDebugMessage("All players should be ready!");
			}
		}
		//클라이언트는 게임 레디버튼으로 처리
		else
		{
			LobbyGameMode->TogglePlayerReady(this); //로비 플레이어 컨트롤러를 넘기지만 받는 곳은 플레이어 컨트롤러. 큰 문제 없으려나?
		}
	}
}


//서버에서 레디 상태 배열 받고 UI 갱신
void ALobbyPlayerController::Client_UpdateReadyState_Implementation(const TArray<bool>& PlayerReadyStateArray)
{
	//ui 에서 함수호출
}


//서버->클라 메시지 전송 (임시)
void ALobbyPlayerController::Client_SendToDebugMessage_Implementation(const FString& Message)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, Message);
}