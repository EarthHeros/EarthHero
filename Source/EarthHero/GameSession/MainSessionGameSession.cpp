// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSessionGameSession.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"


void AMainSessionGameSession::BeginPlay()
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
            if (PortNumber.Equals("7777"))
            {
                UE_LOG(LogTemp, Log, TEXT("Main Session Port : %s"), *PortNumber);
                CreateSession(PortNumber);
            }
            else UE_LOG(LogTemp, Error, TEXT("Main Session Port is not 7777"));

        }
        else UE_LOG(LogTemp, Error, TEXT("Port number error"));
    }
}

void AMainSessionGameSession::CreateSession(FString PortNumber)
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

            SessionSettings->bAllowInvites = false; // 초대 X
            //SessionSettings->bUsesStats = false; // 스탯 사용 여부 
            //SessionSettings->bAntiCheatProtected = true; // 안티 치트 보호 여부

            // 세션 검색을 위한 사용자 지정 속성 추가
            SessionSettings->Set("GameName", FString("EH2"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
            SessionSettings->Set("PortNumber", PortNumber, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
            //SessionSettings->Set("LobbyType", FString("Public"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing); //음...

            UE_LOG(LogTemp, Log, TEXT("Creating session..."));

            // 세션 생성 시도
            if (!Session->CreateSession(0, SessionName, *SessionSettings))
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to create session!"));
                Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegateHandle);
                CreateSessionDelegateHandle.Reset();
            }
        }
    }
}

void AMainSessionGameSession::HandleCreateSessionCompleted(FName EOSSessionName, bool bWasSuccessful)
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
                UE_LOG(LogTemp, Log, TEXT("Session: %s Created!"), *EOSSessionName.ToString());
            }
            else UE_LOG(LogTemp, Warning, TEXT("Failed to create session!"));

            // Clear our handle and reset the delegate. 
            Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegateHandle);
            CreateSessionDelegateHandle.Reset();
        }
    }
}

//서버에서 로그인하는 것을 방지 (AGameModeBase::InitGame()에서 불림)
bool AMainSessionGameSession::ProcessAutoLogin()
{
    return true;
}

void AMainSessionGameSession::RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite)
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
void AMainSessionGameSession::HandleRegisterPlayerCompleted(FName EOSSessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccesful) //성공
            {
                UE_LOG(LogTemp, Log, TEXT("Player registered in Session!"));
                NumberOfPlayersInSession++; //플레이어 수 세어줌
            }
            else UE_LOG(LogTemp, Warning, TEXT("Failed to register player! (From Callback)"));

            // Clear and reset delegates
            Session->ClearOnRegisterPlayersCompleteDelegate_Handle(RegisterPlayerDelegateHandle);
            RegisterPlayerDelegateHandle.Reset();
        }
    }
}

//NotifyLogout에서 호출됨
void AMainSessionGameSession::UnregisterPlayer(const APlayerController* ExitingPlayer)
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

void AMainSessionGameSession::HandleUnregisterPlayerCompleted(FName EOSSessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccesful)
            {
                UE_LOG(LogTemp, Log, TEXT("Player unregistered in Session!"));
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
void AMainSessionGameSession::NotifyLogout(const APlayerController* ExitingPlayer)
{
    Super::NotifyLogout(ExitingPlayer); //UnregisterPlayer를 호출함

    if (IsRunningDedicatedServer())
    {
        NumberOfPlayersInSession--; //현재 플레이어 수 --
    }
}