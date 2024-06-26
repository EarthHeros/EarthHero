// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameSession.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include <EarthHero/PlayerController/LobbyPlayerController.h>
#include <Kismet/GameplayStatics.h>



void ALobbyGameSession::BeginPlay()
{
    Super::BeginPlay();

    //dedicated 서버일 때만 세션 생성
    if (IsRunningDedicatedServer() && !bSessionExists)
    {
        //포트 번호를 key값으로 사용함
        FString PortNumber;
        UWorld* World = GetWorld();
        if (World)
        {
            PortNumber = FString::FromInt(World->URL.Port);
        }
        if (!PortNumber.IsEmpty())
        {
            if (!PortNumber.Equals("7777"))
            {
                UE_LOG(LogTemp, Log, TEXT("Lobby port : %s"), *PortNumber);
                CreateSession(PortNumber);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Lobby is not 7777 port"));
            }
        }
        else UE_LOG(LogTemp, Error, TEXT("Port number error"));
    }
}

void ALobbyGameSession::CreateSession(FString PortNumber)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            CreateSessionDelegateHandle =
                Session->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(
                    this,
                    &ThisClass::HandleCreateSessionCompleted));

            TSharedRef<FOnlineSessionSettings> SessionSettings = MakeShared<FOnlineSessionSettings>();
            SessionSettings->bIsDedicated = true; // Dedicated 서버 설정
            SessionSettings->bIsLANMatch = false; // LAN 세션이 아님
            SessionSettings->NumPublicConnections = MaxNumberOfPlayersInSession; // 최대 접속 인원 설정
            SessionSettings->NumPrivateConnections = 0; // 비공개 접속 인원 설정
            SessionSettings->bShouldAdvertise = true; // 세션 광고
            SessionSettings->bUsesPresence = false; // Presence를 사용하지 않음
            SessionSettings->bUseLobbiesIfAvailable = false; // 로비 사용 안함

            SessionSettings->bAllowInvites = true; // 초대 허용
            SessionSettings->bAllowJoinInProgress = true; // 진행 중인 세션에 참가 안됨
            //SessionSettings->bUsesStats = false; // 스탯 사용 여부 
            //SessionSettings->bAntiCheatProtected = true; // 안티 치트 보호 여부

            // 세션 검색을 위한 사용자 지정 속성 추가
            SessionSettings->Set("GameName", FString("EH2"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
            SessionSettings->Set("PortNumber", PortNumber, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
            //SessionSettings->Set("LobbyType", FString("Public"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing); //음...

            UE_LOG(LogTemp, Log, TEXT("Creating lobby..."));

            // 세션 생성 시도
            if (!Session->CreateSession(0, SessionName, *SessionSettings))
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to create lobby!"));
                Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegateHandle);
                CreateSessionDelegateHandle.Reset();
            }
        }
    }
}

void ALobbyGameSession::HandleCreateSessionCompleted(FName EOSSessionName, bool bWasSuccessful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccessful)
            {
                bSessionExists = true;
                UE_LOG(LogTemp, Log, TEXT("Lobby: %s Created!"), *EOSSessionName.ToString());
            }
            else UE_LOG(LogTemp, Warning, TEXT("Failed to create lobby!"));

            // Clear our handle and reset the delegate. 
            Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegateHandle);
            CreateSessionDelegateHandle.Reset();
        }
    }
}

//서버에서 로그인하는 것을 방지 (AGameModeBase::InitGame()에서 불림)
bool ALobbyGameSession::ProcessAutoLogin()
{
    return true;
}

void ALobbyGameSession::RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite)
{
    Super::RegisterPlayer(NewPlayer, UniqueId, bWasFromInvite);

    if (IsRunningDedicatedServer())
    {
        IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
        if (Subsystem)
        {
            IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
            if (Session.IsValid())
            {
                RegisterPlayerDelegateHandle =
                    Session->AddOnRegisterPlayersCompleteDelegate_Handle(FOnRegisterPlayersCompleteDelegate::CreateUObject(
                        this,
                        &ThisClass::HandleRegisterPlayerCompleted));

                //세션에 플레이어 등록
                if (!Session->RegisterPlayer(SessionName, *UniqueId, false))
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to Register Player!"));
                    Session->ClearOnRegisterPlayersCompleteDelegate_Handle(RegisterPlayerDelegateHandle);
                    RegisterPlayerDelegateHandle.Reset();
                }
            }
        }
    }
}

//플레이어 등록 결과
void ALobbyGameSession::HandleRegisterPlayerCompleted(FName EOSSessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccesful) //성공
            {
                UE_LOG(LogTemp, Log, TEXT("Player registered in Lobby!"));
                NumberOfPlayersInSession++; //플레이어 수 세어줌
                UE_LOG(LogTemp, Error, TEXT("NumberOfPlayersInSession = %d"), NumberOfPlayersInSession); //테스트

                //첫플레이어 = 방장
                if (NumberOfPlayersInSession == 1)
                {
                    //첫 번째 플레이어가 방장
                    HostPlayerId = PlayerIds[0];
                    UE_LOG(LogTemp, Log, TEXT("Host Assigment..."), *HostPlayerId->ToString());

                    //클라이언트에게 방장 권한을 부여
                    APlayerController* HostPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
                    HostAssignment(HostPlayerController);
                }

                //플레이어가 꽉 찼으면 세션 시작
                if (NumberOfPlayersInSession == MaxNumberOfPlayersInSession)
                {
                    UE_LOG(LogTemp, Log, TEXT("Lobby Start!"));
                    StartSession();
                }
            }
            else UE_LOG(LogTemp, Warning, TEXT("Failed to register player! (From Callback)"));

            // Clear and reset delegates
            Session->ClearOnRegisterPlayersCompleteDelegate_Handle(RegisterPlayerDelegateHandle);
            RegisterPlayerDelegateHandle.Reset();
        }
    }
}

//클라이언트가 들어왔는데 첫번째 플레이어라면 host 부여 
void ALobbyGameSession::HostAssignment(APlayerController* HostPlayer)
{
    if (HostPlayer)
    {
        ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(HostPlayer);
        if (LobbyPlayerController)
        {
            UE_LOG(LogTemp, Log, TEXT("Host Assigment : %s"), *HostPlayerId->ToString());
            LobbyPlayerController->bHost = true;
        }
    }
}

//세션에 플레이어 전부 다 차면 호출됨
void ALobbyGameSession::StartSession()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            StartSessionDelegateHandle =
                Session->AddOnStartSessionCompleteDelegate_Handle(FOnStartSessionCompleteDelegate::CreateUObject(
                    this,
                    &ThisClass::HandleStartSessionCompleted));

            //세션 시작 시도
            if (!Session->StartSession(SessionName))
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to start lobby!"));
                Session->ClearOnStartSessionCompleteDelegate_Handle(StartSessionDelegateHandle);
                StartSessionDelegateHandle.Reset();
            }
        }
    }
}

void ALobbyGameSession::HandleStartSessionCompleted(FName EOSSessionName, bool bWasSuccessful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccessful)
            {
                UE_LOG(LogTemp, Log, TEXT("Lobby Started!"));

                FTimerHandle UnusedHandle;
                GetWorldTimerManager().SetTimer(UnusedHandle, this, &ALobbyGameSession::ChangeMap, 10.0f, false);
            }
            else UE_LOG(LogTemp, Warning, TEXT("Failed to start lobby!! (From Callback)"));

            Session->ClearOnStartSessionCompleteDelegate_Handle(StartSessionDelegateHandle);
            StartSessionDelegateHandle.Reset();
        }
    }
}

void ALobbyGameSession::ChangeMap()
{
    // 맵 전환 //테스트 //x
    this->GetWorld()->ServerTravel(InGameMap, true);
}

//NotifyLogout에서 호출됨
void ALobbyGameSession::UnregisterPlayer(const APlayerController* ExitingPlayer)
{
    Super::UnregisterPlayer(ExitingPlayer);

    if (IsRunningDedicatedServer())
    {
        IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
        if (Subsystem)
        {
            //IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
            IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
            if (Session.IsValid())
            {
                // If the player leaves ungracefully this could be null
                if (ExitingPlayer->PlayerState)
                {
                    UnregisterPlayerDelegateHandle =
                        Session->AddOnUnregisterPlayersCompleteDelegate_Handle(FOnUnregisterPlayersCompleteDelegate::CreateUObject(
                            this,
                            &ThisClass::HandleUnregisterPlayerCompleted));

                    // 세션에서 플레이어 제거
                    if (ExitingPlayer && !Session->UnregisterPlayer(SessionName, *ExitingPlayer->PlayerState->UniqueId))
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Failed to Unregister Player!"));
                        Session->ClearOnUnregisterPlayersCompleteDelegate_Handle(UnregisterPlayerDelegateHandle);
                        UnregisterPlayerDelegateHandle.Reset();
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to Unregister Player!"));
                    Session->ClearOnUnregisterPlayersCompleteDelegate_Handle(UnregisterPlayerDelegateHandle);
                    UnregisterPlayerDelegateHandle.Reset();
                }
            }
        }
    }
}

void ALobbyGameSession::HandleUnregisterPlayerCompleted(FName EOSSessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccesful)
            {
                UE_LOG(LogTemp, Log, TEXT("Player unregistered in Lobby!"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to unregister player! (From Callback)"));
            }
            Session->ClearOnUnregisterPlayersCompleteDelegate_Handle(UnregisterPlayerDelegateHandle);
            UnregisterPlayerDelegateHandle.Reset();
        }
    }
}

//플레이어가 서버 세션을 떠날 때 자동으로 호출됨
void ALobbyGameSession::NotifyLogout(const APlayerController* ExitingPlayer)
{
    Super::NotifyLogout(ExitingPlayer); //UnregisterPlayer를 호출함

    if (IsRunningDedicatedServer())
    {
        NumberOfPlayersInSession--; //현재 플레이어 수 --

        //세션에 아무도 없다면 세션 종료
        if (NumberOfPlayersInSession == 0)
        {
            EndSession();
        }
    }
}

void ALobbyGameSession::EndSession()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            EndSessionDelegateHandle =
                Session->AddOnEndSessionCompleteDelegate_Handle(FOnEndSessionCompleteDelegate::CreateUObject(
                    this,
                    &ThisClass::HandleEndSessionCompleted));

            //세션 종료 시도
            if (!Session->EndSession(SessionName))
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to end session!"));
                Session->ClearOnEndSessionCompleteDelegate_Handle(StartSessionDelegateHandle);
                EndSessionDelegateHandle.Reset();
            }
        }
    }
}

void ALobbyGameSession::HandleEndSessionCompleted(FName EOSSessionName, bool bWasSuccessful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccessful)
            {
                UE_LOG(LogTemp, Log, TEXT("Lobby ended!"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to end lobby! (From Callback)"));
            }

            Session->ClearOnEndSessionCompleteDelegate_Handle(EndSessionDelegateHandle);
            EndSessionDelegateHandle.Reset();
        }
    }
}

//제거 시 서버 및 클라이언트 둘 다 호출됨
void ALobbyGameSession::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    DestroySession();
}

// 모든 플레이어가 전용 서버를 떠날 때 호출됨
void ALobbyGameSession::DestroySession()
{
    // 세션을 파괴하여 백엔드에서 제거
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            DestroySessionDelegateHandle =
                Session->AddOnDestroySessionCompleteDelegate_Handle(FOnDestroySessionCompleteDelegate::CreateUObject(
                    this,
                    &ThisClass::HandleDestroySessionCompleted));

            //세션 제거 시도
            if (!Session->DestroySession(SessionName))
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to destroy lobby.")); // Log to the UE logs that we are trying to log in. 
                Session->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionDelegateHandle);
                DestroySessionDelegateHandle.Reset();
            }
        }
    }
}

void ALobbyGameSession::HandleDestroySessionCompleted(FName EOSSessionName, bool bWasSuccesful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccesful)
            {
                bSessionExists = false;
                UE_LOG(LogTemp, Log, TEXT("Destroyed lobby succesfully."));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to destroy lobby."));
            }

            Session->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionDelegateHandle);
            DestroySessionDelegateHandle.Reset();
        }
    }
}