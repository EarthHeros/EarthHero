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
				ReadyButton_Tb->SetText(FText::FromString("Game Start"));
			else
				ReadyButton_Tb->SetText(FText::FromString("Game Ready"));
		}
	}
	Ready_Btn->OnClicked.AddDynamic(this, &ULobbyWidget::ReadyClicked);

	PlayerTexts.Add(Player1_Txt);
	PlayerTexts.Add(Player2_Txt);
	PlayerTexts.Add(Player3_Txt);
	PlayerTexts.Add(Player4_Txt);

	ClassBtns.Add(Warrior_Btn);
	ClassBtns.Add(Mechanic_Btn);
	ClassBtns.Add(Shooter_Btn);
	ClassBtns.Add(Archor_Btn);

	Warrior_Btn->OnClicked.AddDynamic(this, &ULobbyWidget::WarriorClicked);
	Mechanic_Btn->OnClicked.AddDynamic(this, &ULobbyWidget::MechanicClicked);
	Shooter_Btn->OnClicked.AddDynamic(this, &ULobbyWidget::ShooterClicked);
	Archor_Btn->OnClicked.AddDynamic(this, &ULobbyWidget::ArchorClicked);

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

void ULobbyWidget::WarriorClicked()
{
	ChangeSelectedButton(EClassType::Warrior);
}
void ULobbyWidget::MechanicClicked()
{
	ChangeSelectedButton(EClassType::Mechanic);
}
void ULobbyWidget::ShooterClicked()
{
	ChangeSelectedButton(EClassType::Shooter);
}
void ULobbyWidget::ArchorClicked()
{
	ChangeSelectedButton(EClassType::Archor);
}

void ULobbyWidget::ChangeSelectedButton(EClassType ClassType)
{
	for (int i = 0; i < (int)EClassType::NumberOfClass; i++)
	{
		ClassBtns[i]->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
	ClassBtns[(int)ClassType]->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));
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