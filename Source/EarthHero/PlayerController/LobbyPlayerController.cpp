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
			if (EHGameInstance->IsCheckedPrivate)
			{
				UE_LOG(LogTemp, Log, TEXT("Lobby mode request : Private"));

				//ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
				this->Server_ChangeAdvertiseState(false);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Lobby mode request : Public"));
			}
		}
	}
}

//클라이언트가 방장인지 검사 + 방장이면 advertise 상태 변경
void ALobbyPlayerController::Server_ChangeAdvertiseState_Implementation(bool bAdvertise)
{
	//로비게임세션에서 이미 플레이어 접속을 감지하고 bHost에 할당해주었음
	if (bHost)
	{
		this->Client_HostAssignment();

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
	UE_LOG(LogTemp, Log, TEXT("Host Assignmented!"));

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, FString::Printf(TEXT("You are host!!!!!!!!!!")));

	//방장 받음
}