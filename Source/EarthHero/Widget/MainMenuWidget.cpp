// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "Components/Button.h"
//#include "MultiplayerSessionsSubsystem.h"
#include "../EHGameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


void UMainMenuWidget::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	/*
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}
	
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
		MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
	}*/
}

bool UMainMenuWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (Play_Btn)
	{
		Play_Btn->OnClicked.AddDynamic(this, &ThisClass::Play_BtnClicked);
	}
	if (Join_Btn)
	{
		Join_Btn->OnClicked.AddDynamic(this, &ThisClass::Join_BtnClicked);
	}
	if (Exit_Btn)
	{
		Exit_Btn->OnClicked.AddDynamic(this, &ThisClass::Exit_BtnClicked);
	}

	return true;
}

void UMainMenuWidget::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Created session, entering game!"));
		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel(PathToLobby);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Failed to create session!"))
			);
		}
		Play_Btn->SetIsEnabled(true);
	}
}

void UMainMenuWidget::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if (MultiplayerSessionsSubsystem == nullptr)
	{
		return;
	}

	for (auto Result : SessionResults)
	{
		FString SettingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
		if (SettingsValue == MatchType)
		{
			//MultiplayerSessionsSubsystem->JoinSession(Result);
			return;
		}
	}
	if (!bWasSuccessful || SessionResults.Num() == 0)
	{
		Join_Btn->SetIsEnabled(true);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No Session Found!"));
	}
}

void UMainMenuWidget::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UMainMenuWidget::OnDestroySession(bool bWasSuccessful)
{
}

void UMainMenuWidget::OnStartSession(bool bWasSuccessful)
{
}

void UMainMenuWidget::Play_BtnClicked()
{
	//인원 0명 로비로 참가
	UE_LOG(LogTemp, Log, TEXT("Create Lobby Clicked"));
	UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(GetWorld()->GetGameInstance());
	if (EHGameInstance)
	{
		//if (!Check_IsPrivate->IsChecked())
		{
			EHGameInstance->IsCheckedPrivate = true; //이거 좀 바꾸고 싶고, 로비 컨트롤러에서 바로 서버에게 private 변경값 요청해야 함
			EHGameInstance->LeaveMainSession("CreateLobby");
		}
		/*
		else
		{
			EHGameInstance->IsCheckedPrivate = false;
			EHGameInstance->LeaveMainSession("CreateLobby");
		}*/
	}

	/* 이전 꺼
	Play_Btn->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->CreateSession(NumPublicConnections, MatchType);
	}*/
}

void UMainMenuWidget::Join_BtnClicked()
{
	//랜덤 public 로비 참가
	UE_LOG(LogTemp, Log, TEXT("Join Lobby Clicked"));
	UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(GetWorld()->GetGameInstance());
	if (EHGameInstance)
	{
		//로비 참가 전 세션 떠나기. (이름 변경 예정)
		EHGameInstance->LeaveMainSession("JoinLobby");
	}

	/* 이전 꺼
	Join_Btn->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->FindSessions(10000);
	}*/
}

void UMainMenuWidget::Exit_BtnClicked()
{
	// Get the current player controller
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Ensure the PlayerController is valid
	if (PlayerController)
	{
		// Call the QuitGame function from the UKismetSystemLibrary
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}

void UMainMenuWidget::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}

void UMainMenuWidget::NativeDestruct()
{
	MenuTearDown();

	Super::NativeDestruct();
}