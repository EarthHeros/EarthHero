// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "GameFramework/PlayerController.h"
#include <EarthHero/EHGameInstance.h>
#include <Kismet/GameplayStatics.h>


void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(GetWorld()->GetGameInstance());

	//일단 로비에 오면 공개여부 설정 요청을 보냄
	if (EHGameInstance)
	{
		if (EHGameInstance->IsCheckedPrivate)
		{
			UE_LOG(LogTemp, Log, TEXT("Lobby mode request : Private"));

			ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
			if (PC)
			{
				PC->Server_ChangeAdvertiseState(false);
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Lobby mode request : Public"));
		}
	}

}

void ALobbyPlayerController::Server_ChangeAdvertiseState_Implementation(bool bAdvertise)
{

	//게임세션에서 방장을 파악하고 그 요청만 수락해야함
}

void ALobbyPlayerController::Client_HostAssignment_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Host Assignmented!"));

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, FString::Printf(TEXT("You are host!!!!!!!!!!")));

	//방장 받음
}