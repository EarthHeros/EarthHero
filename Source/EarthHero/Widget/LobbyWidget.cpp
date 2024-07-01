// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include <EarthHero/PlayerController/LobbyPlayerController.h>

bool ULobbyWidget::Initialize()
{
	Super::Initialize();

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);
		if (LobbyPlayerController)
		{
			if (LobbyPlayerController->bHost)
			{
				ReadyButton_Tb->SetText(FText::FromString("Game Start"));
			}
			else
			{
				ReadyButton_Tb->SetText(FText::FromString("Game Ready"));
			}
		}
	}
	Ready_Btn->OnClicked.AddDynamic(this, &ULobbyWidget::ReadyClicked);

	PlayerTexts.Add(Player1_Txt);
	PlayerTexts.Add(Player2_Txt);
	PlayerTexts.Add(Player3_Txt);
	PlayerTexts.Add(Player4_Txt);

	return true;
}

void ULobbyWidget::ReadyClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Ready Button Clicked"));

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerController);

		if (LobbyPlayerController)
		{
			LobbyPlayerController->Server_ClientReady();
		}
	}
}
void ULobbyWidget::UpdatePlayerNameList(const TArray<FString>& PlayerNameList)
{
	int NumberOfPlayers = PlayerNameList.Num();

	UE_LOG(LogTemp, Log, TEXT("Widget : update player name list (%d players)"), NumberOfPlayers);

	for (int i = 0; i < NumberOfPlayers; i++)
	{
		UE_LOG(LogTemp, Log, TEXT("Player %d name : %s"), i, *PlayerNameList[i]);
		PlayerTexts[i]->SetText(FText::FromString(PlayerNameList[i]));
	}
}

void ULobbyWidget::UpdateReadyState(const TArray<bool>& PlayerReadyStateArray)
{
	int NumberOfPlayers = PlayerReadyStateArray.Num();

	UE_LOG(LogTemp, Log, TEXT("Widget : update player ready state (%d players)"), NumberOfPlayers);

	for(int i = 0; i < NumberOfPlayers; i++)
	{
		if (PlayerReadyStateArray[i])
		{
			UE_LOG(LogTemp, Log, TEXT("Player %d ready : true"), i);
		}
		else UE_LOG(LogTemp, Log, TEXT("Player %d ready : false"), i)

		if(PlayerReadyStateArray[i]) PlayerTexts[i]->SetColorAndOpacity(FLinearColor::Red);
		else PlayerTexts[i]->SetColorAndOpacity(FLinearColor::Black);
	}
}